/*
 * generate "multirate2" code for TOPPERS/ATK2
 */

#include <string.h>

#include "aos.h"
#include "counter_handler.h"

#include <math.h>
#include <string.h>
#include "rtwtypes.h"
#include "multirate2_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif


/* Real-time Model Data Structure */
struct tag_RTM_multirate2 {
  const char_T * volatile errorStatus;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    struct {
      uint8_T TID[3];
    } TaskCounters;
  } Timing;
};

#include "multirate2_private.h"

#include "testout_dbg.h"

#ifndef __GNUC__
#define __inline__
#endif /* __GNUC__ */

#ifndef CORE_TASK_DATA_PREPARE
#define CORE_TASK_DATA_PREPARE(N)	/* none */
#endif /* CORE_TASK_DATA_PREPARE */

#ifndef CORE_TASK_DATA_ATTRIBUTE
#define CORE_TASK_DATA_ATTRIBUTE(N)	/* none */
#endif /* CORE_TASK_DATA_ATTRIBUTE */

#ifndef CORE_TASK_CODE_PREPARE
#define CORE_TASK_CODE_PREPARE(N)	/* none */
#endif /* CORE_TASK_CODE_PREPARE */

#ifndef CORE_TASK_CODE_ATTRIBUTE
#define CORE_TASK_CODE_ATTRIBUTE(N)	/* none */
#endif /* CORE_TASK_CODE_ATTRIBUTE */

#ifndef TASK_DATA_PREPARE
#define TASK_DATA_PREPARE(N)	/* none */
#endif /* TASK_DATA_PREPARE */

#ifndef TASK_DATA_ATTRIBUTE
#define TASK_DATA_ATTRIBUTE(N)	/* none */
#endif /* TASK_DATA_ATTRIBUTE */

#ifndef TASK_CODE_PREPARE
#define TASK_CODE_PREPARE(N)	/* none */
#endif /* TASK_CODE_PREPARE */

#ifndef TASK_CODE_ATTRIBUTE
#define TASK_CODE_ATTRIBUTE(N)	/* none */
#endif /* TASK_CODE_ATTRIBUTE */

/*
 * Channel Macros
 */
#define CH__VEC__CONCAT(A,B)	A##B
#define CH__VEC_CONCAT(A,B)	CH__VEC__CONCAT(A,B)
#define CH__VEC_NAME(N)		CH__VEC_CONCAT(ch__vec_,N)
#define CH__VEC_ELEMENT_TYPE	unsigned long
#define CH__VEC_ELEMENT_BITS	(sizeof (CH__VEC_ELEMENT_TYPE) * 8)
#define CH__VEC_ELEMENT_SIZE(S)	\
  (((S)+CH__VEC_ELEMENT_BITS-1)/CH__VEC_ELEMENT_BITS)
#define CH__VEC_ELEMENT_0	((CH__VEC_ELEMENT_TYPE)0)
#define CH__VEC_ELEMENT_1	((CH__VEC_ELEMENT_TYPE)1)
#define CH__VEC(N,S)	\
  CH__VEC_ELEMENT_TYPE CH__VEC_NAME(N)[CH__VEC_ELEMENT_SIZE(S)] = {0,}
#define CH__VEC_INIT(N)		memset(CH__VEC_NAME(N), 0, sizeof CH__VEC_NAME(N))
#define CH__BITCHECK(N,B)	ch__bitcheck(CH__VEC_NAME(N),B)
#define CH__BITSET(N,B)		ch__bitset(CH__VEC_NAME(N),B)
#define CH__COMPLETED(N,S)	ch__completed(CH__VEC_NAME(N),S)

#define CH__TIME_LT(L,R)	((long)((L)-(R)) < (long)0)
#define CH__TIME_LE(L,R)	((long)((L)-(R)) <= (long)0)
#define CH__SEND_COUNT(N,D,R)	(((N)-(D)+((R)-1))/R)

/*
 * Task Macros for TOPPERS/ATK2
 */

#ifndef SwCnt0_TIMER_CLOCK_HZ
#define SwCnt0_TIMER_CLOCK_HZ	1000
#endif /* SwCnt0_TIMER_CLOCK_HZ */

#define ATK2_TASK_WAIT_TASKS	multirate2_Core1_1000_0_Task
#define ATK2_WE_WAIT_TASKS	multirate2_CompleteEvent
#define ATK2_SE_WAIT_TASKS	multirate2_CompleteEvent
#define ATK2_TNAME_WAIT_TASKS	"multirate2_Core1_1000_0_Task"
#define ATK2_TNAME_TASK_WAIT_TASKS	"multirate2_Core1_1000_0_Task"

/* Task: multirate2_Core1_1000_0_Task */
#define ATK2_TASK_multirate2_Core1_1000_0_Task	multirate2_Core1_1000_0_Task
#define ATK2_WE_multirate2_Core1_1000_0_Task	multirate2_WakeupEvent
#define ATK2_SE_multirate2_Core1_1000_0_Task	multirate2_WakeupEvent
#define ATK2_TNAME_TASK_multirate2_Core1_1000_0_Task	"multirate2_Core1_1000_0_Task"
#define ATK2_ALARM_multirate2_Core1_1000_0_Task	multirate2_Core1_1000_0_Alarm
#define ATK2_TNAME_ALARM_multirate2_Core1_1000_0_Task	"multirate2_Core1_1000_0_Alarm"

/* Task_0001: multirate2_FromWorkspace */
#define ATK2_TASK_0001		multirate2_Core1_1000_0_Task
#define ATK2_TASK_IN_0001	ATK2_TASK_0001
#define ATK2_TASK_OUT_0001	ATK2_TASK_0001
#define ATK2_SE_IN_0001		ATK2_TASK_0001, multirate2_InputEvent
#define ATK2_SE_OUT_0001	ATK2_TASK_0001, multirate2_OutputEvent
#define ATK2_WE_IN_0001		multirate2_InputEvent
#define ATK2_WE_OUT_0001	multirate2_OutputEvent
#define ATK2_TNAME_TASK_0001	"multirate2_Core1_1000_0_Task"
#define ATK2_TNAME_IN_0001	ATK2_TNAME_TASK_0001
#define ATK2_TNAME_OUT_0001	ATK2_TNAME_TASK_0001
#define ATK2_TID_TASK_0001	"0001"
#define ATK2_TID_IN_0001	ATK2_TID_TASK_0001
#define ATK2_TID_OUT_0001	ATK2_TID_TASK_0001
#define ATK2_WE_TASK_0001	multirate2_WakeupEvent
#define ATK2_SE_TASK_0001	multirate2_WakeupEvent

/* Task_0027: multirate2_pid_Sum2 */
#define ATK2_TASK_0027		multirate2_Core1_1000_0_Task
#define ATK2_TASK_IN_0027	ATK2_TASK_0027
#define ATK2_TASK_OUT_0027	ATK2_TASK_0027
#define ATK2_SE_IN_0027		ATK2_TASK_0027, multirate2_InputEvent
#define ATK2_SE_OUT_0027	ATK2_TASK_0027, multirate2_OutputEvent
#define ATK2_WE_IN_0027		multirate2_InputEvent
#define ATK2_WE_OUT_0027	multirate2_OutputEvent
#define ATK2_TNAME_TASK_0027	"multirate2_Core1_1000_0_Task"
#define ATK2_TNAME_IN_0027	ATK2_TNAME_TASK_0027
#define ATK2_TNAME_OUT_0027	ATK2_TNAME_TASK_0027
#define ATK2_TID_TASK_0027	"0027"
#define ATK2_TID_IN_0027	ATK2_TID_TASK_0027
#define ATK2_TID_OUT_0027	ATK2_TID_TASK_0027
#define ATK2_WE_TASK_0027	multirate2_WakeupEvent
#define ATK2_SE_TASK_0027	multirate2_WakeupEvent

/* Task_0026: multirate2_pid_Saturation */
#define ATK2_TASK_0026		multirate2_Core1_1000_0_Task
#define ATK2_TASK_IN_0026	ATK2_TASK_0026
#define ATK2_TASK_OUT_0026	ATK2_TASK_0026
#define ATK2_SE_IN_0026		ATK2_TASK_0026, multirate2_InputEvent
#define ATK2_SE_OUT_0026	ATK2_TASK_0026, multirate2_OutputEvent
#define ATK2_WE_IN_0026		multirate2_InputEvent
#define ATK2_WE_OUT_0026	multirate2_OutputEvent
#define ATK2_TNAME_TASK_0026	"multirate2_Core1_1000_0_Task"
#define ATK2_TNAME_IN_0026	ATK2_TNAME_TASK_0026
#define ATK2_TNAME_OUT_0026	ATK2_TNAME_TASK_0026
#define ATK2_TID_TASK_0026	"0026"
#define ATK2_TID_IN_0026	ATK2_TID_TASK_0026
#define ATK2_TID_OUT_0026	ATK2_TID_TASK_0026
#define ATK2_WE_TASK_0026	multirate2_WakeupEvent
#define ATK2_SE_TASK_0026	multirate2_WakeupEvent

/* Task_0029: multirate2_voltage */
#define ATK2_TASK_0029		multirate2_Core1_1000_0_Task
#define ATK2_TASK_IN_0029	ATK2_TASK_0029
#define ATK2_TASK_OUT_0029	ATK2_TASK_0029
#define ATK2_SE_IN_0029		ATK2_TASK_0029, multirate2_InputEvent
#define ATK2_SE_OUT_0029	ATK2_TASK_0029, multirate2_OutputEvent
#define ATK2_WE_IN_0029		multirate2_InputEvent
#define ATK2_WE_OUT_0029	multirate2_OutputEvent
#define ATK2_TNAME_TASK_0029	"multirate2_Core1_1000_0_Task"
#define ATK2_TNAME_IN_0029	ATK2_TNAME_TASK_0029
#define ATK2_TNAME_OUT_0029	ATK2_TNAME_TASK_0029
#define ATK2_TID_TASK_0029	"0029"
#define ATK2_TID_IN_0029	ATK2_TID_TASK_0029
#define ATK2_TID_OUT_0029	ATK2_TID_TASK_0029
#define ATK2_WE_TASK_0029	multirate2_WakeupEvent
#define ATK2_SE_TASK_0029	multirate2_WakeupEvent

/* Task: multirate2_Core2_1000_0_Task */
#define ATK2_TASK_multirate2_Core2_1000_0_Task	multirate2_Core2_1000_0_Task
#define ATK2_WE_multirate2_Core2_1000_0_Task	multirate2_WakeupEvent
#define ATK2_SE_multirate2_Core2_1000_0_Task	multirate2_WakeupEvent
#define ATK2_TNAME_TASK_multirate2_Core2_1000_0_Task	"multirate2_Core2_1000_0_Task"
#define ATK2_ALARM_multirate2_Core2_1000_0_Task	multirate2_Core2_1000_0_Alarm
#define ATK2_TNAME_ALARM_multirate2_Core2_1000_0_Task	"multirate2_Core2_1000_0_Alarm"

