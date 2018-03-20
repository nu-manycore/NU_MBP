/*
 * File: vipcellcounting_win.c
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

#include "vipcellcounting_win.h"
#include "vipcellcounting_win_private.h"

/* Exported block signals */
real_T vipcellcounting_win_DisplayResults_Probe_1[2];/* '<S1>/Probe' */
real_T vipcellcounting_win_DisplayResults_Selector1_1;/* '<S1>/Selector1' */
real32_T vipcellcounting_win_Video_1[228096];/* '<Root>/Video' */
real32_T vipcellcounting_win_DisplayResults_InsertText1_1[228096];/* '<S1>/Insert Text1' */
real32_T vipcellcounting_win_DisplayResults_InsertText2_1[228096];/* '<S1>/Insert Text2' */
real32_T vipcellcounting_win_DisplayResults_ConverttoRGB_MatrixConcatenate_1
  [684288];                            /* '<S4>/Matrix Concatenate' */
real32_T vipcellcounting_win_DisplayResults_DrawMarkers_1[684288];/* '<S1>/Draw Markers' */
real32_T vipcellcounting_win_Subsystem_CountCells_1[3000];/* '<S3>/Count Cells' */
real32_T vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Dilation_1
  [228096];                            /* '<S11>/Dilation' */
real32_T vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Gain_1[228096];/* '<S11>/Gain' */
real32_T vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Subtract_1
  [228096];                            /* '<S11>/Subtract' */
real32_T vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Saturation_1
  [228096];                            /* '<S11>/Saturation' */
real32_T vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Dilation1_1
  [228096];                            /* '<S11>/Dilation1' */
real32_T vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Subtract2_1
  [228096];                            /* '<S11>/Subtract2' */
uint16_T vipcellcounting_win_DisplayResults_DataTypeConversion_1;/* '<S1>/Data Type Conversion' */
uint16_T vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_Output_1;/* '<S6>/Output' */
uint16_T vipcellcounting_win_DisplayResults_Counter1_Bias_1;/* '<S5>/Bias' */
uint16_T
  vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_IncrementRealWorld_FixPtConstant_1;/* '<S7>/FixPt Constant' */
uint16_T
  vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_IncrementRealWorld_FixPtSum1_1;/* '<S7>/FixPt Sum1' */
uint16_T
  vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_WrapToZero_Constant_1;/* '<S8>/Constant' */
uint16_T
  vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_WrapToZero_FixPtSwitch_1;/* '<S8>/FixPt Switch' */
boolean_T vipcellcounting_win_Video_2; /* '<Root>/Video' */
boolean_T vipcellcounting_win_DisplayResults_LogicalOperator_1;/* '<S1>/Logical Operator' */
boolean_T vipcellcounting_win_SegmentCells_Subsystem_Autothreshold_1[228096];/* '<S9>/Autothreshold' */

/* Exported block states */
int32_T vipcellcounting_win_Subsystem_CountCells_1Dim[2];/* '<S3>/Count Cells' */

/* Block signals (auto storage) */
BlockIO_vipcellcounting_win vipcellcounting_win_B;

/* Block states (auto storage) */
D_Work_vipcellcounting_win vipcellcounting_win_DWork;

/* Real-time model */
RT_MODEL_vipcellcounting_win vipcellcounting_win_M_;
RT_MODEL_vipcellcounting_win *const vipcellcounting_win_M =
  &vipcellcounting_win_M_;
int32_T div_s32_floor(int32_T numerator, int32_T denominator)
{
  int32_T quotient;
  uint32_T absNumerator;
  uint32_T absDenominator;
  uint32_T tempAbsQuotient;
  uint32_T quotientNeedsNegation;
  if (denominator == 0) {
    quotient = numerator >= 0 ? MAX_int32_T : MIN_int32_T;

    /* Divide by zero handler */
  } else {
    absNumerator = (uint32_T)(numerator >= 0 ? numerator : -numerator);
    absDenominator = (uint32_T)(denominator >= 0 ? denominator : -denominator);
    quotientNeedsNegation = ((numerator < 0) != (denominator < 0));
    tempAbsQuotient = absDenominator == 0 ? MAX_uint32_T : absNumerator /
      absDenominator;
    if (quotientNeedsNegation) {
      absNumerator %= absDenominator;
      if (absNumerator > 0) {
        tempAbsQuotient++;
      }
    }

    quotient = quotientNeedsNegation ? -(int32_T)tempAbsQuotient : (int32_T)
      tempAbsQuotient;
  }

  return quotient;
}

/* Output and update for atomic system: '<S2>/Subsystem' */
void vipcellcounting_win_Subsystem(void)
{
  int32_T i;
  real32_T Autothreshold_P_DW[256];
  real32_T Autothreshold_MU_DW[256];
  real32_T cnt;
  int32_T idxMaxVal;

  /* S-Function (svipgraythresh): '<S9>/Autothreshold' */
  for (i = 0; i < 256; i++) {
    Autothreshold_P_DW[i] = 0.0F;
    Autothreshold_MU_DW[i] = 0.0F;
  }

  for (i = 0; i < 228096; i++) {
    if (vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Subtract2_1[i] <
        0.0F) {
      Autothreshold_P_DW[0]++;
    } else if
        (vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Subtract2_1[i]
         > 1.0F) {
      Autothreshold_P_DW[255]++;
    } else {
      Autothreshold_P_DW[(uint8_T)
        (vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Subtract2_1[i]
         * 255.0F + 0.5F)]++;
    }
  }

  for (i = 0; i < 256; i++) {
    Autothreshold_P_DW[i] /= 228096.0F;
  }

  Autothreshold_MU_DW[0] = Autothreshold_P_DW[0];
  cnt = 2.0F;
  for (i = 0; i < 255; i++) {
    Autothreshold_MU_DW[i + 1] = Autothreshold_P_DW[i + 1] * cnt +
      Autothreshold_MU_DW[i];
    cnt++;
  }

  for (i = 0; i < 255; i++) {
    Autothreshold_P_DW[i + 1] += Autothreshold_P_DW[i];
  }

  for (i = 0; i < 256; i++) {
    cnt = Autothreshold_P_DW[i] * Autothreshold_MU_DW[255] -
      Autothreshold_MU_DW[i];
    cnt *= cnt;
    Autothreshold_P_DW[i] = cnt / ((1.0F - Autothreshold_P_DW[i]) *
      Autothreshold_P_DW[i]);
  }

  idxMaxVal = 0;
  cnt = 0.0F;
  for (i = 0; i < 256; i++) {
    if (Autothreshold_P_DW[i] > cnt) {
      cnt = Autothreshold_P_DW[i];
      idxMaxVal = i;
    }
  }

  cnt = (real32_T)idxMaxVal * vipcellcounting_win_P.Autothreshold_FACTOR_RTP /
    255.0F;
  for (i = 0; i < 228096; i++) {
    vipcellcounting_win_SegmentCells_Subsystem_Autothreshold_1[i] =
      (vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Subtract2_1[i] <=
       cnt);
  }

  /* End of S-Function (svipgraythresh): '<S9>/Autothreshold' */
}

/* Start for atomic system: '<S2>/Subsystem1' */
void vipcellcountin_Subsystem1_Start(void)
{
  int32_T previous;
  int32_T step;
  boolean_T isValid;
  int32_T idxOut;
  int32_T curNumNonZ;
  int32_T n;
  int32_T m;

  /* Start for S-Function (svipmorphop): '<S11>/Dilation' */
  previous = 0;
  step = 0;
  curNumNonZ = 0;
  isValid = TRUE;
  idxOut = 0;
  n = 0;
  while (n < 1) {
    m = 0;
    while (m < 7) {
      if (curNumNonZ == 0) {
        curNumNonZ = 1;
      } else if (curNumNonZ == 1) {
        step = idxOut - previous;
        curNumNonZ = 2;
      } else if (idxOut - previous == step) {
        curNumNonZ++;
      } else {
        isValid = FALSE;
        m = 7;
        n = 1;
      }

      previous = idxOut;
      idxOut++;
      m++;
    }

    idxOut += 438;
    n++;
  }

  if (isValid && (curNumNonZ >= 4)) {
    if (step == 445) {
      vipcellcounting_win_DWork.Dilation_STREL_DW[0] = 2;
    } else if (step == 1) {
      vipcellcounting_win_DWork.Dilation_STREL_DW[0] = 1;
    } else {
      vipcellcounting_win_DWork.Dilation_STREL_DW[0] = 0;
    }
  } else {
    vipcellcounting_win_DWork.Dilation_STREL_DW[0] = 0;
  }

  curNumNonZ = 0;
  isValid = TRUE;
  idxOut = 0;
  n = 0;
  while (n < 7) {
    m = 0;
    while (m < 1) {
      if (curNumNonZ == 0) {
        curNumNonZ = 1;
      } else if (curNumNonZ == 1) {
        step = idxOut - previous;
        curNumNonZ = 2;
      } else if (idxOut - previous == step) {
        curNumNonZ++;
      } else {
        isValid = FALSE;
        m = 1;
        n = 7;
      }

      previous = idxOut;
      idxOut++;
      m++;
    }

    idxOut += 444;
    n++;
  }

  if (isValid && (curNumNonZ >= 4)) {
    if (step == 445) {
      vipcellcounting_win_DWork.Dilation_STREL_DW[1] = 2;
    } else if (step == 1) {
      vipcellcounting_win_DWork.Dilation_STREL_DW[1] = 1;
    } else {
      vipcellcounting_win_DWork.Dilation_STREL_DW[1] = 0;
    }
  } else {
    vipcellcounting_win_DWork.Dilation_STREL_DW[1] = 0;
  }

  previous = 0;
  curNumNonZ = 0;
  n = 0;
  while (n < 1) {
    for (m = 0; m < 7; m++) {
      vipcellcounting_win_DWork.Dilation_DILATE_OFF_DW[previous] = m;
      curNumNonZ++;
      previous++;
    }

    n = 1;
  }

  vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0] = curNumNonZ;
  curNumNonZ = 0;
  for (n = 0; n < 7; n++) {
    m = 0;
    while (m < 1) {
      vipcellcounting_win_DWork.Dilation_DILATE_OFF_DW[previous] = n * 445;
      curNumNonZ++;
      previous++;
      m = 1;
    }
  }

  vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1] = curNumNonZ;

  /* End of Start for S-Function (svipmorphop): '<S11>/Dilation' */

  /* Start for S-Function (svipmorphop): '<S11>/Dilation1' */
  previous = 0;
  step = 0;
  curNumNonZ = 0;
  isValid = TRUE;
  idxOut = 0;
  n = 0;
  while (n < 1) {
    m = 0;
    while (m < 7) {
      if (curNumNonZ == 0) {
        curNumNonZ = 1;
      } else if (curNumNonZ == 1) {
        step = idxOut - previous;
        curNumNonZ = 2;
      } else if (idxOut - previous == step) {
        curNumNonZ++;
      } else {
        isValid = FALSE;
        m = 7;
        n = 1;
      }

      previous = idxOut;
      idxOut++;
      m++;
    }

    idxOut += 438;
    n++;
  }

  if (isValid && (curNumNonZ >= 4)) {
    if (step == 445) {
      vipcellcounting_win_DWork.Dilation1_STREL_DW[0] = 2;
    } else if (step == 1) {
      vipcellcounting_win_DWork.Dilation1_STREL_DW[0] = 1;
    } else {
      vipcellcounting_win_DWork.Dilation1_STREL_DW[0] = 0;
    }
  } else {
    vipcellcounting_win_DWork.Dilation1_STREL_DW[0] = 0;
  }

  curNumNonZ = 0;
  isValid = TRUE;
  idxOut = 0;
  n = 0;
  while (n < 7) {
    m = 0;
    while (m < 1) {
      if (curNumNonZ == 0) {
        curNumNonZ = 1;
      } else if (curNumNonZ == 1) {
        step = idxOut - previous;
        curNumNonZ = 2;
      } else if (idxOut - previous == step) {
        curNumNonZ++;
      } else {
        isValid = FALSE;
        m = 1;
        n = 7;
      }

      previous = idxOut;
      idxOut++;
      m++;
    }

    idxOut += 444;
    n++;
  }

  if (isValid && (curNumNonZ >= 4)) {
    if (step == 445) {
      vipcellcounting_win_DWork.Dilation1_STREL_DW[1] = 2;
    } else if (step == 1) {
      vipcellcounting_win_DWork.Dilation1_STREL_DW[1] = 1;
    } else {
      vipcellcounting_win_DWork.Dilation1_STREL_DW[1] = 0;
    }
  } else {
    vipcellcounting_win_DWork.Dilation1_STREL_DW[1] = 0;
  }

  previous = 0;
  curNumNonZ = 0;
  n = 0;
  while (n < 1) {
    for (m = 0; m < 7; m++) {
      vipcellcounting_win_DWork.Dilation1_DILATE_OFF_DW[previous] = m;
      curNumNonZ++;
      previous++;
    }

    n = 1;
  }

  vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0] = curNumNonZ;
  curNumNonZ = 0;
  for (n = 0; n < 7; n++) {
    m = 0;
    while (m < 1) {
      vipcellcounting_win_DWork.Dilation1_DILATE_OFF_DW[previous] = n * 445;
      curNumNonZ++;
      previous++;
      m = 1;
    }
  }

  vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1] = curNumNonZ;

  /* End of Start for S-Function (svipmorphop): '<S11>/Dilation1' */
}

