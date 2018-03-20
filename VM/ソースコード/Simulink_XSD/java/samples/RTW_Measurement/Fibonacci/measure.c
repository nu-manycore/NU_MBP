/*
 * File: Fibonacci.c
 *
 * Code generated for Simulink model 'Fibonacci'.
 *
 * Model version                  : 1.2
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Thu May 21 11:34:42 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Fibonacci.h"
#include "Fibonacci_private.h"

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
real_T Fibonacci_Fn_1;                 /* '<Root>/Fn' */
real_T Fibonacci_Fn_1_1;               /* '<Root>/Fn_1' */
real_T Fibonacci_Saturation_1;         /* '<Root>/Saturation' */
real_T Fibonacci_Divide_1;             /* '<Root>/Divide' */
real_T Fibonacci_Golden_ratio_1;       /* '<Root>/Golden_ratio' */
real_T Fibonacci_Subtract_1;           /* '<Root>/Subtract' */
real_T Fibonacci_Abs_1;                /* '<Root>/Abs' */
real_T Fibonacci_Gain_1;               /* '<Root>/Gain' */
real_T Fibonacci_Add_1;                /* '<Root>/Add' */

/* Block states (auto storage) */
DW_Fibonacci_T Fibonacci_DW;

/* Real-time model */
RT_MODEL_Fibonacci_T Fibonacci_M_;
RT_MODEL_Fibonacci_T *const Fibonacci_M = &Fibonacci_M_;

