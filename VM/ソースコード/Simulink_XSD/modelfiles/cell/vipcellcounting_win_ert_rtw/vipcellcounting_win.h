/*
 * File: vipcellcounting_win.h
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

#ifndef RTW_HEADER_vipcellcounting_win_h_
#define RTW_HEADER_vipcellcounting_win_h_
#ifndef vipcellcounting_win_COMMON_INCLUDES_
# define vipcellcounting_win_COMMON_INCLUDES_
#include <math.h>
#include <string.h>
#include "rtwtypes.h"
#include "HostLib_Video.h"
#include "HostLib_Multimedia.h"
#include "HostLib_MMFile.h"
#include "vipdrawtext_rt.h"
#include "rt_nonfinite.h"
#include "rtGetInf.h"
#include "rtGetNaN.h"
#endif                                 /* vipcellcounting_win_COMMON_INCLUDES_ */

#include "vipcellcounting_win_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
# define rtmSetStopRequested(rtm, val) ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
# define rtmGetStopRequestedPtr(rtm)   (&((rtm)->Timing.stopRequestedFlag))
#endif

/* Block signals (auto storage) */
typedef struct {
  uint16_T TmpSignalConversionAtInsertText[2];
} BlockIO_vipcellcounting_win;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real32_T Dilation_ONE_PAD_IMG_DW[240745];/* '<S11>/Dilation' */
  real32_T Dilation_TWO_PAD_IMG_DW[240745];/* '<S11>/Dilation' */
  real32_T Dilation1_ONE_PAD_IMG_DW[240745];/* '<S11>/Dilation1' */
  real32_T Dilation1_TWO_PAD_IMG_DW[240745];/* '<S11>/Dilation1' */
  uint32_T CountCells_STACK_DW[228096];/* '<S3>/Count Cells' */
  uint16_T CountCells_PAD_DW[230020];  /* '<S3>/Count Cells' */
  int16_T CountCells_N_PIXLIST_DW[228096];/* '<S3>/Count Cells' */
  int16_T CountCells_M_PIXLIST_DW[228096];/* '<S3>/Count Cells' */
  uint32_T CountCells_NUM_PIX_DW[1500];/* '<S3>/Count Cells' */
  real_T Results_ToVideoDevice[133];   /* '<S1>/Results' */
  real_T Results_VideoInfo[10];        /* '<S1>/Results' */
  real_T Video_HostLib[133];           /* '<Root>/Video' */
  real_T Video_AudioInfo[5];           /* '<Root>/Video' */
  real_T Video_VideoInfo[10];          /* '<Root>/Video' */
  real32_T InsertText1_colorVectorDW;  /* '<S1>/Insert Text1' */
  real32_T InsertText2_colorVectorDW;  /* '<S1>/Insert Text2' */
  int32_T InsertText1_TxtLoc[2];       /* '<S1>/Insert Text1' */
  int32_T InsertText2_TxtLoc[2];       /* '<S1>/Insert Text2' */
  int32_T Dilation_NUMNONZ_DW[2];      /* '<S11>/Dilation' */
  int32_T Dilation_STREL_DW[2];        /* '<S11>/Dilation' */
  int32_T Dilation_DILATE_OFF_DW[14];  /* '<S11>/Dilation' */
  int32_T Dilation1_NUMNONZ_DW[2];     /* '<S11>/Dilation1' */
  int32_T Dilation1_STREL_DW[2];       /* '<S11>/Dilation1' */
  int32_T Dilation1_DILATE_OFF_DW[14]; /* '<S11>/Dilation1' */
  uint32_T InsertText1_TxtSelctn;      /* '<S1>/Insert Text1' */
  uint32_T InsertText2_TxtSelctn;      /* '<S1>/Insert Text2' */
  uint16_T Output_DSTATE;              /* '<S6>/Output' */
  uint16_T InsertText1_VarHolder[3];   /* '<S1>/Insert Text1' */
  uint8_T Results_ID_Dwork[1025];      /* '<S1>/Results' */
  uint8_T InsertText1_RepBuf[56];      /* '<S1>/Insert Text1' */
  uint8_T InsertText1_StringDataDW[19];/* '<S1>/Insert Text1' */
  uint8_T InsertText2_RepBuf;          /* '<S1>/Insert Text2' */
  uint8_T InsertText2_VarHolder;       /* '<S1>/Insert Text2' */
  uint8_T InsertText2_StringDataDW[92];/* '<S1>/Insert Text2' */
  boolean_T Counter1_MODE;             /* '<S1>/Counter1' */
} D_Work_vipcellcounting_win;