/* Output and update for atomic system: '<S2>/Subsystem1' */
void vipcellcounting_win_Subsystem1(void)
{
  int32_T ky;
  int32_T ku;
  int32_T i;
  int32_T inIdx;
  int32_T outIdx;
  int32_T col;
  int32_T row;
  int32_T numIter;
  int32_T gOffset;
  int32_T hOffset;
  real32_T Dilation_GBUF_DW[555];
  real32_T Dilation_HBUF_DW[555];
  real32_T Dilation1_GBUF_DW[555];
  real32_T Dilation1_HBUF_DW[555];
  real32_T maxVal;

  /* S-Function (svipmorphop): '<S11>/Dilation' */
  ky = 0;
  ku = 0;
  for (i = 0; i < 3; i++) {
    for (row = 0; row < 445; row++) {
      vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[ky] = (rtMinusInfF);
      ky++;
    }
  }

  for (i = 0; i < 528; i++) {
    vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[ky] = (rtMinusInfF);
    ky++;
    vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[ky] = (rtMinusInfF);
    ky++;
    vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[ky] = (rtMinusInfF);
    ky++;
    memcpy(&vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[ky],
           &vipcellcounting_win_Video_1[ku], 432U * sizeof(real32_T));
    ky += 432;
    ku += 432;
    for (row = 0; row < 10; row++) {
      vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[ky] = (rtMinusInfF);
      ky++;
    }
  }

  for (i = 0; i < 10; i++) {
    for (row = 0; row < 445; row++) {
      vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[ky] = (rtMinusInfF);
      ky++;
    }
  }

  for (i = 0; i < 240745; i++) {
    vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[i] = (rtMinusInfF);
  }

  inIdx = 0;
  outIdx = 3;
  if (vipcellcounting_win_DWork.Dilation_STREL_DW[0] == 0) {
    for (col = 0; col < 540; col++) {
      for (row = 0; row < 438; row++) {
        maxVal = vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[inIdx +
          vipcellcounting_win_DWork.Dilation_DILATE_OFF_DW[0]];
        for (i = 1; i < vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0]; i++) {
          if (vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[inIdx +
              vipcellcounting_win_DWork.Dilation_DILATE_OFF_DW[i]] > maxVal) {
            maxVal = vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[inIdx +
              vipcellcounting_win_DWork.Dilation_DILATE_OFF_DW[i]];
          }
        }

        vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[outIdx] = maxVal;
        inIdx++;
        outIdx++;
      }

      inIdx += 7;
      outIdx += 7;
    }
  } else if (vipcellcounting_win_DWork.Dilation_STREL_DW[0] == 1) {
    numIter = 441 / vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0];
    ky = vipcellcounting_win_DWork.Dilation_DILATE_OFF_DW[0] / 445 * 445;
    ku = vipcellcounting_win_DWork.Dilation_DILATE_OFF_DW[0] - ky;
    gOffset = (vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0] + ku) - 3;
    hOffset = ku - 3;
    inIdx = 3 + ky;
    for (i = 0; i < vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0]; i++) {
      Dilation_GBUF_DW[i] = (rtMinusInfF);
    }

    ky = (numIter + 1) * vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0];
    for (i = ky; i < ky + vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0]; i++)
    {
      Dilation_GBUF_DW[i] = (rtMinusInfF);
    }

    for (i = 0; i < vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0]; i++) {
      Dilation_HBUF_DW[i] = (rtMinusInfF);
    }

    for (i = ky; i < ky + vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0]; i++)
    {
      Dilation_HBUF_DW[i] = (rtMinusInfF);
    }

    for (col = 0; col < 540; col++) {
      ky = vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0];
      for (ku = 0; ku < numIter; ku++) {
        Dilation_GBUF_DW[ky] =
          vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[inIdx];
        ky++;
        inIdx++;
        for (i = 1; i < vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0]; i++) {
          if ((Dilation_GBUF_DW[ky - 1] >=
               vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[inIdx]) ||
              rtIsNaNF(vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[inIdx]))
          {
            Dilation_GBUF_DW[ky] = Dilation_GBUF_DW[ky - 1];
          } else {
            Dilation_GBUF_DW[ky] =
              vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[inIdx];
          }

          ky++;
          inIdx++;
        }
      }

      for (ku = 0; ku < numIter; ku++) {
        Dilation_HBUF_DW[ky - 1] =
          vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[inIdx - 1];
        ky--;
        inIdx--;
        for (i = 1; i < vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0]; i++) {
          if ((Dilation_HBUF_DW[ky] >=
               vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[inIdx - 1]) ||
              rtIsNaNF(vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[inIdx -
                       1])) {
            Dilation_HBUF_DW[ky - 1] = Dilation_HBUF_DW[ky];
          } else {
            Dilation_HBUF_DW[ky - 1] =
              vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[inIdx - 1];
          }

          ky--;
          inIdx--;
        }
      }

      inIdx += 445;
      for (row = 0; row < 438; row++) {
        if ((Dilation_GBUF_DW[(ky + gOffset) - 1] >= Dilation_HBUF_DW[ky +
             hOffset]) || rtIsNaNF(Dilation_HBUF_DW[ky + hOffset])) {
          vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[outIdx] =
            Dilation_GBUF_DW[(ky + gOffset) - 1];
        } else {
          vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[outIdx] =
            Dilation_HBUF_DW[ky + hOffset];
        }

        ky++;
        outIdx++;
      }

      outIdx += 7;
    }
  } else {
    numIter = 540 / vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0];
    ku = vipcellcounting_win_DWork.Dilation_DILATE_OFF_DW[0] / 445;
    gOffset = vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0] + ku;
    hOffset = ku;
    inIdx = vipcellcounting_win_DWork.Dilation_DILATE_OFF_DW[0] - ku * 445;
    for (i = 0; i < vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0]; i++) {
      Dilation_GBUF_DW[i] = (rtMinusInfF);
    }

    ky = (numIter + 1) * vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0];
    for (i = ky; i < ky + vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0]; i++)
    {
      Dilation_GBUF_DW[i] = (rtMinusInfF);
    }

    for (i = 0; i < vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0]; i++) {
      Dilation_HBUF_DW[i] = (rtMinusInfF);
    }

    for (i = ky; i < ky + vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0]; i++)
    {
      Dilation_HBUF_DW[i] = (rtMinusInfF);
    }

    for (row = 0; row < 438; row++) {
      ky = vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0];
      for (ku = 0; ku < numIter; ku++) {
        Dilation_GBUF_DW[ky] =
          vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[inIdx];
        ky++;
        inIdx += 445;
        for (i = 1; i < vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0]; i++) {
          if ((Dilation_GBUF_DW[ky - 1] >=
               vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[inIdx]) ||
              rtIsNaNF(vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[inIdx]))
          {
            Dilation_GBUF_DW[ky] = Dilation_GBUF_DW[ky - 1];
          } else {
            Dilation_GBUF_DW[ky] =
              vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[inIdx];
          }

          ky++;
          inIdx += 445;
        }
      }

      for (ku = 0; ku < numIter; ku++) {
        Dilation_HBUF_DW[ky - 1] =
          vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[inIdx - 445];
        ky--;
        inIdx -= 445;
        for (i = 1; i < vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0]; i++) {
          if ((Dilation_HBUF_DW[ky] >=
               vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[inIdx - 445]) ||
              rtIsNaNF(vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[inIdx -
                       445])) {
            Dilation_HBUF_DW[ky - 1] = Dilation_HBUF_DW[ky];
          } else {
            Dilation_HBUF_DW[ky - 1] =
              vipcellcounting_win_DWork.Dilation_ONE_PAD_IMG_DW[inIdx - 445];
          }

          ky--;
          inIdx -= 445;
        }
      }

      inIdx++;
      for (col = 0; col < 540; col++) {
        if ((Dilation_GBUF_DW[(ky + gOffset) - 1] >= Dilation_HBUF_DW[ky +
             hOffset]) || rtIsNaNF(Dilation_HBUF_DW[ky + hOffset])) {
          vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[outIdx] =
            Dilation_GBUF_DW[(ky + gOffset) - 1];
        } else {
          vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[outIdx] =
            Dilation_HBUF_DW[ky + hOffset];
        }

        ky++;
        outIdx += 445;
      }

      outIdx += -240299;
    }
  }

  inIdx = 3;
  outIdx = 0;
  if (vipcellcounting_win_DWork.Dilation_STREL_DW[1] == 0) {
    for (col = 0; col < 528; col++) {
      for (row = 3; row < 435; row++) {
        maxVal = vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[inIdx +
          vipcellcounting_win_DWork.Dilation_DILATE_OFF_DW[vipcellcounting_win_DWork.Dilation_NUMNONZ_DW
          [0]]];
        for (i = 1; i < vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1]; i++) {
          if (vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[vipcellcounting_win_DWork.Dilation_DILATE_OFF_DW
              [i + vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0]] + inIdx] >
              maxVal) {
            maxVal =
              vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[vipcellcounting_win_DWork.Dilation_DILATE_OFF_DW
              [i + vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[0]] + inIdx];
          }
        }

        vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Dilation_1[outIdx]
          = maxVal;
        inIdx++;
        outIdx++;
      }

      inIdx += 13;
    }
  } else if (vipcellcounting_win_DWork.Dilation_STREL_DW[1] == 1) {
    numIter = 432 / vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1];
    ky =
      vipcellcounting_win_DWork.Dilation_DILATE_OFF_DW[vipcellcounting_win_DWork.Dilation_NUMNONZ_DW
      [0]] / 445 * 445;
    ku =
      vipcellcounting_win_DWork.Dilation_DILATE_OFF_DW[vipcellcounting_win_DWork.Dilation_NUMNONZ_DW
      [0]] - ky;
    gOffset = vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1] + ku;
    hOffset = ku;
    inIdx = ky + 3;
    for (i = 0; i < vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1]; i++) {
      Dilation_GBUF_DW[i] = (rtMinusInfF);
    }

    ky = (numIter + 1) * vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1];
    for (i = ky; i < ky + vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1]; i++)
    {
      Dilation_GBUF_DW[i] = (rtMinusInfF);
    }

    for (i = 0; i < vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1]; i++) {
      Dilation_HBUF_DW[i] = (rtMinusInfF);
    }

    for (i = ky; i < ky + vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1]; i++)
    {
      Dilation_HBUF_DW[i] = (rtMinusInfF);
    }

    for (col = 0; col < 528; col++) {
      ky = vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1];
      for (ku = 0; ku < numIter; ku++) {
        Dilation_GBUF_DW[ky] =
          vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[inIdx];
        ky++;
        inIdx++;
        for (i = 1; i < vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1]; i++) {
          if ((Dilation_GBUF_DW[ky - 1] >=
               vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[inIdx]) ||
              rtIsNaNF(vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[inIdx]))
          {
            Dilation_GBUF_DW[ky] = Dilation_GBUF_DW[ky - 1];
          } else {
            Dilation_GBUF_DW[ky] =
              vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[inIdx];
          }

          ky++;
          inIdx++;
        }
      }

      for (ku = 0; ku < numIter; ku++) {
        Dilation_HBUF_DW[ky - 1] =
          vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[inIdx - 1];
        ky--;
        inIdx--;
        for (i = 1; i < vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1]; i++) {
          if ((Dilation_HBUF_DW[ky] >=
               vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[inIdx - 1]) ||
              rtIsNaNF(vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[inIdx -
                       1])) {
            Dilation_HBUF_DW[ky - 1] = Dilation_HBUF_DW[ky];
          } else {
            Dilation_HBUF_DW[ky - 1] =
              vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[inIdx - 1];
          }

          ky--;
          inIdx--;
        }
      }

      inIdx += 445;
      for (row = 3; row < 435; row++) {
        if ((Dilation_GBUF_DW[(ky + gOffset) - 1] >= Dilation_HBUF_DW[ky +
             hOffset]) || rtIsNaNF(Dilation_HBUF_DW[ky + hOffset])) {
          vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Dilation_1[outIdx]
            = Dilation_GBUF_DW[(ky + gOffset) - 1];
        } else {
          vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Dilation_1[outIdx]
            = Dilation_HBUF_DW[ky + hOffset];
        }

        ky++;
        outIdx++;
      }
    }
  } else {
    numIter = 531 / vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1];
    ku =
      vipcellcounting_win_DWork.Dilation_DILATE_OFF_DW[vipcellcounting_win_DWork.Dilation_NUMNONZ_DW
      [0]] / 445;
    gOffset = (vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1] + ku) - 3;
    hOffset = ku - 3;
    inIdx =
      (vipcellcounting_win_DWork.Dilation_DILATE_OFF_DW[vipcellcounting_win_DWork.Dilation_NUMNONZ_DW
       [0]] - ku * 445) + 1338;
    for (i = 0; i < vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1]; i++) {
      Dilation_GBUF_DW[i] = (rtMinusInfF);
    }

    ky = (numIter + 1) * vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1];
    for (i = ky; i < ky + vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1]; i++)
    {
      Dilation_GBUF_DW[i] = (rtMinusInfF);
    }

    for (i = 0; i < vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1]; i++) {
      Dilation_HBUF_DW[i] = (rtMinusInfF);
    }

    for (i = ky; i < ky + vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1]; i++)
    {
      Dilation_HBUF_DW[i] = (rtMinusInfF);
    }

    for (row = 3; row < 435; row++) {
      ky = vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1];
      for (ku = 0; ku < numIter; ku++) {
        Dilation_GBUF_DW[ky] =
          vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[inIdx];
        ky++;
        inIdx += 445;
        for (i = 1; i < vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1]; i++) {
          if ((Dilation_GBUF_DW[ky - 1] >=
               vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[inIdx]) ||
              rtIsNaNF(vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[inIdx]))
          {
            Dilation_GBUF_DW[ky] = Dilation_GBUF_DW[ky - 1];
          } else {
            Dilation_GBUF_DW[ky] =
              vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[inIdx];
          }

          ky++;
          inIdx += 445;
        }
      }

      for (ku = 0; ku < numIter; ku++) {
        Dilation_HBUF_DW[ky - 1] =
          vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[inIdx - 445];
        ky--;
        inIdx -= 445;
        for (i = 1; i < vipcellcounting_win_DWork.Dilation_NUMNONZ_DW[1]; i++) {
          if ((Dilation_HBUF_DW[ky] >=
               vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[inIdx - 445]) ||
              rtIsNaNF(vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[inIdx -
                       445])) {
            Dilation_HBUF_DW[ky - 1] = Dilation_HBUF_DW[ky];
          } else {
            Dilation_HBUF_DW[ky - 1] =
              vipcellcounting_win_DWork.Dilation_TWO_PAD_IMG_DW[inIdx - 445];
          }

          ky--;
          inIdx -= 445;
        }
      }

      inIdx++;
      for (col = 0; col < 528; col++) {
        if ((Dilation_GBUF_DW[(ky + gOffset) - 1] >= Dilation_HBUF_DW[ky +
             hOffset]) || rtIsNaNF(Dilation_HBUF_DW[ky + hOffset])) {
          vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Dilation_1[outIdx]
            = Dilation_GBUF_DW[(ky + gOffset) - 1];
        } else {
          vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Dilation_1[outIdx]
            = Dilation_HBUF_DW[ky + hOffset];
        }

        ky++;
        outIdx += 432;
      }

      outIdx += -228095;
    }
  }

  for (i = 0; i < 228096; i++) {
    /* Gain: '<S11>/Gain' */
    vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Gain_1[i] =
      vipcellcounting_win_P.Gain_Gain * vipcellcounting_win_Video_1[i];

    /* Sum: '<S11>/Subtract' */
    vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Subtract_1[i] =
      vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Gain_1[i] -
      vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Dilation_1[i];

    /* Saturate: '<S11>/Saturation' */
    if (vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Subtract_1[i] >=
        vipcellcounting_win_P.Saturation_UpperSat) {
      vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Saturation_1[i] =
        vipcellcounting_win_P.Saturation_UpperSat;
    } else if
        (vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Subtract_1[i] <=
         vipcellcounting_win_P.Saturation_LowerSat) {
      vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Saturation_1[i] =
        vipcellcounting_win_P.Saturation_LowerSat;
    } else {
      vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Saturation_1[i] =
        vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Subtract_1[i];
    }

    /* End of Saturate: '<S11>/Saturation' */
  }

  /* End of S-Function (svipmorphop): '<S11>/Dilation' */

  /* S-Function (svipmorphop): '<S11>/Dilation1' */
  ky = 0;
  ku = 0;
  for (i = 0; i < 3; i++) {
    for (row = 0; row < 445; row++) {
      vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[ky] = (rtMinusInfF);
      ky++;
    }
  }

  for (i = 0; i < 528; i++) {
    vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[ky] = (rtMinusInfF);
    ky++;
    vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[ky] = (rtMinusInfF);
    ky++;
    vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[ky] = (rtMinusInfF);
    ky++;
    memcpy(&vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[ky],
           &vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Saturation_1
           [ku], 432U * sizeof(real32_T));
    ky += 432;
    ku += 432;
    for (row = 0; row < 10; row++) {
      vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[ky] = (rtMinusInfF);
      ky++;
    }
  }

  for (i = 0; i < 10; i++) {
    for (row = 0; row < 445; row++) {
      vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[ky] = (rtMinusInfF);
      ky++;
    }
  }

  for (i = 0; i < 240745; i++) {
    vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[i] = (rtMinusInfF);
  }

  inIdx = 0;
  outIdx = 3;
  if (vipcellcounting_win_DWork.Dilation1_STREL_DW[0] == 0) {
    for (col = 0; col < 540; col++) {
      for (row = 0; row < 438; row++) {
        maxVal = vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[inIdx +
          vipcellcounting_win_DWork.Dilation1_DILATE_OFF_DW[0]];
        for (i = 1; i < vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0]; i++)
        {
          if (vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[inIdx +
              vipcellcounting_win_DWork.Dilation1_DILATE_OFF_DW[i]] > maxVal) {
            maxVal = vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[inIdx +
              vipcellcounting_win_DWork.Dilation1_DILATE_OFF_DW[i]];
          }
        }

        vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[outIdx] = maxVal;
        inIdx++;
        outIdx++;
      }

      inIdx += 7;
      outIdx += 7;
    }
  } else if (vipcellcounting_win_DWork.Dilation1_STREL_DW[0] == 1) {
    numIter = 441 / vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0];
    ky = vipcellcounting_win_DWork.Dilation1_DILATE_OFF_DW[0] / 445 * 445;
    ku = vipcellcounting_win_DWork.Dilation1_DILATE_OFF_DW[0] - ky;
    gOffset = (vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0] + ku) - 3;
    hOffset = ku - 3;
    inIdx = 3 + ky;
    for (i = 0; i < vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0]; i++) {
      Dilation1_GBUF_DW[i] = (rtMinusInfF);
    }

    ky = (numIter + 1) * vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0];
    for (i = ky; i < ky + vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0]; i++)
    {
      Dilation1_GBUF_DW[i] = (rtMinusInfF);
    }

    for (i = 0; i < vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0]; i++) {
      Dilation1_HBUF_DW[i] = (rtMinusInfF);
    }

    for (i = ky; i < ky + vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0]; i++)
    {
      Dilation1_HBUF_DW[i] = (rtMinusInfF);
    }

    for (col = 0; col < 540; col++) {
      ky = vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0];
      for (ku = 0; ku < numIter; ku++) {
        Dilation1_GBUF_DW[ky] =
          vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[inIdx];
        ky++;
        inIdx++;
        for (i = 1; i < vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0]; i++)
        {
          if ((Dilation1_GBUF_DW[ky - 1] >=
               vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[inIdx]) ||
              rtIsNaNF(vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[inIdx]))
          {
            Dilation1_GBUF_DW[ky] = Dilation1_GBUF_DW[ky - 1];
          } else {
            Dilation1_GBUF_DW[ky] =
              vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[inIdx];
          }

          ky++;
          inIdx++;
        }
      }

      for (ku = 0; ku < numIter; ku++) {
        Dilation1_HBUF_DW[ky - 1] =
          vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[inIdx - 1];
        ky--;
        inIdx--;
        for (i = 1; i < vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0]; i++)
        {
          if ((Dilation1_HBUF_DW[ky] >=
               vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[inIdx - 1]) ||
              rtIsNaNF(vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[inIdx
                       - 1])) {
            Dilation1_HBUF_DW[ky - 1] = Dilation1_HBUF_DW[ky];
          } else {
            Dilation1_HBUF_DW[ky - 1] =
              vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[inIdx - 1];
          }

          ky--;
          inIdx--;
        }
      }

      inIdx += 445;
      for (row = 0; row < 438; row++) {
        if ((Dilation1_GBUF_DW[(ky + gOffset) - 1] >= Dilation1_HBUF_DW[ky +
             hOffset]) || rtIsNaNF(Dilation1_HBUF_DW[ky + hOffset])) {
          vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[outIdx] =
            Dilation1_GBUF_DW[(ky + gOffset) - 1];
        } else {
          vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[outIdx] =
            Dilation1_HBUF_DW[ky + hOffset];
        }

        ky++;
        outIdx++;
      }

      outIdx += 7;
    }
  } else {
    numIter = 540 / vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0];
    ku = vipcellcounting_win_DWork.Dilation1_DILATE_OFF_DW[0] / 445;
    gOffset = vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0] + ku;
    hOffset = ku;
    inIdx = vipcellcounting_win_DWork.Dilation1_DILATE_OFF_DW[0] - ku * 445;
    for (i = 0; i < vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0]; i++) {
      Dilation1_GBUF_DW[i] = (rtMinusInfF);
    }

    ky = (numIter + 1) * vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0];
    for (i = ky; i < ky + vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0]; i++)
    {
      Dilation1_GBUF_DW[i] = (rtMinusInfF);
    }

    for (i = 0; i < vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0]; i++) {
      Dilation1_HBUF_DW[i] = (rtMinusInfF);
    }

    for (i = ky; i < ky + vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0]; i++)
    {
      Dilation1_HBUF_DW[i] = (rtMinusInfF);
    }

    for (row = 0; row < 438; row++) {
      ky = vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0];
      for (ku = 0; ku < numIter; ku++) {
        Dilation1_GBUF_DW[ky] =
          vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[inIdx];
        ky++;
        inIdx += 445;
        for (i = 1; i < vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0]; i++)
        {
          if ((Dilation1_GBUF_DW[ky - 1] >=
               vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[inIdx]) ||
              rtIsNaNF(vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[inIdx]))
          {
            Dilation1_GBUF_DW[ky] = Dilation1_GBUF_DW[ky - 1];
          } else {
            Dilation1_GBUF_DW[ky] =
              vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[inIdx];
          }

          ky++;
          inIdx += 445;
        }
      }

      for (ku = 0; ku < numIter; ku++) {
        Dilation1_HBUF_DW[ky - 1] =
          vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[inIdx - 445];
        ky--;
        inIdx -= 445;
        for (i = 1; i < vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0]; i++)
        {
          if ((Dilation1_HBUF_DW[ky] >=
               vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[inIdx - 445]) ||
              rtIsNaNF(vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[inIdx
                       - 445])) {
            Dilation1_HBUF_DW[ky - 1] = Dilation1_HBUF_DW[ky];
          } else {
            Dilation1_HBUF_DW[ky - 1] =
              vipcellcounting_win_DWork.Dilation1_ONE_PAD_IMG_DW[inIdx - 445];
          }

          ky--;
          inIdx -= 445;
        }
      }

      inIdx++;
      for (col = 0; col < 540; col++) {
        if ((Dilation1_GBUF_DW[(ky + gOffset) - 1] >= Dilation1_HBUF_DW[ky +
             hOffset]) || rtIsNaNF(Dilation1_HBUF_DW[ky + hOffset])) {
          vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[outIdx] =
            Dilation1_GBUF_DW[(ky + gOffset) - 1];
        } else {
          vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[outIdx] =
            Dilation1_HBUF_DW[ky + hOffset];
        }

        ky++;
        outIdx += 445;
      }

      outIdx += -240299;
    }
  }

  inIdx = 3;
  outIdx = 0;
  if (vipcellcounting_win_DWork.Dilation1_STREL_DW[1] == 0) {
    for (col = 0; col < 528; col++) {
      for (row = 3; row < 435; row++) {
        maxVal = vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[inIdx +
          vipcellcounting_win_DWork.Dilation1_DILATE_OFF_DW[vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW
          [0]]];
        for (i = 1; i < vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1]; i++)
        {
          if (vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[vipcellcounting_win_DWork.Dilation1_DILATE_OFF_DW
              [i + vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0]] + inIdx] >
              maxVal) {
            maxVal =
              vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[vipcellcounting_win_DWork.Dilation1_DILATE_OFF_DW
              [i + vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[0]] + inIdx];
          }
        }

        vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Dilation1_1[outIdx]
          = maxVal;
        inIdx++;
        outIdx++;
      }

      inIdx += 13;
    }
  } else if (vipcellcounting_win_DWork.Dilation1_STREL_DW[1] == 1) {
    numIter = 432 / vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1];
    ky =
      vipcellcounting_win_DWork.Dilation1_DILATE_OFF_DW[vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW
      [0]] / 445 * 445;
    ku =
      vipcellcounting_win_DWork.Dilation1_DILATE_OFF_DW[vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW
      [0]] - ky;
    gOffset = vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1] + ku;
    hOffset = ku;
    inIdx = ky + 3;
    for (i = 0; i < vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1]; i++) {
      Dilation1_GBUF_DW[i] = (rtMinusInfF);
    }

    ky = (numIter + 1) * vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1];
    for (i = ky; i < ky + vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1]; i++)
    {
      Dilation1_GBUF_DW[i] = (rtMinusInfF);
    }

    for (i = 0; i < vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1]; i++) {
      Dilation1_HBUF_DW[i] = (rtMinusInfF);
    }

    for (i = ky; i < ky + vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1]; i++)
    {
      Dilation1_HBUF_DW[i] = (rtMinusInfF);
    }

    for (col = 0; col < 528; col++) {
      ky = vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1];
      for (ku = 0; ku < numIter; ku++) {
        Dilation1_GBUF_DW[ky] =
          vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[inIdx];
        ky++;
        inIdx++;
        for (i = 1; i < vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1]; i++)
        {
          if ((Dilation1_GBUF_DW[ky - 1] >=
               vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[inIdx]) ||
              rtIsNaNF(vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[inIdx]))
          {
            Dilation1_GBUF_DW[ky] = Dilation1_GBUF_DW[ky - 1];
          } else {
            Dilation1_GBUF_DW[ky] =
              vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[inIdx];
          }

          ky++;
          inIdx++;
        }
      }

      for (ku = 0; ku < numIter; ku++) {
        Dilation1_HBUF_DW[ky - 1] =
          vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[inIdx - 1];
        ky--;
        inIdx--;
        for (i = 1; i < vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1]; i++)
        {
          if ((Dilation1_HBUF_DW[ky] >=
               vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[inIdx - 1]) ||
              rtIsNaNF(vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[inIdx
                       - 1])) {
            Dilation1_HBUF_DW[ky - 1] = Dilation1_HBUF_DW[ky];
          } else {
            Dilation1_HBUF_DW[ky - 1] =
              vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[inIdx - 1];
          }

          ky--;
          inIdx--;
        }
      }

      inIdx += 445;
      for (row = 3; row < 435; row++) {
        if ((Dilation1_GBUF_DW[(ky + gOffset) - 1] >= Dilation1_HBUF_DW[ky +
             hOffset]) || rtIsNaNF(Dilation1_HBUF_DW[ky + hOffset])) {
          vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Dilation1_1[outIdx]
            = Dilation1_GBUF_DW[(ky + gOffset) - 1];
        } else {
          vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Dilation1_1[outIdx]
            = Dilation1_HBUF_DW[ky + hOffset];
        }

        ky++;
        outIdx++;
      }
    }
  } else {
    numIter = 531 / vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1];
    ku =
      vipcellcounting_win_DWork.Dilation1_DILATE_OFF_DW[vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW
      [0]] / 445;
    gOffset = (vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1] + ku) - 3;
    hOffset = ku - 3;
    inIdx =
      (vipcellcounting_win_DWork.Dilation1_DILATE_OFF_DW[vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW
       [0]] - ku * 445) + 1338;
    for (i = 0; i < vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1]; i++) {
      Dilation1_GBUF_DW[i] = (rtMinusInfF);
    }

    ky = (numIter + 1) * vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1];
    for (i = ky; i < ky + vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1]; i++)
    {
      Dilation1_GBUF_DW[i] = (rtMinusInfF);
    }

    for (i = 0; i < vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1]; i++) {
      Dilation1_HBUF_DW[i] = (rtMinusInfF);
    }

    for (i = ky; i < ky + vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1]; i++)
    {
      Dilation1_HBUF_DW[i] = (rtMinusInfF);
    }

    for (row = 3; row < 435; row++) {
      ky = vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1];
      for (ku = 0; ku < numIter; ku++) {
        Dilation1_GBUF_DW[ky] =
          vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[inIdx];
        ky++;
        inIdx += 445;
        for (i = 1; i < vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1]; i++)
        {
          if ((Dilation1_GBUF_DW[ky - 1] >=
               vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[inIdx]) ||
              rtIsNaNF(vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[inIdx]))
          {
            Dilation1_GBUF_DW[ky] = Dilation1_GBUF_DW[ky - 1];
          } else {
            Dilation1_GBUF_DW[ky] =
              vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[inIdx];
          }

          ky++;
          inIdx += 445;
        }
      }

      for (ku = 0; ku < numIter; ku++) {
        Dilation1_HBUF_DW[ky - 1] =
          vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[inIdx - 445];
        ky--;
        inIdx -= 445;
        for (i = 1; i < vipcellcounting_win_DWork.Dilation1_NUMNONZ_DW[1]; i++)
        {
          if ((Dilation1_HBUF_DW[ky] >=
               vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[inIdx - 445]) ||
              rtIsNaNF(vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[inIdx
                       - 445])) {
            Dilation1_HBUF_DW[ky - 1] = Dilation1_HBUF_DW[ky];
          } else {
            Dilation1_HBUF_DW[ky - 1] =
              vipcellcounting_win_DWork.Dilation1_TWO_PAD_IMG_DW[inIdx - 445];
          }

          ky--;
          inIdx -= 445;
        }
      }

      inIdx++;
      for (col = 0; col < 528; col++) {
        if ((Dilation1_GBUF_DW[(ky + gOffset) - 1] >= Dilation1_HBUF_DW[ky +
             hOffset]) || rtIsNaNF(Dilation1_HBUF_DW[ky + hOffset])) {
          vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Dilation1_1[outIdx]
            = Dilation1_GBUF_DW[(ky + gOffset) - 1];
        } else {
          vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Dilation1_1[outIdx]
            = Dilation1_HBUF_DW[ky + hOffset];
        }

        ky++;
        outIdx += 432;
      }

      outIdx += -228095;
    }
  }

  /* End of S-Function (svipmorphop): '<S11>/Dilation1' */

  /* Sum: '<S11>/Subtract2' */
  for (i = 0; i < 228096; i++) {
    vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Subtract2_1[i] =
      vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Dilation1_1[i] -
      vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Saturation_1[i];
  }

  /* End of Sum: '<S11>/Subtract2' */
}