/* Task_0004: multirate2_pid_D */
#define ATK2_TASK_0004		multirate2_Core2_1000_0_Task
#define ATK2_TASK_IN_0004	ATK2_TASK_0004
#define ATK2_TASK_OUT_0004	ATK2_TASK_0004
#define ATK2_SE_IN_0004		ATK2_TASK_0004, multirate2_InputEvent
#define ATK2_SE_OUT_0004	ATK2_TASK_0004, multirate2_OutputEvent
#define ATK2_WE_IN_0004		multirate2_InputEvent
#define ATK2_WE_OUT_0004	multirate2_OutputEvent
#define ATK2_TNAME_TASK_0004	"multirate2_Core2_1000_0_Task"
#define ATK2_TNAME_IN_0004	ATK2_TNAME_TASK_0004
#define ATK2_TNAME_OUT_0004	ATK2_TNAME_TASK_0004
#define ATK2_TID_TASK_0004	"0004"
#define ATK2_TID_IN_0004	ATK2_TID_TASK_0004
#define ATK2_TID_OUT_0004	ATK2_TID_TASK_0004
#define ATK2_WE_TASK_0004	multirate2_WakeupEvent
#define ATK2_SE_TASK_0004	multirate2_WakeupEvent

/* Task: multirate2_Core1_2000_0_Task */
#define ATK2_TASK_multirate2_Core1_2000_0_Task	multirate2_Core1_2000_0_Task
#define ATK2_WE_multirate2_Core1_2000_0_Task	multirate2_WakeupEvent
#define ATK2_SE_multirate2_Core1_2000_0_Task	multirate2_WakeupEvent
#define ATK2_TNAME_TASK_multirate2_Core1_2000_0_Task	"multirate2_Core1_2000_0_Task"
#define ATK2_ALARM_multirate2_Core1_2000_0_Task	multirate2_Core1_2000_0_Alarm
#define ATK2_TNAME_ALARM_multirate2_Core1_2000_0_Task	"multirate2_Core1_2000_0_Alarm"

/* Task_0022: multirate2_pid_RateTransition */
#define ATK2_TASK_0022		multirate2_Core1_2000_0_Task
#define ATK2_TASK_IN_0022	ATK2_TASK_0022
#define ATK2_TASK_OUT_0022	ATK2_TASK_0022
#define ATK2_SE_IN_0022		ATK2_TASK_0022, multirate2_InputEvent
#define ATK2_SE_OUT_0022	ATK2_TASK_0022, multirate2_OutputEvent
#define ATK2_WE_IN_0022		multirate2_InputEvent
#define ATK2_WE_OUT_0022	multirate2_OutputEvent
#define ATK2_TNAME_TASK_0022	"multirate2_Core1_2000_0_Task"
#define ATK2_TNAME_IN_0022	ATK2_TNAME_TASK_0022
#define ATK2_TNAME_OUT_0022	ATK2_TNAME_TASK_0022
#define ATK2_TID_TASK_0022	"0022"
#define ATK2_TID_IN_0022	ATK2_TID_TASK_0022
#define ATK2_TID_OUT_0022	ATK2_TID_TASK_0022
#define ATK2_WE_TASK_0022	multirate2_WakeupEvent
#define ATK2_SE_TASK_0022	multirate2_WakeupEvent

/* Task_0023: multirate2_pid_RateTransition1 */
#define ATK2_TASK_0023		multirate2_Core1_2000_0_Task
#define ATK2_TASK_IN_0023	ATK2_TASK_0023
#define ATK2_TASK_OUT_0023	ATK2_TASK_0023
#define ATK2_SE_IN_0023		ATK2_TASK_0023, multirate2_InputEvent
#define ATK2_SE_OUT_0023	ATK2_TASK_0023, multirate2_OutputEvent
#define ATK2_WE_IN_0023		multirate2_InputEvent
#define ATK2_WE_OUT_0023	multirate2_OutputEvent
#define ATK2_TNAME_TASK_0023	"multirate2_Core1_2000_0_Task"
#define ATK2_TNAME_IN_0023	ATK2_TNAME_TASK_0023
#define ATK2_TNAME_OUT_0023	ATK2_TNAME_TASK_0023
#define ATK2_TID_TASK_0023	"0023"
#define ATK2_TID_IN_0023	ATK2_TID_TASK_0023
#define ATK2_TID_OUT_0023	ATK2_TID_TASK_0023
#define ATK2_WE_TASK_0023	multirate2_WakeupEvent
#define ATK2_SE_TASK_0023	multirate2_WakeupEvent

/* Task_0011: multirate2_pid_I */
#define ATK2_TASK_0011		multirate2_Core1_2000_0_Task
#define ATK2_TASK_IN_0011	ATK2_TASK_0011
#define ATK2_TASK_OUT_0011	ATK2_TASK_0011
#define ATK2_SE_IN_0011		ATK2_TASK_0011, multirate2_InputEvent
#define ATK2_SE_OUT_0011	ATK2_TASK_0011, multirate2_OutputEvent
#define ATK2_WE_IN_0011		multirate2_InputEvent
#define ATK2_WE_OUT_0011	multirate2_OutputEvent
#define ATK2_TNAME_TASK_0011	"multirate2_Core1_2000_0_Task"
#define ATK2_TNAME_IN_0011	ATK2_TNAME_TASK_0011
#define ATK2_TNAME_OUT_0011	ATK2_TNAME_TASK_0011
#define ATK2_TID_TASK_0011	"0011"
#define ATK2_TID_IN_0011	ATK2_TID_TASK_0011
#define ATK2_TID_OUT_0011	ATK2_TID_TASK_0011
#define ATK2_WE_TASK_0011	multirate2_WakeupEvent
#define ATK2_SE_TASK_0011	multirate2_WakeupEvent

/* Task: multirate2_Core1_3000_0_Task */
#define ATK2_TASK_multirate2_Core1_3000_0_Task	multirate2_Core1_3000_0_Task
#define ATK2_WE_multirate2_Core1_3000_0_Task	multirate2_WakeupEvent
#define ATK2_SE_multirate2_Core1_3000_0_Task	multirate2_WakeupEvent
#define ATK2_TNAME_TASK_multirate2_Core1_3000_0_Task	"multirate2_Core1_3000_0_Task"
#define ATK2_ALARM_multirate2_Core1_3000_0_Task	multirate2_Core1_3000_0_Alarm
#define ATK2_TNAME_ALARM_multirate2_Core1_3000_0_Task	"multirate2_Core1_3000_0_Alarm"

/* Task_0024: multirate2_pid_RateTransition2 */
#define ATK2_TASK_0024		multirate2_Core1_3000_0_Task
#define ATK2_TASK_IN_0024	ATK2_TASK_0024
#define ATK2_TASK_OUT_0024	ATK2_TASK_0024
#define ATK2_SE_IN_0024		ATK2_TASK_0024, multirate2_InputEvent
#define ATK2_SE_OUT_0024	ATK2_TASK_0024, multirate2_OutputEvent
#define ATK2_WE_IN_0024		multirate2_InputEvent
#define ATK2_WE_OUT_0024	multirate2_OutputEvent
#define ATK2_TNAME_TASK_0024	"multirate2_Core1_3000_0_Task"
#define ATK2_TNAME_IN_0024	ATK2_TNAME_TASK_0024
#define ATK2_TNAME_OUT_0024	ATK2_TNAME_TASK_0024
#define ATK2_TID_TASK_0024	"0024"
#define ATK2_TID_IN_0024	ATK2_TID_TASK_0024
#define ATK2_TID_OUT_0024	ATK2_TID_TASK_0024
#define ATK2_WE_TASK_0024	multirate2_WakeupEvent
#define ATK2_SE_TASK_0024	multirate2_WakeupEvent

/* Task: multirate2_Core2_3000_0_Task */
#define ATK2_TASK_multirate2_Core2_3000_0_Task	multirate2_Core2_3000_0_Task
#define ATK2_WE_multirate2_Core2_3000_0_Task	multirate2_WakeupEvent
#define ATK2_SE_multirate2_Core2_3000_0_Task	multirate2_WakeupEvent
#define ATK2_TNAME_TASK_multirate2_Core2_3000_0_Task	"multirate2_Core2_3000_0_Task"
#define ATK2_ALARM_multirate2_Core2_3000_0_Task	multirate2_Core2_3000_0_Alarm
#define ATK2_TNAME_ALARM_multirate2_Core2_3000_0_Task	"multirate2_Core2_3000_0_Alarm"

/* Task_0025: multirate2_pid_RateTransition3 */
#define ATK2_TASK_0025		multirate2_Core2_3000_0_Task
#define ATK2_TASK_IN_0025	ATK2_TASK_0025
#define ATK2_TASK_OUT_0025	ATK2_TASK_0025
#define ATK2_SE_IN_0025		ATK2_TASK_0025, multirate2_InputEvent
#define ATK2_SE_OUT_0025	ATK2_TASK_0025, multirate2_OutputEvent
#define ATK2_WE_IN_0025		multirate2_InputEvent
#define ATK2_WE_OUT_0025	multirate2_OutputEvent
#define ATK2_TNAME_TASK_0025	"multirate2_Core2_3000_0_Task"
#define ATK2_TNAME_IN_0025	ATK2_TNAME_TASK_0025
#define ATK2_TNAME_OUT_0025	ATK2_TNAME_TASK_0025
#define ATK2_TID_TASK_0025	"0025"
#define ATK2_TID_IN_0025	ATK2_TID_TASK_0025
#define ATK2_TID_OUT_0025	ATK2_TID_TASK_0025
#define ATK2_WE_TASK_0025	multirate2_WakeupEvent
#define ATK2_SE_TASK_0025	multirate2_WakeupEvent

