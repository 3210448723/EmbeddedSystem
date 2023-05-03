/*
*********************************************************************************************************
uC/OS-II实时控制内核
主要的包含文件
内核管理文件

文 件: OS_CORE.C 内核结构管理文件
作 者: Jean J. Labrosse
*********************************************************************************************************
*/

#ifndef  OS_MASTER_FILE	 //如果没有定义OS_MASTER_FILE主文件，则
#define  OS_GLOBALS		 //定义全程变量 OS_GLOBALS
#include "includes.h"
#endif

//*******************************************************************************************************
//                             MAPPING TABLE TO MAP BIT POSITION TO BIT MASK
//注意: 变址索引表是对应OSRbyTbl[i]的位值(0~7)，给定值符合OSMapTbl[]的数据(二进制)

//OSMapTbl[]：就绪任务表；对应OSRdy Grp和OSRbyTbl[i]的位值(0~7)
INT8U  const  OSMapTbl[]   = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

//*******************************************************************************************************
//最高优先级任务查找表(PRIORITY RESOLUTION TABLE)
//注意: 变址索引表是位模式，找出就绪态最高优先级别任务，给定值应符合高优先级位位值(0~7)

//OSUnMapTbl[]：最高优先级任务查找表；对应OSRdy Grp和OSRbyTbl[i]的位值(0~7)
INT8U  const  OSUnMapTbl[] = {
    0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x00 to 0x0F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x10 to 0x1F                             */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x20 to 0x2F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x30 to 0x3F                             */
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x40 to 0x4F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x50 to 0x5F                             */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x60 to 0x6F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x70 to 0x7F                             */
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x80 to 0x8F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x90 to 0x9F                             */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xA0 to 0xAF                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xB0 to 0xBF                             */
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xC0 to 0xCF                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xD0 to 0xDF                             */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xE0 to 0xEF                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0        /* 0xF0 to 0xFF                             */
};

/*
*********************************************************************************************************
*                                       FUNCTION PROTOTYPES
*********************************************************************************************************
*/
static  void  OS_InitEventList(void);
static  void  OS_InitMisc(void);
static  void  OS_InitRdyList(void);
static  void  OS_InitTaskIdle(void);
static  void  OS_InitTaskStat(void);
static  void  OS_InitTCBList(void);

//初始化:(INITIALIZATION)
//描述: 初始化uC/OS-II。对这个函数的调用必须在调用OSStart()函数之前。
//OSStart()函数才是真正开始运行多任务
void  OSInit (void)
{
#if OS_VERSION >= 204
    OSInitHookBegin();                                           /* Call port specific initialization code   */
#endif

    OS_InitMisc();                                               /* Initialize miscellaneous variables       */

    OS_InitRdyList();                                            /* Initialize the Ready List                */
    OS_InitTCBList();                                            /* Initialize the free list of OS_TCBs      */
    OS_InitEventList();                                          /* Initialize the free list of OS_EVENTs    */

#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
    OS_FlagInit();                                               /* Initialize the event flag structures     */
#endif

#if (OS_MEM_EN > 0) && (OS_MAX_MEM_PART > 0)
    OS_MemInit();                                                /* Initialize the memory manager            */
#endif

#if (OS_Q_EN > 0) && (OS_MAX_QS > 0)
    OS_QInit();                                                  /* Initialize the message queue structures  */
#endif

    OS_InitTaskIdle();                                           /* Create the Idle Task                     */
#if OS_TASK_STAT_EN > 0
    OS_InitTaskStat();                                           /* Create the Statistic Task                */
#endif

#if OS_VERSION >= 204
    OSInitHookEnd();                                             /* Call port specific init. code            */
#endif
}
//中断函数正在执行 ENTER ISR
//描述: 通知uC/OS-II，一个中断处理函数正在进行，这有助于uC/OS-II掌握中断嵌套情况。
//OSIntEnter ()和OSIntExit (函数联合使用)，调用者，只能在中断程序中。
//参数: 无	   
//返回: 无	  
//注意: 1) 在任务级不能调用该函数
//      2) 如果系统使用的处理器能够执行自动的独立执行读取－修改－写入的操作，那么就可以直接递增
//         中断嵌套层数(OSIntNesting)，这样可以避免调用函数所带来的额外开销。在中断服务子程序中
//         给OSIntNesting加1是不会有问题的，因为给OSIntNesting加1时，中断是关闭的
//      3) 中断嵌套深度可达255
void  OSIntEnter (void)
{
#if OS_CRITICAL_METHOD == 3                                //中断函数被设定为模式3
    OS_CPU_SR  cpu_sr;
#endif
	//OS_ENTER_CRITICAL(); //关闭中断
    if (OSRunning == TRUE) {
        if (OSIntNesting < 255) {
            OSIntNesting++;                      //中断嵌套计数变量加1
        }
    }
	//OS_EXIT_CRITICAL(); //打开中断
}

