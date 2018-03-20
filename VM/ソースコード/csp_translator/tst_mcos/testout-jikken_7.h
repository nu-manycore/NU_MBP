#ifndef SEQUENCIAL_PROGRAM
#include "testout-jikken_7_res.c"
#endif

/* Constant:JIKKEN_trigger_7_Constant4 */
#define TASK_EXECUTE_0001_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_Constant4_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_Constant4_1);\
  } while (0)

/* UnitDelay:JIKKEN_trigger_7_Delayforfeedback */
#define TASK_EXECUTE_0002_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_Delayforfeedback_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_Delayforfeedback_1);\
  } while (0)

/* UnitDelay:JIKKEN_trigger_7_Delayforfeedback1 */
#define TASK_EXECUTE_0003_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_Delayforfeedback1_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_Delayforfeedback1_1);\
  } while (0)

/* Gain:JIKKEN_trigger_7_Maketargetvalue_DiscretePIDController_2DOF_DerivativeGain */
#define TASK_EXECUTE_0026_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]DerivativeGain_c:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.DerivativeGain_c);\
  } while (0)

/* DiscreteIntegrator:JIKKEN_trigger_7_Maketargetvalue_DiscretePIDController_2DOF_Filter */
#define TASK_EXECUTE_0027_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]Filter_n:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.Filter_n);\
  } while (0)

/* Gain:JIKKEN_trigger_7_Maketargetvalue_DiscretePIDController_2DOF_FilterCoefficient */
#define TASK_EXECUTE_0028_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]FilterCoefficient_o:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.FilterCoefficient_o);\
  } while (0)

/* Gain:JIKKEN_trigger_7_Maketargetvalue_DiscretePIDController_2DOF_IntegralGain */
#define TASK_EXECUTE_0029_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]IntegralGain_p:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.IntegralGain_p);\
  } while (0)

/* DiscreteIntegrator:JIKKEN_trigger_7_Maketargetvalue_DiscretePIDController_2DOF_Integrator */
#define TASK_EXECUTE_0030_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]Integrator_j:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.Integrator_j);\
  } while (0)

/* Gain:JIKKEN_trigger_7_Maketargetvalue_DiscretePIDController_2DOF_ProportionalGain */
#define TASK_EXECUTE_0031_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]ProportionalGain_k:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.ProportionalGain_k);\
  } while (0)

/* Gain:JIKKEN_trigger_7_Maketargetvalue_DiscretePIDController_2DOF_SetpointWeightingDerivative */
#define TASK_EXECUTE_0032_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]SetpointWeightingDerivative_f:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.SetpointWeightingDerivative_f);\
  } while (0)

/* Gain:JIKKEN_trigger_7_Maketargetvalue_DiscretePIDController_2DOF_SetpointWeightingProportional */
#define TASK_EXECUTE_0033_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]SetpointWeightingProportional_i:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.SetpointWeightingProportional_i);\
  } while (0)

/* Sum:JIKKEN_trigger_7_Maketargetvalue_DiscretePIDController_2DOF_Sum */
#define TASK_EXECUTE_0034_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]Sum_g:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.Sum_g);\
  } while (0)

/* Sum:JIKKEN_trigger_7_Maketargetvalue_DiscretePIDController_2DOF_Sum1 */
#define TASK_EXECUTE_0035_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]Sum1_a:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.Sum1_a);\
  } while (0)

/* Sum:JIKKEN_trigger_7_Maketargetvalue_DiscretePIDController_2DOF_Sum2 */
#define TASK_EXECUTE_0036_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]Sum2_a:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.Sum2_a);\
  } while (0)

/* Sum:JIKKEN_trigger_7_Maketargetvalue_DiscretePIDController_2DOF_Sum3 */
#define TASK_EXECUTE_0037_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]Sum3_b:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.Sum3_b);\
  } while (0)

/* Sum:JIKKEN_trigger_7_Maketargetvalue_DiscretePIDController_2DOF_SumD */
#define TASK_EXECUTE_0038_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]SumD_h:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.SumD_h);\
  } while (0)

/* UnitDelay:JIKKEN_trigger_7_Maketargetvalue_UnitDelay */
#define TASK_EXECUTE_0040_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_Maketargetvalue_UnitDelay_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_Maketargetvalue_UnitDelay_1);\
  } while (0)

/* DiscretePulseGenerator:JIKKEN_trigger_7_ModeSelector */
#define TASK_EXECUTE_0042_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_ModeSelector_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_ModeSelector_1);\
  } while (0)

