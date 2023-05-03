/*
*********************************************************************************************************
* uC/OS-II实时控制内核
* 主要的包含文件
* 文 件: uCOS_II.H ucos内部函数参数设定
* 作 者: Jean J. Labrosse
*********************************************************************************************************
*/
#include "includes.h"
#include "os_cpu.h"
#include "os_cfg.h"

//混杂的定义
#define  OS_VERSION              252       //定义UCOSII的版本号,这里是2.52版本

#ifdef   OS_GLOBALS						   //如果 OS_GLOBALS 已被声明定义, 紧随代码将会被编译
#define  OS_EXT							   //则定义 OS_EXT
#else
#define  OS_EXT  extern					   //否则，定义 OS_EXT 为 extern
#endif


//这里在stm32f10x_type.h里面用枚举的方式定义了，所以不需要再定义了
//#ifndef  FALSE						   //是否未定义 FALSE
//#define  FALSE                     0	   //如果没定义则定义 FALSE 为 0
//#endif
//
//#ifndef  TRUE							   //是否未定义 TRUE
//#define  TRUE                      1	   //如果没定义则定义 TRUE 为 1
//#endif

#define  OS_PRIO_SELF           0xFF       //定义 OS_PRIO_SELF 为 0xFF

#if OS_TASK_STAT_EN > 0
#define  OS_N_SYS_TASKS            2       //系统任务数为2   
#else
#define  OS_N_SYS_TASKS            1	   //系统任务数为1 
#endif

#define  OS_STAT_PRIO       (OS_LOWEST_PRIO - 1)        //统计任务优先级
#define  OS_IDLE_PRIO       (OS_LOWEST_PRIO)            //空闲任务优先级 

#define  OS_EVENT_TBL_SIZE ((OS_LOWEST_PRIO) / 8 + 1)   //事件列表大小
#define  OS_RDY_TBL_SIZE   ((OS_LOWEST_PRIO) / 8 + 1)   //就绪列表大小

#define  OS_TASK_IDLE_ID       65535                    //定义两个系统任务的ID
#define  OS_TASK_STAT_ID       65534

#define  OS_EVENT_EN       (((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0) || (OS_SEM_EN > 0) || (OS_MUTEX_EN > 0))

//*********************************************************************************************************
//任务状态字 TASK STATUS (OSTCBStat的位定义)       
#define  OS_STAT_RDY            0x00        // (将任务的状态字)处于就绪状态
#define  OS_STAT_SEM            0x01        // (将任务的状态字)处于信号量状态                               
#define  OS_STAT_MBOX           0x02        // (将任务的状态字)处于邮箱状态  
#define  OS_STAT_Q              0x04        // (将任务的状态字)处于Q状态
#define  OS_STAT_SUSPEND        0x08        // 表示任务被挂起
#define  OS_STAT_MUTEX          0x10        // (将任务的状态字)处于MUTEX状态
#define  OS_STAT_FLAG           0x20        // (将任务的状态字)处于FLAG状态

//*********************************************************************************************************
//事件类型(OS_EVENT types)
#define  OS_EVENT_TYPE_UNUSED      0        // 定义事件类型的种类(无事件类型------------0)
#define  OS_EVENT_TYPE_MBOX        1		// 定义事件类型的种类(邮箱为数字序列--------1)
#define  OS_EVENT_TYPE_Q           2		// 定义事件类型的种类(消息队列为数字序列----2)
#define  OS_EVENT_TYPE_SEM         3	    // 定义事件类型的种类(信号量为数字序列------3)
#define  OS_EVENT_TYPE_MUTEX       4	    // 定义事件类型的种类(互斥型信号量为数字序列4)
#define  OS_EVENT_TYPE_FLAG        5		// 定义事件类型的种类(事件标志组为数字序列--5)

//*********************************************************************************************************
//事件标志(EVENT FLAGS)
#define  OS_FLAG_WAIT_CLR_ALL      0        // 定义所有指定事件标志位清0 ------ 0
#define  OS_FLAG_WAIT_CLR_AND      0

#define  OS_FLAG_WAIT_CLR_ANY      1        // 定义任意指定事件标志位清0 ------ 1
#define  OS_FLAG_WAIT_CLR_OR       1

#define  OS_FLAG_WAIT_SET_ALL      2        // 定义所有指定事件标志位置1 ------ 2
#define  OS_FLAG_WAIT_SET_AND      2

#define  OS_FLAG_WAIT_SET_ANY      3        // 定义任意指定事件标志位置1 ------ 3
#define  OS_FLAG_WAIT_SET_OR       3

// 如果需要在得到期望标志后，恢复该事件标志，加入此常量
#define  OS_FLAG_CONSUME        0x80        // 定义常量OS_FLAG_CONSUME为0x80

											
#define  OS_FLAG_CLR               0		// 定义 OS_FLAG_CLR 为清0
#define  OS_FLAG_SET               1	    // 定义 OS_FLAG_SET 为置1

//*********************************************************************************************************
//设置字在'opt'中，适用于 OSSemDel(), OSMboxDel(), OSQDel() 和 OSMutexDel()函数
#define  OS_DEL_NO_PEND            0		// 可以选择只能在已经没有任何任务在等待该信号量时，才能删除该信号量
#define  OS_DEL_ALWAYS             1		// 不管有没有任务在等待该信号量，立即删除该信号量

//*********************************************************************************************************
//OS???PostOpt() OPTIONS(设置)
//这些设置适用用 OSMboxPostOpt() 和 OSQPostOpt()两个函数.	 
#define  OS_POST_OPT_NONE       0x00       	// 发送一个消息(或邮箱)给一个等待消息的优先级最高的任务
#define  OS_POST_OPT_BROADCAST  0x01        // 发送消息给所有等待队列消息的任务 
#define  OS_POST_OPT_FRONT      0x02        // 以后进先出方式发消息(仿真OSQPostFront())

//*********************************************************************************************************
//任务设置 TASK OPTIONS (查看OSTaskCreateExt())   
#define  OS_TASK_OPT_STK_CHK  0x0001        // 决定是否进行任务堆栈检查
#define  OS_TASK_OPT_STK_CLR  0x0002        // 决定是否清空堆栈
#define  OS_TASK_OPT_SAVE_FP  0x0004        // 决定是否保存浮点寄存器的数值。此项操作仅当处理器有浮点件时有效。保存操作由硬件相关的代码完成

//*********************************************************************************************************
//错误代码 ERROR CODES
#define OS_NO_ERR                 0			// 函数返回成功;

#define OS_ERR_EVENT_TYPE         1			// 不是指向事件(相关)类型的指针;
#define OS_ERR_PEND_ISR           2			// 在中断服务子程序中调用 OS各种信号类Accept()函数.
#define OS_ERR_POST_NULL_PTR      3			// 用户发出空指针。根据规则，这里不支持空指针;
#define OS_ERR_PEVENT_NULL        4			// 'pevent'是指空指针;
#define OS_ERR_POST_ISR           5			// 试图在中断服务子程序中调用OSMutexPost()函数[释放一个mutex];
#define OS_ERR_QUERY_ISR          6			// 试图在中断子程序中调用OSMutexQuery()[得到mutex当前状态信息]
#define OS_ERR_INVALID_OPT        7			// 定义的opt参数无效;
#define OS_ERR_TASK_WAITING       8			// 有一个或一个以上的任务在等待消息队列中的消息;