/* Constant parameters (auto storage) */
typedef struct {
  /* Computed Parameter: CountCells_WALKER_RTP
   * Referenced by: '<S3>/Count Cells'
   */
  int32_T CountCells_WALKER_RTP[8];

  /* Computed Parameter: InsertText1_GlyLBea
   * Referenced by: '<S1>/Insert Text1'
   */
  int32_T InsertText1_GlyLBea[256];

  /* Computed Parameter: InsertText1_GlyTBea
   * Referenced by: '<S1>/Insert Text1'
   */
  int32_T InsertText1_GlyTBea[256];

  /* Computed Parameter: InsertText2_GlyLBea
   * Referenced by: '<S1>/Insert Text2'
   */
  int32_T InsertText2_GlyLBea[256];

  /* Computed Parameter: InsertText2_GlyTBea
   * Referenced by: '<S1>/Insert Text2'
   */
  int32_T InsertText2_GlyTBea[256];

  /* Computed Parameter: InsertText1_GlyInde
   * Referenced by: '<S1>/Insert Text1'
   */
  uint32_T InsertText1_GlyInde[256];

  /* Computed Parameter: InsertText2_GlyInde
   * Referenced by: '<S1>/Insert Text2'
   */
  uint32_T InsertText2_GlyInde[256];

  /* Computed Parameter: InsertText1_GlyWidt
   * Referenced by: '<S1>/Insert Text1'
   */
  uint16_T InsertText1_GlyWidt[256];

  /* Computed Parameter: InsertText1_GlyHeig
   * Referenced by: '<S1>/Insert Text1'
   */
  uint16_T InsertText1_GlyHeig[256];

  /* Computed Parameter: InsertText1_GlyXAdv
   * Referenced by: '<S1>/Insert Text1'
   */
  uint16_T InsertText1_GlyXAdv[256];

  /* Computed Parameter: InsertText2_GlyWidt
   * Referenced by: '<S1>/Insert Text2'
   */
  uint16_T InsertText2_GlyWidt[256];

  /* Computed Parameter: InsertText2_GlyHeig
   * Referenced by: '<S1>/Insert Text2'
   */
  uint16_T InsertText2_GlyHeig[256];

  /* Computed Parameter: InsertText2_GlyXAdv
   * Referenced by: '<S1>/Insert Text2'
   */
  uint16_T InsertText2_GlyXAdv[256];

  /* Computed Parameter: InsertText1_StrArray
   * Referenced by: '<S1>/Insert Text1'
   */
  uint8_T InsertText1_StrArray[19];

  /* Computed Parameter: InsertText1_GlyDat
   * Referenced by: '<S1>/Insert Text1'
   */
  uint8_T InsertText1_GlyDat[2929];

  /* Computed Parameter: InsertText2_StrArray
   * Referenced by: '<S1>/Insert Text2'
   */
  uint8_T InsertText2_StrArray[92];

  /* Computed Parameter: InsertText2_GlyDat
   * Referenced by: '<S1>/Insert Text2'
   */
  uint8_T InsertText2_GlyDat[1487];
} ConstParam_vipcellcounting_win;