/* Task_0018: multirate2_pid_P */
#define ATK2_TASK_0018		multirate2_Core2_3000_0_Task
#define ATK2_TASK_IN_0018	ATK2_TASK_0018
#define ATK2_TASK_OUT_0018	ATK2_TASK_0018
#define ATK2_SE_IN_0018		ATK2_TASK_0018, multirate2_InputEvent
#define ATK2_SE_OUT_0018	ATK2_TASK_0018, multirate2_OutputEvent
#define ATK2_WE_IN_0018		multirate2_InputEvent
#define ATK2_WE_OUT_0018	multirate2_OutputEvent
#define ATK2_TNAME_TASK_0018	"multirate2_Core2_3000_0_Task"
#define ATK2_TNAME_IN_0018	ATK2_TNAME_TASK_0018
#define ATK2_TNAME_OUT_0018	ATK2_TNAME_TASK_0018
#define ATK2_TID_TASK_0018	"0018"
#define ATK2_TID_IN_0018	ATK2_TID_TASK_0018
#define ATK2_TID_OUT_0018	ATK2_TID_TASK_0018
#define ATK2_WE_TASK_0018	multirate2_WakeupEvent
#define ATK2_SE_TASK_0018	multirate2_WakeupEvent

/* Synchronus Memory */
#ifndef CH__SYNCM
#if defined(__v850__) && defined(__GNUC__)
#define CH__SYNCM()	__asm__ __volatile__ ("syncm" ::: "memory")
#elif defined(__nios2__) && defined(__GNUC__)
#define CH__SYNCM()	__asm__ __volatile__ ("sync" ::: "memory")
#else
#define CH__SYNCM()	do { ; } while (0)
#endif /* !__v850__ || !__nios2__ || !__GNUC__ */
#endif /* !CH__SYNCM */

/* Memory Barrier for Compiler */
#ifndef CH__MEM_BARRIER
#ifdef __GNUC__
#define CH__MEM_BARRIER()	__asm__ __volatile__ ("" ::: "memory")
#else /* !__GNUC__ */
#define CH__MEM_BARRIER()	do { ; } while (0)
#endif /* !__GNUC__ */
#endif /* !CH__MEM_BARRIER */

/* TOPPERS/ATK2 OS API Wrapper Macros */
#ifndef OS__SENDEVENT
#define OS__SENDEVENT(T,R,E)	\
  SetEvent(CH__VEC_CONCAT(ATK2_,R),CH__VEC_CONCAT(ATK2_,E))
#endif
#ifndef OS__WAITEVENT
#define OS__WAITEVENT(T,E)	\
  do { WaitEvent(CH__VEC_CONCAT(ATK2_,E));	\
       ClearEvent(CH__VEC_CONCAT(ATK2_,E)); } while (0)
#endif
#ifndef OS__SETALARM
#define OS__SETALARM(A,O,R)	\
  SetRelAlarm(CH__VEC_CONCAT(ATK2_,A),O,R)
#endif

/* TOPPERS/ATK2 Channel Macros */
#ifndef CH__TASK_SCHED
#define CH__TASK_SCHED(C,V,S)	\
  do { WaitEvent(CH__VEC_CONCAT(ATK2_WE_,C));	\
       ClearEvent(CH__VEC_CONCAT(ATK2_WE_,C)); } while (0)
#endif
#ifndef CH__CORE_THREAD_SCHED
#define CH__CORE_THREAD_SCHED(C,V,S)	CH__TASK_SCHED(C,V,S)
#endif
/* one argument wrapper macro */
#ifndef CH__SetEvent
#define CH__SetEvent(C)	SetEvent(C)
#endif
#ifndef CH__EVENT
#define CH__EVENT(T,C)	\
  do { CH__SYNCM(); \
       CH__SetEvent(CH__VEC_CONCAT(ATK2_SE_,C)); } while (0)
#endif
#ifndef CH__END_LOG
#define CH__END_LOG(C)	do { ; } while (0)
#endif

/* check channle bitvec */
static __inline__ int
ch__bitcheck (CH__VEC_ELEMENT_TYPE *v, size_t b)
{
  return (v[b/CH__VEC_ELEMENT_BITS]
          & (CH__VEC_ELEMENT_1 << (b % CH__VEC_ELEMENT_BITS)));
}

/* set channle bitvec */
static __inline__ void
ch__bitset (CH__VEC_ELEMENT_TYPE *v, size_t b)
{
  v[b/CH__VEC_ELEMENT_BITS]
    |= (CH__VEC_ELEMENT_1 << (b % CH__VEC_ELEMENT_BITS));
}

/* completed channel wait */
static __inline__ int
ch__completed (CH__VEC_ELEMENT_TYPE *v, size_t n)
{
  const size_t restbits = n % CH__VEC_ELEMENT_BITS;
  size_t i;
  size_t e = (n - 1) / CH__VEC_ELEMENT_BITS;
  CH__VEC_ELEMENT_TYPE const
    restmask =  ((restbits == 0)
		 ? (~CH__VEC_ELEMENT_0)
		 : ((CH__VEC_ELEMENT_1 << restbits) - 1));
  for (i = 0; i < e; i++, v++)
    {
      if (*v != ~(CH__VEC_ELEMENT_0))
	return 0;
    }
  if (*v != restmask)
    return 0;
  return 1;
}

/*
 * Channels
 */

/* From: multirate2_FromWorkspace
   To: multirate2_pid_D */
typedef struct {
  int flag__0001_0004;
  real_T multirate2_FromWorkspace_1;
} TYPE_multirate2_0001_0004;
TYPE_multirate2_0001_0004 CH_multirate2_0001_0004;

/* From: multirate2_FromWorkspace
   To: multirate2_pid_RateTransition1
    [1000] -> [2000] */
typedef struct {
  int flag__0001_0023;
  unsigned long time__0001_0023;
  real_T multirate2_FromWorkspace_1;
} TYPE_multirate2_0001_0023;
TYPE_multirate2_0001_0023 CH_multirate2_0001_0023;

/* From: multirate2_FromWorkspace
   To: multirate2_pid_RateTransition2
    [1000] -> [3000] */
typedef struct {
  int flag__0001_0024;
  unsigned long time__0001_0024;
  real_T multirate2_FromWorkspace_1;
} TYPE_multirate2_0001_0024;
TYPE_multirate2_0001_0024 CH_multirate2_0001_0024;

/* From: multirate2_pid_D
   To: multirate2_pid_Sum2 */
typedef struct {
  int flag__0004_0027;
  real_T multirate2_pid_D_Gain3_1;
} TYPE_multirate2_0004_0027;
TYPE_multirate2_0004_0027 CH_multirate2_0004_0027;

/* From: multirate2_pid_RateTransition
   To: multirate2_pid_Sum2
    [2000] -> [1000] */
typedef struct {
  int flag__0022_0027[2];
  unsigned long time__0022_0027;
  int input_buffer_side;
  int output_buffer_side;
  real_T multirate2_pid_RateTransition_1[2];
} TYPE_multirate2_0022_0027;
TYPE_multirate2_0022_0027 CH_multirate2_0022_0027;

/* From: multirate2_pid_RateTransition2
   To: multirate2_pid_P */
typedef struct {
  int flag__0024_0018;
  real_T multirate2_pid_RateTransition2_1;
} TYPE_multirate2_0024_0018;
TYPE_multirate2_0024_0018 CH_multirate2_0024_0018;

/* From: multirate2_pid_RateTransition3
   To: multirate2_pid_Sum2
    [3000] -> [1000] */
typedef struct {
  int flag__0025_0027[2];
  unsigned long time__0025_0027;
  int input_buffer_side;
  int output_buffer_side;
  real_T multirate2_pid_RateTransition3_1[2];
} TYPE_multirate2_0025_0027;
TYPE_multirate2_0025_0027 CH_multirate2_0025_0027;

CORE_TASK_CODE_PREPARE(1)
/* rate: 1000 */
/* Core#1: Rate: 1000 */
TASK(multirate2_Core1_1000_0_Task)
{
  /* task time */
  unsigned long task__time = 0;

  /* definitions of step function */
  real_T u0;
  real_T u1;
  real_T u2;

  /* end of definitions */
  /* output channel bit vector */
  CH__VEC(OUT_0001,3);

  /* input channel bit vector */
  CH__VEC(IN_0027,3);

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0027,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0026,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0026,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0029,1); */

  /* Real-Time Model Object */
  RT_MODEL_multirate2 multirate2_M_;
  RT_MODEL_multirate2 * const multirate2_M = &multirate2_M_;

  /* states */
  struct {
      struct {
    void *TimePtr;
    void *DataPtr;
    void *RSimInfoPtr;
  } FromWorkspace_PWORK;
      struct {
    int_T PrevIndex;
  } FromWorkspace_IWORK;
  } multirate2_DWork;

  /* input/output/auto variables */
  real_T multirate2_FromWorkspace_1;
  real_T multirate2_pid_D_Gain3_1;
  real_T multirate2_pid_RateTransition3_1;
  real_T multirate2_pid_RateTransition_1;
  real_T multirate2_pid_Sum2_1;
  real_T multirate2_pid_Saturation_1;

  /* functions */

  /* Block: multirate2_FromWorkspace */

#ifdef TASK_multirate2_Core1_1000_0_Task_DEFINITIONS
  TASK_multirate2_Core1_1000_0_Task_DEFINITIONS
#endif

#ifdef TASK_0001_DEFINITIONS
  TASK_0001_DEFINITIONS
#endif

#ifdef TASK_0027_DEFINITIONS
  TASK_0027_DEFINITIONS
#endif

#ifdef TASK_0026_DEFINITIONS
  TASK_0026_DEFINITIONS
#endif

#ifdef TASK_0029_DEFINITIONS
  TASK_0029_DEFINITIONS
