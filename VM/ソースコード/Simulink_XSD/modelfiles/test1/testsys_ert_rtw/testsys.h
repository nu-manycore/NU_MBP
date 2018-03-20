/*
 * File: testsys.h
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

#ifndef RTW_HEADER_testsys_h_
#define RTW_HEADER_testsys_h_
#ifndef testsys_COMMON_INCLUDES_
# define testsys_COMMON_INCLUDES_
#include <stddef.h>
#include "rtwtypes.h"
#endif                                 /* testsys_COMMON_INCLUDES_ */

#include "testsys_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Parameters (auto storage) */
struct Parameters_testsys_ {
  real_T Constant_Value;               /* Expression: 1
                                        * Referenced by: '<S1>/Constant'
                                        */
  real_T Constant_Value_o;             /* Expression: 2
                                        * Referenced by: '<Root>/Constant'
                                        */
};

/* Real-time Model Data Structure */
struct RT_MODEL_testsys {
  const char_T * volatile errorStatus;
};

/* Block parameters (auto storage) */
extern Parameters_testsys testsys_P;

/*
 * Exported Global Signals
 *
 * Note: Exported global signals are block signals with an exported global
 * storage class designation.  Code generation will declare the memory for
 * these signals and export their symbols.
 *
 */
extern real_T line1;                   /* '<Root>/x' */
extern real_T line6;                   /* '<S1>/Constant' */
extern real_T line7;                   /* '<S1>/Sum' */
extern real_T line3;                   /* '<Root>/Constant' */
extern real_T line4;                   /* '<Root>/Product' */

/* Model entry point functions */
extern void testsys_initialize(void);
extern void testsys_step(void);
extern void testsys_terminate(void);

/* Real-time Model object */
extern struct RT_MODEL_testsys *const testsys_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'testsys'
 * '<S1>'   : 'testsys/Subsystem'
 */
#endif                                 /* RTW_HEADER_testsys_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
