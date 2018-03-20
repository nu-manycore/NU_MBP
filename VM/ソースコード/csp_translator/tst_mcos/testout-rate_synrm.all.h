#ifndef SEQUENCIAL_PROGRAM
#include "testout-rate_synrm_res.c"
#define INC_MCOS__CYCLE_STEP()	do { mcos__cycle_step++; } while (0)
#else /* SEQUENCIAL_PROGRAM */
#define INC_MCOS__CYCLE_STEP()	do { ; } while (0)
#endif /* SEQUENCIAL_PROGRAM */

/* Clock:synrmmodel_mt_RTW_DataStore_Clock1 */
#define TASK_EXECUTE_0002_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_DataStore_Clock1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_DataStore_Clock1_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_IPMSM1_Constant */
#define TASK_EXECUTE_0023_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_IPMSM1_Constant_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_IPMSM1_Constant_1);\
  } while (0)

/* S-Function:synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder */
#define TASK_EXECUTE_0024_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_1);\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_2:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_2);\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_3:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_3);\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_4:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_4);\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_5:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_5);\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_6:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_6);\
  } while (0)

/* S-Function:synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_S_FunctionBuilder */
#define TASK_EXECUTE_0038_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_S_FunctionBuilder_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_S_FunctionBuilder_1);\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_S_FunctionBuilder_2:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_S_FunctionBuilder_2);\
  } while (0)

/* Gain:synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_m2e */
#define TASK_EXECUTE_0039_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_m2e_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_m2e_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_wrmerr */
#define TASK_EXECUTE_0040_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_wrmerr_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_wrmerr_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_MotorPara1_Constant */
#define TASK_EXECUTE_0046_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_MotorPara1_Constant_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_MotorPara1_Constant_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_MotorPara1_Constant1 */
#define TASK_EXECUTE_0047_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_MotorPara1_Constant1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_MotorPara1_Constant1_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_MotorPara1_Constant2 */
#define TASK_EXECUTE_0048_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_MotorPara1_Constant2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_MotorPara1_Constant2_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Observer_observerab_Constant */
#define TASK_EXECUTE_0069_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Observer_observerab_Constant_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Observer_observerab_Constant_1);\
  } while (0)

/* S-Function:synrmmodel_mt_RTW_Observer_observerab_S_FunctionBuilder1 */
#define TASK_EXECUTE_0070_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Observer_observerab_S_FunctionBuilder1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Observer_observerab_S_FunctionBuilder1_1);\
  } while (0)

/* S-Function:synrmmodel_mt_RTW_Observer_observerab_S_FunctionBuilder2 */
#define TASK_EXECUTE_0071_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Observer_observerab_S_FunctionBuilder2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Observer_observerab_S_FunctionBuilder2_1);\
  } while (0)

/* S-Function:synrmmodel_mt_RTW_Observer_observerab_Subsystem_S_FunctionBuilder */
#define TASK_EXECUTE_0081_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Observer_observerab_Subsystem_S_FunctionBuilder_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Observer_observerab_Subsystem_S_FunctionBuilder_1);\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Observer_observerab_Subsystem_S_FunctionBuilder_2:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Observer_observerab_Subsystem_S_FunctionBuilder_2);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Observer_observerab_Subsystem_Sum */
#define TASK_EXECUTE_0082_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Observer_observerab_Subsystem_Sum_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Observer_observerab_Subsystem_Sum_1);\
  } while (0)

/* UnitDelay:synrmmodel_mt_RTW_Observer_observerab_UnitDelay1 */
#define TASK_EXECUTE_0086_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Observer_observerab_UnitDelay1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Observer_observerab_UnitDelay1_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Outputwtheta1_Constant */
#define TASK_EXECUTE_0100_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Outputwtheta1_Constant_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Outputwtheta1_Constant_1);\
  } while (0)

/* S-Function:synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder */
#define TASK_EXECUTE_0108_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_1);\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_2:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_2);\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_3:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_3);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Outputwtheta1_Subsystem_Sum */
#define TASK_EXECUTE_0109_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Outputwtheta1_Subsystem_Sum_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Outputwtheta1_Subsystem_Sum_1);\
  } while (0)

