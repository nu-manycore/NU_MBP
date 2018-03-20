/*
 * File: testmux3.h
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

#ifndef RTW_HEADER_testmux3_h_
#define RTW_HEADER_testmux3_h_
#ifndef testmux3_COMMON_INCLUDES_
# define testmux3_COMMON_INCLUDES_
#include <stddef.h>
#include <string.h>
#include "rtwtypes.h"
#endif                                 /* testmux3_COMMON_INCLUDES_ */

#include "testmux3_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Parameters (auto storage) */
struct Parameters_testmux3_ {
  real_T Constant1_Value;              /* Expression: 2
                                        * Referenced by: '<Root>/Constant1'
                                        */
  real_T Constant_Value;               /* Expression: 1
                                        * Referenced by: '<Root>/Constant'
                                        */
  real_T Constant2_Value;              /* Expression: 3
                                        * Referenced by: '<Root>/Constant2'
                                        */
};

/* Real-time Model Data Structure */
struct RT_MODEL_testmux3 {
  const char_T * volatile errorStatus;
};

/* Block parameters (auto storage) */
extern Parameters_testmux3 testmux3_P;

/*
 * Exported Global Signals
 *
 * Note: Exported global signals are block signals with an exported global
 * storage class designation.  Code generation will declare the memory for
 * these signals and export their symbols.
 *
 */
extern real_T line1[2];                /* '<Root>/x' */
extern real_T line4;                   /* '<Root>/Constant1' */
extern real_T line6;                   /* '<Root>/Product' */
extern real_T line5;                   /* '<Root>/Constant' */
extern real_T line7;                   /* '<Root>/Sum' */
extern real_T line9;                   /* '<Root>/Constant2' */
extern real_T line10[2];               /* '<Root>/Sum1' */

/* Model entry point functions */
extern void testmux3_initialize(void);
extern void testmux3_step(void);
extern void testmux3_terminate(void);

/* Real-time Model object */
extern struct RT_MODEL_testmux3 *const testmux3_M;

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
 * '<Root>' : 'testmux3'
 */
#endif                                 /* RTW_HEADER_testmux3_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