/* MultiPortSwitch:JIKKEN_trigger_7_MultiportSwitch */
#define TASK_EXECUTE_0043_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_MultiportSwitch_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_MultiportSwitch_1);\
  } while (0)

/* Gain:JIKKEN_trigger_7_PIDcontroller_Eventdriven_DiscretePIDController_2DOF_DerivativeGain */
#define TASK_EXECUTE_0051_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]DerivativeGain_p:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.DerivativeGain_p);\
  } while (0)

/* DiscreteIntegrator:JIKKEN_trigger_7_PIDcontroller_Eventdriven_DiscretePIDController_2DOF_Filter */
#define TASK_EXECUTE_0052_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]Filter_m:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.Filter_m);\
  } while (0)

/* Gain:JIKKEN_trigger_7_PIDcontroller_Eventdriven_DiscretePIDController_2DOF_FilterCoefficient */
#define TASK_EXECUTE_0053_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]FilterCoefficient_b:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.FilterCoefficient_b);\
  } while (0)

/* Gain:JIKKEN_trigger_7_PIDcontroller_Eventdriven_DiscretePIDController_2DOF_IntegralGain */
#define TASK_EXECUTE_0054_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]IntegralGain_h:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.IntegralGain_h);\
  } while (0)

/* DiscreteIntegrator:JIKKEN_trigger_7_PIDcontroller_Eventdriven_DiscretePIDController_2DOF_Integrator */
#define TASK_EXECUTE_0055_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]Integrator_i:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.Integrator_i);\
  } while (0)

/* Gain:JIKKEN_trigger_7_PIDcontroller_Eventdriven_DiscretePIDController_2DOF_ProportionalGain */
#define TASK_EXECUTE_0056_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]ProportionalGain_h:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.ProportionalGain_h);\
  } while (0)

/* Gain:JIKKEN_trigger_7_PIDcontroller_Eventdriven_DiscretePIDController_2DOF_SetpointWeightingDerivative */
#define TASK_EXECUTE_0057_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]SetpointWeightingDerivative_b:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.SetpointWeightingDerivative_b);\
  } while (0)

/* Gain:JIKKEN_trigger_7_PIDcontroller_Eventdriven_DiscretePIDController_2DOF_SetpointWeightingProportional */
#define TASK_EXECUTE_0058_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]SetpointWeightingProportional_a:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.SetpointWeightingProportional_a);\
  } while (0)

/* Sum:JIKKEN_trigger_7_PIDcontroller_Eventdriven_DiscretePIDController_2DOF_Sum */
#define TASK_EXECUTE_0059_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]Sum_j:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.Sum_j);\
  } while (0)

/* Sum:JIKKEN_trigger_7_PIDcontroller_Eventdriven_DiscretePIDController_2DOF_Sum1 */
#define TASK_EXECUTE_0060_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]Sum1_d:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.Sum1_d);\
  } while (0)

/* Sum:JIKKEN_trigger_7_PIDcontroller_Eventdriven_DiscretePIDController_2DOF_Sum2 */
#define TASK_EXECUTE_0061_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]Sum2_k:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.Sum2_k);\
  } while (0)

/* Sum:JIKKEN_trigger_7_PIDcontroller_Eventdriven_DiscretePIDController_2DOF_Sum3 */
#define TASK_EXECUTE_0062_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]Sum3_n:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.Sum3_n);\
  } while (0)

/* Sum:JIKKEN_trigger_7_PIDcontroller_Eventdriven_DiscretePIDController_2DOF_SumD */
#define TASK_EXECUTE_0063_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]SumD_b:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.SumD_b);\
  } while (0)

/* Sum:JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_Add */
#define TASK_EXECUTE_0069_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_Add_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_Add_1);\
  } while (0)

/* Constant:JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_Constant1 */
#define TASK_EXECUTE_0070_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_Constant1_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_Constant1_1);\
  } while (0)

/* Constant:JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_Constant2 */
#define TASK_EXECUTE_0071_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_Constant2_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_Constant2_1);\
  } while (0)

/* Gain:JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_Gain */
#define TASK_EXECUTE_0072_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_Gain_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_Gain_1);\
  } while (0)

/* Delay:JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_IntegerDelay */
#define TASK_EXECUTE_0073_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_IntegerDelay_1:%d\n", mcos__cycle_step, (int)JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_IntegerDelay_1);\
  } while (0)

/* RelationalOperator:JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_RelationalOperator3 */
#define TASK_EXECUTE_0074_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_RelationalOperator3_1:%u\n", mcos__cycle_step, (unsigned int)JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_RelationalOperator3_1);\
  } while (0)