/* UnitDelay:synrmmodel_mt_RTW_Outputwtheta1_UnitDelay1 */
#define TASK_EXECUTE_0114_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Outputwtheta1_UnitDelay1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Outputwtheta1_UnitDelay1_1);\
  } while (0)

/* Gain:synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_M2E */
#define TASK_EXECUTE_0133_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_M2E_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_M2E_1);\
  } while (0)

/* S-Function:synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_S_FunctionBuilder */
#define TASK_EXECUTE_0134_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_S_FunctionBuilder_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_S_FunctionBuilder_1);\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_S_FunctionBuilder_2:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_S_FunctionBuilder_2);\
  } while (0)

/* Fcn:synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_ab2dq_fa */
#define TASK_EXECUTE_0140_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_ab2dq_fa_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_ab2dq_fa_1);\
  } while (0)

/* Fcn:synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_ab2dq_fb */
#define TASK_EXECUTE_0141_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_ab2dq_fb_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_ab2dq_fb_1);\
  } while (0)

/* Product:synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product4 */
#define TASK_EXECUTE_0148_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product4_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product4_1);\
  } while (0)

/* Product:synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product5 */
#define TASK_EXECUTE_0149_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product5_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product5_1);\
  } while (0)

/* Product:synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product6 */
#define TASK_EXECUTE_0150_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product6_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product6_1);\
  } while (0)

/* Product:synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product7 */
#define TASK_EXECUTE_0151_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product7_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product7_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Subtract1 */
#define TASK_EXECUTE_0152_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Subtract1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Subtract1_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Sum1 */
#define TASK_EXECUTE_0153_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Sum1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Sum1_1);\
  } while (0)

/* Fcn:synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_fa1 */
#define TASK_EXECUTE_0154_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_fa1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_fa1_1);\
  } while (0)

/* Fcn:synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_fa2 */
#define TASK_EXECUTE_0155_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_fa2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_fa2_1);\
  } while (0)

/* Abs:synrmmodel_mt_RTW_Subsystem2_Abs */
#define TASK_EXECUTE_0187_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Abs_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Abs_1);\
  } while (0)

/* Bias:synrmmodel_mt_RTW_Subsystem2_Bias */
#define TASK_EXECUTE_0188_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Bias_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Bias_1);\
  } while (0)

/* ComplexToMagnitudeAngle:synrmmodel_mt_RTW_Subsystem2_C2A */
#define TASK_EXECUTE_0189_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_C2A_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_C2A_1);\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_C2A_2:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_C2A_2);\
    INC_MCOS__CYCLE_STEP();\
  } while (0)

/* ComplexToMagnitudeAngle:synrmmodel_mt_RTW_Subsystem2_C2A1 */
#define TASK_EXECUTE_0190_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_C2A1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_C2A1_1);\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_C2A1_2:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_C2A1_2);\
  } while (0)

/* ComplexToMagnitudeAngle:synrmmodel_mt_RTW_Subsystem2_C2A2 */
#define TASK_EXECUTE_0191_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_C2A2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_C2A2_1);\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_C2A2_2:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_C2A2_2);\
  } while (0)

/* RelationalOperator:synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Compare */
#define TASK_EXECUTE_0194_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Compare_1:%u\n", mcos__cycle_step, (unsigned int)synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Compare_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Constant */
#define TASK_EXECUTE_0195_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Constant_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Constant_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_Constant1 */
#define TASK_EXECUTE_0197_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Constant1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Constant1_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_Constant2 */
#define TASK_EXECUTE_0198_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Constant2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Constant2_1);\
  } while (0)

/* Product:synrmmodel_mt_RTW_Subsystem2_Divide3 */
#define TASK_EXECUTE_0201_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Divide3_1:%.7f,%.7f\n", mcos__cycle_step,(double)synrmmodel_mt_RTW_Subsystem2_Divide3_1[0],(double)synrmmodel_mt_RTW_Subsystem2_Divide3_1[1]);\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Divide3_1:%.7f,%.7f\n", mcos__cycle_step,(double)synrmmodel_mt_RTW_Subsystem2_Divide3_1[0],(double)synrmmodel_mt_RTW_Subsystem2_Divide3_1[1]);\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Divide3_1:%.7f,%.7f\n", mcos__cycle_step,(double)synrmmodel_mt_RTW_Subsystem2_Divide3_1[0],(double)synrmmodel_mt_RTW_Subsystem2_Divide3_1[1]);\
  } while (0)