//中断函数已经完成 EXIT ISR
//描述: 通知uC/OS-II，一个中断服务已经执行完成，这有助于uC/OS-II掌握中断嵌套的情况。通常
//OSIntExit()和OSIntEnter()联合使用。当最后一层嵌套的中断执行完毕时，如果有更高优先级任务
//准备就绪，则uC/OS-II会调用任务调度函数。在这种情况下，中断返回到更高优先级的任务，而不
//是被中断了的任务。调用者，只能在中断程序中。
//参数: 无	   
//返回: 无	  
//注意: 1) 在任务级不能调用该函数，并且即使没有调用OSIntEnter()函数，而是使用直接递增OSIntNesting的方法，
//         也必须调用OSIntExit()。
//      2) 给调度器上锁用于禁止任务调度 (查看 OSSchedLock()函数)	  
void  OSIntExit (void)
{
#if OS_CRITICAL_METHOD == 3                                //中断函数被设定为模式3
    OS_CPU_SR  cpu_sr;
#endif
    
    
    if (OSRunning == TRUE) {							   //关闭中断
        OS_ENTER_CRITICAL();
        if (OSIntNesting > 0) {                            //如果中断嵌套大于0
            OSIntNesting--;								   //中断嵌套计数变量减1
        }
//1)中断嵌套层数计数器和锁定嵌套计数器(OSLockNesting)二者都必须是零
//2)OSRdyTbl[]所需的检索值Y是保存在全程变量OSIntExitY中
//3)检查具有最高优先级别的就绪任务的优先级是否是正在运行的任务的优先级
//4)将任务控制块优先级表保存到指向最高级优先级就绪任务控制块的指针
//5)上下文切换的次数(统计任务计数器)
//6)做中断任务切换	
        if ((OSIntNesting == 0) && (OSLockNesting == 0)) { //1
            OSIntExitY    = OSUnMapTbl[OSRdyGrp];          //2
            OSPrioHighRdy = (INT8U)((OSIntExitY << 3) + OSUnMapTbl[OSRdyTbl[OSIntExitY]]);
            if (OSPrioHighRdy != OSPrioCur) {              //3
                OSTCBHighRdy  = OSTCBPrioTbl[OSPrioHighRdy];//4
                OSCtxSwCtr++;                              //5
                OSIntCtxSw();                              //6
            }
        }
        OS_EXIT_CRITICAL();	//打开中断
    }
}

//给调度器上锁 PREVENT SCHEDULING
//描述: 本函数用于禁止任务调度，直到任务完成后调用给调度器开锁函数OSSchedUnlock()为止。调用
//	  	OSSchedlock()的任务保持对CPU的控制权，尽管有个优先级更高的任务进入了就绪态。然而,此时
//		中断是可以被识别的,中断服务也能得到(假设中断是开着的)。OSSchedlock()和OSSchedUnlock()
//		必须成对使用.变量OSLockNesting跟踪OSSchedLock()函数被调用的次数,以允许嵌套的函数包含临
//		界段代码,这段代码其它任务不得干预.uC/OS-II允许嵌套深度达255层.当OSLockNesting等于零时，
//		调度重新得到允许.函数OSSchedLock()和OSSchedUnlock()的使用要非常谨慎,因为它们影响uC/OS-II
//		对任务的正常管理。
//说明：当OSLockNesting减到零的时候，OSSchedUnlock()调用OSSched。OSSchedUnlock()是被某任务调用的，
//		在调度器上锁的期间，可能有什么事件发生了并使一个更高优先级的任务进入就绪态。
//参数: 无
//返回: 无
//警告：不得调用把当前任务挂起的程序
//注意: 1) 调用OSSchedLock()以后，用户的应用程序不得使用任何能将现行任务挂起的系统调用。也就说，
//		用户程序不得调用OSMboxPend()、OSQPend()、OSSemPend()、OSTaskSuspend(OS_PR1O_SELF)、
//		OSTimeDly()或OSTimeDlyHMSM(),直到OSLockNesting回零为止。因为调度器上了锁，用户就锁住
//		了系统，任何其它任务都不能运行。
//		2) 当低优先级的任务要发消息给多任务的邮箱、消息队列、信号量时，用户不希望高优先级的任
//		务在邮箱、队列和信号量没有得到消息之前就取得了CPU的控制权，此时，用户可以使用禁止
//		调度器函数。
#if OS_SCHED_LOCK_EN > 0   //允许生产OSSchedLock()函数
void  OSSchedLock (void)
{
#if OS_CRITICAL_METHOD == 3                      //中断函数被设定为模式3
    OS_CPU_SR  cpu_sr;
#endif    
    
    
    if (OSRunning == TRUE) {                      //如果有多个任务在运行
        OS_ENTER_CRITICAL();
        if (OSLockNesting < 255) {               //上锁嵌套是否大于255
            OSLockNesting++;                     //给上锁嵌套加1
        }
        OS_EXIT_CRITICAL();					     //打开中断
    }
}
#endif    

//给调度器解锁 ENABLE SCHEDULING
//描述: 本函数用于解禁任务调度
//参数: 无
//返回: 无
//注意: 1) OSSchedlock()和OSSchedUnlock()必须成对使用，在使用OSSchedUnlock()函数之前必须使
//		   用OSSchedLock()函数
#if OS_SCHED_LOCK_EN > 0
void  OSSchedUnlock (void)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif    
    
    
    if (OSRunning == TRUE) {                                   /* Make sure multitasking is running    */
        OS_ENTER_CRITICAL();
        if (OSLockNesting > 0) {                               /* Do not decrement if already 0        */
            OSLockNesting--;                                   /* Decrement lock nesting level         */
            if ((OSLockNesting == 0) && (OSIntNesting == 0)) { /* See if sched. enabled and not an ISR */
                OS_EXIT_CRITICAL();
                OS_Sched();                                    /* See if a HPT is ready                */
            } else {
                OS_EXIT_CRITICAL();
            }
        } else {
            OS_EXIT_CRITICAL();
        }
    }
}
#endif  
  
//启动多个任务 START MULTITASKING
//描述: 当调用OSStart()时，OSStart()从任务就绪表中找出那个用户建立的优先级最高任务的任务控制
//		块。然后，OSStart()调用高优先级就绪任务启动函数OSStartHighRdy()，(见汇编语言文件
//		OS_CPU_A.ASM)，这个文件与选择的微处理器有关。实质上，函数OSStartHighRdy()是将任务栈中
//		保存的值弹回到CPU寄存器中，然后执行一条中断返回指令，中断返回指令强制执行该任务代码。
//		高优先级就绪任务启动函数OSStartHighRdy()。
//参数: 无
//返回: 无
//注意: OSStartHighRdy() 必须:
//		a) OSRunning为真，指出多任务已经开始
//		b) 启动uC/OS-II之前，至少必须建立一个应用任务
//	    c) OSStartHighRdy()将永远不返回到OSStart()
void  OSStart (void)
{
    INT8U y;
    INT8U x;
    if (OSRunning == FALSE) {//OSRunning已设为"真"，指出多任务已经开始
        y             = OSUnMapTbl[OSRdyGrp];        //查找最高优先级别任务号码
        x             = OSUnMapTbl[OSRdyTbl[y]];
        OSPrioHighRdy = (INT8U)((y << 3) + x);		 //找出就绪态最高级任务控制块
        OSPrioCur     = OSPrioHighRdy;				 //OSPrioCur和OSPrioHighRdy存放的是用户应用任务的优先级
        OSTCBHighRdy  = OSTCBPrioTbl[OSPrioHighRdy]; /* Point to highest priority task ready to run    */
        OSTCBCur      = OSTCBHighRdy;
        OSStartHighRdy();                            //调用高优先级就绪任务启动函数
    }
}

