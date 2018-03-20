/*
 * File: pid_controller1_data.c
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

/* Block parameters (auto storage) */
Parameters_pid_controller1 pid_controller1_P = {
  100.0F,                              /* Expression: Fs
                                        * Referenced by: '<S1>/Constant'
                                        */
  1.53F,                               /* Expression: Kp
                                        * Referenced by: '<S1>/Gain'
                                        */
  0.0F,                                /* Computed Parameter: UnitDelay_X0
                                        * Referenced by: '<S1>/Unit Delay'
                                        */
  0.387F,                              /* Expression: Ki
                                        * Referenced by: '<S1>/Gain1'
                                        */
  0.0F,                                /* Computed Parameter: UnitDelay1_X0
                                        * Referenced by: '<S1>/Unit Delay1'
                                        */
  0.0081F,                             /* Expression: Kd
                                        * Referenced by: '<S1>/Gain2'
                                        */
  100.0F,                              /* Expression: Fs
                                        * Referenced by: '<S1>/Gain3'
                                        */
  5.0F,                                /* Computed Parameter: Saturation_UpperSat
                                        * Referenced by: '<S1>/Saturation'
                                        */
  -5.0F                                /* Computed Parameter: Saturation_LowerSat
                                        * Referenced by: '<S1>/Saturation'
                                        */
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