/* Gain:synrmmodel_mt_RTW_Subsystem2_E2M */
#define TASK_EXECUTE_0202_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_E2M_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_E2M_1);\
  } while (0)

/* Gain:synrmmodel_mt_RTW_Subsystem2_E2M1 */
#define TASK_EXECUTE_0203_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_E2M1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_E2M1_1);\
  } while (0)

/* Gain:synrmmodel_mt_RTW_Subsystem2_E2M2 */
#define TASK_EXECUTE_0204_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_E2M2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_E2M2_1);\
  } while (0)

/* Gain:synrmmodel_mt_RTW_Subsystem2_E2M3 */
#define TASK_EXECUTE_0205_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_E2M3_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_E2M3_1);\
  } while (0)

/* LookupNDDirect:synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_DirectLookupTablen_D */
#define TASK_EXECUTE_0236_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_DirectLookupTablen_D_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_DirectLookupTablen_D_1);\
  } while (0)

/* Merge:synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_Merge */
#define TASK_EXECUTE_0237_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_Merge_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_Merge_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_VH */
#define TASK_EXECUTE_0259_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_VH_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_VH_1);\
  } while (0)

/* Lookup_n-D:synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_thetaVa */
#define TASK_EXECUTE_0260_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_thetaVa_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_thetaVa_1);\
  } while (0)

/* Lookup_n-D:synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_thetaVb */
#define TASK_EXECUTE_0261_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_thetaVb_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_thetaVb_1);\
  } while (0)

/* Math:synrmmodel_mt_RTW_Subsystem2_MathFunction */
#define TASK_EXECUTE_0264_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_MathFunction_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_MathFunction_1);\
  } while (0)

/* Merge:synrmmodel_mt_RTW_Subsystem2_Merge */
#define TASK_EXECUTE_0265_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Merge_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Merge_1);\
  } while (0)

/* DiscretePulseGenerator:synrmmodel_mt_RTW_Subsystem2_ModeSelector */
#define TASK_EXECUTE_0266_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_ModeSelector_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_ModeSelector_1);\
  } while (0)

/* MultiPortSwitch:synrmmodel_mt_RTW_Subsystem2_MultiportSwitch */
#define TASK_EXECUTE_0269_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_MultiportSwitch_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_MultiportSwitch_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Subsystem2_PWM_Add */
#define TASK_EXECUTE_0285_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_Add_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_Add_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Subsystem2_PWM_Add1 */
#define TASK_EXECUTE_0286_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_Add1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_Add1_1);\
  } while (0)

/* Product:synrmmodel_mt_RTW_Subsystem2_PWM_Divide */
#define TASK_EXECUTE_0287_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_Divide_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_Divide_1);\
  } while (0)

/* Product:synrmmodel_mt_RTW_Subsystem2_PWM_Divide1 */
#define TASK_EXECUTE_0288_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_Divide1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_Divide1_1);\
  } while (0)

/* Product:synrmmodel_mt_RTW_Subsystem2_PWM_Divide2 */
#define TASK_EXECUTE_0289_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_Divide2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_Divide2_1);\
  } while (0)

/* Product:synrmmodel_mt_RTW_Subsystem2_PWM_Divide3 */
#define TASK_EXECUTE_0290_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_Divide3_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_Divide3_1);\
  } while (0)

/* Product:synrmmodel_mt_RTW_Subsystem2_PWM_Divide4 */
#define TASK_EXECUTE_0291_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_Divide4_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_Divide4_1);\
  } while (0)

/* Product:synrmmodel_mt_RTW_Subsystem2_PWM_Divide5 */
#define TASK_EXECUTE_0292_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_Divide5_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_Divide5_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Constant1 */
#define TASK_EXECUTE_0298_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Constant1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Constant1_1);\
  } while (0)

/* Gain:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Gain */
#define TASK_EXECUTE_0299_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Gain_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Gain_1);\
  } while (0)

