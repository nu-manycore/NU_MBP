/*
 * File: synrmmodel_mt_RTW.c
 *
 * Code generated for Simulink model 'synrmmodel_mt_RTW'.
 *
 * Model version                  : 1.1387
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Thu Sep 11 10:11:41 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "synrmmodel_mt_RTW.h"
#include "synrmmodel_mt_RTW_private.h"

/* user include files */
#include "measure.h"
#include "measure_cforest.h"

/*
 * empty measurement macros
 */
#ifndef MEASUREMENT_START
#define MEASUREMENT_START(ID,UPD,INDEX,PARAM)	do { ; } while (0)
#endif
#ifndef MEASUREMENT_STOP
#define MEASUREMENT_STOP(ID,UPD,INDEX,PARAM)	do { ; } while (0)
#endif
#ifndef MEASUREMENT_STEP_BEGIN
#define MEASUREMENT_STEP_BEGIN()	do { ; } while (0)
#endif
#ifndef MEASUREMENT_STEP_END
#define MEASUREMENT_STEP_END()	do { ; } while (0)
#endif
#ifndef MEASUREMENT_IF_CONDITION
#define MEASUREMENT_IF_CONDITION(X)	X
#endif
#ifndef MEASUREMENT_TID_CONDITION
#define MEASUREMENT_TID_CONDITION(X)	X
#endif

/* Exported block signals */
creal_T synrmmodel_mt_RTW_Subsystem2_R2C_1;/* '<S8>/R2C' */
creal_T synrmmodel_mt_RTW_Subsystem2_R2C1_1;/* '<S8>/R2C1' */
creal_T synrmmodel_mt_RTW_Subsystem2_R2C2_1;/* '<S8>/R2C2' */
real_T synrmmodel_mt_RTW_Subsystem2_VH2_1;/* '<S8>/VH2' */
real_T synrmmodel_mt_RTW_UnitDelay_1;  /* '<Root>/UnitDelay' */
real_T synrmmodel_mt_RTW_UnitDelay2_1; /* '<Root>/UnitDelay2' */
real_T synrmmodel_mt_RTW_MotorPara1_Constant2_1;/* '<S4>/Constant2' */
real_T synrmmodel_mt_RTW_MotorPara1_Constant1_1;/* '<S4>/Constant1' */
real_T synrmmodel_mt_RTW_MotorPara1_Constant_1;/* '<S4>/Constant' */
real_T synrmmodel_mt_RTW_IPMSM1_Constant_1;/* '<S2>/Constant' */
real_T synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_1;/* '<S2>/S_FunctionBuilder' */
real_T synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_2;/* '<S2>/S_FunctionBuilder' */
real_T synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_3;/* '<S2>/S_FunctionBuilder' */
real_T synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_4;/* '<S2>/S_FunctionBuilder' */
real_T synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_5;/* '<S2>/S_FunctionBuilder' */
real_T synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_6;/* '<S2>/S_FunctionBuilder' */
real_T synrmmodel_mt_RTW_Zero_OrderHold1_1;/* '<Root>/Zero_OrderHold1' */
real_T synrmmodel_mt_RTW_Subsystem2_E2M1_1;/* '<S8>/E2M1' */
real_T synrmmodel_mt_RTW_Subsystem2_Bias_1;/* '<S8>/Bias' */
real_T synrmmodel_mt_RTW_Subsystem2_Merge_1;/* '<S8>/Merge' */
real_T synrmmodel_mt_RTW_Subsystem2_MathFunction_1;/* '<S8>/MathFunction' */
real_T synrmmodel_mt_RTW_Subsystem2_Abs_1;/* '<S8>/Abs' */
real_T synrmmodel_mt_RTW_Subsystem2_PulseGenerator3_1;/* '<S8>/PulseGenerator3' */
real_T synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Step_1;/* '<S37>/Step' */
real_T synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Clock_1;/* '<S37>/Clock' */
real_T synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Constant_1;/* '<S37>/Constant' */
real_T synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Sum_1;/* '<S37>/Sum' */
real_T synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Product_1;/* '<S37>/Product' */
real_T synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Constant1_1;/* '<S37>/Constant1' */
real_T synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Output_1;/* '<S37>/Output' */
real_T synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Saturation_1;/* '<S21>/Saturation' */
real_T synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Step_1;/* '<S21>/Step' */
real_T synrmmodel_mt_RTW_Subsystem2_Referencevelocity_StepDown_1;/* '<S21>/StepDown' */
real_T synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Sum_1;/* '<S21>/Sum' */
real_T synrmmodel_mt_RTW_Zero_OrderHold2_1;/* '<Root>/Zero_OrderHold2' */
real_T synrmmodel_mt_RTW_Subsystem2_E2M_1;/* '<S8>/E2M' */
real_T synrmmodel_mt_RTW_Zero_OrderHold_1;/* '<Root>/Zero_OrderHold' */
real_T synrmmodel_mt_RTW_Zero_OrderHold3_1;/* '<Root>/Zero_OrderHold3' */
real_T synrmmodel_mt_RTW_Subsystem2_UnitDelay_1[2];/* '<S8>/UnitDelay' */
real_T synrmmodel_mt_RTW_Subsystem2_UnitDelay1_1;/* '<S8>/UnitDelay1' */
real_T synrmmodel_mt_RTW_Observer_observerab_Constant_1;/* '<S11>/Constant' */
real_T synrmmodel_mt_RTW_Observer_observerab_UnitDelay1_1;/* '<S11>/UnitDelay1' */
real_T synrmmodel_mt_RTW_Observer_observerab_S_FunctionBuilder1_1;/* '<S11>/S_FunctionBuilder1' */
real_T synrmmodel_mt_RTW_Subsystem2_E2M3_1;/* '<S8>/E2M3' */
real_T synrmmodel_mt_RTW_Observer_observerab_S_FunctionBuilder2_1;/* '<S11>/S_FunctionBuilder2' */
real_T synrmmodel_mt_RTW_Outputwtheta1_Constant_1;/* '<S6>/Constant' */
real_T synrmmodel_mt_RTW_Outputwtheta1_UnitDelay1_1;/* '<S6>/UnitDelay1' */
real_T synrmmodel_mt_RTW_Subsystem2_Reffreq_1;/* '<S8>/Reffreq' */
real_T synrmmodel_mt_RTW_Subsystem2_Subsystem_Constant_1;/* '<S22>/Constant' */
real_T synrmmodel_mt_RTW_Subsystem2_ModeSelector_1;/* '<S8>/ModeSelector' */
real_T synrmmodel_mt_RTW_Subsystem2_Constant1_1;/* '<S8>/Constant1' */
real_T synrmmodel_mt_RTW_Subsystem2_Constant2_1;/* '<S8>/Constant2' */
real_T synrmmodel_mt_RTW_Subsystem2_MultiportSwitch_1;/* '<S8>/MultiportSwitch' */
real_T synrmmodel_mt_RTW_Subsystem2_Subsystem_Divide_1;/* '<S22>/Divide' */
real_T synrmmodel_mt_RTW_Subsystem2_Subsystem_Constant6_1;/* '<S22>/Constant6' */
real_T synrmmodel_mt_RTW_Subsystem2_Subsystem_Divide1_1;/* '<S22>/Divide1' */
real_T synrmmodel_mt_RTW_Subsystem2_feedback_4_1;/* '<S8>/feedback_4' */
real_T synrmmodel_mt_RTW_Subsystem2_Subsystem_Sum6_1;/* '<S22>/Sum6' */
real_T synrmmodel_mt_RTW_Subsystem2_Subsystem_UnaryMinus_1;/* '<S22>/UnaryMinus' */
real_T synrmmodel_mt_RTW_Subsystem2_Subsystem_Sign2_1;/* '<S22>/Sign2' */
real_T synrmmodel_mt_RTW_Subsystem2_Trigger_Constant_1;/* '<S24>/Constant' */
real_T synrmmodel_mt_RTW_Subsystem2_C2A_1;/* '<S8>/C2A' */
real_T synrmmodel_mt_RTW_Subsystem2_C2A_2;/* '<S8>/C2A' */
real_T synrmmodel_mt_RTW_Subsystem2_E2M2_1;/* '<S8>/E2M2' */
real_T synrmmodel_mt_RTW_Subsystem2_VH_1;/* '<S8>/VH' */
real_T synrmmodel_mt_RTW_Subsystem2_feedback_2_1[64];/* '<S8>/feedback_2' */
real_T synrmmodel_mt_RTW_Subsystem2_feedback_3_1;/* '<S8>/feedback_3' */
real_T synrmmodel_mt_RTW_Subsystem2_feedback_1_1;/* '<S8>/feedback_1' */
real_T synrmmodel_mt_RTW_Subsystem2_assignandselector_Sum1_1;/* '<S25>/Sum1' */
real_T synrmmodel_mt_RTW_Subsystem2_assignandselector_VH3_1;/* '<S25>/VH3' */
real_T synrmmodel_mt_RTW_Subsystem2_assignandselector_MathFunction2_1;/* '<S25>/MathFunction2' */
real_T synrmmodel_mt_RTW_Subsystem2_assignandselector_Bias1_1;/* '<S25>/Bias1' */
real_T synrmmodel_mt_RTW_Subsystem2_assignandselector_Conversion_1;/* '<S25>/Conversion' */
real_T synrmmodel_mt_RTW_Subsystem2_assignandselector_Selector1_1;/* '<S25>/Selector1' */
real_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay1_1;/* '<S29>/UnitDelay1' */
real_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Bias1_1;/* '<S29>/Bias1' */
real_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay2_1;/* '<S29>/UnitDelay2' */
real_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch5_1;/* '<S29>/Switch5' */
real_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Bias_1;/* '<S29>/Bias' */
real_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch4_1;/* '<S29>/Switch4' */
real_T synrmmodel_mt_RTW_Subsystem2_Divide3_1[2];/* '<S8>/Divide3' */
real_T synrmmodel_mt_RTW_Subsystem2_C2A1_1;/* '<S8>/C2A1' */
real_T synrmmodel_mt_RTW_Subsystem2_C2A1_2;/* '<S8>/C2A1' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_Divide_1;/* '<S20>/Divide' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sum1_1;/* '<S36>/Sum1' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sign_1;/* '<S36>/Sign' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Constant1_1;/* '<S36>/Constant1' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sum2_1;/* '<S36>/Sum2' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Gain_1;/* '<S36>/Gain' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_Divide2_1;/* '<S20>/Divide2' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus1_1;/* '<S20>/UnaryMinus1' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sum1_1;/* '<S33>/Sum1' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sign_1;/* '<S33>/Sign' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Constant1_1;/* '<S33>/Constant1' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sum2_1;/* '<S33>/Sum2' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Gain_1;/* '<S33>/Gain' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus_1;/* '<S20>/UnaryMinus' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_Divide3_1;/* '<S20>/Divide3' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_Add_1;/* '<S20>/Add' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_Divide1_1;/* '<S20>/Divide1' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sum1_1;/* '<S34>/Sum1' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sign_1;/* '<S34>/Sign' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Constant1_1;/* '<S34>/Constant1' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sum2_1;/* '<S34>/Sum2' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Gain_1;/* '<S34>/Gain' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_Divide4_1;/* '<S20>/Divide4' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus2_1;/* '<S20>/UnaryMinus2' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sum1_1;/* '<S35>/Sum1' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sign_1;/* '<S35>/Sign' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Constant1_1;/* '<S35>/Constant1' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sum2_1;/* '<S35>/Sum2' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Gain_1;/* '<S35>/Gain' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_Divide5_1;/* '<S20>/Divide5' */
real_T synrmmodel_mt_RTW_Subsystem2_PWM_Add1_1;/* '<S20>/Add1' */
real_T synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Constant_1;/* '<S17>/Constant' */
real_T synrmmodel_mt_RTW_Subsystem2_Switch_1[2];/* '<S8>/Switch' */
real_T synrmmodel_mt_RTW_Subsystem2_C2A2_1;/* '<S8>/C2A2' */
real_T synrmmodel_mt_RTW_Subsystem2_C2A2_2;/* '<S8>/C2A2' */
real_T synrmmodel_mt_RTW_Subsystem2_Subsystem_Switch1_1;/* '<S22>/Switch1' */
real_T synrmmodel_mt_RTW_Subsystem2_Subsystem1_Constant_1;/* '<S23>/Constant' */
real_T synrmmodel_mt_RTW_Subsystem2_Subsystem1_Constant6_1;/* '<S23>/Constant6' */
real_T synrmmodel_mt_RTW_Subsystem2_VH1_1;/* '<S8>/VH1' */
real_T synrmmodel_mt_RTW_Subsystem2_Subsystem1_Divide_1;/* '<S23>/Divide' */
real_T synrmmodel_mt_RTW_Subsystem2_Subsystem1_Divide1_1;/* '<S23>/Divide1' */
real_T synrmmodel_mt_RTW_Subsystem2_Subsystem1_Sum6_1;/* '<S23>/Sum6' */
real_T synrmmodel_mt_RTW_Subsystem2_Subsystem1_UnaryMinus_1;/* '<S23>/UnaryMinus' */
real_T synrmmodel_mt_RTW_Subsystem2_Subsystem1_Sign2_1;/* '<S23>/Sign2' */
real_T synrmmodel_mt_RTW_Subsystem2_Subsystem1_Switch1_1;/* '<S23>/Switch1' */
real_T
  synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf
  [64];                                /* '<S25>/Assignment' */
real_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_theta_step_1;/* '<S29>/theta_step' */
real_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_genten_1;/* '<S29>/genten' */
real_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch1_1;/* '<S29>/Switch1' */
real_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Add2_1;/* '<S29>/Add2' */
real_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Constant4_1;/* '<S29>/Constant4' */
real_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Constant7_1;/* '<S29>/Constant7' */
real_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch2_1;/* '<S29>/Switch2' */
real_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch3_1;/* '<S29>/Switch3' */
real_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Saturation_1;/* '<S29>/Saturation' */
real_T synrmmodel_mt_RTW_VeloPosiError_raderror2_1;/* '<S9>/raderror2' */
real_T synrmmodel_mt_RTW_VeloPosiError_raderror1_1;/* '<S9>/raderror1' */
real_T synrmmodel_mt_RTW_VeloPosiError_S_FunctionBuilder_1;/* '<S9>/S_FunctionBuilder' */
real_T synrmmodel_mt_RTW_VeloPosiError_position_err;/* '<S9>/radtodeg' */
real_T synrmmodel_mt_RTW_DataStore_Clock1_1;/* '<S1>/Clock1' */
real_T synrmmodel_mt_RTW_Subsystem2_calc_rpm_UnitDelay_1;/* '<S26>/UnitDelay' */
real_T synrmmodel_mt_RTW_Subsystem2_calc_rpm_Subtract_1;/* '<S26>/Subtract' */
real_T synrmmodel_mt_RTW_Subsystem2_calc_rpm_Constant2_1;/* '<S26>/Constant2' */
real_T synrmmodel_mt_RTW_Subsystem2_calc_rpm_Add_1;/* '<S26>/Add' */
real_T synrmmodel_mt_RTW_Subsystem2_calc_rpm_Constant1_1;/* '<S26>/Constant1' */
real_T synrmmodel_mt_RTW_Subsystem2_calc_rpm_Switch_1;/* '<S26>/Switch' */
real_T synrmmodel_mt_RTW_Subsystem2_calc_rpm_Gain_1;/* '<S26>/Gain' */
real_T synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_thetaVa_1;/* '<S18>/thetaVa' */
real_T
  synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_DirectLookupTablen_D_1;/* '<S18>/DirectLookupTablen_D' */