#define OS_TIMEOUT               10			// 消息没有在指定的周期数内送到;
#define OS_TASK_NOT_EXIST        11			// 指定的任务不存;

#define OS_MBOX_FULL             20			// 消息邮箱已经包含了其他消息，不空;

#define OS_Q_FULL                30			// 消息队列中已经存满;

#define OS_PRIO_EXIST            40			// 优先级为PIP的任务已经存在;
#define OS_PRIO_ERR              41			// 参数中的任务原先优先级不存在;
#define OS_PRIO_INVALID          42			// 参数指定的优先级大于OS_LOWEST_PRIO;

#define OS_SEM_OVF               50			// 信号量的值溢出;

#define OS_TASK_DEL_ERR          60			// 指定要删除的任务不存在
#define OS_TASK_DEL_IDLE         61			// 错误操作，试图删除空闲任务(Idle task);
#define OS_TASK_DEL_REQ          62			// 当前任务收到来自其他任务的删除请求;
#define OS_TASK_DEL_ISR          63			// 错误操作，试图在中断处理程序中删除任务;

#define OS_NO_MORE_TCB           70			// 系统中没有OS_TCB可以分配给任务了;

#define OS_TIME_NOT_DLY          80			// 要唤醒的任务不在延时状态;
#define OS_TIME_INVALID_MINUTES  81			// 参数错误，分钟数大于59;
#define OS_TIME_INVALID_SECONDS  82			// 参数错误，秒数大于59
#define OS_TIME_INVALID_MILLI    83			// 则返回参数错误，毫秒数大于999;
#define OS_TIME_ZERO_DLY         84		    // 四个参数全为0

#define OS_TASK_SUSPEND_PRIO     90			// 要挂起的任务不存在
#define OS_TASK_SUSPEND_IDLE     91			// 试图挂起uC/OS-II中的空闲任务(Idle task)

#define OS_TASK_RESUME_PRIO     100		    // 要唤醒的任务不存在;
#define OS_TASK_NOT_SUSPENDED   101			// 要唤醒的任务不在挂起状态

#define OS_MEM_INVALID_PART     110			// 没有空闲的内存区;
#define OS_MEM_INVALID_BLKS     111			// 没有为每一个内存区建立至少2个内存块;
#define OS_MEM_INVALID_SIZE     112			// 内存块大小不足以容纳一个指针变量;
#define OS_MEM_NO_FREE_BLKS     113			// 内存区已经没有空间分配给内存块;
#define OS_MEM_FULL             114			// 内存区已经不能再接受更多释放的内存块。这种情况说明用户程序出现
#define OS_MEM_INVALID_PBLK     115
#define OS_MEM_INVALID_PMEM     116		    // 'pmem'是空指针;
#define OS_MEM_INVALID_PDATA    117			// pdata是空指针;
#define OS_MEM_INVALID_ADDR     118			// 非法地址，即地址为空指针;

#define OS_ERR_NOT_MUTEX_OWNER  120			// 发出mutex的任务实际上并不占用mutex;

#define OS_TASK_OPT_ERR         130			// 任务用OSTaskCreateExt()函数建立的时候没有指定

#define OS_ERR_DEL_ISR          140			// 试图在中断服务子程序中删除(消息、邮箱、信号量、消息对列、互斥型信号量)
#define OS_ERR_CREATE_ISR       141			// 试图在中断服务子程序中建立(事件标志组、互斥型信号量);

#define OS_FLAG_INVALID_PGRP    150			// pgrp是一个空指针;
#define OS_FLAG_ERR_WAIT_TYPE   151			// 'wait_type'不是指定的参数之一;
#define OS_FLAG_ERR_NOT_RDY     152			// 指定的事件标志没有发生;
#define OS_FLAG_INVALID_OPT     153		    // opt不是指定的参数之一;
#define OS_FLAG_GRP_DEPLETED    154			// 系统没有剩余的空闲事件标志组，需要更改OS_CFG.H中的事件标志组数目配置

//*********************************************************************************************************
//事件控制块(EVENT CONTROL BLOCK)
#if (OS_EVENT_EN > 0) && (OS_MAX_EVENTS > 0)
typedef struct {						   // 定义一个事件控制块结构(OS_EVENT)
    INT8U   OSEventType;                   // 事件类型
    INT8U   OSEventGrp;                    // 等待任务所在的组
    INT16U  OSEventCnt;                    // 计数器(当事件是信号量时)
    void   *OSEventPtr;                    // 指向消息或者消息队列的指针
    INT8U   OSEventTbl[OS_EVENT_TBL_SIZE]; // 等待任务列表
} OS_EVENT;
#endif

//*********************************************************************************************************
//事件标志控制块 (EVENT FLAGS CONTROL BLOCK)
#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)//当版本为2.51 且 事件标志允许 且最大事件标志大于0时
typedef struct {                             // 定义一个OS_FLAG_GRP结构                                   
    INT8U         OSFlagType;                // 用来检验指针的类型是否是指向事件标志组的指针
    void         *OSFlagWaitList;            // 包含了一个等待事件的任务列表
    OS_FLAGS      OSFlagFlags;               // 包含了一系列表明当前事件标志状态的位               
} OS_FLAG_GRP;    							 // 事件标志组
											      
typedef struct {                            // 定义一个事件标志等待列表节点(OS_FLAG_NODE)结构
    void         *OSFlagNodeNext;           // 构建双向OS_FLAG_NODE数据结构链表的后一个链接
    void         *OSFlagNodePrev;           // 构建双向OS_FLAG_NODE数据结构链表的前一个链接
    void         *OSFlagNodeTCB;            // 指向某个等待事件标志组中的事件标志任务的控制块
    void         *OSFlagNodeFlagGrp;        // 是一个反向指回事件标志组的指针 
    OS_FLAGS      OSFlagNodeFlags;          // 用来指明任务等待事件标志组中的哪些事件标志
    INT8U         OSFlagNodeWaitType;       // 指明等待事件标志组中的所有事件标志的发生(与、或)
                                            /*      OS_FLAG_WAIT_AND                                   */
                                            /*      OS_FLAG_WAIT_ALL                                   */
                                            /*      OS_FLAG_WAIT_OR                                    */
                                            /*      OS_FLAG_WAIT_ANY                                   */
} OS_FLAG_NODE;
#endif


//*********************************************************************************************************
// 设定一个消息队列的数据结构 (MESSAGE MAILBOX DATA)
#if OS_MBOX_EN > 0
typedef struct {						   // 定义一个OS_MBOX_DATA结构
    void   *OSMsg;                         // 如果消息队列中有消息，它包含指针.OSQOut所指向的队列单元中
										   // 的内容。如果队列是空的，.OSMsg包含一个NULL指针
    INT8U   OSEventTbl[OS_EVENT_TBL_SIZE]; // 消息队列的等待任务列表
    INT8U   OSEventGrp;                    // 与OSEventTbl[]配合使用
} OS_MBOX_DATA;
#endif