/* Signum:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sign */
#define TASK_EXECUTE_0300_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sign_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sign_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sum1 */
#define TASK_EXECUTE_0301_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sum1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sum1_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sum2 */
#define TASK_EXECUTE_0302_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sum2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sum2_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Constant1 */
#define TASK_EXECUTE_0307_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Constant1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Constant1_1);\
  } while (0)

/* Gain:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Gain */
#define TASK_EXECUTE_0308_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Gain_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Gain_1);\
  } while (0)

/* Signum:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sign */
#define TASK_EXECUTE_0309_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sign_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sign_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sum1 */
#define TASK_EXECUTE_0310_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sum1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sum1_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sum2 */
#define TASK_EXECUTE_0311_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sum2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sum2_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Constant1 */
#define TASK_EXECUTE_0316_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Constant1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Constant1_1);\
  } while (0)

/* Gain:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Gain */
#define TASK_EXECUTE_0317_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Gain_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Gain_1);\
  } while (0)

/* Signum:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sign */
#define TASK_EXECUTE_0318_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sign_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sign_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sum1 */
#define TASK_EXECUTE_0319_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sum1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sum1_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sum2 */
#define TASK_EXECUTE_0320_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sum2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sum2_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Constant1 */
#define TASK_EXECUTE_0325_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Constant1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Constant1_1);\
  } while (0)

/* Gain:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Gain */
#define TASK_EXECUTE_0326_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Gain_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Gain_1);\
  } while (0)

/* Signum:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sign */
#define TASK_EXECUTE_0327_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sign_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sign_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sum1 */
#define TASK_EXECUTE_0328_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sum1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sum1_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sum2 */
#define TASK_EXECUTE_0329_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sum2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sum2_1);\
  } while (0)

/* UnaryMinus:synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus */
#define TASK_EXECUTE_0331_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus_1);\
  } while (0)

/* UnaryMinus:synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus1 */
#define TASK_EXECUTE_0332_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus1_1);\
  } while (0)

/* UnaryMinus:synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus2 */
#define TASK_EXECUTE_0333_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus2_1);\
  } while (0)

/* DiscretePulseGenerator:synrmmodel_mt_RTW_Subsystem2_PulseGenerator3 */
#define TASK_EXECUTE_0335_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_PulseGenerator3_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_PulseGenerator3_1);\
  } while (0)

/* RealImagToComplex:synrmmodel_mt_RTW_Subsystem2_R2C */
#define TASK_EXECUTE_0336_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_R2C_1:(%.7f,%.7f)\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_R2C_1.re, (double)synrmmodel_mt_RTW_Subsystem2_R2C_1.im);\
  } while (0)

/* RealImagToComplex:synrmmodel_mt_RTW_Subsystem2_R2C1 */
#define TASK_EXECUTE_0337_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_R2C1_1:(%.7f,%.7f)\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_R2C1_1.re, (double)synrmmodel_mt_RTW_Subsystem2_R2C1_1.im);\
  } while (0)

/* RealImagToComplex:synrmmodel_mt_RTW_Subsystem2_R2C2 */
#define TASK_EXECUTE_0338_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_R2C2_1:(%.7f,%.7f)\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_R2C2_1.re, (double)synrmmodel_mt_RTW_Subsystem2_R2C2_1.im);\
  } while (0)

/* Clock:synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Clock */
#define TASK_EXECUTE_0341_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Clock_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Clock_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Constant */
#define TASK_EXECUTE_0342_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Constant_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Constant_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Constant1 */
#define TASK_EXECUTE_0343_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Constant1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Constant1_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Output */
#define TASK_EXECUTE_0344_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Output_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Output_1);\
  } while (0)

/* Product:synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Product */
#define TASK_EXECUTE_0345_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Product_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Product_1);\
  } while (0)

/* Step:synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Step */
#define TASK_EXECUTE_0346_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Step_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Step_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Sum */
#define TASK_EXECUTE_0347_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Sum_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Sum_1);\
  } while (0)

/* Saturate:synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Saturation */
#define TASK_EXECUTE_0349_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Saturation_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Saturation_1);\
  } while (0)

/* Step:synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Step */
#define TASK_EXECUTE_0350_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Step_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Step_1);\
  } while (0)

/* Step:synrmmodel_mt_RTW_Subsystem2_Referencevelocity_StepDown */
#define TASK_EXECUTE_0351_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Referencevelocity_StepDown_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Referencevelocity_StepDown_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Sum */
#define TASK_EXECUTE_0352_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Sum_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Sum_1);\
  } while (0)