/* Output and update for atomic system: '<Root>/Subsystem' */
void vipcellcounting_win_Subsystem_c(void)
{
  boolean_T maxNumBlobsReached;
  int32_T loop;
  uint16_T currentLabel;
  int32_T i;
  int32_T idx;
  int32_T n;
  uint32_T stackIdx;
  uint32_T pixIdx;
  uint32_T start_pixIdx;
  uint32_T walkerIdx;
  uint32_T numBlobs;
  int32_T ns;
  int32_T j;

  /* S-Function (svipblob): '<S3>/Count Cells' */
  maxNumBlobsReached = FALSE;
  memset(&vipcellcounting_win_DWork.CountCells_PAD_DW[0], 0, 435U * sizeof
         (uint16_T));
  currentLabel = 1U;
  i = 0;
  idx = 435;
  for (n = 0; n < 528; n++) {
    for (j = 0; j < 432; j++) {
      vipcellcounting_win_DWork.CountCells_PAD_DW[idx] = (uint16_T)
        (vipcellcounting_win_SegmentCells_Subsystem_Autothreshold_1[i] ? 65535 :
         0);
      i++;
      idx++;
    }

    vipcellcounting_win_DWork.CountCells_PAD_DW[idx] = 0U;
    vipcellcounting_win_DWork.CountCells_PAD_DW[idx + 1] = 0U;
    idx += 2;
  }

  memset(&vipcellcounting_win_DWork.CountCells_PAD_DW[idx], 0, 433U * sizeof
         (uint16_T));
  loop = 0;
  pixIdx = 0U;
  n = 0;
  while (n < 528) {
    idx = 0;
    ns = (loop + 1) * 434;
    j = 0;
    while (j < 432) {
      numBlobs = (uint32_T)((ns + idx) + 1);
      start_pixIdx = pixIdx;
      if (vipcellcounting_win_DWork.CountCells_PAD_DW[numBlobs] == 65535) {
        vipcellcounting_win_DWork.CountCells_PAD_DW[numBlobs] = currentLabel;
        vipcellcounting_win_DWork.CountCells_N_PIXLIST_DW[pixIdx] = (int16_T)
          loop;
        vipcellcounting_win_DWork.CountCells_M_PIXLIST_DW[pixIdx] = (int16_T)idx;
        pixIdx++;
        vipcellcounting_win_DWork.CountCells_NUM_PIX_DW[currentLabel - 1] = 1U;
        vipcellcounting_win_DWork.CountCells_STACK_DW[0U] = numBlobs;
        stackIdx = 1U;
        while (stackIdx != 0U) {
          stackIdx--;
          numBlobs = vipcellcounting_win_DWork.CountCells_STACK_DW[stackIdx];
          for (i = 0; i < 8; i++) {
            walkerIdx = numBlobs + (uint32_T)
              vipcellcounting_win_ConstP.CountCells_WALKER_RTP[i];
            if (vipcellcounting_win_DWork.CountCells_PAD_DW[walkerIdx] == 65535)
            {
              vipcellcounting_win_DWork.CountCells_PAD_DW[walkerIdx] =
                currentLabel;
              vipcellcounting_win_DWork.CountCells_N_PIXLIST_DW[pixIdx] =
                (int16_T)((int16_T)(walkerIdx / 434U) - 1);
              vipcellcounting_win_DWork.CountCells_M_PIXLIST_DW[pixIdx] =
                (int16_T)(walkerIdx % 434U - 1U);
              pixIdx++;
              vipcellcounting_win_DWork.CountCells_NUM_PIX_DW[currentLabel - 1]
                ++;
              vipcellcounting_win_DWork.CountCells_STACK_DW[stackIdx] =
                walkerIdx;
              stackIdx++;
            }
          }
        }

        if ((vipcellcounting_win_DWork.CountCells_NUM_PIX_DW[currentLabel - 1] <
             vipcellcounting_win_P.CountCells_MINAREA_RTP) ||
            (vipcellcounting_win_DWork.CountCells_NUM_PIX_DW[currentLabel - 1] >
             vipcellcounting_win_P.CountCells_MAXAREA_RTP)) {
          currentLabel--;
          pixIdx = start_pixIdx;
        }

        if (currentLabel == 1500) {
          maxNumBlobsReached = TRUE;
          n = 528;
          j = 432;
        }

        if (j < 432) {
          currentLabel++;
        }
      }

      idx++;
      j++;
    }

    loop++;
    n++;
  }

  numBlobs = (uint32_T)(maxNumBlobsReached ? currentLabel : (uint16_T)((uint32_T)
    currentLabel - 1U));
  loop = 0;
  idx = 0;
  for (i = 0; i < (int32_T)numBlobs; i++) {
    ns = 0;
    n = 0;
    for (j = 0; j < (int32_T)vipcellcounting_win_DWork.CountCells_NUM_PIX_DW[i];
         j++) {
      ns += vipcellcounting_win_DWork.CountCells_N_PIXLIST_DW[j + idx];
      n += vipcellcounting_win_DWork.CountCells_M_PIXLIST_DW[j + loop];
    }

    vipcellcounting_win_Subsystem_CountCells_1[i] = (real32_T)ns / (real32_T)
      vipcellcounting_win_DWork.CountCells_NUM_PIX_DW[i] + 1.0F;
    vipcellcounting_win_Subsystem_CountCells_1[numBlobs + (uint32_T)i] =
      (real32_T)n / (real32_T)vipcellcounting_win_DWork.CountCells_NUM_PIX_DW[i]
      + 1.0F;
    loop += (int32_T)vipcellcounting_win_DWork.CountCells_NUM_PIX_DW[i];
    idx += (int32_T)vipcellcounting_win_DWork.CountCells_NUM_PIX_DW[i];
  }

  vipcellcounting_win_Subsystem_CountCells_1Dim[0] = (int32_T)numBlobs;
  vipcellcounting_win_Subsystem_CountCells_1Dim[1] = 2;

  /* End of S-Function (svipblob): '<S3>/Count Cells' */
}

