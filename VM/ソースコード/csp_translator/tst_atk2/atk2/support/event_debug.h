#ifndef EVENT_DEBUG_H
#define EVENT_DEBUG_H

#ifndef ENABLE_CH__DEBUG
#define ENABLE_CH__DEBUG	0
#endif

#ifndef ENABLE_OSAPI__DEBUG
#define ENABLE_OSAPI__DEBUG	0
#endif

#ifndef OS_WRAPPER___MKSTR
#define OS_WRAPPER___MKSTR(X)	#X
#endif

#define CH__MKSTR(S) OS_WRAPPER___MKSTR(S)

#if ENABLE_CH__DEBUG
#define CH__END_LOG(C)							\
  do {									\
    TickType tick__0;							\
    GetCounterValue(MAIN_HW_COUNTER_CORE0, &tick__0);			\
    syslog(LOG_INFO, "[%lu:%lu]:%d:" CH__VEC_CONCAT(ATK2_TNAME_,C)	\
	   ":" CH__MKSTR(C) ":END", task__time, tick__0, __LINE__);	\
  } while (0)

#define CH__EVENT(T,C)							\
  do {									\
    TickType tick__0;							\
    GetCounterValue(MAIN_HW_COUNTER_CORE0, &tick__0);			\
    syslog(LOG_INFO, "[%lu:%lu]:%d:" CH__VEC_CONCAT(ATK2_TNAME_,T)	\
	   "(" CH__VEC_CONCAT(ATK2_TID_,T) ")"				\
	   ":" CH__VEC_CONCAT(ATK2_TNAME_,C) ":" CH__MKSTR(C)		\
	   ":%s", task__time, tick__0, __LINE__,			\
	   CH__MKSTR(C)[0] == 'I' ? "SEND" : "ACK");			\
    CH__SetEvent(CH__VEC_CONCAT(ATK2_SE_,C));				\
} while (0)

#define CH__TASK_SCHED(C,V,S)						\
  do {									\
    unsigned long *ch___vec = (V);					\
    TickType tick__0;							\
    GetCounterValue(MAIN_HW_COUNTER_CORE0, &tick__0);			\
    if (ch___vec)							\
      syslog(LOG_INFO, "[%lu:%lu]:%d:" CH__VEC_CONCAT(ATK2_TNAME_,C)	\
	     ":" CH__MKSTR(C) ":WAIT(%08x,%d)",				\
	     task__time, tick__0, __LINE__, ch___vec[0], S);		\
    else								\
      syslog(LOG_INFO, "[%lu:%lu]:%d:" CH__VEC_CONCAT(ATK2_TNAME_,C)	\
	     ":" CH__MKSTR(C) ":WAIT", task__time, tick__0, __LINE__);	\
    WaitEvent(CH__VEC_CONCAT(ATK2_WE_,C));				\
    ClearEvent(CH__VEC_CONCAT(ATK2_WE_,C));				\
    GetCounterValue(MAIN_HW_COUNTER_CORE0, &tick__0);			\
    syslog(LOG_INFO, "[%lu:%lu]:%d:" CH__VEC_CONCAT(ATK2_TNAME_,C)	\
	   ":" CH__MKSTR(C) ":RUN", task__time, tick__0, __LINE__);	\
} while (0)

#endif	/* ENABLE_CH__DEBUG */

#if ENABLE_OSAPI__DEBUG
#define OS__WAITEVENT(T,E) \
  do {\
    TickType tick__0;\
    GetCounterValue(MAIN_HW_COUNTER_CORE0, &tick__0);\
    syslog(LOG_INFO, "[%lu:%lu]:%d:" CH__VEC_CONCAT(ATK2_TNAME_,T) \
           ":(wakeup):WAIT", task__time, tick__0, __LINE__); \
    WaitEvent(CH__VEC_CONCAT(ATK2_,E)); \
    ClearEvent(CH__VEC_CONCAT(ATK2_,E)); \
    GetCounterValue(MAIN_HW_COUNTER_CORE0, &tick__0);\
    syslog(LOG_INFO, "[%lu:%lu]:%d:" CH__VEC_CONCAT(ATK2_TNAME_,T) \
           ":(wakeup):RUN", task__time, tick__0, __LINE__); \
  } while (0)

#define OS__SENDEVENT(T,R,E) \
  do {\
    TickType tick__0;\
    GetCounterValue(MAIN_HW_COUNTER_CORE0, &tick__0);\
    syslog(LOG_INFO, "[%lu:%lu]:%d:" CH__VEC_CONCAT(ATK2_TNAME_,T) \
           ":" CH__VEC_CONCAT(ATK2_TNAME_,R) ":SEND",\
	   task__time, tick__0, __LINE__); \
    SetEvent(CH__VEC_CONCAT(ATK2_,R),CH__VEC_CONCAT(ATK2_,E));\
  } while (0)

#endif	/* ENABLE_OSAPI__DEBUG */

#endif	/* EVENT_DEBUG_H */