#endif

  /* initialize Real-Time Model Object */
  /* initialize real-time model */
  (void) memset((void *)multirate2_M, 0,
                sizeof(RT_MODEL_multirate2));

  /* initialize state structure */
  memset(&multirate2_DWork, 0, sizeof multirate2_DWork);

  /* Block: multirate2_FromWorkspace */

  /* initialize task context */
{
  multirate2_FromWorkspace_1 = 0.0;
  /* Start for FromWorkspace: '<Root>/FromWorkspace' */
  {
    static real_T pTimeValues0[] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0,
      9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0,
      21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0,
      33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0, 40.0, 41.0, 42.0, 43.0, 44.0,
      45.0, 46.0, 47.0, 48.0, 49.0, 50.0, 51.0, 52.0, 53.0, 54.0, 55.0, 56.0,
      57.0, 58.0, 59.0, 60.0, 61.0, 62.0, 63.0, 64.0, 65.0, 66.0, 67.0, 68.0,
      69.0, 70.0, 71.0, 72.0, 73.0, 74.0, 75.0, 76.0, 77.0, 78.0, 79.0, 80.0,
      81.0, 82.0, 83.0, 84.0, 85.0, 86.0, 87.0, 88.0, 89.0, 90.0, 91.0, 92.0,
      93.0, 94.0, 95.0, 96.0, 97.0, 98.0, 99.0, 100.0, 101.0, 102.0, 103.0,
      104.0, 105.0, 106.0, 107.0, 108.0, 109.0, 110.0, 111.0, 112.0, 113.0,
      114.0, 115.0, 116.0, 117.0, 118.0, 119.0, 120.0, 121.0, 122.0, 123.0,
      124.0, 125.0, 126.0, 127.0, 128.0, 129.0, 130.0, 131.0, 132.0, 133.0,
      134.0, 135.0, 136.0, 137.0, 138.0, 139.0, 140.0, 141.0, 142.0, 143.0,
      144.0, 145.0, 146.0, 147.0, 148.0, 149.0, 150.0, 151.0, 152.0, 153.0,
      154.0, 155.0, 156.0, 157.0, 158.0, 159.0, 160.0, 161.0, 162.0, 163.0,
      164.0, 165.0, 166.0, 167.0, 168.0, 169.0, 170.0, 171.0, 172.0, 173.0,
      174.0, 175.0, 176.0, 177.0, 178.0, 179.0, 180.0, 181.0, 182.0, 183.0,
      184.0, 185.0, 186.0, 187.0, 188.0, 189.0, 190.0, 191.0, 192.0, 193.0,
      194.0, 195.0, 196.0, 197.0, 198.0, 199.0, 200.0, 201.0, 202.0, 203.0,
      204.0, 205.0, 206.0, 207.0, 208.0, 209.0, 210.0, 211.0, 212.0, 213.0,
      214.0, 215.0, 216.0, 217.0, 218.0, 219.0, 220.0, 221.0, 222.0, 223.0,
      224.0, 225.0, 226.0, 227.0, 228.0, 229.0, 230.0, 231.0, 232.0, 233.0,
      234.0, 235.0, 236.0, 237.0, 238.0, 239.0, 240.0, 241.0, 242.0, 243.0,
      244.0, 245.0, 246.0, 247.0, 248.0, 249.0, 250.0, 251.0, 252.0, 253.0,
      254.0, 255.0, 256.0, 257.0, 258.0, 259.0, 260.0, 261.0, 262.0, 263.0,
      264.0, 265.0, 266.0, 267.0, 268.0, 269.0, 270.0, 271.0, 272.0, 273.0,
      274.0, 275.0, 276.0, 277.0, 278.0, 279.0, 280.0, 281.0, 282.0, 283.0,
      284.0, 285.0, 286.0, 287.0, 288.0, 289.0, 290.0, 291.0, 292.0, 293.0,
      294.0, 295.0, 296.0, 297.0, 298.0, 299.0, 300.0, 301.0, 302.0, 303.0,
      304.0, 305.0, 306.0, 307.0, 308.0, 309.0, 310.0, 311.0, 312.0, 313.0,
      314.0, 315.0, 316.0, 317.0, 318.0, 319.0, 320.0, 321.0, 322.0, 323.0,
      324.0, 325.0, 326.0, 327.0, 328.0, 329.0, 330.0, 331.0, 332.0, 333.0,
      334.0, 335.0, 336.0, 337.0, 338.0, 339.0, 340.0, 341.0, 342.0, 343.0,
      344.0, 345.0, 346.0, 347.0, 348.0, 349.0, 350.0, 351.0, 352.0, 353.0,
      354.0, 355.0, 356.0, 357.0, 358.0, 359.0, 360.0, 361.0, 362.0, 363.0,
      364.0, 365.0, 366.0, 367.0, 368.0, 369.0, 370.0, 371.0, 372.0, 373.0,
      374.0, 375.0, 376.0, 377.0, 378.0, 379.0, 380.0, 381.0, 382.0, 383.0,
      384.0, 385.0, 386.0, 387.0, 388.0, 389.0, 390.0, 391.0, 392.0, 393.0,
      394.0, 395.0, 396.0, 397.0, 398.0, 399.0, 400.0, 401.0, 402.0, 403.0,
      404.0, 405.0, 406.0, 407.0, 408.0, 409.0, 410.0, 411.0, 412.0, 413.0,
      414.0, 415.0, 416.0, 417.0, 418.0, 419.0, 420.0, 421.0, 422.0, 423.0,
      424.0, 425.0, 426.0, 427.0, 428.0, 429.0, 430.0, 431.0, 432.0, 433.0,
      434.0, 435.0, 436.0, 437.0, 438.0, 439.0, 440.0, 441.0, 442.0, 443.0,
      444.0, 445.0, 446.0, 447.0, 448.0, 449.0, 450.0, 451.0, 452.0, 453.0,
      454.0, 455.0, 456.0, 457.0, 458.0, 459.0, 460.0, 461.0, 462.0, 463.0,
      464.0, 465.0, 466.0, 467.0, 468.0, 469.0, 470.0, 471.0, 472.0, 473.0,
      474.0, 475.0, 476.0, 477.0, 478.0, 479.0, 480.0, 481.0, 482.0, 483.0,
      484.0, 485.0, 486.0, 487.0, 488.0, 489.0, 490.0, 491.0, 492.0, 493.0,
      494.0, 495.0, 496.0, 497.0, 498.0, 499.0, 500.0, 501.0, 502.0, 503.0,
      504.0, 505.0, 506.0, 507.0, 508.0, 509.0, 510.0, 511.0, 512.0, 513.0,
      514.0, 515.0, 516.0, 517.0, 518.0, 519.0, 520.0, 521.0, 522.0, 523.0,
      524.0, 525.0, 526.0, 527.0, 528.0, 529.0, 530.0, 531.0, 532.0, 533.0,
      534.0, 535.0, 536.0, 537.0, 538.0, 539.0, 540.0, 541.0, 542.0, 543.0,
      544.0, 545.0, 546.0, 547.0, 548.0, 549.0, 550.0, 551.0, 552.0, 553.0,
      554.0, 555.0, 556.0, 557.0, 558.0, 559.0, 560.0, 561.0, 562.0, 563.0,
      564.0, 565.0, 566.0, 567.0, 568.0, 569.0, 570.0, 571.0, 572.0, 573.0,
      574.0, 575.0, 576.0, 577.0, 578.0, 579.0, 580.0, 581.0, 582.0, 583.0,
      584.0, 585.0, 586.0, 587.0, 588.0, 589.0, 590.0, 591.0, 592.0, 593.0,
      594.0, 595.0, 596.0, 597.0, 598.0, 599.0, 600.0, 601.0, 602.0, 603.0,
      604.0, 605.0, 606.0, 607.0, 608.0, 609.0, 610.0, 611.0, 612.0, 613.0,
      614.0, 615.0, 616.0, 617.0, 618.0, 619.0, 620.0, 621.0, 622.0, 623.0,
      624.0, 625.0, 626.0, 627.0, 628.0, 629.0, 630.0, 631.0, 632.0, 633.0,
      634.0, 635.0, 636.0, 637.0, 638.0, 639.0, 640.0, 641.0, 642.0, 643.0,
      644.0, 645.0, 646.0, 647.0, 648.0, 649.0, 650.0, 651.0, 652.0, 653.0,
      654.0, 655.0, 656.0, 657.0, 658.0, 659.0, 660.0, 661.0, 662.0, 663.0,
      664.0, 665.0, 666.0, 667.0, 668.0, 669.0, 670.0, 671.0, 672.0, 673.0,
      674.0, 675.0, 676.0, 677.0, 678.0, 679.0, 680.0, 681.0, 682.0, 683.0,
      684.0, 685.0, 686.0, 687.0, 688.0, 689.0, 690.0, 691.0, 692.0, 693.0,
      694.0, 695.0, 696.0, 697.0, 698.0, 699.0, 700.0, 701.0, 702.0, 703.0,
      704.0, 705.0, 706.0, 707.0, 708.0, 709.0, 710.0, 711.0, 712.0, 713.0,
      714.0, 715.0, 716.0, 717.0, 718.0, 719.0, 720.0, 721.0, 722.0, 723.0,
      724.0, 725.0, 726.0, 727.0, 728.0, 729.0, 730.0, 731.0, 732.0, 733.0,
      734.0, 735.0, 736.0, 737.0, 738.0, 739.0, 740.0, 741.0, 742.0, 743.0,
      744.0, 745.0, 746.0, 747.0, 748.0, 749.0, 750.0, 751.0, 752.0, 753.0,
      754.0, 755.0, 756.0, 757.0, 758.0, 759.0, 760.0, 761.0, 762.0, 763.0,
      764.0, 765.0, 766.0, 767.0, 768.0, 769.0, 770.0, 771.0, 772.0, 773.0,
      774.0, 775.0, 776.0, 777.0, 778.0, 779.0, 780.0, 781.0, 782.0, 783.0,
      784.0, 785.0, 786.0, 787.0, 788.0, 789.0, 790.0, 791.0, 792.0, 793.0,
      794.0, 795.0, 796.0, 797.0, 798.0, 799.0, 800.0, 801.0, 802.0, 803.0,
      804.0, 805.0, 806.0, 807.0, 808.0, 809.0, 810.0, 811.0, 812.0, 813.0,
      814.0, 815.0, 816.0, 817.0, 818.0, 819.0, 820.0, 821.0, 822.0, 823.0,
      824.0, 825.0, 826.0, 827.0, 828.0, 829.0, 830.0, 831.0, 832.0, 833.0,
      834.0, 835.0, 836.0, 837.0, 838.0, 839.0, 840.0, 841.0, 842.0, 843.0,
      844.0, 845.0, 846.0, 847.0, 848.0, 849.0, 850.0, 851.0, 852.0, 853.0,
      854.0, 855.0, 856.0, 857.0, 858.0, 859.0, 860.0, 861.0, 862.0, 863.0,
      864.0, 865.0, 866.0, 867.0, 868.0, 869.0, 870.0, 871.0, 872.0, 873.0,
      874.0, 875.0, 876.0, 877.0, 878.0, 879.0, 880.0, 881.0, 882.0, 883.0,
      884.0, 885.0, 886.0, 887.0, 888.0, 889.0, 890.0, 891.0, 892.0, 893.0,
      894.0, 895.0, 896.0, 897.0, 898.0, 899.0, 900.0, 901.0, 902.0, 903.0,
      904.0, 905.0, 906.0, 907.0, 908.0, 909.0, 910.0, 911.0, 912.0, 913.0,
      914.0, 915.0, 916.0, 917.0, 918.0, 919.0, 920.0, 921.0, 922.0, 923.0,
      924.0, 925.0, 926.0, 927.0, 928.0, 929.0, 930.0, 931.0, 932.0, 933.0,
      934.0, 935.0, 936.0, 937.0, 938.0, 939.0, 940.0, 941.0, 942.0, 943.0,
      944.0, 945.0, 946.0, 947.0, 948.0, 949.0, 950.0, 951.0, 952.0, 953.0,
      954.0, 955.0, 956.0, 957.0, 958.0, 959.0, 960.0, 961.0, 962.0, 963.0,
      964.0, 965.0, 966.0, 967.0, 968.0, 969.0, 970.0, 971.0, 972.0, 973.0,
      974.0, 975.0, 976.0, 977.0, 978.0, 979.0, 980.0, 981.0, 982.0, 983.0,
      984.0, 985.0, 986.0, 987.0, 988.0, 989.0, 990.0, 991.0, 992.0, 993.0,
      994.0, 995.0, 996.0, 997.0, 998.0, 999.0, 1000.0, 1001.0, 1002.0 } ;

    static real_T pDataValues0[] = { 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
      2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
      2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
      2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
      2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
      2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
      2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
      2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
      2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
      2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
      2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
      2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
      2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
      2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
      2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
      2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
      2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 2.0, 2.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0, 2.0,
      2.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 2.0 } ;

    multirate2_DWork.FromWorkspace_PWORK.TimePtr = (void *) pTimeValues0;
    multirate2_DWork.FromWorkspace_PWORK.DataPtr = (void *) pDataValues0;
    multirate2_DWork.FromWorkspace_IWORK.PrevIndex = 0;
  }
}

  /* Block: multirate2_pid_Sum2 */

  /* initialize task context */
{
  multirate2_pid_Sum2_1 = 0.0;
}

  /* Block: multirate2_pid_Saturation */

  /* initialize task context */
{
  multirate2_pid_Saturation_1 = 0.0;
}

  /* Block: multirate2_voltage */