real_T synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_thetaVb_1;/* '<S18>/thetaVb' */
real_T synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_VH_1;/* '<S18>/VH' */
real_T synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_Merge_1;/* '<S18>/Merge' */
real_T synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_M2E_1;/* '<S14>/M2E' */
real_T
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_ab2dq_fa_1;/* '<S15>/fa' */
real_T
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_ab2dq_fb_1;/* '<S15>/fb' */
real_T
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_S_FunctionBuilder_1;/* '<S14>/S_FunctionBuilder' */
real_T
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_S_FunctionBuilder_2;/* '<S14>/S_FunctionBuilder' */
real_T
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_fa1_1;/* '<S16>/fa1' */
real_T
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product4_1;/* '<S16>/Product4' */
real_T
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product5_1;/* '<S16>/Product5' */
real_T
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_fa2_1;/* '<S16>/fa2' */
real_T
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product6_1;/* '<S16>/Product6' */
real_T
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product7_1;/* '<S16>/Product7' */
real_T
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Subtract1_1;/* '<S16>/Subtract1' */
real_T
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Sum1_1;/* '<S16>/Sum1' */
real_T synrmmodel_mt_RTW_Outputwtheta1_Subsystem_Sum_1;/* '<S13>/Sum' */
real_T synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_1;/* '<S13>/S_FunctionBuilder' */
real_T synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_2;/* '<S13>/S_FunctionBuilder' */
real_T synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_3;/* '<S13>/S_FunctionBuilder' */
real_T synrmmodel_mt_RTW_Observer_observerab_Subsystem_Sum_1;/* '<S12>/Sum' */
real_T synrmmodel_mt_RTW_Observer_observerab_Subsystem_S_FunctionBuilder_1;/* '<S12>/S_FunctionBuilder' */
real_T synrmmodel_mt_RTW_Observer_observerab_Subsystem_S_FunctionBuilder_2;/* '<S12>/S_FunctionBuilder' */
real_T synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_wrmerr_1;/* '<S10>/wrmerr' */
real_T synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_m2e_1;/* '<S10>/m2e' */
real_T synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_S_FunctionBuilder_1;/* '<S10>/S_FunctionBuilder' */
real_T synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_S_FunctionBuilder_2;/* '<S10>/S_FunctionBuilder' */
boolean_T synrmmodel_mt_RTW_Subsystem2_Trigger_Compare_1;/* '<S24>/Compare' */
boolean_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator2_1;/* '<S29>/RelationalOperator2' */
boolean_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator5_1;/* '<S29>/RelationalOperator5' */
boolean_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator2_1;/* '<S29>/LogicalOperator2' */
boolean_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator_1;/* '<S29>/RelationalOperator' */
boolean_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator4_1;/* '<S29>/RelationalOperator4' */
boolean_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator1_1;/* '<S29>/LogicalOperator1' */
boolean_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator1_1;/* '<S29>/RelationalOperator1' */
boolean_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator_1;/* '<S29>/LogicalOperator' */
boolean_T synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Compare_1;/* '<S17>/Compare' */
boolean_T synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator3_1;/* '<S29>/RelationalOperator3' */
boolean_T synrmmodel_mt_RTW_Subsystem2_calc_rpm_RelationalOperator3_1;/* '<S26>/RelationalOperator3' */

/* Block states (auto storage) */
D_Work_synrmmodel_mt_RTW synrmmodel_mt_RTW_DWork;

/* Previous zero-crossings (trigger) states */
PrevZCSigStates_synrmmodel_mt_R synrmmodel_mt_RT_PrevZCSigState;

/* Real-time model */
RT_MODEL_synrmmodel_mt_RTW synrmmodel_mt_RTW_M_;
RT_MODEL_synrmmodel_mt_RTW *const synrmmodel_mt_RTW_M = &synrmmodel_mt_RTW_M_;
static void rate_scheduler(void);
real_T look1_binlxpw(real_T u0, const real_T bp0[], const real_T table[],
                     uint32_T maxIndex)
{
  real_T frac;
  uint32_T iRght;
  uint32_T iLeft;
  uint32_T bpIdx;

  /* Lookup 1-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear'
     Extrapolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Linear'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0U]) {
    iLeft = 0U;
    frac = (u0 - bp0[0U]) / (bp0[1U] - bp0[0U]);
  } else if (u0 < bp0[maxIndex]) {
    /* Binary Search */
    bpIdx = maxIndex >> 1U;
    iLeft = 0U;
    iRght = maxIndex;
    while (iRght - iLeft > 1U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
  } else {
    iLeft = maxIndex - 1U;
    frac = (u0 - bp0[maxIndex - 1U]) / (bp0[maxIndex] - bp0[maxIndex - 1U]);
  }

  /* Interpolation 1-D
     Interpolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'portable wrapping'
   */
  return (table[iLeft + 1U] - table[iLeft]) * frac + table[iLeft];
}

/*
 *   This function updates active task flag for each subrate.
 * The function is called at model base rate, hence the
 * generated code self-manages all its subrates.
 */
static void rate_scheduler(void)
{
  /* Compute which subrates run during the next base time step.  Subrates
   * are an integer multiple of the base rate counter.  Therefore, the subtask
   * counter is reset when it reaches its limit (zero means run).
   */
  (synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[2])++;
  if ((synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[2]) > 4) {/* Sample time: [5.0E-6s, 0.0s] */
    synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[2] = 0;
  }

  (synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[3])++;
  if ((synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[3]) > 9) {/* Sample time: [1.0E-5s, 0.0s] */
    synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[3] = 0;
  }

  (synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[4])++;
  if ((synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[4]) > 49) {/* Sample time: [5.0E-5s, 0.0s] */
    synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[4] = 0;
  }

  (synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[5])++;
  if ((synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[5]) > 99) {/* Sample time: [9.9999999999999991E-5s, 0.0s] */
    synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[5] = 0;
  }

  (synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[6])++;
  if ((synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[6]) > 499) {/* Sample time: [0.0005s, 0.0s] */
    synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[6] = 0;
  }
}

/*
 * Output and update for action system:
 *    '<S18>/SwitchCaseActionSubsystem'
 *    '<S8>/sonomama'
 */
void synrm_SwitchCaseActionSubsystem(real_T rtu_In1, real_T *rty_Out1)
{
  /* Inport: '<S30>/In1' */
  *rty_Out1 = rtu_In1;
}

/*
 * Output and update for action system:
 *    '<S18>/SwitchCaseActionSubsystem1'
 *    '<S18>/SwitchCaseActionSubsystem2'
 */
void synr_SwitchCaseActionSubsystem1(real_T rtu_In1, real_T *rty_Out1,
  rtP_SwitchCaseActionSubsystem1_ *localP)
{
  /* Bias: '<S31>/Bias1' */
  *rty_Out1 = rtu_In1 + localP->Bias1_Bias;
}

real_T rt_roundd_snf(real_T u)
{
  real_T y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

real_T rt_modd_snf(real_T u0, real_T u1)
{
  real_T y;
  real_T tmp;
  if (u1 == 0.0) {
    y = u0;
  } else if (!((!rtIsNaN(u0)) && (!rtIsInf(u0)) && ((!rtIsNaN(u1)) && (!rtIsInf
                (u1))))) {
    y = (rtNaN);
  } else {
    tmp = u0 / u1;
    if (u1 <= floor(u1)) {
      y = u0 - floor(tmp) * u1;
    } else if (fabs(tmp - rt_roundd_snf(tmp)) <= DBL_EPSILON * fabs(tmp)) {
      y = 0.0;
    } else {
      y = (tmp - floor(tmp)) * u1;
    }
  }

  return y;
}

real_T rt_hypotd_snf(real_T u0, real_T u1)
{
  real_T y;
  real_T a;
  real_T b;
  a = fabs(u0);
  b = fabs(u1);
  if (a < b) {
    a /= b;
    y = sqrt(a * a + 1.0) * b;
  } else if (a > b) {
    b /= a;
    y = sqrt(b * b + 1.0) * a;
  } else if (rtIsNaN(b)) {
    y = b;
  } else {
    y = a * 1.4142135623730951;
  }

  return y;
}

real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  int32_T u0_0;
  int32_T u1_0;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    if (u0 > 0.0) {
      u0_0 = 1;
    } else {
      u0_0 = -1;
    }

    if (u1 > 0.0) {
      u1_0 = 1;
    } else {
      u1_0 = -1;
    }

    y = atan2(u0_0, u1_0);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

/* Model step function */
void synrmmodel_mt_RTW_step(void)
{
  /* local block i/o variables */
  real_T rtb_Saturation;
  real_T rtb_TmpSignalConversionAtToWork[11];
  real_T currentTime;
  ZCEventType zcEvent;
  real_T tmp;

MEASUREMENT_STEP_BEGIN();
MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_VH2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_VH2_PARAM);
  /* Constant: '<S8>/VH2' */
  synrmmodel_mt_RTW_Subsystem2_VH2_1 = synrmmodel_mt_RTW_P.VH2_Value;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_VH2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_VH2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_UNITDELAY,0,0,SYNRMMODEL_MT_RTW_UNITDELAY_PARAM);
  /* UnitDelay: '<Root>/UnitDelay' */
  synrmmodel_mt_RTW_UnitDelay_1 = synrmmodel_mt_RTW_DWork.UnitDelay_DSTATE;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_UNITDELAY,0,0,SYNRMMODEL_MT_RTW_UNITDELAY_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_UNITDELAY2,0,0,SYNRMMODEL_MT_RTW_UNITDELAY2_PARAM);
  /* UnitDelay: '<Root>/UnitDelay2' */
  synrmmodel_mt_RTW_UnitDelay2_1 = synrmmodel_mt_RTW_DWork.UnitDelay2_DSTATE;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_UNITDELAY2,0,0,SYNRMMODEL_MT_RTW_UNITDELAY2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_MOTORPARA1_CONSTANT2,0,0,SYNRMMODEL_MT_RTW_MOTORPARA1_CONSTANT2_PARAM);
  /* Constant: '<S4>/Constant2' */
  synrmmodel_mt_RTW_MotorPara1_Constant2_1 = synrmmodel_mt_RTW_P.Ld;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_MOTORPARA1_CONSTANT2,0,0,SYNRMMODEL_MT_RTW_MOTORPARA1_CONSTANT2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_MOTORPARA1_CONSTANT1,0,0,SYNRMMODEL_MT_RTW_MOTORPARA1_CONSTANT1_PARAM);
  /* Constant: '<S4>/Constant1' */
  synrmmodel_mt_RTW_MotorPara1_Constant1_1 = synrmmodel_mt_RTW_P.Lq;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_MOTORPARA1_CONSTANT1,0,0,SYNRMMODEL_MT_RTW_MOTORPARA1_CONSTANT1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_MOTORPARA1_CONSTANT,0,0,SYNRMMODEL_MT_RTW_MOTORPARA1_CONSTANT_PARAM);
  /* Constant: '<S4>/Constant' */
  synrmmodel_mt_RTW_MotorPara1_Constant_1 = synrmmodel_mt_RTW_P.R;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_MOTORPARA1_CONSTANT,0,0,SYNRMMODEL_MT_RTW_MOTORPARA1_CONSTANT_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_IPMSM1_CONSTANT,0,0,SYNRMMODEL_MT_RTW_IPMSM1_CONSTANT_PARAM);
  /* Constant: '<S2>/Constant' */
  synrmmodel_mt_RTW_IPMSM1_Constant_1 = synrmmodel_mt_RTW_P.load;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_IPMSM1_CONSTANT,0,0,SYNRMMODEL_MT_RTW_IPMSM1_CONSTANT_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_IPMSM1_S_FUNCTIONBUILDER,0,0,SYNRMMODEL_MT_RTW_IPMSM1_S_FUNCTIONBUILDER_PARAM);
  /* S-Function (mydIpmsm): '<S2>/S_FunctionBuilder' */
  mydIpmsm_Outputs_wrapper(&synrmmodel_mt_RTW_UnitDelay_1,
    &synrmmodel_mt_RTW_UnitDelay2_1, &synrmmodel_mt_RTW_MotorPara1_Constant2_1,
    &synrmmodel_mt_RTW_MotorPara1_Constant1_1,
    &synrmmodel_mt_RTW_MotorPara1_Constant_1,
    &synrmmodel_mt_RTW_IPMSM1_Constant_1,
    &synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_1,
    &synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_2,
    &synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_3,
    &synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_4,
    &synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_5,
    &synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_6,
    &synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE[0], &synrmmodel_mt_RTW_P.p,
    1, &synrmmodel_mt_RTW_P.jm, 1, &synrmmodel_mt_RTW_P.d, 1,
    &synrmmodel_mt_RTW_P.wre0, 1, &synrmmodel_mt_RTW_P.theta0, 1,
    &synrmmodel_mt_RTW_P.ke, 1, &synrmmodel_mt_RTW_P.tmo, 1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_IPMSM1_S_FUNCTIONBUILDER,0,0,SYNRMMODEL_MT_RTW_IPMSM1_S_FUNCTIONBUILDER_PARAM);
  if (MEASUREMENT_TID_CONDITION(synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[5] == 0)) {
MEASUREMENT_START(SYNRMMODEL_MT_RTW_ZERO_ORDERHOLD1,0,0,SYNRMMODEL_MT_RTW_ZERO_ORDERHOLD1_PARAM);
    /* ZeroOrderHold: '<Root>/Zero_OrderHold1' */
    synrmmodel_mt_RTW_Zero_OrderHold1_1 =
      synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_6;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_ZERO_ORDERHOLD1,0,0,SYNRMMODEL_MT_RTW_ZERO_ORDERHOLD1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_E2M1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_E2M1_PARAM);
    /* Gain: '<S8>/E2M1' */
    synrmmodel_mt_RTW_Subsystem2_E2M1_1 = synrmmodel_mt_RTW_P.E2M1_Gain *
      synrmmodel_mt_RTW_Zero_OrderHold1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_E2M1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_E2M1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_BIAS,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_BIAS_PARAM);
    /* Bias: '<S8>/Bias' */
    synrmmodel_mt_RTW_Subsystem2_Bias_1 = synrmmodel_mt_RTW_Subsystem2_E2M1_1 +
      synrmmodel_mt_RTW_P.Bias_Bias_a;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_BIAS,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_BIAS_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_IF,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_IF_PARAM);
    /* If: '<S8>/If' */
    if (synrmmodel_mt_RTW_Subsystem2_Bias_1 >= 0.0) {
      /* Outputs for IfAction SubSystem: '<S8>/sonomama' incorporates:
       *  ActionPort: '<S28>/ActionPort'
       */
      synrm_SwitchCaseActionSubsystem(synrmmodel_mt_RTW_Subsystem2_Bias_1,
        &synrmmodel_mt_RTW_Subsystem2_Merge_1);

      /* End of Outputs for SubSystem: '<S8>/sonomama' */
    } else {
      /* Outputs for IfAction SubSystem: '<S8>/offsetari' incorporates:
       *  ActionPort: '<S27>/ActionPort'
       */
      /* Bias: '<S27>/Bias' */
      synrmmodel_mt_RTW_Subsystem2_Merge_1 = synrmmodel_mt_RTW_Subsystem2_Bias_1
        + synrmmodel_mt_RTW_P.Bias_Bias;

      /* End of Outputs for SubSystem: '<S8>/offsetari' */
    }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_IF,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_IF_PARAM);

    /* End of If: '<S8>/If' */
  }

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_MATHFUNCTION,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_MATHFUNCTION_PARAM);
  /* Math: '<S8>/MathFunction' */
  synrmmodel_mt_RTW_Subsystem2_MathFunction_1 = rt_modd_snf
    (synrmmodel_mt_RTW_Subsystem2_Merge_1, synrmmodel_mt_RTW_Subsystem2_VH2_1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_MATHFUNCTION,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_MATHFUNCTION_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_ABS,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_ABS_PARAM);
  /* Abs: '<S8>/Abs' */
  synrmmodel_mt_RTW_Subsystem2_Abs_1 = fabs
    (synrmmodel_mt_RTW_Subsystem2_MathFunction_1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_ABS,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_ABS_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PULSEGENERATOR3,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PULSEGENERATOR3_PARAM);
  /* DiscretePulseGenerator: '<S8>/PulseGenerator3' */
  synrmmodel_mt_RTW_Subsystem2_PulseGenerator3_1 =
    (synrmmodel_mt_RTW_DWork.clockTickCounter <
     synrmmodel_mt_RTW_P.PulseGenerator3_Duty) &&
    (synrmmodel_mt_RTW_DWork.clockTickCounter >= 0) ?
    synrmmodel_mt_RTW_P.PulseGenerator3_Amp : 0.0;
  if (synrmmodel_mt_RTW_DWork.clockTickCounter >=
      synrmmodel_mt_RTW_P.PulseGenerator3_Period - 1.0) {
    synrmmodel_mt_RTW_DWork.clockTickCounter = 0;
  } else {
    synrmmodel_mt_RTW_DWork.clockTickCounter++;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PULSEGENERATOR3,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PULSEGENERATOR3_PARAM);

  /* End of DiscretePulseGenerator: '<S8>/PulseGenerator3' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_CALC_RPM,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_CALC_RPM_PARAM);
  /* Outputs for Triggered SubSystem: '<S8>/calc_rpm' incorporates:
   *  TriggerPort: '<S26>/Trigger'
   */
  zcEvent = rt_ZCFcn(RISING_ZERO_CROSSING,
                     &synrmmodel_mt_RT_PrevZCSigState.calc_rpm_Trig_ZCE,
                     (synrmmodel_mt_RTW_Subsystem2_PulseGenerator3_1));
  if (MEASUREMENT_IF_CONDITION(zcEvent != NO_ZCEVENT)) {
    /* UnitDelay: '<S26>/UnitDelay' */
    synrmmodel_mt_RTW_Subsystem2_calc_rpm_UnitDelay_1 =
      synrmmodel_mt_RTW_DWork.UnitDelay_DSTATE_d;

    /* Sum: '<S26>/Subtract' */
    synrmmodel_mt_RTW_Subsystem2_calc_rpm_Subtract_1 =
      synrmmodel_mt_RTW_Subsystem2_Abs_1 -
      synrmmodel_mt_RTW_Subsystem2_calc_rpm_UnitDelay_1;

    /* Constant: '<S26>/Constant2' */
    synrmmodel_mt_RTW_Subsystem2_calc_rpm_Constant2_1 =
      synrmmodel_mt_RTW_P.Constant2_Value;

    /* Sum: '<S26>/Add' */
    synrmmodel_mt_RTW_Subsystem2_calc_rpm_Add_1 =
      synrmmodel_mt_RTW_Subsystem2_calc_rpm_Constant2_1 +
      synrmmodel_mt_RTW_Subsystem2_calc_rpm_Subtract_1;

    /* Constant: '<S26>/Constant1' */
    synrmmodel_mt_RTW_Subsystem2_calc_rpm_Constant1_1 =
      synrmmodel_mt_RTW_P.Constant1_Value;

    /* RelationalOperator: '<S26>/RelationalOperator3' */
    synrmmodel_mt_RTW_Subsystem2_calc_rpm_RelationalOperator3_1 =
      (synrmmodel_mt_RTW_Subsystem2_calc_rpm_Constant1_1 >
       synrmmodel_mt_RTW_Subsystem2_calc_rpm_Subtract_1);

    /* Switch: '<S26>/Switch' */
    if (synrmmodel_mt_RTW_Subsystem2_calc_rpm_RelationalOperator3_1) {
      synrmmodel_mt_RTW_Subsystem2_calc_rpm_Switch_1 =
        synrmmodel_mt_RTW_Subsystem2_calc_rpm_Add_1;
    } else {
      synrmmodel_mt_RTW_Subsystem2_calc_rpm_Switch_1 =
        synrmmodel_mt_RTW_Subsystem2_calc_rpm_Subtract_1;
    }

    /* End of Switch: '<S26>/Switch' */

    /* Gain: '<S26>/Gain' */
    synrmmodel_mt_RTW_Subsystem2_calc_rpm_Gain_1 = synrmmodel_mt_RTW_P.Gain_Gain
      * synrmmodel_mt_RTW_Subsystem2_calc_rpm_Switch_1;

    /* Saturate: '<S26>/Saturation' */
    if (synrmmodel_mt_RTW_Subsystem2_calc_rpm_Subtract_1 >
        synrmmodel_mt_RTW_P.Saturation_UpperSat) {
      rtb_Saturation = synrmmodel_mt_RTW_P.Saturation_UpperSat;
    } else if (synrmmodel_mt_RTW_Subsystem2_calc_rpm_Subtract_1 <
               synrmmodel_mt_RTW_P.Saturation_LowerSat) {
      rtb_Saturation = synrmmodel_mt_RTW_P.Saturation_LowerSat;
    } else {
      rtb_Saturation = synrmmodel_mt_RTW_Subsystem2_calc_rpm_Subtract_1;
    }

    /* End of Saturate: '<S26>/Saturation' */

    /* Update for UnitDelay: '<S26>/UnitDelay' */
    synrmmodel_mt_RTW_DWork.UnitDelay_DSTATE_d =
      synrmmodel_mt_RTW_Subsystem2_Abs_1;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_CALC_RPM,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_CALC_RPM_PARAM);

  /* End of Outputs for SubSystem: '<S8>/calc_rpm' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_STEP,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_STEP_PARAM);
  /* Step: '<S37>/Step' */
  currentTime = synrmmodel_mt_RTW_M->Timing.t[0];
  if (currentTime < synrmmodel_mt_RTW_P.slopetime) {
    synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Step_1 =
      synrmmodel_mt_RTW_P.Step_Y0;
  } else {
    synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Step_1 =
      synrmmodel_mt_RTW_P.slope;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_STEP,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_STEP_PARAM);

  /* End of Step: '<S37>/Step' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_CLOCK,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_CLOCK_PARAM);
  /* Clock: '<S37>/Clock' */
  synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Clock_1 =
    synrmmodel_mt_RTW_M->Timing.t[0];
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_CLOCK,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_CLOCK_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_CONSTANT,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_CONSTANT_PARAM);
  /* Constant: '<S37>/Constant' */
  synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Constant_1 =
    synrmmodel_mt_RTW_P.slopetime;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_CONSTANT,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_CONSTANT_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_SUM,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_SUM_PARAM);
  /* Sum: '<S37>/Sum' */
  synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Sum_1 =
    synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Clock_1 -
    synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Constant_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_SUM,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_SUM_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_PRODUCT,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_PRODUCT_PARAM);
  /* Product: '<S37>/Product' */
  synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Product_1 =
    synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Step_1 *
    synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Sum_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_PRODUCT,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_PRODUCT_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_CONSTANT1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_CONSTANT1_PARAM);
  /* Constant: '<S37>/Constant1' */
  synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Constant1_1 =
    synrmmodel_mt_RTW_P.Ramp_X0;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_CONSTANT1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_CONSTANT1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_OUTPUT,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_OUTPUT_PARAM);
  /* Sum: '<S37>/Output' */
  synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Output_1 =
    synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Product_1 +
    synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Constant1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_OUTPUT,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_RAMP_OUTPUT_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_SATURATION,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_SATURATION_PARAM);
  /* Saturate: '<S21>/Saturation' */
  if (synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Output_1 >
      synrmmodel_mt_RTW_P.rimv) {
    synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Saturation_1 =
      synrmmodel_mt_RTW_P.rimv;
  } else if (synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Output_1 <
             -synrmmodel_mt_RTW_P.rimv) {
    synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Saturation_1 =
      -synrmmodel_mt_RTW_P.rimv;
  } else {
    synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Saturation_1 =
      synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Output_1;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_SATURATION,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_SATURATION_PARAM);

  /* End of Saturate: '<S21>/Saturation' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_STEP,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_STEP_PARAM);
  /* Step: '<S21>/Step' */
  currentTime = synrmmodel_mt_RTW_M->Timing.t[0];
  if (currentTime < synrmmodel_mt_RTW_P.hendou_time) {
    synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Step_1 =
      synrmmodel_mt_RTW_P.inispeed;
  } else {
    synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Step_1 =
      synrmmodel_mt_RTW_P.norspeed;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_STEP,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_STEP_PARAM);

  /* End of Step: '<S21>/Step' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_STEPDOWN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_STEPDOWN_PARAM);
  /* Step: '<S21>/StepDown' */
  currentTime = synrmmodel_mt_RTW_M->Timing.t[0];
  if (currentTime < synrmmodel_mt_RTW_P.stepdown) {
    synrmmodel_mt_RTW_Subsystem2_Referencevelocity_StepDown_1 =
      synrmmodel_mt_RTW_P.StepDown_Y0;
  } else {
    synrmmodel_mt_RTW_Subsystem2_Referencevelocity_StepDown_1 =
      synrmmodel_mt_RTW_P.downspeed;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_STEPDOWN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_STEPDOWN_PARAM);

  /* End of Step: '<S21>/StepDown' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_SUM,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_SUM_PARAM);
  /* Sum: '<S21>/Sum' */
  synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Sum_1 =
    (synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Saturation_1 +
     synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Step_1) +
    synrmmodel_mt_RTW_Subsystem2_Referencevelocity_StepDown_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_SUM,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFERENCEVELOCITY_SUM_PARAM);
  if (MEASUREMENT_TID_CONDITION(synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[5] == 0)) {
MEASUREMENT_START(SYNRMMODEL_MT_RTW_ZERO_ORDERHOLD2,0,0,SYNRMMODEL_MT_RTW_ZERO_ORDERHOLD2_PARAM);
    /* ZeroOrderHold: '<Root>/Zero_OrderHold2' */
    synrmmodel_mt_RTW_Zero_OrderHold2_1 =
      synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_5;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_ZERO_ORDERHOLD2,0,0,SYNRMMODEL_MT_RTW_ZERO_ORDERHOLD2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_E2M,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_E2M_PARAM);
    /* Gain: '<S8>/E2M' */
    synrmmodel_mt_RTW_Subsystem2_E2M_1 = synrmmodel_mt_RTW_P.E2M *
      synrmmodel_mt_RTW_Zero_OrderHold2_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_E2M,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_E2M_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_ZERO_ORDERHOLD,0,0,SYNRMMODEL_MT_RTW_ZERO_ORDERHOLD_PARAM);
    /* ZeroOrderHold: '<Root>/Zero_OrderHold' */
    synrmmodel_mt_RTW_Zero_OrderHold_1 =
      synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_3;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_ZERO_ORDERHOLD,0,0,SYNRMMODEL_MT_RTW_ZERO_ORDERHOLD_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_ZERO_ORDERHOLD3,0,0,SYNRMMODEL_MT_RTW_ZERO_ORDERHOLD3_PARAM);
    /* ZeroOrderHold: '<Root>/Zero_OrderHold3' */
    synrmmodel_mt_RTW_Zero_OrderHold3_1 =
      synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_4;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_ZERO_ORDERHOLD3,0,0,SYNRMMODEL_MT_RTW_ZERO_ORDERHOLD3_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_UNITDELAY,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_UNITDELAY_PARAM);
    /* UnitDelay: '<S8>/UnitDelay' */
    synrmmodel_mt_RTW_Subsystem2_UnitDelay_1[0] =
      synrmmodel_mt_RTW_DWork.UnitDelay_DSTATE_n[0];
    synrmmodel_mt_RTW_Subsystem2_UnitDelay_1[1] =
      synrmmodel_mt_RTW_DWork.UnitDelay_DSTATE_n[1];
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_UNITDELAY,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_UNITDELAY_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_UNITDELAY1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_UNITDELAY1_PARAM);
    /* UnitDelay: '<S8>/UnitDelay1' */
    synrmmodel_mt_RTW_Subsystem2_UnitDelay1_1 =
      synrmmodel_mt_RTW_DWork.UnitDelay1_DSTATE;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_UNITDELAY1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_UNITDELAY1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_CONSTANT,0,0,SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_CONSTANT_PARAM);
    /* Constant: '<S11>/Constant' */
    synrmmodel_mt_RTW_Observer_observerab_Constant_1 = synrmmodel_mt_RTW_P.tob;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_CONSTANT,0,0,SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_CONSTANT_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_UNITDELAY1,0,0,SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_UNITDELAY1_PARAM);
    /* UnitDelay: '<S11>/UnitDelay1' */
    synrmmodel_mt_RTW_Observer_observerab_UnitDelay1_1 =
      synrmmodel_mt_RTW_DWork.UnitDelay1_DSTATE_j;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_UNITDELAY1,0,0,SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_UNITDELAY1_PARAM);

    /* Outputs for Atomic SubSystem: '<S11>/Subsystem' */
