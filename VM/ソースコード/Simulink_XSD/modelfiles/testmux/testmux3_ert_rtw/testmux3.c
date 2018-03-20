/*
 * File: testmux3.c
 *
 * Code generated for Simulink model 'testmux3'.
 *
 * Model version                  : 1.107
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Fri Nov 29 16:17:48 2013
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "testmux3.h"
#include "testmux3_private.h"

/* Exported block signals */
real_T line1[2];                       /* '<Root>/x' */
real_T line4;                          /* '<Root>/Constant1' */
real_T line6;                          /* '<Root>/Product' */
real_T line5;                          /* '<Root>/Constant' */
real_T line7;                          /* '<Root>/Sum' */
real_T line9;                          /* '<Root>/Constant2' */
real_T line10[2];                      /* '<Root>/Sum1' */

/* Real-time model */
RT_MODEL_testmux3 testmux3_M_;
RT_MODEL_testmux3 *const testmux3_M = &testmux3_M_;

/* Model step function */
void testmux3_step(void)
{
  /* Constant: '<Root>/Constant1' */
  line4 = testmux3_P.Constant1_Value;

  /* Product: '<Root>/Product' incorporates:
   *  Inport: '<Root>/x'
   */
  line6 = line4 * line1[0];

  /* Constant: '<Root>/Constant' */
  line5 = testmux3_P.Constant_Value;

  /* Sum: '<Root>/Sum' incorporates:
   *  Inport: '<Root>/x'
   */
  line7 = line1[1] + line5;

  /* Constant: '<Root>/Constant2' */
  line9 = testmux3_P.Constant2_Value;

  /* Sum: '<Root>/Sum1' */
  line10[0] = line6 + line9;
  line10[1] = line7 + line9;
}

/* Model initialize function */
void testmux3_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(testmux3_M, (NULL));

  /* block I/O */

  /* exported global signals */
  line4 = 0.0;
  line6 = 0.0;
  line5 = 0.0;
  line7 = 0.0;
  line9 = 0.0;
  line10[0] = 0.0;
  line10[1] = 0.0;

  /* external inputs */
  (void) memset(line1, 0,
                2U*sizeof(real_T));
}

/* Model terminate function */
void testmux3_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