/* DiscretePulseGenerator:synrmmodel_mt_RTW_Subsystem2_Reffreq */
#define TASK_EXECUTE_0354_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Reffreq_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Reffreq_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_Subsystem_Constant */
#define TASK_EXECUTE_0358_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Subsystem_Constant_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Subsystem_Constant_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_Subsystem_Constant6 */
#define TASK_EXECUTE_0359_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Subsystem_Constant6_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Subsystem_Constant6_1);\
  } while (0)

/* Product:synrmmodel_mt_RTW_Subsystem2_Subsystem_Divide */
#define TASK_EXECUTE_0360_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Subsystem_Divide_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Subsystem_Divide_1);\
  } while (0)

/* Product:synrmmodel_mt_RTW_Subsystem2_Subsystem_Divide1 */
#define TASK_EXECUTE_0361_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Subsystem_Divide1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Subsystem_Divide1_1);\
  } while (0)

/* Signum:synrmmodel_mt_RTW_Subsystem2_Subsystem_Sign2 */
#define TASK_EXECUTE_0362_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Subsystem_Sign2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Subsystem_Sign2_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Subsystem2_Subsystem_Sum6 */
#define TASK_EXECUTE_0363_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Subsystem_Sum6_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Subsystem_Sum6_1);\
  } while (0)

/* Switch:synrmmodel_mt_RTW_Subsystem2_Subsystem_Switch1 */
#define TASK_EXECUTE_0364_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Subsystem_Switch1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Subsystem_Switch1_1);\
  } while (0)

/* UnaryMinus:synrmmodel_mt_RTW_Subsystem2_Subsystem_UnaryMinus */
#define TASK_EXECUTE_0365_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Subsystem_UnaryMinus_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Subsystem_UnaryMinus_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_Subsystem1_Constant */
#define TASK_EXECUTE_0372_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Subsystem1_Constant_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Subsystem1_Constant_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_Subsystem1_Constant6 */
#define TASK_EXECUTE_0373_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Subsystem1_Constant6_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Subsystem1_Constant6_1);\
  } while (0)

/* Product:synrmmodel_mt_RTW_Subsystem2_Subsystem1_Divide */
#define TASK_EXECUTE_0374_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Subsystem1_Divide_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Subsystem1_Divide_1);\
  } while (0)

/* Product:synrmmodel_mt_RTW_Subsystem2_Subsystem1_Divide1 */
#define TASK_EXECUTE_0375_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Subsystem1_Divide1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Subsystem1_Divide1_1);\
  } while (0)

/* Signum:synrmmodel_mt_RTW_Subsystem2_Subsystem1_Sign2 */
#define TASK_EXECUTE_0376_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Subsystem1_Sign2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Subsystem1_Sign2_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Subsystem2_Subsystem1_Sum6 */
#define TASK_EXECUTE_0377_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Subsystem1_Sum6_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Subsystem1_Sum6_1);\
  } while (0)

/* Switch:synrmmodel_mt_RTW_Subsystem2_Subsystem1_Switch1 */
#define TASK_EXECUTE_0378_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Subsystem1_Switch1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Subsystem1_Switch1_1);\
  } while (0)

/* UnaryMinus:synrmmodel_mt_RTW_Subsystem2_Subsystem1_UnaryMinus */
#define TASK_EXECUTE_0379_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Subsystem1_UnaryMinus_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Subsystem1_UnaryMinus_1);\
  } while (0)

/* Switch:synrmmodel_mt_RTW_Subsystem2_Switch */
#define TASK_EXECUTE_0383_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Switch_1:%.7f,%.7f\n", mcos__cycle_step,(double)synrmmodel_mt_RTW_Subsystem2_Switch_1[0],(double)synrmmodel_mt_RTW_Subsystem2_Switch_1[1]);\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Switch_1:%.7f,%.7f\n", mcos__cycle_step,(double)synrmmodel_mt_RTW_Subsystem2_Switch_1[0],(double)synrmmodel_mt_RTW_Subsystem2_Switch_1[1]);\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Switch_1:%.7f,%.7f\n", mcos__cycle_step,(double)synrmmodel_mt_RTW_Subsystem2_Switch_1[0],(double)synrmmodel_mt_RTW_Subsystem2_Switch_1[1]);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_Trigger_Constant */
#define TASK_EXECUTE_0388_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_Trigger_Constant_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_Trigger_Constant_1);\
  } while (0)