MEASUREMENT_START(SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_SUBSYSTEM_SUM,0,0,SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_SUBSYSTEM_SUM_PARAM);
    /* Sum: '<S12>/Sum' */
    synrmmodel_mt_RTW_Observer_observerab_Subsystem_Sum_1 =
      synrmmodel_mt_RTW_Observer_observerab_Constant_1 +
      synrmmodel_mt_RTW_Observer_observerab_UnitDelay1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_SUBSYSTEM_SUM,0,0,SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_SUBSYSTEM_SUM_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_SUBSYSTEM_S_FUNCTIONBUILDER,0,0,SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_SUBSYSTEM_S_FUNCTIONBUILDER_PARAM);
    /* S-Function (myobserverab): '<S12>/S_FunctionBuilder' */
    myobserverab_Outputs_wrapper(&synrmmodel_mt_RTW_Zero_OrderHold2_1,
      &synrmmodel_mt_RTW_Observer_observerab_Subsystem_Sum_1,
      &synrmmodel_mt_RTW_Zero_OrderHold_1, &synrmmodel_mt_RTW_Zero_OrderHold3_1,
      &synrmmodel_mt_RTW_Subsystem2_UnitDelay_1[0],
      &synrmmodel_mt_RTW_Subsystem2_UnitDelay_1[1],
      &synrmmodel_mt_RTW_Subsystem2_UnitDelay1_1,
      &synrmmodel_mt_RTW_Observer_observerab_Subsystem_S_FunctionBuilder_1,
      &synrmmodel_mt_RTW_Observer_observerab_Subsystem_S_FunctionBuilder_2,
      &synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE_hg[0],
      &synrmmodel_mt_RTW_P.alpha, 1, &synrmmodel_mt_RTW_P.Ldob, 1,
      &synrmmodel_mt_RTW_P.Lqob, 1, &synrmmodel_mt_RTW_P.R0, 1,
      &synrmmodel_mt_RTW_P.onposi, 1, &synrmmodel_mt_RTW_P.settime, 1,
      &synrmmodel_mt_RTW_P.tob, 1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_SUBSYSTEM_S_FUNCTIONBUILDER,0,0,SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_SUBSYSTEM_S_FUNCTIONBUILDER_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_SUBSYSTEM_S_FUNCTIONBUILDER,1,0,SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_SUBSYSTEM_S_FUNCTIONBUILDER_PARAM);
    /* S-Function "myobserverab_wrapper" Block: <S12>/S_FunctionBuilder */
    myobserverab_Update_wrapper(&synrmmodel_mt_RTW_Zero_OrderHold2_1,
      &synrmmodel_mt_RTW_Observer_observerab_Subsystem_Sum_1,
      &synrmmodel_mt_RTW_Zero_OrderHold_1, &synrmmodel_mt_RTW_Zero_OrderHold3_1,
      &synrmmodel_mt_RTW_Subsystem2_UnitDelay_1[0],
      &synrmmodel_mt_RTW_Subsystem2_UnitDelay_1[1],
      &synrmmodel_mt_RTW_Subsystem2_UnitDelay1_1,
      &synrmmodel_mt_RTW_Observer_observerab_Subsystem_S_FunctionBuilder_1,
      &synrmmodel_mt_RTW_Observer_observerab_Subsystem_S_FunctionBuilder_2,
      &synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE_hg[0],
      &synrmmodel_mt_RTW_P.alpha, 1, &synrmmodel_mt_RTW_P.Ldob, 1,
      &synrmmodel_mt_RTW_P.Lqob, 1, &synrmmodel_mt_RTW_P.R0, 1,
      &synrmmodel_mt_RTW_P.onposi, 1, &synrmmodel_mt_RTW_P.settime, 1,
      &synrmmodel_mt_RTW_P.tob, 1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_SUBSYSTEM_S_FUNCTIONBUILDER,1,0,SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_SUBSYSTEM_S_FUNCTIONBUILDER_PARAM);

    /* End of Outputs for SubSystem: '<S11>/Subsystem' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_S_FUNCTIONBUILDER1,0,0,SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_S_FUNCTIONBUILDER1_PARAM);
    /* S-Function (myvelocityesti): '<S11>/S_FunctionBuilder1' */
    myvelocityesti_Outputs_wrapper
      (&synrmmodel_mt_RTW_Observer_observerab_Subsystem_S_FunctionBuilder_1,
       &synrmmodel_mt_RTW_Observer_observerab_Subsystem_S_FunctionBuilder_2,
       &synrmmodel_mt_RTW_Observer_observerab_S_FunctionBuilder1_1,
       &synrmmodel_mt_RTW_DWork.S_FunctionBuilder1_DSTATE[0],
       &synrmmodel_mt_RTW_P.Kvp, 1, &synrmmodel_mt_RTW_P.Kvi, 1,
       &synrmmodel_mt_RTW_P.Kvpp, 1, &synrmmodel_mt_RTW_P.wre0, 1,
       &synrmmodel_mt_RTW_P.tob, 1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_S_FUNCTIONBUILDER1,0,0,SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_S_FUNCTIONBUILDER1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_E2M3,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_E2M3_PARAM);
    /* Gain: '<S8>/E2M3' */
    synrmmodel_mt_RTW_Subsystem2_E2M3_1 = synrmmodel_mt_RTW_P.E2M *
      synrmmodel_mt_RTW_Observer_observerab_S_FunctionBuilder1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_E2M3,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_E2M3_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_S_FUNCTIONBUILDER2,0,0,SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_S_FUNCTIONBUILDER2_PARAM);
    /* S-Function (mypositionestiab): '<S11>/S_FunctionBuilder2' */
    mypositionestiab_Outputs_wrapper
      (&synrmmodel_mt_RTW_Observer_observerab_Subsystem_S_FunctionBuilder_1,
       &synrmmodel_mt_RTW_Observer_observerab_Subsystem_S_FunctionBuilder_2,
       &synrmmodel_mt_RTW_Observer_observerab_S_FunctionBuilder2_1,
       &synrmmodel_mt_RTW_P.tob, 1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_S_FUNCTIONBUILDER2,0,0,SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_S_FUNCTIONBUILDER2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_CONSTANT,0,0,SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_CONSTANT_PARAM);
    /* Constant: '<S6>/Constant' */
    synrmmodel_mt_RTW_Outputwtheta1_Constant_1 = synrmmodel_mt_RTW_P.tcc;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_CONSTANT,0,0,SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_CONSTANT_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_UNITDELAY1,0,0,SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_UNITDELAY1_PARAM);
    /* UnitDelay: '<S6>/UnitDelay1' */
    synrmmodel_mt_RTW_Outputwtheta1_UnitDelay1_1 =
      synrmmodel_mt_RTW_DWork.UnitDelay1_DSTATE_i;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_UNITDELAY1,0,0,SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_UNITDELAY1_PARAM);

    /* Outputs for Atomic SubSystem: '<S6>/Subsystem' */