//*********************************************************************************************************
//设定一个内存的数据结构 (MEMORY PARTITION DATA STRUCTURES)
#if (OS_MEM_EN > 0) && (OS_MAX_MEM_PART > 0)
typedef struct {                       // 使用内存控制块（memory control blocks）的数据结构来跟踪每一
									   // 个内存分区，系统中的每个内存分区都有它自己的内存控制块。
    void   *OSMemAddr;                 // 指向内存分区起始地址的指针
    void   *OSMemFreeList;             // 指向下一个空闲内存控制块或者下一个空闲的内存块的指针
    INT32U  OSMemBlkSize;              // 内存分区中内存块的大小，是用户建立该内存分区时指定的
    INT32U  OSMemNBlks;                // 内存分区中总的内存块数量，也是用户建立该内存分区时指定的
    INT32U  OSMemNFree;                // 内存分区中当前可以得空闲内存块数量
} OS_MEM;


typedef struct {
    void   *OSAddr;                    // 指向内存区起始地址的指针
    void   *OSFreeList;                /// 指向空闲内存块列表起始地址的指针
    INT32U  OSBlkSize;                 // 每个内存块的大小
    INT32U  OSNBlks;                   // 该内存区的内存块总数
    INT32U  OSNFree;                   // 空闲的内存块数目
    INT32U  OSNUsed;                   // 使用的内存块数目
} OS_MEM_DATA;
#endif

//*********************************************************************************************************
//互斥型信号量数据(MUTUAL EXCLUSION SEMAPHORE DATA)
#if OS_MUTEX_EN > 0	// 允许(1)或者产生互斥型信号量相关代码
typedef struct {	// 定义指向类型为(OS_MUTEX_DATA)的数据结构的指针
    INT8U   OSEventTbl[OS_EVENT_TBL_SIZE];  /* List of tasks waiting for event to occur                */
    INT8U   OSEventGrp;                     // 复制等待mutex的任务列表
    INT8U   OSValue;                        // 当前mutex的值.1表示可以使用，0表示不能使用
    INT8U   OSOwnerPrio;                    // 占用mutex任务的优先级
    INT8U   OSMutexPIP;                     // mutex的优先级继承优先级PIP
} OS_MUTEX_DATA;
#endif

//*********************************************************************************************************
// 消息队列数据 (MESSAGE QUEUE DATA)
/*
队列控制块是一个用于维护消息队列信息的数据结构，它包含了以下的一些域。这里，仍然在各个变量前加入
一个[.]来表示它们是数据结构中的一个域。
1).OSQPtr: 在空闲队列控制块中链接所有的队列控制块。一旦建立了消息队列，该域就不再有用了。
2).OSQStart: 是指向消息队列的指针数组的起始地址的指针。用户应用程序在使用消息队列之前必须先定义该数组
。
3).OSQEnd: 是指向消息队列结束单元的下一个地址的指针。该指针使得消息队列构成一个循环的缓冲区。
4).OSQIn: 是指向消息队列中插入下一条消息的位置的指针。当.OSQIn和.OSQEnd相等时，.OSQIn被调整指向
消息队列的起始单元。
5).OSQOut: 是指向消息队列中下一个取出消息的位置的指针。当.OSQOut和.OSQEnd相等时，.OSQOut被调整指
向消息队列的起始单元。
6).OSQSize: 是消息队列中总的单元数。该值是在建立消息队列时由用户应用程序决定的。在uC/OS-II中,该值最
大可以是65,535。
7).OSQEntries: 是消息队列中当前的消息数量。当消息队列是空的时，该值为0。当消息队列满了以后，该值和
.OSQSize值一样。 在消息队列刚刚建立时，该值为0。
*/

#if OS_Q_EN > 0
typedef struct os_q {                   // 定义一个OS_Q队列控制块                                    */
    struct os_q   *OSQPtr;              /* Link to next queue control block in list of free blocks     */
    void         **OSQStart;            /* Pointer to start of queue data                              */
    void         **OSQEnd;              /* Pointer to end   of queue data                              */
    void         **OSQIn;               /* Pointer to where next message will be inserted  in   the Q  */
    void         **OSQOut;              /* Pointer to where next message will be extracted from the Q  */
    INT16U         OSQSize;             /* Size of queue (maximum number of entries)                   */
    INT16U         OSQEntries;          /* Current number of entries in the queue                      */
} OS_Q;								 

typedef struct {						// 定义一个消息队列数据(OS_Q_DATA)结构
    void          *OSMsg;               // 如果消息队列中有消息，它包含指针
    INT16U         OSNMsgs;             // 消息队列中的消息数
    INT16U         OSQSize;             // 消息队列的总的容量                                      */
    INT8U          OSEventTbl[OS_EVENT_TBL_SIZE];  /* List of tasks waiting for event to occur         */
    INT8U          OSEventGrp;          //和OSEventTbl[]一起结合，是消息队列的等待任务列表
} OS_Q_DATA;
#endif

//*********************************************************************************************************
//信号量数据结构 (SEMAPHORE DATA)
#if OS_SEM_EN > 0
typedef struct {						    // 定义一个信号量数据结构(OS_SEM_DATA)
    INT16U  OSCnt;                          // 定义信号量计数值                                      
    INT8U   OSEventTbl[OS_EVENT_TBL_SIZE];  // 定义任务等待列表
    INT8U   OSEventGrp;                     // 定义等待事件的任务组
} OS_SEM_DATA;
#endif

//*********************************************************************************************************
//任务堆栈数据 (TASK STACK DATA)
#if OS_TASK_CREATE_EXT_EN > 0
typedef struct {					   // 定义一个堆栈数据结构(OS_STK_DATA)
    INT32U  OSFree;                    // 堆栈中未使用的字节数
    INT32U  OSUsed;                    // 堆栈中已使用的字节数                
} OS_STK_DATA;
#endif

