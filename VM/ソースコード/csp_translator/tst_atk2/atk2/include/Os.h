#ifndef OS_H
#define OS_H

#include <stdint.h>
#include <stddef.h>

#ifndef UINT_C
#define UINT_C(V)	(V##U)
#endif

typedef unsigned char boolean;

typedef char char8;

typedef int8_t sint8;
typedef int16_t sint16;
typedef int32_t sint32;
typedef int64_t sint64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef unsigned int uint8_least;
typedef unsigned int uint16_least;
typedef unsigned int uint32_least;
typedef int sint8_least;
typedef int sint16_least;
typedef int sint32_least;

typedef float float32;
typedef double float64;

typedef uint32 uintptr;
typedef sint32 sintptr;

typedef uint8 Std_ReturnType;
typedef unsigned char StatusType;

typedef uint8	TaskStateType;
typedef uint32	EventMaskType;
typedef uint32	TickType;
typedef uint32	AppModeType;
typedef uint8	OSServiceIdType;
typedef uint8	ScheduleTableStatusType;
typedef uint8	ProtectionReturnType;
typedef uintptr	MemorySizeType;
typedef uint8	ApplicationType;
typedef uint8	ObjectTypeType;

typedef uint32	TimeType;
typedef uint32	AlarmType;
typedef uint32	ResourceType;
typedef uint32	TaskType;
typedef uint32	ISRType;
typedef uint32	CounterType;
typedef uint32	ScheduleTableType;
typedef float32	PhysicalTimeType;
typedef uint8	CoreIdType;
typedef uint32	SpinlockIdType;
typedef uint32	IocType;
typedef uint8	SenderIdType;

typedef struct
{
  TickType	maxallowedvalue;
  TickType	ticksperbase;
  TickType	mincycle;
} AlarmBaseType;

typedef enum {
  TRYTOGETSPINLOCK_SUCCESS,
  TRYTOGETSPINLOCK_NOSUCCESS
} TryToGetSpinlockType;

typedef AlarmBaseType *AlarmBaseRefType;
typedef TaskType *TaskRefType;
typedef TaskStateType *TaskStateRefType;
typedef EventMaskType *EventMaskRefType;
typedef TickType *TickRefType;
typedef ScheduleTableStatusType *ScheduleTableStatusRefType;

#define E_OK		0U
#define E_NOT_OK	1U
#define STD_HIGH	iU
#define STD_LOW		0U

#define STD_ACTIVE	1U
#define STD_IDLE	0U

#define STD_ON		1U
#define STD_OFF		0U

#include "Os_Lcfg.h"

#ifndef TASK
#define TASK(TaskName)	void TaskMain  ## TaskName(void)
#endif
#ifndef ISR
#define ISR(ISRName)	void ISRMain   ## ISRName(void)
#endif
#ifndef C1ISR
#define C1ISR(ISRName)	void C1ISRMain ## ISRName(void)
#endif /* C1ISR */
#ifndef ICISR
#define ICISR(ICIName)	void ICIMain   ## ICIName(void)
#endif
#ifndef ALARMTYPE
#define ALARMCALLBACK(AlarmCallBackName) \
	void AlarmMain ## AlarmCallBackName(void)
#endif

#define E_OS_ACCESS			1U
#define E_OS_CALLEVEL			2U
#define E_OS_ID				3U
#define E_OS_LIMIT			4U
#define E_OS_NOFUNC			5U
#define E_OS_RESOURCE			6U
#define E_OS_STATE			7U
#define E_OS_VALUE			8U
#define E_OS_SERVICEID			9U
#define E_OS_ILLEGAL_ADDRESS		10U
#define E_OS_MISSINGEND			11U
#define E_OS_DISABLEDINT		12U
#define E_OS_STACKFAULT			13U
#define E_OS_PROTECTION_MEMORY		14U
#define E_OS_PROTECTION_TIME_TASK	15U
#define E_OS_PROTECTION_TIME_ISR	16U
#define E_OS_PROTECTION_ARRIVAL_TASK	17U
#define E_OS_PROTECTION_ARRIVAL_ISR	18U
#define E_OS_PROTECTION_LOCKED_RESOURCE	19U
#define E_OS_PROTECTION_LOCKED_OSINT	20U
#define E_OS_PROTECTION_LOCKED_ALLINT	21U
#define E_OS_PROTECTION_EXCEPTION	22U
#define E_OS_PROTECTION_FATAL		23U
#define E_OS_MODE			24U
#define E_OS_SHUTDOWN_FATAL		25U
#define E_OS_PARAM_POINTER		26U
#define E_OS_SYS_ASSERT_FATAL		27U
#define E_OS_STACKINSUFFICIENT		28U
#define E_OS_CORE			29U
#define E_OS_SPINLOCK			30U
#define E_OS_INTERFERENCE_DEADLOCK	31U
#define E_OS_NESTING_DEADLOCK		32U
#define E_OS_SHUTDOWN_OTHER_CORE	33U
#define E_OS_TIMEINSUFFICIENT		34U
#define E_OS_PROTECTION_TIMEWINDOW	35U
#define E_OS_PROTECTION_COUNT_ISR	36U