MEASUREMENT_START(SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_SUBSYSTEM_SUM,0,0,SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_SUBSYSTEM_SUM_PARAM);
    /* Sum: '<S13>/Sum' */
    synrmmodel_mt_RTW_Outputwtheta1_Subsystem_Sum_1 =
      synrmmodel_mt_RTW_Outputwtheta1_Constant_1 +
      synrmmodel_mt_RTW_Outputwtheta1_UnitDelay1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_SUBSYSTEM_SUM,0,0,SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_SUBSYSTEM_SUM_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_SUBSYSTEM_S_FUNCTIONBUILDER,0,0,SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_SUBSYSTEM_S_FUNCTIONBUILDER_PARAM);
    /* S-Function (myconswitch): '<S13>/S_FunctionBuilder' */
    myconswitch_Outputs_wrapper(&synrmmodel_mt_RTW_Subsystem2_E2M_1,
      &synrmmodel_mt_RTW_Outputwtheta1_Subsystem_Sum_1,
      &synrmmodel_mt_RTW_Subsystem2_E2M3_1, &synrmmodel_mt_RTW_Zero_OrderHold1_1,
      &synrmmodel_mt_RTW_Observer_observerab_S_FunctionBuilder2_1,
      &synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_1,
      &synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_2,
      &synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_3,
      &synrmmodel_mt_RTW_P.sless, 1, &synrmmodel_mt_RTW_P.settime, 1,
      &synrmmodel_mt_RTW_P.tcc, 1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_SUBSYSTEM_S_FUNCTIONBUILDER,0,0,SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_SUBSYSTEM_S_FUNCTIONBUILDER_PARAM);

    /* End of Outputs for SubSystem: '<S6>/Subsystem' */
  }

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFFREQ,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFFREQ_PARAM);
  /* DiscretePulseGenerator: '<S8>/Reffreq' */
  synrmmodel_mt_RTW_Subsystem2_Reffreq_1 =
    (synrmmodel_mt_RTW_DWork.clockTickCounter_d <
     synrmmodel_mt_RTW_P.Reffreq_Duty) &&
    (synrmmodel_mt_RTW_DWork.clockTickCounter_d >= 0) ?
    synrmmodel_mt_RTW_P.Reffreq_Amp : 0.0;
  if (synrmmodel_mt_RTW_DWork.clockTickCounter_d >=
      synrmmodel_mt_RTW_P.Reffreq_Period - 1.0) {
    synrmmodel_mt_RTW_DWork.clockTickCounter_d = 0;
  } else {
    synrmmodel_mt_RTW_DWork.clockTickCounter_d++;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFFREQ,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_REFFREQ_PARAM);

  /* End of DiscretePulseGenerator: '<S8>/Reffreq' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_MAKETARGETVALUE1,0,0,SYNRMMODEL_MT_RTW_MAKETARGETVALUE1_PARAM);
  /* Outputs for Triggered SubSystem: '<Root>/Maketargetvalue1' incorporates:
   *  TriggerPort: '<S3>/Trigger'
   */
  zcEvent = rt_ZCFcn(RISING_ZERO_CROSSING,
                     &synrmmodel_mt_RT_PrevZCSigState.Maketargetvalue1_Trig_ZCE,
                     (synrmmodel_mt_RTW_Subsystem2_Reffreq_1));
  if (MEASUREMENT_IF_CONDITION(zcEvent != NO_ZCEVENT)) {
    /* Sum: '<S10>/wrmerr' */
    synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_wrmerr_1 =
      synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Sum_1 -
      synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_2;

    /* Gain: '<S10>/m2e' */
    synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_m2e_1 =
      synrmmodel_mt_RTW_P.M2E *
      synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_wrmerr_1;

    /* S-Function (myvelocitycon): '<S10>/S_FunctionBuilder' */
    myvelocitycon_Outputs_wrapper
      (&synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_m2e_1,
       &synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_S_FunctionBuilder_1,
       &synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_S_FunctionBuilder_2,
       &synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE_j,
       &synrmmodel_mt_RTW_P.kp, 1, &synrmmodel_mt_RTW_P.ki, 1,
       &synrmmodel_mt_RTW_P.bound, 1, &synrmmodel_mt_RTW_P.isou, 1,
       &synrmmodel_mt_RTW_P.tvc, 1);

    /* S-Function "myvelocitycon_wrapper" Block: <S10>/S_FunctionBuilder */
    myvelocitycon_Update_wrapper
      (&synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_m2e_1,
       &synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_S_FunctionBuilder_1,
       &synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_S_FunctionBuilder_2,
       &synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE_j,
       &synrmmodel_mt_RTW_P.kp, 1, &synrmmodel_mt_RTW_P.ki, 1,
       &synrmmodel_mt_RTW_P.bound, 1, &synrmmodel_mt_RTW_P.isou, 1,
       &synrmmodel_mt_RTW_P.tvc, 1);
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_MAKETARGETVALUE1,0,0,SYNRMMODEL_MT_RTW_MAKETARGETVALUE1_PARAM);

  /* End of Outputs for SubSystem: '<Root>/Maketargetvalue1' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_CONSTANT,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_CONSTANT_PARAM);
  /* Constant: '<S22>/Constant' */
  synrmmodel_mt_RTW_Subsystem2_Subsystem_Constant_1 =
    synrmmodel_mt_RTW_P.Constant_Value;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_CONSTANT,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_CONSTANT_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_MODESELECTOR,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_MODESELECTOR_PARAM);
  /* DiscretePulseGenerator: '<S8>/ModeSelector' */
  synrmmodel_mt_RTW_Subsystem2_ModeSelector_1 =
    (synrmmodel_mt_RTW_DWork.clockTickCounter_m <
     synrmmodel_mt_RTW_P.ModeSelector_Duty) &&
    (synrmmodel_mt_RTW_DWork.clockTickCounter_m >= 0) ?
    synrmmodel_mt_RTW_P.ModeSelector_Amp : 0.0;
  if (synrmmodel_mt_RTW_DWork.clockTickCounter_m >=
      synrmmodel_mt_RTW_P.ModeSelector_Period - 1.0) {
    synrmmodel_mt_RTW_DWork.clockTickCounter_m = 0;
  } else {
    synrmmodel_mt_RTW_DWork.clockTickCounter_m++;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_MODESELECTOR,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_MODESELECTOR_PARAM);

  /* End of DiscretePulseGenerator: '<S8>/ModeSelector' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_CONSTANT1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_CONSTANT1_PARAM);
  /* Constant: '<S8>/Constant1' */
  synrmmodel_mt_RTW_Subsystem2_Constant1_1 =
    synrmmodel_mt_RTW_P.Constant1_Value_j;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_CONSTANT1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_CONSTANT1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_CONSTANT2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_CONSTANT2_PARAM);
  /* Constant: '<S8>/Constant2' */
  synrmmodel_mt_RTW_Subsystem2_Constant2_1 =
    synrmmodel_mt_RTW_P.Constant2_Value_e;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_CONSTANT2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_CONSTANT2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_MULTIPORTSWITCH,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_MULTIPORTSWITCH_PARAM);
  /* MultiPortSwitch: '<S8>/MultiportSwitch' */
  if ((int32_T)synrmmodel_mt_RTW_Subsystem2_ModeSelector_1 == 0) {
    synrmmodel_mt_RTW_Subsystem2_MultiportSwitch_1 =
      synrmmodel_mt_RTW_Subsystem2_Constant1_1;
  } else {
    synrmmodel_mt_RTW_Subsystem2_MultiportSwitch_1 =
      synrmmodel_mt_RTW_Subsystem2_Constant2_1;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_MULTIPORTSWITCH,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_MULTIPORTSWITCH_PARAM);

  /* End of MultiPortSwitch: '<S8>/MultiportSwitch' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_DIVIDE,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_DIVIDE_PARAM);
  /* Product: '<S22>/Divide' */
  synrmmodel_mt_RTW_Subsystem2_Subsystem_Divide_1 =
    synrmmodel_mt_RTW_Subsystem2_Subsystem_Constant_1 /
    synrmmodel_mt_RTW_Subsystem2_MultiportSwitch_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_DIVIDE,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_DIVIDE_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_CONSTANT6,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_CONSTANT6_PARAM);
  /* Constant: '<S22>/Constant6' */
  synrmmodel_mt_RTW_Subsystem2_Subsystem_Constant6_1 =
    synrmmodel_mt_RTW_P.Constant6_Value;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_CONSTANT6,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_CONSTANT6_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_DIVIDE1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_DIVIDE1_PARAM);
  /* Product: '<S22>/Divide1' */
  synrmmodel_mt_RTW_Subsystem2_Subsystem_Divide1_1 = 1.0 /
    synrmmodel_mt_RTW_Subsystem2_Subsystem_Divide_1 *
    synrmmodel_mt_RTW_Subsystem2_Subsystem_Constant6_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_DIVIDE1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_DIVIDE1_PARAM);
  if (MEASUREMENT_TID_CONDITION(synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[3] == 0)) {
MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_4,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_4_PARAM);
    /* UnitDelay: '<S8>/feedback_4' */
    synrmmodel_mt_RTW_Subsystem2_feedback_4_1 =
      synrmmodel_mt_RTW_DWork.feedback_4_DSTATE;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_4,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_4_PARAM);
  }

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_SUM6,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_SUM6_PARAM);
  /* Sum: '<S22>/Sum6' */
  synrmmodel_mt_RTW_Subsystem2_Subsystem_Sum6_1 =
    synrmmodel_mt_RTW_Subsystem2_feedback_4_1 -
    synrmmodel_mt_RTW_Subsystem2_Subsystem_Divide1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_SUM6,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_SUM6_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_UNARYMINUS,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_UNARYMINUS_PARAM);
  /* UnaryMinus: '<S22>/UnaryMinus' */
  synrmmodel_mt_RTW_Subsystem2_Subsystem_UnaryMinus_1 =
    -synrmmodel_mt_RTW_Subsystem2_Subsystem_Sum6_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_UNARYMINUS,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_UNARYMINUS_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_SIGN2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_SIGN2_PARAM);
  /* Signum: '<S22>/Sign2' */
  if (synrmmodel_mt_RTW_Subsystem2_Subsystem_UnaryMinus_1 < 0.0) {
    synrmmodel_mt_RTW_Subsystem2_Subsystem_Sign2_1 = -1.0;
  } else if (synrmmodel_mt_RTW_Subsystem2_Subsystem_UnaryMinus_1 > 0.0) {
    synrmmodel_mt_RTW_Subsystem2_Subsystem_Sign2_1 = 1.0;
  } else if (synrmmodel_mt_RTW_Subsystem2_Subsystem_UnaryMinus_1 == 0.0) {
    synrmmodel_mt_RTW_Subsystem2_Subsystem_Sign2_1 = 0.0;
  } else {
    synrmmodel_mt_RTW_Subsystem2_Subsystem_Sign2_1 =
      synrmmodel_mt_RTW_Subsystem2_Subsystem_UnaryMinus_1;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_SIGN2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_SIGN2_PARAM);

  /* End of Signum: '<S22>/Sign2' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGER_CONSTANT,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGER_CONSTANT_PARAM);
  /* Constant: '<S24>/Constant' */
  synrmmodel_mt_RTW_Subsystem2_Trigger_Constant_1 =
    synrmmodel_mt_RTW_P.Trigger_const;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGER_CONSTANT,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGER_CONSTANT_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGER_COMPARE,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGER_COMPARE_PARAM);
  /* RelationalOperator: '<S24>/Compare' */
  synrmmodel_mt_RTW_Subsystem2_Trigger_Compare_1 =
    (synrmmodel_mt_RTW_Subsystem2_Subsystem_Sign2_1 <=
     synrmmodel_mt_RTW_Subsystem2_Trigger_Constant_1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGER_COMPARE,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGER_COMPARE_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_PIDCONTROLLER_CHANGEDPERIODIC,0,0,SYNRMMODEL_MT_RTW_PIDCONTROLLER_CHANGEDPERIODIC_PARAM);
  /* Outputs for Triggered SubSystem: '<Root>/PIDcontroller_changedperiodic' incorporates:
   *  TriggerPort: '<S7>/Trigger'
   */
  if (MEASUREMENT_IF_CONDITION(synrmmodel_mt_RTW_Subsystem2_Trigger_Compare_1 &&
      (synrmmodel_mt_RT_PrevZCSigState.PIDcontroller_changedperiodic_T !=
       POS_ZCSIG))) {
    /* Gain: '<S14>/M2E' */
    synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_M2E_1 =
      synrmmodel_mt_RTW_P.M2E *
      synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_2;

    /* Fcn: '<S15>/fa' */
    synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_ab2dq_fa_1
      = cos(synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_1) *
      synrmmodel_mt_RTW_Zero_OrderHold_1 + sin
      (synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_1) *
      synrmmodel_mt_RTW_Zero_OrderHold3_1;

    /* Fcn: '<S15>/fb' */
    synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_ab2dq_fb_1
      = -sin(synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_1) *
      synrmmodel_mt_RTW_Zero_OrderHold_1 + cos
      (synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_1) *
      synrmmodel_mt_RTW_Zero_OrderHold3_1;

    /* S-Function (mycurrentcon): '<S14>/S_FunctionBuilder' */
    mycurrentcon_Outputs_wrapper
      (&synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_S_FunctionBuilder_1,
       &synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_S_FunctionBuilder_2,
       &synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_ab2dq_fa_1,
       &synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_ab2dq_fb_1,
       &synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_M2E_1,
       &synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_S_FunctionBuilder_1,
       &synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_S_FunctionBuilder_2,
       &synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE_h[0],
       &synrmmodel_mt_RTW_P.kcdp, 1, &synrmmodel_mt_RTW_P.kcdi, 1,
       &synrmmodel_mt_RTW_P.kcqp, 1, &synrmmodel_mt_RTW_P.kcqi, 1,
       &synrmmodel_mt_RTW_P.Ld0, 1, &synrmmodel_mt_RTW_P.Lq0, 1,
       &synrmmodel_mt_RTW_P.R0, 1, &synrmmodel_mt_RTW_P.ke, 1,
       &synrmmodel_mt_RTW_P.vdc2, 1, &synrmmodel_mt_RTW_P.tcc, 1);

    /* Fcn: '<S16>/fa1' */
    synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_fa1_1
      = cos(synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_1);

    /* Product: '<S16>/Product4' */
    synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product4_1
      =
      synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_S_FunctionBuilder_1
      * synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_fa1_1;

    /* Product: '<S16>/Product5' */
    synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product5_1
      =
      synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_S_FunctionBuilder_2
      * synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_fa1_1;

    /* Fcn: '<S16>/fa2' */
    synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_fa2_1
      = sin(synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_1);

    /* Product: '<S16>/Product6' */
    synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product6_1
      =
      synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_S_FunctionBuilder_1
      * synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_fa2_1;

    /* Product: '<S16>/Product7' */
    synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product7_1
      =
      synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_S_FunctionBuilder_2
      * synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_fa2_1;

    /* Sum: '<S16>/Subtract1' */
    synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Subtract1_1
      =
      synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product4_1
      - synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product7_1;

    /* Sum: '<S16>/Sum1' */
    synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Sum1_1
      =
      synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product5_1
      + synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product6_1;

    /* S-Function "mycurrentcon_wrapper" Block: <S14>/S_FunctionBuilder */
    mycurrentcon_Update_wrapper
      (&synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_S_FunctionBuilder_1,
       &synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_S_FunctionBuilder_2,
       &synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_ab2dq_fa_1,
       &synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_ab2dq_fb_1,
       &synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_M2E_1,
       &synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_S_FunctionBuilder_1,
       &synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_S_FunctionBuilder_2,
       &synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE_h[0],
       &synrmmodel_mt_RTW_P.kcdp, 1, &synrmmodel_mt_RTW_P.kcdi, 1,
       &synrmmodel_mt_RTW_P.kcqp, 1, &synrmmodel_mt_RTW_P.kcqi, 1,
       &synrmmodel_mt_RTW_P.Ld0, 1, &synrmmodel_mt_RTW_P.Lq0, 1,
       &synrmmodel_mt_RTW_P.R0, 1, &synrmmodel_mt_RTW_P.ke, 1,
       &synrmmodel_mt_RTW_P.vdc2, 1, &synrmmodel_mt_RTW_P.tcc, 1);
  }

  synrmmodel_mt_RT_PrevZCSigState.PIDcontroller_changedperiodic_T = (uint8_T)
    (synrmmodel_mt_RTW_Subsystem2_Trigger_Compare_1 ? (int32_T)POS_ZCSIG :
     (int32_T)ZERO_ZCSIG);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_PIDCONTROLLER_CHANGEDPERIODIC,0,0,SYNRMMODEL_MT_RTW_PIDCONTROLLER_CHANGEDPERIODIC_PARAM);

  /* End of Outputs for SubSystem: '<Root>/PIDcontroller_changedperiodic' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_R2C,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_R2C_PARAM);
  /* RealImagToComplex: '<S8>/R2C' */
  synrmmodel_mt_RTW_Subsystem2_R2C_1.re =
    synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Subtract1_1;
  synrmmodel_mt_RTW_Subsystem2_R2C_1.im =
    synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Sum1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_R2C,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_R2C_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_C2A,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_C2A_PARAM);
  /* ComplexToMagnitudeAngle: '<S8>/C2A' */
  synrmmodel_mt_RTW_Subsystem2_C2A_1 = rt_hypotd_snf
    (synrmmodel_mt_RTW_Subsystem2_R2C_1.re,
     synrmmodel_mt_RTW_Subsystem2_R2C_1.im);
  synrmmodel_mt_RTW_Subsystem2_C2A_2 = rt_atan2d_snf
    (synrmmodel_mt_RTW_Subsystem2_R2C_1.im,
     synrmmodel_mt_RTW_Subsystem2_R2C_1.re);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_C2A,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_C2A_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_E2M2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_E2M2_PARAM);
  /* Gain: '<S8>/E2M2' */
  synrmmodel_mt_RTW_Subsystem2_E2M2_1 = synrmmodel_mt_RTW_P.E2M2_Gain *
    synrmmodel_mt_RTW_Subsystem2_Abs_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_E2M2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_E2M2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_VH,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_VH_PARAM);
  /* Constant: '<S8>/VH' */
  synrmmodel_mt_RTW_Subsystem2_VH_1 = synrmmodel_mt_RTW_P.VH_Value_f;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_VH,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_VH_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_2_PARAM);
  /* UnitDelay: '<S8>/feedback_2' */
  memcpy(&synrmmodel_mt_RTW_Subsystem2_feedback_2_1[0],
         &synrmmodel_mt_RTW_DWork.feedback_2_DSTATE[0], sizeof(real_T) << 6U);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_2_PARAM);
  if (MEASUREMENT_TID_CONDITION(synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[5] == 0)) {
MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_3,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_3_PARAM);
    /* UnitDelay: '<S8>/feedback_3' */
    synrmmodel_mt_RTW_Subsystem2_feedback_3_1 =
      synrmmodel_mt_RTW_DWork.feedback_3_DSTATE;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_3,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_3_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_1_PARAM);
    /* UnitDelay: '<S8>/feedback_1' */
    synrmmodel_mt_RTW_Subsystem2_feedback_1_1 =
      synrmmodel_mt_RTW_DWork.feedback_1_DSTATE;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_SUM1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_SUM1_PARAM);
    /* Sum: '<S25>/Sum1' */
    synrmmodel_mt_RTW_Subsystem2_assignandselector_Sum1_1 =
      synrmmodel_mt_RTW_Subsystem2_feedback_3_1 +
      synrmmodel_mt_RTW_Subsystem2_feedback_1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_SUM1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_SUM1_PARAM);
  }

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_VH3,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_VH3_PARAM);
  /* Constant: '<S25>/VH3' */
  synrmmodel_mt_RTW_Subsystem2_assignandselector_VH3_1 =
    synrmmodel_mt_RTW_P.VH3_Value;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_VH3,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_VH3_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_MATHFUNCTION2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_MATHFUNCTION2_PARAM);
  /* Math: '<S25>/MathFunction2' */
  synrmmodel_mt_RTW_Subsystem2_assignandselector_MathFunction2_1 = rt_modd_snf
    (synrmmodel_mt_RTW_Subsystem2_assignandselector_Sum1_1,
     synrmmodel_mt_RTW_Subsystem2_assignandselector_VH3_1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_MATHFUNCTION2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_MATHFUNCTION2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_BIAS1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_BIAS1_PARAM);
  /* Bias: '<S25>/Bias1' */
  synrmmodel_mt_RTW_Subsystem2_assignandselector_Bias1_1 =
    synrmmodel_mt_RTW_Subsystem2_assignandselector_MathFunction2_1 +
    synrmmodel_mt_RTW_P.Bias1_Bias;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_BIAS1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_BIAS1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_CONVERSION,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_CONVERSION_PARAM);
  /* DataTypeConversion: '<S25>/Conversion' */
  synrmmodel_mt_RTW_Subsystem2_assignandselector_Conversion_1 =
    synrmmodel_mt_RTW_Subsystem2_assignandselector_Bias1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_CONVERSION,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_CONVERSION_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_SELECTOR1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_SELECTOR1_PARAM);
  /* Selector: '<S25>/Selector1' */
  synrmmodel_mt_RTW_Subsystem2_assignandselector_Selector1_1 =
    synrmmodel_mt_RTW_Subsystem2_feedback_2_1[(int32_T)
    synrmmodel_mt_RTW_Subsystem2_assignandselector_Conversion_1 - 1];
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_SELECTOR1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_SELECTOR1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_UNITDELAY1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_UNITDELAY1_PARAM);
  /* UnitDelay: '<S29>/UnitDelay1' */
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay1_1 =
    synrmmodel_mt_RTW_DWork.UnitDelay1_DSTATE_im;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_UNITDELAY1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_UNITDELAY1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_BIAS1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_BIAS1_PARAM);
  /* Bias: '<S29>/Bias1' */
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Bias1_1 =
    synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay1_1 +
    synrmmodel_mt_RTW_P.Bias1_Bias_e;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_BIAS1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_BIAS1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR2_PARAM);
  /* RelationalOperator: '<S29>/RelationalOperator2' */
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator2_1 =
    (synrmmodel_mt_RTW_Subsystem2_Abs_1 <
     synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay1_1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_UNITDELAY2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_UNITDELAY2_PARAM);
  /* UnitDelay: '<S29>/UnitDelay2' */
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay2_1 =
    synrmmodel_mt_RTW_DWork.UnitDelay2_DSTATE_e;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_UNITDELAY2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_UNITDELAY2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR5,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR5_PARAM);
  /* RelationalOperator: '<S29>/RelationalOperator5' */
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator5_1 =
    (synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay2_1 <
     synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay1_1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR5,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR5_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_LOGICALOPERATOR2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_LOGICALOPERATOR2_PARAM);
  /* Logic: '<S29>/LogicalOperator2' */
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator2_1 =
    (synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator2_1 &&
     synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator5_1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_LOGICALOPERATOR2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_LOGICALOPERATOR2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH5,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH5_PARAM);
  /* Switch: '<S29>/Switch5' */
  if (synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator2_1) {
    synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch5_1 =
      synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Bias1_1;
  } else {
    synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch5_1 =
      synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay1_1;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH5,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH5_PARAM);

  /* End of Switch: '<S29>/Switch5' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR_PARAM);
  /* RelationalOperator: '<S29>/RelationalOperator' */
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator_1 =
    (synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch5_1 <=
     synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay2_1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_BIAS,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_BIAS_PARAM);
  /* Bias: '<S29>/Bias' */
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Bias_1 =
    synrmmodel_mt_RTW_Subsystem2_Abs_1 + synrmmodel_mt_RTW_P.Bias_Bias_i;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_BIAS,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_BIAS_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR4,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR4_PARAM);
  /* RelationalOperator: '<S29>/RelationalOperator4' */
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator4_1 =
    (synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay1_1 <=
     synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay2_1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR4,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR4_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_LOGICALOPERATOR1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_LOGICALOPERATOR1_PARAM);
  /* Logic: '<S29>/LogicalOperator1' */
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator1_1 =
    (synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator2_1 &&
     synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator4_1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_LOGICALOPERATOR1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_LOGICALOPERATOR1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH4,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH4_PARAM);
  /* Switch: '<S29>/Switch4' */
  if (synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator1_1) {
    synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch4_1 =
      synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Bias_1;
  } else {
    synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch4_1 =
      synrmmodel_mt_RTW_Subsystem2_Abs_1;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH4,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH4_PARAM);

  /* End of Switch: '<S29>/Switch4' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR1_PARAM);
  /* RelationalOperator: '<S29>/RelationalOperator1' */
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator1_1 =
    (synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay2_1 <
     synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch4_1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_LOGICALOPERATOR,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_LOGICALOPERATOR_PARAM);
  /* Logic: '<S29>/LogicalOperator' */
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator_1 =
    (synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator_1 &&
     synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator1_1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_LOGICALOPERATOR,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_LOGICALOPERATOR_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_HALL_SENSOR_EVENT_DRIVEN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_HALL_SENSOR_EVENT_DRIVEN_PARAM);
  /* Outputs for Triggered SubSystem: '<S8>/Hall_sensor_Event_driven' incorporates:
   *  TriggerPort: '<S18>/Trigger'
   */
  if (MEASUREMENT_IF_CONDITION(synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator_1 &&
      (synrmmodel_mt_RT_PrevZCSigState.Hall_sensor_Event_driven_Trig_Z !=
       POS_ZCSIG))) {
    /* Lookup_n-D: '<S18>/thetaVa' */
    synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_thetaVa_1 =
      look1_binlxpw(synrmmodel_mt_RTW_Subsystem2_Abs_1,
                    synrmmodel_mt_RTW_P.HALL_angle, synrmmodel_mt_RTW_P.HALL_Va,
                    7U);

    /* LookupNDDirect: '<S18>/DirectLookupTablen_D'
     *
     * About '<S18>/DirectLookupTablen_D':
     *  2-dimensional Direct Look-Up returning a Scalar
     */
    if (synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_thetaVa_1 > 1.0) {
      currentTime = 1.0;
    } else if (synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_thetaVa_1 <
               0.0) {
      currentTime = 0.0;
    } else {
      currentTime =
        synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_thetaVa_1;
    }

    currentTime = floor(currentTime);
    if (synrmmodel_mt_RTW_Subsystem2_assignandselector_Selector1_1 > 1.0) {
      tmp = 1.0;
    } else if (synrmmodel_mt_RTW_Subsystem2_assignandselector_Selector1_1 < 0.0)
    {
      tmp = 0.0;
    } else {
      tmp = synrmmodel_mt_RTW_Subsystem2_assignandselector_Selector1_1;
    }

    tmp = floor(tmp);
    if (rtIsNaN(currentTime) || rtIsInf(currentTime)) {
      currentTime = 0.0;
    } else {
      currentTime = fmod(currentTime, 4.294967296E+9);
    }

    if (rtIsNaN(tmp) || rtIsInf(tmp)) {
      tmp = 0.0;
    } else {
      tmp = fmod(tmp, 4.294967296E+9);
    }

    synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_DirectLookupTablen_D_1
      = synrmmodel_mt_RTW_P.DirectLookupTablen_D_table[((uint32_T)currentTime <<
      1) + (uint32_T)tmp];

    /* End of LookupNDDirect: '<S18>/DirectLookupTablen_D' */

    /* Lookup_n-D: '<S18>/thetaVb' */
    synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_thetaVb_1 =
      look1_binlxpw(synrmmodel_mt_RTW_Subsystem2_Abs_1,
                    synrmmodel_mt_RTW_P.HALL_angle, synrmmodel_mt_RTW_P.HALL_Vb,
                    7U);

    /* Constant: '<S18>/VH' */
    synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_VH_1 =
      synrmmodel_mt_RTW_P.VH_Value;

    /* SwitchCase: '<S18>/SwitchCase' */
    if (synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_thetaVb_1 < 0.0) {
      currentTime = ceil
        (synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_thetaVb_1);
    } else {
      currentTime = floor
        (synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_thetaVb_1);
    }

    if (rtIsNaN(currentTime) || rtIsInf(currentTime)) {
      currentTime = 0.0;
    } else {
      currentTime = fmod(currentTime, 4.294967296E+9);
    }

    switch (currentTime < 0.0 ? -(int32_T)(uint32_T)-currentTime : (int32_T)
            (uint32_T)currentTime) {
     case 0:
      /* Outputs for IfAction SubSystem: '<S18>/SwitchCaseActionSubsystem' incorporates:
       *  ActionPort: '<S30>/ActionPort'
       */
      synrm_SwitchCaseActionSubsystem
        (synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_VH_1,
         &synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_Merge_1);

      /* End of Outputs for SubSystem: '<S18>/SwitchCaseActionSubsystem' */
      break;

     case 1:
      /* Outputs for IfAction SubSystem: '<S18>/SwitchCaseActionSubsystem1' incorporates:
       *  ActionPort: '<S31>/ActionPort'
       */
      synr_SwitchCaseActionSubsystem1
        (synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_VH_1,
         &synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_Merge_1,
         (rtP_SwitchCaseActionSubsystem1_ *)
         &synrmmodel_mt_RTW_P.SwitchCaseActionSubsystem1);

      /* End of Outputs for SubSystem: '<S18>/SwitchCaseActionSubsystem1' */
      break;

     case 2:
      /* Outputs for IfAction SubSystem: '<S18>/SwitchCaseActionSubsystem2' incorporates:
       *  ActionPort: '<S32>/ActionPort'
       */
      synr_SwitchCaseActionSubsystem1
        (synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_VH_1,
         &synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_Merge_1,
         (rtP_SwitchCaseActionSubsystem1_ *)
         &synrmmodel_mt_RTW_P.SwitchCaseActionSubsystem2);

      /* End of Outputs for SubSystem: '<S18>/SwitchCaseActionSubsystem2' */
      break;
    }

    /* End of SwitchCase: '<S18>/SwitchCase' */
  }

  synrmmodel_mt_RT_PrevZCSigState.Hall_sensor_Event_driven_Trig_Z = (uint8_T)
    (synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator_1 ? (int32_T)
     POS_ZCSIG : (int32_T)ZERO_ZCSIG);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_HALL_SENSOR_EVENT_DRIVEN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_HALL_SENSOR_EVENT_DRIVEN_PARAM);

  /* End of Outputs for SubSystem: '<S8>/Hall_sensor_Event_driven' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_DIVIDE3,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_DIVIDE3_PARAM);
  /* Product: '<S8>/Divide3' */
  synrmmodel_mt_RTW_Subsystem2_Divide3_1[0] = synrmmodel_mt_RTW_Subsystem2_VH_1 *
    synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_DirectLookupTablen_D_1;
  synrmmodel_mt_RTW_Subsystem2_Divide3_1[1] = synrmmodel_mt_RTW_Subsystem2_VH_1 *
    synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_Merge_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_DIVIDE3,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_DIVIDE3_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_R2C1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_R2C1_PARAM);
  /* RealImagToComplex: '<S8>/R2C1' */
  synrmmodel_mt_RTW_Subsystem2_R2C1_1.re =
    synrmmodel_mt_RTW_Subsystem2_Divide3_1[0];
  synrmmodel_mt_RTW_Subsystem2_R2C1_1.im =
    synrmmodel_mt_RTW_Subsystem2_Divide3_1[1];
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_R2C1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_R2C1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_C2A1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_C2A1_PARAM);
  /* ComplexToMagnitudeAngle: '<S8>/C2A1' */
  synrmmodel_mt_RTW_Subsystem2_C2A1_1 = rt_hypotd_snf
    (synrmmodel_mt_RTW_Subsystem2_R2C1_1.re,
     synrmmodel_mt_RTW_Subsystem2_R2C1_1.im);
  synrmmodel_mt_RTW_Subsystem2_C2A1_2 = rt_atan2d_snf
    (synrmmodel_mt_RTW_Subsystem2_R2C1_1.im,
     synrmmodel_mt_RTW_Subsystem2_R2C1_1.re);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_C2A1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_C2A1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE_PARAM);
  /* Product: '<S20>/Divide' */
  synrmmodel_mt_RTW_Subsystem2_PWM_Divide_1 =
    synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Subtract1_1
    / synrmmodel_mt_RTW_Subsystem2_VH_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_SUM1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_SUM1_PARAM);
  /* Sum: '<S36>/Sum1' */
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sum1_1 =
    synrmmodel_mt_RTW_Subsystem2_PWM_Divide_1 -
    synrmmodel_mt_RTW_Subsystem2_feedback_4_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_SUM1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_SUM1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_SIGN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_SIGN_PARAM);
  /* Signum: '<S36>/Sign' */
  if (synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sum1_1 < 0.0) {
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sign_1 = -1.0;
  } else if (synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sum1_1 > 0.0) {
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sign_1 = 1.0;
  } else if (synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sum1_1 == 0.0) {
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sign_1 = 0.0;
  } else {
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sign_1 =
      synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sum1_1;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_SIGN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_SIGN_PARAM);

  /* End of Signum: '<S36>/Sign' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_CONSTANT1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_CONSTANT1_PARAM);
  /* Constant: '<S36>/Constant1' */
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Constant1_1 =
    synrmmodel_mt_RTW_P.Constant1_Value_e;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_CONSTANT1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_CONSTANT1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_SUM2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_SUM2_PARAM);
  /* Sum: '<S36>/Sum2' */
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sum2_1 =
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sign_1 +
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Constant1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_SUM2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_SUM2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_GAIN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_GAIN_PARAM);
  /* Gain: '<S36>/Gain' */
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Gain_1 =
    synrmmodel_mt_RTW_P.Gain_Gain_n *
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sum2_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_GAIN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP4_GAIN_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE2_PARAM);
  /* Product: '<S20>/Divide2' */
  synrmmodel_mt_RTW_Subsystem2_PWM_Divide2_1 =
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Gain_1 *
    synrmmodel_mt_RTW_Subsystem2_VH_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_UNARYMINUS1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_UNARYMINUS1_PARAM);
  /* UnaryMinus: '<S20>/UnaryMinus1' */
  synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus1_1 =
    -synrmmodel_mt_RTW_Subsystem2_PWM_Divide_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_UNARYMINUS1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_UNARYMINUS1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_SUM1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_SUM1_PARAM);
  /* Sum: '<S33>/Sum1' */
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sum1_1 =
    synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus1_1 -
    synrmmodel_mt_RTW_Subsystem2_feedback_4_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_SUM1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_SUM1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_SIGN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_SIGN_PARAM);
  /* Signum: '<S33>/Sign' */
  if (synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sum1_1 < 0.0) {
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sign_1 = -1.0;
  } else if (synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sum1_1 > 0.0) {
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sign_1 = 1.0;
  } else if (synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sum1_1 == 0.0) {
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sign_1 = 0.0;
  } else {
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sign_1 =
      synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sum1_1;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_SIGN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_SIGN_PARAM);

  /* End of Signum: '<S33>/Sign' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_CONSTANT1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_CONSTANT1_PARAM);
  /* Constant: '<S33>/Constant1' */
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Constant1_1 =
    synrmmodel_mt_RTW_P.Constant1_Value_d;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_CONSTANT1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_CONSTANT1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_SUM2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_SUM2_PARAM);
  /* Sum: '<S33>/Sum2' */
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sum2_1 =
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sign_1 +
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Constant1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_SUM2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_SUM2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_GAIN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_GAIN_PARAM);
  /* Gain: '<S33>/Gain' */
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Gain_1 =
    synrmmodel_mt_RTW_P.Gain_Gain_j *
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sum2_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_GAIN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP1_GAIN_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_UNARYMINUS,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_UNARYMINUS_PARAM);
  /* UnaryMinus: '<S20>/UnaryMinus' */
  synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus_1 =
    -synrmmodel_mt_RTW_Subsystem2_VH_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_UNARYMINUS,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_UNARYMINUS_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE3,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE3_PARAM);
  /* Product: '<S20>/Divide3' */
  synrmmodel_mt_RTW_Subsystem2_PWM_Divide3_1 =
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Gain_1 *
    synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE3,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE3_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_ADD,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_ADD_PARAM);
  /* Sum: '<S20>/Add' */
  synrmmodel_mt_RTW_Subsystem2_PWM_Add_1 =
    synrmmodel_mt_RTW_Subsystem2_PWM_Divide2_1 +
    synrmmodel_mt_RTW_Subsystem2_PWM_Divide3_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_ADD,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_ADD_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE1_PARAM);
  /* Product: '<S20>/Divide1' */
  synrmmodel_mt_RTW_Subsystem2_PWM_Divide1_1 =
    synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Sum1_1
    / synrmmodel_mt_RTW_Subsystem2_VH_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_SUM1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_SUM1_PARAM);
  /* Sum: '<S34>/Sum1' */
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sum1_1 =
    synrmmodel_mt_RTW_Subsystem2_PWM_Divide1_1 -
    synrmmodel_mt_RTW_Subsystem2_feedback_4_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_SUM1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_SUM1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_SIGN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_SIGN_PARAM);
  /* Signum: '<S34>/Sign' */
  if (synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sum1_1 < 0.0) {
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sign_1 = -1.0;
  } else if (synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sum1_1 > 0.0) {
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sign_1 = 1.0;
  } else if (synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sum1_1 == 0.0) {
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sign_1 = 0.0;
  } else {
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sign_1 =
      synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sum1_1;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_SIGN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_SIGN_PARAM);

  /* End of Signum: '<S34>/Sign' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_CONSTANT1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_CONSTANT1_PARAM);
  /* Constant: '<S34>/Constant1' */
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Constant1_1 =
    synrmmodel_mt_RTW_P.Constant1_Value_l;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_CONSTANT1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_CONSTANT1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_SUM2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_SUM2_PARAM);
  /* Sum: '<S34>/Sum2' */
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sum2_1 =
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sign_1 +
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Constant1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_SUM2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_SUM2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_GAIN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_GAIN_PARAM);
  /* Gain: '<S34>/Gain' */
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Gain_1 =
    synrmmodel_mt_RTW_P.Gain_Gain_m *
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sum2_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_GAIN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP2_GAIN_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE4,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE4_PARAM);
  /* Product: '<S20>/Divide4' */
  synrmmodel_mt_RTW_Subsystem2_PWM_Divide4_1 =
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Gain_1 *
    synrmmodel_mt_RTW_Subsystem2_VH_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE4,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE4_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_UNARYMINUS2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_UNARYMINUS2_PARAM);
  /* UnaryMinus: '<S20>/UnaryMinus2' */
  synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus2_1 =
    -synrmmodel_mt_RTW_Subsystem2_PWM_Divide1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_UNARYMINUS2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_UNARYMINUS2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_SUM1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_SUM1_PARAM);
  /* Sum: '<S35>/Sum1' */
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sum1_1 =
    synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus2_1 -
    synrmmodel_mt_RTW_Subsystem2_feedback_4_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_SUM1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_SUM1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_SIGN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_SIGN_PARAM);
  /* Signum: '<S35>/Sign' */
  if (synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sum1_1 < 0.0) {
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sign_1 = -1.0;
  } else if (synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sum1_1 > 0.0) {
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sign_1 = 1.0;
  } else if (synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sum1_1 == 0.0) {
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sign_1 = 0.0;
  } else {
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sign_1 =
      synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sum1_1;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_SIGN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_SIGN_PARAM);

  /* End of Signum: '<S35>/Sign' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_CONSTANT1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_CONSTANT1_PARAM);
  /* Constant: '<S35>/Constant1' */
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Constant1_1 =
    synrmmodel_mt_RTW_P.Constant1_Value_i;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_CONSTANT1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_CONSTANT1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_SUM2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_SUM2_PARAM);
  /* Sum: '<S35>/Sum2' */
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sum2_1 =
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sign_1 +
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Constant1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_SUM2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_SUM2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_GAIN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_GAIN_PARAM);
  /* Gain: '<S35>/Gain' */
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Gain_1 =
    synrmmodel_mt_RTW_P.Gain_Gain_p *
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sum2_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_GAIN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_PWM_COMP3_GAIN_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE5,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE5_PARAM);
  /* Product: '<S20>/Divide5' */
  synrmmodel_mt_RTW_Subsystem2_PWM_Divide5_1 =
    synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Gain_1 *
    synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE5,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_DIVIDE5_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_ADD1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_ADD1_PARAM);
  /* Sum: '<S20>/Add1' */
  synrmmodel_mt_RTW_Subsystem2_PWM_Add1_1 =
    synrmmodel_mt_RTW_Subsystem2_PWM_Divide4_1 +
    synrmmodel_mt_RTW_Subsystem2_PWM_Divide5_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_ADD1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_PWM_ADD1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_COMPARETOCONSTANT_CONSTANT,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_COMPARETOCONSTANT_CONSTANT_PARAM);
  /* Constant: '<S17>/Constant' */
  synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Constant_1 =
    synrmmodel_mt_RTW_P.CompareToConstant_const;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_COMPARETOCONSTANT_CONSTANT,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_COMPARETOCONSTANT_CONSTANT_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_COMPARETOCONSTANT_COMPARE,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_COMPARETOCONSTANT_COMPARE_PARAM);
  /* RelationalOperator: '<S17>/Compare' */
  synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Compare_1 =
    (synrmmodel_mt_RTW_Subsystem2_calc_rpm_Gain_1 <
     synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Constant_1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_COMPARETOCONSTANT_COMPARE,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_COMPARETOCONSTANT_COMPARE_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SWITCH,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SWITCH_PARAM);
  /* Switch: '<S8>/Switch' */
  if (synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Compare_1) {
    synrmmodel_mt_RTW_Subsystem2_Switch_1[0] =
      synrmmodel_mt_RTW_Subsystem2_PWM_Add_1;
  } else {
    synrmmodel_mt_RTW_Subsystem2_Switch_1[0] =
      synrmmodel_mt_RTW_Subsystem2_Divide3_1[0];
  }

  if (synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Compare_1) {
    synrmmodel_mt_RTW_Subsystem2_Switch_1[1] =
      synrmmodel_mt_RTW_Subsystem2_PWM_Add1_1;
  } else {
    synrmmodel_mt_RTW_Subsystem2_Switch_1[1] =
      synrmmodel_mt_RTW_Subsystem2_Divide3_1[1];
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SWITCH,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SWITCH_PARAM);

  /* End of Switch: '<S8>/Switch' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_R2C2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_R2C2_PARAM);
  /* RealImagToComplex: '<S8>/R2C2' */
  synrmmodel_mt_RTW_Subsystem2_R2C2_1.re =
    synrmmodel_mt_RTW_Subsystem2_Switch_1[0];
  synrmmodel_mt_RTW_Subsystem2_R2C2_1.im =
    synrmmodel_mt_RTW_Subsystem2_Switch_1[1];
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_R2C2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_R2C2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_C2A2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_C2A2_PARAM);
  /* ComplexToMagnitudeAngle: '<S8>/C2A2' */
  synrmmodel_mt_RTW_Subsystem2_C2A2_1 = rt_hypotd_snf
    (synrmmodel_mt_RTW_Subsystem2_R2C2_1.re,
     synrmmodel_mt_RTW_Subsystem2_R2C2_1.im);
  synrmmodel_mt_RTW_Subsystem2_C2A2_2 = rt_atan2d_snf
    (synrmmodel_mt_RTW_Subsystem2_R2C2_1.im,
     synrmmodel_mt_RTW_Subsystem2_R2C2_1.re);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_C2A2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_C2A2_PARAM);
  if (MEASUREMENT_TID_CONDITION(synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[4] == 0)) {
  }

  if (MEASUREMENT_TID_CONDITION(synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[2] == 0)) {
  }

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_SWITCH1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_SWITCH1_PARAM);
  /* Switch: '<S22>/Switch1' */
  if (synrmmodel_mt_RTW_Subsystem2_Subsystem_Sum6_1 >
      synrmmodel_mt_RTW_P.Switch1_Threshold) {
    synrmmodel_mt_RTW_Subsystem2_Subsystem_Switch1_1 =
      synrmmodel_mt_RTW_Subsystem2_Subsystem_Sum6_1;
  } else {
    synrmmodel_mt_RTW_Subsystem2_Subsystem_Switch1_1 =
      synrmmodel_mt_RTW_Subsystem2_Subsystem_Constant_1;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_SWITCH1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM_SWITCH1_PARAM);

  /* End of Switch: '<S22>/Switch1' */
  if (MEASUREMENT_TID_CONDITION(synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[5] == 0)) {
MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_CONSTANT,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_CONSTANT_PARAM);
    /* Constant: '<S23>/Constant' */
    synrmmodel_mt_RTW_Subsystem2_Subsystem1_Constant_1 =
      synrmmodel_mt_RTW_P.Constant_Value_i;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_CONSTANT,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_CONSTANT_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_CONSTANT6,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_CONSTANT6_PARAM);
    /* Constant: '<S23>/Constant6' */
    synrmmodel_mt_RTW_Subsystem2_Subsystem1_Constant6_1 =
      synrmmodel_mt_RTW_P.Constant6_Value_o;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_CONSTANT6,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_CONSTANT6_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_VH1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_VH1_PARAM);
    /* Constant: '<S8>/VH1' */
    synrmmodel_mt_RTW_Subsystem2_VH1_1 = synrmmodel_mt_RTW_P.VH1_Value;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_VH1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_VH1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_DIVIDE,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_DIVIDE_PARAM);
    /* Product: '<S23>/Divide' */
    synrmmodel_mt_RTW_Subsystem2_Subsystem1_Divide_1 =
      synrmmodel_mt_RTW_Subsystem2_Subsystem1_Constant_1 /
      synrmmodel_mt_RTW_Subsystem2_VH1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_DIVIDE,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_DIVIDE_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_DIVIDE1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_DIVIDE1_PARAM);
    /* Product: '<S23>/Divide1' */
    synrmmodel_mt_RTW_Subsystem2_Subsystem1_Divide1_1 = 1.0 /
      synrmmodel_mt_RTW_Subsystem2_Subsystem1_Divide_1 *
      synrmmodel_mt_RTW_Subsystem2_Subsystem1_Constant6_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_DIVIDE1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_DIVIDE1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_SUM6,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_SUM6_PARAM);
    /* Sum: '<S23>/Sum6' */
    synrmmodel_mt_RTW_Subsystem2_Subsystem1_Sum6_1 =
      synrmmodel_mt_RTW_Subsystem2_feedback_3_1 -
      synrmmodel_mt_RTW_Subsystem2_Subsystem1_Divide1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_SUM6,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_SUM6_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_UNARYMINUS,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_UNARYMINUS_PARAM);
    /* UnaryMinus: '<S23>/UnaryMinus' */
    synrmmodel_mt_RTW_Subsystem2_Subsystem1_UnaryMinus_1 =
      -synrmmodel_mt_RTW_Subsystem2_Subsystem1_Sum6_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_UNARYMINUS,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_UNARYMINUS_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_SIGN2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_SIGN2_PARAM);
    /* Signum: '<S23>/Sign2' */
    if (synrmmodel_mt_RTW_Subsystem2_Subsystem1_UnaryMinus_1 < 0.0) {
      synrmmodel_mt_RTW_Subsystem2_Subsystem1_Sign2_1 = -1.0;
    } else if (synrmmodel_mt_RTW_Subsystem2_Subsystem1_UnaryMinus_1 > 0.0) {
      synrmmodel_mt_RTW_Subsystem2_Subsystem1_Sign2_1 = 1.0;
    } else if (synrmmodel_mt_RTW_Subsystem2_Subsystem1_UnaryMinus_1 == 0.0) {
      synrmmodel_mt_RTW_Subsystem2_Subsystem1_Sign2_1 = 0.0;
    } else {
      synrmmodel_mt_RTW_Subsystem2_Subsystem1_Sign2_1 =
        synrmmodel_mt_RTW_Subsystem2_Subsystem1_UnaryMinus_1;
    }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_SIGN2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_SIGN2_PARAM);

    /* End of Signum: '<S23>/Sign2' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_SWITCH1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_SWITCH1_PARAM);
    /* Switch: '<S23>/Switch1' */
    if (synrmmodel_mt_RTW_Subsystem2_Subsystem1_Sum6_1 >
        synrmmodel_mt_RTW_P.Switch1_Threshold_g) {
      synrmmodel_mt_RTW_Subsystem2_Subsystem1_Switch1_1 =
        synrmmodel_mt_RTW_Subsystem2_Subsystem1_Sum6_1;
    } else {
      synrmmodel_mt_RTW_Subsystem2_Subsystem1_Switch1_1 =
        synrmmodel_mt_RTW_Subsystem2_Subsystem1_Constant_1;
    }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_SWITCH1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_SUBSYSTEM1_SWITCH1_PARAM);

    /* End of Switch: '<S23>/Switch1' */
  }

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_ASSIGNMENT,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_ASSIGNMENT_PARAM);
  /* Assignment: '<S25>/Assignment' */
  memcpy
    (&synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf
     [0], &synrmmodel_mt_RTW_Subsystem2_feedback_2_1[0], sizeof(real_T) << 6U);
  synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf
    [(int32_T)synrmmodel_mt_RTW_Subsystem2_assignandselector_Conversion_1 - 1] =
    synrmmodel_mt_RTW_Subsystem2_calc_rpm_Gain_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_ASSIGNMENT,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_ASSIGNANDSELECTOR_ASSIGNMENT_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_THETA_STEP,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_THETA_STEP_PARAM);
  /* Constant: '<S29>/theta_step' */
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_theta_step_1 =
    synrmmodel_mt_RTW_P.theta_step_Value;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_THETA_STEP,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_THETA_STEP_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_GENTEN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_GENTEN_PARAM);
  /* Constant: '<S29>/genten' */
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_genten_1 =
    synrmmodel_mt_RTW_P.genten_Value;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_GENTEN,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_GENTEN_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH1_PARAM);
  /* Switch: '<S29>/Switch1' */
  if (synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator_1) {
    synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch1_1 =
      synrmmodel_mt_RTW_Subsystem2_triggerfortheta_theta_step_1;
  } else {
    synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch1_1 =
      synrmmodel_mt_RTW_Subsystem2_triggerfortheta_genten_1;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH1,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH1_PARAM);

  /* End of Switch: '<S29>/Switch1' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_ADD2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_ADD2_PARAM);
  /* Sum: '<S29>/Add2' */
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Add2_1 =
    synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay2_1 +
    synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_ADD2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_ADD2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_CONSTANT4,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_CONSTANT4_PARAM);
  /* Constant: '<S29>/Constant4' */
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Constant4_1 =
    synrmmodel_mt_RTW_P.Constant4_Value;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_CONSTANT4,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_CONSTANT4_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_CONSTANT7,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_CONSTANT7_PARAM);
  /* Constant: '<S29>/Constant7' */
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Constant7_1 =
    synrmmodel_mt_RTW_P.Constant7_Value;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_CONSTANT7,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_CONSTANT7_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR3,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR3_PARAM);
  /* RelationalOperator: '<S29>/RelationalOperator3' */
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator3_1 =
    (synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Constant4_1 <=
     synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Add2_1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR3,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_RELATIONALOPERATOR3_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH2_PARAM);
  /* Switch: '<S29>/Switch2' */
  if (synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator3_1) {
    synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch2_1 =
      synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Constant7_1;
  } else {
    synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch2_1 =
      synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Add2_1;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH2,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH2_PARAM);

  /* End of Switch: '<S29>/Switch2' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH3,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH3_PARAM);
  /* Switch: '<S29>/Switch3' */
  if (synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator_1) {
    synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch3_1 =
      synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch2_1;
  } else {
    synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch3_1 =
      synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay2_1;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH3,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SWITCH3_PARAM);

  /* End of Switch: '<S29>/Switch3' */

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SATURATION,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SATURATION_PARAM);
  /* Saturate: '<S29>/Saturation' */
  if (synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch3_1 >
      synrmmodel_mt_RTW_P.Saturation_UpperSat_n) {
    synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Saturation_1 =
      synrmmodel_mt_RTW_P.Saturation_UpperSat_n;
  } else if (synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch3_1 <
             synrmmodel_mt_RTW_P.Saturation_LowerSat_b) {
    synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Saturation_1 =
      synrmmodel_mt_RTW_P.Saturation_LowerSat_b;
  } else {
    synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Saturation_1 =
      synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch3_1;
  }
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SATURATION,0,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_SATURATION_PARAM);

  /* End of Saturate: '<S29>/Saturation' */
  if (MEASUREMENT_TID_CONDITION(synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[5] == 0)) {
MEASUREMENT_START(SYNRMMODEL_MT_RTW_VELOPOSIERROR_RADERROR2,0,0,SYNRMMODEL_MT_RTW_VELOPOSIERROR_RADERROR2_PARAM);
    /* Fcn: '<S9>/raderror2' */
    synrmmodel_mt_RTW_VeloPosiError_raderror2_1 =
      synrmmodel_mt_RTW_Subsystem2_E2M3_1 - synrmmodel_mt_RTW_Subsystem2_E2M_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_VELOPOSIERROR_RADERROR2,0,0,SYNRMMODEL_MT_RTW_VELOPOSIERROR_RADERROR2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_VELOPOSIERROR_RADERROR1,0,0,SYNRMMODEL_MT_RTW_VELOPOSIERROR_RADERROR1_PARAM);
    /* Fcn: '<S9>/raderror1' */
    synrmmodel_mt_RTW_VeloPosiError_raderror1_1 =
      synrmmodel_mt_RTW_Observer_observerab_S_FunctionBuilder2_1 -
      synrmmodel_mt_RTW_Zero_OrderHold1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_VELOPOSIERROR_RADERROR1,0,0,SYNRMMODEL_MT_RTW_VELOPOSIERROR_RADERROR1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_VELOPOSIERROR_S_FUNCTIONBUILDER,0,0,SYNRMMODEL_MT_RTW_VELOPOSIERROR_S_FUNCTIONBUILDER_PARAM);
    /* S-Function (mydegfilter): '<S9>/S_FunctionBuilder' */
    mydegfilter_Outputs_wrapper(&synrmmodel_mt_RTW_VeloPosiError_raderror1_1,
      &synrmmodel_mt_RTW_VeloPosiError_S_FunctionBuilder_1,
      &synrmmodel_mt_RTW_P.tob, 1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_VELOPOSIERROR_S_FUNCTIONBUILDER,0,0,SYNRMMODEL_MT_RTW_VELOPOSIERROR_S_FUNCTIONBUILDER_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_VELOPOSIERROR_RADTODEG,0,0,SYNRMMODEL_MT_RTW_VELOPOSIERROR_RADTODEG_PARAM);
    /* Gain: '<S9>/radtodeg' */
    synrmmodel_mt_RTW_VeloPosiError_position_err = synrmmodel_mt_RTW_P.r2d *
      synrmmodel_mt_RTW_VeloPosiError_S_FunctionBuilder_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_VELOPOSIERROR_RADTODEG,0,0,SYNRMMODEL_MT_RTW_VELOPOSIERROR_RADTODEG_PARAM);
  }