/* Parameters (auto storage) */
struct Parameters_vipcellcounting_win_ {
  real_T Results_P1;                   /* Expression: openAtMdlStart
                                        * Referenced by: '<S1>/Results'
                                        */
  real_T Results_P2;                   /* Expression: inputColorFormat
                                        * Referenced by: '<S1>/Results'
                                        */
  real32_T Autothreshold_FACTOR_RTP;   /* Expression: scaleFactor
                                        * Referenced by: '<S9>/Autothreshold'
                                        */
  real32_T Gain_Gain;                  /* Computed Parameter: Gain_Gain
                                        * Referenced by: '<S11>/Gain'
                                        */
  real32_T Saturation_UpperSat;        /* Computed Parameter: Saturation_UpperSat
                                        * Referenced by: '<S11>/Saturation'
                                        */
  real32_T Saturation_LowerSat;        /* Computed Parameter: Saturation_LowerSat
                                        * Referenced by: '<S11>/Saturation'
                                        */
  real32_T InsertText1_colorRTP;       /* Computed Parameter: InsertText1_colorRTP
                                        * Referenced by: '<S1>/Insert Text1'
                                        */
  real32_T InsertText1_TxtOpacity;     /* Computed Parameter: InsertText1_TxtOpacity
                                        * Referenced by: '<S1>/Insert Text1'
                                        */
  real32_T InsertText2_colorRTP;       /* Computed Parameter: InsertText2_colorRTP
                                        * Referenced by: '<S1>/Insert Text2'
                                        */
  real32_T InsertText2_TxtOpacity;     /* Computed Parameter: InsertText2_TxtOpacity
                                        * Referenced by: '<S1>/Insert Text2'
                                        */
  real32_T DrawMarkers_RTP_FILLCOLOR[3];/* Computed Parameter: DrawMarkers_RTP_FILLCOLOR
                                         * Referenced by: '<S1>/Draw Markers'
                                         */
  real32_T DrawMarkers_RTP_SIZE;       /* Computed Parameter: DrawMarkers_RTP_SIZE
                                        * Referenced by: '<S1>/Draw Markers'
                                        */
  int32_T InsertText1_TxtLocation[2];  /* Computed Parameter: InsertText1_TxtLocation
                                        * Referenced by: '<S1>/Insert Text1'
                                        */
  int32_T InsertText2_TxtLocation[2];  /* Computed Parameter: InsertText2_TxtLocation
                                        * Referenced by: '<S1>/Insert Text2'
                                        */
  uint32_T CountCells_MINAREA_RTP;     /* Computed Parameter: CountCells_MINAREA_RTP
                                        * Referenced by: '<S3>/Count Cells'
                                        */
  uint32_T CountCells_MAXAREA_RTP;     /* Computed Parameter: CountCells_MAXAREA_RTP
                                        * Referenced by: '<S3>/Count Cells'
                                        */
  uint16_T Output_X0;                  /* Computed Parameter: Output_X0
                                        * Referenced by: '<S6>/Output'
                                        */
  uint16_T Bias_Bias;                  /* Computed Parameter: Bias_Bias
                                        * Referenced by: '<S5>/Bias'
                                        */
  uint16_T FixPtConstant_Value;        /* Computed Parameter: FixPtConstant_Value
                                        * Referenced by: '<S7>/FixPt Constant'
                                        */
  uint16_T Constant_Value;             /* Computed Parameter: Constant_Value
                                        * Referenced by: '<S8>/Constant'
                                        */
  uint16_T FixPtSwitch_Threshold;      /* Computed Parameter: FixPtSwitch_Threshold
                                        * Referenced by: '<S8>/FixPt Switch'
                                        */
};

/* Real-time Model Data Structure */
struct RT_MODEL_vipcellcounting_win {
  const char_T * volatile errorStatus;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    boolean_T stopRequestedFlag;
  } Timing;
};

/* Block parameters (auto storage) */
extern Parameters_vipcellcounting_win vipcellcounting_win_P;

/* Block signals (auto storage) */
extern BlockIO_vipcellcounting_win vipcellcounting_win_B;

/* Block states (auto storage) */
extern D_Work_vipcellcounting_win vipcellcounting_win_DWork;

/* Constant parameters (auto storage) */
extern const ConstParam_vipcellcounting_win vipcellcounting_win_ConstP;

/*
 * Exported Global Signals
 *
 * Note: Exported global signals are block signals with an exported global
 * storage class designation.  Code generation will declare the memory for
 * these signals and export their symbols.
 *
 */
extern real_T vipcellcounting_win_DisplayResults_Probe_1[2];/* '<S1>/Probe' */
extern real_T vipcellcounting_win_DisplayResults_Selector1_1;/* '<S1>/Selector1' */
extern real32_T vipcellcounting_win_Video_1[228096];/* '<Root>/Video' */
extern real32_T vipcellcounting_win_DisplayResults_InsertText1_1[228096];/* '<S1>/Insert Text1' */
extern real32_T vipcellcounting_win_DisplayResults_InsertText2_1[228096];/* '<S1>/Insert Text2' */
extern real32_T
  vipcellcounting_win_DisplayResults_ConverttoRGB_MatrixConcatenate_1[684288];/* '<S4>/Matrix Concatenate' */
