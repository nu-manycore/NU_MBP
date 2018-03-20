/* FromWorkspace:multirate2_FromWorkspace */
#define TASK_EXECUTE_0001_COMPLETED()\
  do {\
    syslog (LOG_INFO, "[%lu]"ATK2_TNAME_TASK_0001"(0001)", task__time);\
  } while (0);

/* SubSystem:multirate2_pid */
#define TASK_EXECUTE_0002_COMPLETED()\
  do {\
    syslog (LOG_INFO, "[%lu]"ATK2_TNAME_TASK_0002"(0002)", task__time);\
  } while (0);

/* Inport:multirate2_pid_error */
#define TASK_EXECUTE_0003_COMPLETED()\
  do {\
    syslog (LOG_INFO, "[%lu]"ATK2_TNAME_TASK_0003"(0003)", task__time);\
  } while (0);

/* SubSystem:multirate2_pid_D */
#define TASK_EXECUTE_0004_COMPLETED()\
  do {\
    syslog (LOG_INFO, "[%lu]"ATK2_TNAME_TASK_0004"(0004)", task__time);\
  } while (0);

/* SubSystem:multirate2_pid_I */
#define TASK_EXECUTE_0011_COMPLETED()\
  do {\
    syslog (LOG_INFO, "[%lu]"ATK2_TNAME_TASK_0011"(0011)", task__time);\
  } while (0);

/* SubSystem:multirate2_pid_P */
#define TASK_EXECUTE_0018_COMPLETED()\
  do {\
    syslog (LOG_INFO, "[%lu]"ATK2_TNAME_TASK_0018"(0018)", task__time);\
  } while (0);

/* RateTransition:multirate2_pid_RateTransition */
#define TASK_EXECUTE_0022_COMPLETED()\
  do {\
    syslog (LOG_INFO, "[%lu]"ATK2_TNAME_TASK_0022"(0022)", task__time);\
  } while (0);

/* RateTransition:multirate2_pid_RateTransition1 */
#define TASK_EXECUTE_0023_COMPLETED()\
  do {\
    syslog (LOG_INFO, "[%lu]"ATK2_TNAME_TASK_0023"(0023)", task__time);\
  } while (0);

/* RateTransition:multirate2_pid_RateTransition2 */
#define TASK_EXECUTE_0024_COMPLETED()\
  do {\
    syslog (LOG_INFO, "[%lu]"ATK2_TNAME_TASK_0024"(0024)", task__time);\
  } while (0);

/* RateTransition:multirate2_pid_RateTransition3 */
#define TASK_EXECUTE_0025_COMPLETED()\
  do {\
    syslog (LOG_INFO, "[%lu]"ATK2_TNAME_TASK_0025"(0025)", task__time);\
  } while (0);

/* Saturate:multirate2_pid_Saturation */
#define TASK_EXECUTE_0026_COMPLETED()\
  do {\
    syslog (LOG_INFO, "[%lu]"ATK2_TNAME_TASK_0026"(0026)", task__time);\
  } while (0);

/* Sum:multirate2_pid_Sum2 */
#define TASK_EXECUTE_0027_COMPLETED()\
  do {\
    syslog (LOG_INFO, "[%lu]"ATK2_TNAME_TASK_0027"(0027)", task__time);\
  } while (0);

/* Outport:multirate2_pid_voltage */
#define TASK_EXECUTE_0028_COMPLETED()\
  do {\
    syslog (LOG_INFO, "[%lu]"ATK2_TNAME_TASK_0028"(0028)", task__time);\
  } while (0);

/* Outport:multirate2_voltage */
#define TASK_EXECUTE_0029_COMPLETED()\
  do {\
    syslog (LOG_INFO, "[%lu]"ATK2_TNAME_TASK_0029"(0029)", task__time);\
  } while (0);