MEASUREMENT_START(SYNRMMODEL_MT_RTW_DATASTORE_CLOCK1,0,0,SYNRMMODEL_MT_RTW_DATASTORE_CLOCK1_PARAM);
  /* Clock: '<S1>/Clock1' */
  synrmmodel_mt_RTW_DataStore_Clock1_1 = synrmmodel_mt_RTW_M->Timing.t[0];
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_DATASTORE_CLOCK1,0,0,SYNRMMODEL_MT_RTW_DATASTORE_CLOCK1_PARAM);
  if (MEASUREMENT_TID_CONDITION(synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[6] == 0)) {
MEASUREMENT_START(SYNRMMODEL_MT_RTW_DATASTORE_TOWORKSPACE,0,0,SYNRMMODEL_MT_RTW_DATASTORE_TOWORKSPACE_PARAM);
    /* SignalConversion: '<S1>/TmpSignal ConversionAtToWorkspaceInport1' */
    rtb_TmpSignalConversionAtToWork[0] =
      synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Sum_1;
    rtb_TmpSignalConversionAtToWork[1] = 0.0;
    rtb_TmpSignalConversionAtToWork[2] = 0.0;
    rtb_TmpSignalConversionAtToWork[3] = synrmmodel_mt_RTW_Zero_OrderHold1_1;
    rtb_TmpSignalConversionAtToWork[4] =
      synrmmodel_mt_RTW_Observer_observerab_S_FunctionBuilder2_1;
    rtb_TmpSignalConversionAtToWork[5] = 0.0;
    rtb_TmpSignalConversionAtToWork[6] =
      synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_S_FunctionBuilder_1;
    rtb_TmpSignalConversionAtToWork[7] =
      synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_S_FunctionBuilder_2;
    rtb_TmpSignalConversionAtToWork[8] =
      synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_1;
    rtb_TmpSignalConversionAtToWork[9] =
      synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_2;
    rtb_TmpSignalConversionAtToWork[10] = synrmmodel_mt_RTW_DataStore_Clock1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_DATASTORE_TOWORKSPACE,0,0,SYNRMMODEL_MT_RTW_DATASTORE_TOWORKSPACE_PARAM);
  }

