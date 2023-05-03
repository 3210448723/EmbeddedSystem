/*
*********************************************************************************************************
uC/OS-II实时控制内核
主要的包含文件

文 件: OS_CFG.H ucos内核构造文件
作 者: Jean J. Labrosse
*********************************************************************************************************
*/


#ifndef __OS_CFG_H__
#define __OS_CFG_H__

//*********************************************************************************************************
//uC/OS-II 的内核构造

#define OS_MAX_EVENTS             2    //应用中最多事件控制块的数目 必须大于0                                                                       
#define OS_MAX_FLAGS              5    //应用中最多事件标志组的数目	必须大于 0							  
#define OS_MAX_MEM_PART           5    //最多内存块的数目           MUST be > 0 							 
#define OS_MAX_QS                 2    //应用中最多对列控制块的数目	MUST be > 0					   
#define OS_MAX_TASKS             11    //应用中最多任务数目			MUST be >= 2 					  
#define OS_LOWEST_PRIO           12    //定义任务的最低优先级		不得大于 63
#define OS_TASK_IDLE_STK_SIZE   512    //统计任务堆栈容量( # 按照OS_STK的宽度数目) 

#define OS_TASK_STAT_EN           1    //允许 (1) 或者禁止 (0) 统计任务
#define OS_TASK_STAT_STK_SIZE   512    //空闲任务堆栈容量 (#按照OS_STK的宽度数目) 

#define OS_ARG_CHK_EN             1    //允许 (1) 或者禁止 (0) 变量检查
#define OS_CPU_HOOKS_EN           1    //在处理器移植文件中允许使用 uC/OS-II 的接口函数


                                       // ----------------------- 事件标志管理 ------------------------ 
#define OS_FLAG_EN                1    //允许 (1) 或者禁止 (0) 产生事件标志相关代码
#define OS_FLAG_WAIT_CLR_EN       1    //允许生成 Wait on Clear 事件标志代码
#define OS_FLAG_ACCEPT_EN         1    //允许生成 OSFlagAccept()                          
#define OS_FLAG_DEL_EN            1    //允许生成 OSFlagDel()
#define OS_FLAG_QUERY_EN          1    //允许生成 OSFlagQuery() 


                                       // -------------------- 消息邮箱管理--------------------- 
#define OS_MBOX_EN                1    //允许 (1) 或者禁止 (0) 产生消息邮箱相关代码
#define OS_MBOX_ACCEPT_EN         1    //允许生成 OSMboxAccept()
#define OS_MBOX_DEL_EN            1    //允许生成 OSMboxDel() 
#define OS_MBOX_POST_EN           1    //允许生成 OSMboxPost()
#define OS_MBOX_POST_OPT_EN       1    //允许生成 OSMboxPostOpt() 
#define OS_MBOX_QUERY_EN          1    //允许生成 OSMboxQuery()


                                       // ---------------------内存管理  -------------------- 
#define OS_MEM_EN                 1    //允许 (1) 或者禁止 (0) 产生内存相关代码
#define OS_MEM_QUERY_EN           1    //允许生成 OSMemQuery() 


                                       // ---------------- 互斥型信号量管理 --------------- 
#define OS_MUTEX_EN               1    //允许 (1) 或者禁止 (0) 产生互斥型信号量相关代码
#define OS_MUTEX_ACCEPT_EN        1    //允许生成 OSMutexAccept()
#define OS_MUTEX_DEL_EN           1    //允许生成 OSMutexDel()
#define OS_MUTEX_QUERY_EN         1    //允许生成 OSMutexQuery() 


                                       // ---------------------- 消息队列号管理 ---------------------- 
#define OS_Q_EN                   1    //允许 (1) 或者禁止 (0) 产生消息队列相关代码
#define OS_Q_ACCEPT_EN            1    //允许生成 OSQAccept() 
#define OS_Q_DEL_EN               1    //允许生成 OSQDel()
#define OS_Q_FLUSH_EN             1    //允许生成 OSQFlush() 
#define OS_Q_POST_EN              1    //允许生成 OSQPost() 
#define OS_Q_POST_FRONT_EN        1    //允许生成 OSQPostFront()
#define OS_Q_POST_OPT_EN          1    //允许生成OSQPostOpt()                          
#define OS_Q_QUERY_EN             1    //允许生成OSQQuery()  


                                       // ------------------------ 信号管理 ------------------------ 
#define OS_SEM_EN                 1    //允许 (1) 或者禁止 (0) 产生信号量相关代码
#define OS_SEM_ACCEPT_EN          1    //允许生成OSSemAccept()                            
#define OS_SEM_DEL_EN             1    //允许生成OSSemDel()                               
#define OS_SEM_QUERY_EN           1    //允许生成OSSemQuery()                             


                                       // ---------------------任务管理  ---------------------- 
#define OS_TASK_CHANGE_PRIO_EN    1    //允许生成OSTaskChangePrio()                    
#define OS_TASK_CREATE_EN         1    //允许生成OSTaskCreate()                          
#define OS_TASK_CREATE_EXT_EN     1    //允许生成OSTaskCreateExt()                       
#define OS_TASK_DEL_EN            1    //允许生成OSTaskDel()                             
#define OS_TASK_SUSPEND_EN        1    //允许生成OSTaskSuspend() and OSTaskResume()      
#define OS_TASK_QUERY_EN          1    //允许生成OSTaskQuery()                           


                                       // --------------------- -时间管理 ---------------------- 
#define OS_TIME_DLY_HMSM_EN       1    //允许生成OSTimeDlyHMSM()                         
#define OS_TIME_DLY_RESUME_EN     1    //允许生成OSTimeDlyResume()                       
#define OS_TIME_GET_SET_EN        1    //允许生成OSTimeGet() and OSTimeSet()             


                                       // ---------------------- 混合管理 ----------------------- 
#define OS_SCHED_LOCK_EN          1    //允许生成OSSchedLock() and OSSchedUnlock()       


#define OS_TICKS_PER_SEC        1000   //设置每秒的节拍数目                       


//typedef INT16U             OS_FLAGS;   // Date type for event flag bits (8, 16 or 32 bits)             
typedef unsigned short            OS_FLAGS;   //事件标志的数据类型 (8位, 16位 或 32 位)	   

#endif



















