/*
 * File: vipcellcounting_win_private.h
 *
 * Code generated for Simulink model 'vipcellcounting_win'.
 *
 * Model version                  : 1.228
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Fri Nov 29 16:14:39 2013
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Emulation hardware selection:
 *    Differs from embedded hardware (Generic->MATLAB Host Computer)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_vipcellcounting_win_private_h_
#define RTW_HEADER_vipcellcounting_win_private_h_
#include "rtwtypes.h"
#ifndef __RTWTYPES_H__
#error This file requires rtwtypes.h to be included
#else
#ifdef TMWTYPES_PREVIOUSLY_INCLUDED
#error This file requires rtwtypes.h to be included before tmwtypes.h
#else

/* Check for inclusion of an incorrect version of rtwtypes.h */
#ifndef RTWTYPES_ID_C08S16I32L32N32F1
#error This code was generated with a different "rtwtypes.h" than the file included
#endif                                 /* RTWTYPES_ID_C08S16I32L32N32F1 */
#endif                                 /* TMWTYPES_PREVIOUSLY_INCLUDED */
#endif                                 /* __RTWTYPES_H__ */

extern int32_T div_s32_floor(int32_T numerator, int32_T denominator);
extern void vipcellcounting_win_Subsystem(void);
extern void vipcellcountin_Subsystem1_Start(void);
extern void vipcellcounting_win_Subsystem1(void);
extern void vipcellcounting_win_Subsystem_c(void);

#endif                                 /* RTW_HEADER_vipcellcounting_win_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