#define OS_E_PARAM_POINTER		E_OS_PARAM_POINTER

#define ERRCODE_NUM			36

/* definitions for OS Wrapper */

/* Task Type */
typedef struct {
  const OS_WRAPPER(_structId) sid;
  const char *name;
  const int tid;
  const int *appmode;
  const size_t napp;
  const int coreid;
  const unsigned int pri;
  const unsigned int mask;
  unsigned int event;
  int running;
  pthread_t thread;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  void (*func)(OS_WRAPPER(_ANY_TASK_TYPE) *res);
} OS_WRAPPER(_TASK_TYPE);

/* AppMode Type */
typedef struct {
  const char *name;
  const int apid;
  void (*startuphook)(OS_WRAPPER(_ANY_TASK_TYPE) *res);
  void (*errorhook)(OS_WRAPPER(_ANY_TASK_TYPE) *res, StatusType error);
  void (*shutdownhook)(OS_WRAPPER(_ANY_TASK_TYPE) *res, StatusType error);
} OS_WRAPPER(_APPMODE_TYPE);

/* OsApp Type */
typedef struct {
  OS_WRAPPER(_APPMODE_TYPE) *appmode;
  struct {
    unsigned int startup:1;
    unsigned int error:1;
    unsigned int shutdown:1;
  };
} OS_WRAPPER(_OSAPP_TYPE);

/* Core Task Type */
typedef struct {
  const OS_WRAPPER(_structId) sid;
  OS_WRAPPER(_TASK_TYPE) **tasklist;
  size_t ntask;
  OS_WRAPPER(_OSAPP_TYPE) *osapplist;
  size_t nosapp;
  struct {
    unsigned int err:1;
    unsigned int post:1;
    unsigned int pre:1;
    unsigned int stup:1;
    unsigned int shtd:1;
    unsigned int prot:1;
    unsigned int enable:1;
  };
} OS_WRAPPER(_CORE_TASK_TYPE);

/* Counter Struct */
typedef struct {
  const char *name;
  const int cid;
  size_t timer_clock;
  size_t nsec_mul;
  size_t nsec_div;
  size_t max_load;
  size_t min_cycle;
  size_t ticks_per_base;
  int hw_type;
} OS_WRAPPER(_COUNTER_TYPE);

/* Alarm Struct */
typedef struct {
  const char *name;
  const int aid;
  struct sigevent timer_event;
  timer_t timer_id;
  struct itimerspec timer_value;
  OS_WRAPPER(_COUNTER_TYPE) *counter;
} OS_WRAPPER(_ALARM_TYPE);

#define ErrorHook(E)		OS_WRAPPER(ErrorHook)(TASK__DECL,E)
#define PreTaskHook(VOID)	OS_WRAPPER(PreTaskHook)(TASK__DECL)
#define PostTaskHook(VOID)	OS_WRAPPER(PostTaskHook)(TASK__DECL)
#define StartupHook(VOID)	OS_WRAPPER(StartupHook)(TASK__DECL)
#define ShutdownHook(E)		OS_WRAPPER(ShutdownHook)(TASK__DECL,E)
#define ProtectionHook(E)	OS_WRAPPER(ProtectionHook)(TASK__DECL,E)

#define main(VOID)		OS_WRAPPER(main)(TASK__DECL)

/* main function */
sint32 main(void);

/* Task Resources */
extern OS_WRAPPER(_TASK_TYPE) OS_WRAPPER(_task_resource)[TNUM_TASK];

/* Counter List */
extern OS_WRAPPER(_COUNTER_TYPE) OS_WRAPPER(_counter_list)[TNUM_COUNTER];

/* Alaram List */
extern OS_WRAPPER(_ALARM_TYPE) OS_WRAPPER(_alarm_list)[TNUM_ALARM];

/* Start time */
extern struct timespec OS_WRAPPER(_start_time);

/* Core assigned Task */
extern OS_WRAPPER(_CORE_TASK_TYPE) OS_WRAPPER(_core_assign)[TNUM_HWCORE];

