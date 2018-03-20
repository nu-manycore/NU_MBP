/*
 * File: testsys.c
 *
 * Code generated for Simulink model 'testsys'.
 *
 * Model version                  : 1.103
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Fri Nov 29 16:06:19 2013
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "testsys.h"
#include "testsys_private.h"

/* Exported block signals */
real_T line1;                          /* '<Root>/x' */
real_T line6;                          /* '<S1>/Constant' */
real_T line7;                          /* '<S1>/Sum' */
real_T line3;                          /* '<Root>/Constant' */
real_T line4;                          /* '<Root>/Product' */

/* Real-time model */
RT_MODEL_testsys testsys_M_;
RT_MODEL_testsys *const testsys_M = &testsys_M_;

/* Model step function */
void testsys_step(void)
{
  /* Constant: '<S1>/Constant' */
  line6 = testsys_P.Constant_Value;

  /* Sum: '<S1>/Sum' incorporates:
   *  Inport: '<Root>/x'
   */
  line7 = line1 + line6;

  /* Constant: '<Root>/Constant' */
  line3 = testsys_P.Constant_Value_o;

  /* Product: '<Root>/Product' */
  line4 = line7 * line3;
}

/* Model initialize function */
void testsys_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(testsys_M, (NULL));

  /* block I/O */

  /* exported global signals */
  line6 = 0.0;
  line7 = 0.0;
  line3 = 0.0;
  line4 = 0.0;

  /* external inputs */
  line1 = 0.0;
}

/* Model terminate function */
void testsys_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