#ifdef TASK_INITIALIZE_0001_COMPLETED
  TASK_INITIALIZE_0001_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0027_COMPLETED
  TASK_INITIALIZE_0027_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0026_COMPLETED
  TASK_INITIALIZE_0026_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0029_COMPLETED
  TASK_INITIALIZE_0029_COMPLETED();
#endif
  
  /* wait Preload Events */
  {
    CH__VEC(WAIT_TASKS,2);
    CH__VEC_INIT(WAIT_TASKS);
    while (1) {
      if (CH__BITCHECK(WAIT_TASKS,0) == 0
	  && CH_multirate2_0022_0027.flag__0022_0027[0])
	CH__BITSET(WAIT_TASKS,0);

      if (CH__BITCHECK(WAIT_TASKS,1) == 0
	  && CH_multirate2_0025_0027.flag__0025_0027[0])
	CH__BITSET(WAIT_TASKS,1);

      if (CH__COMPLETED(WAIT_TASKS,2)) {
	CH__END_LOG(WAIT_TASKS);
	break;
      }
      CH__TASK_SCHED(WAIT_TASKS,CH__VEC_NAME(WAIT_TASKS),2);
    }
  }

  /*
   * Set Alarm and Send start event
   */

  /* multirate2_Core1_1000_0_Task */
  OS__SETALARM(ALARM_multirate2_Core1_1000_0_Task,
	       (SwCnt0_TIMER_CLOCK_HZ / 1000) * 1000,
	       (SwCnt0_TIMER_CLOCK_HZ / 1000) * 1000);

  /* multirate2_Core2_1000_0_Task */
  OS__SETALARM(ALARM_multirate2_Core2_1000_0_Task,
	       (SwCnt0_TIMER_CLOCK_HZ / 1000) * 1000,
	       (SwCnt0_TIMER_CLOCK_HZ / 1000) * 1000);
  OS__SENDEVENT(TASK_multirate2_Core1_1000_0_Task,
		TASK_multirate2_Core2_1000_0_Task,
		SE_multirate2_Core2_1000_0_Task);

  /* multirate2_Core1_2000_0_Task */
  OS__SETALARM(ALARM_multirate2_Core1_2000_0_Task,
	       (SwCnt0_TIMER_CLOCK_HZ / 1000) * 2000,
	       (SwCnt0_TIMER_CLOCK_HZ / 1000) * 2000);
  OS__SENDEVENT(TASK_multirate2_Core1_1000_0_Task,
		TASK_multirate2_Core1_2000_0_Task,
		SE_multirate2_Core1_2000_0_Task);

  /* multirate2_Core1_3000_0_Task */
  OS__SETALARM(ALARM_multirate2_Core1_3000_0_Task,
	       (SwCnt0_TIMER_CLOCK_HZ / 1000) * 3000,
	       (SwCnt0_TIMER_CLOCK_HZ / 1000) * 3000);
  OS__SENDEVENT(TASK_multirate2_Core1_1000_0_Task,
		TASK_multirate2_Core1_3000_0_Task,
		SE_multirate2_Core1_3000_0_Task);

  /* multirate2_Core2_3000_0_Task */
  OS__SETALARM(ALARM_multirate2_Core2_3000_0_Task,
	       (SwCnt0_TIMER_CLOCK_HZ / 1000) * 3000,
	       (SwCnt0_TIMER_CLOCK_HZ / 1000) * 3000);
  OS__SENDEVENT(TASK_multirate2_Core1_1000_0_Task,
		TASK_multirate2_Core2_3000_0_Task,
		SE_multirate2_Core2_3000_0_Task);

  /* task loop */
  while (1) {
#ifdef TASK_multirate2_Core1_1000_0_Task_LOOP_TOP
    TASK_multirate2_Core1_1000_0_Task_LOOP_TOP
#endif
    /*
     * Block: multirate2_FromWorkspace
     * Task: 0001
     * rate: 1000
     */

    /* C code */
    {
{

  /* FromWorkspace: '<Root>/FromWorkspace' */
  {
    real_T *pDataValues = (real_T *)
      multirate2_DWork.FromWorkspace_PWORK.DataPtr;
    real_T *pTimeValues = (real_T *)
      multirate2_DWork.FromWorkspace_PWORK.TimePtr;
    int_T currTimeIndex = multirate2_DWork.FromWorkspace_IWORK.PrevIndex;
    real_T t = ((multirate2_M->Timing.clockTick0) );

    /* Get index */
    if (t <= pTimeValues[0]) {
      currTimeIndex = 0;
    } else if (t >= pTimeValues[1002]) {
      currTimeIndex = 1001;
    } else {
      if (t < pTimeValues[currTimeIndex]) {
        while (t < pTimeValues[currTimeIndex]) {
          currTimeIndex--;
        }
      } else {
        while (t >= pTimeValues[currTimeIndex + 1]) {
          currTimeIndex++;
        }
      }
    }

    multirate2_DWork.FromWorkspace_IWORK.PrevIndex = currTimeIndex;

    /* Post output */
    {
      real_T t1 = pTimeValues[currTimeIndex];
      real_T t2 = pTimeValues[currTimeIndex + 1];
      if (t1 == t2) {
        if (t < t1) {
          multirate2_FromWorkspace_1 = pDataValues[currTimeIndex];
        } else {
          multirate2_FromWorkspace_1 = pDataValues[currTimeIndex + 1];
        }
      } else {
        real_T f1 = (t2 - t) / (t2 - t1);
        real_T f2 = 1.0 - f1;
        real_T d1;
        real_T d2;
        int_T TimeIndex= currTimeIndex;
        d1 = pDataValues[TimeIndex];
        d2 = pDataValues[TimeIndex + 1];
        multirate2_FromWorkspace_1 = (real_T) rtInterpolate(d1, d2, f1, f2);
        pDataValues += 1003;
      }
    }
  }


}

    }

#ifdef TASK_EXECUTE_0001_COMPLETED
    TASK_EXECUTE_0001_COMPLETED();
#endif

    /* clear output channel bit vector */
    CH__VEC_INIT(OUT_0001);

    /* wait output channel */
    while (1) {
      /* output: multirate2_pid_D */
      if (CH__BITCHECK(OUT_0001,0) == 0 && CH_multirate2_0001_0004.flag__0001_0004 == 0) {
	CH_multirate2_0001_0004.multirate2_FromWorkspace_1 = multirate2_FromWorkspace_1;
	CH__SYNCM();
	CH_multirate2_0001_0004.flag__0001_0004 = 1;
	CH__EVENT(TASK_0001,IN_0004);
	CH__BITSET(OUT_0001,0);
      }

      /* output: multirate2_pid_RateTransition1 */
      if (CH__BITCHECK(OUT_0001,1) == 0 && CH__TIME_LE(task__time+1000, CH_multirate2_0001_0023.time__0001_0023)) {
	/* no send this period */
	CH__BITSET(OUT_0001,1);
      } else if (CH__BITCHECK(OUT_0001,1) == 0 && CH_multirate2_0001_0023.flag__0001_0023 == 0) {
	CH_multirate2_0001_0023.multirate2_FromWorkspace_1 = multirate2_FromWorkspace_1;
	CH__SYNCM();
	CH_multirate2_0001_0023.flag__0001_0023 = 1;
	CH_multirate2_0001_0023.time__0001_0023 += 2000;
	CH__EVENT(TASK_0001,IN_0023);
	CH__BITSET(OUT_0001,1);
      }

      /* output: multirate2_pid_RateTransition2 */
      if (CH__BITCHECK(OUT_0001,2) == 0 && CH__TIME_LE(task__time+1000, CH_multirate2_0001_0024.time__0001_0024)) {
	/* no send this period */
	CH__BITSET(OUT_0001,2);
      } else if (CH__BITCHECK(OUT_0001,2) == 0 && CH_multirate2_0001_0024.flag__0001_0024 == 0) {
	CH_multirate2_0001_0024.multirate2_FromWorkspace_1 = multirate2_FromWorkspace_1;
	CH__SYNCM();
	CH_multirate2_0001_0024.flag__0001_0024 = 1;
	CH_multirate2_0001_0024.time__0001_0024 += 3000;
	CH__EVENT(TASK_0001,IN_0024);
	CH__BITSET(OUT_0001,2);
      }

      if (CH__COMPLETED(OUT_0001,3)) {
	CH__END_LOG(OUT_0001);
	break;
      }

      CH__TASK_SCHED(OUT_0001,CH__VEC_NAME(OUT_0001),3);

      CH__MEM_BARRIER();
    }

#ifdef TASK_OUT_0001_COMPLETED
    TASK_OUT_0001_COMPLETED();
#endif
    /*
     * Block: multirate2_pid_Sum2
     * Task: 0027
     * rate: 1000
     */

    /* clear input channel bit vector */
    CH__VEC_INIT(IN_0027);

    /* wait input channel */
    while (1) {
      /* input: multirate2_pid_D */
      if (CH__BITCHECK(IN_0027,0) == 0 && CH_multirate2_0004_0027.flag__0004_0027 > 0) {
	int ch__flag;
	multirate2_pid_D_Gain3_1 = CH_multirate2_0004_0027.multirate2_pid_D_Gain3_1;
	ch__flag = CH_multirate2_0004_0027.flag__0004_0027 - 1;
	CH_multirate2_0004_0027.flag__0004_0027 = ch__flag;
	if (ch__flag == 0) {
	  CH__EVENT(TASK_0027, OUT_0004);
	}
	else
	  CH__SYNCM();
	CH__BITSET(IN_0027,0);
      }

      /* input: multirate2_pid_RateTransition */
      if (CH__BITCHECK(IN_0027,1) == 0 && CH_multirate2_0022_0027.flag__0022_0027[CH_multirate2_0022_0027.input_buffer_side] > 0) {
	int ch__flag;
	int ch__side = CH_multirate2_0022_0027.input_buffer_side;
	multirate2_pid_RateTransition_1 = CH_multirate2_0022_0027.multirate2_pid_RateTransition_1[ch__side];
	ch__flag = CH_multirate2_0022_0027.flag__0022_0027[ch__side] - 1;
	CH_multirate2_0022_0027.flag__0022_0027[ch__side] = ch__flag;
	if (ch__flag == 0) {
	  /* change side */
	  if (++ch__side == 2)
	    ch__side = 0;
	  CH_multirate2_0022_0027.input_buffer_side = ch__side;
	  CH__EVENT(TASK_0027, OUT_0022);
	}
	else
	  CH__SYNCM();
	CH__BITSET(IN_0027,1);
      }

      /* input: multirate2_pid_RateTransition3 */
      if (CH__BITCHECK(IN_0027,2) == 0 && CH_multirate2_0025_0027.flag__0025_0027[CH_multirate2_0025_0027.input_buffer_side] > 0) {
	int ch__flag;
	int ch__side = CH_multirate2_0025_0027.input_buffer_side;
	multirate2_pid_RateTransition3_1 = CH_multirate2_0025_0027.multirate2_pid_RateTransition3_1[ch__side];
	ch__flag = CH_multirate2_0025_0027.flag__0025_0027[ch__side] - 1;
	CH_multirate2_0025_0027.flag__0025_0027[ch__side] = ch__flag;
	if (ch__flag == 0) {
	  /* change side */
	  if (++ch__side == 2)
	    ch__side = 0;
	  CH_multirate2_0025_0027.input_buffer_side = ch__side;
	  CH__EVENT(TASK_0027, OUT_0025);
	}
	else
	  CH__SYNCM();
	CH__BITSET(IN_0027,2);
      }

      if (CH__COMPLETED(IN_0027,3)) {
	CH__END_LOG(IN_0027);
	break;
      }

      CH__TASK_SCHED(IN_0027,CH__VEC_NAME(IN_0027),3);

      CH__MEM_BARRIER();
    }

#ifdef TASK_IN_0027_COMPLETED
    TASK_IN_0027_COMPLETED();
#endif

    /* C code */
    {
{

  /* Sum: '<S1>/Sum2' */
  multirate2_pid_Sum2_1 = (multirate2_pid_RateTransition3_1 +
    multirate2_pid_D_Gain3_1) + multirate2_pid_RateTransition_1;


}

    }

#ifdef TASK_EXECUTE_0027_COMPLETED
    TASK_EXECUTE_0027_COMPLETED();
#endif
    /*
     * Block: multirate2_pid_Saturation
     * Task: 0026
     * rate: 1000
     */

    /* C code */
    {
{

  /* Saturate: '<S1>/Saturation' */
  u0 = multirate2_pid_Sum2_1;
  u1 = (-5.0);
  u2 = 5.0;
  if (u0 > u2) {
    multirate2_pid_Saturation_1 = u2;
  } else if (u0 < u1) {
    multirate2_pid_Saturation_1 = u1;
  } else {
    multirate2_pid_Saturation_1 = u0;
  }

  /* End of Saturate: '<S1>/Saturation' */

}

    }

#ifdef TASK_EXECUTE_0026_COMPLETED
    TASK_EXECUTE_0026_COMPLETED();
#endif
    /*
     * Block: multirate2_voltage
     * Task: 0029
     * rate: 1000
     */

#ifdef TASK_EXECUTE_0029_COMPLETED
    TASK_EXECUTE_0029_COMPLETED();
#endif

    /****************************************************
     *
     * You may insert some codes here for external output.
     *
     ****************************************************/

    /* Update Real-Time Model */
    {
  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The resolution of this integer timer is 1.0, which is the step size
   * of the task. Size of "clockTick0" ensures timer will not overflow during the
   * application lifespan selected.
   */
  multirate2_M->Timing.clockTick0++;
    }

#ifdef TASK_multirate2_Core1_1000_0_Task_BEFORE_SLEEP
    TASK_multirate2_Core1_1000_0_Task_BEFORE_SLEEP
#endif

    /* wait for next time */
    OS__WAITEVENT(TASK_multirate2_Core1_1000_0_Task,
		  WE_multirate2_Core1_1000_0_Task);

    task__time += 1000;

#ifdef TASK_multirate2_Core1_1000_0_Task_AFTER_SLEEP
    TASK_multirate2_Core1_1000_0_Task_AFTER_SLEEP
#endif
  }
}