/* Sum:JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_Subtract */
#define TASK_EXECUTE_0075_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_Subtract_1:%d\n", mcos__cycle_step, (int)JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_Subtract_1);\
  } while (0)

/* Switch:JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_Switch */
#define TASK_EXECUTE_0076_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_Switch_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_PIDcontroller_Eventdriven_1_Switch_1);\
  } while (0)

/* Gain:JIKKEN_trigger_7_PIDcontroller_changedperiodic_DiscretePIDController_2DOF_DerivativeGain */
#define TASK_EXECUTE_0085_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]DerivativeGain:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.DerivativeGain);\
  } while (0)

/* DiscreteIntegrator:JIKKEN_trigger_7_PIDcontroller_changedperiodic_DiscretePIDController_2DOF_Filter */
#define TASK_EXECUTE_0086_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]Filter:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.Filter);\
  } while (0)

/* Gain:JIKKEN_trigger_7_PIDcontroller_changedperiodic_DiscretePIDController_2DOF_FilterCoefficient */
#define TASK_EXECUTE_0087_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]FilterCoefficient:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.FilterCoefficient);\
  } while (0)

/* Gain:JIKKEN_trigger_7_PIDcontroller_changedperiodic_DiscretePIDController_2DOF_IntegralGain */
#define TASK_EXECUTE_0088_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]IntegralGain:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.IntegralGain);\
  } while (0)

/* DiscreteIntegrator:JIKKEN_trigger_7_PIDcontroller_changedperiodic_DiscretePIDController_2DOF_Integrator */
#define TASK_EXECUTE_0089_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]Integrator:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.Integrator);\
  } while (0)

/* Gain:JIKKEN_trigger_7_PIDcontroller_changedperiodic_DiscretePIDController_2DOF_ProportionalGain */
#define TASK_EXECUTE_0090_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]ProportionalGain:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.ProportionalGain);\
  } while (0)

/* Gain:JIKKEN_trigger_7_PIDcontroller_changedperiodic_DiscretePIDController_2DOF_SetpointWeightingDerivative */
#define TASK_EXECUTE_0091_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]SetpointWeightingDerivative:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.SetpointWeightingDerivative);\
  } while (0)

/* Gain:JIKKEN_trigger_7_PIDcontroller_changedperiodic_DiscretePIDController_2DOF_SetpointWeightingProportional */
#define TASK_EXECUTE_0092_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]SetpointWeightingProportional:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.SetpointWeightingProportional);\
  } while (0)

/* Sum:JIKKEN_trigger_7_PIDcontroller_changedperiodic_DiscretePIDController_2DOF_Sum */
#define TASK_EXECUTE_0093_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]Sum:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.Sum);\
  } while (0)

/* Sum:JIKKEN_trigger_7_PIDcontroller_changedperiodic_DiscretePIDController_2DOF_Sum1 */
#define TASK_EXECUTE_0094_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]Sum1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.Sum1);\
  } while (0)

/* Sum:JIKKEN_trigger_7_PIDcontroller_changedperiodic_DiscretePIDController_2DOF_Sum2 */
#define TASK_EXECUTE_0095_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]Sum2:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.Sum2);\
  } while (0)

/* Sum:JIKKEN_trigger_7_PIDcontroller_changedperiodic_DiscretePIDController_2DOF_Sum3 */
#define TASK_EXECUTE_0096_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]Sum3:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.Sum3);\
  } while (0)

/* Sum:JIKKEN_trigger_7_PIDcontroller_changedperiodic_DiscretePIDController_2DOF_SumD */
#define TASK_EXECUTE_0097_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]SumD:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_B.SumD);\
  } while (0)

/* Sum:JIKKEN_trigger_7_PLANT_Motortheta_Add1 */
#define TASK_EXECUTE_0102_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PLANT_Motortheta_Add1_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_PLANT_Motortheta_Add1_1);\
  } while (0)

/* DataTypeConversion:JIKKEN_trigger_7_PLANT_Motortheta_DataTypeConversion */
#define TASK_EXECUTE_0103_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PLANT_Motortheta_DataTypeConversion_1:%d\n", mcos__cycle_step, (int)JIKKEN_trigger_7_PLANT_Motortheta_DataTypeConversion_1);\
  } while (0)

/* Sum:JIKKEN_trigger_7_PLANT_Motortheta_LampfunctionforCSPtranslator_Add */
#define TASK_EXECUTE_0105_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PLANT_Motortheta_LampfunctionforCSPtranslator_Add_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_PLANT_Motortheta_LampfunctionforCSPtranslator_Add_1);\
  } while (0)