//统计任务初始化 STATISTICS INITIALIZATION
//描述: 统计初始化函数OSStatInit()决定在没有其它应用任务运行时，空闲计数器(OSIdleCtr)的计数
//		有多快。这个任务每秒执行一次，以确定所有应用程序中的任务消耗了多少CPU时间。当用户的
//		应用程序代码加入以后，运行空闲任务的CPU时间就少了，OSIdleCtr就不会像原来什么任务都不
//		运行时有那么多计数。要知道，OSIdleCtr的最大计数值是OSStatInit()在初始化时保存在计数
//		器最大值OSIdleCtrMax中的。CPU利用率：
//		                                        空闲计数值OSIdleCtr
//      CPU 使用率Usage (%) = 100 * (1 - --------------------------------)
//                                        设定最大空闲计数值OSIdleCtrMax
//参数: 无
//返回: 无											   
#if OS_TASK_STAT_EN > 0
void  OSStatInit (void)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    
    
    OSTimeDly(2);                                //调用延迟函数OSTimeDly()将自身延时2个时钟节拍以停止自身的运行
												 //这是为了使OSStatInit()与时钟节拍同步
    OS_ENTER_CRITICAL();
    OSIdleCtr    = 0L;                           //执行OSStartInit()时，空闲计数器OSIdleCtr被清零                                */
    OS_EXIT_CRITICAL();
    OSTimeDly(OS_TICKS_PER_SEC);                 //将自身延时整整一秒
	                                             //(因为没有其它进入就绪态的任务，OSTaskIdle()又获得了CPU的控制权)
    OS_ENTER_CRITICAL();
    OSIdleCtrMax = OSIdleCtr;                    //空闲计数器将1秒钟内计数的值存入空闲计数器最大值
    OSStatRdy    = TRUE;						 //将统计任务就绪标志OSStatRdy设为"真"，以此来允许两个时钟节拍
    OS_EXIT_CRITICAL();
}
#endif
//时钟节拍函数 PROCESS SYSTEM TICK
//描述: uC/OS需要用户提供周期性信号源，用于实现时间延时和确认超时。节拍率应在每秒10次到100次
//		之间，或者说10到100Hz。时钟节拍率越高，系统的额外负荷就越重.时钟节拍的实际频率取决于
//		用户应用程序的精度。时钟节拍源可以是专门的硬件定时器，也可以是来自50/60Hz交流电源的
//		信号														   
//参数: 无
//返回: 无
void  OSTimeTick (void)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif    
    OS_TCB    *ptcb; //定义任务控制块优先级表变量


    OSTimeTickHook();                                      //调用户·自定义函数(钩子函数)
#if OS_TIME_GET_SET_EN > 0   
    OS_ENTER_CRITICAL();                                   //累加从开机以来的时间，用的是一个无符号32位变量
    OSTime++;
    OS_EXIT_CRITICAL();
#endif
    if (OSRunning == TRUE) {    
        ptcb = OSTCBList;                                  //保存任务控制块列表首地址
														   //从OSTCBList开始，沿着OS_TCB链表做，一直做到空闲任务
        while (ptcb->OSTCBPrio != OS_IDLE_PRIO) {          /* Go through all TCBs in TCB list          */
            OS_ENTER_CRITICAL();
            if (ptcb->OSTCBDly != 0) {                     //如果任务等待时的最多节拍数不为0
										   				//而确切被任务挂起的函数OSTaskSuspend()挂起的任务则不会进入就绪态
														//执行时间直接与应用程序中建立了多少个务成正比
             if (--ptcb->OSTCBDly == 0) {               //如果任务等待时的最多节拍数为0
														//当某任务的任务控制块中的时间延时项OSTCBDly减到了零，这个任务就进入了就绪态
                    if ((ptcb->OSTCBStat & OS_STAT_SUSPEND) == OS_STAT_RDY) { /* Is task suspended?    */
                        OSRdyGrp               |= ptcb->OSTCBBitY; /* No,  Make task R-to-R (timed out)*/
                        OSRdyTbl[ptcb->OSTCBY] |= ptcb->OSTCBBitX;
                    } else {                               /* Yes, Leave 1 tick to prevent ...         */
                        ptcb->OSTCBDly = 1;                /* ... loosing the task when the ...        */
                    }                                      /* ... suspension is removed.               */
                }
            }
            ptcb = ptcb->OSTCBNext;                        //指向任务块双向链接表的后链接
            OS_EXIT_CRITICAL();
        }
    }
}		 
//获得版本号GET VERSION
//描述: 这个函数是返回一个uC/OS-II的版本值. 这个返回值除以100是uC/OS-II的版本号. 														   
//参数: 无
//返回: uC/OS-II的版本号除以100.
INT16U  OSVersion (void)
{
    return (OS_VERSION);
}

//虚拟函数 DUMMY FUNCTION
//描述: 这个函数不做任务工作. 它是随便访问OSTaskDel()函数.														   
//参数: 无
//返回: uC/OS-II的版本号除以100.
#if OS_TASK_DEL_EN > 0
void  OS_Dummy (void)
{
}
#endif