MEASUREMENT_START(SYNRMMODEL_MT_RTW_UNITDELAY,1,0,SYNRMMODEL_MT_RTW_UNITDELAY_PARAM);
  /* Update for UnitDelay: '<Root>/UnitDelay' */
  synrmmodel_mt_RTW_DWork.UnitDelay_DSTATE =
    synrmmodel_mt_RTW_Subsystem2_Switch_1[0];
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_UNITDELAY,1,0,SYNRMMODEL_MT_RTW_UNITDELAY_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_UNITDELAY2,1,0,SYNRMMODEL_MT_RTW_UNITDELAY2_PARAM);
  /* Update for UnitDelay: '<Root>/UnitDelay2' */
  synrmmodel_mt_RTW_DWork.UnitDelay2_DSTATE =
    synrmmodel_mt_RTW_Subsystem2_Switch_1[1];
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_UNITDELAY2,1,0,SYNRMMODEL_MT_RTW_UNITDELAY2_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_IPMSM1_S_FUNCTIONBUILDER,1,0,SYNRMMODEL_MT_RTW_IPMSM1_S_FUNCTIONBUILDER_PARAM);
  /* S-Function "mydIpmsm_wrapper" Block: <S2>/S_FunctionBuilder */
  mydIpmsm_Update_wrapper(&synrmmodel_mt_RTW_UnitDelay_1,
    &synrmmodel_mt_RTW_UnitDelay2_1, &synrmmodel_mt_RTW_MotorPara1_Constant2_1,
    &synrmmodel_mt_RTW_MotorPara1_Constant1_1,
    &synrmmodel_mt_RTW_MotorPara1_Constant_1,
    &synrmmodel_mt_RTW_IPMSM1_Constant_1,
    &synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_1,
    &synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_2,
    &synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_3,
    &synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_4,
    &synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_5,
    &synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_6,
    &synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE[0], &synrmmodel_mt_RTW_P.p,
    1, &synrmmodel_mt_RTW_P.jm, 1, &synrmmodel_mt_RTW_P.d, 1,
    &synrmmodel_mt_RTW_P.wre0, 1, &synrmmodel_mt_RTW_P.theta0, 1,
    &synrmmodel_mt_RTW_P.ke, 1, &synrmmodel_mt_RTW_P.tmo, 1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_IPMSM1_S_FUNCTIONBUILDER,1,0,SYNRMMODEL_MT_RTW_IPMSM1_S_FUNCTIONBUILDER_PARAM);
  if (MEASUREMENT_TID_CONDITION(synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[5] == 0)) {
MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_UNITDELAY,1,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_UNITDELAY_PARAM);
    /* Update for UnitDelay: '<S8>/UnitDelay' */
    synrmmodel_mt_RTW_DWork.UnitDelay_DSTATE_n[0] =
      synrmmodel_mt_RTW_Subsystem2_Switch_1[0];
    synrmmodel_mt_RTW_DWork.UnitDelay_DSTATE_n[1] =
      synrmmodel_mt_RTW_Subsystem2_Switch_1[1];
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_UNITDELAY,1,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_UNITDELAY_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_UNITDELAY1,1,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_UNITDELAY1_PARAM);
    /* Update for UnitDelay: '<S8>/UnitDelay1' */
    synrmmodel_mt_RTW_DWork.UnitDelay1_DSTATE =
      synrmmodel_mt_RTW_Observer_observerab_S_FunctionBuilder1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_UNITDELAY1,1,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_UNITDELAY1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_UNITDELAY1,1,0,SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_UNITDELAY1_PARAM);
    /* Update for UnitDelay: '<S11>/UnitDelay1' */
    synrmmodel_mt_RTW_DWork.UnitDelay1_DSTATE_j =
      synrmmodel_mt_RTW_Observer_observerab_Subsystem_Sum_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_UNITDELAY1,1,0,SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_UNITDELAY1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_S_FUNCTIONBUILDER1,1,0,SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_S_FUNCTIONBUILDER1_PARAM);
    /* S-Function "myvelocityesti_wrapper" Block: <S11>/S_FunctionBuilder1 */
    myvelocityesti_Update_wrapper
      (&synrmmodel_mt_RTW_Observer_observerab_Subsystem_S_FunctionBuilder_1,
       &synrmmodel_mt_RTW_Observer_observerab_Subsystem_S_FunctionBuilder_2,
       &synrmmodel_mt_RTW_Observer_observerab_S_FunctionBuilder1_1,
       &synrmmodel_mt_RTW_DWork.S_FunctionBuilder1_DSTATE[0],
       &synrmmodel_mt_RTW_P.Kvp, 1, &synrmmodel_mt_RTW_P.Kvi, 1,
       &synrmmodel_mt_RTW_P.Kvpp, 1, &synrmmodel_mt_RTW_P.wre0, 1,
       &synrmmodel_mt_RTW_P.tob, 1);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_S_FUNCTIONBUILDER1,1,0,SYNRMMODEL_MT_RTW_OBSERVER_OBSERVERAB_S_FUNCTIONBUILDER1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_UNITDELAY1,1,0,SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_UNITDELAY1_PARAM);
    /* Update for UnitDelay: '<S6>/UnitDelay1' */
    synrmmodel_mt_RTW_DWork.UnitDelay1_DSTATE_i =
      synrmmodel_mt_RTW_Outputwtheta1_Subsystem_Sum_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_UNITDELAY1,1,0,SYNRMMODEL_MT_RTW_OUTPUTWTHETA1_UNITDELAY1_PARAM);
  }

  if (MEASUREMENT_TID_CONDITION(synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[3] == 0)) {
MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_4,1,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_4_PARAM);
    /* Update for UnitDelay: '<S8>/feedback_4' */
    synrmmodel_mt_RTW_DWork.feedback_4_DSTATE =
      synrmmodel_mt_RTW_Subsystem2_Subsystem_Switch1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_4,1,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_4_PARAM);
  }

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_2,1,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_2_PARAM);
  /* Update for UnitDelay: '<S8>/feedback_2' */
  memcpy(&synrmmodel_mt_RTW_DWork.feedback_2_DSTATE[0],
         &synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf
         [0], sizeof(real_T) << 6U);
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_2,1,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_2_PARAM);
  if (MEASUREMENT_TID_CONDITION(synrmmodel_mt_RTW_M->Timing.TaskCounters.TID[5] == 0)) {
MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_3,1,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_3_PARAM);
    /* Update for UnitDelay: '<S8>/feedback_3' */
    synrmmodel_mt_RTW_DWork.feedback_3_DSTATE =
      synrmmodel_mt_RTW_Subsystem2_Subsystem1_Switch1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_3,1,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_3_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_1,1,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_1_PARAM);
    /* Update for UnitDelay: '<S8>/feedback_1' */
    synrmmodel_mt_RTW_DWork.feedback_1_DSTATE =
      synrmmodel_mt_RTW_Subsystem2_assignandselector_Sum1_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_1,1,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_FEEDBACK_1_PARAM);
  }

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_UNITDELAY1,1,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_UNITDELAY1_PARAM);
  /* Update for UnitDelay: '<S29>/UnitDelay1' */
  synrmmodel_mt_RTW_DWork.UnitDelay1_DSTATE_im =
    synrmmodel_mt_RTW_Subsystem2_Abs_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_UNITDELAY1,1,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_UNITDELAY1_PARAM);