//*********************************************************************************************************
//任务控制块 (TASK CONTROL BLOCK)
typedef struct os_tcb {
    OS_STK        *OSTCBStkPtr;        //当前TCB的栈顶指针

#if OS_TASK_CREATE_EXT_EN > 0		   //允许生成OSTaskCreateExt()函数
    void          *OSTCBExtPtr;        //指向用户定义的任务控制块(扩展指针)
    OS_STK        *OSTCBStkBottom;     //指向指向栈底的指针
    INT32U         OSTCBStkSize;       //设定堆栈的容量
    INT16U         OSTCBOpt;           //保存OS_TCB的选择项
    INT16U         OSTCBId;            //任务ID  (0..65535)             
#endif

    struct os_tcb *OSTCBNext;          //定义指向TCB的双向链接的后链接
    struct os_tcb *OSTCBPrev;          //定义指向TCB的双向链接的前链接

#if ((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0) || (OS_SEM_EN > 0) || (OS_MUTEX_EN > 0)
//当以上各种事件允许时
    OS_EVENT      *OSTCBEventPtr;      //定义指向事件控制块的指针
#endif

#if ((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0)
//满足以上条件,定义传递给任务的消息指针
    void          *OSTCBMsg;           /* Message received from OSMboxPost() or OSQPost()              */
#endif

#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
#if OS_TASK_DEL_EN > 0
    OS_FLAG_NODE  *OSTCBFlagNode;      ///定义事件标志节点的指针
#endif    
    OS_FLAGS       OSTCBFlagsRdy;      //定义运行准备完毕的任务控制块中的任务?
#endif

    INT16U         OSTCBDly;           //定义允许任务等待时的最多节拍数
    INT8U          OSTCBStat;          //定义任务的状态字        
    INT8U          OSTCBPrio;          //定义任务的优先级(0 == highest, 63 == lowest)                 

    INT8U          OSTCBX;             //定义指向任务优先级的低3位，即=priority&0x07
    INT8U          OSTCBY;             //定义指向任务优先级的高3位，即=priority>>3
    INT8U          OSTCBBitX;          //定义低3位就绪表对应值（0～7），即＝OSMapTbl[priority&0x07]
    INT8U          OSTCBBitY;          //定义高3位就绪表对应值（0～7），即＝OSMapTbl[priority>>3]

#if OS_TASK_DEL_EN > 0				   //允许生成 OSTaskDel() 函数代码函数
    BOOLEAN        OSTCBDelReq;        //定义用于表示该任务是否须删除
#endif
} OS_TCB;

//*********************************************************************************************************
//全局变量 (GLOBAL VARIABLES)
OS_EXT  INT32U            OSCtxSwCtr;               //上下文切换的次数(统计任务计数器)

#if (OS_EVENT_EN > 0) && (OS_MAX_EVENTS > 0)	    //如果有消息事件，并且最大消息事件数>0
OS_EXT  OS_EVENT         *OSEventFreeList;          //空余事件管理列表指针
OS_EXT  OS_EVENT          OSEventTbl[OS_MAX_EVENTS];//事件控制块表
#endif
//当满足版本大于2.51且事件标志允许且有最大事件标志
#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
OS_EXT  OS_FLAG_GRP       OSFlagTbl[OS_MAX_FLAGS];  //定义一个事件标志列表
OS_EXT  OS_FLAG_GRP      *OSFlagFreeList;           //定义一个空闲的事件标志组
#endif

#if OS_TASK_STAT_EN > 0							    //定义允许生产OS_TaskStat()函数
OS_EXT  INT8S             OSCPUUsage;               //定义CPU 使用率                        
OS_EXT  INT32U            OSIdleCtrMax;             //定义最大空闲计数值
OS_EXT  INT32U            OSIdleCtrRun;             //定义当前的空闲计数值
OS_EXT  BOOLEAN           OSStatRdy;                //定义统计任务就绪标志
OS_EXT  OS_STK            OSTaskStatStk[OS_TASK_STAT_STK_SIZE];/* Statistics task stack          */
#endif

OS_EXT  INT8U             OSIntNesting;             //定义中断嵌套层数
OS_EXT  INT8U             OSIntExitY;				//用于函数OSInieExt( )

OS_EXT  INT8U             OSLockNesting;            //定义锁定嵌套计数器

OS_EXT  INT8U             OSPrioCur;                //定义正在运行的任务的优先级
OS_EXT  INT8U             OSPrioHighRdy;            //定义具有最高优先级别的就绪任务的优先级

OS_EXT  INT8U             OSRdyGrp;                        /* Ready list group                         */
OS_EXT  INT8U             OSRdyTbl[OS_RDY_TBL_SIZE];       //即将要运行的任务列表

OS_EXT  BOOLEAN           OSRunning;                       //多任务已经开始=1,任务处于不运行状态=0

OS_EXT  INT8U             OSTaskCtr;                       //定义任务计数器

OS_EXT  volatile  INT32U  OSIdleCtr;                        //定义32位空闲任务的计数器

OS_EXT  OS_STK            OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE];//分配空闲任务堆栈栈顶指针


OS_EXT  OS_TCB           *OSTCBCur;                        //定义指向正在运行任务控制块的指针
OS_EXT  OS_TCB           *OSTCBFreeList;                   //定义空任务控制块指针  
OS_EXT  OS_TCB           *OSTCBHighRdy;                    //定义指向最高级优先级就绪任务控制块的指针
OS_EXT  OS_TCB           *OSTCBList;                       //定义任务控制块列表首地址
OS_EXT  OS_TCB           *OSTCBPrioTbl[OS_LOWEST_PRIO + 1];//定义任务控制块优先级表
OS_EXT  OS_TCB            OSTCBTbl[OS_MAX_TASKS + OS_N_SYS_TASKS];//定义当前任务控制块列表

//条件编译：若两个条件满足时，产生以下代码
//OS_MEM_EN允许 (1) 或者禁止 (0) 产生内存相关代码
//OS_MAX_MEM_PART 最多内存块的数目
#if (OS_MEM_EN > 0) && (OS_MAX_MEM_PART > 0)
OS_EXT  OS_MEM           *OSMemFreeList;            //空余内存控制块(链接)指针
OS_EXT  OS_MEM            OSMemTbl[OS_MAX_MEM_PART];/* Storage for memory partition manager            */
#endif

#if (OS_Q_EN > 0) && (OS_MAX_QS > 0)				//条件编译：OS_Q_EN 允许 (1)产生消息队列相关代码
//条件编译：应用中最多对列控制块的数目 > 0			
OS_EXT  OS_Q             *OSQFreeList;              /* Pointer to list of free QUEUE control blocks    */
OS_EXT  OS_Q              OSQTbl[OS_MAX_QS];        /* Table of QUEUE control blocks                   */
#endif

#if OS_TIME_GET_SET_EN > 0   					    //允许生成OSTimeGet() 函数代码	 
OS_EXT  volatile  INT32U  OSTime;                   //当前系统时钟数值(in ticks)        
#endif

extern  INT8U  const      OSMapTbl[];               /* Priority->Bit Mask lookup table                 */
extern  INT8U  const      OSUnMapTbl[];             /* Priority->Index    lookup table                 */

//*********************************************************************************************************
//功能原型 (FUNCTION PROTOTYPES)
//不受约束的函数 (Target Independent Functions)
//事件标志管理 (EVENT FLAGS MANAGEMENT)
//OSFlagAccept() 检查事件标志组函数(标志组的指针、事件标志位、等待事件标志位的方式、错误码指针)
//OSFlagCreate() 建立一个事件标志组(初值、错误码)
//OSFlagDel() 删除一个事件标志组(指针、条件值、错误值)
//OSFlagPend() 等待事件标志组的事件标志位(事件组指针、需要检查的标志位、等待事件标志位的方式、
//允许等待的时钟节拍、出错代码的时钟节拍)
//OSFlagPost() 置位或清0事件标志组中的标志位(指针、标志位、条件值、错误码)
//OSFlagQuery() 查询事件标志组的当前事件标志状态(事件标志组的指针、错误代码的指针)

#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)