//使一个任务进入就绪态 MAKE TASK READY TO RUN BASED ON EVENT OCCURING
//描述: 当发生了某个事件，该事件等待任务列表中的最高优先级任务(HPT)要置于就绪态时，该事件对应		  
//		的OSSemPost()，OSMboxPost()，OSQPost()，和OSQPostFront()函数调用OSEventTaskRdy()实现
//		该操作。换句话说，该函数从等待任务队列中删除HPT任务，并把该任务置于就绪态
//参数: pevent    is a pointer to the event control block corresponding to the event.
//      msg       is a pointer to a message.  This pointer is used by message oriented services
//                such as MAILBOXEs and QUEUEs.  The pointer is not used when called by other
//                service functions.
//      msk       is a mask that is used to clear the status byte of the TCB.  For example,
//                OSSemPost() will pass OS_STAT_SEM, OSMboxPost() will pass OS_STAT_MBOX etc.//返回: 无
//注意: 这个函数是uC/OS-II内部函数，你不可以在应用程序调用它,调用此函数也应当关闭中断
#if OS_EVENT_EN > 0
INT8U  OS_EventTaskRdy (OS_EVENT *pevent, void *msg, INT8U msk)
{
    OS_TCB *ptcb;
    INT8U   x;
    INT8U   y;
    INT8U   bitx;
    INT8U   bity;
    INT8U   prio;

//1)首先计算HPT任务在.OSEventTbl[]中的字节索引，其结果是一个从0到OS_LOWEST_PRIO/8+1之间的数
//2)并利用该索引得到该优先级任务在.OSEventGrp中的位屏蔽码
//3)判断HPT任务在.OSEventTbl[]中相应位的位置
//4)其结果是一个从0到OS_LOWEST_PRIO/8+1之间的数，以及相应的位屏蔽码
//5)根据以上结果，OSEventTaskRdy()函数计算出HPT任务的优先级
//6)然后就可以从等待任务列表中删除该任务了
    y    = OSUnMapTbl[pevent->OSEventGrp];            /* Find highest prio. task waiting for message   */
    bity = OSMapTbl[y];
    x    = OSUnMapTbl[pevent->OSEventTbl[y]];
    bitx = OSMapTbl[x];
    prio = (INT8U)((y << 3) + x);                     /* Find priority of task getting the msg         */
    if ((pevent->OSEventTbl[y] &= ~bitx) == 0x00) {   /* Remove this task from the waiting list        */
        pevent->OSEventGrp &= ~bity;                  /* Clr group bit if this was only task pending   */
    }
//7)任务的TCB中包含有需要改变的信息。知道了HPT任务的优先级，就可得到指向该任务的TCB的指针
//8)因为最高优先级任务运行条件已经得到满足，必须停止OSTimeTick()函数对.OSTCBDly域的递减操作，
// 所以OSEventTaskRdy()直接将该域清澈0
//9)因为该任务不再等待该事件的发生，所以本函数将其任务控制块中指向事件控制块的指针指向NULL
//10)如果OSEventTaskRdy()是由OSMboxPost()或者OSQPost()调用的，该函数还要将相应的消息传递给
// HPT，放在它的任务控制块中
    ptcb                 =  OSTCBPrioTbl[prio];       /* Point to this task's OS_TCB                   */
    ptcb->OSTCBDly       =  0;                        /* Prevent OSTimeTick() from readying task       */
    ptcb->OSTCBEventPtr  = (OS_EVENT *)0;             /* Unlink ECB from this task                     */
#if ((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0)
    ptcb->OSTCBMsg       = msg;                       /* Send message directly to waiting task         */
#else
    msg                  = msg;                       /* Prevent compiler warning if not used          */
#endif
//11)当OSEventTaskRdy()被调用时，位屏蔽码msk作为参数传递给它。该参数是用于对任务控制块中的
// 位清零的位屏蔽码，和所发生事件的类型相对应
//12)根据.OSTCBStat判断该任务是否已处于就绪状态
//13)如果是, 则将HPT插入到uC/OS-II的就绪任务列表中。注意，HPT任务得到该事件后不一定进入就绪
// 状态，也许该任务已经由于其它原因挂起了
    ptcb->OSTCBStat     &= ~msk;                      /* Clear bit associated with event type          */
    if (ptcb->OSTCBStat == OS_STAT_RDY) {             /* See if task is ready (could be susp'd)        */
        OSRdyGrp        |=  bity;                     /* Put task in the ready to run list             */
        OSRdyTbl[y]     |=  bitx;
    }
    return (prio);
}
#endif
//使一个任务进入等待某事件发生状态 MAKE TASK WAIT FOR EVENT TO OCCUR		  
//描述: 当某个任务须等待一个事件的发生时，信号量、互斥型信号量、邮箱以及消息队列会通过相应的
//		PEND函数调用本函数，使当前任务从就绪任务表中脱离就绪态，并放到相应的事件控制块ECB的等
// 		待任务表中	 
//参数: pevent 分配给事件控制块的指针，为等待某事件发生的任务		 
//返回: 无	   
//注意: 这个函数是uC/OS-II内部函数，你不可在应用程序中调用它，调用OS_EventTO()也应当关闭中断
#if OS_EVENT_EN > 0
void  OS_EventTaskWait (OS_EVENT *pevent)
{	//将指向事件控制块ECB的指针放到任务的任务控制块TCB中，建立任务与事件控制块ECB之间的链接
    OSTCBCur->OSTCBEventPtr = pevent;             //将任务从就绪任务表中删除
    if ((OSRdyTbl[OSTCBCur->OSTCBY] &= ~OSTCBCur->OSTCBBitX) == 0x00) {   /* Task no longer ready      */
        OSRdyGrp &= ~OSTCBCur->OSTCBBitY;        /* Clear event grp bit if this was only task pending  */
    }
    //把该任务放到事件控制块ECB的等待事件的任务列表中
	pevent->OSEventTbl[OSTCBCur->OSTCBY] |= OSTCBCur->OSTCBBitX;          /* Put task in waiting list  */
    pevent->OSEventGrp                   |= OSTCBCur->OSTCBBitY;
}
#endif

