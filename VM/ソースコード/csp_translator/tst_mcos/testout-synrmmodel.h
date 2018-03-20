#include "testout-synrmmodel_res.c"

#define TASK_EXECUTE_0189_COMPLETED()					\
  do {									\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_C2A_1:%f\n",	\
		      mcos__cycle_step, synrmmodel_mt_RTW_Subsystem2_C2A_1);		\
    mcos__cycle_step++;							\
  } while (0)

#define TASK_EXECUTE_0383_COMPLETED()					\
  mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Compare_1:%u\n", \
		    mcos__cycle_step, synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Compare_1)

#define TASK_EXECUTE_0416_COMPLETED()				\
  mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_calc_rpm_Gain_1:%f\n", \
		    mcos__cycle_step, synrmmodel_mt_RTW_Subsystem2_calc_rpm_Gain_1)

