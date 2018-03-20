/*
 * File: pid_controller1.c
 *
 * Code generated for Simulink model 'pid_controller1'.
 *
 * Model version                  : 1.59
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Fri Nov 29 15:50:59 2013
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "pid_controller1.h"
#include "pid_controller1_private.h"

/* Exported block signals */
real32_T pid_controller1_error_1;      /* '<Root>/error' */
real32_T pid_controller1_pid_Constant_1;/* '<S1>/Constant' */
real32_T pid_controller1_pid_Gain_1;   /* '<S1>/Gain' */
real32_T pid_controller1_pid_UnitDelay_1;/* '<S1>/Unit Delay' */
real32_T pid_controller1_pid_Gain1_1;  /* '<S1>/Gain1' */
real32_T pid_controller1_pid_UnitDelay1_1;/* '<S1>/Unit Delay1' */
real32_T pid_controller1_pid_Sum1_1;   /* '<S1>/Sum1' */
real32_T pid_controller1_pid_Gain2_1;  /* '<S1>/Gain2' */
real32_T pid_controller1_pid_Gain3_1;  /* '<S1>/Gain3' */
real32_T pid_controller1_pid_Product_1;/* '<S1>/Product' */
real32_T pid_controller1_pid_Sum2_1;   /* '<S1>/Sum2' */
real32_T pid_controller1_pid_Saturation_1;/* '<S1>/Saturation' */
real32_T pid_controller1_pid_Sum_1;    /* '<S1>/Sum' */

/* Block states (auto storage) */
D_Work_pid_controller1 pid_controller1_DWork;

/* Real-time model */
RT_MODEL_pid_controller1 pid_controller1_M_;
RT_MODEL_pid_controller1 *const pid_controller1_M = &pid_controller1_M_;

/* Model step function */
void pid_controller1_step(void)
{
  /* Outputs for Atomic SubSystem: '<Root>/pid' */
  /* Constant: '<S1>/Constant' */
  pid_controller1_pid_Constant_1 = pid_controller1_P.Constant_Value;

  /* Gain: '<S1>/Gain' incorporates:
   *  Inport: '<Root>/error'
   */
  pid_controller1_pid_Gain_1 = pid_controller1_P.Gain_Gain *
    pid_controller1_error_1;

  /* UnitDelay: '<S1>/Unit Delay' */
  pid_controller1_pid_UnitDelay_1 = pid_controller1_DWork.UnitDelay_DSTATE;

  /* Gain: '<S1>/Gain1' */
  pid_controller1_pid_Gain1_1 = pid_controller1_P.Gain1_Gain *
    pid_controller1_pid_UnitDelay_1;

  /* UnitDelay: '<S1>/Unit Delay1' */
  pid_controller1_pid_UnitDelay1_1 = pid_controller1_DWork.UnitDelay1_DSTATE;

  /* Sum: '<S1>/Sum1' incorporates:
   *  Inport: '<Root>/error'
   */
  pid_controller1_pid_Sum1_1 = pid_controller1_error_1 -
    pid_controller1_pid_UnitDelay1_1;

  /* Gain: '<S1>/Gain2' */
  pid_controller1_pid_Gain2_1 = pid_controller1_P.Gain2_Gain *
    pid_controller1_pid_Sum1_1;

  /* Gain: '<S1>/Gain3' */
  pid_controller1_pid_Gain3_1 = pid_controller1_P.Gain3_Gain *
    pid_controller1_pid_Gain2_1;

  /* Product: '<S1>/Product' */
  pid_controller1_pid_Product_1 = pid_controller1_pid_Gain1_1 /
    pid_controller1_pid_Constant_1;

  /* Sum: '<S1>/Sum2' */
  pid_controller1_pid_Sum2_1 = (pid_controller1_pid_Gain_1 +
    pid_controller1_pid_Product_1) + pid_controller1_pid_Gain3_1;

  /* Saturate: '<S1>/Saturation' */
  if (pid_controller1_pid_Sum2_1 >= pid_controller1_P.Saturation_UpperSat) {
    pid_controller1_pid_Saturation_1 = pid_controller1_P.Saturation_UpperSat;
  } else if (pid_controller1_pid_Sum2_1 <= pid_controller1_P.Saturation_LowerSat)
  {
    pid_controller1_pid_Saturation_1 = pid_controller1_P.Saturation_LowerSat;
  } else {
    pid_controller1_pid_Saturation_1 = pid_controller1_pid_Sum2_1;
  }

  /* End of Saturate: '<S1>/Saturation' */

  /* Sum: '<S1>/Sum' incorporates:
   *  Inport: '<Root>/error'
   */
  pid_controller1_pid_Sum_1 = pid_controller1_error_1 +
    pid_controller1_pid_UnitDelay_1;

  /* Update for UnitDelay: '<S1>/Unit Delay' */
  pid_controller1_DWork.UnitDelay_DSTATE = pid_controller1_pid_Sum_1;

  /* Update for UnitDelay: '<S1>/Unit Delay1' incorporates:
   *  Inport: '<Root>/error'
   */
  pid_controller1_DWork.UnitDelay1_DSTATE = pid_controller1_error_1;

  /* End of Outputs for SubSystem: '<Root>/pid' */
}

/* Model initialize function */
void pid_controller1_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(pid_controller1_M, (NULL));

  /* block I/O */

  /* exported global signals */
  pid_controller1_pid_Constant_1 = 0.0F;
  pid_controller1_pid_Gain_1 = 0.0F;
  pid_controller1_pid_UnitDelay_1 = 0.0F;
  pid_controller1_pid_Gain1_1 = 0.0F;
  pid_controller1_pid_UnitDelay1_1 = 0.0F;
  pid_controller1_pid_Sum1_1 = 0.0F;
  pid_controller1_pid_Gain2_1 = 0.0F;
  pid_controller1_pid_Gain3_1 = 0.0F;
  pid_controller1_pid_Product_1 = 0.0F;
  pid_controller1_pid_Sum2_1 = 0.0F;
  pid_controller1_pid_Saturation_1 = 0.0F;
  pid_controller1_pid_Sum_1 = 0.0F;

  /* states (dwork) */
  pid_controller1_DWork.UnitDelay_DSTATE = 0.0F;
  pid_controller1_DWork.UnitDelay1_DSTATE = 0.0F;

  /* external inputs */
  pid_controller1_error_1 = 0.0F;

  /* InitializeConditions for Atomic SubSystem: '<Root>/pid' */
  /* InitializeConditions for UnitDelay: '<S1>/Unit Delay' */
  pid_controller1_DWork.UnitDelay_DSTATE = pid_controller1_P.UnitDelay_X0;

  /* InitializeConditions for UnitDelay: '<S1>/Unit Delay1' */
  pid_controller1_DWork.UnitDelay1_DSTATE = pid_controller1_P.UnitDelay1_X0;

  /* End of InitializeConditions for SubSystem: '<Root>/pid' */
}

/* Model terminate function */
void pid_controller1_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
