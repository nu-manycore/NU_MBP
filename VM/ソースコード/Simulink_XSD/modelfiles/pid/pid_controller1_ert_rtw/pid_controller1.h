/*
 * File: pid_controller1.h
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

#ifndef RTW_HEADER_pid_controller1_h_
#define RTW_HEADER_pid_controller1_h_
#ifndef pid_controller1_COMMON_INCLUDES_
# define pid_controller1_COMMON_INCLUDES_
#include <stddef.h>
#include "rtwtypes.h"
#endif                                 /* pid_controller1_COMMON_INCLUDES_ */

#include "pid_controller1_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real32_T UnitDelay_DSTATE;           /* '<S1>/Unit Delay' */
  real32_T UnitDelay1_DSTATE;          /* '<S1>/Unit Delay1' */
} D_Work_pid_controller1;

/* Parameters (auto storage) */
struct Parameters_pid_controller1_ {
  real32_T Constant_Value;             /* Expression: Fs
                                        * Referenced by: '<S1>/Constant'
                                        */
  real32_T Gain_Gain;                  /* Expression: Kp
                                        * Referenced by: '<S1>/Gain'
                                        */
  real32_T UnitDelay_X0;               /* Computed Parameter: UnitDelay_X0
                                        * Referenced by: '<S1>/Unit Delay'
                                        */
  real32_T Gain1_Gain;                 /* Expression: Ki
                                        * Referenced by: '<S1>/Gain1'
                                        */
  real32_T UnitDelay1_X0;              /* Computed Parameter: UnitDelay1_X0
                                        * Referenced by: '<S1>/Unit Delay1'
                                        */
  real32_T Gain2_Gain;                 /* Expression: Kd
                                        * Referenced by: '<S1>/Gain2'
                                        */
  real32_T Gain3_Gain;                 /* Expression: Fs
                                        * Referenced by: '<S1>/Gain3'
                                        */
  real32_T Saturation_UpperSat;        /* Computed Parameter: Saturation_UpperSat
                                        * Referenced by: '<S1>/Saturation'
                                        */
  real32_T Saturation_LowerSat;        /* Computed Parameter: Saturation_LowerSat
                                        * Referenced by: '<S1>/Saturation'
                                        */
};

/* Real-time Model Data Structure */
struct RT_MODEL_pid_controller1 {
  const char_T * volatile errorStatus;
};

/* Block parameters (auto storage) */
extern Parameters_pid_controller1 pid_controller1_P;

/* Block states (auto storage) */
extern D_Work_pid_controller1 pid_controller1_DWork;

/*
 * Exported Global Signals
 *
 * Note: Exported global signals are block signals with an exported global
 * storage class designation.  Code generation will declare the memory for
 * these signals and export their symbols.
 *
 */
extern real32_T pid_controller1_error_1;/* '<Root>/error' */
extern real32_T pid_controller1_pid_Constant_1;/* '<S1>/Constant' */
extern real32_T pid_controller1_pid_Gain_1;/* '<S1>/Gain' */
extern real32_T pid_controller1_pid_UnitDelay_1;/* '<S1>/Unit Delay' */
extern real32_T pid_controller1_pid_Gain1_1;/* '<S1>/Gain1' */
extern real32_T pid_controller1_pid_UnitDelay1_1;/* '<S1>/Unit Delay1' */
extern real32_T pid_controller1_pid_Sum1_1;/* '<S1>/Sum1' */
extern real32_T pid_controller1_pid_Gain2_1;/* '<S1>/Gain2' */
extern real32_T pid_controller1_pid_Gain3_1;/* '<S1>/Gain3' */
extern real32_T pid_controller1_pid_Product_1;/* '<S1>/Product' */
extern real32_T pid_controller1_pid_Sum2_1;/* '<S1>/Sum2' */
extern real32_T pid_controller1_pid_Saturation_1;/* '<S1>/Saturation' */
extern real32_T pid_controller1_pid_Sum_1;/* '<S1>/Sum' */

/* Model entry point functions */
extern void pid_controller1_initialize(void);
extern void pid_controller1_step(void);
extern void pid_controller1_terminate(void);

/* Real-time Model object */
extern struct RT_MODEL_pid_controller1 *const pid_controller1_M;

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
 * '<Root>' : 'pid_controller1'
 * '<S1>'   : 'pid_controller1/pid'
 */
#endif                                 /* RTW_HEADER_pid_controller1_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