#if OS_FLAG_ACCEPT_EN > 0
OS_FLAGS      OSFlagAccept(OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U wait_type, INT8U *err);
#endif

OS_FLAG_GRP  *OSFlagCreate(OS_FLAGS flags, INT8U *err);

#if OS_FLAG_DEL_EN > 0
OS_FLAG_GRP  *OSFlagDel(OS_FLAG_GRP *pgrp, INT8U opt, INT8U *err);
#endif

OS_FLAGS      OSFlagPend(OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U wait_type, INT16U timeout, INT8U *err);
OS_FLAGS      OSFlagPost(OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U operation, INT8U *err);

#if OS_FLAG_QUERY_EN > 0
OS_FLAGS      OSFlagQuery(OS_FLAG_GRP *pgrp, INT8U *err);
#endif
#endif

//*********************************************************************************************************
//消息邮箱管理 (MESSAGE MAILBOX MANAGEMENT)
//OSMboxAccept () 查看消息邮箱(消息邮箱指针)
//OSMboxCreate () 建立并初始化一个消息邮箱(msg 参数不为空含内容)
//OSMboxDel () 删除消息邮箱(消息邮箱指针、删除条件、出错代码指针)
//OSMboxPend () 等待一个消息邮箱函数(消息邮箱指针、允许等待的时钟节拍、代码错误指针)
//OSMboxPost () 发送消息函数(消息邮箱指针、即将实际发送给任务的消息)
//OSMboxPostOpt () 向邮箱发送一则消息(邮箱指针、消息、条件)
//OSMboxQuery () 查询一个邮箱的当前状态(信号量指针、状态数据结构指针)


#if OS_MBOX_EN > 0

#if OS_MBOX_ACCEPT_EN > 0
void         *OSMboxAccept(OS_EVENT *pevent);
#endif

OS_EVENT     *OSMboxCreate(void *msg);

#if OS_MBOX_DEL_EN > 0
OS_EVENT     *OSMboxDel(OS_EVENT *pevent, INT8U opt, INT8U *err);
#endif

void         *OSMboxPend(OS_EVENT *pevent, INT16U timeout, INT8U *err);

#if OS_MBOX_POST_EN > 0
INT8U         OSMboxPost(OS_EVENT *pevent, void *msg);
#endif

#if OS_MBOX_POST_OPT_EN > 0
INT8U         OSMboxPostOpt(OS_EVENT *pevent, void *msg, INT8U opt);
#endif

#if OS_MBOX_QUERY_EN > 0
INT8U         OSMboxQuery(OS_EVENT *pevent, OS_MBOX_DATA *pdata);
#endif
#endif

//*********************************************************************************************************
//内存管理项 (MEMORY MANAGEMENT)	  
//OSMemCreate () 建立并初始化一块内存区(起始地址、需要的内存块数目、内存块大小、返回错误的指针)
//OSMemGet () 从内存区分配一个内存块
//OSMemPut () 释放一个内存块，内存块必须释放回原先申请的内存区
//OSMemQuery () 得到内存区的信息


#if (OS_MEM_EN > 0) && (OS_MAX_MEM_PART > 0)

OS_MEM       *OSMemCreate(void *addr, INT32U nblks, INT32U blksize, INT8U *err);
void         *OSMemGet(OS_MEM *pmem, INT8U *err);
INT8U         OSMemPut(OS_MEM *pmem, void *pblk);

#if OS_MEM_QUERY_EN > 0
INT8U         OSMemQuery(OS_MEM *pmem, OS_MEM_DATA *pdata);
#endif

#endif

//*********************************************************************************************************
//互斥型信号量项管理 (MUTUAL EXCLUSION SEMAPHORE MANAGEMENT) 
//OSMutexAccept () 无等待地获取互斥型信号量[任务不挂起](信号量指针、错误代码)
//OSMutexCreate () 建立并初始化一个互斥型信号量(优先级继承优先级(PIP)、出错代码指针)
//OSMutexDel () 删除互斥型信号量(信号指针、删除条件、错误指针)
//OSMutexPend () 等待一个互斥型信号量(指针、等待超时时限、出错代码指针)
//OSMutexPost () 释放一个互斥型信号量(互斥型信号量指针)
//OSMutexQuery () 查询一个互斥型信号量的当前状态(互斥型信号量指针、状态数据结构指针)


#if OS_MUTEX_EN > 0

#if OS_MUTEX_ACCEPT_EN > 0
INT8U         OSMutexAccept(OS_EVENT *pevent, INT8U *err);
#endif

OS_EVENT     *OSMutexCreate(INT8U prio, INT8U *err);

#if OS_MUTEX_DEL_EN > 0
OS_EVENT     *OSMutexDel(OS_EVENT *pevent, INT8U opt, INT8U *err);
#endif

void          OSMutexPend(OS_EVENT *pevent, INT16U timeout, INT8U *err);
INT8U         OSMutexPost(OS_EVENT *pevent);

#if OS_MUTEX_QUERY_EN > 0
INT8U         OSMutexQuery(OS_EVENT *pevent, OS_MUTEX_DATA *pdata);
#endif

#endif

//*********************************************************************************************************
//消息队列管理 (MESSAGE QUEUE MANAGEMENT)	  
//OSQAccept () 检查消息队列中是否已经有需要的消息(消息队列的指针)
//OSQCreate () 建立一个消息队列(消息内存区的基地址(指针数组)、消息内存区的大小)
//OSQDel () 删除一个消息队列(消息队列指针、删除条件、错误指针)
//OSQFlush () 清空消息队列(指向得到消息队列的指针)
//OSQPend () 任务等待消息队列中的消息(消息队列指针、允许等待的时钟节拍、代码错误指针)
//OSQPost () 向消息队列发送一则消息FIFO(消息队列指针、发送的消息)
//OSQPostFront () 向消息队列发送一则消息LIFO(消息队列指针、发送的消息)
//OSQPostOpt () 向消息队列发送一则消息LIFO(消息队列指针、发送的消息、发送条件)
//OSQQuery () 查询一个消息队列的当前状态(信号量指针、状态数据结构指针)

#if (OS_Q_EN > 0) && (OS_MAX_QS > 0)

#if OS_Q_ACCEPT_EN > 0
void         *OSQAccept(OS_EVENT *pevent);
#endif

OS_EVENT     *OSQCreate(void **start, INT16U size);

#if OS_Q_DEL_EN > 0
OS_EVENT     *OSQDel(OS_EVENT *pevent, INT8U opt, INT8U *err);
#endif

#if OS_Q_FLUSH_EN > 0
INT8U         OSQFlush(OS_EVENT *pevent);
#endif

void         *OSQPend(OS_EVENT *pevent, INT16U timeout, INT8U *err);

#if OS_Q_POST_EN > 0
INT8U         OSQPost(OS_EVENT *pevent, void *msg);
#endif

#if OS_Q_POST_FRONT_EN > 0
INT8U         OSQPostFront(OS_EVENT *pevent, void *msg);
#endif

#if OS_Q_POST_OPT_EN > 0
INT8U         OSQPostOpt(OS_EVENT *pevent, void *msg, INT8U opt);
#endif

