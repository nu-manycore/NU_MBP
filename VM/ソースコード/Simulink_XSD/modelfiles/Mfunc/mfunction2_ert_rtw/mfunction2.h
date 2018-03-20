/*
 * File: mfunction2.h
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

#ifndef RTW_HEADER_mfunction2_h_
#define RTW_HEADER_mfunction2_h_
#ifndef mfunction2_COMMON_INCLUDES_
# define mfunction2_COMMON_INCLUDES_
#include <stddef.h>
#include "rtwtypes.h"
#endif                                 /* mfunction2_COMMON_INCLUDES_ */

#include "mfunction2_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Parameters (auto storage) */
struct Parameters_mfunction2_ {
  real_T k1_Value;                     /* Expression: 2
                                        * Referenced by: '<Root>/k1'
                                        */
};

/* Real-time Model Data Structure */
struct RT_MODEL_mfunction2 {
  const char_T * volatile errorStatus;
};

/* Block parameters (auto storage) */
extern Parameters_mfunction2 mfunction2_P;

/*
 * Exported Global Signals
 *
 * Note: Exported global signals are block signals with an exported global
 * storage class designation.  Code generation will declare the memory for
 * these signals and export their symbols.
 *
 */
extern real_T mfunction2_y2_1;         /* '<Root>/y2' */
extern real_T mfunction2_k1_1;         /* '<Root>/k1' */
extern real_T mfunction2_mul1_1;       /* '<Root>/mul1' */
extern real_T mfunction2_MATLABFunction1_SFunction_y;/* '<Root>/MATLAB Function1' */

/* Model entry point functions */
extern void mfunction2_initialize(void);
extern void mfunction2_step(void);
extern void mfunction2_terminate(void);

/* Real-time Model object */
extern struct RT_MODEL_mfunction2 *const mfunction2_M;

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
 * '<Root>' : 'mfunction2'
 * '<S1>'   : 'mfunction2/MATLAB Function1'
 */
#endif                                 /* RTW_HEADER_mfunction2_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