CORE_TASK_CODE_PREPARE(2)
/* rate: 1000 */
/* Core#2: Rate: 1000 */
TASK(multirate2_Core2_1000_0_Task)
{
  /* task time */
  unsigned long task__time = 0;

  /* definitions of step function */
  real_T u0;
  real_T u1;
  real_T u2;

  /* end of definitions */
  /* omit input channel bit vector */
  /* CH__VEC(IN_0004,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0004,1); */

  /* states */
  struct {
    real_T UnitDelay1_DSTATE;
  } multirate2_DWork;

  /* input/output/auto variables */
  real_T multirate2_pid_D_Gain2_1;
  real_T multirate2_pid_D_Sum1_1;
  real_T multirate2_pid_D_UnitDelay1_1;
  real_T multirate2_FromWorkspace_1;
  real_T multirate2_pid_D_Gain3_1;

#ifdef TASK_multirate2_Core2_1000_0_Task_DEFINITIONS
  TASK_multirate2_Core2_1000_0_Task_DEFINITIONS
#endif

#ifdef TASK_0004_DEFINITIONS
  TASK_0004_DEFINITIONS
#endif

  /* initialize state structure */
  memset(&multirate2_DWork, 0, sizeof multirate2_DWork);

  /* Block: multirate2_pid_D */

  /* initialize states */
{
  multirate2_DWork.UnitDelay1_DSTATE = 0.0;
}

  /* initialize task context */
{
  multirate2_pid_D_UnitDelay1_1 = 0.0;
}
{
  multirate2_pid_D_Sum1_1 = 0.0;
}
{
  multirate2_pid_D_Gain2_1 = 0.0;
}
{
  multirate2_pid_D_Gain3_1 = 0.0;

}

#ifdef TASK_INITIALIZE_0004_COMPLETED
  TASK_INITIALIZE_0004_COMPLETED();
#endif

  /* wait start event */
  OS__WAITEVENT(TASK_multirate2_Core2_1000_0_Task,
		WE_multirate2_Core2_1000_0_Task);

  /* task loop */
  while (1) {
#ifdef TASK_multirate2_Core2_1000_0_Task_LOOP_TOP
    TASK_multirate2_Core2_1000_0_Task_LOOP_TOP
#endif
    /*
     * Block: multirate2_pid_D
     * Task: 0004
     * rate: 1000
     */

    /* wait input channel */
    while (1) {
      /* input: multirate2_FromWorkspace */
      if (CH_multirate2_0001_0004.flag__0001_0004 > 0) {
	int ch__flag;
	multirate2_FromWorkspace_1 = CH_multirate2_0001_0004.multirate2_FromWorkspace_1;
	ch__flag = CH_multirate2_0001_0004.flag__0001_0004 - 1;
	CH_multirate2_0001_0004.flag__0001_0004 = ch__flag;
	if (ch__flag == 0) {
	  CH__EVENT(TASK_0004, OUT_0001);
	}
	else
	  CH__SYNCM();
	CH__END_LOG(IN_0004);
	break;
      }

      CH__TASK_SCHED(IN_0004,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_IN_0004_COMPLETED
    TASK_IN_0004_COMPLETED();
#endif

    /* C code */
    {
{

  /* Outputs for Atomic SubSystem: '<S1>/D' */
  /* UnitDelay: '<S2>/UnitDelay1' */
  multirate2_pid_D_UnitDelay1_1 = multirate2_DWork.UnitDelay1_DSTATE;

  /* Sum: '<S2>/Sum1' */
  multirate2_pid_D_Sum1_1 = multirate2_FromWorkspace_1 -
    multirate2_pid_D_UnitDelay1_1;

  /* Gain: '<S2>/Gain2' */
  multirate2_pid_D_Gain2_1 = 0.0081000002101063728 * multirate2_pid_D_Sum1_1;

  /* Gain: '<S2>/Gain3' */
  multirate2_pid_D_Gain3_1 = 100.0 * multirate2_pid_D_Gain2_1;

  /* Update for UnitDelay: '<S2>/UnitDelay1' */
  multirate2_DWork.UnitDelay1_DSTATE = multirate2_FromWorkspace_1;

  /* End of Outputs for SubSystem: '<S1>/D' */

}

    }

#ifdef TASK_EXECUTE_0004_COMPLETED
    TASK_EXECUTE_0004_COMPLETED();
#endif

    /* wait output channel */
    while (1) {
      /* output: multirate2_pid_Sum2 */
      if (CH_multirate2_0004_0027.flag__0004_0027 == 0) {
	CH_multirate2_0004_0027.multirate2_pid_D_Gain3_1 = multirate2_pid_D_Gain3_1;
	CH__SYNCM();
	CH_multirate2_0004_0027.flag__0004_0027 = 1;
	CH__EVENT(TASK_0004,IN_0027);
	CH__END_LOG(OUT_0004);
	break;
      }

      CH__TASK_SCHED(OUT_0004,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_OUT_0004_COMPLETED
    TASK_OUT_0004_COMPLETED();
#endif

#ifdef TASK_multirate2_Core2_1000_0_Task_BEFORE_SLEEP
    TASK_multirate2_Core2_1000_0_Task_BEFORE_SLEEP
#endif

    /* wait for next time */
    OS__WAITEVENT(TASK_multirate2_Core2_1000_0_Task,
		  WE_multirate2_Core2_1000_0_Task);

    task__time += 1000;

#ifdef TASK_multirate2_Core2_1000_0_Task_AFTER_SLEEP
    TASK_multirate2_Core2_1000_0_Task_AFTER_SLEEP
#endif
  }
}

CORE_TASK_CODE_PREPARE(1)
/* rate: 2000 */
/* Core#1: Rate: 2000 */
TASK(multirate2_Core1_2000_0_Task)
{
  /* task time */
  unsigned long task__time = 0;

  /* definitions of step function */
  real_T u0;
  real_T u1;
  real_T u2;

  /* end of definitions */
  /* omit input channel bit vector */
  /* CH__VEC(IN_0022,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0022,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0023,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0023,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0011,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0011,1); */

  /* states */
  struct {
    real_T RateTransition_Buffer0;
    real_T UnitDelay_DSTATE;
  } multirate2_DWork;

  /* input/output/auto variables */
  real_T multirate2_pid_I_Product_1;
  real_T multirate2_pid_RateTransition_1;
  real_T multirate2_FromWorkspace_1;
  real_T multirate2_pid_RateTransition1_1;
  real_T multirate2_pid_I_Gain1_1;
  real_T multirate2_pid_I_Sum_1;
  real_T multirate2_pid_I_UnitDelay_1;

#ifdef TASK_multirate2_Core1_2000_0_Task_DEFINITIONS
  TASK_multirate2_Core1_2000_0_Task_DEFINITIONS
#endif

#ifdef TASK_0022_DEFINITIONS
  TASK_0022_DEFINITIONS
#endif

#ifdef TASK_0023_DEFINITIONS
  TASK_0023_DEFINITIONS
#endif

#ifdef TASK_0011_DEFINITIONS
  TASK_0011_DEFINITIONS
#endif

  /* initialize state structure */
  memset(&multirate2_DWork, 0, sizeof multirate2_DWork);

  /* Block: multirate2_pid_RateTransition */

  /* initialize states */
{
  multirate2_DWork.RateTransition_Buffer0 = 0.0;

}

  /* initialize task context */
{
  multirate2_pid_RateTransition_1 = 0.0;
}

  /* Block: multirate2_pid_RateTransition1 */

  /* initialize task context */
{
  multirate2_pid_RateTransition1_1 = 0.0;
}

  /* Block: multirate2_pid_I */

  /* initialize states */
{
  multirate2_DWork.UnitDelay_DSTATE = 0.0;
}

  /* initialize task context */
{
  multirate2_pid_I_UnitDelay_1 = 0.0;
}
{
  multirate2_pid_I_Gain1_1 = 0.0;
}
{
  multirate2_pid_I_Product_1 = 0.0;
}
{
  multirate2_pid_I_Sum_1 = 0.0;
}

#ifdef TASK_INITIALIZE_0022_COMPLETED
  TASK_INITIALIZE_0022_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0023_COMPLETED
  TASK_INITIALIZE_0023_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0011_COMPLETED
  TASK_INITIALIZE_0011_COMPLETED();
#endif

  /*
   * setup SampleTime Offset and Double buffer
   */

  /* setup double buffer planes */
  CH_multirate2_0022_0027.output_buffer_side = 0;
  CH_multirate2_0022_0027.input_buffer_side = 0;

  /* preload initial values */
  CH_multirate2_0022_0027.multirate2_pid_RateTransition_1[CH_multirate2_0022_0027.output_buffer_side] = multirate2_pid_RateTransition_1;
  CH__SYNCM();
  {
    unsigned long ch__sendcount;
    ch__sendcount = CH__SEND_COUNT(2000, 0, 1000);
    CH_multirate2_0022_0027.flag__0022_0027[0] = ch__sendcount;
    CH_multirate2_0022_0027.time__0022_0027 = ch__sendcount * 1000;
    CH_multirate2_0022_0027.output_buffer_side = 1;
  }
  CH__EVENT(TASK_0022,IN_0027);

  /* send Completed-Preload Event to the fastest task */
  OS__SENDEVENT(TASK_multirate2_Core1_2000_0_Task,
		TASK_WAIT_TASKS, SE_WAIT_TASKS);

  /* wait start event */
  OS__WAITEVENT(TASK_multirate2_Core1_2000_0_Task,
		WE_multirate2_Core1_2000_0_Task);

  /* task loop */
  while (1) {
#ifdef TASK_multirate2_Core1_2000_0_Task_LOOP_TOP
    TASK_multirate2_Core1_2000_0_Task_LOOP_TOP
#endif
    /*
     * Block: multirate2_pid_RateTransition1
     * Task: 0023
     * rate: 2000
     */

    /* wait input channel */
    while (1) {
      /* input: multirate2_FromWorkspace */
      if (CH_multirate2_0001_0023.flag__0001_0023 > 0) {
	int ch__flag;
	multirate2_FromWorkspace_1 = CH_multirate2_0001_0023.multirate2_FromWorkspace_1;
	ch__flag = CH_multirate2_0001_0023.flag__0001_0023 - 1;
	CH_multirate2_0001_0023.flag__0001_0023 = ch__flag;
	if (ch__flag == 0) {
	  CH__EVENT(TASK_0023, OUT_0001);
	}
	else
	  CH__SYNCM();
	CH__END_LOG(IN_0023);
	break;
      }

      CH__TASK_SCHED(IN_0023,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_IN_0023_COMPLETED
    TASK_IN_0023_COMPLETED();
#endif

    /* C code */
    {
{

  /* RateTransition: '<S1>/RateTransition1' */

    multirate2_pid_RateTransition1_1 = multirate2_FromWorkspace_1;


  /* End of RateTransition: '<S1>/RateTransition1' */

}

    }

#ifdef TASK_EXECUTE_0023_COMPLETED
    TASK_EXECUTE_0023_COMPLETED();
#endif
    /*
     * Block: multirate2_pid_I
     * Task: 0011
     * rate: 2000
     */

    /* C code */
    {
{

    /* Outputs for Atomic SubSystem: '<S1>/I' */
    /* UnitDelay: '<S3>/UnitDelay' */
    multirate2_pid_I_UnitDelay_1 = multirate2_DWork.UnitDelay_DSTATE;

    /* Gain: '<S3>/Gain1' */
    multirate2_pid_I_Gain1_1 = 0.38699999451637268 *
      multirate2_pid_I_UnitDelay_1;

    /* Product: '<S3>/Product' */
    multirate2_pid_I_Product_1 = multirate2_pid_I_Gain1_1;

    /* Sum: '<S3>/Sum' */
    multirate2_pid_I_Sum_1 = multirate2_pid_RateTransition1_1 +
      multirate2_pid_I_UnitDelay_1;

    /* Update for UnitDelay: '<S3>/UnitDelay' */
    multirate2_DWork.UnitDelay_DSTATE = multirate2_pid_I_Sum_1;

    /* End of Outputs for SubSystem: '<S1>/I' */

}

    }

#ifdef TASK_EXECUTE_0011_COMPLETED
    TASK_EXECUTE_0011_COMPLETED();
#endif
    /*
     * Block: multirate2_pid_RateTransition
     * Task: 0022
     * rate: 2000
     */

    /* Update */
    {
{

  /* Update for RateTransition: '<S1>/RateTransition' */

    multirate2_DWork.RateTransition_Buffer0 = multirate2_pid_I_Product_1;

}

    }

#ifdef TASK_UPDATE_0022_COMPLETED
    TASK_UPDATE_0022_COMPLETED();
#endif

    /* C code */
    {
{

  /* RateTransition: '<S1>/RateTransition' */

    multirate2_pid_RateTransition_1 = multirate2_DWork.RateTransition_Buffer0;

  /* End of RateTransition: '<S1>/RateTransition' */
  /* End of Update for RateTransition: '<S1>/RateTransition' */

}

    }

#ifdef TASK_EXECUTE_0022_COMPLETED
    TASK_EXECUTE_0022_COMPLETED();
#endif

    /* wait output channel */
    while (1) {
      /* output: multirate2_pid_Sum2 */
      if (CH_multirate2_0022_0027.flag__0022_0027[CH_multirate2_0022_0027.output_buffer_side] == 0) {
	unsigned long ch__sendcount;
	CH_multirate2_0022_0027.multirate2_pid_RateTransition_1[CH_multirate2_0022_0027.output_buffer_side] = multirate2_pid_RateTransition_1;
	CH__SYNCM();
	ch__sendcount = CH__SEND_COUNT(task__time+2000*2, CH_multirate2_0022_0027.time__0022_0027, 1000);
	CH_multirate2_0022_0027.flag__0022_0027[CH_multirate2_0022_0027.output_buffer_side] = ch__sendcount;
	CH_multirate2_0022_0027.time__0022_0027 += ch__sendcount * 1000;
	/* change side */
	int ch__side = CH_multirate2_0022_0027.output_buffer_side;
	if (++ch__side == 2)
	  ch__side = 0;
	CH_multirate2_0022_0027.output_buffer_side = ch__side;
	CH__EVENT(TASK_0022,IN_0027);
	CH__END_LOG(OUT_0022);
	break;
      }

      CH__TASK_SCHED(OUT_0022,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_OUT_0022_COMPLETED
    TASK_OUT_0022_COMPLETED();
#endif

#ifdef TASK_multirate2_Core1_2000_0_Task_BEFORE_SLEEP
    TASK_multirate2_Core1_2000_0_Task_BEFORE_SLEEP
#endif

    /* wait for next time */
    OS__WAITEVENT(TASK_multirate2_Core1_2000_0_Task,
		  WE_multirate2_Core1_2000_0_Task);

    task__time += 2000;

#ifdef TASK_multirate2_Core1_2000_0_Task_AFTER_SLEEP
    TASK_multirate2_Core1_2000_0_Task_AFTER_SLEEP
#endif
  }
}

CORE_TASK_CODE_PREPARE(1)
/* rate: 3000 */
/* Core#1: Rate: 3000 */
TASK(multirate2_Core1_3000_0_Task)
{
  /* task time */
  unsigned long task__time = 0;

  /* definitions of step function */
  real_T u0;
  real_T u1;
  real_T u2;

  /* end of definitions */
  /* omit input channel bit vector */
  /* CH__VEC(IN_0024,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0024,1); */

  /* input/output/auto variables */
  real_T multirate2_FromWorkspace_1;
  real_T multirate2_pid_RateTransition2_1;

#ifdef TASK_multirate2_Core1_3000_0_Task_DEFINITIONS
  TASK_multirate2_Core1_3000_0_Task_DEFINITIONS
#endif

#ifdef TASK_0024_DEFINITIONS
  TASK_0024_DEFINITIONS
#endif

  /* Block: multirate2_pid_RateTransition2 */

  /* initialize task context */
{
  multirate2_pid_RateTransition2_1 = 0.0;
}

#ifdef TASK_INITIALIZE_0024_COMPLETED
  TASK_INITIALIZE_0024_COMPLETED();
#endif

  /* wait start event */
  OS__WAITEVENT(TASK_multirate2_Core1_3000_0_Task,
		WE_multirate2_Core1_3000_0_Task);

  /* task loop */
  while (1) {
#ifdef TASK_multirate2_Core1_3000_0_Task_LOOP_TOP
    TASK_multirate2_Core1_3000_0_Task_LOOP_TOP
#endif
    /*
     * Block: multirate2_pid_RateTransition2
     * Task: 0024
     * rate: 3000
     */

    /* wait input channel */
    while (1) {
      /* input: multirate2_FromWorkspace */
      if (CH_multirate2_0001_0024.flag__0001_0024 > 0) {
	int ch__flag;
	multirate2_FromWorkspace_1 = CH_multirate2_0001_0024.multirate2_FromWorkspace_1;
	ch__flag = CH_multirate2_0001_0024.flag__0001_0024 - 1;
	CH_multirate2_0001_0024.flag__0001_0024 = ch__flag;
	if (ch__flag == 0) {
	  CH__EVENT(TASK_0024, OUT_0001);
	}
	else
	  CH__SYNCM();
	CH__END_LOG(IN_0024);
	break;
      }

      CH__TASK_SCHED(IN_0024,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_IN_0024_COMPLETED
    TASK_IN_0024_COMPLETED();
#endif

    /* C code */
    {
{

  /* RateTransition: '<S1>/RateTransition2' */

    multirate2_pid_RateTransition2_1 = multirate2_FromWorkspace_1;


  /* End of RateTransition: '<S1>/RateTransition2' */

}

    }

#ifdef TASK_EXECUTE_0024_COMPLETED
    TASK_EXECUTE_0024_COMPLETED();
#endif

    /* wait output channel */
    while (1) {
      /* output: multirate2_pid_P */
      if (CH_multirate2_0024_0018.flag__0024_0018 == 0) {
	CH_multirate2_0024_0018.multirate2_pid_RateTransition2_1 = multirate2_pid_RateTransition2_1;
	CH__SYNCM();
	CH_multirate2_0024_0018.flag__0024_0018 = 1;
	CH__EVENT(TASK_0024,IN_0018);
	CH__END_LOG(OUT_0024);
	break;
      }

      CH__TASK_SCHED(OUT_0024,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_OUT_0024_COMPLETED
    TASK_OUT_0024_COMPLETED();
#endif

#ifdef TASK_multirate2_Core1_3000_0_Task_BEFORE_SLEEP
    TASK_multirate2_Core1_3000_0_Task_BEFORE_SLEEP
#endif

    /* wait for next time */
    OS__WAITEVENT(TASK_multirate2_Core1_3000_0_Task,
		  WE_multirate2_Core1_3000_0_Task);

    task__time += 3000;

#ifdef TASK_multirate2_Core1_3000_0_Task_AFTER_SLEEP
    TASK_multirate2_Core1_3000_0_Task_AFTER_SLEEP
#endif
  }
}

CORE_TASK_CODE_PREPARE(2)
/* rate: 3000 */
/* Core#2: Rate: 3000 */
TASK(multirate2_Core2_3000_0_Task)
{
  /* task time */
  unsigned long task__time = 0;

  /* definitions of step function */
  real_T u0;
  real_T u1;
  real_T u2;

  /* end of definitions */
  /* omit input channel bit vector */
  /* CH__VEC(IN_0025,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0025,1); */

  /* omit input channel bit vector */
  /* CH__VEC(IN_0018,1); */

  /* omit output channel bit vector */
  /* CH__VEC(OUT_0018,1); */

  /* states */
  struct {
    real_T RateTransition3_Buffer0;
  } multirate2_DWork;

  /* input/output/auto variables */
  real_T multirate2_pid_P_Gain_1;
  real_T multirate2_pid_RateTransition3_1;
  real_T multirate2_pid_RateTransition2_1;

#ifdef TASK_multirate2_Core2_3000_0_Task_DEFINITIONS
  TASK_multirate2_Core2_3000_0_Task_DEFINITIONS
#endif

#ifdef TASK_0025_DEFINITIONS
  TASK_0025_DEFINITIONS
#endif

#ifdef TASK_0018_DEFINITIONS
  TASK_0018_DEFINITIONS
#endif

  /* initialize state structure */
  memset(&multirate2_DWork, 0, sizeof multirate2_DWork);

  /* Block: multirate2_pid_RateTransition3 */

  /* initialize states */
{
  multirate2_DWork.RateTransition3_Buffer0 = 0.0;
}

  /* initialize task context */
{
  multirate2_pid_RateTransition3_1 = 0.0;
}

  /* Block: multirate2_pid_P */

  /* initialize task context */
{
  multirate2_pid_P_Gain_1 = 0.0;
}

#ifdef TASK_INITIALIZE_0025_COMPLETED
  TASK_INITIALIZE_0025_COMPLETED();
#endif

#ifdef TASK_INITIALIZE_0018_COMPLETED
  TASK_INITIALIZE_0018_COMPLETED();
#endif

  /*
   * setup SampleTime Offset and Double buffer
   */

  /* setup double buffer planes */
  CH_multirate2_0025_0027.output_buffer_side = 0;
  CH_multirate2_0025_0027.input_buffer_side = 0;

  /* preload initial values */
  CH_multirate2_0025_0027.multirate2_pid_RateTransition3_1[CH_multirate2_0025_0027.output_buffer_side] = multirate2_pid_RateTransition3_1;
  CH__SYNCM();
  {
    unsigned long ch__sendcount;
    ch__sendcount = CH__SEND_COUNT(3000, 0, 1000);
    CH_multirate2_0025_0027.flag__0025_0027[0] = ch__sendcount;
    CH_multirate2_0025_0027.time__0025_0027 = ch__sendcount * 1000;
    CH_multirate2_0025_0027.output_buffer_side = 1;
  }
  CH__EVENT(TASK_0025,IN_0027);

  /* send Completed-Preload Event to the fastest task */
  OS__SENDEVENT(TASK_multirate2_Core2_3000_0_Task,
		TASK_WAIT_TASKS, SE_WAIT_TASKS);

  /* wait start event */
  OS__WAITEVENT(TASK_multirate2_Core2_3000_0_Task,
		WE_multirate2_Core2_3000_0_Task);

  /* task loop */
  while (1) {
#ifdef TASK_multirate2_Core2_3000_0_Task_LOOP_TOP
    TASK_multirate2_Core2_3000_0_Task_LOOP_TOP
#endif
    /*
     * Block: multirate2_pid_P
     * Task: 0018
     * rate: 3000
     */

    /* wait input channel */
    while (1) {
      /* input: multirate2_pid_RateTransition2 */
      if (CH_multirate2_0024_0018.flag__0024_0018 > 0) {
	int ch__flag;
	multirate2_pid_RateTransition2_1 = CH_multirate2_0024_0018.multirate2_pid_RateTransition2_1;
	ch__flag = CH_multirate2_0024_0018.flag__0024_0018 - 1;
	CH_multirate2_0024_0018.flag__0024_0018 = ch__flag;
	if (ch__flag == 0) {
	  CH__EVENT(TASK_0018, OUT_0024);
	}
	else
	  CH__SYNCM();
	CH__END_LOG(IN_0018);
	break;
      }

      CH__TASK_SCHED(IN_0018,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_IN_0018_COMPLETED
    TASK_IN_0018_COMPLETED();
#endif

    /* C code */
    {
{

    /* Outputs for Atomic SubSystem: '<S1>/P' */
    /* Gain: '<S4>/Gain' */
    multirate2_pid_P_Gain_1 = 1.5299999713897705 *
      multirate2_pid_RateTransition2_1;

    /* End of Outputs for SubSystem: '<S1>/P' */

}

    }

#ifdef TASK_EXECUTE_0018_COMPLETED
    TASK_EXECUTE_0018_COMPLETED();
#endif
    /*
     * Block: multirate2_pid_RateTransition3
     * Task: 0025
     * rate: 3000
     */

    /* Update */
    {
{

    /* Update for RateTransition: '<S1>/RateTransition3' */
    multirate2_DWork.RateTransition3_Buffer0 = multirate2_pid_P_Gain_1;

}

    }

#ifdef TASK_UPDATE_0025_COMPLETED
    TASK_UPDATE_0025_COMPLETED();
#endif

    /* C code */
    {
{

  /* RateTransition: '<S1>/RateTransition3' */

    multirate2_pid_RateTransition3_1 = multirate2_DWork.RateTransition3_Buffer0;

  /* End of RateTransition: '<S1>/RateTransition3' */

}

    }

#ifdef TASK_EXECUTE_0025_COMPLETED
    TASK_EXECUTE_0025_COMPLETED();
#endif

    /* wait output channel */
    while (1) {
      /* output: multirate2_pid_Sum2 */
      if (CH_multirate2_0025_0027.flag__0025_0027[CH_multirate2_0025_0027.output_buffer_side] == 0) {
	unsigned long ch__sendcount;
	CH_multirate2_0025_0027.multirate2_pid_RateTransition3_1[CH_multirate2_0025_0027.output_buffer_side] = multirate2_pid_RateTransition3_1;
	CH__SYNCM();
	ch__sendcount = CH__SEND_COUNT(task__time+3000*2, CH_multirate2_0025_0027.time__0025_0027, 1000);
	CH_multirate2_0025_0027.flag__0025_0027[CH_multirate2_0025_0027.output_buffer_side] = ch__sendcount;
	CH_multirate2_0025_0027.time__0025_0027 += ch__sendcount * 1000;
	/* change side */
	int ch__side = CH_multirate2_0025_0027.output_buffer_side;
	if (++ch__side == 2)
	  ch__side = 0;
	CH_multirate2_0025_0027.output_buffer_side = ch__side;
	CH__EVENT(TASK_0025,IN_0027);
	CH__END_LOG(OUT_0025);
	break;
      }

      CH__TASK_SCHED(OUT_0025,NULL,1);

      CH__MEM_BARRIER();
    }

#ifdef TASK_OUT_0025_COMPLETED
    TASK_OUT_0025_COMPLETED();
#endif

#ifdef TASK_multirate2_Core2_3000_0_Task_BEFORE_SLEEP
    TASK_multirate2_Core2_3000_0_Task_BEFORE_SLEEP
#endif

    /* wait for next time */
    OS__WAITEVENT(TASK_multirate2_Core2_3000_0_Task,
		  WE_multirate2_Core2_3000_0_Task);

    task__time += 3000;

#ifdef TASK_multirate2_Core2_3000_0_Task_AFTER_SLEEP
    TASK_multirate2_Core2_3000_0_Task_AFTER_SLEEP
#endif
  }
}