/* Model step function */
void vipcellcounting_win_step(void)
{
  char_T *sErr;
  void *source_R;
  void *eof;
  int32_T indxLocation;
  int32_T indxColor;
  int32_T opacityIndx;
  real32_T opacityVal;
  int32_T strLen;
  int32_T strPtrIdx;
  real32_T opacityVal_0;
  boolean_T isSizeValid;
  int32_T numShape;
  int32_T size;
  int32_T in;
  int32_T i;
  boolean_T visited;
  boolean_T visited_0;
  boolean_T done;
  int32_T idxFillColor;
  int32_T line_idx;
  int32_T line_idx_0;
  int32_T line_idx_1;
  int32_T line_idx_2;
  int32_T line_idx_3;
  int32_T line_idx_4;
  int32_T line_idx_5;
  int32_T line_idx_6;
  real_T tmp;
  real32_T v;

  /* S-Function (sdspwmmfi2): '<Root>/Video' */
  sErr = GetErrorBuffer(&vipcellcounting_win_DWork.Video_HostLib[0U]);
  source_R = (void *)&vipcellcounting_win_Video_1[0U];
  eof = (void *)&(&vipcellcounting_win_Video_2)[0U];
  LibOutputs_FromMMFile(&vipcellcounting_win_DWork.Video_HostLib[0U], eof,
                        GetNullPointer(), source_R, GetNullPointer(),
                        GetNullPointer());
  if (*sErr != 0) {
    rtmSetErrorStatus(vipcellcounting_win_M, sErr);
    rtmSetStopRequested(vipcellcounting_win_M, 1);
  }

  /* End of S-Function (sdspwmmfi2): '<Root>/Video' */

  /* Logic: '<S1>/Logical Operator' */
  vipcellcounting_win_DisplayResults_LogicalOperator_1 =
    !vipcellcounting_win_Video_2;

  /* Outputs for Enabled SubSystem: '<S1>/Counter1' incorporates:
   *  EnablePort: '<S5>/Reset'
   */
  if (vipcellcounting_win_DisplayResults_LogicalOperator_1) {
    if (!vipcellcounting_win_DWork.Counter1_MODE) {
      /* InitializeConditions for UnitDelay: '<S6>/Output' */
      vipcellcounting_win_DWork.Output_DSTATE = vipcellcounting_win_P.Output_X0;
      vipcellcounting_win_DWork.Counter1_MODE = TRUE;
    }

    /* UnitDelay: '<S6>/Output' */
    vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_Output_1 =
      vipcellcounting_win_DWork.Output_DSTATE;

    /* Bias: '<S5>/Bias' */
    vipcellcounting_win_DisplayResults_Counter1_Bias_1 = (uint16_T)((uint32_T)
      vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_Output_1 +
      (uint32_T)vipcellcounting_win_P.Bias_Bias);

    /* Constant: '<S7>/FixPt Constant' */
    vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_IncrementRealWorld_FixPtConstant_1
      = vipcellcounting_win_P.FixPtConstant_Value;

    /* Sum: '<S7>/FixPt Sum1' */
    vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_IncrementRealWorld_FixPtSum1_1
      = (uint16_T)((uint32_T)
                   vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_Output_1
                   + (uint32_T)
                   vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_IncrementRealWorld_FixPtConstant_1);

    /* Constant: '<S8>/Constant' */
    vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_WrapToZero_Constant_1
      = vipcellcounting_win_P.Constant_Value;

    /* Switch: '<S8>/FixPt Switch' */
    if (vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_IncrementRealWorld_FixPtSum1_1
        > vipcellcounting_win_P.FixPtSwitch_Threshold) {
      vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_WrapToZero_FixPtSwitch_1
        =
        vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_WrapToZero_Constant_1;
    } else {
      vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_WrapToZero_FixPtSwitch_1
        =
        vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_IncrementRealWorld_FixPtSum1_1;
    }

    /* End of Switch: '<S8>/FixPt Switch' */

    /* Update for UnitDelay: '<S6>/Output' */
    vipcellcounting_win_DWork.Output_DSTATE =
      vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_WrapToZero_FixPtSwitch_1;
  } else {
    if (vipcellcounting_win_DWork.Counter1_MODE) {
      vipcellcounting_win_DWork.Counter1_MODE = FALSE;
    }
  }

  /* End of Outputs for SubSystem: '<S1>/Counter1' */

  /* Outputs for Atomic SubSystem: '<S2>/Subsystem1' */
  vipcellcounting_win_Subsystem1();

  /* End of Outputs for SubSystem: '<S2>/Subsystem1' */

  /* Outputs for Atomic SubSystem: '<S2>/Subsystem' */
  vipcellcounting_win_Subsystem();

  /* End of Outputs for SubSystem: '<S2>/Subsystem' */

  /* Outputs for Atomic SubSystem: '<Root>/Subsystem' */
  vipcellcounting_win_Subsystem_c();

  /* End of Outputs for SubSystem: '<Root>/Subsystem' */

  /* Probe: '<S1>/Probe' */
  vipcellcounting_win_DisplayResults_Probe_1[0] = (real_T)
    vipcellcounting_win_Subsystem_CountCells_1Dim[0];
  vipcellcounting_win_DisplayResults_Probe_1[1] = (real_T)
    vipcellcounting_win_Subsystem_CountCells_1Dim[1];

  /* Selector: '<S1>/Selector1' */
  vipcellcounting_win_DisplayResults_Selector1_1 =
    vipcellcounting_win_DisplayResults_Probe_1[0];

  /* DataTypeConversion: '<S1>/Data Type Conversion' */
  tmp = floor(vipcellcounting_win_DisplayResults_Selector1_1);
  if (rtIsNaN(tmp) || rtIsInf(tmp)) {
    tmp = 0.0;
  } else {
    tmp = fmod(tmp, 65536.0);
  }

  vipcellcounting_win_DisplayResults_DataTypeConversion_1 = (uint16_T)(tmp < 0.0
    ? (uint16_T)(int32_T)(int16_T)-(int16_T)(uint16_T)-tmp : (uint16_T)tmp);

  /* End of DataTypeConversion: '<S1>/Data Type Conversion' */

  /* SignalConversion: '<S1>/TmpSignal ConversionAtInsert Text1Inport2' */
  vipcellcounting_win_B.TmpSignalConversionAtInsertText[0] =
    vipcellcounting_win_DisplayResults_Counter1_Bias_1;
  vipcellcounting_win_B.TmpSignalConversionAtInsertText[1] =
    vipcellcounting_win_DisplayResults_DataTypeConversion_1;

  /* S-Function (sviptextrender): '<S1>/Insert Text1' */
  /* put the pen at the beginning of the string */
  strLen = 2;
  strPtrIdx = 0;

  /* copy input to output since Simulink isn't giving us in-place operation */
  memcpy(&vipcellcounting_win_DisplayResults_InsertText1_1[0],
         &vipcellcounting_win_Video_1[0], 228096U * sizeof(real32_T));
  while (strLen > 0) {
    isSizeValid = TRUE;
    if (vipcellcounting_win_P.InsertText1_TxtOpacity == 0.0F) {
      isSizeValid = FALSE;
    }

    /* copy location to int32 container */
    line_idx_1 = vipcellcounting_win_P.InsertText1_TxtLocation[1] - 1;
    line_idx = vipcellcounting_win_P.InsertText1_TxtLocation[0] - 1;

    /* update the color/intensity */
    vipcellcounting_win_DWork.InsertText1_colorVectorDW =
      vipcellcounting_win_P.InsertText1_colorRTP;

    /* Saturate color values */
    if (vipcellcounting_win_DWork.InsertText1_colorVectorDW < 0.0F) {
      vipcellcounting_win_DWork.InsertText1_colorVectorDW = 0.0F;
    } else {
      if (vipcellcounting_win_DWork.InsertText1_colorVectorDW > 1.0F) {
        vipcellcounting_win_DWork.InsertText1_colorVectorDW = 1.0F;
      }
    }

    vipcellcounting_win_DWork.InsertText1_VarHolder[0] =
      vipcellcounting_win_B.TmpSignalConversionAtInsertText[strPtrIdx];
    vipcellcounting_win_DWork.InsertText1_VarHolder[1] =
      vipcellcounting_win_B.TmpSignalConversionAtInsertText[1 + strPtrIdx];
    vipcellcounting_win_DWork.InsertText1_VarHolder[2] =
      vipcellcounting_win_B.TmpSignalConversionAtInsertText[2 + strPtrIdx];
    MWVIP_snprintf_wrapper(&vipcellcounting_win_DWork.InsertText1_RepBuf[0U],
      &vipcellcounting_win_DWork.InsertText1_StringDataDW[0],
      &vipcellcounting_win_DWork.InsertText1_VarHolder[0U], strLen + 1, 5, FALSE,
      56);
    in = MWVIP_strlen(&vipcellcounting_win_DWork.InsertText1_RepBuf[0]);
    strPtrIdx += 2;
    strLen -= 2;
    if (isSizeValid) {
      /* put the pen at the beginning of the string */
      idxFillColor = line_idx;
      for (line_idx_0 = 0; line_idx_0 < in; line_idx_0++) {
        if (vipcellcounting_win_DWork.InsertText1_RepBuf[line_idx_0] == 10) {
          line_idx_1 += 20;
          idxFillColor = line_idx;
        } else {
          numShape = vipcellcounting_win_DWork.InsertText1_RepBuf[line_idx_0];
          line_idx_2 = (int32_T)
            vipcellcounting_win_ConstP.InsertText1_GlyInde[numShape];
          if (!((vipcellcounting_win_ConstP.InsertText1_GlyWidt[numShape] != 0) &&
                (vipcellcounting_win_ConstP.InsertText1_GlyHeig[numShape] != 0) &&
                (vipcellcounting_win_ConstP.InsertText1_GlyXAdv[numShape] != 0)))
          {
            numShape = 32;
            line_idx_2 = 0;
          }

          opacityVal = vipcellcounting_win_P.InsertText1_TxtOpacity;
          MWVIP_DrawText_I_single_AA
            (&vipcellcounting_win_ConstP.InsertText1_GlyDat[line_idx_2],
             idxFillColor, line_idx_1 + 16,
             vipcellcounting_win_ConstP.InsertText1_GlyLBea[numShape],
             vipcellcounting_win_ConstP.InsertText1_GlyTBea[numShape],
             vipcellcounting_win_ConstP.InsertText1_GlyWidt[numShape],
             vipcellcounting_win_ConstP.InsertText1_GlyHeig[numShape], 528U,
             432U, &vipcellcounting_win_DisplayResults_InsertText1_1[0],
             &vipcellcounting_win_DWork.InsertText1_colorVectorDW, &(&opacityVal)
             [0U], FALSE);
          idxFillColor +=
            vipcellcounting_win_ConstP.InsertText1_GlyXAdv[numShape];
        }
      }
    }
  }

  /* End of S-Function (sviptextrender): '<S1>/Insert Text1' */

  /* S-Function (sviptextrender): '<S1>/Insert Text2' */
  /* put the pen at the beginning of the string */
  strLen = 1;

  /* copy input to output since Simulink isn't giving us in-place operation */
  memcpy(&vipcellcounting_win_DisplayResults_InsertText2_1[0],
         &vipcellcounting_win_DisplayResults_InsertText1_1[0], 228096U * sizeof
         (real32_T));
  while (strLen > 0) {
    isSizeValid = TRUE;
    if (vipcellcounting_win_P.InsertText2_TxtOpacity == 0.0F) {
      isSizeValid = FALSE;
    }

    /* copy location to int32 container */
    line_idx_1 = vipcellcounting_win_P.InsertText2_TxtLocation[1] - 1;
    line_idx = vipcellcounting_win_P.InsertText2_TxtLocation[0] - 1;

    /* update the color/intensity */
    vipcellcounting_win_DWork.InsertText2_colorVectorDW =
      vipcellcounting_win_P.InsertText2_colorRTP;

    /* Saturate color values */
    if (vipcellcounting_win_DWork.InsertText2_colorVectorDW < 0.0F) {
      vipcellcounting_win_DWork.InsertText2_colorVectorDW = 0.0F;
    } else {
      if (vipcellcounting_win_DWork.InsertText2_colorVectorDW > 1.0F) {
        vipcellcounting_win_DWork.InsertText2_colorVectorDW = 1.0F;
      }
    }

    in = MWVIP_strlen(&vipcellcounting_win_DWork.InsertText2_StringDataDW[0]);
    strLen = 0;
    if (isSizeValid) {
      /* put the pen at the beginning of the string */
      idxFillColor = line_idx;
      for (line_idx_0 = 0; line_idx_0 < in; line_idx_0++) {
        if (vipcellcounting_win_DWork.InsertText2_StringDataDW[line_idx_0] == 10)
        {
          line_idx_1 += 15;
          idxFillColor = line_idx;
        } else {
          numShape =
            vipcellcounting_win_DWork.InsertText2_StringDataDW[line_idx_0];
          line_idx_2 = (int32_T)
            vipcellcounting_win_ConstP.InsertText2_GlyInde[numShape];
          if (!((vipcellcounting_win_ConstP.InsertText2_GlyWidt[numShape] != 0) &&
                (vipcellcounting_win_ConstP.InsertText2_GlyHeig[numShape] != 0) &&
                (vipcellcounting_win_ConstP.InsertText2_GlyXAdv[numShape] != 0)))
          {
            numShape = 32;
            line_idx_2 = 0;
          }

          opacityVal_0 = vipcellcounting_win_P.InsertText2_TxtOpacity;
          MWVIP_DrawText_I_single_AA
            (&vipcellcounting_win_ConstP.InsertText2_GlyDat[line_idx_2],
             idxFillColor, line_idx_1 + 12,
             vipcellcounting_win_ConstP.InsertText2_GlyLBea[numShape],
             vipcellcounting_win_ConstP.InsertText2_GlyTBea[numShape],
             vipcellcounting_win_ConstP.InsertText2_GlyWidt[numShape],
             vipcellcounting_win_ConstP.InsertText2_GlyHeig[numShape], 528U,
             432U, &vipcellcounting_win_DisplayResults_InsertText2_1[0],
             &vipcellcounting_win_DWork.InsertText2_colorVectorDW,
             &(&opacityVal_0)[0U], FALSE);
          idxFillColor +=
            vipcellcounting_win_ConstP.InsertText2_GlyXAdv[numShape];
        }
      }
    }
  }

  /* End of S-Function (sviptextrender): '<S1>/Insert Text2' */

  /* SignalConversion: '<S4>/ConcatBufferAtMatrix ConcatenateIn1' */
  memcpy(&vipcellcounting_win_DisplayResults_ConverttoRGB_MatrixConcatenate_1[0],
         &vipcellcounting_win_DisplayResults_InsertText2_1[0], 228096U * sizeof
         (real32_T));

  /* SignalConversion: '<S4>/ConcatBufferAtMatrix ConcatenateIn2' */
  memcpy(&vipcellcounting_win_DisplayResults_ConverttoRGB_MatrixConcatenate_1
         [228096], &vipcellcounting_win_DisplayResults_InsertText2_1[0], 228096U
         * sizeof(real32_T));

  /* SignalConversion: '<S4>/ConcatBufferAtMatrix ConcatenateIn3' */
  memcpy(&vipcellcounting_win_DisplayResults_ConverttoRGB_MatrixConcatenate_1
         [456192], &vipcellcounting_win_DisplayResults_InsertText2_1[0], 228096U
         * sizeof(real32_T));

  /* S-Function (svipdrawmarkers): '<S1>/Draw Markers' */
  isSizeValid = TRUE;
  numShape = 0;
  if (vipcellcounting_win_Subsystem_CountCells_1Dim[1] > 1) {
    isSizeValid = !((vipcellcounting_win_Subsystem_CountCells_1Dim[1] & 1) != 0);
    numShape = vipcellcounting_win_Subsystem_CountCells_1Dim[0];
  } else {
    if (vipcellcounting_win_Subsystem_CountCells_1Dim[1] == 1) {
      numShape = 1;
      isSizeValid = !((vipcellcounting_win_Subsystem_CountCells_1Dim[0] & 1) !=
                      0);
    }
  }

  /* Copy the image from input to output. */
  memcpy(&vipcellcounting_win_DisplayResults_DrawMarkers_1[0],
         &vipcellcounting_win_DisplayResults_ConverttoRGB_MatrixConcatenate_1[0],
         684288U * sizeof(real32_T));
  if (isSizeValid) {
    /* Update view port. */
    /* Draw all stars. */
    v = (real32_T)fabs(vipcellcounting_win_P.DrawMarkers_RTP_SIZE);
    if (v < 8.388608E+6F) {
      if (v >= 0.5F) {
        size = (int32_T)(real32_T)floor
          (vipcellcounting_win_P.DrawMarkers_RTP_SIZE + 0.5F);
      } else {
        size = (int32_T)(vipcellcounting_win_P.DrawMarkers_RTP_SIZE * 0.0F);
      }
    } else {
      size = (int32_T)vipcellcounting_win_P.DrawMarkers_RTP_SIZE;
    }

    for (indxLocation = 0; indxLocation < numShape; indxLocation++) {
      v = (real32_T)fabs(vipcellcounting_win_Subsystem_CountCells_1[indxLocation
                         + numShape]);
      if (v < 8.388608E+6F) {
        if (v >= 0.5F) {
          v = (real32_T)floor
            (vipcellcounting_win_Subsystem_CountCells_1[indxLocation + numShape]
             + 0.5F);
        } else {
          v = vipcellcounting_win_Subsystem_CountCells_1[indxLocation + numShape]
            * 0.0F;
        }
      } else {
        v = vipcellcounting_win_Subsystem_CountCells_1[indxLocation + numShape];
      }

      indxColor = (int32_T)v - 1;
      v = (real32_T)fabs(vipcellcounting_win_Subsystem_CountCells_1[indxLocation]);
      if (v < 8.388608E+6F) {
        if (v >= 0.5F) {
          v = (real32_T)floor
            (vipcellcounting_win_Subsystem_CountCells_1[indxLocation] + 0.5F);
        } else {
          v = vipcellcounting_win_Subsystem_CountCells_1[indxLocation] * 0.0F;
        }
      } else {
        v = vipcellcounting_win_Subsystem_CountCells_1[indxLocation];
      }

      opacityIndx = (int32_T)v - 1;
      line_idx = opacityIndx - size;
      line_idx_0 = opacityIndx + size;
      isSizeValid = FALSE;

      /* Find the visible portion of a line. */
      visited = FALSE;
      visited_0 = FALSE;
      done = FALSE;
      line_idx_2 = indxColor;
      line_idx_1 = line_idx;
      line_idx_6 = indxColor;
      line_idx_3 = line_idx_0;
      while (!done) {
        strLen = 0;
        strPtrIdx = 0;

        /* Determine viewport violations. */
        if (line_idx_2 < 0) {
          strLen = 4;
        } else {
          if (line_idx_2 > 431) {
            strLen = 8;
          }
        }

        if (line_idx_6 < 0) {
          strPtrIdx = 4;
        } else {
          if (line_idx_6 > 431) {
            strPtrIdx = 8;
          }
        }

        if (line_idx_1 < 0) {
          strLen = (int32_T)((uint32_T)strLen | 1U);
        } else {
          if (line_idx_1 > 527) {
            strLen = (int32_T)((uint32_T)strLen | 2U);
          }
        }

        if (line_idx_3 < 0) {
          strPtrIdx = (int32_T)((uint32_T)strPtrIdx | 1U);
        } else {
          if (line_idx_3 > 527) {
            strPtrIdx = (int32_T)((uint32_T)strPtrIdx | 2U);
          }
        }

        if (!(((uint32_T)strLen | (uint32_T)strPtrIdx) != 0U)) {
          /* Line falls completely within bounds. */
          done = TRUE;
          isSizeValid = TRUE;
        } else if (((uint32_T)strLen & (uint32_T)strPtrIdx) != 0U) {
          /* Line falls completely out of bounds. */
          done = TRUE;
          isSizeValid = FALSE;
        } else if ((uint32_T)strLen != 0U) {
          /* Clip 1st point; if it's in-bounds, clip 2nd point. */
          if (visited) {
            line_idx_2 = indxColor;
            line_idx_1 = line_idx;
          }

          i = line_idx_6 - line_idx_2;
          in = line_idx_3 - line_idx_1;
          if (((uint32_T)strLen & 4U) != 0U) {
            /* Violated RMin. */
            idxFillColor = -line_idx_2 * in;
            if (((idxFillColor >= 0) && (i >= 0)) || ((idxFillColor < 0) && (i <
                  0))) {
              line_idx_1 += (div_s32_floor(idxFillColor << 1, i) + 1) >> 1;
            } else {
              line_idx_1 -= (div_s32_floor(-idxFillColor << 1, i) + 1) >> 1;
            }

            line_idx_2 = 0;
            visited = TRUE;
          } else if (((uint32_T)strLen & 8U) != 0U) {
            /* Violated RMax. */
            idxFillColor = (431 - line_idx_2) * in;
            if (((idxFillColor >= 0) && (i >= 0)) || ((idxFillColor < 0) && (i <
                  0))) {
              line_idx_1 += (div_s32_floor(idxFillColor << 1, i) + 1) >> 1;
            } else {
              line_idx_1 -= (div_s32_floor(-idxFillColor << 1, i) + 1) >> 1;
            }

            line_idx_2 = 431;
            visited = TRUE;
          } else if (((uint32_T)strLen & 1U) != 0U) {
            /* Violated CMin. */
            idxFillColor = -line_idx_1 * i;
            if (((idxFillColor >= 0) && (in >= 0)) || ((idxFillColor < 0) && (in
                  < 0))) {
              line_idx_2 += (div_s32_floor(idxFillColor << 1, in) + 1) >> 1;
            } else {
              line_idx_2 -= (div_s32_floor(-idxFillColor << 1, in) + 1) >> 1;
            }

            line_idx_1 = 0;
            visited = TRUE;
          } else {
            /* Violated CMax. */
            idxFillColor = (527 - line_idx_1) * i;
            if (((idxFillColor >= 0) && (in >= 0)) || ((idxFillColor < 0) && (in
                  < 0))) {
              line_idx_2 += (div_s32_floor(idxFillColor << 1, in) + 1) >> 1;
            } else {
              line_idx_2 -= (div_s32_floor(-idxFillColor << 1, in) + 1) >> 1;
            }

            line_idx_1 = 527;
            visited = TRUE;
          }
        } else {
          /* Clip the 2nd point. */
          if (visited_0) {
            line_idx_6 = indxColor;
            line_idx_3 = line_idx_0;
          }

          i = line_idx_6 - line_idx_2;
          in = line_idx_3 - line_idx_1;
          if (((uint32_T)strPtrIdx & 4U) != 0U) {
            /* Violated RMin. */
            idxFillColor = -line_idx_6 * in;
            if (((idxFillColor >= 0) && (i >= 0)) || ((idxFillColor < 0) && (i <
                  0))) {
              line_idx_3 += (div_s32_floor(idxFillColor << 1, i) + 1) >> 1;
            } else {
              line_idx_3 -= (div_s32_floor(-idxFillColor << 1, i) + 1) >> 1;
            }

            line_idx_6 = 0;
            visited_0 = TRUE;
          } else if (((uint32_T)strPtrIdx & 8U) != 0U) {
            /* Violated RMax. */
            idxFillColor = (431 - line_idx_6) * in;
            if (((idxFillColor >= 0) && (i >= 0)) || ((idxFillColor < 0) && (i <
                  0))) {
              line_idx_3 += (div_s32_floor(idxFillColor << 1, i) + 1) >> 1;
            } else {
              line_idx_3 -= (div_s32_floor(-idxFillColor << 1, i) + 1) >> 1;
            }

            line_idx_6 = 431;
            visited_0 = TRUE;
          } else if (((uint32_T)strPtrIdx & 1U) != 0U) {
            /* Violated CMin. */
            idxFillColor = -line_idx_3 * i;
            if (((idxFillColor >= 0) && (in >= 0)) || ((idxFillColor < 0) && (in
                  < 0))) {
              line_idx_6 += (div_s32_floor(idxFillColor << 1, in) + 1) >> 1;
            } else {
              line_idx_6 -= (div_s32_floor(-idxFillColor << 1, in) + 1) >> 1;
            }

            line_idx_3 = 0;
            visited_0 = TRUE;
          } else {
            /* Violated CMax. */
            idxFillColor = (527 - line_idx_3) * i;
            if (((idxFillColor >= 0) && (in >= 0)) || ((idxFillColor < 0) && (in
                  < 0))) {
              line_idx_6 += (div_s32_floor(idxFillColor << 1, in) + 1) >> 1;
            } else {
              line_idx_6 -= (div_s32_floor(-idxFillColor << 1, in) + 1) >> 1;
            }

            line_idx_3 = 527;
            visited_0 = TRUE;
          }
        }
      }

      if (isSizeValid) {
        i = line_idx_1 * 432 + line_idx_2;
        for (in = line_idx_1; in <= line_idx_3; in++) {
          vipcellcounting_win_DisplayResults_DrawMarkers_1[i] =
            vipcellcounting_win_P.DrawMarkers_RTP_FILLCOLOR[0];
          line_idx_1 = i + 228096;
          vipcellcounting_win_DisplayResults_DrawMarkers_1[line_idx_1] =
            vipcellcounting_win_P.DrawMarkers_RTP_FILLCOLOR[1];
          line_idx_1 += 228096;
          vipcellcounting_win_DisplayResults_DrawMarkers_1[line_idx_1] =
            vipcellcounting_win_P.DrawMarkers_RTP_FILLCOLOR[2];
          i += 432;
        }
      }

      line_idx_2 = indxColor - size;
      line_idx = opacityIndx - size;
      line_idx_6 = indxColor + size;
      line_idx_0 = opacityIndx + size;
      isSizeValid = FALSE;

      /* Find the visible portion of a line. */
      visited = FALSE;
      visited_0 = FALSE;
      done = FALSE;
      line_idx_4 = line_idx_2;
      line_idx_1 = line_idx;
      line_idx_5 = line_idx_6;
      line_idx_3 = line_idx_0;
      while (!done) {
        strLen = 0;
        strPtrIdx = 0;

        /* Determine viewport violations. */
        if (line_idx_4 < 0) {
          strLen = 4;
        } else {
          if (line_idx_4 > 431) {
            strLen = 8;
          }
        }

        if (line_idx_5 < 0) {
          strPtrIdx = 4;
        } else {
          if (line_idx_5 > 431) {
            strPtrIdx = 8;
          }
        }

        if (line_idx_1 < 0) {
          strLen = (int32_T)((uint32_T)strLen | 1U);
        } else {
          if (line_idx_1 > 527) {
            strLen = (int32_T)((uint32_T)strLen | 2U);
          }
        }

        if (line_idx_3 < 0) {
          strPtrIdx = (int32_T)((uint32_T)strPtrIdx | 1U);
        } else {
          if (line_idx_3 > 527) {
            strPtrIdx = (int32_T)((uint32_T)strPtrIdx | 2U);
          }
        }

        if (!(((uint32_T)strLen | (uint32_T)strPtrIdx) != 0U)) {
          /* Line falls completely within bounds. */
          done = TRUE;
          isSizeValid = TRUE;
        } else if (((uint32_T)strLen & (uint32_T)strPtrIdx) != 0U) {
          /* Line falls completely out of bounds. */
          done = TRUE;
          isSizeValid = FALSE;
        } else if ((uint32_T)strLen != 0U) {
          /* Clip 1st point; if it's in-bounds, clip 2nd point. */
          if (visited) {
            line_idx_4 = line_idx_2;
            line_idx_1 = line_idx;
          }

          i = line_idx_5 - line_idx_4;
          in = line_idx_3 - line_idx_1;
          if (((uint32_T)strLen & 4U) != 0U) {
            /* Violated RMin. */
            idxFillColor = -line_idx_4 * in;
            if (((idxFillColor >= 0) && (i >= 0)) || ((idxFillColor < 0) && (i <
                  0))) {
              line_idx_1 += (div_s32_floor(idxFillColor << 1, i) + 1) >> 1;
            } else {
              line_idx_1 -= (div_s32_floor(-idxFillColor << 1, i) + 1) >> 1;
            }

            line_idx_4 = 0;
            visited = TRUE;
          } else if (((uint32_T)strLen & 8U) != 0U) {
            /* Violated RMax. */
            idxFillColor = (431 - line_idx_4) * in;
            if (((idxFillColor >= 0) && (i >= 0)) || ((idxFillColor < 0) && (i <
                  0))) {
              line_idx_1 += (div_s32_floor(idxFillColor << 1, i) + 1) >> 1;
            } else {
              line_idx_1 -= (div_s32_floor(-idxFillColor << 1, i) + 1) >> 1;
            }

            line_idx_4 = 431;
            visited = TRUE;
          } else if (((uint32_T)strLen & 1U) != 0U) {
            /* Violated CMin. */
            idxFillColor = -line_idx_1 * i;
            if (((idxFillColor >= 0) && (in >= 0)) || ((idxFillColor < 0) && (in
                  < 0))) {
              line_idx_4 += (div_s32_floor(idxFillColor << 1, in) + 1) >> 1;
            } else {
              line_idx_4 -= (div_s32_floor(-idxFillColor << 1, in) + 1) >> 1;
            }

            line_idx_1 = 0;
            visited = TRUE;
          } else {
            /* Violated CMax. */
            idxFillColor = (527 - line_idx_1) * i;
            if (((idxFillColor >= 0) && (in >= 0)) || ((idxFillColor < 0) && (in
                  < 0))) {
              line_idx_4 += (div_s32_floor(idxFillColor << 1, in) + 1) >> 1;
            } else {
              line_idx_4 -= (div_s32_floor(-idxFillColor << 1, in) + 1) >> 1;
            }

            line_idx_1 = 527;
            visited = TRUE;
          }
        } else {
          /* Clip the 2nd point. */
          if (visited_0) {
            line_idx_5 = line_idx_6;
            line_idx_3 = line_idx_0;
          }

          i = line_idx_5 - line_idx_4;
          in = line_idx_3 - line_idx_1;
          if (((uint32_T)strPtrIdx & 4U) != 0U) {
            /* Violated RMin. */
            idxFillColor = -line_idx_5 * in;
            if (((idxFillColor >= 0) && (i >= 0)) || ((idxFillColor < 0) && (i <
                  0))) {
              line_idx_3 += (div_s32_floor(idxFillColor << 1, i) + 1) >> 1;
            } else {
              line_idx_3 -= (div_s32_floor(-idxFillColor << 1, i) + 1) >> 1;
            }

            line_idx_5 = 0;
            visited_0 = TRUE;
          } else if (((uint32_T)strPtrIdx & 8U) != 0U) {
            /* Violated RMax. */
            idxFillColor = (431 - line_idx_5) * in;
            if (((idxFillColor >= 0) && (i >= 0)) || ((idxFillColor < 0) && (i <
                  0))) {
              line_idx_3 += (div_s32_floor(idxFillColor << 1, i) + 1) >> 1;
            } else {
              line_idx_3 -= (div_s32_floor(-idxFillColor << 1, i) + 1) >> 1;
            }

            line_idx_5 = 431;
            visited_0 = TRUE;
          } else if (((uint32_T)strPtrIdx & 1U) != 0U) {
            /* Violated CMin. */
            idxFillColor = -line_idx_3 * i;
            if (((idxFillColor >= 0) && (in >= 0)) || ((idxFillColor < 0) && (in
                  < 0))) {
              line_idx_5 += (div_s32_floor(idxFillColor << 1, in) + 1) >> 1;
            } else {
              line_idx_5 -= (div_s32_floor(-idxFillColor << 1, in) + 1) >> 1;
            }

            line_idx_3 = 0;
            visited_0 = TRUE;
          } else {
            /* Violated CMax. */
            idxFillColor = (527 - line_idx_3) * i;
            if (((idxFillColor >= 0) && (in >= 0)) || ((idxFillColor < 0) && (in
                  < 0))) {
              line_idx_5 += (div_s32_floor(idxFillColor << 1, in) + 1) >> 1;
            } else {
              line_idx_5 -= (div_s32_floor(-idxFillColor << 1, in) + 1) >> 1;
            }

            line_idx_3 = 527;
            visited_0 = TRUE;
          }
        }
      }

      if (isSizeValid) {
        i = line_idx_1 * 432 + line_idx_4;
        for (in = line_idx_4; in <= line_idx_5; in++) {
          vipcellcounting_win_DisplayResults_DrawMarkers_1[i] =
            vipcellcounting_win_P.DrawMarkers_RTP_FILLCOLOR[0];
          line_idx_1 = i + 228096;
          vipcellcounting_win_DisplayResults_DrawMarkers_1[line_idx_1] =
            vipcellcounting_win_P.DrawMarkers_RTP_FILLCOLOR[1];
          line_idx_1 += 228096;
          vipcellcounting_win_DisplayResults_DrawMarkers_1[line_idx_1] =
            vipcellcounting_win_P.DrawMarkers_RTP_FILLCOLOR[2];
          i += 433;
        }
      }

      line_idx_2 = indxColor - size;
      line_idx = opacityIndx + size;
      line_idx_6 = indxColor + size;
      line_idx_0 = opacityIndx - size;
      isSizeValid = FALSE;

      /* Find the visible portion of a line. */
      visited = FALSE;
      visited_0 = FALSE;
      done = FALSE;
      line_idx_4 = line_idx_2;
      line_idx_1 = line_idx;
      line_idx_5 = line_idx_6;
      line_idx_3 = line_idx_0;
      while (!done) {
        strLen = 0;
        strPtrIdx = 0;

        /* Determine viewport violations. */
        if (line_idx_4 < 0) {
          strLen = 4;
        } else {
          if (line_idx_4 > 431) {
            strLen = 8;
          }
        }

        if (line_idx_5 < 0) {
          strPtrIdx = 4;
        } else {
          if (line_idx_5 > 431) {
            strPtrIdx = 8;
          }
        }

        if (line_idx_1 < 0) {
          strLen = (int32_T)((uint32_T)strLen | 1U);
        } else {
          if (line_idx_1 > 527) {
            strLen = (int32_T)((uint32_T)strLen | 2U);
          }
        }

        if (line_idx_3 < 0) {
          strPtrIdx = (int32_T)((uint32_T)strPtrIdx | 1U);
        } else {
          if (line_idx_3 > 527) {
            strPtrIdx = (int32_T)((uint32_T)strPtrIdx | 2U);
          }
        }

        if (!(((uint32_T)strLen | (uint32_T)strPtrIdx) != 0U)) {
          /* Line falls completely within bounds. */
          done = TRUE;
          isSizeValid = TRUE;
        } else if (((uint32_T)strLen & (uint32_T)strPtrIdx) != 0U) {
          /* Line falls completely out of bounds. */
          done = TRUE;
          isSizeValid = FALSE;
        } else if ((uint32_T)strLen != 0U) {
          /* Clip 1st point; if it's in-bounds, clip 2nd point. */
          if (visited) {
            line_idx_4 = line_idx_2;
            line_idx_1 = line_idx;
          }

          i = line_idx_5 - line_idx_4;
          in = line_idx_3 - line_idx_1;
          if (((uint32_T)strLen & 4U) != 0U) {
            /* Violated RMin. */
            idxFillColor = -line_idx_4 * in;
            if (((idxFillColor >= 0) && (i >= 0)) || ((idxFillColor < 0) && (i <
                  0))) {
              line_idx_1 += (div_s32_floor(idxFillColor << 1, i) + 1) >> 1;
            } else {
              line_idx_1 -= (div_s32_floor(-idxFillColor << 1, i) + 1) >> 1;
            }

            line_idx_4 = 0;
            visited = TRUE;
          } else if (((uint32_T)strLen & 8U) != 0U) {
            /* Violated RMax. */
            idxFillColor = (431 - line_idx_4) * in;
            if (((idxFillColor >= 0) && (i >= 0)) || ((idxFillColor < 0) && (i <
                  0))) {
              line_idx_1 += (div_s32_floor(idxFillColor << 1, i) + 1) >> 1;
            } else {
              line_idx_1 -= (div_s32_floor(-idxFillColor << 1, i) + 1) >> 1;
            }

            line_idx_4 = 431;
            visited = TRUE;
          } else if (((uint32_T)strLen & 1U) != 0U) {
            /* Violated CMin. */
            idxFillColor = -line_idx_1 * i;
            if (((idxFillColor >= 0) && (in >= 0)) || ((idxFillColor < 0) && (in
                  < 0))) {
              line_idx_4 += (div_s32_floor(idxFillColor << 1, in) + 1) >> 1;
            } else {
              line_idx_4 -= (div_s32_floor(-idxFillColor << 1, in) + 1) >> 1;
            }

            line_idx_1 = 0;
            visited = TRUE;
          } else {
            /* Violated CMax. */
            idxFillColor = (527 - line_idx_1) * i;
            if (((idxFillColor >= 0) && (in >= 0)) || ((idxFillColor < 0) && (in
                  < 0))) {
              line_idx_4 += (div_s32_floor(idxFillColor << 1, in) + 1) >> 1;
            } else {
              line_idx_4 -= (div_s32_floor(-idxFillColor << 1, in) + 1) >> 1;
            }

            line_idx_1 = 527;
            visited = TRUE;
          }
        } else {
          /* Clip the 2nd point. */
          if (visited_0) {
            line_idx_5 = line_idx_6;
            line_idx_3 = line_idx_0;
          }

          i = line_idx_5 - line_idx_4;
          in = line_idx_3 - line_idx_1;
          if (((uint32_T)strPtrIdx & 4U) != 0U) {
            /* Violated RMin. */
            idxFillColor = -line_idx_5 * in;
            if (((idxFillColor >= 0) && (i >= 0)) || ((idxFillColor < 0) && (i <
                  0))) {
              line_idx_3 += (div_s32_floor(idxFillColor << 1, i) + 1) >> 1;
            } else {
              line_idx_3 -= (div_s32_floor(-idxFillColor << 1, i) + 1) >> 1;
            }

            line_idx_5 = 0;
            visited_0 = TRUE;
          } else if (((uint32_T)strPtrIdx & 8U) != 0U) {
            /* Violated RMax. */
            idxFillColor = (431 - line_idx_5) * in;
            if (((idxFillColor >= 0) && (i >= 0)) || ((idxFillColor < 0) && (i <
                  0))) {
              line_idx_3 += (div_s32_floor(idxFillColor << 1, i) + 1) >> 1;
            } else {
              line_idx_3 -= (div_s32_floor(-idxFillColor << 1, i) + 1) >> 1;
            }

            line_idx_5 = 431;
            visited_0 = TRUE;
          } else if (((uint32_T)strPtrIdx & 1U) != 0U) {
            /* Violated CMin. */
            idxFillColor = -line_idx_3 * i;
            if (((idxFillColor >= 0) && (in >= 0)) || ((idxFillColor < 0) && (in
                  < 0))) {
              line_idx_5 += (div_s32_floor(idxFillColor << 1, in) + 1) >> 1;
            } else {
              line_idx_5 -= (div_s32_floor(-idxFillColor << 1, in) + 1) >> 1;
            }

            line_idx_3 = 0;
            visited_0 = TRUE;
          } else {
            /* Violated CMax. */
            idxFillColor = (527 - line_idx_3) * i;
            if (((idxFillColor >= 0) && (in >= 0)) || ((idxFillColor < 0) && (in
                  < 0))) {
              line_idx_5 += (div_s32_floor(idxFillColor << 1, in) + 1) >> 1;
            } else {
              line_idx_5 -= (div_s32_floor(-idxFillColor << 1, in) + 1) >> 1;
            }

            line_idx_3 = 527;
            visited_0 = TRUE;
          }
        }
      }

      if (isSizeValid) {
        i = line_idx_1 * 432 + line_idx_4;
        for (in = line_idx_4; in <= line_idx_5; in++) {
          vipcellcounting_win_DisplayResults_DrawMarkers_1[i] =
            vipcellcounting_win_P.DrawMarkers_RTP_FILLCOLOR[0];
          line_idx_1 = i + 228096;
          vipcellcounting_win_DisplayResults_DrawMarkers_1[line_idx_1] =
            vipcellcounting_win_P.DrawMarkers_RTP_FILLCOLOR[1];
          line_idx_1 += 228096;
          vipcellcounting_win_DisplayResults_DrawMarkers_1[line_idx_1] =
            vipcellcounting_win_P.DrawMarkers_RTP_FILLCOLOR[2];
          i -= 431;
        }
      }
    }
  }

  /* End of S-Function (svipdrawmarkers): '<S1>/Draw Markers' */

  /* Update for S-Function (svipwvo2): '<S1>/Results' */
  sErr = GetErrorBuffer(&vipcellcounting_win_DWork.Results_ToVideoDevice[0U]);
  LibUpdate_Video(&vipcellcounting_win_DWork.Results_ToVideoDevice[0U],
                  &vipcellcounting_win_DisplayResults_DrawMarkers_1[0U],
                  GetNullPointer(), GetNullPointer(), 528, 432);
  if (*sErr != 0) {
    rtmSetErrorStatus(vipcellcounting_win_M, sErr);
    rtmSetStopRequested(vipcellcounting_win_M, 1);
  }

  /* End of Update for S-Function (svipwvo2): '<S1>/Results' */
}