#if OS_Q_QUERY_EN > 0
INT8U         OSQQuery(OS_EVENT *pevent, OS_Q_DATA *pdata);
#endif

#endif

//*********************************************************************************************************
//信号量管理 (SEMAPHORE MANAGEMENT)	    
//OSSemAccept() 无条件地等待请求一个信号量函数
//OSSemCreate() 建立并初始化一个信号量(输入一个信号量值)
//OSSemDel() 删除一个信号量(信号指针、删除条件、错误指针)
//OSSemPend () 等待一个信号量函数(信号量指针、允许等待的时钟节拍、代码错误指针)
//OSSemPost () 发出一个信号量函数(信号量指针)
//OSSemQuery () 查询一个信号量的当前状态(信号量指针、状态数据结构指针)

#if OS_SEM_EN > 0

#if OS_SEM_ACCEPT_EN > 0
INT16U        OSSemAccept(OS_EVENT *pevent);
#endif

OS_EVENT     *OSSemCreate(INT16U cnt);

#if OS_SEM_DEL_EN > 0
OS_EVENT     *OSSemDel(OS_EVENT *pevent, INT8U opt, INT8U *err);
#endif

void          OSSemPend(OS_EVENT *pevent, INT16U timeout, INT8U *err);
INT8U         OSSemPost(OS_EVENT *pevent);

#if OS_SEM_QUERY_EN > 0
INT8U         OSSemQuery(OS_EVENT *pevent, OS_SEM_DATA *pdata);
#endif

#endif
	   
//*********************************************************************************************************
//任务管理 (TASK MANAGEMENT)    
//OSTaskChangePrio () 改变一个任务的优先级(任务旧的优先级、任务新的优先级)
//OSTaskCreate () 建立任务(任务代码指针、传递参数指针、分配任务堆栈栈顶指针、任务优先级)
//OSTaskCreateExt () 建立扩展任务(任务代码指针/传递参数指针/分配任务堆栈栈顶指针/分配任务优先级
//(未来的)优先级标识(与优先级相同)/分配任务堆栈栈底指针/指定堆栈的容量(检验用)
//指向用户附加的数据域的指针/建立任务设定选项)
//OSTaskDel () 删除任务(任务的优先级)
//OSTaskDelReq () 请求一个任务删除其它任务或自身?(任务的优先级)
//OSTaskResume () 唤醒一个用OSTaskSuspend()函数挂起的任务(任务的优先级)
//OSTaskStkChk () 检查任务堆栈状态(任务优先级、检验堆栈数据结构)
//OSTaskSuspend () 无条件挂起一个任务(任务优先级)
//OSTaskQuery () 获取任务信息(任务指针、保存数据结构指针)

#if OS_TASK_CHANGE_PRIO_EN > 0
INT8U         OSTaskChangePrio(INT8U oldprio, INT8U newprio);
#endif

#if OS_TASK_CREATE_EN > 0
INT8U         OSTaskCreate(void (*task)(void *pd), void *pdata, OS_STK *ptos, INT8U prio);
#endif

#if OS_TASK_CREATE_EXT_EN > 0
INT8U         OSTaskCreateExt(void  (*task)(void *pd),
                              void   *pdata,
                              OS_STK *ptos,
                              INT8U   prio,
                              INT16U  id,
                              OS_STK *pbos,
                              INT32U  stk_size,
                              void   *pext,
                              INT16U  opt);
#endif

#if OS_TASK_DEL_EN > 0
INT8U         OSTaskDel(INT8U prio);
INT8U         OSTaskDelReq(INT8U prio);
#endif

#if OS_TASK_SUSPEND_EN > 0
INT8U         OSTaskResume(INT8U prio);
INT8U         OSTaskSuspend(INT8U prio);
#endif

#if OS_TASK_CREATE_EXT_EN > 0
INT8U         OSTaskStkChk(INT8U prio, OS_STK_DATA *pdata);
#endif

#if OS_TASK_QUERY_EN > 0
INT8U         OSTaskQuery(INT8U prio, OS_TCB *pdata);
#endif

//*********************************************************************************************************
//时钟管理项 (TIME MANAGEMENT)  
//OSTimeDly () 任务延时函数(时钟节拍数)
//OSTimeDlyHMSM () 将一个任务延时若干时间(设定时、分、秒、毫秒)
//OSTimeDlyResume () 唤醒一个用OSTimeDly()或OSTimeDlyHMSM()函数的任务(优先级)
//OSTimeGet () 获取当前系统时钟数值
//OSTimeSet () 设置当前系统时钟数值	 

void          OSTimeDly(INT16U ticks);

#if OS_TIME_DLY_HMSM_EN > 0
INT8U         OSTimeDlyHMSM(INT8U hours, INT8U minutes, INT8U seconds, INT16U milli);
#endif

#if OS_TIME_DLY_RESUME_EN > 0
INT8U         OSTimeDlyResume(INT8U prio);
#endif

#if OS_TIME_GET_SET_EN > 0
INT32U        OSTimeGet(void);
void          OSTimeSet(INT32U ticks);
#endif

void          OSTimeTick(void);

//*********************************************************************************************************
//混杂函数定义	 
//OSInit() 初始化UCOS-II函数
//OSIntEnter() 中断函数正在执行
//OSIntExit() 中断函数已经完成(脱离中断)
//OSSchedLock() 给调度器上锁
//OSSchedUnlock() 给调度器解锁
//OSStart() 启动多个任务
//OSStatInit() 统计任务初始化
//OSVersion() 获得版本号

void          OSInit(void);

void          OSIntEnter(void);
void          OSIntExit(void);

#if OS_SCHED_LOCK_EN > 0
void          OSSchedLock(void);
void          OSSchedUnlock(void);
#endif

void          OSStart(void);

void          OSStatInit(void);

INT16U        OSVersion(void);

//*********************************************************************************************************
//内部函数原型 INTERNAL FUNCTION PROTOTYPES
//你在应用程序中不能使用它们 (Your application MUST NOT call these functions)
 
//OS_Dummy() 建立一个虚拟函数
//OS_EventTaskRdy() 使一个任务进入就绪态(OS_EVENT *pevent, void *msg, INT8U msk)
//OS_EventTaskWait() 使一个任务进入等待某事件发生状态(ECB指针)
//OS_EventTO() 由于超时而将任务置为就绪态(ECB指针)
//OS_EventWaitListInit()事件控制块列表初始化(事件控制块指针)
//OS_FlagInit() 初始化事件标志结构
//OS_FlagUnlink() 把这个OS_FLAG_NODE从事件标志组的等待任务链表中删除(OS_FLAG_NODE *pnode)
//OS_MemInit() 初始化内存分区
//OS_QInit() 初始化事件队列结构
//OS_Sched() 任务调度函数
//OS_TaskIdle() 空闲任务函数(指向一个数据结构)
//OS_TaskStat() 统计任务(指向一个数据结构)
//OS_TCBInit() 初始化任务控制块TCB(优先级指针、栈顶指针、栈底指针、任务标志符、
//堆栈容量、扩展指针、选择项)