/* UnitDelay:synrmmodel_mt_RTW_Subsystem2_UnitDelay */
#define TASK_EXECUTE_0390_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_UnitDelay_1:%.7f,%.7f\n", mcos__cycle_step,(double)synrmmodel_mt_RTW_Subsystem2_UnitDelay_1[0],(double)synrmmodel_mt_RTW_Subsystem2_UnitDelay_1[1]);\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_UnitDelay_1:%.7f,%.7f\n", mcos__cycle_step,(double)synrmmodel_mt_RTW_Subsystem2_UnitDelay_1[0],(double)synrmmodel_mt_RTW_Subsystem2_UnitDelay_1[1]);\
  } while (0)

/* UnitDelay:synrmmodel_mt_RTW_Subsystem2_UnitDelay1 */
#define TASK_EXECUTE_0391_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_UnitDelay1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_UnitDelay1_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_VH */
#define TASK_EXECUTE_0392_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_VH_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_VH_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_VH1 */
#define TASK_EXECUTE_0393_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_VH1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_VH1_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_VH2 */
#define TASK_EXECUTE_0394_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_VH2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_VH2_1);\
  } while (0)

/* Assignment:synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment */
#define TASK_EXECUTE_0400_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf:%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f\n", mcos__cycle_step,(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[0],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[1],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[2],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[3],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[4],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[5],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[6],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[7],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[8],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[9],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[10],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[11],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[12],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[13],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[14],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[15],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[16],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[17],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[18],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[19],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[20],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[21],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[22],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[23],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[24],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[25],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[26],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[27],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[28],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[29],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[30],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[31],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[32],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[33],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[34],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[35],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[36],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[37],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[38],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[39],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[40],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[41],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[42],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[43],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[44],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[45],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[46],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[47],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[48],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[49],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[50],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[51],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[52],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[53],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[54],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[55],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[56],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[57],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[58],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[59],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[60],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[61],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[62],(double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf[63]);\
  } while (0)

/* Bias:synrmmodel_mt_RTW_Subsystem2_assignandselector_Bias1 */
#define TASK_EXECUTE_0401_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_assignandselector_Bias1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Bias1_1);\
  } while (0)

/* DataTypeConversion:synrmmodel_mt_RTW_Subsystem2_assignandselector_Conversion */
#define TASK_EXECUTE_0402_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_assignandselector_Conversion_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Conversion_1);\
  } while (0)

/* Math:synrmmodel_mt_RTW_Subsystem2_assignandselector_MathFunction2 */
#define TASK_EXECUTE_0403_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_assignandselector_MathFunction2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_assignandselector_MathFunction2_1);\
  } while (0)

/* Selector:synrmmodel_mt_RTW_Subsystem2_assignandselector_Selector1 */
#define TASK_EXECUTE_0404_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_assignandselector_Selector1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Selector1_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Subsystem2_assignandselector_Sum1 */
#define TASK_EXECUTE_0405_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_assignandselector_Sum1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_assignandselector_Sum1_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_assignandselector_VH3 */
#define TASK_EXECUTE_0406_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_assignandselector_VH3_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_assignandselector_VH3_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Subsystem2_calc_rpm_Add */
#define TASK_EXECUTE_0413_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_calc_rpm_Add_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_calc_rpm_Add_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_calc_rpm_Constant1 */
#define TASK_EXECUTE_0414_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_calc_rpm_Constant1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_calc_rpm_Constant1_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_calc_rpm_Constant2 */
#define TASK_EXECUTE_0415_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_calc_rpm_Constant2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_calc_rpm_Constant2_1);\
  } while (0)

/* Gain:synrmmodel_mt_RTW_Subsystem2_calc_rpm_Gain */
#define TASK_EXECUTE_0416_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_calc_rpm_Gain_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_calc_rpm_Gain_1);\
  } while (0)