/* Constant:JIKKEN_trigger_7_PLANT_Motortheta_LampfunctionforCSPtranslator_Constant */
#define TASK_EXECUTE_0106_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PLANT_Motortheta_LampfunctionforCSPtranslator_Constant_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_PLANT_Motortheta_LampfunctionforCSPtranslator_Constant_1);\
  } while (0)

/* Fcn:JIKKEN_trigger_7_PLANT_Motortheta_LampfunctionforCSPtranslator_Fcn */
#define TASK_EXECUTE_0107_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PLANT_Motortheta_LampfunctionforCSPtranslator_Fcn_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_PLANT_Motortheta_LampfunctionforCSPtranslator_Fcn_1);\
  } while (0)

/* Delay:JIKKEN_trigger_7_PLANT_Motortheta_LampfunctionforCSPtranslator_IntegerDelay */
#define TASK_EXECUTE_0108_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PLANT_Motortheta_LampfunctionforCSPtranslator_IntegerDelay_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_PLANT_Motortheta_LampfunctionforCSPtranslator_IntegerDelay_1);\
  } while (0)

/* UnitDelay:JIKKEN_trigger_7_PLANT_Motortheta_UnitDelay1 */
#define TASK_EXECUTE_0110_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PLANT_Motortheta_UnitDelay1_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_PLANT_Motortheta_UnitDelay1_1);\
  } while (0)

/* Constant:JIKKEN_trigger_7_PLANT_Motortheta_theta_limit_Constant2 */
#define TASK_EXECUTE_0113_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PLANT_Motortheta_theta_limit_Constant2_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_PLANT_Motortheta_theta_limit_Constant2_1);\
  } while (0)

/* Constant:JIKKEN_trigger_7_PLANT_Motortheta_theta_limit_Constant3 */
#define TASK_EXECUTE_0114_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PLANT_Motortheta_theta_limit_Constant3_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_PLANT_Motortheta_theta_limit_Constant3_1);\
  } while (0)

/* RelationalOperator:JIKKEN_trigger_7_PLANT_Motortheta_theta_limit_RelationalOperator1 */
#define TASK_EXECUTE_0115_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PLANT_Motortheta_theta_limit_RelationalOperator1_1:%u\n", mcos__cycle_step, (unsigned int)JIKKEN_trigger_7_PLANT_Motortheta_theta_limit_RelationalOperator1_1);\
  } while (0)

/* Switch:JIKKEN_trigger_7_PLANT_Motortheta_theta_limit_Switch */
#define TASK_EXECUTE_0116_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PLANT_Motortheta_theta_limit_Switch_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_PLANT_Motortheta_theta_limit_Switch_1);\
  } while (0)

/* DiscretePulseGenerator:JIKKEN_trigger_7_PulseGenerator */
#define TASK_EXECUTE_0119_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PulseGenerator_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_PulseGenerator_1);\
  } while (0)

/* DiscretePulseGenerator:JIKKEN_trigger_7_PulseGenerator1 */
#define TASK_EXECUTE_0120_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PulseGenerator1_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_PulseGenerator1_1);\
  } while (0)

/* DiscretePulseGenerator:JIKKEN_trigger_7_PulseGenerator2 */
#define TASK_EXECUTE_0121_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PulseGenerator2_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_PulseGenerator2_1);\
  } while (0)

/* DiscretePulseGenerator:JIKKEN_trigger_7_PulseGenerator3 */
#define TASK_EXECUTE_0122_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_PulseGenerator3_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_PulseGenerator3_1);\
  } while (0)

/* RelationalOperator:JIKKEN_trigger_7_RelationalOperator3 */
#define TASK_EXECUTE_0123_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_RelationalOperator3_1:%u\n", mcos__cycle_step, (unsigned int)JIKKEN_trigger_7_RelationalOperator3_1);\
  } while (0)

/* Clock:JIKKEN_trigger_7_Samplesignal_Clock1 */
#define TASK_EXECUTE_0125_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_Samplesignal_Clock1_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_Samplesignal_Clock1_1);\
  } while (0)

/* Gain:JIKKEN_trigger_7_Samplesignal_Gain */
#define TASK_EXECUTE_0126_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_Samplesignal_Gain_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_Samplesignal_Gain_1);\
  } while (0)

/* Trigonometry:JIKKEN_trigger_7_Samplesignal_Output */
#define TASK_EXECUTE_0127_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_Samplesignal_Output_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_Samplesignal_Output_1);\
  } while (0)