/* Model step function */
void Fibonacci_step(void)
{
  real_T u0;
  real_T u1;
  real_T u2;

MEASUREMENT_STEP_BEGIN();
MEASUREMENT_START(FIBONACCI_FN,0,0,FIBONACCI_FN_PARAM);
  /* UnitDelay: '<Root>/Fn' */
  Fibonacci_Fn_1 = Fibonacci_DW.Fn_DSTATE;
MEASUREMENT_STOP(FIBONACCI_FN,0,0,FIBONACCI_FN_PARAM);

MEASUREMENT_START(FIBONACCI_FN_1,0,0,FIBONACCI_FN_1_PARAM);
  /* UnitDelay: '<Root>/Fn_1' */
  Fibonacci_Fn_1_1 = Fibonacci_DW.Fn_1_DSTATE;
MEASUREMENT_STOP(FIBONACCI_FN_1,0,0,FIBONACCI_FN_1_PARAM);

MEASUREMENT_START(FIBONACCI_SATURATION,0,0,FIBONACCI_SATURATION_PARAM);
  /* Saturate: '<Root>/Saturation' */
  u0 = Fibonacci_Fn_1_1;
  u1 = Fibonacci_P.Saturation_LowerSat;
  u2 = Fibonacci_P.Saturation_UpperSat;
  if (u0 > u2) {
    Fibonacci_Saturation_1 = u2;
  } else if (u0 < u1) {
    Fibonacci_Saturation_1 = u1;
  } else {
    Fibonacci_Saturation_1 = u0;
  }
MEASUREMENT_STOP(FIBONACCI_SATURATION,0,0,FIBONACCI_SATURATION_PARAM);

  /* End of Saturate: '<Root>/Saturation' */

MEASUREMENT_START(FIBONACCI_DIVIDE,0,0,FIBONACCI_DIVIDE_PARAM);
  /* Product: '<Root>/Divide' */
  Fibonacci_Divide_1 = Fibonacci_Fn_1 / Fibonacci_Saturation_1;
MEASUREMENT_STOP(FIBONACCI_DIVIDE,0,0,FIBONACCI_DIVIDE_PARAM);

MEASUREMENT_START(FIBONACCI_GOLDEN_RATIO,0,0,FIBONACCI_GOLDEN_RATIO_PARAM);
  /* Constant: '<Root>/Golden_ratio' */
  Fibonacci_Golden_ratio_1 = Fibonacci_P.Golden_ratio_Value;
MEASUREMENT_STOP(FIBONACCI_GOLDEN_RATIO,0,0,FIBONACCI_GOLDEN_RATIO_PARAM);

MEASUREMENT_START(FIBONACCI_SUBTRACT,0,0,FIBONACCI_SUBTRACT_PARAM);
  /* Sum: '<Root>/Subtract' */
  Fibonacci_Subtract_1 = Fibonacci_Divide_1 - Fibonacci_Golden_ratio_1;
MEASUREMENT_STOP(FIBONACCI_SUBTRACT,0,0,FIBONACCI_SUBTRACT_PARAM);

MEASUREMENT_START(FIBONACCI_ABS,0,0,FIBONACCI_ABS_PARAM);
  /* Abs: '<Root>/Abs' */
  Fibonacci_Abs_1 = fabs(Fibonacci_Subtract_1);
MEASUREMENT_STOP(FIBONACCI_ABS,0,0,FIBONACCI_ABS_PARAM);

MEASUREMENT_START(FIBONACCI_GAIN,0,0,FIBONACCI_GAIN_PARAM);
  /* Gain: '<Root>/Gain' */
  Fibonacci_Gain_1 = Fibonacci_P.Gain_Gain * Fibonacci_Abs_1;
MEASUREMENT_STOP(FIBONACCI_GAIN,0,0,FIBONACCI_GAIN_PARAM);

MEASUREMENT_START(FIBONACCI_ADD,0,0,FIBONACCI_ADD_PARAM);
  /* Sum: '<Root>/Add' */
  Fibonacci_Add_1 = Fibonacci_Fn_1 + Fibonacci_Fn_1_1;
MEASUREMENT_STOP(FIBONACCI_ADD,0,0,FIBONACCI_ADD_PARAM);

MEASUREMENT_START(FIBONACCI_FN,1,0,FIBONACCI_FN_PARAM);
  /* Update for UnitDelay: '<Root>/Fn' */
  Fibonacci_DW.Fn_DSTATE = Fibonacci_Add_1;
MEASUREMENT_STOP(FIBONACCI_FN,1,0,FIBONACCI_FN_PARAM);

MEASUREMENT_START(FIBONACCI_FN_1,1,0,FIBONACCI_FN_1_PARAM);
  /* Update for UnitDelay: '<Root>/Fn_1' */
  Fibonacci_DW.Fn_1_DSTATE = Fibonacci_Fn_1;
MEASUREMENT_STOP(FIBONACCI_FN_1,1,0,FIBONACCI_FN_1_PARAM);
MEASUREMENT_STEP_END();
}

/* Model initialize function */
void Fibonacci_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  Fibonacci_P.Saturation_UpperSat = rtInf;

  /* initialize error status */
  rtmSetErrorStatus(Fibonacci_M, (NULL));

  /* block I/O */

  /* exported global signals */
  Fibonacci_Fn_1 = 0.0;
  Fibonacci_Fn_1_1 = 0.0;
  Fibonacci_Saturation_1 = 0.0;
  Fibonacci_Divide_1 = 0.0;
  Fibonacci_Golden_ratio_1 = 0.0;
  Fibonacci_Subtract_1 = 0.0;
  Fibonacci_Abs_1 = 0.0;
  Fibonacci_Gain_1 = 0.0;
  Fibonacci_Add_1 = 0.0;

  /* states (dwork) */
  (void) memset((void *)&Fibonacci_DW, 0,
                sizeof(DW_Fibonacci_T));

  /* InitializeConditions for UnitDelay: '<Root>/Fn' */
  Fibonacci_DW.Fn_DSTATE = Fibonacci_P.Fn_InitialCondition;

  /* InitializeConditions for UnitDelay: '<Root>/Fn_1' */
  Fibonacci_DW.Fn_1_DSTATE = Fibonacci_P.Fn_1_InitialCondition;
}

/* Model terminate function */
void Fibonacci_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