/* RelationalOperator:synrmmodel_mt_RTW_Subsystem2_calc_rpm_RelationalOperator3 */
#define TASK_EXECUTE_0417_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_calc_rpm_RelationalOperator3_1:%u\n", mcos__cycle_step, (unsigned int)synrmmodel_mt_RTW_Subsystem2_calc_rpm_RelationalOperator3_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Subsystem2_calc_rpm_Subtract */
#define TASK_EXECUTE_0419_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_calc_rpm_Subtract_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_calc_rpm_Subtract_1);\
  } while (0)

/* Switch:synrmmodel_mt_RTW_Subsystem2_calc_rpm_Switch */
#define TASK_EXECUTE_0420_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_calc_rpm_Switch_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_calc_rpm_Switch_1);\
  } while (0)

/* UnitDelay:synrmmodel_mt_RTW_Subsystem2_calc_rpm_UnitDelay */
#define TASK_EXECUTE_0421_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_calc_rpm_UnitDelay_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_calc_rpm_UnitDelay_1);\
  } while (0)

/* UnitDelay:synrmmodel_mt_RTW_Subsystem2_feedback_1 */
#define TASK_EXECUTE_0423_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_feedback_1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_feedback_1_1);\
  } while (0)

/* UnitDelay:synrmmodel_mt_RTW_Subsystem2_feedback_2 */
#define TASK_EXECUTE_0424_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_feedback_2_1:%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f\n", mcos__cycle_step,(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[0],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[1],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[2],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[3],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[4],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[5],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[6],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[7],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[8],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[9],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[10],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[11],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[12],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[13],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[14],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[15],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[16],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[17],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[18],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[19],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[20],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[21],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[22],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[23],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[24],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[25],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[26],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[27],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[28],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[29],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[30],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[31],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[32],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[33],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[34],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[35],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[36],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[37],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[38],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[39],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[40],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[41],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[42],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[43],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[44],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[45],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[46],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[47],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[48],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[49],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[50],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[51],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[52],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[53],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[54],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[55],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[56],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[57],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[58],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[59],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[60],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[61],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[62],(double)synrmmodel_mt_RTW_Subsystem2_feedback_2_1[63]);\
  } while (0)

/* UnitDelay:synrmmodel_mt_RTW_Subsystem2_feedback_3 */
#define TASK_EXECUTE_0425_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_feedback_3_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_feedback_3_1);\
  } while (0)

/* UnitDelay:synrmmodel_mt_RTW_Subsystem2_feedback_4 */
#define TASK_EXECUTE_0426_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_feedback_4_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_feedback_4_1);\
  } while (0)

/* Sum:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Add2 */
#define TASK_EXECUTE_0438_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Add2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Add2_1);\
  } while (0)

/* Bias:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Bias */
#define TASK_EXECUTE_0439_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Bias_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Bias_1);\
  } while (0)

/* Bias:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Bias1 */
#define TASK_EXECUTE_0440_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Bias1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Bias1_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Constant4 */
#define TASK_EXECUTE_0441_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Constant4_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Constant4_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Constant7 */
#define TASK_EXECUTE_0442_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Constant7_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Constant7_1);\
  } while (0)

/* Logic:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator */
#define TASK_EXECUTE_0443_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator_1:%u\n", mcos__cycle_step, (unsigned int)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator_1);\
  } while (0)

/* Logic:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator1 */
#define TASK_EXECUTE_0444_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator1_1:%u\n", mcos__cycle_step, (unsigned int)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator1_1);\
  } while (0)

/* Logic:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator2 */
#define TASK_EXECUTE_0445_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator2_1:%u\n", mcos__cycle_step, (unsigned int)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator2_1);\
  } while (0)

/* RelationalOperator:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator */
#define TASK_EXECUTE_0447_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator_1:%u\n", mcos__cycle_step, (unsigned int)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator_1);\
  } while (0)

/* RelationalOperator:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator1 */
#define TASK_EXECUTE_0448_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator1_1:%u\n", mcos__cycle_step, (unsigned int)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator1_1);\
  } while (0)

/* RelationalOperator:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator2 */
#define TASK_EXECUTE_0449_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator2_1:%u\n", mcos__cycle_step, (unsigned int)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator2_1);\
  } while (0)