/* Product:JIKKEN_trigger_7_Samplesignal_Product */
#define TASK_EXECUTE_0128_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_Samplesignal_Product_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_Samplesignal_Product_1);\
  } while (0)

/* Product:JIKKEN_trigger_7_Samplesignal_Product1 */
#define TASK_EXECUTE_0129_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_Samplesignal_Product1_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_Samplesignal_Product1_1);\
  } while (0)

/* Product:JIKKEN_trigger_7_Samplesignal_Product2 */
#define TASK_EXECUTE_0130_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_Samplesignal_Product2_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_Samplesignal_Product2_1);\
  } while (0)

/* Sum:JIKKEN_trigger_7_Samplesignal_Sum */
#define TASK_EXECUTE_0131_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_Samplesignal_Sum_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_Samplesignal_Sum_1);\
  } while (0)

/* Constant:JIKKEN_trigger_7_Samplesignal_deltaFreq */
#define TASK_EXECUTE_0132_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_Samplesignal_deltaFreq_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_Samplesignal_deltaFreq_1);\
  } while (0)

/* Constant:JIKKEN_trigger_7_Samplesignal_initialFreq */
#define TASK_EXECUTE_0133_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_Samplesignal_initialFreq_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_Samplesignal_initialFreq_1);\
  } while (0)

/* Constant:JIKKEN_trigger_7_Samplesignal_targetTime */
#define TASK_EXECUTE_0134_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_Samplesignal_targetTime_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_Samplesignal_targetTime_1);\
  } while (0)

/* Switch:JIKKEN_trigger_7_Switch */
#define TASK_EXECUTE_0136_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_Switch_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_Switch_1);\
  } while (0)

/* Sum:JIKKEN_trigger_7_triggerfortheta_Add2 */
#define TASK_EXECUTE_0139_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_triggerfortheta_Add2_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_triggerfortheta_Add2_1);\
  } while (0)

/* Constant:JIKKEN_trigger_7_triggerfortheta_Constant4 */
#define TASK_EXECUTE_0140_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_triggerfortheta_Constant4_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_triggerfortheta_Constant4_1);\
  } while (0)

/* Constant:JIKKEN_trigger_7_triggerfortheta_Constant7 */
#define TASK_EXECUTE_0141_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_triggerfortheta_Constant7_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_triggerfortheta_Constant7_1);\
  } while (0)

/* RelationalOperator:JIKKEN_trigger_7_triggerfortheta_RelationalOperator2 */
#define TASK_EXECUTE_0142_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_triggerfortheta_RelationalOperator2_1:%u\n", mcos__cycle_step, (unsigned int)JIKKEN_trigger_7_triggerfortheta_RelationalOperator2_1);\
  } while (0)

/* RelationalOperator:JIKKEN_trigger_7_triggerfortheta_RelationalOperator3 */
#define TASK_EXECUTE_0143_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_triggerfortheta_RelationalOperator3_1:%u\n", mcos__cycle_step, (unsigned int)JIKKEN_trigger_7_triggerfortheta_RelationalOperator3_1);\
  } while (0)

/* Switch:JIKKEN_trigger_7_triggerfortheta_Switch1 */
#define TASK_EXECUTE_0144_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_triggerfortheta_Switch1_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_triggerfortheta_Switch1_1);\
  } while (0)

/* Switch:JIKKEN_trigger_7_triggerfortheta_Switch2 */
#define TASK_EXECUTE_0145_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_triggerfortheta_Switch2_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_triggerfortheta_Switch2_1);\
  } while (0)

/* Switch:JIKKEN_trigger_7_triggerfortheta_Switch3 */
#define TASK_EXECUTE_0146_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_triggerfortheta_Switch3_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_triggerfortheta_Switch3_1);\
  } while (0)

/* UnitDelay:JIKKEN_trigger_7_triggerfortheta_UnitDelay3 */
#define TASK_EXECUTE_0147_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_triggerfortheta_UnitDelay3_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_triggerfortheta_UnitDelay3_1);\
  } while (0)

/* Constant:JIKKEN_trigger_7_triggerfortheta_genten */
#define TASK_EXECUTE_0148_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_triggerfortheta_genten_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_triggerfortheta_genten_1);\
  } while (0)

/* Constant:JIKKEN_trigger_7_triggerfortheta_theta_step */
#define TASK_EXECUTE_0149_COMPLETED()\
  do {\
    mcos_debug_printf("[%u]JIKKEN_trigger_7_triggerfortheta_theta_step_1:%.7f\n", mcos__cycle_step, (double)JIKKEN_trigger_7_triggerfortheta_theta_step_1);\
  } while (0)