#if OS_TASK_DEL_EN > 0
void          OS_Dummy(void);
#endif

#if ((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0) || (OS_SEM_EN > 0) || (OS_MUTEX_EN > 0)
INT8U         OS_EventTaskRdy(OS_EVENT *pevent, void *msg, INT8U msk);
void          OS_EventTaskWait(OS_EVENT *pevent);
void          OS_EventTO(OS_EVENT *pevent);
void          OS_EventWaitListInit(OS_EVENT *pevent);
#endif

#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
void          OS_FlagInit(void);
void          OS_FlagUnlink(OS_FLAG_NODE *pnode);
#endif

#if (OS_MEM_EN > 0) && (OS_MAX_MEM_PART > 0)
void          OS_MemInit(void);
#endif

#if OS_Q_EN > 0
void          OS_QInit(void);
#endif

void          OS_Sched(void);

void          OS_TaskIdle(void *data);

#if OS_TASK_STAT_EN > 0
void          OS_TaskStat(void *data);
#endif

INT8U         OS_TCBInit(INT8U prio, OS_STK *ptos, OS_STK *pbos, INT16U id, INT32U stk_size, void *pext, INT16U opt);

//*********************************************************************************************************
//各类钩子程序函数定义项 (FUNCTION PROTOTYPES)
//特别钩子函数原型 (Target Specific Functions)
 

#if OS_VERSION >= 204
void          OSInitHookBegin(void);
void          OSInitHookEnd(void);
#endif

void          OSIntCtxSw(void);

void          OSStartHighRdy(void);

void          OSTaskCreateHook(OS_TCB *ptcb);
void          OSTaskDelHook(OS_TCB *ptcb);

#if OS_VERSION >= 251
void          OSTaskIdleHook(void);
#endif

void          OSTaskStatHook(void);
OS_STK       *OSTaskStkInit(void (*task)(void *pd), void *pdata, OS_STK *ptos, INT16U opt);
void          OSTaskSwHook(void);

#if OS_VERSION >= 204
void          OSTCBInitHook(OS_TCB *ptcb);
#endif

void          OSTimeTickHook(void);

//*********************************************************************************************************
//函数原型 FUNCTION PROTOTYPES
//特殊中断函数原型 (Compiler Specific ISR prototypes)


#ifndef OS_ISR_PROTO_EXT
void          OSCtxSw(void);
void          OSTickISR(void);
#endif

/*
*********************************************************************************************************
*                                   LOOK FOR MISSING #define CONSTANTS
*
* This section is used to generate ERROR messages at compile time if certain #define constants are 
* MISSING in OS_CFG.H.  This allows you to quickly determine the source of the error.
*
* You SHOULD NOT change this section UNLESS you would like to add more comments as to the source of the
* compile time error.
*********************************************************************************************************
*/

//*********************************************************************************************************
//事件标志管理条件编译

#ifndef OS_FLAG_EN
#error  "OS_CFG.H, Missing OS_FLAG_EN: Enable (1) or Disable (0) code generation for Event Flags"
#else
    #ifndef OS_MAX_FLAGS
    #error  "OS_CFG.H, Missing OS_MAX_FLAGS: Max. number of Event Flag Groups in your application"
    #else
        #if     OS_MAX_FLAGS == 0
        #error  "OS_CFG.H, OS_MAX_FLAGS must be > 0"
        #endif
        #if     OS_MAX_FLAGS > 255
        #error  "OS_CFG.H, OS_MAX_FLAGS must be <= 255"
        #endif
    #endif

    #ifndef OS_FLAG_WAIT_CLR_EN
    #error  "OS_CFG.H, Missing OS_FLAG_WAIT_CLR_EN: Include code for Wait on Clear EVENT FLAGS"
    #endif

    #ifndef OS_FLAG_ACCEPT_EN
    #error  "OS_CFG.H, Missing OS_FLAG_ACCEPT_EN: Include code for OSFlagAccept()"
    #endif

    #ifndef OS_FLAG_DEL_EN
    #error  "OS_CFG.H, Missing OS_FLAG_DEL_EN: Include code for OSFlagDel()"
    #endif

    #ifndef OS_FLAG_QUERY_EN
    #error  "OS_CFG.H, Missing OS_FLAG_QUERY_EN: Include code for OSFlagQuery()"
    #endif
#endif

//*********************************************************************************************************
//消息邮箱管理条件编译

#ifndef OS_MBOX_EN
#error  "OS_CFG.H, Missing OS_MBOX_EN: Enable (1) or Disable (0) code generation for MAILBOXES"
#else
    #ifndef OS_MBOX_ACCEPT_EN
    #error  "OS_CFG.H, Missing OS_MBOX_ACCEPT_EN: Include code for OSMboxAccept()"
    #endif

    #ifndef OS_MBOX_DEL_EN
    #error  "OS_CFG.H, Missing OS_MBOX_DEL_EN: Include code for OSMboxDel()"
    #endif

    #ifndef OS_MBOX_POST_EN
    #error  "OS_CFG.H, Missing OS_MBOX_POST_EN: Include code for OSMboxPost()"
    #endif

    #ifndef OS_MBOX_POST_OPT_EN
    #error  "OS_CFG.H, Missing OS_MBOX_POST_OPT_EN: Include code for OSMboxPostOpt()"
    #endif

    #ifndef OS_MBOX_QUERY_EN
    #error  "OS_CFG.H, Missing OS_MBOX_QUERY_EN: Include code for OSMboxQuery()"
    #endif
#endif

//*********************************************************************************************************
//内存管理条件编译

#ifndef OS_MEM_EN
#error  "OS_CFG.H, Missing OS_MEM_EN: Enable (1) or Disable (0) code generation for MEMORY MANAGER"
#else
    #ifndef OS_MAX_MEM_PART
    #error  "OS_CFG.H, Missing OS_MAX_MEM_PART: Max. number of memory partitions"
    #else
        #if     OS_MAX_MEM_PART == 0
        #error  "OS_CFG.H, OS_MAX_MEM_PART must be > 0"
        #endif
        #if     OS_MAX_MEM_PART > 255
        #error  "OS_CFG.H, OS_MAX_MEM_PART must be <= 255"
        #endif
    #endif

    #ifndef OS_MEM_QUERY_EN
    #error  "OS_CFG.H, Missing OS_MEM_QUERY_EN: Include code for OSMemQuery()"
    #endif
#endif

//*********************************************************************************************************
//互斥型信号量管理条件编译

#ifndef OS_MUTEX_EN
#error  "OS_CFG.H, Missing OS_MUTEX_EN: Enable (1) or Disable (0) code generation for MUTEX"
#else
    #ifndef OS_MUTEX_ACCEPT_EN
    #error  "OS_CFG.H, Missing OS_MUTEX_ACCEPT_EN: Include code for OSMutexAccept()"
    #endif

    #ifndef OS_MUTEX_DEL_EN
    #error  "OS_CFG.H, Missing OS_MUTEX_DEL_EN: Include code for OSMutexDel()"
    #endif

    #ifndef OS_MUTEX_QUERY_EN
    #error  "OS_CFG.H, Missing OS_MUTEX_QUERY_EN: Include code for OSMutexQuery()"
    #endif