/* RelationalOperator:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator3 */
#define TASK_EXECUTE_0450_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator3_1:%u\n", mcos__cycle_step, (unsigned int)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator3_1);\
  } while (0)

/* RelationalOperator:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator4 */
#define TASK_EXECUTE_0451_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator4_1:%u\n", mcos__cycle_step, (unsigned int)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator4_1);\
  } while (0)

/* RelationalOperator:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator5 */
#define TASK_EXECUTE_0452_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator5_1:%u\n", mcos__cycle_step, (unsigned int)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator5_1);\
  } while (0)

/* Saturate:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Saturation */
#define TASK_EXECUTE_0453_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Saturation_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Saturation_1);\
  } while (0)

/* Switch:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch1 */
#define TASK_EXECUTE_0454_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch1_1);\
  } while (0)

/* Switch:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch2 */
#define TASK_EXECUTE_0455_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch2_1);\
  } while (0)

/* Switch:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch3 */
#define TASK_EXECUTE_0456_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch3_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch3_1);\
  } while (0)

/* Switch:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch4 */
#define TASK_EXECUTE_0457_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch4_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch4_1);\
  } while (0)

/* Switch:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch5 */
#define TASK_EXECUTE_0458_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch5_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch5_1);\
  } while (0)

/* UnitDelay:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay1 */
#define TASK_EXECUTE_0459_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay1_1);\
  } while (0)

/* UnitDelay:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay2 */
#define TASK_EXECUTE_0460_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay2_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_genten */
#define TASK_EXECUTE_0461_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_genten_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_genten_1);\
  } while (0)

/* Constant:synrmmodel_mt_RTW_Subsystem2_triggerfortheta_theta_step */
#define TASK_EXECUTE_0462_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Subsystem2_triggerfortheta_theta_step_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Subsystem2_triggerfortheta_theta_step_1);\
  } while (0)

/* UnitDelay:synrmmodel_mt_RTW_UnitDelay */
#define TASK_EXECUTE_0486_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_UnitDelay_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_UnitDelay_1);\
  } while (0)

/* UnitDelay:synrmmodel_mt_RTW_UnitDelay2 */
#define TASK_EXECUTE_0487_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_UnitDelay2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_UnitDelay2_1);\
  } while (0)

/* S-Function:synrmmodel_mt_RTW_VeloPosiError_S_FunctionBuilder */
#define TASK_EXECUTE_0493_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_VeloPosiError_S_FunctionBuilder_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_VeloPosiError_S_FunctionBuilder_1);\
  } while (0)

/* Fcn:synrmmodel_mt_RTW_VeloPosiError_raderror1 */
#define TASK_EXECUTE_0494_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_VeloPosiError_raderror1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_VeloPosiError_raderror1_1);\
  } while (0)

/* Fcn:synrmmodel_mt_RTW_VeloPosiError_raderror2 */
#define TASK_EXECUTE_0495_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_VeloPosiError_raderror2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_VeloPosiError_raderror2_1);\
  } while (0)

/* Gain:synrmmodel_mt_RTW_VeloPosiError_radtodeg */
#define TASK_EXECUTE_0496_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_VeloPosiError_position_err:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_VeloPosiError_position_err);\
  } while (0)

/* ZeroOrderHold:synrmmodel_mt_RTW_Zero_OrderHold */
#define TASK_EXECUTE_0499_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Zero_OrderHold_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Zero_OrderHold_1);\
  } while (0)

/* ZeroOrderHold:synrmmodel_mt_RTW_Zero_OrderHold1 */
#define TASK_EXECUTE_0500_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Zero_OrderHold1_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Zero_OrderHold1_1);\
  } while (0)

/* ZeroOrderHold:synrmmodel_mt_RTW_Zero_OrderHold2 */
#define TASK_EXECUTE_0501_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Zero_OrderHold2_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Zero_OrderHold2_1);\
  } while (0)

/* ZeroOrderHold:synrmmodel_mt_RTW_Zero_OrderHold3 */
#define TASK_EXECUTE_0502_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]synrmmodel_mt_RTW_Zero_OrderHold3_1:%.7f\n", mcos__cycle_step, (double)synrmmodel_mt_RTW_Zero_OrderHold3_1);\
  } while (0)
