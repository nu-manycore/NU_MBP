/*
 * File: mfunction2.c
 *
 * Code generated for Simulink model 'mfunction2'.
 *
 * Model version                  : 1.19
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Tue Jul 01 15:16:01 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "mfunction2.h"
#include "mfunction2_private.h"

/* Exported block signals */
real_T mfunction2_y2_1;                /* '<Root>/y2' */
real_T mfunction2_k1_1;                /* '<Root>/k1' */
real_T mfunction2_mul1_1;              /* '<Root>/mul1' */
real_T mfunction2_MATLABFunction1_SFunction_y;/* '<Root>/MATLAB Function1' */

/* Real-time model */
RT_MODEL_mfunction2 mfunction2_M_;
RT_MODEL_mfunction2 *const mfunction2_M = &mfunction2_M_;

/* Model step function */
void mfunction2_step(void)
{
  /* Constant: '<Root>/k1' */
  mfunction2_k1_1 = mfunction2_P.k1_Value;

  /* Product: '<Root>/mul1' incorporates:
   *  Inport: '<Root>/y2'
   */
  mfunction2_mul1_1 = mfunction2_k1_1 * mfunction2_y2_1;

  /* MATLAB Function: '<Root>/MATLAB Function1' */
  /* MATLAB Function 'MATLAB Function1': '<S1>:1' */
  if (mfunction2_mul1_1 < 0.5) {
    /* '<S1>:1:3' */
    /* y=1/sqrt(2); */
    /* '<S1>:1:5' */
    mfunction2_MATLABFunction1_SFunction_y = 1.0;
  } else if (mfunction2_mul1_1 <= 1.0) {
    /* '<S1>:1:6' */
    /* y=sqrt(2*x*(1-x)); */
    /* '<S1>:1:8' */
    mfunction2_MATLABFunction1_SFunction_y = 2.0;
  } else {
    /* '<S1>:1:10' */
    mfunction2_MATLABFunction1_SFunction_y = 0.0;
  }

  /* End of MATLAB Function: '<Root>/MATLAB Function1' */
}

/* Model initialize function */
void mfunction2_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(mfunction2_M, (NULL));

  /* block I/O */

  /* exported global signals */
  mfunction2_k1_1 = 0.0;
  mfunction2_mul1_1 = 0.0;
  mfunction2_MATLABFunction1_SFunction_y = 0.0;

  /* external inputs */
  mfunction2_y2_1 = 0.0;
}

/* Model terminate function */
void mfunction2_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