#endif

//*********************************************************************************************************
//消息队列条件编译

#ifndef OS_Q_EN
#error  "OS_CFG.H, Missing OS_Q_EN: Enable (1) or Disable (0) code generation for QUEUES"
#else
    #ifndef OS_MAX_QS
    #error  "OS_CFG.H, Missing OS_MAX_QS: Max. number of queue control blocks"
    #else
        #if     OS_MAX_QS == 0
        #error  "OS_CFG.H, OS_MAX_QS must be > 0"
        #endif
        #if     OS_MAX_QS > 255
        #error  "OS_CFG.H, OS_MAX_QS must be <= 255"
        #endif
    #endif

    #ifndef OS_Q_ACCEPT_EN
    #error  "OS_CFG.H, Missing OS_Q_ACCEPT_EN: Include code for OSQAccept()"
    #endif

    #ifndef OS_Q_DEL_EN
    #error  "OS_CFG.H, Missing OS_Q_DEL_EN: Include code for OSQDel()"
    #endif

    #ifndef OS_Q_FLUSH_EN
    #error  "OS_CFG.H, Missing OS_Q_FLUSH_EN: Include code for OSQFlush()"
    #endif

    #ifndef OS_Q_POST_EN
    #error  "OS_CFG.H, Missing OS_Q_POST_EN: Include code for OSQPost()"
    #endif

    #ifndef OS_Q_POST_FRONT_EN
    #error  "OS_CFG.H, Missing OS_Q_POST_FRONT_EN: Include code for OSQPostFront()"
    #endif

    #ifndef OS_Q_POST_OPT_EN
    #error  "OS_CFG.H, Missing OS_Q_POST_OPT_EN: Include code for OSQPostOpt()"
    #endif

    #ifndef OS_Q_QUERY_EN
    #error  "OS_CFG.H, Missing OS_Q_QUERY_EN: Include code for OSQQuery()"
    #endif
#endif

//*********************************************************************************************************
//信号量条件编译

#ifndef OS_SEM_EN
#error  "OS_CFG.H, Missing OS_SEM_EN: Enable (1) or Disable (0) code generation for SEMAPHORES"
#else
    #ifndef OS_SEM_ACCEPT_EN
    #error  "OS_CFG.H, Missing OS_SEM_ACCEPT_EN: Include code for OSSemAccept()"
    #endif

    #ifndef OS_SEM_DEL_EN
    #error  "OS_CFG.H, Missing OS_SEM_DEL_EN: Include code for OSSemDel()"
    #endif

    #ifndef OS_SEM_QUERY_EN
    #error  "OS_CFG.H, Missing OS_SEM_QUERY_EN: Include code for OSSemQuery()"
    #endif
#endif

//*********************************************************************************************************
//任务管理条件编译

#ifndef OS_MAX_TASKS
#error  "OS_CFG.H, Missing OS_MAX_TASKS: Max. number of tasks in your application"
#else
    #if     OS_MAX_TASKS == 0
    #error  "OS_CFG.H,         OS_MAX_TASKS must be >= 2"
    #endif
    #if     OS_MAX_TASKS > 63
    #error  "OS_CFG.H,         OS_MAX_TASKS must be <= 63"
    #endif
#endif

#ifndef OS_TASK_IDLE_STK_SIZE
#error  "OS_CFG.H, Missing OS_TASK_IDLE_STK_SIZE: Idle task stack size"
#endif

#ifndef OS_TASK_STAT_EN
#error  "OS_CFG.H, Missing OS_TASK_STAT_EN: Enable (1) or Disable(0) the statistics task"
#endif

#ifndef OS_TASK_STAT_STK_SIZE
#error  "OS_CFG.H, Missing OS_TASK_STAT_STK_SIZE: Statistics task stack size"
#endif

#ifndef OS_TASK_CHANGE_PRIO_EN
#error  "OS_CFG.H, Missing OS_TASK_CHANGE_PRIO_EN: Include code for OSTaskChangePrio()"
#endif

#ifndef OS_TASK_CREATE_EN
#error  "OS_CFG.H, Missing OS_TASK_CREATE_EN: Include code for OSTaskCreate()"
#endif

#ifndef OS_TASK_CREATE_EXT_EN
#error  "OS_CFG.H, Missing OS_TASK_CREATE_EXT_EN: Include code for OSTaskCreateExt()"
#endif

#ifndef OS_TASK_DEL_EN
#error  "OS_CFG.H, Missing OS_TASK_DEL_EN: Include code for OSTaskDel()"
#endif

#ifndef OS_TASK_SUSPEND_EN
#error  "OS_CFG.H, Missing OS_TASK_SUSPEND_EN: Include code for OSTaskSuspend() and OSTaskResume()"
#endif

#ifndef OS_TASK_QUERY_EN
#error  "OS_CFG.H, Missing OS_TASK_QUERY_EN: Include code for OSTaskQuery()"
#endif

//*********************************************************************************************************
//时间管理条件编译

#ifndef OS_TICKS_PER_SEC
#error  "OS_CFG.H, Missing OS_TICKS_PER_SEC: Sets the number of ticks in one second"
#endif

#ifndef OS_TIME_DLY_HMSM_EN
#error  "OS_CFG.H, Missing OS_TIME_DLY_HMSM_EN: Include code for OSTimeDlyHMSM()"
#endif

#ifndef OS_TIME_DLY_RESUME_EN
#error  "OS_CFG.H, Missing OS_TIME_DLY_RESUME_EN: Include code for OSTimeDlyResume()"
#endif

#ifndef OS_TIME_GET_SET_EN
#error  "OS_CFG.H, Missing OS_TIME_GET_SET_EN: Include code for OSTimeGet() and OSTimeSet()"
#endif

//*********************************************************************************************************
//混合管理条件编译

#ifndef OS_MAX_EVENTS
#error  "OS_CFG.H, Missing OS_MAX_EVENTS: Max. number of event control blocks in your application"
#else
    #if     OS_MAX_EVENTS == 0
    #error  "OS_CFG.H, OS_MAX_EVENTS must be > 0"
    #endif
    #if     OS_MAX_EVENTS > 255
    #error  "OS_CFG.H, OS_MAX_EVENTS must be <= 255"
    #endif
#endif

#ifndef OS_LOWEST_PRIO
#error  "OS_CFG.H, Missing OS_LOWEST_PRIO: Defines the lowest priority that can be assigned"
#endif

#ifndef OS_ARG_CHK_EN
#error  "OS_CFG.H, Missing OS_ARG_CHK_EN: Enable (1) or Disable (0) argument checking"
#endif

#ifndef OS_CPU_HOOKS_EN
#error  "OS_CFG.H, Missing OS_CPU_HOOKS_EN: uC/OS-II hooks are found in the processor port files when 1"
#endif

#ifndef OS_SCHED_LOCK_EN
#error  "OS_CFG.H, Missing OS_SCHED_LOCK_EN: Include code for OSSchedLock() and OSSchedUnlock()"
#endif
