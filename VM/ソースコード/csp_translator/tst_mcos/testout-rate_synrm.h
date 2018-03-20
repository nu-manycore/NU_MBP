#ifndef SEQUENCIAL_PROGRAM
#include "testout-rate_synrm_res.c"
#define INC_MCOS__CYCLE_STEP()	do { mcos__cycle_step++; } while (0)
#else /* SEQUENCIAL_PROGRAM */
#define INC_MCOS__CYCLE_STEP()	do { ; } while (0)
#endif /* SEQUENCIAL_PROGRAM */

/* synrmmodel_mt_RTW_Subsystem2_C2A */
#define TASK_EXECUTE_0189_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_C2A_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_C2A_1);\
    INC_MCOS__CYCLE_STEP();\
  } while (0)

/* synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Compare */
#define TASK_EXECUTE_0194_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Compare_1:%u\n", mcos__cycle_step, (unsigned int)synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Compare_1);\
  } while (0)

/* DiscretePulseGenerator:synrmmodel_mt_RTW_Subsystem2_ModeSelector */
#define TASK_EXECUTE_0266_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_ModeSelector_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_ModeSelector_1);\
  } while (0)

/* DiscretePulseGenerator:synrmmodel_mt_RTW_Subsystem2_PulseGenerator3 */
#define TASK_EXECUTE_0335_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PulseGenerator3_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PulseGenerator3_1);\
  } while (0)

/* DiscretePulseGenerator:synrmmodel_mt_RTW_Subsystem2_Reffreq */
#define TASK_EXECUTE_0354_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Reffreq_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Reffreq_1);\
  } while (0)

/* synrmmodel_mt_RTW_Subsystem2_calc_rpm_Gain */
#define TASK_EXECUTE_0416_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_calc_rpm_Gain_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_calc_rpm_Gain_1);\
  } while (0)