/* OS Wrapper function */
StatusType OS_WRAPPER(SetEvent)(TASK__DECL,TaskType,EventMaskType);
StatusType OS_WRAPPER(WaitEvent)(TASK__DECL,EventMaskType);
StatusType OS_WRAPPER(ClearEvent)(TASK__DECL,EventMaskType);
StatusType OS_WRAPPER(SetRelAlarm)(TASK__DECL,AlarmType,TickType,TickType);
StatusType OS_WRAPPER(GetCounterValue)(TASK__DECL,CounterType,TickRefType);
CoreIdType OS_WRAPPER(GetCoreID)(TASK__DECL);
void OS_WRAPPER(StartCore)(TASK__DECL,CoreIdType,StatusType*);
void OS_WRAPPER(StartOS)(TASK__DECL,AppModeType);

/* syslog */
void OS_WRAPPER(syslog_initialize)(TASK__DECL);
StatusType OS_WRAPPER(syslog_msk_log)(TASK__DECL,uint32);
void OS_WRAPPER(syslog)(TASK__DECL,uint32,const char8*,...);

/* misc outputs */
void OS_WRAPPER(InitSerial)(TASK__DECL);
void OS_WRAPPER(print_banner)(TASK__DECL);

/* Hooks */
void __attribute__ ((weak)) OS_WRAPPER(Errorhook)(TASK__DECL,StatusType);
void __attribute__ ((weak)) OS_WRAPPER(PreTaskHook)(TASK__DECL);
void __attribute__ ((weak)) OS_WRAPPER(PostTaskHook)(TASK__DECL);
void __attribute__ ((weak)) OS_WRAPPER(StartupHook)(TASK__DECL);
ProtectionReturnType __attribute__ ((weak))
  OS_WRAPPER(ProtectionHook)(TASK__DECL,StatusType);

/* Application hooks */
void __attribute__ ((weak))
  OS_WRAPPER(StartupHook_multirate2_AppMode)(TASK__DECL);
void __attribute__ ((weak))
  OS_WRAPPER(ShutdownHook_multirate2_AppMode)(TASK__DECL, StatusType Error);
void __attribute__ ((weak))
  OS_WRAPPER(ErrorHook_multirate2_AppMode)(TASK__DECL, StatusType Error);

/* OSAPI Macro */
#ifndef SetEvent
#define SetEvent(T,E)		OS_WRAPPER(SetEvent)(TASK__CTX,T,E)
#endif
#ifndef WaitEvent
#define WaitEvent(E)		OS_WRAPPER(WaitEvent)(TASK__CTX,E)
#endif
#ifndef ClearEvent
#define ClearEvent(E)		OS_WRAPPER(ClearEvent)(TASK__CTX,E)
#endif
#ifndef SetRelAlarm
#define SetRelAlarm(A,I,C)	OS_WRAPPER(SetRelAlarm)(TASK__CTX,A,I,C)
#endif
#ifndef GetCounterValue
#define GetCounterValue(C,T)	OS_WRAPPER(GetCounterValue)(TASK__CTX,C,T)
#endif
#ifndef GetCoreID
#define GetCoreID()		OS_WRAPPER(GetCoreID)(TASK__CTX)
#endif
#ifndef StartCore
#define StartCore(C,S)		OS_WRAPPER(StartCore)(TASK__CTX,C,S)
#endif
#ifndef StartOS
#define StartOS(A)		OS_WRAPPER(StartOS)(TASK__CTX,A)
#endif
#ifndef syslog_initialize
#define syslog_initialize()	OS_WRAPPER(syslog_initialize)(TASK__CTX)
#endif
#ifndef syslog_msk_log
#define syslog_msk_log(M)	OS_WRAPPER(syslog_msk_log)(TASK__CTX,M)
#endif
#ifndef syslog
#define syslog(L,F,...)		OS_WRAPPER(syslog)(TASK__CTX,L,F,##__VA_ARGS__)
#endif
#ifndef InitSerial
#define InitSerial()		OS_WRAPPER(InitSerial)(TASK__CTX)
#endif
#ifndef print_banner
#define print_banner()		OS_WRAPPER(print_banner)(TASK__CTX)
#endif
#ifndef Errorhook
#define Errorhook(E)		OS_WRAPPER(Errorhook)(TASK__CTX,E)
#endif
#ifndef PreTaskHook
#define PreTaskHook()		OS_WRAPPER(PreTaskHook)(TASK__CTX)
#endif
#ifndef PostTaskHook
#define PostTaskHook()		OS_WRAPPER(PostTaskHook)(TASK__CTX)
#endif
#ifndef StartupHook
#define StartupHook()		OS_WRAPPER(StartupHook)(TASK__CTX)
#endif
#ifndef ProtectionHook
#define ProtectionHook(F)	OS_WRAPPER(ProtectionHook)(TASK__CTX,F)
#endif

/* dummy */
#ifndef IncrementCounter
#define IncrementCounter(C)	(E_OK)
#endif
#ifndef ShutdownAllCores
#define ShutdownAllCores(E)	do { ; } while (0)
#endif

#endif	/* OS_H */