//由于超时而将任务置为就绪态 MAKE TASK READY TO RUN BASED ON EVENT TIMEOUT	    
//描述: 如果在预先指定的等待时限内任务等待的事件没有发生，那么本函数会因为等待超时而将任务的
//		状态置为就绪态。在这种情况下，信号量、互斥型信号量、邮箱以及消息队列会通过PEND函数调
//		用本函数，以完成这项工作	    
//参数: pevent 分配给事件控制块的指针，为超时就绪态的任务   
//返回: 无	    
//注意: 这个函数是uC/OS-II内部函数，你不可以在应用程序中调用它，调用OS_EventTO()也应当关闭中断
#if OS_EVENT_EN > 0
void  OS_EventTO (OS_EVENT *pevent)
{
    if ((pevent->OSEventTbl[OSTCBCur->OSTCBY] &= ~OSTCBCur->OSTCBBitX) == 0x00) {
        pevent->OSEventGrp &= ~OSTCBCur->OSTCBBitY;
    }
    OSTCBCur->OSTCBStat     = OS_STAT_RDY;       /* Set status to ready                                */
    OSTCBCur->OSTCBEventPtr = (OS_EVENT *)0;     /* No longer waiting for event                        */
}
#endif
//事件控制块列表初始化 INITIALIZE EVENT CONTROL BLOCK'S WAIT LIST
//描述: 当建立一个信号量、邮箱或者消息队列时，相应的建立函数OSSemInit()，OSMboxCreate()，或者
//		OSQCreate()通过调用OSEventWaitListInit()对事件控制块中的等待任务列表进行初始化。该函数
//		初始化一个空的等待任务列表，其中没有任何任务。该函数的调用参数只有一个，就是指向需要初
// 		始化的事件控制块的指针pevent。
//参数: pevent 传递一个指针给事件控制块，该指针变量就是创建信号量、互斥型信号量、邮箱或消息队
// 		列时分配的事件控制块的指针
//返回: 无
//注意: 这个函数是uC/OS-II内部函数，你不可以调用它。
#if ((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0) || (OS_SEM_EN > 0) || (OS_MUTEX_EN > 0)
void  OS_EventWaitListInit (OS_EVENT *pevent)
{
    INT8U  *ptbl;


    pevent->OSEventGrp = 0x00;                   /* No task waiting on event                           */
    ptbl               = &pevent->OSEventTbl[0];

#if OS_EVENT_TBL_SIZE > 0
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 1
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 2
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 3
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 4
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 5
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 6
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 7
    *ptbl              = 0x00;
#endif
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                             INITIALIZATION
*                           INITIALIZE THE FREE LIST OF EVENT CONTROL BLOCKS
*
* Description: This function is called by OSInit() to initialize the free list of event control blocks.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************
*/

static  void  OS_InitEventList (void)
{
#if (OS_EVENT_EN > 0) && (OS_MAX_EVENTS > 0)
#if (OS_MAX_EVENTS > 1)
    INT16U     i;
    OS_EVENT  *pevent1;
    OS_EVENT  *pevent2;


    pevent1 = &OSEventTbl[0];
    pevent2 = &OSEventTbl[1];
    for (i = 0; i < (OS_MAX_EVENTS - 1); i++) {                  /* Init. list of free EVENT control blocks  */
        pevent1->OSEventType = OS_EVENT_TYPE_UNUSED;
        pevent1->OSEventPtr  = pevent2;
        pevent1++;
        pevent2++;
    }
    pevent1->OSEventType = OS_EVENT_TYPE_UNUSED;
    pevent1->OSEventPtr  = (OS_EVENT *)0;
    OSEventFreeList      = &OSEventTbl[0];
#else
    OSEventFreeList              = &OSEventTbl[0];               /* Only have ONE event control block        */
    OSEventFreeList->OSEventType = OS_EVENT_TYPE_UNUSED;
    OSEventFreeList->OSEventPtr  = (OS_EVENT *)0;
#endif
#endif
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                             INITIALIZATION
*                                    INITIALIZE MISCELLANEOUS VARIABLES
*
* Description: This function is called by OSInit() to initialize miscellaneous variables.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************
*/

static  void  OS_InitMisc (void)
{
#if OS_TIME_GET_SET_EN > 0   
    OSTime        = 0L;                                          /* Clear the 32-bit system clock            */
#endif

    OSIntNesting  = 0;                                           /* Clear the interrupt nesting counter      */
    OSLockNesting = 0;                                           /* Clear the scheduling lock counter        */

    OSTaskCtr     = 0;                                           /* Clear the number of tasks                */

    OSRunning     = FALSE;                                       /* Indicate that multitasking not started   */
    
    OSCtxSwCtr    = 0;                                           /* Clear the context switch counter         */
    OSIdleCtr     = 0L;                                          /* Clear the 32-bit idle counter            */

#if (OS_TASK_STAT_EN > 0) && (OS_TASK_CREATE_EXT_EN > 0)
    OSIdleCtrRun  = 0L;
    OSIdleCtrMax  = 0L;
    OSStatRdy     = FALSE;                                       /* Statistic task is not ready              */
#endif
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                             INITIALIZATION
*                                       INITIALIZE THE READY LIST
*
* Description: This function is called by OSInit() to initialize the Ready List.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************
*/

static  void  OS_InitRdyList (void)
{
    INT16U   i;
    INT8U   *prdytbl;


    OSRdyGrp      = 0x00;                                        /* Clear the ready list                     */
    prdytbl       = &OSRdyTbl[0];
    for (i = 0; i < OS_RDY_TBL_SIZE; i++) {
        *prdytbl++ = 0x00;
    }

    OSPrioCur     = 0;
    OSPrioHighRdy = 0;

    OSTCBHighRdy  = (OS_TCB *)0;                                 
    OSTCBCur      = (OS_TCB *)0;
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                             INITIALIZATION
*                                         CREATING THE IDLE TASK
*
* Description: This function creates the Idle Task.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************
*/

static  void  OS_InitTaskIdle (void)
{
#if OS_TASK_CREATE_EXT_EN > 0
    #if OS_STK_GROWTH == 1
    (void)OSTaskCreateExt(OS_TaskIdle,
                          (void *)0,                                 /* No arguments passed to OS_TaskIdle() */
                          &OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE - 1], /* Set Top-Of-Stack                     */
                          OS_IDLE_PRIO,                              /* Lowest priority level                */
                          OS_TASK_IDLE_ID,
                          &OSTaskIdleStk[0],                         /* Set Bottom-Of-Stack                  */
                          OS_TASK_IDLE_STK_SIZE,
                          (void *)0,                                 /* No TCB extension                     */
                          OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);/* Enable stack checking + clear stack  */
    #else
    (void)OSTaskCreateExt(OS_TaskIdle,
                          (void *)0,                                 /* No arguments passed to OS_TaskIdle() */
                          &OSTaskIdleStk[0],                         /* Set Top-Of-Stack                     */
                          OS_IDLE_PRIO,                              /* Lowest priority level                */
                          OS_TASK_IDLE_ID,
                          &OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE - 1], /* Set Bottom-Of-Stack                  */
                          OS_TASK_IDLE_STK_SIZE,
                          (void *)0,                                 /* No TCB extension                     */
                          OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);/* Enable stack checking + clear stack  */
    #endif
#else
    #if OS_STK_GROWTH == 1
    (void)OSTaskCreate(OS_TaskIdle,
                       (void *)0,
                       &OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE - 1],
                       OS_IDLE_PRIO);
    #else
    (void)OSTaskCreate(OS_TaskIdle,
                       (void *)0,
                       &OSTaskIdleStk[0],
                       OS_IDLE_PRIO);
    #endif
#endif
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                             INITIALIZATION
*                                      CREATING THE STATISTIC TASK
*
* Description: This function creates the Statistic Task.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************
*/

#if OS_TASK_STAT_EN > 0
static  void  OS_InitTaskStat (void)
{
#if OS_TASK_CREATE_EXT_EN > 0
    #if OS_STK_GROWTH == 1
    (void)OSTaskCreateExt(OS_TaskStat,
                          (void *)0,                                   /* No args passed to OS_TaskStat()*/
                          &OSTaskStatStk[OS_TASK_STAT_STK_SIZE - 1],   /* Set Top-Of-Stack               */
                          OS_STAT_PRIO,                                /* One higher than the idle task  */
                          OS_TASK_STAT_ID,
                          &OSTaskStatStk[0],                           /* Set Bottom-Of-Stack            */
                          OS_TASK_STAT_STK_SIZE,
                          (void *)0,                                   /* No TCB extension               */
                          OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);  /* Enable stack checking + clear  */
    #else
    (void)OSTaskCreateExt(OS_TaskStat,
                          (void *)0,                                   /* No args passed to OS_TaskStat()*/
                          &OSTaskStatStk[0],                           /* Set Top-Of-Stack               */
                          OS_STAT_PRIO,                                /* One higher than the idle task  */
                          OS_TASK_STAT_ID,
                          &OSTaskStatStk[OS_TASK_STAT_STK_SIZE - 1],   /* Set Bottom-Of-Stack            */
                          OS_TASK_STAT_STK_SIZE,
                          (void *)0,                                   /* No TCB extension               */
                          OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);  /* Enable stack checking + clear  */
    #endif
#else
    #if OS_STK_GROWTH == 1
    (void)OSTaskCreate(OS_TaskStat,
                       (void *)0,                                      /* No args passed to OS_TaskStat()*/
                       &OSTaskStatStk[OS_TASK_STAT_STK_SIZE - 1],      /* Set Top-Of-Stack               */
                       OS_STAT_PRIO);                                  /* One higher than the idle task  */
    #else
    (void)OSTaskCreate(OS_TaskStat,
                       (void *)0,                                      /* No args passed to OS_TaskStat()*/
                       &OSTaskStatStk[0],                              /* Set Top-Of-Stack               */
                       OS_STAT_PRIO);                                  /* One higher than the idle task  */
    #endif
#endif
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                             INITIALIZATION
*                            INITIALIZE THE FREE LIST OF TASK CONTROL BLOCKS
*
* Description: This function is called by OSInit() to initialize the free list of OS_TCBs.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************
*/

static  void  OS_InitTCBList (void)
{
    INT8U    i;
    OS_TCB  *ptcb1;
    OS_TCB  *ptcb2;


    OSTCBList     = (OS_TCB *)0;                                 /* TCB Initialization                       */
    for (i = 0; i < (OS_LOWEST_PRIO + 1); i++) {                 /* Clear the priority table                 */
        OSTCBPrioTbl[i] = (OS_TCB *)0;
    }
    ptcb1 = &OSTCBTbl[0];
    ptcb2 = &OSTCBTbl[1];
    for (i = 0; i < (OS_MAX_TASKS + OS_N_SYS_TASKS - 1); i++) {  /* Init. list of free TCBs                  */
        ptcb1->OSTCBNext = ptcb2;
        ptcb1++;
        ptcb2++;
    }
    ptcb1->OSTCBNext = (OS_TCB *)0;                              /* Last OS_TCB                              */
    OSTCBFreeList    = &OSTCBTbl[0];
}

//任务调度
//描述: uC/OS-II总是运行进入就绪态任务中优先级最高的那一个。确定哪个任务优先级最高，下面该哪
// 		个任务运行了的工作是由调度器(Scheduler)完成的。任务级的调度是由函数OSSched()完成的。
//		中断级的调度是由另一个函数OSIntExt()完成的eduling).
//参数: none
//返回: none
//注意: 1) 这是一个uC/OS-II内部函数，你不能在应用程序中使用它
//		2) 给调度器上锁用于禁止任务调度 (查看 OSSchedLock()函数)
//说明: 1)任务切换很简单，由以下两步完成，将被挂起任务的微处理器寄存器推入堆栈，然后将较高优先
//		级的任务的寄存器值从栈中恢复到寄存器中。在uC/OS-II中，就绪任务的栈结构总是看起来跟刚
//	 	刚发生过中断一样，所有微处理器的寄存器都保存在栈中。换句话说，uC/OS-II运行就绪态的任
//		务所要做的一切，只是恢复所有的CPU寄存器并运行中断返回指令。为了做任务切换，运行
//		OS_TASK_SW(),人为模仿了一次中断。多数微处理器有软中断指令或者陷阱指令TRAP来实现上述操
//		作。中断服务子程序或陷阱处理(Trap hardler)，也称作事故处理(exception handler)，必须提
//		供中断向量给汇编语言函数OSCtxSw()。OSCtxSw()除了需要OS_TCBHighRdy指向即将被挂起的任务，
//		还需要让当前任务控制块OSTCBCur指向即将被挂起的任务，参见第8章，移植uC/OS-II，有关于
//		OSCtxSw()的更详尽的解释。
//		2) OSSched()的所有代码都属临界段代码。在寻找进入就绪态的优先级最高的任务过程中，为防止中
//		断服务子程序把一个或几个任务的就绪位置位，中断是被关掉的。为缩短切换时间，OSSched()全
//		部代码都可以用汇编语言写。为增加可读性，可移植性和将汇编语言代码最少化，OSSched()是用
//		C写的。


void  OS_Sched (void)
{
#if OS_CRITICAL_METHOD == 3                            /* Allocate storage for CPU status register     */
    OS_CPU_SR  cpu_sr;
#endif    
    INT8U      y;


    OS_ENTER_CRITICAL();
	//为实现任务切换，OSTCBHighRdy必须指向优先级最高的那个任务控制块OS_TCB，这是通过将
    if ((OSIntNesting == 0) && (OSLockNesting == 0)) { /* Sched. only if all ISRs done & not locked    */
	//如果函数不是在中断服务子程序中调用的，且调度允许的，则任务调度函数将找出进入就绪态的
	//最高优先级任务，进入就绪态的任务在就绪表中OSRdyTbl[ ]中相应位置位.
        y             = OSUnMapTbl[OSRdyGrp];          /* Get pointer to HPT ready to run              */
        OSPrioHighRdy = (INT8U)((y << 3) + OSUnMapTbl[OSRdyTbl[y]]);
 		//找到最高优先级任务后，函数检查这个优先级最高的任务是否是当前正在运行的任务，以避免不
		//必要的任务调度，多花时间
       if (OSPrioHighRdy != OSPrioCur) {              /* No Ctx Sw if current task is highest rdy     */
		//为实现任务切换，OSTCBHighRdy必须指向优先级最高的那个任务控制块OS_TCB，这是通过将
		//以OSPrioHighRdy为下标的OSTCBPrioTbl[]数组中的那个元素赋给OSTCBHighRdy来实现的
            OSTCBHighRdy = OSTCBPrioTbl[OSPrioHighRdy];
            OSCtxSwCtr++;                              //统计计数器OSCtxSwCtr加1，以跟踪任务切换次数
            OS_TASK_SW();                              //最后宏调用OS_TASK_SW()来完成实际上的任务切换
        }
    }
    OS_EXIT_CRITICAL();
}
//空闲任务 IDLE TASK
//描述: 这个函数是uC/OS-II内部函数，uC/OS-II总要建立一个空闲任务，这个任务在没有其它任务进入
//		就绪态时投入运行。这个空闲任务永远设为最低优先级，即OS_LOWEST_PRIO.空闲任务不可能被应
//		用软件删除。
//参数: 无
//返回: 无
//注意: 1) OSTaskIdleHook()可以允许用户在函数中写入自己的代码，可以借助OSTaskIdleHook()，让
//		CPU执行STOP指令，从而进入低功耗模式，当应用系统由电池供电时，这种方式特别有用。
//		2) 这个函数永远处于就绪态，所以不要在OSTaskIdleHook()中调用可以使任务挂起的PEND函数、
//		OSTineDly???()函数和OSTaskSuspend()函数
void  OS_TaskIdle (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    
    
    pdata = pdata;                               /* Prevent compiler warning for not using 'pdata'     */
    for (;;) {
        OS_ENTER_CRITICAL();
        OSIdleCtr++;
        OS_EXIT_CRITICAL();
        OSTaskIdleHook();                        /* Call user definable HOOK                           */
    }
}
//统计任务 STATISTICS TASK
//描述: uC/OS-II有一个提供运行时间统计的任务。这个任务叫做OSTaskStat(),如果用户将系统定义常
//		数OS_TASK_STAT_EN(见文件OS_CFG.H)设为1，这个任务就会建立。一旦得到了允许，OSTaskStat()
//		每秒钟运行一次(见文件OS_CORE.C)，计算当前的CPU利用率。换句话说，OSTaskStat()告诉用户
//		应用程序使用了多少CPU时间，用百分比表示，这个值放在一个有符号8位整数OSCPUsage中，精读
//		度是1个百分点。
//		如果用户应用程序打算使用统计任务，用户必须在初始化时建立一个唯一的任务，在这个任务中
//		调用OSStatInit()(见文件OS_CORE.C)。换句话说，在调用系统启动函数OSStart()之前，用户初
//		始代码必须先建立一个任务，在这个任务中调用系统统计初始化函数OSStatInit()，然后再建立
//		应用程序中的其它任务
//								 OSIdleCtr
//		OSCPUUsage = 100 * (1 - ------------) (units are in %)
//								OSIdleCtrMax
//参数: pdata 指向一个数据结构，该结构用来在建立统计任务时向任务传递参数
//返回: 无
//注意: 1) uC/OS-II已经将空闲任务的优先级设为最低，即OS_LOWEST_PR10，统计任务的优先级设为次
//		低，OS_LOWEST_PR10-1.
//		2) 因为用户的应用程序必须先建立一个起始任务TaskStart()。在使用统计任务前，用户必须首
//		先调用的是uC/OS-II中的系统初始化函数OSInit()，
//		3) 在创建统计任务之前，为了保持系统达到稳定状态，需要延迟5秒钟，你必须至少延时2秒钟
//		以设定最大空闲计数值
#if OS_TASK_STAT_EN > 0
void  OS_TaskStat (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    INT32U     run;
    INT32U     max;
    INT8S      usage;


    pdata = pdata;                               /* Prevent compiler warning for not using 'pdata'     */
    while (OSStatRdy == FALSE) {
        OSTimeDly(2 * OS_TICKS_PER_SEC);         /* Wait until statistic task is ready                 */
    }
    max = OSIdleCtrMax / 100L;
    for (;;) {
        OS_ENTER_CRITICAL();
        OSIdleCtrRun = OSIdleCtr;                /* Obtain the of the idle counter for the past second */
        run          = OSIdleCtr;
        OSIdleCtr    = 0L;                       /* Reset the idle counter for the next second         */
        OS_EXIT_CRITICAL();
        if (max > 0L) {
            usage = (INT8S)(100L - run / max);
            if (usage >= 0) {                    /* Make sure we don't have a negative percentage      */
                OSCPUUsage = usage;
            } else {
                OSCPUUsage = 0;
            }
        } else {
            OSCPUUsage = 0;
            max        = OSIdleCtrMax / 100L;
        }
        OSTaskStatHook();                        /* Invoke user definable hook                         */
        OSTimeDly(OS_TICKS_PER_SEC);             /* Accumulate OSIdleCtr for the next second           */
    }
}
#endif
//任务控制块初始化 INITIALIZE TCB
//描述: 这个函数是uC/OS-II内部函数，在建立任务时调用的初始化任务控制块OS_TCB函数,含7个参数，
//		(查看 OSTaskCreate() 和 OSTaskCreateExt()).
//参数: prio 任务的优先级
//		ptos OSTaskInit()建立栈结构以后,ptos是指向栈顶的指针,且保存在OS_TCB的OSTCBStkPrt中
//		pbos 指向栈底的指针，保存在OSTCBStkBottom变元中
//		id 任务标志符(0..65535)，保存在.OSTCBId中
//		stk_size 堆栈的容量，保存在OS_TCB的OSTABStkSize中
//		pext OS_TCB中的扩展指针，.OSTCBExtPtr的值
//		opt OS_TCB的选择项，保存在.OSTCBOpt中
//返回: OS_NO_ERR 调用成功
//		OS_NO_MORE_TCB 没有更多的任务控制块被分配，将无法创建新的任务
//注意: 这个函数是uC/OS-II内部函数，你不可以调用它。  
INT8U  OS_TCBInit (INT8U prio, OS_STK *ptos, OS_STK *pbos, INT16U id, INT32U stk_size, void *pext, INT16U opt)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif    
    OS_TCB    *ptcb;


    OS_ENTER_CRITICAL();
    ptcb = OSTCBFreeList;                                  /* Get a free TCB from the free TCB list    */
    if (ptcb != (OS_TCB *)0) {
        OSTCBFreeList        = ptcb->OSTCBNext;            /* Update pointer to free TCB list          */
        OS_EXIT_CRITICAL();
        ptcb->OSTCBStkPtr    = ptos;                       /* Load Stack pointer in TCB                */
        ptcb->OSTCBPrio      = (INT8U)prio;                /* Load task priority into TCB              */
        ptcb->OSTCBStat      = OS_STAT_RDY;                /* Task is ready to run                     */
        ptcb->OSTCBDly       = 0;                          /* Task is not delayed                      */

#if OS_TASK_CREATE_EXT_EN > 0
        ptcb->OSTCBExtPtr    = pext;                       /* Store pointer to TCB extension           */
        ptcb->OSTCBStkSize   = stk_size;                   /* Store stack size                         */
        ptcb->OSTCBStkBottom = pbos;                       /* Store pointer to bottom of stack         */
        ptcb->OSTCBOpt       = opt;                        /* Store task options                       */
        ptcb->OSTCBId        = id;                         /* Store task ID                            */
#else
        pext                 = pext;                       /* Prevent compiler warning if not used     */
        stk_size             = stk_size;
        pbos                 = pbos;
        opt                  = opt;
        id                   = id;
#endif

#if OS_TASK_DEL_EN > 0
        ptcb->OSTCBDelReq    = OS_NO_ERR;
#endif

        ptcb->OSTCBY         = prio >> 3;                  /* Pre-compute X, Y, BitX and BitY          */
        ptcb->OSTCBBitY      = OSMapTbl[ptcb->OSTCBY];
        ptcb->OSTCBX         = prio & 0x07;
        ptcb->OSTCBBitX      = OSMapTbl[ptcb->OSTCBX];

#if OS_EVENT_EN > 0
        ptcb->OSTCBEventPtr  = (OS_EVENT *)0;              /* Task is not pending on an event          */
#endif

#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0) && (OS_TASK_DEL_EN > 0)
        ptcb->OSTCBFlagNode  = (OS_FLAG_NODE *)0;          /* Task is not pending on an event flag     */
#endif

#if (OS_MBOX_EN > 0) || ((OS_Q_EN > 0) && (OS_MAX_QS > 0))
        ptcb->OSTCBMsg       = (void *)0;                  /* No message received                      */
#endif

#if OS_VERSION >= 204
        OSTCBInitHook(ptcb);
#endif

        OSTaskCreateHook(ptcb);                            /* Call user defined hook                   */
        
        OS_ENTER_CRITICAL();
        OSTCBPrioTbl[prio] = ptcb;
        ptcb->OSTCBNext    = OSTCBList;                    /* Link into TCB chain                      */
        ptcb->OSTCBPrev    = (OS_TCB *)0;
        if (OSTCBList != (OS_TCB *)0) {
            OSTCBList->OSTCBPrev = ptcb;
        }
        OSTCBList               = ptcb;
        OSRdyGrp               |= ptcb->OSTCBBitY;         /* Make task ready to run                   */
        OSRdyTbl[ptcb->OSTCBY] |= ptcb->OSTCBBitX;
        OS_EXIT_CRITICAL();
        return (OS_NO_ERR);
    }
    OS_EXIT_CRITICAL();
    return (OS_NO_MORE_TCB);
}