extern real32_T vipcellcounting_win_DisplayResults_DrawMarkers_1[684288];/* '<S1>/Draw Markers' */
extern real32_T vipcellcounting_win_Subsystem_CountCells_1[3000];/* '<S3>/Count Cells' */
extern real32_T
  vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Dilation_1[228096];/* '<S11>/Dilation' */
extern real32_T vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Gain_1
  [228096];                            /* '<S11>/Gain' */
extern real32_T
  vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Subtract_1[228096];/* '<S11>/Subtract' */
extern real32_T
  vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Saturation_1[228096];/* '<S11>/Saturation' */
extern real32_T
  vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Dilation1_1[228096];/* '<S11>/Dilation1' */
extern real32_T
  vipcellcounting_win_SegmentCells_Subsystem1_IsolateCells_Subtract2_1[228096];/* '<S11>/Subtract2' */
extern uint16_T vipcellcounting_win_DisplayResults_DataTypeConversion_1;/* '<S1>/Data Type Conversion' */
extern uint16_T
  vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_Output_1;/* '<S6>/Output' */
extern uint16_T vipcellcounting_win_DisplayResults_Counter1_Bias_1;/* '<S5>/Bias' */
extern uint16_T
  vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_IncrementRealWorld_FixPtConstant_1;/* '<S7>/FixPt Constant' */
extern uint16_T
  vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_IncrementRealWorld_FixPtSum1_1;/* '<S7>/FixPt Sum1' */
extern uint16_T
  vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_WrapToZero_Constant_1;/* '<S8>/Constant' */
extern uint16_T
  vipcellcounting_win_DisplayResults_Counter1_CounterFreeRunning_WrapToZero_FixPtSwitch_1;/* '<S8>/FixPt Switch' */
extern boolean_T vipcellcounting_win_Video_2;/* '<Root>/Video' */
extern boolean_T vipcellcounting_win_DisplayResults_LogicalOperator_1;/* '<S1>/Logical Operator' */
extern boolean_T vipcellcounting_win_SegmentCells_Subsystem_Autothreshold_1
  [228096];                            /* '<S9>/Autothreshold' */

/*
 * Exported States
 *
 * Note: Exported states are block states with an exported global
 * storage class designation.  Code generation will declare the memory for these
 * states and exports their symbols.
 *
 */
extern int32_T vipcellcounting_win_Subsystem_CountCells_1Dim[2];/* '<S3>/Count Cells' */

/* Model entry point functions */
extern void vipcellcounting_win_initialize(void);
extern void vipcellcounting_win_step(void);
extern void vipcellcounting_win_terminate(void);

/* Real-time Model object */
extern struct RT_MODEL_vipcellcounting_win *const vipcellcounting_win_M;

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
 * '<Root>' : 'vipcellcounting_win'
 * '<S1>'   : 'vipcellcounting_win/Display Results'
 * '<S2>'   : 'vipcellcounting_win/Segment Cells'
 * '<S3>'   : 'vipcellcounting_win/Subsystem'
 * '<S4>'   : 'vipcellcounting_win/Display Results/Convert to RGB'
 * '<S5>'   : 'vipcellcounting_win/Display Results/Counter1'
 * '<S6>'   : 'vipcellcounting_win/Display Results/Counter1/Counter Free-Running'
 * '<S7>'   : 'vipcellcounting_win/Display Results/Counter1/Counter Free-Running/Increment Real World'
 * '<S8>'   : 'vipcellcounting_win/Display Results/Counter1/Counter Free-Running/Wrap To Zero'
 * '<S9>'   : 'vipcellcounting_win/Segment Cells/Subsystem'
 * '<S10>'  : 'vipcellcounting_win/Segment Cells/Subsystem1'
 * '<S11>'  : 'vipcellcounting_win/Segment Cells/Subsystem1/Isolate Cells'
 */
#endif                                 /* RTW_HEADER_vipcellcounting_win_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