/* Model initialize function */
void vipcellcounting_win_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)vipcellcounting_win_M, 0,
                sizeof(RT_MODEL_vipcellcounting_win));

  /* block I/O */
  (void) memset(((void *) &vipcellcounting_win_B), 0,
                sizeof(BlockIO_vipcellcounting_win));

  /* exported global signals */
  vipcellcounting_win_DisplayResults_Probe_1[0] = 0.0;
  vipcellcounting_win_DisplayResults_Probe_1[1] = 0.0;
  vipcellcounting_win_DisplayResults_Selector1_1 = 0.0;

  {
    int_T i;
    for (i = 0; i < 228096; i++) {
      vipcellcounting_win_Video_1[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 228096; i++) {
      vipcellcounting_win_DisplayResults_InsertText1_1[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 228096; i++) {
      vipcellcounting_win_DisplayResults_InsertText2_1[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 684288; i++) {
      vipcellcounting_win_DisplayResults_ConverttoRGB_MatrixConcatenate_1[i] =
        0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 684288; i++) {
      vipcellcounting_win_DisplayResults_DrawMarkers_1[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 3000; i++) {
      vipcellcounting_win_Subsystem_CountCells_1[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 228096; i++) {
      vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Dilation_1[i] =
        0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 228096; i++) {
      vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Gain_1[i] = 0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 228096; i++) {
      vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Subtract_1[i] =
        0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 228096; i++) {
      vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Saturation_1[i] =
        0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 228096; i++) {
      vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Dilation1_1[i] =
        0.0F;
    }
  }

  {
    int_T i;
    for (i = 0; i < 228096; i++) {
      vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Subtract2_1[i] =
        0.0F;
    }
  }

  vipcellcounting_win_DisplayResults_DataTypeConversion_1 = 0U;
  vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_Output_1 = 0U;
  vipcellcounting_win_DisplayResults_Counter1_Bias_1 = 0U;
  vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_IncrementRealWorld_FixPtConstant_1
    = 0U;
  vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_IncrementRealWorld_FixPtSum1_1
    = 0U;
  vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_WrapToZero_Constant_1
    = 0U;
  vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_WrapToZero_FixPtSwitch_1
    = 0U;
  vipcellcounting_win_Video_2 = FALSE;
  vipcellcounting_win_DisplayResults_LogicalOperator_1 = FALSE;

  {
    int_T i;
    for (i = 0; i < 228096; i++) {
      vipcellcounting_win_SegmentCells_Subsystem_Autothreshold_1[i] = FALSE;
    }
  }

  /* states (dwork) */
  (void) memset((void *)&vipcellcounting_win_DWork, 0,
                sizeof(D_Work_vipcellcounting_win));

  {
    int_T i;
    for (i = 0; i < 133; i++) {
      vipcellcounting_win_DWork.Results_ToVideoDevice[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 10; i++) {
      vipcellcounting_win_DWork.Results_VideoInfo[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 133; i++) {
      vipcellcounting_win_DWork.Video_HostLib[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 5; i++) {
      vipcellcounting_win_DWork.Video_AudioInfo[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 10; i++) {
      vipcellcounting_win_DWork.Video_VideoInfo[i] = 0.0;
    }
  }

  vipcellcounting_win_DWork.InsertText1_colorVectorDW = 0.0F;
  vipcellcounting_win_DWork.InsertText2_colorVectorDW = 0.0F;

  /* exported global states */
  (void) memset(&vipcellcounting_win_Subsystem_CountCells_1Dim, 0,
                2U*sizeof(int32_T));

  {
    char_T *sErr;
    int32_T i;

    /* Start for S-Function (svipwvo2): '<S1>/Results' */
    sErr = GetErrorBuffer(&vipcellcounting_win_DWork.Results_ToVideoDevice[0U]);
    CreateHostLibrary("tovideodevice.dll",
                      &vipcellcounting_win_DWork.Results_ToVideoDevice[0U]);
    if (*sErr == 0) {
      createVideoInfo(&vipcellcounting_win_DWork.Results_VideoInfo[0U], 1U, 30.0,
                      30.0, "RGB ", 1, 3, 528, 432, 0U, 1, 1, NULL);
      LibCreate_Video(&vipcellcounting_win_DWork.Results_ToVideoDevice[0U], 0,
                      "vipcellcounting_win/Display\nResults/Results", "Results",
                      0U, 1, &vipcellcounting_win_DWork.Results_VideoInfo[0U],
                      1U, 115, 145, 0U, 528, 432, 0, 0U);
      LibStart(&vipcellcounting_win_DWork.Results_ToVideoDevice[0U]);
    }

    if (*sErr != 0) {
      DestroyHostLibrary(&vipcellcounting_win_DWork.Results_ToVideoDevice[0U]);
      if (*sErr != 0) {
        rtmSetErrorStatus(vipcellcounting_win_M, sErr);
        rtmSetStopRequested(vipcellcounting_win_M, 1);
      }
    }

    /* End of Start for S-Function (svipwvo2): '<S1>/Results' */

    /* Start for S-Function (sdspwmmfi2): '<Root>/Video' */
    sErr = GetErrorBuffer(&vipcellcounting_win_DWork.Video_HostLib[0U]);
    CreateHostLibrary("frommmfile.dll",
                      &vipcellcounting_win_DWork.Video_HostLib[0U]);
    createAudioInfo(&vipcellcounting_win_DWork.Video_AudioInfo[0U], 0U, 0U, 0.0,
                    0, 0, 0, 0, 0);
    createVideoInfo(&vipcellcounting_win_DWork.Video_VideoInfo[0U], 1U,
                    30.00003000003, 30.00003000003, "RGB ", 1, 1, 528, 432, 0U,
                    1, 1, 0);
    if (*sErr == 0) {
      LibCreate_FromMMFile(&vipcellcounting_win_DWork.Video_HostLib[0U], 0,
                           "C:\\Program Files\\MATLAB\\R2012a\\toolbox\\vision\\visiondemos\\ecolicells.avi",
                           "", &vipcellcounting_win_DWork.Video_AudioInfo[0U],
                           &vipcellcounting_win_DWork.Video_VideoInfo[0U], 0U,
                           1U, 2U, 0U, 0U);
      LibStart(&vipcellcounting_win_DWork.Video_HostLib[0U]);
    }

    if (*sErr != 0) {
      DestroyHostLibrary(&vipcellcounting_win_DWork.Video_HostLib[0U]);
      if (*sErr != 0) {
        rtmSetErrorStatus(vipcellcounting_win_M, sErr);
        rtmSetStopRequested(vipcellcounting_win_M, 1);
      }
    }

    /* End of Start for S-Function (sdspwmmfi2): '<Root>/Video' */

    /* Start for Enabled SubSystem: '<S1>/Counter1' */
    /* InitializeConditions for UnitDelay: '<S6>/Output' */
    vipcellcounting_win_DWork.Output_DSTATE = vipcellcounting_win_P.Output_X0;

    /* End of Start for SubSystem: '<S1>/Counter1' */

    /* Start for Atomic SubSystem: '<S2>/Subsystem1' */
    vipcellcountin_Subsystem1_Start();

    /* End of Start for SubSystem: '<S2>/Subsystem1' */

    /* Start for S-Function (sviptextrender): '<S1>/Insert Text1' */
    for (i = 0; i < 19; i++) {
      vipcellcounting_win_DWork.InsertText1_StringDataDW[i] =
        vipcellcounting_win_ConstP.InsertText1_StrArray[i];
    }

    /* End of Start for S-Function (sviptextrender): '<S1>/Insert Text1' */

    /* Start for S-Function (sviptextrender): '<S1>/Insert Text2' */
    memcpy(&vipcellcounting_win_DWork.InsertText2_StringDataDW[0],
           vipcellcounting_win_ConstP.InsertText2_StrArray, 92U * sizeof(uint8_T));
  }

  /* InitializeConditions for S-Function (sdspwmmfi2): '<Root>/Video' */
  vipcellcounting_win_Video_2 = FALSE;
  LibReset(&vipcellcounting_win_DWork.Video_HostLib[0U]);
}

/* Model terminate function */
void vipcellcounting_win_terminate(void)
{
  char_T *sErr;

  /* Terminate for S-Function (svipwvo2): '<S1>/Results' */
  sErr = GetErrorBuffer(&vipcellcounting_win_DWork.Results_ToVideoDevice[0U]);
  LibTerminate(&vipcellcounting_win_DWork.Results_ToVideoDevice[0U]);
  if (*sErr != 0) {
    rtmSetErrorStatus(vipcellcounting_win_M, sErr);
    rtmSetStopRequested(vipcellcounting_win_M, 1);
  }

  LibDestroy(&vipcellcounting_win_DWork.Results_ToVideoDevice[0U], 0);
  DestroyHostLibrary(&vipcellcounting_win_DWork.Results_ToVideoDevice[0U]);

  /* End of Terminate for S-Function (svipwvo2): '<S1>/Results' */

  /* Terminate for S-Function (sdspwmmfi2): '<Root>/Video' */
  sErr = GetErrorBuffer(&vipcellcounting_win_DWork.Video_HostLib[0U]);
  LibTerminate(&vipcellcounting_win_DWork.Video_HostLib[0U]);
  if (*sErr != 0) {
    rtmSetErrorStatus(vipcellcounting_win_M, sErr);
    rtmSetStopRequested(vipcellcounting_win_M, 1);
  }

  LibDestroy(&vipcellcounting_win_DWork.Video_HostLib[0U], 0);
  DestroyHostLibrary(&vipcellcounting_win_DWork.Video_HostLib[0U]);

  /* End of Terminate for S-Function (sdspwmmfi2): '<Root>/Video' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