MEASUREMENT_START(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_UNITDELAY2,1,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_UNITDELAY2_PARAM);
  /* Update for UnitDelay: '<S29>/UnitDelay2' */
  synrmmodel_mt_RTW_DWork.UnitDelay2_DSTATE_e =
    synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Saturation_1;
MEASUREMENT_STOP(SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_UNITDELAY2,1,0,SYNRMMODEL_MT_RTW_SUBSYSTEM2_TRIGGERFORTHETA_UNITDELAY2_PARAM);

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++synrmmodel_mt_RTW_M->Timing.clockTick0)) {
    ++synrmmodel_mt_RTW_M->Timing.clockTickH0;
  }

  synrmmodel_mt_RTW_M->Timing.t[0] = synrmmodel_mt_RTW_M->Timing.clockTick0 *
    synrmmodel_mt_RTW_M->Timing.stepSize0 +
    synrmmodel_mt_RTW_M->Timing.clockTickH0 *
    synrmmodel_mt_RTW_M->Timing.stepSize0 * 4294967296.0;

  {
    /* Update absolute timer for sample time: [1.0E-6s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The resolution of this integer timer is 1.0E-6, which is the step size
     * of the task. Size of "clockTick1" ensures timer will not overflow during the
     * application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    synrmmodel_mt_RTW_M->Timing.clockTick1++;
    if (!synrmmodel_mt_RTW_M->Timing.clockTick1) {
      synrmmodel_mt_RTW_M->Timing.clockTickH1++;
    }
  }

  rate_scheduler();
MEASUREMENT_STEP_END();
}

/* Model initialize function */
void synrmmodel_mt_RTW_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)synrmmodel_mt_RTW_M, 0,
                sizeof(RT_MODEL_synrmmodel_mt_RTW));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&synrmmodel_mt_RTW_M->solverInfo,
                          &synrmmodel_mt_RTW_M->Timing.simTimeStep);
    rtsiSetTPtr(&synrmmodel_mt_RTW_M->solverInfo, &rtmGetTPtr
                (synrmmodel_mt_RTW_M));
    rtsiSetStepSizePtr(&synrmmodel_mt_RTW_M->solverInfo,
                       &synrmmodel_mt_RTW_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&synrmmodel_mt_RTW_M->solverInfo, ((const char_T **)
      (&rtmGetErrorStatus(synrmmodel_mt_RTW_M))));
    rtsiSetRTModelPtr(&synrmmodel_mt_RTW_M->solverInfo, synrmmodel_mt_RTW_M);
  }

  rtsiSetSimTimeStep(&synrmmodel_mt_RTW_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&synrmmodel_mt_RTW_M->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr(synrmmodel_mt_RTW_M, &synrmmodel_mt_RTW_M->Timing.tArray[0]);
  synrmmodel_mt_RTW_M->Timing.stepSize0 = 1.0E-6;

  /* block I/O */

  /* exported global signals */
  synrmmodel_mt_RTW_Subsystem2_R2C_1.re = 0.0;
  synrmmodel_mt_RTW_Subsystem2_R2C_1.im = 0.0;
  synrmmodel_mt_RTW_Subsystem2_R2C1_1.re = 0.0;
  synrmmodel_mt_RTW_Subsystem2_R2C1_1.im = 0.0;
  synrmmodel_mt_RTW_Subsystem2_R2C2_1.re = 0.0;
  synrmmodel_mt_RTW_Subsystem2_R2C2_1.im = 0.0;
  synrmmodel_mt_RTW_Subsystem2_VH2_1 = 0.0;
  synrmmodel_mt_RTW_UnitDelay_1 = 0.0;
  synrmmodel_mt_RTW_UnitDelay2_1 = 0.0;
  synrmmodel_mt_RTW_MotorPara1_Constant2_1 = 0.0;
  synrmmodel_mt_RTW_MotorPara1_Constant1_1 = 0.0;
  synrmmodel_mt_RTW_MotorPara1_Constant_1 = 0.0;
  synrmmodel_mt_RTW_IPMSM1_Constant_1 = 0.0;
  synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_1 = 0.0;
  synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_2 = 0.0;
  synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_3 = 0.0;
  synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_4 = 0.0;
  synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_5 = 0.0;
  synrmmodel_mt_RTW_IPMSM1_S_FunctionBuilder_6 = 0.0;
  synrmmodel_mt_RTW_Zero_OrderHold1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_E2M1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Bias_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Merge_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_MathFunction_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Abs_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PulseGenerator3_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Step_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Clock_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Constant_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Sum_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Product_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Constant1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Ramp_Output_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Saturation_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Step_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Referencevelocity_StepDown_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Referencevelocity_Sum_1 = 0.0;
  synrmmodel_mt_RTW_Zero_OrderHold2_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_E2M_1 = 0.0;
  synrmmodel_mt_RTW_Zero_OrderHold_1 = 0.0;
  synrmmodel_mt_RTW_Zero_OrderHold3_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_UnitDelay_1[0] = 0.0;
  synrmmodel_mt_RTW_Subsystem2_UnitDelay_1[1] = 0.0;
  synrmmodel_mt_RTW_Subsystem2_UnitDelay1_1 = 0.0;
  synrmmodel_mt_RTW_Observer_observerab_Constant_1 = 0.0;
  synrmmodel_mt_RTW_Observer_observerab_UnitDelay1_1 = 0.0;
  synrmmodel_mt_RTW_Observer_observerab_S_FunctionBuilder1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_E2M3_1 = 0.0;
  synrmmodel_mt_RTW_Observer_observerab_S_FunctionBuilder2_1 = 0.0;
  synrmmodel_mt_RTW_Outputwtheta1_Constant_1 = 0.0;
  synrmmodel_mt_RTW_Outputwtheta1_UnitDelay1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Reffreq_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Subsystem_Constant_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_ModeSelector_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Constant1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Constant2_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_MultiportSwitch_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Subsystem_Divide_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Subsystem_Constant6_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Subsystem_Divide1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_feedback_4_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Subsystem_Sum6_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Subsystem_UnaryMinus_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Subsystem_Sign2_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Trigger_Constant_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_C2A_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_C2A_2 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_E2M2_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_VH_1 = 0.0;

  {
    int_T i;
    for (i = 0; i < 64; i++) {
      synrmmodel_mt_RTW_Subsystem2_feedback_2_1[i] = 0.0;
    }
  }

  synrmmodel_mt_RTW_Subsystem2_feedback_3_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_feedback_1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_assignandselector_Sum1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_assignandselector_VH3_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_assignandselector_MathFunction2_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_assignandselector_Bias1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_assignandselector_Conversion_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_assignandselector_Selector1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Bias1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_UnitDelay2_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch5_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Bias_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch4_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Divide3_1[0] = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Divide3_1[1] = 0.0;
  synrmmodel_mt_RTW_Subsystem2_C2A1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_C2A1_2 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_Divide_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sum1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sign_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Constant1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Sum2_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp4_Gain_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_Divide2_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sum1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sign_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Constant1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Sum2_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp1_Gain_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_Divide3_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_Add_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_Divide1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sum1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sign_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Constant1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Sum2_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp2_Gain_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_Divide4_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_UnaryMinus2_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sum1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sign_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Constant1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Sum2_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_PWM_comp3_Gain_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_Divide5_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_PWM_Add1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Constant_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Switch_1[0] = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Switch_1[1] = 0.0;
  synrmmodel_mt_RTW_Subsystem2_C2A2_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_C2A2_2 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Subsystem_Switch1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Subsystem1_Constant_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Subsystem1_Constant6_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_VH1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Subsystem1_Divide_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Subsystem1_Divide1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Subsystem1_Sum6_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Subsystem1_UnaryMinus_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Subsystem1_Sign2_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Subsystem1_Switch1_1 = 0.0;

  {
    int_T i;
    for (i = 0; i < 64; i++) {
      synrmmodel_mt_RTW_Subsystem2_assignandselector_Assignment_s_mbbg_rpm_2ms_buf
        [i] = 0.0;
    }
  }

  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_theta_step_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_genten_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Add2_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Constant4_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Constant7_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch2_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Switch3_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_Saturation_1 = 0.0;
  synrmmodel_mt_RTW_VeloPosiError_raderror2_1 = 0.0;
  synrmmodel_mt_RTW_VeloPosiError_raderror1_1 = 0.0;
  synrmmodel_mt_RTW_VeloPosiError_S_FunctionBuilder_1 = 0.0;
  synrmmodel_mt_RTW_VeloPosiError_position_err = 0.0;
  synrmmodel_mt_RTW_DataStore_Clock1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_calc_rpm_UnitDelay_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_calc_rpm_Subtract_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_calc_rpm_Constant2_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_calc_rpm_Add_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_calc_rpm_Constant1_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_calc_rpm_Switch_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_calc_rpm_Gain_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_thetaVa_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_DirectLookupTablen_D_1 =
    0.0;
  synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_thetaVb_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_VH_1 = 0.0;
  synrmmodel_mt_RTW_Subsystem2_Hall_sensor_Event_driven_Merge_1 = 0.0;
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_M2E_1 = 0.0;
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_ab2dq_fa_1 =
    0.0;
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_ab2dq_fb_1 =
    0.0;
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_S_FunctionBuilder_1
    = 0.0;
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_S_FunctionBuilder_2
    = 0.0;
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_fa1_1 =
    0.0;
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product4_1
    = 0.0;
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product5_1
    = 0.0;
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_fa2_1 =
    0.0;
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product6_1
    = 0.0;
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Product7_1
    = 0.0;
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Subtract1_1
    = 0.0;
  synrmmodel_mt_RTW_PIDcontroller_changedperiodic_Currentcontroller_dq2ab_Sum1_1
    = 0.0;
  synrmmodel_mt_RTW_Outputwtheta1_Subsystem_Sum_1 = 0.0;
  synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_1 = 0.0;
  synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_2 = 0.0;
  synrmmodel_mt_RTW_Outputwtheta1_Subsystem_S_FunctionBuilder_3 = 0.0;
  synrmmodel_mt_RTW_Observer_observerab_Subsystem_Sum_1 = 0.0;
  synrmmodel_mt_RTW_Observer_observerab_Subsystem_S_FunctionBuilder_1 = 0.0;
  synrmmodel_mt_RTW_Observer_observerab_Subsystem_S_FunctionBuilder_2 = 0.0;
  synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_wrmerr_1 = 0.0;
  synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_m2e_1 = 0.0;
  synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_S_FunctionBuilder_1 =
    0.0;
  synrmmodel_mt_RTW_Maketargetvalue1_Verocitycontroller_S_FunctionBuilder_2 =
    0.0;
  synrmmodel_mt_RTW_Subsystem2_Trigger_Compare_1 = false;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator2_1 = false;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator5_1 = false;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator2_1 = false;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator_1 = false;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator4_1 = false;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator1_1 = false;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator1_1 = false;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_LogicalOperator_1 = false;
  synrmmodel_mt_RTW_Subsystem2_CompareToConstant_Compare_1 = false;
  synrmmodel_mt_RTW_Subsystem2_triggerfortheta_RelationalOperator3_1 = false;
  synrmmodel_mt_RTW_Subsystem2_calc_rpm_RelationalOperator3_1 = false;

  /* states (dwork) */
  (void) memset((void *)&synrmmodel_mt_RTW_DWork, 0,
                sizeof(D_Work_synrmmodel_mt_RTW));
  synrmmodel_mt_RTW_DWork.UnitDelay_DSTATE = 0.0;
  synrmmodel_mt_RTW_DWork.UnitDelay2_DSTATE = 0.0;
  synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE[0] = 0.0;
  synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE[1] = 0.0;
  synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE[2] = 0.0;
  synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE[3] = 0.0;
  synrmmodel_mt_RTW_DWork.UnitDelay_DSTATE_n[0] = 0.0;
  synrmmodel_mt_RTW_DWork.UnitDelay_DSTATE_n[1] = 0.0;
  synrmmodel_mt_RTW_DWork.UnitDelay1_DSTATE = 0.0;
  synrmmodel_mt_RTW_DWork.UnitDelay1_DSTATE_j = 0.0;
  synrmmodel_mt_RTW_DWork.S_FunctionBuilder1_DSTATE[0] = 0.0;
  synrmmodel_mt_RTW_DWork.S_FunctionBuilder1_DSTATE[1] = 0.0;
  synrmmodel_mt_RTW_DWork.S_FunctionBuilder1_DSTATE[2] = 0.0;
  synrmmodel_mt_RTW_DWork.S_FunctionBuilder1_DSTATE[3] = 0.0;
  synrmmodel_mt_RTW_DWork.UnitDelay1_DSTATE_i = 0.0;
  synrmmodel_mt_RTW_DWork.feedback_4_DSTATE = 0.0;

  {
    int_T i;
    for (i = 0; i < 64; i++) {
      synrmmodel_mt_RTW_DWork.feedback_2_DSTATE[i] = 0.0;
    }
  }

  synrmmodel_mt_RTW_DWork.feedback_3_DSTATE = 0.0;
  synrmmodel_mt_RTW_DWork.feedback_1_DSTATE = 0.0;
  synrmmodel_mt_RTW_DWork.UnitDelay1_DSTATE_im = 0.0;
  synrmmodel_mt_RTW_DWork.UnitDelay2_DSTATE_e = 0.0;
  synrmmodel_mt_RTW_DWork.UnitDelay_DSTATE_d = 0.0;
  synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE_h[0] = 0.0;
  synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE_h[1] = 0.0;
  synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE_hg[0] = 0.0;
  synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE_hg[1] = 0.0;
  synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE_j = 0.0;

  /* Start for Constant: '<S4>/Constant2' */
  synrmmodel_mt_RTW_MotorPara1_Constant2_1 = synrmmodel_mt_RTW_P.Ld;

  /* Start for Constant: '<S4>/Constant1' */
  synrmmodel_mt_RTW_MotorPara1_Constant1_1 = synrmmodel_mt_RTW_P.Lq;

  /* Start for Constant: '<S4>/Constant' */
  synrmmodel_mt_RTW_MotorPara1_Constant_1 = synrmmodel_mt_RTW_P.R;

  /* Start for Constant: '<S2>/Constant' */
  synrmmodel_mt_RTW_IPMSM1_Constant_1 = synrmmodel_mt_RTW_P.load;

  /* Start for DiscretePulseGenerator: '<S8>/PulseGenerator3' */
  synrmmodel_mt_RTW_DWork.clockTickCounter = 0;

  /* Start for DiscretePulseGenerator: '<S8>/Reffreq' */
  synrmmodel_mt_RTW_DWork.clockTickCounter_d = 0;

  /* Start for DiscretePulseGenerator: '<S8>/ModeSelector' */
  synrmmodel_mt_RTW_DWork.clockTickCounter_m = 0;
  synrmmodel_mt_RT_PrevZCSigState.calc_rpm_Trig_ZCE = UNINITIALIZED_ZCSIG;
  synrmmodel_mt_RT_PrevZCSigState.Hall_sensor_Event_driven_Trig_Z = POS_ZCSIG;
  synrmmodel_mt_RT_PrevZCSigState.PIDcontroller_changedperiodic_T = POS_ZCSIG;
  synrmmodel_mt_RT_PrevZCSigState.Maketargetvalue1_Trig_ZCE =
    UNINITIALIZED_ZCSIG;

  /* InitializeConditions for UnitDelay: '<Root>/UnitDelay' */
  synrmmodel_mt_RTW_DWork.UnitDelay_DSTATE =
    synrmmodel_mt_RTW_P.UnitDelay_InitialCondition_m;

  /* InitializeConditions for UnitDelay: '<Root>/UnitDelay2' */
  synrmmodel_mt_RTW_DWork.UnitDelay2_DSTATE =
    synrmmodel_mt_RTW_P.UnitDelay2_InitialCondition;

  /* S-Function Block: <S2>/S_FunctionBuilder */
  {
    real_T initVector[4] = { 0, 0, 0, 0 };

    {
      int_T i1;
      real_T *dw_DSTATE = &synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE[0];
      for (i1=0; i1 < 4; i1++) {
        dw_DSTATE[i1] = initVector[i1];
      }
    }
  }

  /* InitializeConditions for Triggered SubSystem: '<S8>/calc_rpm' */
  /* InitializeConditions for UnitDelay: '<S26>/UnitDelay' */
  synrmmodel_mt_RTW_DWork.UnitDelay_DSTATE_d =
    synrmmodel_mt_RTW_P.UnitDelay_InitialCondition;

  /* End of InitializeConditions for SubSystem: '<S8>/calc_rpm' */

  /* InitializeConditions for UnitDelay: '<S8>/UnitDelay' */
  synrmmodel_mt_RTW_DWork.UnitDelay_DSTATE_n[0] =
    synrmmodel_mt_RTW_P.UnitDelay_InitialCondition_b;
  synrmmodel_mt_RTW_DWork.UnitDelay_DSTATE_n[1] =
    synrmmodel_mt_RTW_P.UnitDelay_InitialCondition_b;

  /* InitializeConditions for UnitDelay: '<S8>/UnitDelay1' */
  synrmmodel_mt_RTW_DWork.UnitDelay1_DSTATE =
    synrmmodel_mt_RTW_P.UnitDelay1_InitialCondition;

  /* InitializeConditions for UnitDelay: '<S11>/UnitDelay1' */
  synrmmodel_mt_RTW_DWork.UnitDelay1_DSTATE_j = -synrmmodel_mt_RTW_P.tob;

  /* InitializeConditions for Atomic SubSystem: '<S11>/Subsystem' */

  /* S-Function Block: <S12>/S_FunctionBuilder */
  {
    real_T initVector[2] = { 0, 0 };

    {
      int_T i1;
      real_T *dw_DSTATE = &synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE_hg[0];
      for (i1=0; i1 < 2; i1++) {
        dw_DSTATE[i1] = initVector[i1];
      }
    }
  }

  /* End of InitializeConditions for SubSystem: '<S11>/Subsystem' */

  /* S-Function Block: <S11>/S_FunctionBuilder1 */
  {
    real_T initVector[4] = { 0, 0, 0, 0 };

    {
      int_T i1;
      real_T *dw_DSTATE = &synrmmodel_mt_RTW_DWork.S_FunctionBuilder1_DSTATE[0];
      for (i1=0; i1 < 4; i1++) {
        dw_DSTATE[i1] = initVector[i1];
      }
    }
  }

  /* InitializeConditions for UnitDelay: '<S6>/UnitDelay1' */
  synrmmodel_mt_RTW_DWork.UnitDelay1_DSTATE_i = -synrmmodel_mt_RTW_P.tcc;

  /* InitializeConditions for Triggered SubSystem: '<Root>/Maketargetvalue1' */

  /* S-Function Block: <S10>/S_FunctionBuilder */
  {
    real_T initVector[1] = { 0 };

    {
      int_T i1;
      for (i1=0; i1 < 1; i1++) {
        synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE_j = initVector[0];
      }
    }
  }

  /* End of InitializeConditions for SubSystem: '<Root>/Maketargetvalue1' */

  /* InitializeConditions for UnitDelay: '<S8>/feedback_4' */
  synrmmodel_mt_RTW_DWork.feedback_4_DSTATE =
    synrmmodel_mt_RTW_P.feedback_4_InitialCondition;

  /* InitializeConditions for Triggered SubSystem: '<Root>/PIDcontroller_changedperiodic' */

  /* S-Function Block: <S14>/S_FunctionBuilder */
  {
    real_T initVector[2] = { 0, 0 };

    {
      int_T i1;
      real_T *dw_DSTATE = &synrmmodel_mt_RTW_DWork.S_FunctionBuilder_DSTATE_h[0];
      for (i1=0; i1 < 2; i1++) {
        dw_DSTATE[i1] = initVector[i1];
      }
    }
  }

  /* End of InitializeConditions for SubSystem: '<Root>/PIDcontroller_changedperiodic' */

  /* InitializeConditions for UnitDelay: '<S8>/feedback_2' */
  memcpy(&synrmmodel_mt_RTW_DWork.feedback_2_DSTATE[0],
         &synrmmodel_mt_RTW_P.feedback_2_InitialCondition[0], sizeof(real_T) <<
         6U);

  /* InitializeConditions for UnitDelay: '<S8>/feedback_3' */
  synrmmodel_mt_RTW_DWork.feedback_3_DSTATE =
    synrmmodel_mt_RTW_P.feedback_3_InitialCondition;

  /* InitializeConditions for UnitDelay: '<S8>/feedback_1' */
  synrmmodel_mt_RTW_DWork.feedback_1_DSTATE =
    synrmmodel_mt_RTW_P.feedback_1_InitialCondition;

  /* InitializeConditions for UnitDelay: '<S29>/UnitDelay1' */
  synrmmodel_mt_RTW_DWork.UnitDelay1_DSTATE_im =
    synrmmodel_mt_RTW_P.UnitDelay1_InitialCondition_e;

  /* InitializeConditions for UnitDelay: '<S29>/UnitDelay2' */
  synrmmodel_mt_RTW_DWork.UnitDelay2_DSTATE_e =
    synrmmodel_mt_RTW_P.UnitDelay2_InitialCondition_j;
}

/* Model terminate function */
void synrmmodel_mt_RTW_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
