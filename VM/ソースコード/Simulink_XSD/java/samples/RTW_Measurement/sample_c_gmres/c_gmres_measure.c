/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: c_gmres.c
 *
 * Code generated for Simulink model 'c_gmres'.
 *
 * Model version                  : 1.71
 * Simulink Coder version         : 8.8 (R2015a) 09-Feb-2015
 * C/C++ source code generated on : Mon Jun 22 14:30:29 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "c_gmres.h"
#include "c_gmres_private.h"

/* user include files */
#include "c_gmres_measure.h"
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
real_T c_gmres_Discrete_TimeIntegrator_1[40];/* '<Root>/Discrete_TimeIntegrator' */
real_T c_gmres_Selector_1[2];          /* '<Root>/Selector' */
real_T c_gmres_plant_Model1_Gain3_1[2];/* '<S148>/Gain3' */
real_T c_gmres_plant_UnitDelay2_1[2];  /* '<S5>/UnitDelay2' */
real_T c_gmres_plant_Model1_Saturation_1[2];/* '<S148>/Saturation' */
real_T c_gmres_plant_Model1_Sqrt_1[2]; /* '<S148>/Sqrt' */
real_T c_gmres_plant_Model1_Gain_1;    /* '<S148>/Gain' */
real_T c_gmres_plant_Model1_Gain1_1;   /* '<S148>/Gain1' */
real_T c_gmres_plant_Model1_Gain2_1;   /* '<S148>/Gain2' */
real_T c_gmres_plant_Model1_Add_1;     /* '<S148>/Add' */
real_T c_gmres_plant_Model1_Add1_1[2]; /* '<S148>/Add1' */
real_T c_gmres_plant_Gain_1[2];        /* '<S5>/Gain' */
real_T c_gmres_plant_Add_1[2];         /* '<S5>/Add' */
real_T c_gmres_Constant2_1;            /* '<Root>/Constant2' */
real_T c_gmres_SliderGain_SliderGain_1;/* '<S1>/SliderGain' */
real_T c_gmres_SliderGain1_SliderGain_1;/* '<S2>/SliderGain' */
real_T c_gmres_Constant1_1;            /* '<Root>/Constant1' */
real_T c_gmres_DigitalClock_1;         /* '<Root>/DigitalClock' */
real_T c_gmres_UnitDelay1_1[2];        /* '<Root>/UnitDelay1' */
real_T c_gmres_Subtract_1[2];          /* '<Root>/Subtract' */
real_T c_gmres_Gain_1[2];              /* '<Root>/Gain' */
real_T c_gmres_Subsystem_UnitDelay_1[40];/* '<S3>/UnitDelay' */
real_T c_gmres_VectorConcatenate_1[2]; /* '<Root>/VectorConcatenate' */
real_T c_gmres_Subsystem_VectorConcatenate1_1[42];/* '<S3>/VectorConcatenate1' */
real_T c_gmres_Subsystem_Selector1_1[40];/* '<S3>/Selector1' */
real_T c_gmres_UnitDelay2_1[40];       /* '<Root>/UnitDelay2' */
real_T c_gmres_fdgmres_Initialize_model_q_matrix_1[160];/* '<S8>/q_matrix' */
real_T c_gmres_fdgmres_calculation_tau_Constant4_1;/* '<S10>/Constant4' */
real_T c_gmres_fdgmres_calculation_tau_Gain1_1;/* '<S10>/Gain1' */
real_T c_gmres_fdgmres_calculation_tau_Constant3_1;/* '<S10>/Constant3' */
real_T c_gmres_fdgmres_calculation_tau_Subtract1_1;/* '<S10>/Subtract1' */
real_T c_gmres_fdgmres_calculation_tau_MathFunction_1;/* '<S10>/MathFunction' */
real_T c_gmres_fdgmres_calculation_tau_Subtract2_1;/* '<S10>/Subtract2' */
real_T c_gmres_fdgmres_calculation_tau_Gain2_1;/* '<S10>/Gain2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_VectorConcatenate_1
  [42];                                /* '<S49>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Selector_1[2];/* '<S49>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Selector1_1[2];/* '<S49>/Selector1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Subtract_1
  [2];                                 /* '<S52>/Subtract' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Constant_1;/* '<S52>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Product_1;/* '<S52>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Constant1_1;/* '<S52>/Constant1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Product1_1;/* '<S52>/Product1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Add_1;/* '<S52>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Constant2_1;/* '<S52>/Constant2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Product2_1;/* '<S52>/Product2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Constant3_1;/* '<S52>/Constant3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Product3_1;/* '<S52>/Product3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Add1_1;/* '<S52>/Add1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_VectorConcatenate_1
  [2];                                 /* '<S52>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_VectorConcatenate1_1
  [40];                                /* '<S49>/VectorConcatenate1' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Constant_1;/* '<S16>/Constant' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Divide1_1[40];/* '<S16>/Divide1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_VectorConcatenate_1
  [42];                                /* '<S66>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Selector_1
  [2];                                 /* '<S66>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Selector1_1
  [2];                                 /* '<S66>/Selector1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Subtract_1
  [2];                                 /* '<S69>/Subtract' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Constant_1;/* '<S69>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Product_1;/* '<S69>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Constant1_1;/* '<S69>/Constant1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Product1_1;/* '<S69>/Product1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Add_1;/* '<S69>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Constant2_1;/* '<S69>/Constant2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Product2_1;/* '<S69>/Product2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Constant3_1;/* '<S69>/Constant3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Product3_1;/* '<S69>/Product3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Add1_1;/* '<S69>/Add1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_VectorConcatenate_1
  [2];                                 /* '<S69>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_VectorConcatenate1_1
  [40];                                /* '<S66>/VectorConcatenate1' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_zero_1[40];/* '<S12>/zero' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Product_1[40];/* '<S50>/Product' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Sum_1
  [40];                                /* '<S50>/Sum' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Product1_1[2];/* '<S50>/Product1' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Sum1_1
  [2];                                 /* '<S50>/Sum1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_VectorConcatenate_1
  [42];                                /* '<S67>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Selector_1
  [2];                                 /* '<S67>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Selector1_1
  [2];                                 /* '<S67>/Selector1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Subtract_1
  [2];                                 /* '<S84>/Subtract' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Constant_1;/* '<S84>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Product_1;/* '<S84>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Constant1_1;/* '<S84>/Constant1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Product1_1;/* '<S84>/Product1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Add_1;/* '<S84>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Constant2_1;/* '<S84>/Constant2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Product2_1;/* '<S84>/Product2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Constant3_1;/* '<S84>/Constant3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Product3_1;/* '<S84>/Product3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Add1_1;/* '<S84>/Add1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_VectorConcatenate_1
  [2];                                 /* '<S84>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_VectorConcatenate1_1
  [40];                                /* '<S67>/VectorConcatenate1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Subtract_1
  [40];                                /* '<S50>/Subtract' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Divide_1[40];/* '<S50>/Divide' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Subtract_1[40];/* '<S16>/Subtract' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subsystem_Product_1[2];/* '<S15>/Product' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subsystem_Add_1
  [2];                                 /* '<S15>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_VectorConcatenate_1
  [42];                                /* '<S17>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Selector_1
  [2];                                 /* '<S17>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Selector1_1
  [2];                                 /* '<S17>/Selector1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Subtract_1
  [2];                                 /* '<S20>/Subtract' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Constant_1;/* '<S20>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Product_1;/* '<S20>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Constant1_1;/* '<S20>/Constant1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Product1_1;/* '<S20>/Product1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Add_1;/* '<S20>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Constant2_1;/* '<S20>/Constant2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Product2_1;/* '<S20>/Product2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Constant3_1;/* '<S20>/Constant3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Product3_1;/* '<S20>/Product3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Add1_1;/* '<S20>/Add1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_VectorConcatenate_1
  [2];                                 /* '<S20>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_VectorConcatenate1_1
  [40];                                /* '<S17>/VectorConcatenate1' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Product_1
  [40];                                /* '<S14>/Product' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Sum_1[40];/* '<S14>/Sum' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_zero1_1[2];/* '<S12>/zero1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Product1_1[2];/* '<S14>/Product1' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Sum1_1[2];/* '<S14>/Sum1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_VectorConcatenate_1
  [42];                                /* '<S18>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Selector_1
  [2];                                 /* '<S18>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Selector1_1
  [2];                                 /* '<S18>/Selector1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Subtract_1
  [2];                                 /* '<S35>/Subtract' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Constant_1;/* '<S35>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Product_1;/* '<S35>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Constant1_1;/* '<S35>/Constant1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Product1_1;/* '<S35>/Product1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Add_1;/* '<S35>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Constant2_1;/* '<S35>/Constant2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Product2_1;/* '<S35>/Product2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Constant3_1;/* '<S35>/Constant3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Product3_1;/* '<S35>/Product3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Add1_1;/* '<S35>/Add1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_VectorConcatenate_1
  [2];                                 /* '<S35>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_VectorConcatenate1_1
  [40];                                /* '<S18>/VectorConcatenate1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Subtract_1[40];/* '<S14>/Subtract' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Divide_1
  [40];                                /* '<S14>/Divide' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subtract_1[40];/* '<S12>/Subtract' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Reshape_1[40];/* '<S12>/Reshape' */
real_T c_gmres_fdgmres_Initialize_model_two_norm_DotProduct_1;/* '<S13>/DotProduct' */
real_T c_gmres_fdgmres_Initialize_model_two_norm_Sqrt_1;/* '<S13>/Sqrt' */
real_T c_gmres_fdgmres_Initialize_model_Divide_1[40];/* '<S8>/Divide' */
real_T c_gmres_fdgmres_Initialize_model_Assignment_1[160];/* '<S8>/Assignment' */
real_T c_gmres_fdgmres_Initialize_model_b_hat_init_1[4];/* '<S8>/b_hat_init' */
real_T c_gmres_fdgmres_Initialize_model_Product_1[4];/* '<S8>/Product' */
real_T c_gmres_fdgmres_Initialize_model_Reshape1_1[4];/* '<S8>/Reshape1' */
real_T c_gmres_fdgmres_Reshape_1[40];  /* '<S4>/Reshape' */
real_T c_gmres_fdgmres_Add_1[40];      /* '<S4>/Add' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Add_1;/* '<S12>/Add' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_zero2_1;/* '<S12>/zero2' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Product2_1;/* '<S14>/Product2' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Sum2_1;/* '<S14>/Sum2' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_one_1;/* '<S12>/one' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Product2_1;/* '<S50>/Product2' */
real_T c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Sum2_1;/* '<S50>/Sum2' */
real_T c_gmres_fdgmres_Iterativvelyminimizeresidualvector_one_1;/* '<S9>/one' */
real_T c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Add_1;/* '<S9>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_h_matrix_1
  [12];                                /* '<S98>/h_matrix' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Subsystem_Product_1
  [2];                                 /* '<S108>/Product' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Subsystem_Add_1
  [2];                                 /* '<S108>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_VectorConcatenate_1
  [42];                                /* '<S109>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Selector_1
  [2];                                 /* '<S109>/Selector' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Selector1_1
  [2];                                 /* '<S109>/Selector1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Subtract_1
  [2];                                 /* '<S112>/Subtract' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Constant_1;/* '<S112>/Constant' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Product_1;/* '<S112>/Product' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Constant1_1;/* '<S112>/Constant1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Product1_1;/* '<S112>/Product1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Add_1;/* '<S112>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Constant2_1;/* '<S112>/Constant2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Product2_1;/* '<S112>/Product2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Constant3_1;/* '<S112>/Constant3' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Product3_1;/* '<S112>/Product3' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Add1_1;/* '<S112>/Add1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_VectorConcatenate_1
  [2];                                 /* '<S112>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_VectorConcatenate1_1
  [40];                                /* '<S109>/VectorConcatenate1' */
real_T c_gmres_fdgmres_Iterativvelyminimizeresidualvector_q_matrix_1[160];/* '<S9>/q_matrix' */
real_T c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Switch_1[160];/* '<S9>/Switch' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Selector_1
  [40];                                /* '<S98>/Selector' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Reshape1_1
  [40];                                /* '<S104>/Reshape1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Product_1
  [40];                                /* '<S107>/Product' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Sum_1
  [40];                                /* '<S107>/Sum' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_zero1_1
  [2];                                 /* '<S104>/zero1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Product1_1
  [2];                                 /* '<S107>/Product1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Sum1_1
  [2];                                 /* '<S107>/Sum1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_VectorConcatenate_1
  [42];                                /* '<S110>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Selector_1
  [2];                                 /* '<S110>/Selector' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Selector1_1
  [2];                                 /* '<S110>/Selector1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Subtract_1
  [2];                                 /* '<S127>/Subtract' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Constant_1;/* '<S127>/Constant' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Product_1;/* '<S127>/Product' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Constant1_1;/* '<S127>/Constant1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Product1_1;/* '<S127>/Product1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Add_1;/* '<S127>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Constant2_1;/* '<S127>/Constant2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Product2_1;/* '<S127>/Product2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Constant3_1;/* '<S127>/Constant3' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Product3_1;/* '<S127>/Product3' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Add1_1;/* '<S127>/Add1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_VectorConcatenate_1
  [2];                                 /* '<S127>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_VectorConcatenate1_1
  [40];                                /* '<S110>/VectorConcatenate1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Subtract_1
  [40];                                /* '<S107>/Subtract' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Divide_1
  [40];                                /* '<S107>/Divide' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Reshape_1
  [40];                                /* '<S104>/Reshape' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_two_norm_DotProduct_1;/* '<S105>/DotProduct' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_two_norm_Sqrt_1;/* '<S105>/Sqrt' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_Width1_1;/* '<S102>/Width1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_Reshape1_1
  [4];                                 /* '<S102>/Reshape1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_matrix_1
  [12];                                /* '<S98>/Assignment_h_matrix' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Divide_1[40];/* '<S98>/Divide' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_q_matrix_1
  [160];                               /* '<S98>/Assignment_q_matrix' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Add_1;/* '<S104>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_zero2_1;/* '<S104>/zero2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Product2_1;/* '<S107>/Product2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Sum2_1;/* '<S107>/Sum2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector3_1
  [3];                                 /* '<S99>/Selector3' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector_1
  [4];                                 /* '<S99>/Selector' */
real_T c_gmres_fdgmres_Iterativvelyminimizeresidualvector_s_vector_1[3];/* '<S9>/s_vector' */
real_T c_gmres_fdgmres_Iterativvelyminimizeresidualvector_c_vector_1[3];/* '<S9>/c_vector' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_one_1;/* '<S99>/one' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Subtract_1;/* '<S99>/Subtract' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Switch_Switch_1
  [3];                                 /* '<S142>/Switch' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Selector1_1;/* '<S141>/Selector1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Square_1;/* '<S141>/Square' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Selector2_1;/* '<S141>/Selector2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Square1_1;/* '<S141>/Square1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Add_1;/* '<S141>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Sqrt_1;/* '<S141>/Sqrt' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Divide3_1;/* '<S141>/Divide3' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Divide2_1;/* '<S141>/Divide2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Add1_1;/* '<S141>/Add1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Divide_1;/* '<S141>/Divide' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment1_1
  [3];                                 /* '<S141>/Assignment1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Divide1_1;/* '<S141>/Divide1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment2_1
  [3];                                 /* '<S141>/Assignment2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_b_hat_1
  [4];                                 /* '<S100>/b_hat' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Switch1_1
  [4];                                 /* '<S100>/Switch1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Selector_1;/* '<S100>/Selector' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Product1_1;/* '<S100>/Product1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Abs_rho;/* '<S100>/Abs' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Product_1;/* '<S100>/Product' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Gain_1;/* '<S100>/Gain' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_VectorConcatenate_1
  [2];                                 /* '<S100>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Assignment_b_hat_1
  [4];                                 /* '<S100>/Assignment_b_hat' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_r_matrix_1[9];/* '<S101>/r_matrix' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_Width1_1;/* '<S146>/Width1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_matrix_1
  [9];                                 /* '<S101>/Assignment_r_matrix' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Selector_1;/* '<S147>/Selector' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Switch_1;/* '<S147>/Switch' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Assignment_1
  [3];                                 /* '<S147>/Assignment' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector1_c_j_1;/* '<S144>/Selector1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_b_hat_1
  [3];                                 /* '<S144>/b_hat' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Switch1_1
  [3];                                 /* '<S144>/Switch1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector2_r_j_1n;/* '<S144>/Selector2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Product_1;/* '<S144>/Product' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector_s_j_1;/* '<S144>/Selector' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_one_1;/* '<S144>/one' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Add2_1;/* '<S144>/Add2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector3_h_jn;/* '<S144>/Selector3' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Product1_1;/* '<S144>/Product1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Add_1;/* '<S144>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Product2_1;/* '<S144>/Product2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Product3_1;/* '<S144>/Product3' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Add1_1;/* '<S144>/Add1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_VectorConcatenate_1
  [2];                                 /* '<S144>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Assignment_1
  [3];                                 /* '<S144>/Assignment' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Constant_1
  [40];                                /* '<S138>/Constant' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S138>/Selector' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
  [2];                                 /* '<S140>/Gain3' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
  [2];                                 /* '<S139>/UnitDelay' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
  [2];                                 /* '<S139>/Switch' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
  [2];                                 /* '<S138>/Reshape1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
  [2];                                 /* '<S140>/Saturation' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
  [2];                                 /* '<S140>/Sqrt' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain_1;/* '<S140>/Gain' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain1_1;/* '<S140>/Gain1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain2_1;/* '<S140>/Gain2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add_1;/* '<S140>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
  [2];                                 /* '<S140>/Add1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Gain_1;/* '<S138>/Gain' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
  [2];                                 /* '<S139>/Product' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
  [2];                                 /* '<S139>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Reshape_1
  [2];                                 /* '<S138>/Reshape' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Assignment_1
  [40];                                /* '<S138>/Assignment' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Constant_1
  [40];                                /* '<S135>/Constant' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S135>/Selector' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_r1_1;/* '<S136>/r1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_Product_1;/* '<S136>/Product' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Selector1_1
  [2];                                 /* '<S135>/Selector1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_A1_1;/* '<S136>/A1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_Divide_1;/* '<S136>/Divide' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_Add_1;/* '<S136>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_r2_1;/* '<S137>/r2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_Product_1;/* '<S137>/Product' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_A2_1;/* '<S137>/A2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_Divide_1;/* '<S137>/Divide' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_Add_1;/* '<S137>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
  [2];                                 /* '<S135>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Assignment_1
  [40];                                /* '<S135>/Assignment' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
  [38];                                /* '<S130>/Constant' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1;/* '<S133>/Q11' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1;/* '<S130>/Constant1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;/* '<S130>/Subtract' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S130>/Selector' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
  [2];                                 /* '<S130>/Selector1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
  [2];                                 /* '<S131>/Subtract' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1;/* '<S133>/Product' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1;/* '<S133>/Q12_Q21' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1;/* '<S133>/Product1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1;/* '<S133>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
  [2];                                 /* '<S132>/UnitDelay' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
  [2];                                 /* '<S132>/Switch' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1;/* '<S133>/C1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1;/* '<S133>/A1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1;/* '<S133>/Divide' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1;/* '<S133>/Product2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1;/* '<S133>/A2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1;/* '<S133>/Divide1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1;/* '<S133>/Product3' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1;/* '<S133>/Add1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
  [2];                                 /* '<S131>/Sqrt' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1;/* '<S133>/Divide2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1;/* '<S133>/Gain' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1;/* '<S133>/Add2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1;/* '<S134>/Q22' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1;/* '<S134>/Product' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1;/* '<S134>/Q12_Q21' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1;/* '<S134>/Product1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1;/* '<S134>/C2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1;/* '<S134>/A2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1;/* '<S134>/Product3' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1;/* '<S134>/Gain' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1;/* '<S134>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
  [2];                                 /* '<S131>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;/* '<S130>/Gain' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
  [2];                                 /* '<S132>/Product' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
  [2];                                 /* '<S132>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
  [2];                                 /* '<S130>/Reshape' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1;/* '<S130>/Constant2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1;/* '<S130>/Subtract1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
  [38];                                /* '<S130>/Assignment' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Constant_1
  [40];                                /* '<S123>/Constant' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S123>/Selector' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
  [2];                                 /* '<S125>/Gain3' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
  [2];                                 /* '<S124>/UnitDelay' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
  [2];                                 /* '<S124>/Switch' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Reshape1_1
  [2];                                 /* '<S123>/Reshape1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
  [2];                                 /* '<S125>/Saturation' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
  [2];                                 /* '<S125>/Sqrt' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain_1;/* '<S125>/Gain' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain1_1;/* '<S125>/Gain1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain2_1;/* '<S125>/Gain2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add_1;/* '<S125>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
  [2];                                 /* '<S125>/Add1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Gain_1;/* '<S123>/Gain' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
  [2];                                 /* '<S124>/Product' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
  [2];                                 /* '<S124>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Reshape_1
  [2];                                 /* '<S123>/Reshape' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Assignment_1
  [40];                                /* '<S123>/Assignment' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Constant_1
  [40];                                /* '<S120>/Constant' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S120>/Selector' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_r1_1;/* '<S121>/r1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_Product_1;/* '<S121>/Product' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Selector1_1
  [2];                                 /* '<S120>/Selector1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_A1_1;/* '<S121>/A1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_Divide_1;/* '<S121>/Divide' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_Add_1;/* '<S121>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_r2_1;/* '<S122>/r2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_Product_1;/* '<S122>/Product' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_A2_1;/* '<S122>/A2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_Divide_1;/* '<S122>/Divide' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_Add_1;/* '<S122>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_VectorConcatenate_1
  [2];                                 /* '<S120>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Assignment_1
  [40];                                /* '<S120>/Assignment' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
  [38];                                /* '<S115>/Constant' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1;/* '<S118>/Q11' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1;/* '<S115>/Constant1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;/* '<S115>/Subtract' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S115>/Selector' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
  [2];                                 /* '<S115>/Selector1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
  [2];                                 /* '<S116>/Subtract' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1;/* '<S118>/Product' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1;/* '<S118>/Q12_Q21' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1;/* '<S118>/Product1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1;/* '<S118>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
  [2];                                 /* '<S117>/UnitDelay' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
  [2];                                 /* '<S117>/Switch' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1;/* '<S118>/C1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1;/* '<S118>/A1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1;/* '<S118>/Divide' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1;/* '<S118>/Product2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1;/* '<S118>/A2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1;/* '<S118>/Divide1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1;/* '<S118>/Product3' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1;/* '<S118>/Add1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
  [2];                                 /* '<S116>/Sqrt' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1;/* '<S118>/Divide2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1;/* '<S118>/Gain' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1;/* '<S118>/Add2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1;/* '<S119>/Q22' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1;/* '<S119>/Product' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1;/* '<S119>/Q12_Q21' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1;/* '<S119>/Product1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1;/* '<S119>/C2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1;/* '<S119>/A2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1;/* '<S119>/Product3' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1;/* '<S119>/Gain' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1;/* '<S119>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
  [2];                                 /* '<S116>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;/* '<S115>/Gain' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
  [2];                                 /* '<S117>/Product' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
  [2];                                 /* '<S117>/Add' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
  [2];                                 /* '<S115>/Reshape' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1;/* '<S115>/Constant2' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1;/* '<S115>/Subtract1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
  [38];                                /* '<S115>/Assignment' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_h_vector_initial_1
  [4];                                 /* '<S103>/h_vector_initial' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_v_vector_1
  [40];                                /* '<S103>/v_vector' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Switch_1
  [40];                                /* '<S103>/Switch' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Selector_1
  [40];                                /* '<S103>/Selector' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_DotProduct_1;/* '<S103>/DotProduct' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Assignment1_1
  [4];                                 /* '<S103>/Assignment1' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Product_1
  [40];                                /* '<S103>/Product' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Subtract_1
  [40];                                /* '<S103>/Subtract' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Selector_1;/* '<S106>/Selector' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Switch_1;/* '<S106>/Switch' */
real_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Assignment_1
  [4];                                 /* '<S106>/Assignment' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Constant_1
  [40];                                /* '<S95>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S95>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
  [2];                                 /* '<S97>/Gain3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
  [2];                                 /* '<S96>/UnitDelay' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
  [2];                                 /* '<S96>/Switch' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
  [2];                                 /* '<S95>/Reshape1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
  [2];                                 /* '<S97>/Saturation' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
  [2];                                 /* '<S97>/Sqrt' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain_1;/* '<S97>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain1_1;/* '<S97>/Gain1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain2_1;/* '<S97>/Gain2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Add_1;/* '<S97>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
  [2];                                 /* '<S97>/Add1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Gain_1;/* '<S95>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
  [2];                                 /* '<S96>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
  [2];                                 /* '<S96>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Reshape_1
  [2];                                 /* '<S95>/Reshape' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Assignment_1
  [40];                                /* '<S95>/Assignment' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Constant_1
  [40];                                /* '<S92>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S92>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_r1_1;/* '<S93>/r1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_Product_1;/* '<S93>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Selector1_1
  [2];                                 /* '<S92>/Selector1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_A1_1;/* '<S93>/A1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_Divide_1;/* '<S93>/Divide' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_Add_1;/* '<S93>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_r2_1;/* '<S94>/r2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_Product_1;/* '<S94>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_A2_1;/* '<S94>/A2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_Divide_1;/* '<S94>/Divide' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_Add_1;/* '<S94>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
  [2];                                 /* '<S92>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Assignment_1
  [40];                                /* '<S92>/Assignment' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
  [38];                                /* '<S87>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1;/* '<S90>/Q11' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1;/* '<S87>/Constant1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;/* '<S87>/Subtract' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S87>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
  [2];                                 /* '<S87>/Selector1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
  [2];                                 /* '<S88>/Subtract' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1;/* '<S90>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1;/* '<S90>/Q12_Q21' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1;/* '<S90>/Product1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1;/* '<S90>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
  [2];                                 /* '<S89>/UnitDelay' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
  [2];                                 /* '<S89>/Switch' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1;/* '<S90>/C1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1;/* '<S90>/A1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1;/* '<S90>/Divide' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1;/* '<S90>/Product2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1;/* '<S90>/A2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1;/* '<S90>/Divide1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1;/* '<S90>/Product3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1;/* '<S90>/Add1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
  [2];                                 /* '<S88>/Sqrt' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1;/* '<S90>/Divide2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1;/* '<S90>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1;/* '<S90>/Add2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1;/* '<S91>/Q22' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1;/* '<S91>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1;/* '<S91>/Q12_Q21' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1;/* '<S91>/Product1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1;/* '<S91>/C2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1;/* '<S91>/A2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1;/* '<S91>/Product3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1;/* '<S91>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1;/* '<S91>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
  [2];                                 /* '<S88>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;/* '<S87>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
  [2];                                 /* '<S89>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
  [2];                                 /* '<S89>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
  [2];                                 /* '<S87>/Reshape' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1;/* '<S87>/Constant2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1;/* '<S87>/Subtract1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
  [38];                                /* '<S87>/Assignment' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Constant_1
  [40];                                /* '<S80>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S80>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
  [2];                                 /* '<S82>/Gain3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
  [2];                                 /* '<S81>/UnitDelay' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
  [2];                                 /* '<S81>/Switch' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Reshape1_1
  [2];                                 /* '<S80>/Reshape1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
  [2];                                 /* '<S82>/Saturation' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
  [2];                                 /* '<S82>/Sqrt' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Gain_1;/* '<S82>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Gain1_1;/* '<S82>/Gain1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Gain2_1;/* '<S82>/Gain2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Add_1;/* '<S82>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
  [2];                                 /* '<S82>/Add1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Gain_1;/* '<S80>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
  [2];                                 /* '<S81>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
  [2];                                 /* '<S81>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Reshape_1
  [2];                                 /* '<S80>/Reshape' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Assignment_1
  [40];                                /* '<S80>/Assignment' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Constant_1
  [40];                                /* '<S77>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S77>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_r1_1;/* '<S78>/r1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_Product_1;/* '<S78>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Selector1_1
  [2];                                 /* '<S77>/Selector1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_A1_1;/* '<S78>/A1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_Divide_1;/* '<S78>/Divide' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_Add_1;/* '<S78>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_r2_1;/* '<S79>/r2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_Product_1;/* '<S79>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_A2_1;/* '<S79>/A2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_Divide_1;/* '<S79>/Divide' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_Add_1;/* '<S79>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_VectorConcatenate_1
  [2];                                 /* '<S77>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Assignment_1
  [40];                                /* '<S77>/Assignment' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
  [38];                                /* '<S72>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1;/* '<S75>/Q11' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1;/* '<S72>/Constant1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;/* '<S72>/Subtract' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S72>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
  [2];                                 /* '<S72>/Selector1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
  [2];                                 /* '<S73>/Subtract' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1;/* '<S75>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1;/* '<S75>/Q12_Q21' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1;/* '<S75>/Product1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1;/* '<S75>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
  [2];                                 /* '<S74>/UnitDelay' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
  [2];                                 /* '<S74>/Switch' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1;/* '<S75>/C1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1;/* '<S75>/A1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1;/* '<S75>/Divide' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1;/* '<S75>/Product2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1;/* '<S75>/A2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1;/* '<S75>/Divide1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1;/* '<S75>/Product3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1;/* '<S75>/Add1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
  [2];                                 /* '<S73>/Sqrt' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1;/* '<S75>/Divide2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1;/* '<S75>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1;/* '<S75>/Add2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1;/* '<S76>/Q22' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1;/* '<S76>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1;/* '<S76>/Q12_Q21' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1;/* '<S76>/Product1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1;/* '<S76>/C2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1;/* '<S76>/A2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1;/* '<S76>/Product3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1;/* '<S76>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1;/* '<S76>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
  [2];                                 /* '<S73>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;/* '<S72>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
  [2];                                 /* '<S74>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
  [2];                                 /* '<S74>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
  [2];                                 /* '<S72>/Reshape' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1;/* '<S72>/Constant2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1;/* '<S72>/Subtract1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
  [38];                                /* '<S72>/Assignment' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Constant_1
  [40];                                /* '<S63>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S63>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
  [2];                                 /* '<S65>/Gain3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
  [2];                                 /* '<S64>/UnitDelay' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
  [2];                                 /* '<S64>/Switch' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Reshape1_1
  [2];                                 /* '<S63>/Reshape1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
  [2];                                 /* '<S65>/Saturation' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
  [2];                                 /* '<S65>/Sqrt' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Gain_1;/* '<S65>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Gain1_1;/* '<S65>/Gain1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Gain2_1;/* '<S65>/Gain2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Add_1;/* '<S65>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
  [2];                                 /* '<S65>/Add1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Gain_1;/* '<S63>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
  [2];                                 /* '<S64>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
  [2];                                 /* '<S64>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Reshape_1
  [2];                                 /* '<S63>/Reshape' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Assignment_1
  [40];                                /* '<S63>/Assignment' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Constant_1
  [40];                                /* '<S60>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S60>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_r1_1;/* '<S61>/r1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_Product_1;/* '<S61>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Selector1_1
  [2];                                 /* '<S60>/Selector1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_A1_1;/* '<S61>/A1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_Divide_1;/* '<S61>/Divide' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_Add_1;/* '<S61>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_r2_1;/* '<S62>/r2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_Product_1;/* '<S62>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_A2_1;/* '<S62>/A2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_Divide_1;/* '<S62>/Divide' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_Add_1;/* '<S62>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_VectorConcatenate_1
  [2];                                 /* '<S60>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Assignment_1
  [40];                                /* '<S60>/Assignment' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
  [38];                                /* '<S55>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1;/* '<S58>/Q11' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1;/* '<S55>/Constant1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;/* '<S55>/Subtract' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S55>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
  [2];                                 /* '<S55>/Selector1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
  [2];                                 /* '<S56>/Subtract' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1;/* '<S58>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1;/* '<S58>/Q12_Q21' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1;/* '<S58>/Product1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1;/* '<S58>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
  [2];                                 /* '<S57>/UnitDelay' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
  [2];                                 /* '<S57>/Switch' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1;/* '<S58>/C1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1;/* '<S58>/A1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1;/* '<S58>/Divide' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1;/* '<S58>/Product2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1;/* '<S58>/A2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1;/* '<S58>/Divide1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1;/* '<S58>/Product3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1;/* '<S58>/Add1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
  [2];                                 /* '<S56>/Sqrt' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1;/* '<S58>/Divide2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1;/* '<S58>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1;/* '<S58>/Add2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1;/* '<S59>/Q22' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1;/* '<S59>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1;/* '<S59>/Q12_Q21' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1;/* '<S59>/Product1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1;/* '<S59>/C2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1;/* '<S59>/A2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1;/* '<S59>/Product3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1;/* '<S59>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1;/* '<S59>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
  [2];                                 /* '<S56>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;/* '<S55>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
  [2];                                 /* '<S57>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
  [2];                                 /* '<S57>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
  [2];                                 /* '<S55>/Reshape' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1;/* '<S55>/Constant2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1;/* '<S55>/Subtract1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
  [38];                                /* '<S55>/Assignment' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Constant_1
  [40];                                /* '<S46>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S46>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
  [2];                                 /* '<S48>/Gain3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
  [2];                                 /* '<S47>/UnitDelay' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
  [2];                                 /* '<S47>/Switch' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
  [2];                                 /* '<S46>/Reshape1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
  [2];                                 /* '<S48>/Saturation' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
  [2];                                 /* '<S48>/Sqrt' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain_1;/* '<S48>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain1_1;/* '<S48>/Gain1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain2_1;/* '<S48>/Gain2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add_1;/* '<S48>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
  [2];                                 /* '<S48>/Add1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Gain_1;/* '<S46>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
  [2];                                 /* '<S47>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
  [2];                                 /* '<S47>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Reshape_1
  [2];                                 /* '<S46>/Reshape' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Assignment_1
  [40];                                /* '<S46>/Assignment' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Constant_1
  [40];                                /* '<S43>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S43>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_r1_1;/* '<S44>/r1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_Product_1;/* '<S44>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Selector1_1
  [2];                                 /* '<S43>/Selector1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_A1_1;/* '<S44>/A1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_Divide_1;/* '<S44>/Divide' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_Add_1;/* '<S44>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_r2_1;/* '<S45>/r2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_Product_1;/* '<S45>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_A2_1;/* '<S45>/A2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_Divide_1;/* '<S45>/Divide' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_Add_1;/* '<S45>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
  [2];                                 /* '<S43>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Assignment_1
  [40];                                /* '<S43>/Assignment' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
  [38];                                /* '<S38>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1;/* '<S41>/Q11' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1;/* '<S38>/Constant1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;/* '<S38>/Subtract' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S38>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
  [2];                                 /* '<S38>/Selector1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
  [2];                                 /* '<S39>/Subtract' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1;/* '<S41>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1;/* '<S41>/Q12_Q21' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1;/* '<S41>/Product1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1;/* '<S41>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
  [2];                                 /* '<S40>/UnitDelay' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
  [2];                                 /* '<S40>/Switch' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1;/* '<S41>/C1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1;/* '<S41>/A1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1;/* '<S41>/Divide' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1;/* '<S41>/Product2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1;/* '<S41>/A2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1;/* '<S41>/Divide1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1;/* '<S41>/Product3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1;/* '<S41>/Add1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
  [2];                                 /* '<S39>/Sqrt' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1;/* '<S41>/Divide2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1;/* '<S41>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1;/* '<S41>/Add2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1;/* '<S42>/Q22' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1;/* '<S42>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1;/* '<S42>/Q12_Q21' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1;/* '<S42>/Product1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1;/* '<S42>/C2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1;/* '<S42>/A2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1;/* '<S42>/Product3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1;/* '<S42>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1;/* '<S42>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
  [2];                                 /* '<S39>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;/* '<S38>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
  [2];                                 /* '<S40>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
  [2];                                 /* '<S40>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
  [2];                                 /* '<S38>/Reshape' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1;/* '<S38>/Constant2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1;/* '<S38>/Subtract1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
  [38];                                /* '<S38>/Assignment' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Constant_1
  [40];                                /* '<S31>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S31>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
  [2];                                 /* '<S33>/Gain3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
  [2];                                 /* '<S32>/UnitDelay' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
  [2];                                 /* '<S32>/Switch' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Reshape1_1
  [2];                                 /* '<S31>/Reshape1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
  [2];                                 /* '<S33>/Saturation' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
  [2];                                 /* '<S33>/Sqrt' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain_1;/* '<S33>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain1_1;/* '<S33>/Gain1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain2_1;/* '<S33>/Gain2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add_1;/* '<S33>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
  [2];                                 /* '<S33>/Add1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Gain_1;/* '<S31>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
  [2];                                 /* '<S32>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
  [2];                                 /* '<S32>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Reshape_1
  [2];                                 /* '<S31>/Reshape' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Assignment_1
  [40];                                /* '<S31>/Assignment' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Constant_1
  [40];                                /* '<S28>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S28>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_r1_1;/* '<S29>/r1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_Product_1;/* '<S29>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Selector1_1
  [2];                                 /* '<S28>/Selector1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_A1_1;/* '<S29>/A1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_Divide_1;/* '<S29>/Divide' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_Add_1;/* '<S29>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_r2_1;/* '<S30>/r2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_Product_1;/* '<S30>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_A2_1;/* '<S30>/A2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_Divide_1;/* '<S30>/Divide' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_Add_1;/* '<S30>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_VectorConcatenate_1
  [2];                                 /* '<S28>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Assignment_1
  [40];                                /* '<S28>/Assignment' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
  [38];                                /* '<S23>/Constant' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1;/* '<S26>/Q11' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1;/* '<S23>/Constant1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;/* '<S23>/Subtract' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
  [2];                                 /* '<S23>/Selector' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
  [2];                                 /* '<S23>/Selector1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
  [2];                                 /* '<S24>/Subtract' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1;/* '<S26>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1;/* '<S26>/Q12_Q21' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1;/* '<S26>/Product1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1;/* '<S26>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
  [2];                                 /* '<S25>/UnitDelay' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
  [2];                                 /* '<S25>/Switch' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1;/* '<S26>/C1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1;/* '<S26>/A1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1;/* '<S26>/Divide' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1;/* '<S26>/Product2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1;/* '<S26>/A2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1;/* '<S26>/Divide1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1;/* '<S26>/Product3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1;/* '<S26>/Add1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
  [2];                                 /* '<S24>/Sqrt' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1;/* '<S26>/Divide2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1;/* '<S26>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1;/* '<S26>/Add2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1;/* '<S27>/Q22' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1;/* '<S27>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1;/* '<S27>/Q12_Q21' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1;/* '<S27>/Product1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1;/* '<S27>/C2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1;/* '<S27>/A2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1;/* '<S27>/Product3' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1;/* '<S27>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1;/* '<S27>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
  [2];                                 /* '<S24>/VectorConcatenate' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;/* '<S23>/Gain' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
  [2];                                 /* '<S25>/Product' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
  [2];                                 /* '<S25>/Add' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
  [2];                                 /* '<S23>/Reshape' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1;/* '<S23>/Constant2' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1;/* '<S23>/Subtract1' */
real_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
  [38];                                /* '<S23>/Assignment' */
real_T c_gmres_fdgmres_Calculatey_iterations_1;/* '<S7>/iterations' */
real_T c_gmres_fdgmres_Calculatey_one_1;/* '<S7>/one' */
real_T c_gmres_fdgmres_Calculatey_Add_1;/* '<S7>/Add' */
real_T c_gmres_fdgmres_Calculatey_y_1[3];/* '<S7>/y' */
real_T c_gmres_fdgmres_Calculatey_Selector_1;/* '<S7>/Selector' */
real_T c_gmres_fdgmres_Calculatey_zero_1;/* '<S7>/zero' */
real_T c_gmres_fdgmres_Calculatey_Switch_1;/* '<S7>/Switch' */
real_T c_gmres_fdgmres_Calculatey_Subtract_1;/* '<S7>/Subtract' */
real_T c_gmres_fdgmres_Calculatey_Selector1_1;/* '<S7>/Selector1' */
real_T c_gmres_fdgmres_Calculatey_Divide_1;/* '<S7>/Divide' */
real_T c_gmres_fdgmres_Calculatey_Assignment_y_1[3];/* '<S7>/Assignment_y' */
real_T c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_Selector1_1;/* '<S11>/Selector1' */
real_T c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_Selector_1;/* '<S11>/Selector' */
real_T c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_Product_1;/* '<S11>/Product' */
real_T c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_UnitDelay_1;/* '<S11>/UnitDelay' */
real_T c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_Sum_1;/* '<S11>/Sum' */
real_T c_gmres_fdgmres_Calcukatex_Selector_1[40];/* '<S6>/Selector' */
real_T c_gmres_fdgmres_Calcukatex_Selector1_1;/* '<S6>/Selector1' */
real_T c_gmres_fdgmres_Calcukatex_Product_1[40];/* '<S6>/Product' */
real_T c_gmres_fdgmres_Calcukatex_UnitDelay_1[40];/* '<S6>/UnitDelay' */
real_T c_gmres_fdgmres_Calcukatex_Add_1[40];/* '<S6>/Add' */
int32_T c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1;/* '<S9>/ForIterator' */
int32_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_ForIterator_1;/* '<S147>/ForIterator' */
int32_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_ForIterator_1;/* '<S144>/ForIterator' */
int32_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_ForIterator_1;/* '<S138>/ForIterator' */
int32_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_ForIterator_1;/* '<S135>/ForIterator' */
int32_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;/* '<S130>/ForIterator' */
int32_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_ForIterator_1;/* '<S123>/ForIterator' */
int32_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_ForIterator_1;/* '<S120>/ForIterator' */
int32_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;/* '<S115>/ForIterator' */
int32_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_ForIterator_1;/* '<S103>/ForIterator' */
int32_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_ForIterator_1;/* '<S106>/ForIterator' */
int32_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_ForIterator_1;/* '<S95>/ForIterator' */
int32_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_ForIterator_1;/* '<S92>/ForIterator' */
int32_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;/* '<S87>/ForIterator' */
int32_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_ForIterator_1;/* '<S80>/ForIterator' */
int32_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_ForIterator_1;/* '<S77>/ForIterator' */
int32_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;/* '<S72>/ForIterator' */
int32_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_ForIterator_1;/* '<S63>/ForIterator' */
int32_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_ForIterator_1;/* '<S60>/ForIterator' */
int32_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;/* '<S55>/ForIterator' */
int32_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_ForIterator_1;/* '<S46>/ForIterator' */
int32_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_ForIterator_1;/* '<S43>/ForIterator' */
int32_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;/* '<S38>/ForIterator' */
int32_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_ForIterator_1;/* '<S31>/ForIterator' */
int32_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_ForIterator_1;/* '<S28>/ForIterator' */
int32_T
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;/* '<S23>/ForIterator' */
int32_T c_gmres_fdgmres_Calculatey_ForIterator_l_1;/* '<S7>/ForIterator_l' */
int32_T c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_ForIterator_1;/* '<S11>/ForIterator' */
int32_T c_gmres_fdgmres_Calcukatex_ForIterator_1;/* '<S6>/ForIterator' */
boolean_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_RelationalOperator_1;/* '<S147>/RelationalOperator' */
boolean_T
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_RelationalOperator_1;/* '<S106>/RelationalOperator' */

/* Block states (auto storage) */
D_Work_c_gmres c_gmres_DWork;

/* Real-time model */
RT_MODEL_c_gmres c_gmres_M_;
RT_MODEL_c_gmres *const c_gmres_M = &c_gmres_M_;

/* Model step function */
void c_gmres_step(void)
{
  int32_T c_gmres_fdgmres_Initialize_mode;
  real_T Bias;
  int32_T c_gmres_fdgmres_Iterativvelymin;
  int32_T i;
  int32_T i_0;
  real_T x;
  real_T x_idx_1;
  real_T y_idx_0;
  real_T y_idx_1;
  int32_T exitg1;
  int32_T exitg2;
  int32_T exitg3;

MEASUREMENT_STEP_BEGIN();
MEASUREMENT_START(C_GMRES_DISCRETE_TIMEINTEGRATOR,0,0,C_GMRES_DISCRETE_TIMEINTEGRATOR_PARAM);
  /* DiscreteIntegrator: '<Root>/Discrete_TimeIntegrator' */
  memcpy(&c_gmres_Discrete_TimeIntegrator_1[0],
         &c_gmres_DWork.Discrete_TimeIntegrator_DSTATE[0], 40U * sizeof(real_T));
MEASUREMENT_STOP(C_GMRES_DISCRETE_TIMEINTEGRATOR,0,0,C_GMRES_DISCRETE_TIMEINTEGRATOR_PARAM);

MEASUREMENT_START(C_GMRES_SELECTOR,0,0,C_GMRES_SELECTOR_PARAM);
  /* Selector: '<Root>/Selector' */
  c_gmres_Selector_1[0] = c_gmres_Discrete_TimeIntegrator_1[0];
  c_gmres_Selector_1[1] = c_gmres_Discrete_TimeIntegrator_1[1];
MEASUREMENT_STOP(C_GMRES_SELECTOR,0,0,C_GMRES_SELECTOR_PARAM);

MEASUREMENT_START(C_GMRES_PLANT_MODEL1_GAIN3,0,0,C_GMRES_PLANT_MODEL1_GAIN3_PARAM);
  /* Gain: '<S148>/Gain3' */
  Bias = 1.0;
  x_idx_1 = 1.0;
  y_idx_0 = c_gmres_P.tank_area[0];
  y_idx_1 = c_gmres_P.tank_area[1];
  x = Bias;
  x /= y_idx_0;
  Bias = x;
  x = x_idx_1;
  x /= y_idx_1;
  x_idx_1 = x;
  c_gmres_plant_Model1_Gain3_1[0] = Bias * c_gmres_Selector_1[0];
  c_gmres_plant_Model1_Gain3_1[1] = x_idx_1 * c_gmres_Selector_1[1];
MEASUREMENT_STOP(C_GMRES_PLANT_MODEL1_GAIN3,0,0,C_GMRES_PLANT_MODEL1_GAIN3_PARAM);

MEASUREMENT_START(C_GMRES_PLANT_UNITDELAY2,0,0,C_GMRES_PLANT_UNITDELAY2_PARAM);
  /* UnitDelay: '<S5>/UnitDelay2' */
  c_gmres_plant_UnitDelay2_1[0] = c_gmres_DWork.UnitDelay2_DSTATE[0];
  c_gmres_plant_UnitDelay2_1[1] = c_gmres_DWork.UnitDelay2_DSTATE[1];
MEASUREMENT_STOP(C_GMRES_PLANT_UNITDELAY2,0,0,C_GMRES_PLANT_UNITDELAY2_PARAM);

MEASUREMENT_START(C_GMRES_PLANT_MODEL1_SATURATION,0,0,C_GMRES_PLANT_MODEL1_SATURATION_PARAM);
  /* Saturate: '<S148>/Saturation' */
  Bias = c_gmres_plant_UnitDelay2_1[0];
  x_idx_1 = c_gmres_P.Saturation_LowerSat_i;
  y_idx_0 = c_gmres_P.Saturation_UpperSat_i;
  if (Bias > y_idx_0) {
    Bias = y_idx_0;
  } else {
    if (Bias < x_idx_1) {
      Bias = x_idx_1;
    }
  }

  c_gmres_plant_Model1_Saturation_1[0] = Bias;
  Bias = c_gmres_plant_UnitDelay2_1[1];
  x_idx_1 = c_gmres_P.Saturation_LowerSat_i;
  y_idx_0 = c_gmres_P.Saturation_UpperSat_i;
  if (Bias > y_idx_0) {
    Bias = y_idx_0;
  } else {
    if (Bias < x_idx_1) {
      Bias = x_idx_1;
    }
  }

  c_gmres_plant_Model1_Saturation_1[1] = Bias;
MEASUREMENT_STOP(C_GMRES_PLANT_MODEL1_SATURATION,0,0,C_GMRES_PLANT_MODEL1_SATURATION_PARAM);

  /* End of Saturate: '<S148>/Saturation' */

MEASUREMENT_START(C_GMRES_PLANT_MODEL1_SQRT,0,0,C_GMRES_PLANT_MODEL1_SQRT_PARAM);
  /* Sqrt: '<S148>/Sqrt' */
  c_gmres_plant_Model1_Sqrt_1[0] = sqrt(c_gmres_plant_Model1_Saturation_1[0]);
  c_gmres_plant_Model1_Sqrt_1[1] = sqrt(c_gmres_plant_Model1_Saturation_1[1]);
MEASUREMENT_STOP(C_GMRES_PLANT_MODEL1_SQRT,0,0,C_GMRES_PLANT_MODEL1_SQRT_PARAM);

MEASUREMENT_START(C_GMRES_PLANT_MODEL1_GAIN,0,0,C_GMRES_PLANT_MODEL1_GAIN_PARAM);
  /* Gain: '<S148>/Gain' */
  Bias = -c_gmres_P.C_plant[0];
  x_idx_1 = c_gmres_P.tank_area[0];
  Bias /= x_idx_1;
  c_gmres_plant_Model1_Gain_1 = Bias * c_gmres_plant_Model1_Sqrt_1[0];
MEASUREMENT_STOP(C_GMRES_PLANT_MODEL1_GAIN,0,0,C_GMRES_PLANT_MODEL1_GAIN_PARAM);

MEASUREMENT_START(C_GMRES_PLANT_MODEL1_GAIN1,0,0,C_GMRES_PLANT_MODEL1_GAIN1_PARAM);
  /* Gain: '<S148>/Gain1' */
  Bias = c_gmres_P.C_plant[0];
  x_idx_1 = c_gmres_P.tank_area[1];
  Bias /= x_idx_1;
  c_gmres_plant_Model1_Gain1_1 = Bias * c_gmres_plant_Model1_Sqrt_1[0];
MEASUREMENT_STOP(C_GMRES_PLANT_MODEL1_GAIN1,0,0,C_GMRES_PLANT_MODEL1_GAIN1_PARAM);

MEASUREMENT_START(C_GMRES_PLANT_MODEL1_GAIN2,0,0,C_GMRES_PLANT_MODEL1_GAIN2_PARAM);
  /* Gain: '<S148>/Gain2' */
  Bias = -c_gmres_P.C_plant[1];
  x_idx_1 = c_gmres_P.tank_area[1];
  Bias /= x_idx_1;
  c_gmres_plant_Model1_Gain2_1 = Bias * c_gmres_plant_Model1_Sqrt_1[1];
MEASUREMENT_STOP(C_GMRES_PLANT_MODEL1_GAIN2,0,0,C_GMRES_PLANT_MODEL1_GAIN2_PARAM);

MEASUREMENT_START(C_GMRES_PLANT_MODEL1_ADD,0,0,C_GMRES_PLANT_MODEL1_ADD_PARAM);
  /* Sum: '<S148>/Add' */
  c_gmres_plant_Model1_Add_1 = c_gmres_plant_Model1_Gain1_1 +
    c_gmres_plant_Model1_Gain2_1;
MEASUREMENT_STOP(C_GMRES_PLANT_MODEL1_ADD,0,0,C_GMRES_PLANT_MODEL1_ADD_PARAM);

MEASUREMENT_START(C_GMRES_PLANT_MODEL1_ADD1,0,0,C_GMRES_PLANT_MODEL1_ADD1_PARAM);
  /* Sum: '<S148>/Add1' */
  c_gmres_plant_Model1_Add1_1[0] = c_gmres_plant_Model1_Gain3_1[0] +
    c_gmres_plant_Model1_Gain_1;
  c_gmres_plant_Model1_Add1_1[1] = c_gmres_plant_Model1_Gain3_1[1] +
    c_gmres_plant_Model1_Add_1;
MEASUREMENT_STOP(C_GMRES_PLANT_MODEL1_ADD1,0,0,C_GMRES_PLANT_MODEL1_ADD1_PARAM);

MEASUREMENT_START(C_GMRES_PLANT_GAIN,0,0,C_GMRES_PLANT_GAIN_PARAM);
  /* Gain: '<S5>/Gain' */
  c_gmres_plant_Gain_1[0] = c_gmres_P.delta_t * c_gmres_plant_Model1_Add1_1[0];
  c_gmres_plant_Gain_1[1] = c_gmres_P.delta_t * c_gmres_plant_Model1_Add1_1[1];
MEASUREMENT_STOP(C_GMRES_PLANT_GAIN,0,0,C_GMRES_PLANT_GAIN_PARAM);

MEASUREMENT_START(C_GMRES_PLANT_ADD,0,0,C_GMRES_PLANT_ADD_PARAM);
  /* Sum: '<S5>/Add' */
  c_gmres_plant_Add_1[0] = c_gmres_plant_Gain_1[0] + c_gmres_plant_UnitDelay2_1
    [0];
  c_gmres_plant_Add_1[1] = c_gmres_plant_Gain_1[1] + c_gmres_plant_UnitDelay2_1
    [1];
MEASUREMENT_STOP(C_GMRES_PLANT_ADD,0,0,C_GMRES_PLANT_ADD_PARAM);

MEASUREMENT_START(C_GMRES_CONSTANT2,0,0,C_GMRES_CONSTANT2_PARAM);
  /* Constant: '<Root>/Constant2' */
  c_gmres_Constant2_1 = c_gmres_P.Constant2_Value_cq;
MEASUREMENT_STOP(C_GMRES_CONSTANT2,0,0,C_GMRES_CONSTANT2_PARAM);

MEASUREMENT_START(C_GMRES_SLIDERGAIN_SLIDERGAIN,0,0,C_GMRES_SLIDERGAIN_SLIDERGAIN_PARAM);
  /* Gain: '<S1>/SliderGain' */
  c_gmres_SliderGain_SliderGain_1 = c_gmres_P.SliderGain_gain *
    c_gmres_Constant2_1;
MEASUREMENT_STOP(C_GMRES_SLIDERGAIN_SLIDERGAIN,0,0,C_GMRES_SLIDERGAIN_SLIDERGAIN_PARAM);

MEASUREMENT_START(C_GMRES_VECTORCONCATENATE,0,0,C_GMRES_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<Root>/ConcatBufferAtVectorConcatenateIn1' */
  c_gmres_VectorConcatenate_1[0] = c_gmres_SliderGain_SliderGain_1;
MEASUREMENT_STOP(C_GMRES_VECTORCONCATENATE,0,0,C_GMRES_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_SLIDERGAIN1_SLIDERGAIN,0,0,C_GMRES_SLIDERGAIN1_SLIDERGAIN_PARAM);
  /* Gain: '<S2>/SliderGain' */
  c_gmres_SliderGain1_SliderGain_1 = c_gmres_P.SliderGain1_gain *
    c_gmres_Constant2_1;
MEASUREMENT_STOP(C_GMRES_SLIDERGAIN1_SLIDERGAIN,0,0,C_GMRES_SLIDERGAIN1_SLIDERGAIN_PARAM);

MEASUREMENT_START(C_GMRES_VECTORCONCATENATE,0,1,C_GMRES_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<Root>/ConcatBufferAtVectorConcatenateIn2' */
  c_gmres_VectorConcatenate_1[1] = c_gmres_SliderGain1_SliderGain_1;
MEASUREMENT_STOP(C_GMRES_VECTORCONCATENATE,0,1,C_GMRES_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_CONSTANT1,0,0,C_GMRES_CONSTANT1_PARAM);
  /* Constant: '<Root>/Constant1' */
  c_gmres_Constant1_1 = c_gmres_P.delta_t;
MEASUREMENT_STOP(C_GMRES_CONSTANT1,0,0,C_GMRES_CONSTANT1_PARAM);

MEASUREMENT_START(C_GMRES_DIGITALCLOCK,0,0,C_GMRES_DIGITALCLOCK_PARAM);
  /* DigitalClock: '<Root>/DigitalClock' */
  c_gmres_DigitalClock_1 = (((c_gmres_M->Timing.clockTick0+
    c_gmres_M->Timing.clockTickH0* 4294967296.0)) * 0.001);
MEASUREMENT_STOP(C_GMRES_DIGITALCLOCK,0,0,C_GMRES_DIGITALCLOCK_PARAM);

MEASUREMENT_START(C_GMRES_UNITDELAY1,0,0,C_GMRES_UNITDELAY1_PARAM);
  /* UnitDelay: '<Root>/UnitDelay1' */
  c_gmres_UnitDelay1_1[0] = c_gmres_DWork.UnitDelay1_DSTATE[0];
  c_gmres_UnitDelay1_1[1] = c_gmres_DWork.UnitDelay1_DSTATE[1];
MEASUREMENT_STOP(C_GMRES_UNITDELAY1,0,0,C_GMRES_UNITDELAY1_PARAM);

MEASUREMENT_START(C_GMRES_SUBTRACT,0,0,C_GMRES_SUBTRACT_PARAM);
  /* Sum: '<Root>/Subtract' */
  c_gmres_Subtract_1[0] = c_gmres_plant_Add_1[0] - c_gmres_UnitDelay1_1[0];
  c_gmres_Subtract_1[1] = c_gmres_plant_Add_1[1] - c_gmres_UnitDelay1_1[1];
MEASUREMENT_STOP(C_GMRES_SUBTRACT,0,0,C_GMRES_SUBTRACT_PARAM);

MEASUREMENT_START(C_GMRES_GAIN,0,0,C_GMRES_GAIN_PARAM);
  /* Gain: '<Root>/Gain' */
  x_idx_1 = c_gmres_P.delta_t;
  Bias = 1.0 / x_idx_1;
  c_gmres_Gain_1[0] = Bias * c_gmres_Subtract_1[0];
  c_gmres_Gain_1[1] = Bias * c_gmres_Subtract_1[1];
MEASUREMENT_STOP(C_GMRES_GAIN,0,0,C_GMRES_GAIN_PARAM);
MEASUREMENT_START(C_GMRES_SUBSYSTEM_UNITDELAY,0,0,C_GMRES_SUBSYSTEM_UNITDELAY_PARAM);
  for (i = 0; i < 40; i++) {
    /* UnitDelay: '<S3>/UnitDelay' */
    c_gmres_Subsystem_UnitDelay_1[i] = c_gmres_DWork.UnitDelay_DSTATE[i];
  }
MEASUREMENT_STOP(C_GMRES_SUBSYSTEM_UNITDELAY,0,0,C_GMRES_SUBSYSTEM_UNITDELAY_PARAM);

MEASUREMENT_START(C_GMRES_SUBSYSTEM_VECTORCONCATENATE1,0,0,C_GMRES_SUBSYSTEM_VECTORCONCATENATE1_PARAM);
  for (i = 0; i < 40; i++) {
    /* SignalConversion: '<S3>/ConcatBufferAtVectorConcatenate1In1' */
    c_gmres_Subsystem_VectorConcatenate1_1[i] = c_gmres_Subsystem_UnitDelay_1[i];
  }
MEASUREMENT_STOP(C_GMRES_SUBSYSTEM_VECTORCONCATENATE1,0,0,C_GMRES_SUBSYSTEM_VECTORCONCATENATE1_PARAM);

MEASUREMENT_START(C_GMRES_SUBSYSTEM_VECTORCONCATENATE1,0,1,C_GMRES_SUBSYSTEM_VECTORCONCATENATE1_PARAM);
  /* SignalConversion: '<S3>/ConcatBufferAtVectorConcatenate1In2' */
  c_gmres_Subsystem_VectorConcatenate1_1[40] = c_gmres_VectorConcatenate_1[0];
  c_gmres_Subsystem_VectorConcatenate1_1[41] = c_gmres_VectorConcatenate_1[1];
MEASUREMENT_STOP(C_GMRES_SUBSYSTEM_VECTORCONCATENATE1,0,1,C_GMRES_SUBSYSTEM_VECTORCONCATENATE1_PARAM);

MEASUREMENT_START(C_GMRES_SUBSYSTEM_SELECTOR1,0,0,C_GMRES_SUBSYSTEM_SELECTOR1_PARAM);
  /* Selector: '<S3>/Selector1' */
  for (i_0 = 0; i_0 < 20; i_0++) {
    c_gmres_Subsystem_Selector1_1[i_0 << 1] =
      c_gmres_Subsystem_VectorConcatenate1_1[(1 + i_0) << 1];
    c_gmres_Subsystem_Selector1_1[1 + (i_0 << 1)] =
      c_gmres_Subsystem_VectorConcatenate1_1[((1 + i_0) << 1) + 1];
  }
MEASUREMENT_STOP(C_GMRES_SUBSYSTEM_SELECTOR1,0,0,C_GMRES_SUBSYSTEM_SELECTOR1_PARAM);

  /* End of Selector: '<S3>/Selector1' */

MEASUREMENT_START(C_GMRES_UNITDELAY2,0,0,C_GMRES_UNITDELAY2_PARAM);
  /* UnitDelay: '<Root>/UnitDelay2' */
  memcpy(&c_gmres_UnitDelay2_1[0], &c_gmres_DWork.UnitDelay2_DSTATE_d[0], 40U *
         sizeof(real_T));
MEASUREMENT_STOP(C_GMRES_UNITDELAY2,0,0,C_GMRES_UNITDELAY2_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_Q_MATRIX,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_Q_MATRIX_PARAM);
  /* Constant: '<S8>/q_matrix' */
  memcpy(&c_gmres_fdgmres_Initialize_model_q_matrix_1[0],
         &c_gmres_P.q_matrix_Value[0], 160U * sizeof(real_T));
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_Q_MATRIX,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_Q_MATRIX_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S49>/ConcatBufferAtVectorConcatenateIn1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_VectorConcatenate_1
    [0] = c_gmres_UnitDelay1_1[0];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_VectorConcatenate_1
    [1] = c_gmres_UnitDelay1_1[1];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_CALCULATION_TAU_CONSTANT4,0,0,C_GMRES_FDGMRES_CALCULATION_TAU_CONSTANT4_PARAM);
  /* Constant: '<S10>/Constant4' */
  c_gmres_fdgmres_calculation_tau_Constant4_1 = c_gmres_P.Constant4_Value;
MEASUREMENT_STOP(C_GMRES_FDGMRES_CALCULATION_TAU_CONSTANT4,0,0,C_GMRES_FDGMRES_CALCULATION_TAU_CONSTANT4_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_CALCULATION_TAU_GAIN1,0,0,C_GMRES_FDGMRES_CALCULATION_TAU_GAIN1_PARAM);
  /* Gain: '<S10>/Gain1' */
  c_gmres_fdgmres_calculation_tau_Gain1_1 = c_gmres_P.Gain1_Gain *
    c_gmres_DigitalClock_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_CALCULATION_TAU_GAIN1,0,0,C_GMRES_FDGMRES_CALCULATION_TAU_GAIN1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_CALCULATION_TAU_CONSTANT3,0,0,C_GMRES_FDGMRES_CALCULATION_TAU_CONSTANT3_PARAM);
  /* Constant: '<S10>/Constant3' */
  c_gmres_fdgmres_calculation_tau_Constant3_1 = c_gmres_P.Constant3_Value_p;
MEASUREMENT_STOP(C_GMRES_FDGMRES_CALCULATION_TAU_CONSTANT3,0,0,C_GMRES_FDGMRES_CALCULATION_TAU_CONSTANT3_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_CALCULATION_TAU_SUBTRACT1,0,0,C_GMRES_FDGMRES_CALCULATION_TAU_SUBTRACT1_PARAM);
  /* Sum: '<S10>/Subtract1' */
  c_gmres_fdgmres_calculation_tau_Subtract1_1 =
    c_gmres_fdgmres_calculation_tau_Gain1_1 -
    c_gmres_fdgmres_calculation_tau_Constant3_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_CALCULATION_TAU_SUBTRACT1,0,0,C_GMRES_FDGMRES_CALCULATION_TAU_SUBTRACT1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_CALCULATION_TAU_MATHFUNCTION,0,0,C_GMRES_FDGMRES_CALCULATION_TAU_MATHFUNCTION_PARAM);
  /* Math: '<S10>/MathFunction'
   *
   * About '<S10>/MathFunction':
   *  Operator: exp
   */
  c_gmres_fdgmres_calculation_tau_MathFunction_1 = exp
    (c_gmres_fdgmres_calculation_tau_Subtract1_1);
MEASUREMENT_STOP(C_GMRES_FDGMRES_CALCULATION_TAU_MATHFUNCTION,0,0,C_GMRES_FDGMRES_CALCULATION_TAU_MATHFUNCTION_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_CALCULATION_TAU_SUBTRACT2,0,0,C_GMRES_FDGMRES_CALCULATION_TAU_SUBTRACT2_PARAM);
  /* Sum: '<S10>/Subtract2' */
  c_gmres_fdgmres_calculation_tau_Subtract2_1 =
    c_gmres_fdgmres_calculation_tau_Constant4_1 -
    c_gmres_fdgmres_calculation_tau_MathFunction_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_CALCULATION_TAU_SUBTRACT2,0,0,C_GMRES_FDGMRES_CALCULATION_TAU_SUBTRACT2_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_CALCULATION_TAU_GAIN2,0,0,C_GMRES_FDGMRES_CALCULATION_TAU_GAIN2_PARAM);
  /* Gain: '<S10>/Gain2' */
  c_gmres_fdgmres_calculation_tau_Gain2_1 = c_gmres_P.Gain2_Gain *
    c_gmres_fdgmres_calculation_tau_Subtract2_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_CALCULATION_TAU_GAIN2,0,0,C_GMRES_FDGMRES_CALCULATION_TAU_GAIN2_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_PREDICT_X_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_PREDICT_X_FORITERATORSUBSYSTEM_PARAM);
  /* Outputs for Iterator SubSystem: '<S54>/ForIteratorSubsystem' incorporates:
   *  ForIterator: '<S63>/ForIterator'
   */
  /* InitializeConditions for UnitDelay: '<S64>/UnitDelay' */
  c_gmres_DWork.UnitDelay_DSTATE_c[0] = c_gmres_P.UnitDelay_InitialCondition_ff;
  c_gmres_DWork.UnitDelay_DSTATE_c[1] = c_gmres_P.UnitDelay_InitialCondition_ff;
  for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
       c_gmres_P.ForIterator_IterationLimit_au; c_gmres_fdgmres_Initialize_mode
       ++) {
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_ForIterator_1
      = c_gmres_fdgmres_Initialize_mode;

    /* Constant: '<S63>/Constant' */
    memcpy
      (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Constant_1
       [0], &c_gmres_P.Constant_Value_ho[0], 40U * sizeof(real_T));

    /* Selector: '<S63>/Selector' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Selector_1
      [0] = c_gmres_Discrete_TimeIntegrator_1[(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Selector_1
      [1] = c_gmres_Discrete_TimeIntegrator_1[((i_0 - 1) << 1) + 1];

    /* Gain: '<S65>/Gain3' */
    Bias = 1.0;
    x_idx_1 = 1.0;
    y_idx_0 = c_gmres_P.tank_area[0];
    y_idx_1 = c_gmres_P.tank_area[1];
    x = Bias;
    x /= y_idx_0;
    Bias = x;
    x = x_idx_1;
    x /= y_idx_1;
    x_idx_1 = x;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [0] = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Selector_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [1] = x_idx_1 *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Selector_1
      [1];

    /* UnitDelay: '<S64>/UnitDelay' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
      [0] = c_gmres_DWork.UnitDelay_DSTATE_c[0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
      [1] = c_gmres_DWork.UnitDelay_DSTATE_c[1];

    /* Switch: '<S64>/Switch' */
    if (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_ForIterator_1
        > c_gmres_P.Switch_Threshold_l) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [0] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
        [0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [1] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
        [1];
    } else {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [0] = c_gmres_UnitDelay1_1[0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [1] = c_gmres_UnitDelay1_1[1];
    }

    /* End of Switch: '<S64>/Switch' */

    /* Reshape: '<S63>/Reshape1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Reshape1_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Reshape1_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [1];

    /* Saturate: '<S65>/Saturation' */
    Bias =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Reshape1_1
      [0];
    x_idx_1 = c_gmres_P.Saturation_LowerSat_k;
    y_idx_0 = c_gmres_P.Saturation_UpperSat_a;
    if (Bias > y_idx_0) {
      Bias = y_idx_0;
    } else {
      if (Bias < x_idx_1) {
        Bias = x_idx_1;
      }
    }

    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
      [0] = Bias;
    Bias =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Reshape1_1
      [1];
    x_idx_1 = c_gmres_P.Saturation_LowerSat_k;
    y_idx_0 = c_gmres_P.Saturation_UpperSat_a;
    if (Bias > y_idx_0) {
      Bias = y_idx_0;
    } else {
      if (Bias < x_idx_1) {
        Bias = x_idx_1;
      }
    }

    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
      [1] = Bias;

    /* End of Saturate: '<S65>/Saturation' */

    /* Sqrt: '<S65>/Sqrt' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [0] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
       [0]);
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [1] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
       [1]);

    /* Gain: '<S65>/Gain' */
    Bias = -c_gmres_P.C_plant[0];
    x_idx_1 = c_gmres_P.tank_area[0];
    Bias /= x_idx_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Gain_1
      = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [0];

    /* Gain: '<S65>/Gain1' */
    Bias = c_gmres_P.C_plant[0];
    x_idx_1 = c_gmres_P.tank_area[1];
    Bias /= x_idx_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
      = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [0];

    /* Gain: '<S65>/Gain2' */
    Bias = -c_gmres_P.C_plant[1];
    x_idx_1 = c_gmres_P.tank_area[1];
    Bias /= x_idx_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Gain2_1
      = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [1];

    /* Sum: '<S65>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Gain2_1;

    /* Sum: '<S65>/Add1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [0] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Gain_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [1] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Add_1;

    /* Gain: '<S63>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Gain_1
      = c_gmres_P.Gain_Gain_f * c_gmres_fdgmres_calculation_tau_Gain2_1;

    /* Product: '<S64>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Gain_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Gain_1;

    /* Sum: '<S64>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [0] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [1] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [1];

    /* Reshape: '<S63>/Reshape' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Reshape_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Reshape_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [1];

    /* Assignment: '<S63>/Assignment' */
    if (c_gmres_fdgmres_Initialize_mode == 1) {
      memcpy
        (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Assignment_1
         [0],
         &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Constant_1
         [0], 40U * sizeof(real_T));
    }

    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Assignment_1
      [(i_0 - 1) << 1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Reshape_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Assignment_1
      [1 + ((i_0 - 1) << 1)] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Reshape_1
      [1];

    /* End of Assignment: '<S63>/Assignment' */

    /* Update for UnitDelay: '<S64>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_c[0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [0];
    c_gmres_DWork.UnitDelay_DSTATE_c[1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [1];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_PREDICT_X_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_PREDICT_X_FORITERATORSUBSYSTEM_PARAM);

  /* End of Outputs for SubSystem: '<S54>/ForIteratorSubsystem' */

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S49>/ConcatBufferAtVectorConcatenateIn2' */
  memcpy
    (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_VectorConcatenate_1
     [2],
     &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Assignment_1
     [0], 40U * sizeof(real_T));
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_SELECTOR,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_SELECTOR_PARAM);
  /* Selector: '<S49>/Selector' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Selector_1[0]
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Assignment_1
    [38];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Selector_1[1]
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Assignment_1
    [39];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_SELECTOR,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_SELECTOR_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_SELECTOR1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_SELECTOR1_PARAM);
  /* Selector: '<S49>/Selector1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Selector1_1[0]
    = c_gmres_Subsystem_Selector1_1[38];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Selector1_1[1]
    = c_gmres_Subsystem_Selector1_1[39];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_SELECTOR1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_SELECTOR1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_SUBTRACT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_SUBTRACT_PARAM);
  /* Sum: '<S52>/Subtract' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Subtract_1
    [0] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Selector_1
    [0] -
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Selector1_1[
    0];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Subtract_1
    [1] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Selector_1
    [1] -
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Selector1_1[
    1];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_SUBTRACT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_SUBTRACT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_CONSTANT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_CONSTANT_PARAM);
  /* Constant: '<S52>/Constant' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Constant_1
    = c_gmres_P.Constant_Value_fo;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_CONSTANT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_CONSTANT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_PRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_PRODUCT_PARAM);
  /* Product: '<S52>/Product' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Product_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Subtract_1
    [0] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Constant_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_PRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_PRODUCT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_CONSTANT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_CONSTANT1_PARAM);
  /* Constant: '<S52>/Constant1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Constant1_1
    = c_gmres_P.Constant1_Value_jh;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_CONSTANT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_CONSTANT1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_PRODUCT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_PRODUCT1_PARAM);
  /* Product: '<S52>/Product1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Product1_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Subtract_1
    [1] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Constant1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_PRODUCT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_PRODUCT1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_ADD,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_ADD_PARAM);
  /* Sum: '<S52>/Add' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Add_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Product_1
    + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Product1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_ADD,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_ADD_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S52>/ConcatBufferAtVectorConcatenateIn1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_VectorConcatenate_1
    [0] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Add_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_CONSTANT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_CONSTANT2_PARAM);
  /* Constant: '<S52>/Constant2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Constant2_1
    = c_gmres_P.Constant2_Value_g;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_CONSTANT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_CONSTANT2_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_PRODUCT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_PRODUCT2_PARAM);
  /* Product: '<S52>/Product2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Product2_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Subtract_1
    [1] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Constant2_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_PRODUCT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_PRODUCT2_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_CONSTANT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_CONSTANT3_PARAM);
  /* Constant: '<S52>/Constant3' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Constant3_1
    = c_gmres_P.Constant3_Value_o;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_CONSTANT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_CONSTANT3_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_PRODUCT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_PRODUCT3_PARAM);
  /* Product: '<S52>/Product3' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Product3_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Subtract_1
    [0] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Constant3_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_PRODUCT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_PRODUCT3_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_ADD1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_ADD1_PARAM);
  /* Sum: '<S52>/Add1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Add1_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Product2_1
    + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Product3_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_ADD1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_ADD1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S52>/ConcatBufferAtVectorConcatenateIn2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_VectorConcatenate_1
    [1] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Add1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM_PARAM);
  /* Outputs for Iterator SubSystem: '<S51>/ForIteratorSubsystem' incorporates:
   *  ForIterator: '<S55>/ForIterator'
   */
  /* InitializeConditions for UnitDelay: '<S57>/UnitDelay' */
  c_gmres_DWork.UnitDelay_DSTATE_gm[0] = c_gmres_P.UnitDelay_InitialCondition_o;
  c_gmres_DWork.UnitDelay_DSTATE_gm[1] = c_gmres_P.UnitDelay_InitialCondition_o;
  for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
       c_gmres_P.ForIterator_IterationLimit_h; c_gmres_fdgmres_Initialize_mode++)
  {
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
      = c_gmres_fdgmres_Initialize_mode;

    /* Constant: '<S55>/Constant' */
    memcpy
      (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
       [0], &c_gmres_P.Constant_Value_k[0], 38U * sizeof(real_T));

    /* Constant: '<S58>/Q11' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
      = c_gmres_P.Q11_Value_b;

    /* Constant: '<S55>/Constant1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
      = c_gmres_P.Constant1_Value_i;

    /* Sum: '<S55>/Subtract' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
      - (real_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;

    /* Selector: '<S55>/Selector' */
    i_0 = (int32_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_VectorConcatenate_1
      [(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_VectorConcatenate_1
      [((i_0 - 1) << 1) + 1];

    /* Selector: '<S55>/Selector1' */
    i_0 = (int32_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [0] = c_gmres_Subsystem_Selector1_1[(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [1] = c_gmres_Subsystem_Selector1_1[((i_0 - 1) << 1) + 1];

    /* Sum: '<S56>/Subtract' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [0] -
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [1] -
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [1];

    /* Product: '<S58>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [0];

    /* Constant: '<S58>/Q12_Q21' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
      = c_gmres_P.Q12_Q21_Value_l;

    /* Product: '<S58>/Product1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [1];

    /* Sum: '<S58>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1;

    /* UnitDelay: '<S57>/UnitDelay' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
      [0] = c_gmres_DWork.UnitDelay_DSTATE_gm[0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
      [1] = c_gmres_DWork.UnitDelay_DSTATE_gm[1];

    /* Switch: '<S57>/Switch' */
    if (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
        > c_gmres_P.Switch_Threshold_g) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [0] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
        [0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [1] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
        [1];
    } else {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [0] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_VectorConcatenate_1
        [0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [1] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_VectorConcatenate_1
        [1];
    }

    /* End of Switch: '<S57>/Switch' */

    /* Constant: '<S58>/C1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
      = c_gmres_P.C_plant[0];

    /* Constant: '<S58>/A1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1
      = c_gmres_P.tank_area[0];

    /* Product: '<S58>/Divide' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1;

    /* Product: '<S58>/Product2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1;

    /* Constant: '<S58>/A2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1
      = c_gmres_P.tank_area[1];

    /* Product: '<S58>/Divide1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1;

    /* Product: '<S58>/Product3' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1;

    /* Sum: '<S58>/Add1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
      - c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1;

    /* Sqrt: '<S56>/Sqrt' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [0] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
       [0]);
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [1] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
       [1]);

    /* Product: '<S58>/Divide2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [0];

    /* Gain: '<S58>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1
      = c_gmres_P.Gain_Gain_ow *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1;

    /* Sum: '<S58>/Add2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1;

    /* SignalConversion: '<S56>/ConcatBufferAtVectorConcatenateIn1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1;

    /* Constant: '<S59>/Q22' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
      = c_gmres_P.Q22_Value_d;

    /* Product: '<S59>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [1];

    /* Constant: '<S59>/Q12_Q21' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
      = c_gmres_P.Q12_Q21_Value_j;

    /* Product: '<S59>/Product1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [0];

    /* Constant: '<S59>/C2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
      = c_gmres_P.C_plant[1];

    /* Constant: '<S59>/A2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
      = c_gmres_P.tank_area[1];

    /* Product: '<S59>/Product3' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [1] /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [1];

    /* Gain: '<S59>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1
      = c_gmres_P.Gain_Gain_g *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1;

    /* Sum: '<S59>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1
      =
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
       + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1)
      - c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1;

    /* SignalConversion: '<S56>/ConcatBufferAtVectorConcatenateIn2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1;

    /* Gain: '<S55>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1
      = c_gmres_P.Gain_Gain_p * c_gmres_fdgmres_calculation_tau_Gain2_1;

    /* Product: '<S57>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;

    /* Sum: '<S57>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [0] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [1] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [1];

    /* Reshape: '<S55>/Reshape' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [1];

    /* Constant: '<S55>/Constant2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
      = c_gmres_P.Constant2_Value_p;

    /* Sum: '<S55>/Subtract1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
      - (real_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;

    /* Assignment: '<S55>/Assignment' */
    if (c_gmres_fdgmres_Initialize_mode == 1) {
      memcpy
        (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
         [0],
         &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
         [0], 38U * sizeof(real_T));
    }

    i_0 = (int32_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
      [(i_0 - 1) << 1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
      [1 + ((i_0 - 1) << 1)] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [1];

    /* End of Assignment: '<S55>/Assignment' */

    /* Update for UnitDelay: '<S57>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_gm[0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [0];
    c_gmres_DWork.UnitDelay_DSTATE_gm[1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [1];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM_PARAM);

  /* End of Outputs for SubSystem: '<S51>/ForIteratorSubsystem' */

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_VECTORCONCATENATE1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_VECTORCONCATENATE1_PARAM);
  /* SignalConversion: '<S49>/ConcatBufferAtVectorConcatenate1In1' */
  memcpy
    (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_VectorConcatenate1_1
     [0],
     &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
     [0], 38U * sizeof(real_T));
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_VECTORCONCATENATE1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_VECTORCONCATENATE1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_VECTORCONCATENATE1,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_VECTORCONCATENATE1_PARAM);
  /* SignalConversion: '<S49>/ConcatBufferAtVectorConcatenate1In2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_VectorConcatenate1_1
    [38] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_VectorConcatenate_1
    [0];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_VectorConcatenate1_1
    [39] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_VectorConcatenate_1
    [1];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_VECTORCONCATENATE1,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_VECTORCONCATENATE1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_F_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_F_FORITERATORSUBSYSTEM_PARAM);
  /* Outputs for Iterator SubSystem: '<S53>/ForIteratorSubsystem' incorporates:
   *  ForIterator: '<S60>/ForIterator'
   */
  for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
       c_gmres_P.ForIterator_IterationLimit_m0; c_gmres_fdgmres_Initialize_mode
       ++) {
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_ForIterator_1
      = c_gmres_fdgmres_Initialize_mode;

    /* Selector: '<S60>/Selector' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Selector_1
      [0] = c_gmres_Discrete_TimeIntegrator_1[(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Selector_1
      [1] = c_gmres_Discrete_TimeIntegrator_1[((i_0 - 1) << 1) + 1];

    /* Constant: '<S61>/r1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_r1_1
      = 2.0 * c_gmres_P.p_idx_r[0];

    /* Product: '<S61>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Selector_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_r1_1;

    /* Selector: '<S60>/Selector1' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Selector1_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_VectorConcatenate1_1
      [(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Selector1_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_VectorConcatenate1_1
      [((i_0 - 1) << 1) + 1];

    /* Constant: '<S61>/A1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_A1_1
      = c_gmres_P.tank_area[0];

    /* Product: '<S61>/Divide' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_Divide_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Selector1_1
      [0] /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_A1_1;

    /* Sum: '<S61>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_Product_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_Divide_1;

    /* SignalConversion: '<S60>/ConcatBufferAtVectorConcatenateIn1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_VectorConcatenate_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_Add_1;

    /* Constant: '<S62>/r2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_r2_1
      = 2.0 * c_gmres_P.p_idx_r[1];

    /* Product: '<S62>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Selector_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_r2_1;

    /* Constant: '<S62>/A2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_A2_1
      = c_gmres_P.tank_area[1];

    /* Product: '<S62>/Divide' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_Divide_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Selector1_1
      [1] /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_A2_1;

    /* Sum: '<S62>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_Product_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_Divide_1;

    /* SignalConversion: '<S60>/ConcatBufferAtVectorConcatenateIn2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_VectorConcatenate_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_Add_1;
    for (i = 0; i < 40; i++) {
      /* Constant: '<S60>/Constant' */
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Constant_1
        [i] = c_gmres_P.Constant_Value_gc[i];

      /* Assignment: '<S60>/Assignment' */
      if (c_gmres_fdgmres_Initialize_mode == 1) {
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Assignment_1
          [i] =
          c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Constant_1
          [i];
      }
    }

    /* Assignment: '<S60>/Assignment' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Assignment_1
      [(i_0 - 1) << 1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_VectorConcatenate_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Assignment_1
      [1 + ((i_0 - 1) << 1)] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_VectorConcatenate_1
      [1];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_F_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL_F_FORITERATORSUBSYSTEM_PARAM);

  /* End of Outputs for SubSystem: '<S53>/ForIteratorSubsystem' */

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_CONSTANT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_CONSTANT_PARAM);
  /* Constant: '<S16>/Constant' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Constant_1 =
    c_gmres_P.delta_t;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_CONSTANT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_CONSTANT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_DIVIDE1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_DIVIDE1_PARAM);
  /* Product: '<S16>/Divide1' */
  for (i = 0; i < 40; i++) {
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Divide1_1[i] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Assignment_1
      [i] /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Constant_1;
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_DIVIDE1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_DIVIDE1_PARAM);

  /* End of Product: '<S16>/Divide1' */

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S66>/ConcatBufferAtVectorConcatenateIn1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_VectorConcatenate_1
    [0] = c_gmres_UnitDelay1_1[0];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_VectorConcatenate_1
    [1] = c_gmres_UnitDelay1_1[1];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_PREDICT_X_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_PREDICT_X_FORITERATORSUBSYSTEM_PARAM);
  /* Outputs for Iterator SubSystem: '<S71>/ForIteratorSubsystem' incorporates:
   *  ForIterator: '<S80>/ForIterator'
   */
  /* InitializeConditions for UnitDelay: '<S81>/UnitDelay' */
  c_gmres_DWork.UnitDelay_DSTATE_d5[0] = c_gmres_P.UnitDelay_InitialCondition_d1;
  c_gmres_DWork.UnitDelay_DSTATE_d5[1] = c_gmres_P.UnitDelay_InitialCondition_d1;
  for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
       c_gmres_P.ForIterator_IterationLimit_m2; c_gmres_fdgmres_Initialize_mode
       ++) {
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_ForIterator_1
      = c_gmres_fdgmres_Initialize_mode;

    /* Constant: '<S80>/Constant' */
    memcpy
      (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Constant_1
       [0], &c_gmres_P.Constant_Value_n2[0], 40U * sizeof(real_T));

    /* Selector: '<S80>/Selector' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Selector_1
      [0] = c_gmres_Discrete_TimeIntegrator_1[(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Selector_1
      [1] = c_gmres_Discrete_TimeIntegrator_1[((i_0 - 1) << 1) + 1];

    /* Gain: '<S82>/Gain3' */
    Bias = 1.0;
    x_idx_1 = 1.0;
    y_idx_0 = c_gmres_P.tank_area[0];
    y_idx_1 = c_gmres_P.tank_area[1];
    x = Bias;
    x /= y_idx_0;
    Bias = x;
    x = x_idx_1;
    x /= y_idx_1;
    x_idx_1 = x;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [0] = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Selector_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [1] = x_idx_1 *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Selector_1
      [1];

    /* UnitDelay: '<S81>/UnitDelay' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
      [0] = c_gmres_DWork.UnitDelay_DSTATE_d5[0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
      [1] = c_gmres_DWork.UnitDelay_DSTATE_d5[1];

    /* Switch: '<S81>/Switch' */
    if (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_ForIterator_1
        > c_gmres_P.Switch_Threshold_ao) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [0] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
        [0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [1] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
        [1];
    } else {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [0] = c_gmres_UnitDelay1_1[0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [1] = c_gmres_UnitDelay1_1[1];
    }

    /* End of Switch: '<S81>/Switch' */

    /* Reshape: '<S80>/Reshape1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Reshape1_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Reshape1_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [1];

    /* Saturate: '<S82>/Saturation' */
    Bias =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Reshape1_1
      [0];
    x_idx_1 = c_gmres_P.Saturation_LowerSat_m;
    y_idx_0 = c_gmres_P.Saturation_UpperSat_b;
    if (Bias > y_idx_0) {
      Bias = y_idx_0;
    } else {
      if (Bias < x_idx_1) {
        Bias = x_idx_1;
      }
    }

    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
      [0] = Bias;
    Bias =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Reshape1_1
      [1];
    x_idx_1 = c_gmres_P.Saturation_LowerSat_m;
    y_idx_0 = c_gmres_P.Saturation_UpperSat_b;
    if (Bias > y_idx_0) {
      Bias = y_idx_0;
    } else {
      if (Bias < x_idx_1) {
        Bias = x_idx_1;
      }
    }

    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
      [1] = Bias;

    /* End of Saturate: '<S82>/Saturation' */

    /* Sqrt: '<S82>/Sqrt' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [0] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
       [0]);
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [1] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
       [1]);

    /* Gain: '<S82>/Gain' */
    Bias = -c_gmres_P.C_plant[0];
    x_idx_1 = c_gmres_P.tank_area[0];
    Bias /= x_idx_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Gain_1
      = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [0];

    /* Gain: '<S82>/Gain1' */
    Bias = c_gmres_P.C_plant[0];
    x_idx_1 = c_gmres_P.tank_area[1];
    Bias /= x_idx_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
      = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [0];

    /* Gain: '<S82>/Gain2' */
    Bias = -c_gmres_P.C_plant[1];
    x_idx_1 = c_gmres_P.tank_area[1];
    Bias /= x_idx_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Gain2_1
      = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [1];

    /* Sum: '<S82>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Gain2_1;

    /* Sum: '<S82>/Add1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [0] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Gain_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [1] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Add_1;

    /* Gain: '<S80>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Gain_1
      = c_gmres_P.Gain_Gain_oe * c_gmres_fdgmres_calculation_tau_Gain2_1;

    /* Product: '<S81>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Gain_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Gain_1;

    /* Sum: '<S81>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [0] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [1] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [1];

    /* Reshape: '<S80>/Reshape' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Reshape_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Reshape_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [1];

    /* Assignment: '<S80>/Assignment' */
    if (c_gmres_fdgmres_Initialize_mode == 1) {
      memcpy
        (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Assignment_1
         [0],
         &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Constant_1
         [0], 40U * sizeof(real_T));
    }

    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Assignment_1
      [(i_0 - 1) << 1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Reshape_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Assignment_1
      [1 + ((i_0 - 1) << 1)] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Reshape_1
      [1];

    /* End of Assignment: '<S80>/Assignment' */

    /* Update for UnitDelay: '<S81>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_d5[0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [0];
    c_gmres_DWork.UnitDelay_DSTATE_d5[1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [1];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_PREDICT_X_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_PREDICT_X_FORITERATORSUBSYSTEM_PARAM);

  /* End of Outputs for SubSystem: '<S71>/ForIteratorSubsystem' */

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S66>/ConcatBufferAtVectorConcatenateIn2' */
  memcpy
    (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_VectorConcatenate_1
     [2],
     &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Assignment_1
     [0], 40U * sizeof(real_T));
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_SELECTOR,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_SELECTOR_PARAM);
  /* Selector: '<S66>/Selector' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Selector_1
    [0] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Assignment_1
    [38];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Selector_1
    [1] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Assignment_1
    [39];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_SELECTOR,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_SELECTOR_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_SELECTOR1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_SELECTOR1_PARAM);
  /* Selector: '<S66>/Selector1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Selector1_1
    [0] = c_gmres_Subsystem_Selector1_1[38];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Selector1_1
    [1] = c_gmres_Subsystem_Selector1_1[39];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_SELECTOR1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_SELECTOR1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_SUBTRACT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_SUBTRACT_PARAM);
  /* Sum: '<S69>/Subtract' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Subtract_1
    [0] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Selector_1
    [0] -
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Selector1_1
    [0];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Subtract_1
    [1] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Selector_1
    [1] -
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Selector1_1
    [1];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_SUBTRACT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_SUBTRACT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_CONSTANT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_CONSTANT_PARAM);
  /* Constant: '<S69>/Constant' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Constant_1
    = c_gmres_P.Constant_Value_ck;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_CONSTANT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_CONSTANT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_PRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_PRODUCT_PARAM);
  /* Product: '<S69>/Product' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Product_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Subtract_1
    [0] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Constant_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_PRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_PRODUCT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_CONSTANT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_CONSTANT1_PARAM);
  /* Constant: '<S69>/Constant1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Constant1_1
    = c_gmres_P.Constant1_Value_l;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_CONSTANT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_CONSTANT1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_PRODUCT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_PRODUCT1_PARAM);
  /* Product: '<S69>/Product1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Product1_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Subtract_1
    [1] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Constant1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_PRODUCT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_PRODUCT1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_ADD,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_ADD_PARAM);
  /* Sum: '<S69>/Add' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Add_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Product_1
    + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Product1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_ADD,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_ADD_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S69>/ConcatBufferAtVectorConcatenateIn1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_VectorConcatenate_1
    [0] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Add_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_CONSTANT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_CONSTANT2_PARAM);
  /* Constant: '<S69>/Constant2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Constant2_1
    = c_gmres_P.Constant2_Value_gw;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_CONSTANT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_CONSTANT2_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_PRODUCT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_PRODUCT2_PARAM);
  /* Product: '<S69>/Product2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Product2_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Subtract_1
    [1] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Constant2_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_PRODUCT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_PRODUCT2_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_CONSTANT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_CONSTANT3_PARAM);
  /* Constant: '<S69>/Constant3' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Constant3_1
    = c_gmres_P.Constant3_Value_c;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_CONSTANT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_CONSTANT3_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_PRODUCT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_PRODUCT3_PARAM);
  /* Product: '<S69>/Product3' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Product3_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Subtract_1
    [0] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Constant3_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_PRODUCT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_PRODUCT3_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_ADD1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_ADD1_PARAM);
  /* Sum: '<S69>/Add1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Add1_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Product2_1
    + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Product3_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_ADD1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_ADD1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S69>/ConcatBufferAtVectorConcatenateIn2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_VectorConcatenate_1
    [1] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Add1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM_PARAM);
  /* Outputs for Iterator SubSystem: '<S68>/ForIteratorSubsystem' incorporates:
   *  ForIterator: '<S72>/ForIterator'
   */
  /* InitializeConditions for UnitDelay: '<S74>/UnitDelay' */
  c_gmres_DWork.UnitDelay_DSTATE_b[0] = c_gmres_P.UnitDelay_InitialCondition_g;
  c_gmres_DWork.UnitDelay_DSTATE_b[1] = c_gmres_P.UnitDelay_InitialCondition_g;
  for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
       c_gmres_P.ForIterator_IterationLimit_ad; c_gmres_fdgmres_Initialize_mode
       ++) {
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
      = c_gmres_fdgmres_Initialize_mode;

    /* Constant: '<S72>/Constant' */
    memcpy
      (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
       [0], &c_gmres_P.Constant_Value_n[0], 38U * sizeof(real_T));

    /* Constant: '<S75>/Q11' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
      = c_gmres_P.Q11_Value_k;

    /* Constant: '<S72>/Constant1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
      = c_gmres_P.Constant1_Value_e;

    /* Sum: '<S72>/Subtract' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
      - (real_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;

    /* Selector: '<S72>/Selector' */
    i_0 = (int32_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_VectorConcatenate_1
      [(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_VectorConcatenate_1
      [((i_0 - 1) << 1) + 1];

    /* Selector: '<S72>/Selector1' */
    i_0 = (int32_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [0] = c_gmres_Subsystem_Selector1_1[(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [1] = c_gmres_Subsystem_Selector1_1[((i_0 - 1) << 1) + 1];

    /* Sum: '<S73>/Subtract' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [0] -
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [1] -
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [1];

    /* Product: '<S75>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [0];

    /* Constant: '<S75>/Q12_Q21' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
      = c_gmres_P.Q12_Q21_Value_b;

    /* Product: '<S75>/Product1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [1];

    /* Sum: '<S75>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1;

    /* UnitDelay: '<S74>/UnitDelay' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
      [0] = c_gmres_DWork.UnitDelay_DSTATE_b[0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
      [1] = c_gmres_DWork.UnitDelay_DSTATE_b[1];

    /* Switch: '<S74>/Switch' */
    if (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
        > c_gmres_P.Switch_Threshold_ag) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [0] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
        [0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [1] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
        [1];
    } else {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [0] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_VectorConcatenate_1
        [0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [1] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_VectorConcatenate_1
        [1];
    }

    /* End of Switch: '<S74>/Switch' */

    /* Constant: '<S75>/C1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
      = c_gmres_P.C_plant[0];

    /* Constant: '<S75>/A1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1
      = c_gmres_P.tank_area[0];

    /* Product: '<S75>/Divide' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1;

    /* Product: '<S75>/Product2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1;

    /* Constant: '<S75>/A2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1
      = c_gmres_P.tank_area[1];

    /* Product: '<S75>/Divide1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1;

    /* Product: '<S75>/Product3' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1;

    /* Sum: '<S75>/Add1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
      - c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1;

    /* Sqrt: '<S73>/Sqrt' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [0] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
       [0]);
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [1] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
       [1]);

    /* Product: '<S75>/Divide2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [0];

    /* Gain: '<S75>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1
      = c_gmres_P.Gain_Gain_fy *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1;

    /* Sum: '<S75>/Add2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1;

    /* SignalConversion: '<S73>/ConcatBufferAtVectorConcatenateIn1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1;

    /* Constant: '<S76>/Q22' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
      = c_gmres_P.Q22_Value_f;

    /* Product: '<S76>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [1];

    /* Constant: '<S76>/Q12_Q21' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
      = c_gmres_P.Q12_Q21_Value_o;

    /* Product: '<S76>/Product1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [0];

    /* Constant: '<S76>/C2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
      = c_gmres_P.C_plant[1];

    /* Constant: '<S76>/A2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
      = c_gmres_P.tank_area[1];

    /* Product: '<S76>/Product3' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [1] /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [1];

    /* Gain: '<S76>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1
      = c_gmres_P.Gain_Gain_m0 *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1;

    /* Sum: '<S76>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1
      =
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
       + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1)
      - c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1;

    /* SignalConversion: '<S73>/ConcatBufferAtVectorConcatenateIn2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1;

    /* Gain: '<S72>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1
      = c_gmres_P.Gain_Gain_j * c_gmres_fdgmres_calculation_tau_Gain2_1;

    /* Product: '<S74>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;

    /* Sum: '<S74>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [0] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [1] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [1];

    /* Reshape: '<S72>/Reshape' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [1];

    /* Constant: '<S72>/Constant2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
      = c_gmres_P.Constant2_Value_d;

    /* Sum: '<S72>/Subtract1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
      - (real_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;

    /* Assignment: '<S72>/Assignment' */
    if (c_gmres_fdgmres_Initialize_mode == 1) {
      memcpy
        (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
         [0],
         &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
         [0], 38U * sizeof(real_T));
    }

    i_0 = (int32_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
      [(i_0 - 1) << 1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
      [1 + ((i_0 - 1) << 1)] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [1];

    /* End of Assignment: '<S72>/Assignment' */

    /* Update for UnitDelay: '<S74>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_b[0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [0];
    c_gmres_DWork.UnitDelay_DSTATE_b[1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [1];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM_PARAM);

  /* End of Outputs for SubSystem: '<S68>/ForIteratorSubsystem' */

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_VECTORCONCATENATE1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_VECTORCONCATENATE1_PARAM);
  /* SignalConversion: '<S66>/ConcatBufferAtVectorConcatenate1In1' */
  memcpy
    (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_VectorConcatenate1_1
     [0],
     &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
     [0], 38U * sizeof(real_T));
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_VECTORCONCATENATE1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_VECTORCONCATENATE1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_VECTORCONCATENATE1,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_VECTORCONCATENATE1_PARAM);
  /* SignalConversion: '<S66>/ConcatBufferAtVectorConcatenate1In2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_VectorConcatenate1_1
    [38] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_VectorConcatenate_1
    [0];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_VectorConcatenate1_1
    [39] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_VectorConcatenate_1
    [1];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_VECTORCONCATENATE1,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_VECTORCONCATENATE1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_F_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_F_FORITERATORSUBSYSTEM_PARAM);
  /* Outputs for Iterator SubSystem: '<S70>/ForIteratorSubsystem' incorporates:
   *  ForIterator: '<S77>/ForIterator'
   */
  for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
       c_gmres_P.ForIterator_IterationLimit_ez; c_gmres_fdgmres_Initialize_mode
       ++) {
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_ForIterator_1
      = c_gmres_fdgmres_Initialize_mode;

    /* Selector: '<S77>/Selector' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Selector_1
      [0] = c_gmres_Discrete_TimeIntegrator_1[(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Selector_1
      [1] = c_gmres_Discrete_TimeIntegrator_1[((i_0 - 1) << 1) + 1];

    /* Constant: '<S78>/r1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_r1_1
      = 2.0 * c_gmres_P.p_idx_r[0];

    /* Product: '<S78>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Selector_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_r1_1;

    /* Selector: '<S77>/Selector1' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Selector1_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_VectorConcatenate1_1
      [(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Selector1_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_VectorConcatenate1_1
      [((i_0 - 1) << 1) + 1];

    /* Constant: '<S78>/A1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_A1_1
      = c_gmres_P.tank_area[0];

    /* Product: '<S78>/Divide' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_Divide_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Selector1_1
      [0] /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_A1_1;

    /* Sum: '<S78>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_Product_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_Divide_1;

    /* SignalConversion: '<S77>/ConcatBufferAtVectorConcatenateIn1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_VectorConcatenate_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_Add_1;

    /* Constant: '<S79>/r2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_r2_1
      = 2.0 * c_gmres_P.p_idx_r[1];

    /* Product: '<S79>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Selector_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_r2_1;

    /* Constant: '<S79>/A2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_A2_1
      = c_gmres_P.tank_area[1];

    /* Product: '<S79>/Divide' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_Divide_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Selector1_1
      [1] /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_A2_1;

    /* Sum: '<S79>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_Product_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_Divide_1;

    /* SignalConversion: '<S77>/ConcatBufferAtVectorConcatenateIn2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_VectorConcatenate_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_Add_1;
    for (i = 0; i < 40; i++) {
      /* Constant: '<S77>/Constant' */
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Constant_1
        [i] = c_gmres_P.Constant_Value_bt[i];

      /* Assignment: '<S77>/Assignment' */
      if (c_gmres_fdgmres_Initialize_mode == 1) {
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Assignment_1
          [i] =
          c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Constant_1
          [i];
      }
    }

    /* Assignment: '<S77>/Assignment' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Assignment_1
      [(i_0 - 1) << 1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_VectorConcatenate_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Assignment_1
      [1 + ((i_0 - 1) << 1)] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_VectorConcatenate_1
      [1];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_F_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL_F_FORITERATORSUBSYSTEM_PARAM);

  /* End of Outputs for SubSystem: '<S70>/ForIteratorSubsystem' */
MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ZERO,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ZERO_PARAM);
  for (i = 0; i < 40; i++) {
    /* Constant: '<S12>/zero' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_zero_1[i] =
      c_gmres_P.zero_Value_d[i];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ZERO,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ZERO_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_PRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_PRODUCT_PARAM);
  for (i = 0; i < 40; i++) {
    /* Product: '<S50>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Product_1[i]
      = c_gmres_fdgmres_Initialize_model_Calculateresidualvector_zero_1[i] *
      c_gmres_Constant1_1;
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_PRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_PRODUCT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_SUM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_SUM_PARAM);
  for (i = 0; i < 40; i++) {
    /* Sum: '<S50>/Sum' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Sum_1[i] =
      c_gmres_Discrete_TimeIntegrator_1[i] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Product_1
      [i];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_SUM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_SUM_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_PRODUCT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_PRODUCT1_PARAM);
  /* Product: '<S50>/Product1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Product1_1[0]
    = c_gmres_Gain_1[0] * c_gmres_Constant1_1;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Product1_1[1]
    = c_gmres_Gain_1[1] * c_gmres_Constant1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_PRODUCT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_PRODUCT1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_SUM1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_SUM1_PARAM);
  /* Sum: '<S50>/Sum1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Sum1_1[0] =
    c_gmres_UnitDelay1_1[0] +
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Product1_1[
    0];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Sum1_1[1] =
    c_gmres_UnitDelay1_1[1] +
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Product1_1[
    1];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_SUM1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_SUM1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S67>/ConcatBufferAtVectorConcatenateIn1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_VectorConcatenate_1
    [0] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Sum1_1[0];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_VectorConcatenate_1
    [1] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Sum1_1[1];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_PREDICT_X_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_PREDICT_X_FORITERATORSUBSYSTEM_PARAM);
  /* Outputs for Iterator SubSystem: '<S86>/ForIteratorSubsystem' incorporates:
   *  ForIterator: '<S95>/ForIterator'
   */
  /* InitializeConditions for UnitDelay: '<S96>/UnitDelay' */
  c_gmres_DWork.UnitDelay_DSTATE_g[0] = c_gmres_P.UnitDelay_InitialCondition_ff3;
  c_gmres_DWork.UnitDelay_DSTATE_g[1] = c_gmres_P.UnitDelay_InitialCondition_ff3;
  for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
       c_gmres_P.ForIterator_IterationLimit_p; c_gmres_fdgmres_Initialize_mode++)
  {
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_ForIterator_1
      = c_gmres_fdgmres_Initialize_mode;

    /* Constant: '<S95>/Constant' */
    memcpy
      (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Constant_1
       [0], &c_gmres_P.Constant_Value_p[0], 40U * sizeof(real_T));

    /* Selector: '<S95>/Selector' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Selector_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Sum_1
      [(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Selector_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Sum_1
      [((i_0 - 1) << 1) + 1];

    /* Gain: '<S97>/Gain3' */
    Bias = 1.0;
    x_idx_1 = 1.0;
    y_idx_0 = c_gmres_P.tank_area[0];
    y_idx_1 = c_gmres_P.tank_area[1];
    x = Bias;
    x /= y_idx_0;
    Bias = x;
    x = x_idx_1;
    x /= y_idx_1;
    x_idx_1 = x;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [0] = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Selector_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [1] = x_idx_1 *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Selector_1
      [1];

    /* UnitDelay: '<S96>/UnitDelay' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
      [0] = c_gmres_DWork.UnitDelay_DSTATE_g[0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
      [1] = c_gmres_DWork.UnitDelay_DSTATE_g[1];

    /* Switch: '<S96>/Switch' */
    if (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_ForIterator_1
        > c_gmres_P.Switch_Threshold_ol) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [0] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
        [0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [1] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
        [1];
    } else {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [0] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Sum1_1[
        0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [1] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Sum1_1[
        1];
    }

    /* End of Switch: '<S96>/Switch' */

    /* Reshape: '<S95>/Reshape1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [1];

    /* Saturate: '<S97>/Saturation' */
    Bias =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
      [0];
    x_idx_1 = c_gmres_P.Saturation_LowerSat_a;
    y_idx_0 = c_gmres_P.Saturation_UpperSat_o;
    if (Bias > y_idx_0) {
      Bias = y_idx_0;
    } else {
      if (Bias < x_idx_1) {
        Bias = x_idx_1;
      }
    }

    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
      [0] = Bias;
    Bias =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
      [1];
    x_idx_1 = c_gmres_P.Saturation_LowerSat_a;
    y_idx_0 = c_gmres_P.Saturation_UpperSat_o;
    if (Bias > y_idx_0) {
      Bias = y_idx_0;
    } else {
      if (Bias < x_idx_1) {
        Bias = x_idx_1;
      }
    }

    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
      [1] = Bias;

    /* End of Saturate: '<S97>/Saturation' */

    /* Sqrt: '<S97>/Sqrt' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [0] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
       [0]);
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [1] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
       [1]);

    /* Gain: '<S97>/Gain' */
    Bias = -c_gmres_P.C_plant[0];
    x_idx_1 = c_gmres_P.tank_area[0];
    Bias /= x_idx_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain_1
      = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [0];

    /* Gain: '<S97>/Gain1' */
    Bias = c_gmres_P.C_plant[0];
    x_idx_1 = c_gmres_P.tank_area[1];
    Bias /= x_idx_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
      = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [0];

    /* Gain: '<S97>/Gain2' */
    Bias = -c_gmres_P.C_plant[1];
    x_idx_1 = c_gmres_P.tank_area[1];
    Bias /= x_idx_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain2_1
      = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [1];

    /* Sum: '<S97>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain2_1;

    /* Sum: '<S97>/Add1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [0] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [1] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Add_1;

    /* Gain: '<S95>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Gain_1
      = c_gmres_P.Gain_Gain_c * c_gmres_fdgmres_calculation_tau_Gain2_1;

    /* Product: '<S96>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Gain_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Gain_1;

    /* Sum: '<S96>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [0] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [1] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [1];

    /* Reshape: '<S95>/Reshape' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Reshape_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Reshape_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [1];

    /* Assignment: '<S95>/Assignment' */
    if (c_gmres_fdgmres_Initialize_mode == 1) {
      memcpy
        (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Assignment_1
         [0],
         &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Constant_1
         [0], 40U * sizeof(real_T));
    }

    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Assignment_1
      [(i_0 - 1) << 1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Reshape_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Assignment_1
      [1 + ((i_0 - 1) << 1)] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Reshape_1
      [1];

    /* End of Assignment: '<S95>/Assignment' */

    /* Update for UnitDelay: '<S96>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_g[0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [0];
    c_gmres_DWork.UnitDelay_DSTATE_g[1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [1];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_PREDICT_X_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_PREDICT_X_FORITERATORSUBSYSTEM_PARAM);

  /* End of Outputs for SubSystem: '<S86>/ForIteratorSubsystem' */

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S67>/ConcatBufferAtVectorConcatenateIn2' */
  memcpy
    (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_VectorConcatenate_1
     [2],
     &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Assignment_1
     [0], 40U * sizeof(real_T));
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_SELECTOR,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_SELECTOR_PARAM);
  /* Selector: '<S67>/Selector' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Selector_1
    [0] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Assignment_1
    [38];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Selector_1
    [1] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Assignment_1
    [39];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_SELECTOR,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_SELECTOR_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_SELECTOR1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_SELECTOR1_PARAM);
  /* Selector: '<S67>/Selector1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Selector1_1
    [0] = c_gmres_Subsystem_Selector1_1[38];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Selector1_1
    [1] = c_gmres_Subsystem_Selector1_1[39];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_SELECTOR1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_SELECTOR1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_SUBTRACT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_SUBTRACT_PARAM);
  /* Sum: '<S84>/Subtract' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Subtract_1
    [0] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Selector_1
    [0] -
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Selector1_1
    [0];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Subtract_1
    [1] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Selector_1
    [1] -
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Selector1_1
    [1];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_SUBTRACT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_SUBTRACT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_CONSTANT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_CONSTANT_PARAM);
  /* Constant: '<S84>/Constant' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Constant_1
    = c_gmres_P.Constant_Value_dm;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_CONSTANT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_CONSTANT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_PRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_PRODUCT_PARAM);
  /* Product: '<S84>/Product' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Product_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Subtract_1
    [0] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Constant_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_PRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_PRODUCT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_CONSTANT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_CONSTANT1_PARAM);
  /* Constant: '<S84>/Constant1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Constant1_1
    = c_gmres_P.Constant1_Value_c;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_CONSTANT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_CONSTANT1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_PRODUCT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_PRODUCT1_PARAM);
  /* Product: '<S84>/Product1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Product1_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Subtract_1
    [1] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Constant1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_PRODUCT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_PRODUCT1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_ADD,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_ADD_PARAM);
  /* Sum: '<S84>/Add' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Add_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Product_1
    + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Product1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_ADD,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_ADD_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S84>/ConcatBufferAtVectorConcatenateIn1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_VectorConcatenate_1
    [0] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Add_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_CONSTANT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_CONSTANT2_PARAM);
  /* Constant: '<S84>/Constant2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Constant2_1
    = c_gmres_P.Constant2_Value_fl;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_CONSTANT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_CONSTANT2_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_PRODUCT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_PRODUCT2_PARAM);
  /* Product: '<S84>/Product2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Product2_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Subtract_1
    [1] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Constant2_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_PRODUCT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_PRODUCT2_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_CONSTANT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_CONSTANT3_PARAM);
  /* Constant: '<S84>/Constant3' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Constant3_1
    = c_gmres_P.Constant3_Value_ob;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_CONSTANT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_CONSTANT3_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_PRODUCT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_PRODUCT3_PARAM);
  /* Product: '<S84>/Product3' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Product3_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Subtract_1
    [0] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Constant3_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_PRODUCT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_PRODUCT3_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_ADD1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_ADD1_PARAM);
  /* Sum: '<S84>/Add1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Add1_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Product2_1
    + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Product3_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_ADD1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_ADD1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S84>/ConcatBufferAtVectorConcatenateIn2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_VectorConcatenate_1
    [1] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Add1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM_PARAM);
  /* Outputs for Iterator SubSystem: '<S83>/ForIteratorSubsystem' incorporates:
   *  ForIterator: '<S87>/ForIterator'
   */
  /* InitializeConditions for UnitDelay: '<S89>/UnitDelay' */
  c_gmres_DWork.UnitDelay_DSTATE_h[0] = c_gmres_P.UnitDelay_InitialCondition_k;
  c_gmres_DWork.UnitDelay_DSTATE_h[1] = c_gmres_P.UnitDelay_InitialCondition_k;
  for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
       c_gmres_P.ForIterator_IterationLimit_fu; c_gmres_fdgmres_Initialize_mode
       ++) {
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
      = c_gmres_fdgmres_Initialize_mode;

    /* Constant: '<S87>/Constant' */
    memcpy
      (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
       [0], &c_gmres_P.Constant_Value_d1[0], 38U * sizeof(real_T));

    /* Constant: '<S90>/Q11' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
      = c_gmres_P.Q11_Value_kw;

    /* Constant: '<S87>/Constant1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
      = c_gmres_P.Constant1_Value_a;

    /* Sum: '<S87>/Subtract' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
      - (real_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;

    /* Selector: '<S87>/Selector' */
    i_0 = (int32_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_VectorConcatenate_1
      [(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_VectorConcatenate_1
      [((i_0 - 1) << 1) + 1];

    /* Selector: '<S87>/Selector1' */
    i_0 = (int32_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [0] = c_gmres_Subsystem_Selector1_1[(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [1] = c_gmres_Subsystem_Selector1_1[((i_0 - 1) << 1) + 1];

    /* Sum: '<S88>/Subtract' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [0] -
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [1] -
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [1];

    /* Product: '<S90>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [0];

    /* Constant: '<S90>/Q12_Q21' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
      = c_gmres_P.Q12_Q21_Value_bw;

    /* Product: '<S90>/Product1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [1];

    /* Sum: '<S90>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1;

    /* UnitDelay: '<S89>/UnitDelay' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
      [0] = c_gmres_DWork.UnitDelay_DSTATE_h[0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
      [1] = c_gmres_DWork.UnitDelay_DSTATE_h[1];

    /* Switch: '<S89>/Switch' */
    if (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
        > c_gmres_P.Switch_Threshold_f) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [0] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
        [0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [1] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
        [1];
    } else {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [0] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_VectorConcatenate_1
        [0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [1] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_VectorConcatenate_1
        [1];
    }

    /* End of Switch: '<S89>/Switch' */

    /* Constant: '<S90>/C1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
      = c_gmres_P.C_plant[0];

    /* Constant: '<S90>/A1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1
      = c_gmres_P.tank_area[0];

    /* Product: '<S90>/Divide' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1;

    /* Product: '<S90>/Product2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1;

    /* Constant: '<S90>/A2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1
      = c_gmres_P.tank_area[1];

    /* Product: '<S90>/Divide1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1;

    /* Product: '<S90>/Product3' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1;

    /* Sum: '<S90>/Add1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
      - c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1;

    /* Sqrt: '<S88>/Sqrt' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [0] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
       [0]);
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [1] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
       [1]);

    /* Product: '<S90>/Divide2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [0];

    /* Gain: '<S90>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1
      = c_gmres_P.Gain_Gain_nm *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1;

    /* Sum: '<S90>/Add2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1;

    /* SignalConversion: '<S88>/ConcatBufferAtVectorConcatenateIn1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1;

    /* Constant: '<S91>/Q22' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
      = c_gmres_P.Q22_Value_fw;

    /* Product: '<S91>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [1];

    /* Constant: '<S91>/Q12_Q21' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
      = c_gmres_P.Q12_Q21_Value_m;

    /* Product: '<S91>/Product1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [0];

    /* Constant: '<S91>/C2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
      = c_gmres_P.C_plant[1];

    /* Constant: '<S91>/A2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
      = c_gmres_P.tank_area[1];

    /* Product: '<S91>/Product3' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [1] /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [1];

    /* Gain: '<S91>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1
      = c_gmres_P.Gain_Gain_gs *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1;

    /* Sum: '<S91>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1
      =
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
       + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1)
      - c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1;

    /* SignalConversion: '<S88>/ConcatBufferAtVectorConcatenateIn2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1;

    /* Gain: '<S87>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Gain_1
      = c_gmres_P.Gain_Gain_dl * c_gmres_fdgmres_calculation_tau_Gain2_1;

    /* Product: '<S89>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;

    /* Sum: '<S89>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [0] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [1] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [1];

    /* Reshape: '<S87>/Reshape' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [1];

    /* Constant: '<S87>/Constant2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
      = c_gmres_P.Constant2_Value_c;

    /* Sum: '<S87>/Subtract1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
      - (real_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;

    /* Assignment: '<S87>/Assignment' */
    if (c_gmres_fdgmres_Initialize_mode == 1) {
      memcpy
        (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
         [0],
         &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
         [0], 38U * sizeof(real_T));
    }

    i_0 = (int32_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
      [(i_0 - 1) << 1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
      [1 + ((i_0 - 1) << 1)] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [1];

    /* End of Assignment: '<S87>/Assignment' */

    /* Update for UnitDelay: '<S89>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_h[0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [0];
    c_gmres_DWork.UnitDelay_DSTATE_h[1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [1];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM_PARAM);

  /* End of Outputs for SubSystem: '<S83>/ForIteratorSubsystem' */

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_VECTORCONCATENATE1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_VECTORCONCATENATE1_PARAM);
  /* SignalConversion: '<S67>/ConcatBufferAtVectorConcatenate1In1' */
  memcpy
    (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_VectorConcatenate1_1
     [0],
     &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
     [0], 38U * sizeof(real_T));
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_VECTORCONCATENATE1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_VECTORCONCATENATE1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_VECTORCONCATENATE1,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_VECTORCONCATENATE1_PARAM);
  /* SignalConversion: '<S67>/ConcatBufferAtVectorConcatenate1In2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_VectorConcatenate1_1
    [38] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_VectorConcatenate_1
    [0];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_VectorConcatenate1_1
    [39] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_VectorConcatenate_1
    [1];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_VECTORCONCATENATE1,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_VECTORCONCATENATE1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_F_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_F_FORITERATORSUBSYSTEM_PARAM);
  /* Outputs for Iterator SubSystem: '<S85>/ForIteratorSubsystem' incorporates:
   *  ForIterator: '<S92>/ForIterator'
   */
  for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
       c_gmres_P.ForIterator_IterationLimit_i; c_gmres_fdgmres_Initialize_mode++)
  {
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_ForIterator_1
      = c_gmres_fdgmres_Initialize_mode;

    /* Selector: '<S92>/Selector' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Selector_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Sum_1
      [(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Selector_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Sum_1
      [((i_0 - 1) << 1) + 1];

    /* Constant: '<S93>/r1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_r1_1
      = 2.0 * c_gmres_P.p_idx_r[0];

    /* Product: '<S93>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Selector_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_r1_1;

    /* Selector: '<S92>/Selector1' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Selector1_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_VectorConcatenate1_1
      [(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Selector1_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_VectorConcatenate1_1
      [((i_0 - 1) << 1) + 1];

    /* Constant: '<S93>/A1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_A1_1
      = c_gmres_P.tank_area[0];

    /* Product: '<S93>/Divide' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_Divide_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Selector1_1
      [0] /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_A1_1;

    /* Sum: '<S93>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_Product_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_Divide_1;

    /* SignalConversion: '<S92>/ConcatBufferAtVectorConcatenateIn1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_Add_1;

    /* Constant: '<S94>/r2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_r2_1
      = 2.0 * c_gmres_P.p_idx_r[1];

    /* Product: '<S94>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Selector_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_r2_1;

    /* Constant: '<S94>/A2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_A2_1
      = c_gmres_P.tank_area[1];

    /* Product: '<S94>/Divide' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_Divide_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Selector1_1
      [1] /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_A2_1;

    /* Sum: '<S94>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_Product_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_Divide_1;

    /* SignalConversion: '<S92>/ConcatBufferAtVectorConcatenateIn2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_Add_1;
    for (i = 0; i < 40; i++) {
      /* Constant: '<S92>/Constant' */
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Constant_1
        [i] = c_gmres_P.Constant_Value_f[i];

      /* Assignment: '<S92>/Assignment' */
      if (c_gmres_fdgmres_Initialize_mode == 1) {
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Assignment_1
          [i] =
          c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Constant_1
          [i];
      }
    }

    /* Assignment: '<S92>/Assignment' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Assignment_1
      [(i_0 - 1) << 1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Assignment_1
      [1 + ((i_0 - 1) << 1)] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
      [1];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_F_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_MODEL1_F_FORITERATORSUBSYSTEM_PARAM);

  /* End of Outputs for SubSystem: '<S85>/ForIteratorSubsystem' */
MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_SUBTRACT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_SUBTRACT_PARAM);
  for (i = 0; i < 40; i++) {
    /* Sum: '<S50>/Subtract' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Subtract_1[
      i] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Assignment_1
      [i] -
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Assignment_1
      [i];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_SUBTRACT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_SUBTRACT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_DIVIDE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_DIVIDE_PARAM);
  for (i = 0; i < 40; i++) {
    /* Product: '<S50>/Divide' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Divide_1[i]
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Subtract_1
      [i] / c_gmres_Constant1_1;
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_DIVIDE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_DIVIDE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_SUBTRACT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_SUBTRACT_PARAM);
  for (i = 0; i < 40; i++) {
    /* Sum: '<S16>/Subtract' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Subtract_1[i] =
      (0.0 -
       c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Divide1_1[i])
      - c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Divide_1
      [i];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_SUBTRACT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_SUBTRACT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_SUBSYSTEM_PRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_SUBSYSTEM_PRODUCT_PARAM);
  /* Product: '<S15>/Product' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subsystem_Product_1[0]
    = c_gmres_Gain_1[0] * c_gmres_Constant1_1;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subsystem_Product_1[1]
    = c_gmres_Gain_1[1] * c_gmres_Constant1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_SUBSYSTEM_PRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_SUBSYSTEM_PRODUCT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_SUBSYSTEM_ADD,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_SUBSYSTEM_ADD_PARAM);
  /* Sum: '<S15>/Add' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subsystem_Add_1[0] =
    c_gmres_UnitDelay1_1[0] +
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subsystem_Product_1[
    0];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subsystem_Add_1[1] =
    c_gmres_UnitDelay1_1[1] +
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subsystem_Product_1[
    1];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_SUBSYSTEM_ADD,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_SUBSYSTEM_ADD_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S17>/ConcatBufferAtVectorConcatenateIn1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_VectorConcatenate_1
    [0] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subsystem_Add_1[0];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_VectorConcatenate_1
    [1] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subsystem_Add_1[1];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_PREDICT_X_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_PREDICT_X_FORITERATORSUBSYSTEM_PARAM);
  /* Outputs for Iterator SubSystem: '<S22>/ForIteratorSubsystem' incorporates:
   *  ForIterator: '<S31>/ForIterator'
   */
  /* InitializeConditions for UnitDelay: '<S32>/UnitDelay' */
  c_gmres_DWork.UnitDelay_DSTATE_gz[0] = c_gmres_P.UnitDelay_InitialCondition_f;
  c_gmres_DWork.UnitDelay_DSTATE_gz[1] = c_gmres_P.UnitDelay_InitialCondition_f;
  for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
       c_gmres_P.ForIterator_IterationLimit_f; c_gmres_fdgmres_Initialize_mode++)
  {
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_ForIterator_1
      = c_gmres_fdgmres_Initialize_mode;

    /* Constant: '<S31>/Constant' */
    memcpy
      (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Constant_1
       [0], &c_gmres_P.Constant_Value_c[0], 40U * sizeof(real_T));

    /* Selector: '<S31>/Selector' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Selector_1
      [0] = c_gmres_Discrete_TimeIntegrator_1[(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Selector_1
      [1] = c_gmres_Discrete_TimeIntegrator_1[((i_0 - 1) << 1) + 1];

    /* Gain: '<S33>/Gain3' */
    Bias = 1.0;
    x_idx_1 = 1.0;
    y_idx_0 = c_gmres_P.tank_area[0];
    y_idx_1 = c_gmres_P.tank_area[1];
    x = Bias;
    x /= y_idx_0;
    Bias = x;
    x = x_idx_1;
    x /= y_idx_1;
    x_idx_1 = x;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [0] = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Selector_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [1] = x_idx_1 *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Selector_1
      [1];

    /* UnitDelay: '<S32>/UnitDelay' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
      [0] = c_gmres_DWork.UnitDelay_DSTATE_gz[0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
      [1] = c_gmres_DWork.UnitDelay_DSTATE_gz[1];

    /* Switch: '<S32>/Switch' */
    if (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_ForIterator_1
        > c_gmres_P.Switch_Threshold_a) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [0] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
        [0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [1] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
        [1];
    } else {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [0] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subsystem_Add_1[
        0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [1] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subsystem_Add_1[
        1];
    }

    /* End of Switch: '<S32>/Switch' */

    /* Reshape: '<S31>/Reshape1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Reshape1_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Reshape1_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [1];

    /* Saturate: '<S33>/Saturation' */
    Bias =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Reshape1_1
      [0];
    x_idx_1 = c_gmres_P.Saturation_LowerSat;
    y_idx_0 = c_gmres_P.Saturation_UpperSat;
    if (Bias > y_idx_0) {
      Bias = y_idx_0;
    } else {
      if (Bias < x_idx_1) {
        Bias = x_idx_1;
      }
    }

    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
      [0] = Bias;
    Bias =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Reshape1_1
      [1];
    x_idx_1 = c_gmres_P.Saturation_LowerSat;
    y_idx_0 = c_gmres_P.Saturation_UpperSat;
    if (Bias > y_idx_0) {
      Bias = y_idx_0;
    } else {
      if (Bias < x_idx_1) {
        Bias = x_idx_1;
      }
    }

    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
      [1] = Bias;

    /* End of Saturate: '<S33>/Saturation' */

    /* Sqrt: '<S33>/Sqrt' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [0] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
       [0]);
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [1] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
       [1]);

    /* Gain: '<S33>/Gain' */
    Bias = -c_gmres_P.C_plant[0];
    x_idx_1 = c_gmres_P.tank_area[0];
    Bias /= x_idx_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain_1
      = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [0];

    /* Gain: '<S33>/Gain1' */
    Bias = c_gmres_P.C_plant[0];
    x_idx_1 = c_gmres_P.tank_area[1];
    Bias /= x_idx_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
      = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [0];

    /* Gain: '<S33>/Gain2' */
    Bias = -c_gmres_P.C_plant[1];
    x_idx_1 = c_gmres_P.tank_area[1];
    Bias /= x_idx_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain2_1
      = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [1];

    /* Sum: '<S33>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain2_1;

    /* Sum: '<S33>/Add1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [0] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [1] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add_1;

    /* Gain: '<S31>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Gain_1
      = c_gmres_P.Gain_Gain_d * c_gmres_fdgmres_calculation_tau_Gain2_1;

    /* Product: '<S32>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Gain_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Gain_1;

    /* Sum: '<S32>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [0] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [1] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [1];

    /* Reshape: '<S31>/Reshape' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Reshape_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Reshape_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [1];

    /* Assignment: '<S31>/Assignment' */
    if (c_gmres_fdgmres_Initialize_mode == 1) {
      memcpy
        (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Assignment_1
         [0],
         &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Constant_1
         [0], 40U * sizeof(real_T));
    }

    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Assignment_1
      [(i_0 - 1) << 1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Reshape_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Assignment_1
      [1 + ((i_0 - 1) << 1)] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Reshape_1
      [1];

    /* End of Assignment: '<S31>/Assignment' */

    /* Update for UnitDelay: '<S32>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_gz[0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [0];
    c_gmres_DWork.UnitDelay_DSTATE_gz[1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [1];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_PREDICT_X_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_PREDICT_X_FORITERATORSUBSYSTEM_PARAM);

  /* End of Outputs for SubSystem: '<S22>/ForIteratorSubsystem' */

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S17>/ConcatBufferAtVectorConcatenateIn2' */
  memcpy
    (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_VectorConcatenate_1
     [2],
     &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Assignment_1
     [0], 40U * sizeof(real_T));
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_SELECTOR,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_SELECTOR_PARAM);
  /* Selector: '<S17>/Selector' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Selector_1
    [0] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Assignment_1
    [38];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Selector_1
    [1] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Assignment_1
    [39];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_SELECTOR,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_SELECTOR_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_SELECTOR1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_SELECTOR1_PARAM);
  /* Selector: '<S17>/Selector1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Selector1_1
    [0] = c_gmres_Subsystem_Selector1_1[38];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Selector1_1
    [1] = c_gmres_Subsystem_Selector1_1[39];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_SELECTOR1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_SELECTOR1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_SUBTRACT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_SUBTRACT_PARAM);
  /* Sum: '<S20>/Subtract' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Subtract_1
    [0] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Selector_1
    [0] -
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Selector1_1
    [0];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Subtract_1
    [1] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Selector_1
    [1] -
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Selector1_1
    [1];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_SUBTRACT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_SUBTRACT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_CONSTANT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_CONSTANT_PARAM);
  /* Constant: '<S20>/Constant' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Constant_1
    = c_gmres_P.Constant_Value_dq;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_CONSTANT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_CONSTANT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_PRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_PRODUCT_PARAM);
  /* Product: '<S20>/Product' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Product_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Subtract_1
    [0] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Constant_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_PRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_PRODUCT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_CONSTANT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_CONSTANT1_PARAM);
  /* Constant: '<S20>/Constant1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Constant1_1
    = c_gmres_P.Constant1_Value_p;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_CONSTANT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_CONSTANT1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_PRODUCT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_PRODUCT1_PARAM);
  /* Product: '<S20>/Product1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Product1_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Subtract_1
    [1] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Constant1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_PRODUCT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_PRODUCT1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_ADD,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_ADD_PARAM);
  /* Sum: '<S20>/Add' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Add_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Product_1
    + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Product1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_ADD,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_ADD_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S20>/ConcatBufferAtVectorConcatenateIn1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_VectorConcatenate_1
    [0] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Add_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_CONSTANT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_CONSTANT2_PARAM);
  /* Constant: '<S20>/Constant2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Constant2_1
    = c_gmres_P.Constant2_Value_eh;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_CONSTANT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_CONSTANT2_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_PRODUCT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_PRODUCT2_PARAM);
  /* Product: '<S20>/Product2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Product2_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Subtract_1
    [1] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Constant2_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_PRODUCT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_PRODUCT2_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_CONSTANT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_CONSTANT3_PARAM);
  /* Constant: '<S20>/Constant3' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Constant3_1
    = c_gmres_P.Constant3_Value_n;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_CONSTANT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_CONSTANT3_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_PRODUCT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_PRODUCT3_PARAM);
  /* Product: '<S20>/Product3' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Product3_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Subtract_1
    [0] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Constant3_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_PRODUCT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_PRODUCT3_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_ADD1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_ADD1_PARAM);
  /* Sum: '<S20>/Add1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Add1_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Product2_1
    + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Product3_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_ADD1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_ADD1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S20>/ConcatBufferAtVectorConcatenateIn2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_VectorConcatenate_1
    [1] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Add1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM_PARAM);
  /* Outputs for Iterator SubSystem: '<S19>/ForIteratorSubsystem' incorporates:
   *  ForIterator: '<S23>/ForIterator'
   */
  /* InitializeConditions for UnitDelay: '<S25>/UnitDelay' */
  c_gmres_DWork.UnitDelay_DSTATE_cn[0] = c_gmres_P.UnitDelay_InitialCondition_a;
  c_gmres_DWork.UnitDelay_DSTATE_cn[1] = c_gmres_P.UnitDelay_InitialCondition_a;
  for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
       c_gmres_P.ForIterator_IterationLimit_o; c_gmres_fdgmres_Initialize_mode++)
  {
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
      = c_gmres_fdgmres_Initialize_mode;

    /* Constant: '<S23>/Constant' */
    memcpy
      (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
       [0], &c_gmres_P.Constant_Value[0], 38U * sizeof(real_T));

    /* Constant: '<S26>/Q11' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
      = c_gmres_P.Q11_Value;

    /* Constant: '<S23>/Constant1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
      = c_gmres_P.Constant1_Value;

    /* Sum: '<S23>/Subtract' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
      - (real_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;

    /* Selector: '<S23>/Selector' */
    i_0 = (int32_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_VectorConcatenate_1
      [(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_VectorConcatenate_1
      [((i_0 - 1) << 1) + 1];

    /* Selector: '<S23>/Selector1' */
    i_0 = (int32_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [0] = c_gmres_Subsystem_Selector1_1[(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [1] = c_gmres_Subsystem_Selector1_1[((i_0 - 1) << 1) + 1];

    /* Sum: '<S24>/Subtract' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [0] -
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [1] -
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [1];

    /* Product: '<S26>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [0];

    /* Constant: '<S26>/Q12_Q21' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
      = c_gmres_P.Q12_Q21_Value;

    /* Product: '<S26>/Product1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [1];

    /* Sum: '<S26>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1;

    /* UnitDelay: '<S25>/UnitDelay' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
      [0] = c_gmres_DWork.UnitDelay_DSTATE_cn[0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
      [1] = c_gmres_DWork.UnitDelay_DSTATE_cn[1];

    /* Switch: '<S25>/Switch' */
    if (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
        > c_gmres_P.Switch_Threshold_k) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [0] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
        [0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [1] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
        [1];
    } else {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [0] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_VectorConcatenate_1
        [0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [1] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_VectorConcatenate_1
        [1];
    }

    /* End of Switch: '<S25>/Switch' */

    /* Constant: '<S26>/C1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
      = c_gmres_P.C_plant[0];

    /* Constant: '<S26>/A1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1
      = c_gmres_P.tank_area[0];

    /* Product: '<S26>/Divide' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1;

    /* Product: '<S26>/Product2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1;

    /* Constant: '<S26>/A2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1
      = c_gmres_P.tank_area[1];

    /* Product: '<S26>/Divide1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1;

    /* Product: '<S26>/Product3' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1;

    /* Sum: '<S26>/Add1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
      - c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1;

    /* Sqrt: '<S24>/Sqrt' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [0] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
       [0]);
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [1] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
       [1]);

    /* Product: '<S26>/Divide2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [0];

    /* Gain: '<S26>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1
      = c_gmres_P.Gain_Gain *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1;

    /* Sum: '<S26>/Add2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1;

    /* SignalConversion: '<S24>/ConcatBufferAtVectorConcatenateIn1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1;

    /* Constant: '<S27>/Q22' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
      = c_gmres_P.Q22_Value;

    /* Product: '<S27>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [1];

    /* Constant: '<S27>/Q12_Q21' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
      = c_gmres_P.Q12_Q21_Value_g;

    /* Product: '<S27>/Product1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [0];

    /* Constant: '<S27>/C2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
      = c_gmres_P.C_plant[1];

    /* Constant: '<S27>/A2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
      = c_gmres_P.tank_area[1];

    /* Product: '<S27>/Product3' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [1] /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [1];

    /* Gain: '<S27>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1
      = c_gmres_P.Gain_Gain_n *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1;

    /* Sum: '<S27>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1
      =
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
       + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1)
      - c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1;

    /* SignalConversion: '<S24>/ConcatBufferAtVectorConcatenateIn2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1;

    /* Gain: '<S23>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1
      = c_gmres_P.Gain_Gain_m * c_gmres_fdgmres_calculation_tau_Gain2_1;

    /* Product: '<S25>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;

    /* Sum: '<S25>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [0] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [1] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [1];

    /* Reshape: '<S23>/Reshape' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [1];

    /* Constant: '<S23>/Constant2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
      = c_gmres_P.Constant2_Value;

    /* Sum: '<S23>/Subtract1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
      - (real_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;

    /* Assignment: '<S23>/Assignment' */
    if (c_gmres_fdgmres_Initialize_mode == 1) {
      memcpy
        (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
         [0],
         &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
         [0], 38U * sizeof(real_T));
    }

    i_0 = (int32_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
      [(i_0 - 1) << 1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
      [1 + ((i_0 - 1) << 1)] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [1];

    /* End of Assignment: '<S23>/Assignment' */

    /* Update for UnitDelay: '<S25>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_cn[0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [0];
    c_gmres_DWork.UnitDelay_DSTATE_cn[1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [1];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM_PARAM);

  /* End of Outputs for SubSystem: '<S19>/ForIteratorSubsystem' */

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_VECTORCONCATENATE1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_VECTORCONCATENATE1_PARAM);
  /* SignalConversion: '<S17>/ConcatBufferAtVectorConcatenate1In1' */
  memcpy
    (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_VectorConcatenate1_1
     [0],
     &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
     [0], 38U * sizeof(real_T));
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_VECTORCONCATENATE1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_VECTORCONCATENATE1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_VECTORCONCATENATE1,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_VECTORCONCATENATE1_PARAM);
  /* SignalConversion: '<S17>/ConcatBufferAtVectorConcatenate1In2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_VectorConcatenate1_1
    [38] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_VectorConcatenate_1
    [0];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_VectorConcatenate1_1
    [39] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_VectorConcatenate_1
    [1];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_VECTORCONCATENATE1,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_VECTORCONCATENATE1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_F_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_F_FORITERATORSUBSYSTEM_PARAM);
  /* Outputs for Iterator SubSystem: '<S21>/ForIteratorSubsystem' incorporates:
   *  ForIterator: '<S28>/ForIterator'
   */
  for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
       c_gmres_P.ForIterator_IterationLimit_e; c_gmres_fdgmres_Initialize_mode++)
  {
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_ForIterator_1
      = c_gmres_fdgmres_Initialize_mode;

    /* Selector: '<S28>/Selector' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Selector_1
      [0] = c_gmres_Discrete_TimeIntegrator_1[(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Selector_1
      [1] = c_gmres_Discrete_TimeIntegrator_1[((i_0 - 1) << 1) + 1];

    /* Constant: '<S29>/r1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_r1_1
      = 2.0 * c_gmres_P.p_idx_r[0];

    /* Product: '<S29>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Selector_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_r1_1;

    /* Selector: '<S28>/Selector1' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Selector1_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_VectorConcatenate1_1
      [(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Selector1_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_VectorConcatenate1_1
      [((i_0 - 1) << 1) + 1];

    /* Constant: '<S29>/A1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_A1_1
      = c_gmres_P.tank_area[0];

    /* Product: '<S29>/Divide' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_Divide_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Selector1_1
      [0] /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_A1_1;

    /* Sum: '<S29>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_Product_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_Divide_1;

    /* SignalConversion: '<S28>/ConcatBufferAtVectorConcatenateIn1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_VectorConcatenate_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_Add_1;

    /* Constant: '<S30>/r2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_r2_1
      = 2.0 * c_gmres_P.p_idx_r[1];

    /* Product: '<S30>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Selector_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_r2_1;

    /* Constant: '<S30>/A2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_A2_1
      = c_gmres_P.tank_area[1];

    /* Product: '<S30>/Divide' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_Divide_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Selector1_1
      [1] /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_A2_1;

    /* Sum: '<S30>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_Product_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_Divide_1;

    /* SignalConversion: '<S28>/ConcatBufferAtVectorConcatenateIn2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_VectorConcatenate_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_Add_1;
    for (i = 0; i < 40; i++) {
      /* Constant: '<S28>/Constant' */
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Constant_1
        [i] = c_gmres_P.Constant_Value_g[i];

      /* Assignment: '<S28>/Assignment' */
      if (c_gmres_fdgmres_Initialize_mode == 1) {
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Assignment_1
          [i] =
          c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Constant_1
          [i];
      }
    }

    /* Assignment: '<S28>/Assignment' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Assignment_1
      [(i_0 - 1) << 1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_VectorConcatenate_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Assignment_1
      [1 + ((i_0 - 1) << 1)] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_VectorConcatenate_1
      [1];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_F_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL_F_FORITERATORSUBSYSTEM_PARAM);

  /* End of Outputs for SubSystem: '<S21>/ForIteratorSubsystem' */
MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_PRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_PRODUCT_PARAM);
  for (i = 0; i < 40; i++) {
    /* Product: '<S14>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Product_1[i] =
      c_gmres_UnitDelay2_1[i] * c_gmres_Constant1_1;
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_PRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_PRODUCT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_SUM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_SUM_PARAM);
  for (i = 0; i < 40; i++) {
    /* Sum: '<S14>/Sum' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Sum_1[i] =
      c_gmres_Discrete_TimeIntegrator_1[i] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Product_1[i];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_SUM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_SUM_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ZERO1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ZERO1_PARAM);
  /* Constant: '<S12>/zero1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_zero1_1[0] =
    c_gmres_P.zero1_Value_j[0];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_zero1_1[1] =
    c_gmres_P.zero1_Value_j[1];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ZERO1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ZERO1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_PRODUCT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_PRODUCT1_PARAM);
  /* Product: '<S14>/Product1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Product1_1[0] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_zero1_1[0] *
    c_gmres_Constant1_1;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Product1_1[1] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_zero1_1[1] *
    c_gmres_Constant1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_PRODUCT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_PRODUCT1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_SUM1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_SUM1_PARAM);
  /* Sum: '<S14>/Sum1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Sum1_1[0] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subsystem_Add_1[0]
    + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Product1_1
    [0];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Sum1_1[1] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subsystem_Add_1[1]
    + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Product1_1
    [1];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_SUM1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_SUM1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S18>/ConcatBufferAtVectorConcatenateIn1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_VectorConcatenate_1
    [0] = c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Sum1_1
    [0];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_VectorConcatenate_1
    [1] = c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Sum1_1
    [1];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_PREDICT_X_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_PREDICT_X_FORITERATORSUBSYSTEM_PARAM);
  /* Outputs for Iterator SubSystem: '<S37>/ForIteratorSubsystem' incorporates:
   *  ForIterator: '<S46>/ForIterator'
   */
  /* InitializeConditions for UnitDelay: '<S47>/UnitDelay' */
  c_gmres_DWork.UnitDelay_DSTATE_hu[0] = c_gmres_P.UnitDelay_InitialCondition_ab;
  c_gmres_DWork.UnitDelay_DSTATE_hu[1] = c_gmres_P.UnitDelay_InitialCondition_ab;
  for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
       c_gmres_P.ForIterator_IterationLimit_a; c_gmres_fdgmres_Initialize_mode++)
  {
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_ForIterator_1
      = c_gmres_fdgmres_Initialize_mode;

    /* Constant: '<S46>/Constant' */
    memcpy
      (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Constant_1
       [0], &c_gmres_P.Constant_Value_b[0], 40U * sizeof(real_T));

    /* Selector: '<S46>/Selector' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Selector_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Sum_1[(i_0
      - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Selector_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Sum_1[((i_0
      - 1) << 1) + 1];

    /* Gain: '<S48>/Gain3' */
    Bias = 1.0;
    x_idx_1 = 1.0;
    y_idx_0 = c_gmres_P.tank_area[0];
    y_idx_1 = c_gmres_P.tank_area[1];
    x = Bias;
    x /= y_idx_0;
    Bias = x;
    x = x_idx_1;
    x /= y_idx_1;
    x_idx_1 = x;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [0] = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Selector_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [1] = x_idx_1 *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Selector_1
      [1];

    /* UnitDelay: '<S47>/UnitDelay' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
      [0] = c_gmres_DWork.UnitDelay_DSTATE_hu[0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
      [1] = c_gmres_DWork.UnitDelay_DSTATE_hu[1];

    /* Switch: '<S47>/Switch' */
    if (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_ForIterator_1
        > c_gmres_P.Switch_Threshold_e) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [0] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
        [0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [1] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
        [1];
    } else {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [0] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Sum1_1[0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [1] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Sum1_1[1];
    }

    /* End of Switch: '<S47>/Switch' */

    /* Reshape: '<S46>/Reshape1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [1];

    /* Saturate: '<S48>/Saturation' */
    Bias =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
      [0];
    x_idx_1 = c_gmres_P.Saturation_LowerSat_b;
    y_idx_0 = c_gmres_P.Saturation_UpperSat_j;
    if (Bias > y_idx_0) {
      Bias = y_idx_0;
    } else {
      if (Bias < x_idx_1) {
        Bias = x_idx_1;
      }
    }

    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
      [0] = Bias;
    Bias =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
      [1];
    x_idx_1 = c_gmres_P.Saturation_LowerSat_b;
    y_idx_0 = c_gmres_P.Saturation_UpperSat_j;
    if (Bias > y_idx_0) {
      Bias = y_idx_0;
    } else {
      if (Bias < x_idx_1) {
        Bias = x_idx_1;
      }
    }

    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
      [1] = Bias;

    /* End of Saturate: '<S48>/Saturation' */

    /* Sqrt: '<S48>/Sqrt' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [0] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
       [0]);
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [1] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
       [1]);

    /* Gain: '<S48>/Gain' */
    Bias = -c_gmres_P.C_plant[0];
    x_idx_1 = c_gmres_P.tank_area[0];
    Bias /= x_idx_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain_1
      = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [0];

    /* Gain: '<S48>/Gain1' */
    Bias = c_gmres_P.C_plant[0];
    x_idx_1 = c_gmres_P.tank_area[1];
    Bias /= x_idx_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
      = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [0];

    /* Gain: '<S48>/Gain2' */
    Bias = -c_gmres_P.C_plant[1];
    x_idx_1 = c_gmres_P.tank_area[1];
    Bias /= x_idx_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain2_1
      = Bias *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
      [1];

    /* Sum: '<S48>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain2_1;

    /* Sum: '<S48>/Add1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [0] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
      [1] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add_1;

    /* Gain: '<S46>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Gain_1
      = c_gmres_P.Gain_Gain_l * c_gmres_fdgmres_calculation_tau_Gain2_1;

    /* Product: '<S47>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Gain_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Gain_1;

    /* Sum: '<S47>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [0] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
      [1] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
      [1];

    /* Reshape: '<S46>/Reshape' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Reshape_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Reshape_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [1];

    /* Assignment: '<S46>/Assignment' */
    if (c_gmres_fdgmres_Initialize_mode == 1) {
      memcpy
        (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Assignment_1
         [0],
         &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Constant_1
         [0], 40U * sizeof(real_T));
    }

    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Assignment_1
      [(i_0 - 1) << 1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Reshape_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Assignment_1
      [1 + ((i_0 - 1) << 1)] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Reshape_1
      [1];

    /* End of Assignment: '<S46>/Assignment' */

    /* Update for UnitDelay: '<S47>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_hu[0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [0];
    c_gmres_DWork.UnitDelay_DSTATE_hu[1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
      [1];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_PREDICT_X_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_PREDICT_X_FORITERATORSUBSYSTEM_PARAM);

  /* End of Outputs for SubSystem: '<S37>/ForIteratorSubsystem' */

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S18>/ConcatBufferAtVectorConcatenateIn2' */
  memcpy
    (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_VectorConcatenate_1
     [2],
     &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Assignment_1
     [0], 40U * sizeof(real_T));
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_SELECTOR,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_SELECTOR_PARAM);
  /* Selector: '<S18>/Selector' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Selector_1
    [0] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Assignment_1
    [38];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Selector_1
    [1] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Assignment_1
    [39];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_SELECTOR,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_SELECTOR_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_SELECTOR1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_SELECTOR1_PARAM);
  /* Selector: '<S18>/Selector1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Selector1_1
    [0] = c_gmres_Subsystem_Selector1_1[38];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Selector1_1
    [1] = c_gmres_Subsystem_Selector1_1[39];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_SELECTOR1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_SELECTOR1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_SUBTRACT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_SUBTRACT_PARAM);
  /* Sum: '<S35>/Subtract' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Subtract_1
    [0] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Selector_1
    [0] -
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Selector1_1
    [0];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Subtract_1
    [1] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Selector_1
    [1] -
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Selector1_1
    [1];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_SUBTRACT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_SUBTRACT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_CONSTANT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_CONSTANT_PARAM);
  /* Constant: '<S35>/Constant' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Constant_1
    = c_gmres_P.Constant_Value_ic;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_CONSTANT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_CONSTANT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_PRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_PRODUCT_PARAM);
  /* Product: '<S35>/Product' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Product_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Subtract_1
    [0] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Constant_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_PRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_PRODUCT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_CONSTANT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_CONSTANT1_PARAM);
  /* Constant: '<S35>/Constant1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Constant1_1
    = c_gmres_P.Constant1_Value_iq;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_CONSTANT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_CONSTANT1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_PRODUCT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_PRODUCT1_PARAM);
  /* Product: '<S35>/Product1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Product1_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Subtract_1
    [1] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Constant1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_PRODUCT1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_PRODUCT1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_ADD,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_ADD_PARAM);
  /* Sum: '<S35>/Add' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Add_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Product_1
    + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Product1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_ADD,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_ADD_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S35>/ConcatBufferAtVectorConcatenateIn1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_VectorConcatenate_1
    [0] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Add_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_VECTORCONCATENATE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_CONSTANT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_CONSTANT2_PARAM);
  /* Constant: '<S35>/Constant2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Constant2_1
    = c_gmres_P.Constant2_Value_b;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_CONSTANT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_CONSTANT2_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_PRODUCT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_PRODUCT2_PARAM);
  /* Product: '<S35>/Product2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Product2_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Subtract_1
    [1] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Constant2_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_PRODUCT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_PRODUCT2_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_CONSTANT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_CONSTANT3_PARAM);
  /* Constant: '<S35>/Constant3' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Constant3_1
    = c_gmres_P.Constant3_Value_p1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_CONSTANT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_CONSTANT3_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_PRODUCT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_PRODUCT3_PARAM);
  /* Product: '<S35>/Product3' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Product3_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Subtract_1
    [0] *
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Constant3_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_PRODUCT3,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_PRODUCT3_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_ADD1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_ADD1_PARAM);
  /* Sum: '<S35>/Add1' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Add1_1
    =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Product2_1
    + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Product3_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_ADD1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_ADD1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);
  /* SignalConversion: '<S35>/ConcatBufferAtVectorConcatenateIn2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_VectorConcatenate_1
    [1] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Add1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_VECTORCONCATENATE,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATELAMBDA_N_VECTORCONCATENATE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM_PARAM);
  /* Outputs for Iterator SubSystem: '<S34>/ForIteratorSubsystem' incorporates:
   *  ForIterator: '<S38>/ForIterator'
   */
  /* InitializeConditions for UnitDelay: '<S40>/UnitDelay' */
  c_gmres_DWork.UnitDelay_DSTATE_al[0] = c_gmres_P.UnitDelay_InitialCondition_d;
  c_gmres_DWork.UnitDelay_DSTATE_al[1] = c_gmres_P.UnitDelay_InitialCondition_d;
  for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
       c_gmres_P.ForIterator_IterationLimit_c; c_gmres_fdgmres_Initialize_mode++)
  {
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
      = c_gmres_fdgmres_Initialize_mode;

    /* Constant: '<S38>/Constant' */
    memcpy
      (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
       [0], &c_gmres_P.Constant_Value_h[0], 38U * sizeof(real_T));

    /* Constant: '<S41>/Q11' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
      = c_gmres_P.Q11_Value_i;

    /* Constant: '<S38>/Constant1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
      = c_gmres_P.Constant1_Value_d;

    /* Sum: '<S38>/Subtract' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
      - (real_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;

    /* Selector: '<S38>/Selector' */
    i_0 = (int32_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_VectorConcatenate_1
      [(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_VectorConcatenate_1
      [((i_0 - 1) << 1) + 1];

    /* Selector: '<S38>/Selector1' */
    i_0 = (int32_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [0] = c_gmres_Subsystem_Selector1_1[(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [1] = c_gmres_Subsystem_Selector1_1[((i_0 - 1) << 1) + 1];

    /* Sum: '<S39>/Subtract' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [0] -
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
      [1] -
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
      [1];

    /* Product: '<S41>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [0];

    /* Constant: '<S41>/Q12_Q21' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
      = c_gmres_P.Q12_Q21_Value_e;

    /* Product: '<S41>/Product1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [1];

    /* Sum: '<S41>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1;

    /* UnitDelay: '<S40>/UnitDelay' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
      [0] = c_gmres_DWork.UnitDelay_DSTATE_al[0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
      [1] = c_gmres_DWork.UnitDelay_DSTATE_al[1];

    /* Switch: '<S40>/Switch' */
    if (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
        > c_gmres_P.Switch_Threshold_o) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [0] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
        [0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [1] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
        [1];
    } else {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [0] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_VectorConcatenate_1
        [0];
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [1] =
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_VectorConcatenate_1
        [1];
    }

    /* End of Switch: '<S40>/Switch' */

    /* Constant: '<S41>/C1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
      = c_gmres_P.C_plant[0];

    /* Constant: '<S41>/A1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1
      = c_gmres_P.tank_area[0];

    /* Product: '<S41>/Divide' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1;

    /* Product: '<S41>/Product2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1;

    /* Constant: '<S41>/A2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1
      = c_gmres_P.tank_area[1];

    /* Product: '<S41>/Divide1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1;

    /* Product: '<S41>/Product3' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1;

    /* Sum: '<S41>/Add1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
      - c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1;

    /* Sqrt: '<S39>/Sqrt' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [0] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
       [0]);
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [1] = sqrt
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
       [1]);

    /* Product: '<S41>/Divide2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [0];

    /* Gain: '<S41>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1
      = c_gmres_P.Gain_Gain_i *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1;

    /* Sum: '<S41>/Add2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1;

    /* SignalConversion: '<S39>/ConcatBufferAtVectorConcatenateIn1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1;

    /* Constant: '<S42>/Q22' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
      = c_gmres_P.Q22_Value_a;

    /* Product: '<S42>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [1];

    /* Constant: '<S42>/Q12_Q21' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
      = c_gmres_P.Q12_Q21_Value_a;

    /* Product: '<S42>/Product1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
      [0];

    /* Constant: '<S42>/C2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
      = c_gmres_P.C_plant[1];

    /* Constant: '<S42>/A2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
      = c_gmres_P.tank_area[1];

    /* Product: '<S42>/Product3' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
      * c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [1] /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
      /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
      [1];

    /* Gain: '<S42>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1
      = c_gmres_P.Gain_Gain_ni *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1;

    /* Sum: '<S42>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1
      =
      (c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
       + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1)
      - c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1;

    /* SignalConversion: '<S39>/ConcatBufferAtVectorConcatenateIn2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1;

    /* Gain: '<S38>/Gain' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Gain_1
      = c_gmres_P.Gain_Gain_o * c_gmres_fdgmres_calculation_tau_Gain2_1;

    /* Product: '<S40>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;

    /* Sum: '<S40>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [0] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
      [1] +
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
      [1];

    /* Reshape: '<S38>/Reshape' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [1];

    /* Constant: '<S38>/Constant2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
      = c_gmres_P.Constant2_Value_f;

    /* Sum: '<S38>/Subtract1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
      - (real_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;

    /* Assignment: '<S38>/Assignment' */
    if (c_gmres_fdgmres_Initialize_mode == 1) {
      memcpy
        (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
         [0],
         &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
         [0], 38U * sizeof(real_T));
    }

    i_0 = (int32_T)
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
      [(i_0 - 1) << 1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
      [1 + ((i_0 - 1) << 1)] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
      [1];

    /* End of Assignment: '<S38>/Assignment' */

    /* Update for UnitDelay: '<S40>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_al[0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [0];
    c_gmres_DWork.UnitDelay_DSTATE_al[1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
      [1];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_CALCULATE_LAMBDA_I_FORITERATORSUBSYSTEM_PARAM);

  /* End of Outputs for SubSystem: '<S34>/ForIteratorSubsystem' */

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_VECTORCONCATENATE1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_VECTORCONCATENATE1_PARAM);
  /* SignalConversion: '<S18>/ConcatBufferAtVectorConcatenate1In1' */
  memcpy
    (&c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_VectorConcatenate1_1
     [0],
     &c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
     [0], 38U * sizeof(real_T));
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_VECTORCONCATENATE1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_VECTORCONCATENATE1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_VECTORCONCATENATE1,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_VECTORCONCATENATE1_PARAM);
  /* SignalConversion: '<S18>/ConcatBufferAtVectorConcatenate1In2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_VectorConcatenate1_1
    [38] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_VectorConcatenate_1
    [0];
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_VectorConcatenate1_1
    [39] =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_VectorConcatenate_1
    [1];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_VECTORCONCATENATE1,0,1,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_VECTORCONCATENATE1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_F_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_F_FORITERATORSUBSYSTEM_PARAM);
  /* Outputs for Iterator SubSystem: '<S36>/ForIteratorSubsystem' incorporates:
   *  ForIterator: '<S43>/ForIterator'
   */
  for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
       c_gmres_P.ForIterator_IterationLimit_oa; c_gmres_fdgmres_Initialize_mode
       ++) {
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_ForIterator_1
      = c_gmres_fdgmres_Initialize_mode;

    /* Selector: '<S43>/Selector' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Selector_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Sum_1[(i_0
      - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Selector_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Sum_1[((i_0
      - 1) << 1) + 1];

    /* Constant: '<S44>/r1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_r1_1
      = 2.0 * c_gmres_P.p_idx_r[0];

    /* Product: '<S44>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Selector_1
      [0] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_r1_1;

    /* Selector: '<S43>/Selector1' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Selector1_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_VectorConcatenate1_1
      [(i_0 - 1) << 1];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Selector1_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_VectorConcatenate1_1
      [((i_0 - 1) << 1) + 1];

    /* Constant: '<S44>/A1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_A1_1
      = c_gmres_P.tank_area[0];

    /* Product: '<S44>/Divide' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_Divide_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Selector1_1
      [0] /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_A1_1;

    /* Sum: '<S44>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_Product_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_Divide_1;

    /* SignalConversion: '<S43>/ConcatBufferAtVectorConcatenateIn1' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
      [0] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_Add_1;

    /* Constant: '<S45>/r2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_r2_1
      = 2.0 * c_gmres_P.p_idx_r[1];

    /* Product: '<S45>/Product' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_Product_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Selector_1
      [1] *
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_r2_1;

    /* Constant: '<S45>/A2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_A2_1
      = c_gmres_P.tank_area[1];

    /* Product: '<S45>/Divide' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_Divide_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Selector1_1
      [1] /
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_A2_1;

    /* Sum: '<S45>/Add' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_Add_1
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_Product_1
      + c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_Divide_1;

    /* SignalConversion: '<S43>/ConcatBufferAtVectorConcatenateIn2' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
      [1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_Add_1;
    for (i = 0; i < 40; i++) {
      /* Constant: '<S43>/Constant' */
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Constant_1
        [i] = c_gmres_P.Constant_Value_d[i];

      /* Assignment: '<S43>/Assignment' */
      if (c_gmres_fdgmres_Initialize_mode == 1) {
        c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Assignment_1
          [i] =
          c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Constant_1
          [i];
      }
    }

    /* Assignment: '<S43>/Assignment' */
    i_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_ForIterator_1;
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Assignment_1
      [(i_0 - 1) << 1] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
      [0];
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Assignment_1
      [1 + ((i_0 - 1) << 1)] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
      [1];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_F_FORITERATORSUBSYSTEM,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_MODEL1_F_FORITERATORSUBSYSTEM_PARAM);

  /* End of Outputs for SubSystem: '<S36>/ForIteratorSubsystem' */

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_TWO_NORM_DOTPRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_TWO_NORM_DOTPRODUCT_PARAM);
  /* DotProduct: '<S13>/DotProduct' */
  Bias = 0.0;
  for (i = 0; i < 40; i++) {
    /* Sum: '<S14>/Subtract' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Subtract_1[i]
      =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Assignment_1
      [i] -
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Assignment_1
      [i];

    /* Product: '<S14>/Divide' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Divide_1[i] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Subtract_1[i]
      / c_gmres_Constant1_1;

    /* Sum: '<S12>/Subtract' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subtract_1[i] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Subtract_1[i] -
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Divide_1[i];

    /* Reshape: '<S12>/Reshape' */
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Reshape_1[i] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subtract_1[i];
    x_idx_1 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Reshape_1[i];
    y_idx_0 =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Reshape_1[i];
    Bias += x_idx_1 * y_idx_0;
  }

  c_gmres_fdgmres_Initialize_model_two_norm_DotProduct_1 = Bias;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_TWO_NORM_DOTPRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_TWO_NORM_DOTPRODUCT_PARAM);

  /* End of DotProduct: '<S13>/DotProduct' */

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_TWO_NORM_SQRT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_TWO_NORM_SQRT_PARAM);
  /* Sqrt: '<S13>/Sqrt' */
  c_gmres_fdgmres_Initialize_model_two_norm_Sqrt_1 = sqrt
    (c_gmres_fdgmres_Initialize_model_two_norm_DotProduct_1);
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_TWO_NORM_SQRT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_TWO_NORM_SQRT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_DIVIDE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_DIVIDE_PARAM);
  /* Product: '<S8>/Divide' */
  for (i = 0; i < 40; i++) {
    c_gmres_fdgmres_Initialize_model_Divide_1[i] =
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Reshape_1[i] /
      c_gmres_fdgmres_Initialize_model_two_norm_Sqrt_1;
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_DIVIDE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_DIVIDE_PARAM);

  /* End of Product: '<S8>/Divide' */

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_ASSIGNMENT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_ASSIGNMENT_PARAM);
  /* Assignment: '<S8>/Assignment' */
  memcpy(&c_gmres_fdgmres_Initialize_model_Assignment_1[0],
         &c_gmres_fdgmres_Initialize_model_q_matrix_1[0], 160U * sizeof(real_T));
  memcpy(&c_gmres_fdgmres_Initialize_model_Assignment_1[0],
         &c_gmres_fdgmres_Initialize_model_Divide_1[0], 40U * sizeof(real_T));
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_ASSIGNMENT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_ASSIGNMENT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_B_HAT_INIT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_B_HAT_INIT_PARAM);
  /* Constant: '<S8>/b_hat_init' */
  c_gmres_fdgmres_Initialize_model_b_hat_init_1[0] = c_gmres_P.b_hat_init_Value
    [0];
  c_gmres_fdgmres_Initialize_model_b_hat_init_1[1] = c_gmres_P.b_hat_init_Value
    [1];
  c_gmres_fdgmres_Initialize_model_b_hat_init_1[2] = c_gmres_P.b_hat_init_Value
    [2];
  c_gmres_fdgmres_Initialize_model_b_hat_init_1[3] = c_gmres_P.b_hat_init_Value
    [3];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_B_HAT_INIT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_B_HAT_INIT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_PRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_PRODUCT_PARAM);
  /* Product: '<S8>/Product' */
  c_gmres_fdgmres_Initialize_model_Product_1[0] =
    c_gmres_fdgmres_Initialize_model_two_norm_Sqrt_1 *
    c_gmres_fdgmres_Initialize_model_b_hat_init_1[0];
  c_gmres_fdgmres_Initialize_model_Product_1[1] =
    c_gmres_fdgmres_Initialize_model_two_norm_Sqrt_1 *
    c_gmres_fdgmres_Initialize_model_b_hat_init_1[1];
  c_gmres_fdgmres_Initialize_model_Product_1[2] =
    c_gmres_fdgmres_Initialize_model_two_norm_Sqrt_1 *
    c_gmres_fdgmres_Initialize_model_b_hat_init_1[2];
  c_gmres_fdgmres_Initialize_model_Product_1[3] =
    c_gmres_fdgmres_Initialize_model_two_norm_Sqrt_1 *
    c_gmres_fdgmres_Initialize_model_b_hat_init_1[3];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_PRODUCT,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_PRODUCT_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_RESHAPE1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_RESHAPE1_PARAM);
  /* Reshape: '<S8>/Reshape1' */
  c_gmres_fdgmres_Initialize_model_Reshape1_1[0] =
    c_gmres_fdgmres_Initialize_model_Product_1[0];
  c_gmres_fdgmres_Initialize_model_Reshape1_1[1] =
    c_gmres_fdgmres_Initialize_model_Product_1[1];
  c_gmres_fdgmres_Initialize_model_Reshape1_1[2] =
    c_gmres_fdgmres_Initialize_model_Product_1[2];
  c_gmres_fdgmres_Initialize_model_Reshape1_1[3] =
    c_gmres_fdgmres_Initialize_model_Product_1[3];
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_RESHAPE1,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_RESHAPE1_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_ITERATIVVELYMINIMIZERESIDUALVECTOR,0,0,C_GMRES_FDGMRES_ITERATIVVELYMINIMIZERESIDUALVECTOR_PARAM);
  /* Outputs for Iterator SubSystem: '<S4>/Iterativvelyminimizeresidualvector' incorporates:
   *  ForIterator: '<S9>/ForIterator'
   */
  /* InitializeConditions for UnitDelay: '<S98>/h_matrix' */
  memcpy(&c_gmres_DWork.h_matrix_DSTATE[0],
         &c_gmres_P.h_matrix_InitialCondition[0], 12U * sizeof(real_T));

  /* InitializeConditions for UnitDelay: '<S9>/q_matrix' */
  memcpy(&c_gmres_DWork.q_matrix_DSTATE[0],
         &c_gmres_P.q_matrix_InitialCondition[0], 160U * sizeof(real_T));

  /* InitializeConditions for UnitDelay: '<S9>/s_vector' */
  c_gmres_DWork.s_vector_DSTATE[0] = c_gmres_P.s_vector_InitialCondition[0];
  c_gmres_DWork.s_vector_DSTATE[1] = c_gmres_P.s_vector_InitialCondition[1];
  c_gmres_DWork.s_vector_DSTATE[2] = c_gmres_P.s_vector_InitialCondition[2];

  /* InitializeConditions for UnitDelay: '<S9>/c_vector' */
  c_gmres_DWork.c_vector_DSTATE[0] = c_gmres_P.c_vector_InitialCondition[0];
  c_gmres_DWork.c_vector_DSTATE[1] = c_gmres_P.c_vector_InitialCondition[1];
  c_gmres_DWork.c_vector_DSTATE[2] = c_gmres_P.c_vector_InitialCondition[2];

  /* InitializeConditions for UnitDelay: '<S100>/b_hat' */
  c_gmres_DWork.b_hat_DSTATE[0] = c_gmres_P.b_hat_InitialCondition_l[0];
  c_gmres_DWork.b_hat_DSTATE[1] = c_gmres_P.b_hat_InitialCondition_l[1];
  c_gmres_DWork.b_hat_DSTATE[2] = c_gmres_P.b_hat_InitialCondition_l[2];
  c_gmres_DWork.b_hat_DSTATE[3] = c_gmres_P.b_hat_InitialCondition_l[3];

  /* InitializeConditions for UnitDelay: '<S101>/r_matrix' */
  memcpy(&c_gmres_DWork.r_matrix_DSTATE[0],
         &c_gmres_P.r_matrix_InitialCondition[0], 9U * sizeof(real_T));
  for (c_gmres_fdgmres_Iterativvelymin = 1; c_gmres_fdgmres_Iterativvelymin <=
       c_gmres_P.ForIterator_IterationLimit_pw; c_gmres_fdgmres_Iterativvelymin
       ++) {
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1 =
      c_gmres_fdgmres_Iterativvelymin;

    /* Constant: '<S9>/one' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_one_1 =
      c_gmres_P.one_Value_m;

    /* Sum: '<S9>/Add' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Add_1 = (real_T)
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1 +
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_one_1;

    /* UnitDelay: '<S98>/h_matrix' */
    memcpy
      (&c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_h_matrix_1
       [0], &c_gmres_DWork.h_matrix_DSTATE[0], 12U * sizeof(real_T));

    /* Product: '<S108>/Product' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Subsystem_Product_1
      [0] = c_gmres_Gain_1[0] * c_gmres_Constant1_1;
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Subsystem_Product_1
      [1] = c_gmres_Gain_1[1] * c_gmres_Constant1_1;

    /* Sum: '<S108>/Add' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Subsystem_Add_1
      [0] = c_gmres_UnitDelay1_1[0] +
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Subsystem_Product_1
      [0];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Subsystem_Add_1
      [1] = c_gmres_UnitDelay1_1[1] +
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Subsystem_Product_1
      [1];

    /* SignalConversion: '<S109>/ConcatBufferAtVectorConcatenateIn1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_VectorConcatenate_1
      [0] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Subsystem_Add_1
      [0];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_VectorConcatenate_1
      [1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Subsystem_Add_1
      [1];

    /* Outputs for Iterator SubSystem: '<S114>/ForIteratorSubsystem' incorporates:
     *  ForIterator: '<S123>/ForIterator'
     */
    if (c_gmres_DWork.ForIterator_IterationMarker_f != 0) {
      /* InitializeConditions for UnitDelay: '<S124>/UnitDelay' */
      c_gmres_DWork.UnitDelay_DSTATE_n[0] =
        c_gmres_P.UnitDelay_InitialCondition_i;
      c_gmres_DWork.UnitDelay_DSTATE_n[1] =
        c_gmres_P.UnitDelay_InitialCondition_i;
    }

    c_gmres_DWork.ForIterator_IterationMarker_f = 1U;
    for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
         c_gmres_P.ForIterator_IterationLimit_l; c_gmres_fdgmres_Initialize_mode
         ++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_ForIterator_1
        = c_gmres_fdgmres_Initialize_mode;

      /* Constant: '<S123>/Constant' */
      memcpy
        (&c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Constant_1
         [0], &c_gmres_P.Constant_Value_i[0], 40U * sizeof(real_T));

      /* Selector: '<S123>/Selector' */
      i_0 =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_ForIterator_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Selector_1
        [0] = c_gmres_Discrete_TimeIntegrator_1[(i_0 - 1) << 1];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Selector_1
        [1] = c_gmres_Discrete_TimeIntegrator_1[((i_0 - 1) << 1) + 1];

      /* Gain: '<S125>/Gain3' */
      Bias = 1.0;
      x_idx_1 = 1.0;
      y_idx_0 = c_gmres_P.tank_area[0];
      y_idx_1 = c_gmres_P.tank_area[1];
      x = Bias;
      x /= y_idx_0;
      Bias = x;
      x = x_idx_1;
      x /= y_idx_1;
      x_idx_1 = x;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
        [0] = Bias *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Selector_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
        [1] = x_idx_1 *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Selector_1
        [1];

      /* UnitDelay: '<S124>/UnitDelay' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
        [0] = c_gmres_DWork.UnitDelay_DSTATE_n[0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
        [1] = c_gmres_DWork.UnitDelay_DSTATE_n[1];

      /* Switch: '<S124>/Switch' */
      if (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_ForIterator_1
          > c_gmres_P.Switch_Threshold_j) {
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
          [0] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
          [0];
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
          [1] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
          [1];
      } else {
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
          [0] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Subsystem_Add_1
          [0];
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
          [1] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Subsystem_Add_1
          [1];
      }

      /* End of Switch: '<S124>/Switch' */

      /* Reshape: '<S123>/Reshape1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Reshape1_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Reshape1_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [1];

      /* Saturate: '<S125>/Saturation' */
      Bias =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Reshape1_1
        [0];
      x_idx_1 = c_gmres_P.Saturation_LowerSat_kc;
      y_idx_0 = c_gmres_P.Saturation_UpperSat_bb;
      if (Bias > y_idx_0) {
        Bias = y_idx_0;
      } else {
        if (Bias < x_idx_1) {
          Bias = x_idx_1;
        }
      }

      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
        [0] = Bias;
      Bias =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Reshape1_1
        [1];
      x_idx_1 = c_gmres_P.Saturation_LowerSat_kc;
      y_idx_0 = c_gmres_P.Saturation_UpperSat_bb;
      if (Bias > y_idx_0) {
        Bias = y_idx_0;
      } else {
        if (Bias < x_idx_1) {
          Bias = x_idx_1;
        }
      }

      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
        [1] = Bias;

      /* End of Saturate: '<S125>/Saturation' */

      /* Sqrt: '<S125>/Sqrt' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
        [0] = sqrt
        (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
         [0]);
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
        [1] = sqrt
        (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
         [1]);

      /* Gain: '<S125>/Gain' */
      Bias = -c_gmres_P.C_plant[0];
      x_idx_1 = c_gmres_P.tank_area[0];
      Bias /= x_idx_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain_1
        = Bias *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
        [0];

      /* Gain: '<S125>/Gain1' */
      Bias = c_gmres_P.C_plant[0];
      x_idx_1 = c_gmres_P.tank_area[1];
      Bias /= x_idx_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
        = Bias *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
        [0];

      /* Gain: '<S125>/Gain2' */
      Bias = -c_gmres_P.C_plant[1];
      x_idx_1 = c_gmres_P.tank_area[1];
      Bias /= x_idx_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain2_1
        = Bias *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
        [1];

      /* Sum: '<S125>/Add' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
        + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain2_1;

      /* Sum: '<S125>/Add1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
        [0] +
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
        [1] +
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add_1;

      /* Gain: '<S123>/Gain' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Gain_1
        = c_gmres_P.Gain_Gain_c1 * c_gmres_fdgmres_calculation_tau_Gain2_1;

      /* Product: '<S124>/Product' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
        [0] *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Gain_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
        [1] *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Gain_1;

      /* Sum: '<S124>/Add' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
        [0] +
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
        [1] +
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [1];

      /* Reshape: '<S123>/Reshape' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Reshape_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Reshape_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
        [1];

      /* Assignment: '<S123>/Assignment' */
      if (c_gmres_fdgmres_Initialize_mode == 1) {
        memcpy
          (&c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Assignment_1
           [0],
           &c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Constant_1
           [0], 40U * sizeof(real_T));
      }

      i_0 =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_ForIterator_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Assignment_1
        [(i_0 - 1) << 1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Reshape_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Assignment_1
        [1 + ((i_0 - 1) << 1)] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Reshape_1
        [1];

      /* End of Assignment: '<S123>/Assignment' */

      /* Update for UnitDelay: '<S124>/UnitDelay' */
      c_gmres_DWork.UnitDelay_DSTATE_n[0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
        [0];
      c_gmres_DWork.UnitDelay_DSTATE_n[1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
        [1];
    }

    /* End of Outputs for SubSystem: '<S114>/ForIteratorSubsystem' */

    /* SignalConversion: '<S109>/ConcatBufferAtVectorConcatenateIn2' */
    memcpy
      (&c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_VectorConcatenate_1
       [2],
       &c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Assignment_1
       [0], 40U * sizeof(real_T));

    /* Selector: '<S109>/Selector' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Selector_1
      [0] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Assignment_1
      [38];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Selector_1
      [1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Assignment_1
      [39];

    /* Selector: '<S109>/Selector1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Selector1_1
      [0] = c_gmres_Subsystem_Selector1_1[38];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Selector1_1
      [1] = c_gmres_Subsystem_Selector1_1[39];

    /* Sum: '<S112>/Subtract' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Subtract_1
      [0] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Selector_1
      [0] -
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Selector1_1
      [0];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Subtract_1
      [1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Selector_1
      [1] -
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Selector1_1
      [1];

    /* Constant: '<S112>/Constant' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Constant_1
      = c_gmres_P.Constant_Value_o;

    /* Product: '<S112>/Product' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Product_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Subtract_1
      [0] *
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Constant_1;

    /* Constant: '<S112>/Constant1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Constant1_1
      = c_gmres_P.Constant1_Value_j2;

    /* Product: '<S112>/Product1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Product1_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Subtract_1
      [1] *
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Constant1_1;

    /* Sum: '<S112>/Add' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Add_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Product_1
      + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Product1_1;

    /* SignalConversion: '<S112>/ConcatBufferAtVectorConcatenateIn1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_VectorConcatenate_1
      [0] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Add_1;

    /* Constant: '<S112>/Constant2' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Constant2_1
      = c_gmres_P.Constant2_Value_c1;

    /* Product: '<S112>/Product2' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Product2_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Subtract_1
      [1] *
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Constant2_1;

    /* Constant: '<S112>/Constant3' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Constant3_1
      = c_gmres_P.Constant3_Value;

    /* Product: '<S112>/Product3' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Product3_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Subtract_1
      [0] *
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Constant3_1;

    /* Sum: '<S112>/Add1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Add1_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Product2_1
      + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Product3_1;

    /* SignalConversion: '<S112>/ConcatBufferAtVectorConcatenateIn2' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_VectorConcatenate_1
      [1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Add1_1;

    /* Outputs for Iterator SubSystem: '<S111>/ForIteratorSubsystem' incorporates:
     *  ForIterator: '<S115>/ForIterator'
     */
    if (c_gmres_DWork.ForIterator_IterationMarker_g != 0) {
      /* InitializeConditions for UnitDelay: '<S117>/UnitDelay' */
      c_gmres_DWork.UnitDelay_DSTATE_l[0] =
        c_gmres_P.UnitDelay_InitialCondition_h;
      c_gmres_DWork.UnitDelay_DSTATE_l[1] =
        c_gmres_P.UnitDelay_InitialCondition_h;
    }

    c_gmres_DWork.ForIterator_IterationMarker_g = 1U;
    for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
         c_gmres_P.ForIterator_IterationLimit_k; c_gmres_fdgmres_Initialize_mode
         ++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
        = c_gmres_fdgmres_Initialize_mode;

      /* Constant: '<S115>/Constant' */
      memcpy
        (&c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
         [0], &c_gmres_P.Constant_Value_c2[0], 38U * sizeof(real_T));

      /* Constant: '<S118>/Q11' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
        = c_gmres_P.Q11_Value_g;

      /* Constant: '<S115>/Constant1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
        = c_gmres_P.Constant1_Value_j;

      /* Sum: '<S115>/Subtract' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
        - (real_T)
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;

      /* Selector: '<S115>/Selector' */
      i_0 = (int32_T)
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_VectorConcatenate_1
        [(i_0 - 1) << 1];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_VectorConcatenate_1
        [((i_0 - 1) << 1) + 1];

      /* Selector: '<S115>/Selector1' */
      i_0 = (int32_T)
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
        [0] = c_gmres_Subsystem_Selector1_1[(i_0 - 1) << 1];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
        [1] = c_gmres_Subsystem_Selector1_1[((i_0 - 1) << 1) + 1];

      /* Sum: '<S116>/Subtract' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
        [0] -
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
        [1] -
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
        [1];

      /* Product: '<S118>/Product' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
        * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
        [0];

      /* Constant: '<S118>/Q12_Q21' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
        = c_gmres_P.Q12_Q21_Value_d;

      /* Product: '<S118>/Product1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
        * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
        [1];

      /* Sum: '<S118>/Add' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
        + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1;

      /* UnitDelay: '<S117>/UnitDelay' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
        [0] = c_gmres_DWork.UnitDelay_DSTATE_l[0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
        [1] = c_gmres_DWork.UnitDelay_DSTATE_l[1];

      /* Switch: '<S117>/Switch' */
      if (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
          > c_gmres_P.Switch_Threshold_h) {
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
          [0] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
          [0];
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
          [1] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
          [1];
      } else {
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
          [0] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_VectorConcatenate_1
          [0];
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
          [1] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_VectorConcatenate_1
          [1];
      }

      /* End of Switch: '<S117>/Switch' */

      /* Constant: '<S118>/C1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
        = c_gmres_P.C_plant[0];

      /* Constant: '<S118>/A1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1
        = c_gmres_P.tank_area[0];

      /* Product: '<S118>/Divide' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
        /
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1;

      /* Product: '<S118>/Product2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [0] *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1;

      /* Constant: '<S118>/A2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1
        = c_gmres_P.tank_area[1];

      /* Product: '<S118>/Divide1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
        /
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1;

      /* Product: '<S118>/Product3' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [1] *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1;

      /* Sum: '<S118>/Add1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
        - c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1;

      /* Sqrt: '<S116>/Sqrt' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
        [0] = sqrt
        (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
         [0]);
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
        [1] = sqrt
        (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
         [1]);

      /* Product: '<S118>/Divide2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
        /
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
        [0];

      /* Gain: '<S118>/Gain' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1
        = c_gmres_P.Gain_Gain_ov *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1;

      /* Sum: '<S118>/Add2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
        + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1;

      /* SignalConversion: '<S116>/ConcatBufferAtVectorConcatenateIn1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1;

      /* Constant: '<S119>/Q22' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
        = c_gmres_P.Q22_Value_k;

      /* Product: '<S119>/Product' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
        * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
        [1];

      /* Constant: '<S119>/Q12_Q21' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
        = c_gmres_P.Q12_Q21_Value_mj;

      /* Product: '<S119>/Product1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
        * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
        [0];

      /* Constant: '<S119>/C2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
        = c_gmres_P.C_plant[1];

      /* Constant: '<S119>/A2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
        = c_gmres_P.tank_area[1];

      /* Product: '<S119>/Product3' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
        * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [1] /
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
        /
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
        [1];

      /* Gain: '<S119>/Gain' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1
        = c_gmres_P.Gain_Gain_jq *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1;

      /* Sum: '<S119>/Add' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1
        =
        (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
         + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1)
        - c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1;

      /* SignalConversion: '<S116>/ConcatBufferAtVectorConcatenateIn2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1;

      /* Gain: '<S115>/Gain' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1
        = c_gmres_P.Gain_Gain_of * c_gmres_fdgmres_calculation_tau_Gain2_1;

      /* Product: '<S117>/Product' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
        [0] *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
        [1] *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;

      /* Sum: '<S117>/Add' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
        [0] +
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
        [1] +
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [1];

      /* Reshape: '<S115>/Reshape' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
        [1];

      /* Constant: '<S115>/Constant2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
        = c_gmres_P.Constant2_Value_o;

      /* Sum: '<S115>/Subtract1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
        - (real_T)
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;

      /* Assignment: '<S115>/Assignment' */
      if (c_gmres_fdgmres_Initialize_mode == 1) {
        memcpy
          (&c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
           [0],
           &c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
           [0], 38U * sizeof(real_T));
      }

      i_0 = (int32_T)
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
        [(i_0 - 1) << 1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
        [1 + ((i_0 - 1) << 1)] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
        [1];

      /* End of Assignment: '<S115>/Assignment' */

      /* Update for UnitDelay: '<S117>/UnitDelay' */
      c_gmres_DWork.UnitDelay_DSTATE_l[0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
        [0];
      c_gmres_DWork.UnitDelay_DSTATE_l[1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
        [1];
    }

    /* End of Outputs for SubSystem: '<S111>/ForIteratorSubsystem' */

    /* SignalConversion: '<S109>/ConcatBufferAtVectorConcatenate1In1' */
    memcpy
      (&c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_VectorConcatenate1_1
       [0],
       &c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
       [0], 38U * sizeof(real_T));

    /* SignalConversion: '<S109>/ConcatBufferAtVectorConcatenate1In2' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_VectorConcatenate1_1
      [38] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_VectorConcatenate_1
      [0];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_VectorConcatenate1_1
      [39] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_VectorConcatenate_1
      [1];

    /* Outputs for Iterator SubSystem: '<S113>/ForIteratorSubsystem' incorporates:
     *  ForIterator: '<S120>/ForIterator'
     */
    c_gmres_DWork.ForIterator_IterationMarker_fd = 1U;
    for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
         c_gmres_P.ForIterator_IterationLimit_b; c_gmres_fdgmres_Initialize_mode
         ++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_ForIterator_1
        = c_gmres_fdgmres_Initialize_mode;

      /* Selector: '<S120>/Selector' */
      i_0 =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_ForIterator_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Selector_1
        [0] = c_gmres_Discrete_TimeIntegrator_1[(i_0 - 1) << 1];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Selector_1
        [1] = c_gmres_Discrete_TimeIntegrator_1[((i_0 - 1) << 1) + 1];

      /* Constant: '<S121>/r1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_r1_1
        = 2.0 * c_gmres_P.p_idx_r[0];

      /* Product: '<S121>/Product' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_Product_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Selector_1
        [0] *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_r1_1;

      /* Selector: '<S120>/Selector1' */
      i_0 =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_ForIterator_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Selector1_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_VectorConcatenate1_1
        [(i_0 - 1) << 1];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Selector1_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_VectorConcatenate1_1
        [((i_0 - 1) << 1) + 1];

      /* Constant: '<S121>/A1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_A1_1
        = c_gmres_P.tank_area[0];

      /* Product: '<S121>/Divide' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_Divide_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Selector1_1
        [0] /
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_A1_1;

      /* Sum: '<S121>/Add' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_Add_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_Product_1
        + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_Divide_1;

      /* SignalConversion: '<S120>/ConcatBufferAtVectorConcatenateIn1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_VectorConcatenate_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_Add_1;

      /* Constant: '<S122>/r2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_r2_1
        = 2.0 * c_gmres_P.p_idx_r[1];

      /* Product: '<S122>/Product' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_Product_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Selector_1
        [1] *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_r2_1;

      /* Constant: '<S122>/A2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_A2_1
        = c_gmres_P.tank_area[1];

      /* Product: '<S122>/Divide' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_Divide_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Selector1_1
        [1] /
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_A2_1;

      /* Sum: '<S122>/Add' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_Add_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_Product_1
        + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_Divide_1;

      /* SignalConversion: '<S120>/ConcatBufferAtVectorConcatenateIn2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_VectorConcatenate_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_Add_1;
      for (i = 0; i < 40; i++) {
        /* Constant: '<S120>/Constant' */
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Constant_1
          [i] = c_gmres_P.Constant_Value_pl[i];

        /* Assignment: '<S120>/Assignment' */
        if (c_gmres_fdgmres_Initialize_mode == 1) {
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Assignment_1
            [i] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Constant_1
            [i];
        }
      }

      /* Assignment: '<S120>/Assignment' */
      i_0 =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_ForIterator_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Assignment_1
        [(i_0 - 1) << 1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_VectorConcatenate_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Assignment_1
        [1 + ((i_0 - 1) << 1)] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_VectorConcatenate_1
        [1];
    }

    /* End of Outputs for SubSystem: '<S113>/ForIteratorSubsystem' */
    for (i = 0; i < 160; i++) {
      /* UnitDelay: '<S9>/q_matrix' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_q_matrix_1[i] =
        c_gmres_DWork.q_matrix_DSTATE[i];

      /* Switch: '<S9>/Switch' */
      if (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1 >
          c_gmres_P.Switch_Threshold_hw) {
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Switch_1[i] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_q_matrix_1[i];
      } else {
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Switch_1[i] =
          c_gmres_fdgmres_Initialize_model_Assignment_1[i];
      }

      /* End of Switch: '<S9>/Switch' */
    }

    /* Selector: '<S98>/Selector' */
    i_0 = c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1;
    for (i = 0; i < 40; i++) {
      /* Selector: '<S98>/Selector' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Selector_1
        [i] = c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Switch_1[(i_0 -
        1) * 40 + i];

      /* Reshape: '<S104>/Reshape1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Reshape1_1
        [i] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Selector_1
        [i];

      /* Product: '<S107>/Product' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Product_1
        [i] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Reshape1_1
        [i] * c_gmres_Constant1_1;

      /* Sum: '<S107>/Sum' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Sum_1
        [i] = c_gmres_Discrete_TimeIntegrator_1[i] +
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Product_1
        [i];
    }

    /* Constant: '<S104>/zero1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_zero1_1
      [0] = c_gmres_P.zero1_Value[0];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_zero1_1
      [1] = c_gmres_P.zero1_Value[1];

    /* Product: '<S107>/Product1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Product1_1
      [0] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_zero1_1
      [0] * c_gmres_Constant1_1;
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Product1_1
      [1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_zero1_1
      [1] * c_gmres_Constant1_1;

    /* Sum: '<S107>/Sum1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Sum1_1
      [0] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Subsystem_Add_1
      [0] +
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Product1_1
      [0];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Sum1_1
      [1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Subsystem_Add_1
      [1] +
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Product1_1
      [1];

    /* SignalConversion: '<S110>/ConcatBufferAtVectorConcatenateIn1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_VectorConcatenate_1
      [0] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Sum1_1
      [0];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_VectorConcatenate_1
      [1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Sum1_1
      [1];

    /* Outputs for Iterator SubSystem: '<S129>/ForIteratorSubsystem' incorporates:
     *  ForIterator: '<S138>/ForIterator'
     */
    if (c_gmres_DWork.ForIterator_IterationMarker_b != 0) {
      /* InitializeConditions for UnitDelay: '<S139>/UnitDelay' */
      c_gmres_DWork.UnitDelay_DSTATE_d[0] =
        c_gmres_P.UnitDelay_InitialCondition_as;
      c_gmres_DWork.UnitDelay_DSTATE_d[1] =
        c_gmres_P.UnitDelay_InitialCondition_as;
    }

    c_gmres_DWork.ForIterator_IterationMarker_b = 1U;
    for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
         c_gmres_P.ForIterator_IterationLimit_kz;
         c_gmres_fdgmres_Initialize_mode++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_ForIterator_1
        = c_gmres_fdgmres_Initialize_mode;

      /* Constant: '<S138>/Constant' */
      memcpy
        (&c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Constant_1
         [0], &c_gmres_P.Constant_Value_l[0], 40U * sizeof(real_T));

      /* Selector: '<S138>/Selector' */
      i_0 =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_ForIterator_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Selector_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Sum_1
        [(i_0 - 1) << 1];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Selector_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Sum_1
        [((i_0 - 1) << 1) + 1];

      /* Gain: '<S140>/Gain3' */
      Bias = 1.0;
      x_idx_1 = 1.0;
      y_idx_0 = c_gmres_P.tank_area[0];
      y_idx_1 = c_gmres_P.tank_area[1];
      x = Bias;
      x /= y_idx_0;
      Bias = x;
      x = x_idx_1;
      x /= y_idx_1;
      x_idx_1 = x;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
        [0] = Bias *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Selector_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
        [1] = x_idx_1 *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Selector_1
        [1];

      /* UnitDelay: '<S139>/UnitDelay' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
        [0] = c_gmres_DWork.UnitDelay_DSTATE_d[0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
        [1] = c_gmres_DWork.UnitDelay_DSTATE_d[1];

      /* Switch: '<S139>/Switch' */
      if (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_ForIterator_1
          > c_gmres_P.Switch_Threshold_az) {
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
          [0] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
          [0];
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
          [1] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
          [1];
      } else {
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
          [0] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Sum1_1
          [0];
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
          [1] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Sum1_1
          [1];
      }

      /* End of Switch: '<S139>/Switch' */

      /* Reshape: '<S138>/Reshape1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [1];

      /* Saturate: '<S140>/Saturation' */
      Bias =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
        [0];
      x_idx_1 = c_gmres_P.Saturation_LowerSat_f;
      y_idx_0 = c_gmres_P.Saturation_UpperSat_jw;
      if (Bias > y_idx_0) {
        Bias = y_idx_0;
      } else {
        if (Bias < x_idx_1) {
          Bias = x_idx_1;
        }
      }

      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
        [0] = Bias;
      Bias =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
        [1];
      x_idx_1 = c_gmres_P.Saturation_LowerSat_f;
      y_idx_0 = c_gmres_P.Saturation_UpperSat_jw;
      if (Bias > y_idx_0) {
        Bias = y_idx_0;
      } else {
        if (Bias < x_idx_1) {
          Bias = x_idx_1;
        }
      }

      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
        [1] = Bias;

      /* End of Saturate: '<S140>/Saturation' */

      /* Sqrt: '<S140>/Sqrt' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
        [0] = sqrt
        (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
         [0]);
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
        [1] = sqrt
        (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
         [1]);

      /* Gain: '<S140>/Gain' */
      Bias = -c_gmres_P.C_plant[0];
      x_idx_1 = c_gmres_P.tank_area[0];
      Bias /= x_idx_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain_1
        = Bias *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
        [0];

      /* Gain: '<S140>/Gain1' */
      Bias = c_gmres_P.C_plant[0];
      x_idx_1 = c_gmres_P.tank_area[1];
      Bias /= x_idx_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
        = Bias *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
        [0];

      /* Gain: '<S140>/Gain2' */
      Bias = -c_gmres_P.C_plant[1];
      x_idx_1 = c_gmres_P.tank_area[1];
      Bias /= x_idx_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain2_1
        = Bias *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
        [1];

      /* Sum: '<S140>/Add' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
        + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain2_1;

      /* Sum: '<S140>/Add1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
        [0] +
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
        [1] +
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add_1;

      /* Gain: '<S138>/Gain' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Gain_1
        = c_gmres_P.Gain_Gain_e * c_gmres_fdgmres_calculation_tau_Gain2_1;

      /* Product: '<S139>/Product' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
        [0] *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Gain_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
        [1] *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Gain_1;

      /* Sum: '<S139>/Add' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
        [0] +
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
        [1] +
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
        [1];

      /* Reshape: '<S138>/Reshape' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Reshape_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Reshape_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
        [1];

      /* Assignment: '<S138>/Assignment' */
      if (c_gmres_fdgmres_Initialize_mode == 1) {
        memcpy
          (&c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Assignment_1
           [0],
           &c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Constant_1
           [0], 40U * sizeof(real_T));
      }

      i_0 =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_ForIterator_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Assignment_1
        [(i_0 - 1) << 1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Reshape_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Assignment_1
        [1 + ((i_0 - 1) << 1)] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Reshape_1
        [1];

      /* End of Assignment: '<S138>/Assignment' */

      /* Update for UnitDelay: '<S139>/UnitDelay' */
      c_gmres_DWork.UnitDelay_DSTATE_d[0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
        [0];
      c_gmres_DWork.UnitDelay_DSTATE_d[1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
        [1];
    }

    /* End of Outputs for SubSystem: '<S129>/ForIteratorSubsystem' */

    /* SignalConversion: '<S110>/ConcatBufferAtVectorConcatenateIn2' */
    memcpy
      (&c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_VectorConcatenate_1
       [2],
       &c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Assignment_1
       [0], 40U * sizeof(real_T));

    /* Selector: '<S110>/Selector' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Selector_1
      [0] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Assignment_1
      [38];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Selector_1
      [1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Assignment_1
      [39];

    /* Selector: '<S110>/Selector1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Selector1_1
      [0] = c_gmres_Subsystem_Selector1_1[38];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Selector1_1
      [1] = c_gmres_Subsystem_Selector1_1[39];

    /* Sum: '<S127>/Subtract' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Subtract_1
      [0] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Selector_1
      [0] -
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Selector1_1
      [0];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Subtract_1
      [1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Selector_1
      [1] -
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Selector1_1
      [1];

    /* Constant: '<S127>/Constant' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Constant_1
      = c_gmres_P.Constant_Value_oa;

    /* Product: '<S127>/Product' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Product_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Subtract_1
      [0] *
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Constant_1;

    /* Constant: '<S127>/Constant1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Constant1_1
      = c_gmres_P.Constant1_Value_b;

    /* Product: '<S127>/Product1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Product1_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Subtract_1
      [1] *
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Constant1_1;

    /* Sum: '<S127>/Add' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Add_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Product_1
      + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Product1_1;

    /* SignalConversion: '<S127>/ConcatBufferAtVectorConcatenateIn1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_VectorConcatenate_1
      [0] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Add_1;

    /* Constant: '<S127>/Constant2' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Constant2_1
      = c_gmres_P.Constant2_Value_e;

    /* Product: '<S127>/Product2' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Product2_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Subtract_1
      [1] *
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Constant2_1;

    /* Constant: '<S127>/Constant3' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Constant3_1
      = c_gmres_P.Constant3_Value_g;

    /* Product: '<S127>/Product3' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Product3_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Subtract_1
      [0] *
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Constant3_1;

    /* Sum: '<S127>/Add1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Add1_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Product2_1
      + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Product3_1;

    /* SignalConversion: '<S127>/ConcatBufferAtVectorConcatenateIn2' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_VectorConcatenate_1
      [1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Add1_1;

    /* Outputs for Iterator SubSystem: '<S126>/ForIteratorSubsystem' incorporates:
     *  ForIterator: '<S130>/ForIterator'
     */
    if (c_gmres_DWork.ForIterator_IterationMarker_c != 0) {
      /* InitializeConditions for UnitDelay: '<S132>/UnitDelay' */
      c_gmres_DWork.UnitDelay_DSTATE_a[0] =
        c_gmres_P.UnitDelay_InitialCondition_m;
      c_gmres_DWork.UnitDelay_DSTATE_a[1] =
        c_gmres_P.UnitDelay_InitialCondition_m;
    }

    c_gmres_DWork.ForIterator_IterationMarker_c = 1U;
    for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
         c_gmres_P.ForIterator_IterationLimit_af;
         c_gmres_fdgmres_Initialize_mode++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
        = c_gmres_fdgmres_Initialize_mode;

      /* Constant: '<S130>/Constant' */
      memcpy
        (&c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
         [0], &c_gmres_P.Constant_Value_m[0], 38U * sizeof(real_T));

      /* Constant: '<S133>/Q11' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
        = c_gmres_P.Q11_Value_a;

      /* Constant: '<S130>/Constant1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
        = c_gmres_P.Constant1_Value_n;

      /* Sum: '<S130>/Subtract' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
        - (real_T)
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;

      /* Selector: '<S130>/Selector' */
      i_0 = (int32_T)
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_VectorConcatenate_1
        [(i_0 - 1) << 1];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_VectorConcatenate_1
        [((i_0 - 1) << 1) + 1];

      /* Selector: '<S130>/Selector1' */
      i_0 = (int32_T)
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
        [0] = c_gmres_Subsystem_Selector1_1[(i_0 - 1) << 1];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
        [1] = c_gmres_Subsystem_Selector1_1[((i_0 - 1) << 1) + 1];

      /* Sum: '<S131>/Subtract' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
        [0] -
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
        [1] -
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
        [1];

      /* Product: '<S133>/Product' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
        * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
        [0];

      /* Constant: '<S133>/Q12_Q21' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
        = c_gmres_P.Q12_Q21_Value_j5;

      /* Product: '<S133>/Product1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
        * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
        [1];

      /* Sum: '<S133>/Add' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
        + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1;

      /* UnitDelay: '<S132>/UnitDelay' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
        [0] = c_gmres_DWork.UnitDelay_DSTATE_a[0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
        [1] = c_gmres_DWork.UnitDelay_DSTATE_a[1];

      /* Switch: '<S132>/Switch' */
      if (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
          > c_gmres_P.Switch_Threshold_gl) {
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
          [0] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
          [0];
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
          [1] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
          [1];
      } else {
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
          [0] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_VectorConcatenate_1
          [0];
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
          [1] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_VectorConcatenate_1
          [1];
      }

      /* End of Switch: '<S132>/Switch' */

      /* Constant: '<S133>/C1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
        = c_gmres_P.C_plant[0];

      /* Constant: '<S133>/A1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1
        = c_gmres_P.tank_area[0];

      /* Product: '<S133>/Divide' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
        /
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1;

      /* Product: '<S133>/Product2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [0] *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1;

      /* Constant: '<S133>/A2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1
        = c_gmres_P.tank_area[1];

      /* Product: '<S133>/Divide1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
        /
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1;

      /* Product: '<S133>/Product3' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [1] *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1;

      /* Sum: '<S133>/Add1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
        - c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1;

      /* Sqrt: '<S131>/Sqrt' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
        [0] = sqrt
        (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
         [0]);
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
        [1] = sqrt
        (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
         [1]);

      /* Product: '<S133>/Divide2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
        /
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
        [0];

      /* Gain: '<S133>/Gain' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1
        = c_gmres_P.Gain_Gain_c1g *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1;

      /* Sum: '<S133>/Add2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
        + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1;

      /* SignalConversion: '<S131>/ConcatBufferAtVectorConcatenateIn1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1;

      /* Constant: '<S134>/Q22' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
        = c_gmres_P.Q22_Value_n;

      /* Product: '<S134>/Product' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
        * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
        [1];

      /* Constant: '<S134>/Q12_Q21' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
        = c_gmres_P.Q12_Q21_Value_f;

      /* Product: '<S134>/Product1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
        * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
        [0];

      /* Constant: '<S134>/C2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
        = c_gmres_P.C_plant[1];

      /* Constant: '<S134>/A2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
        = c_gmres_P.tank_area[1];

      /* Product: '<S134>/Product3' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
        * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [1] /
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
        /
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
        [1];

      /* Gain: '<S134>/Gain' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1
        = c_gmres_P.Gain_Gain_b *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1;

      /* Sum: '<S134>/Add' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1
        =
        (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
         + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1)
        - c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1;

      /* SignalConversion: '<S131>/ConcatBufferAtVectorConcatenateIn2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1;

      /* Gain: '<S130>/Gain' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Gain_1
        = c_gmres_P.Gain_Gain_d1 * c_gmres_fdgmres_calculation_tau_Gain2_1;

      /* Product: '<S132>/Product' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
        [0] *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
        [1] *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Gain_1;

      /* Sum: '<S132>/Add' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
        [0] +
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
        [1] +
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
        [1];

      /* Reshape: '<S130>/Reshape' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
        [1];

      /* Constant: '<S130>/Constant2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
        = c_gmres_P.Constant2_Value_p4;

      /* Sum: '<S130>/Subtract1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
        - (real_T)
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1;

      /* Assignment: '<S130>/Assignment' */
      if (c_gmres_fdgmres_Initialize_mode == 1) {
        memcpy
          (&c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
           [0],
           &c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
           [0], 38U * sizeof(real_T));
      }

      i_0 = (int32_T)
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
        [(i_0 - 1) << 1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
        [1 + ((i_0 - 1) << 1)] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
        [1];

      /* End of Assignment: '<S130>/Assignment' */

      /* Update for UnitDelay: '<S132>/UnitDelay' */
      c_gmres_DWork.UnitDelay_DSTATE_a[0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
        [0];
      c_gmres_DWork.UnitDelay_DSTATE_a[1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
        [1];
    }

    /* End of Outputs for SubSystem: '<S126>/ForIteratorSubsystem' */

    /* SignalConversion: '<S110>/ConcatBufferAtVectorConcatenate1In1' */
    memcpy
      (&c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_VectorConcatenate1_1
       [0],
       &c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
       [0], 38U * sizeof(real_T));

    /* SignalConversion: '<S110>/ConcatBufferAtVectorConcatenate1In2' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_VectorConcatenate1_1
      [38] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_VectorConcatenate_1
      [0];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_VectorConcatenate1_1
      [39] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_VectorConcatenate_1
      [1];

    /* Outputs for Iterator SubSystem: '<S128>/ForIteratorSubsystem' incorporates:
     *  ForIterator: '<S135>/ForIterator'
     */
    c_gmres_DWork.ForIterator_IterationMarker_p = 1U;
    for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
         c_gmres_P.ForIterator_IterationLimit_j; c_gmres_fdgmres_Initialize_mode
         ++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_ForIterator_1
        = c_gmres_fdgmres_Initialize_mode;

      /* Selector: '<S135>/Selector' */
      i_0 =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_ForIterator_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Selector_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Sum_1
        [(i_0 - 1) << 1];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Selector_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Sum_1
        [((i_0 - 1) << 1) + 1];

      /* Constant: '<S136>/r1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_r1_1
        = 2.0 * c_gmres_P.p_idx_r[0];

      /* Product: '<S136>/Product' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_Product_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Selector_1
        [0] *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_r1_1;

      /* Selector: '<S135>/Selector1' */
      i_0 =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_ForIterator_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Selector1_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_VectorConcatenate1_1
        [(i_0 - 1) << 1];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Selector1_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_VectorConcatenate1_1
        [((i_0 - 1) << 1) + 1];

      /* Constant: '<S136>/A1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_A1_1
        = c_gmres_P.tank_area[0];

      /* Product: '<S136>/Divide' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_Divide_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Selector1_1
        [0] /
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_A1_1;

      /* Sum: '<S136>/Add' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_Add_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_Product_1
        + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_Divide_1;

      /* SignalConversion: '<S135>/ConcatBufferAtVectorConcatenateIn1' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_Add_1;

      /* Constant: '<S137>/r2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_r2_1
        = 2.0 * c_gmres_P.p_idx_r[1];

      /* Product: '<S137>/Product' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_Product_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Selector_1
        [1] *
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_r2_1;

      /* Constant: '<S137>/A2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_A2_1
        = c_gmres_P.tank_area[1];

      /* Product: '<S137>/Divide' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_Divide_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Selector1_1
        [1] /
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_A2_1;

      /* Sum: '<S137>/Add' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_Add_1
        =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_Product_1
        + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_Divide_1;

      /* SignalConversion: '<S135>/ConcatBufferAtVectorConcatenateIn2' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_Add_1;
      for (i = 0; i < 40; i++) {
        /* Constant: '<S135>/Constant' */
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Constant_1
          [i] = c_gmres_P.Constant_Value_cl[i];

        /* Assignment: '<S135>/Assignment' */
        if (c_gmres_fdgmres_Initialize_mode == 1) {
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Assignment_1
            [i] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Constant_1
            [i];
        }
      }

      /* Assignment: '<S135>/Assignment' */
      i_0 =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_ForIterator_1;
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Assignment_1
        [(i_0 - 1) << 1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Assignment_1
        [1 + ((i_0 - 1) << 1)] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
        [1];
    }

    /* End of Outputs for SubSystem: '<S128>/ForIteratorSubsystem' */
    for (i = 0; i < 40; i++) {
      /* Sum: '<S107>/Subtract' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Subtract_1
        [i] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Assignment_1
        [i] -
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Assignment_1
        [i];

      /* Product: '<S107>/Divide' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Divide_1
        [i] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Subtract_1
        [i] / c_gmres_Constant1_1;

      /* Reshape: '<S104>/Reshape' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Reshape_1
        [i] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Divide_1
        [i];
    }

    /* Outputs for Iterator SubSystem: '<S98>/ForIteratorSubsystem' incorporates:
     *  ForIterator: '<S103>/ForIterator'
     */
    if (c_gmres_DWork.ForIterator_IterationMarker_j != 0) {
      /* InitializeConditions for UnitDelay: '<S103>/v_vector' */
      memcpy(&c_gmres_DWork.v_vector_DSTATE[0],
             &c_gmres_P.v_vector_InitialCondition[0], 40U * sizeof(real_T));
    }

    c_gmres_DWork.ForIterator_IterationMarker_j = 1U;
    for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
         c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1;
         c_gmres_fdgmres_Initialize_mode++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_ForIterator_1
        = c_gmres_fdgmres_Initialize_mode;

      /* Constant: '<S103>/h_vector_initial' */
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_h_vector_initial_1
        [0] = c_gmres_P.h_vector_initial_Value[0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_h_vector_initial_1
        [1] = c_gmres_P.h_vector_initial_Value[1];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_h_vector_initial_1
        [2] = c_gmres_P.h_vector_initial_Value[2];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_h_vector_initial_1
        [3] = c_gmres_P.h_vector_initial_Value[3];

      /* Selector: '<S103>/Selector' */
      i_0 =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_ForIterator_1;

      /* DotProduct: '<S103>/DotProduct' */
      Bias = 0.0;
      for (i = 0; i < 40; i++) {
        /* UnitDelay: '<S103>/v_vector' */
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_v_vector_1
          [i] = c_gmres_DWork.v_vector_DSTATE[i];

        /* Switch: '<S103>/Switch' */
        if (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_ForIterator_1
            > c_gmres_P.Switch_Threshold_p) {
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Switch_1
            [i] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_v_vector_1
            [i];
        } else {
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Switch_1
            [i] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Reshape_1
            [i];
        }

        /* End of Switch: '<S103>/Switch' */

        /* Selector: '<S103>/Selector' */
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Selector_1
          [i] = c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Switch_1[(i_0
          - 1) * 40 + i];
        x_idx_1 =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Switch_1
          [i];
        y_idx_0 =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Selector_1
          [i];
        Bias += x_idx_1 * y_idx_0;
      }

      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_DotProduct_1
        = Bias;

      /* End of DotProduct: '<S103>/DotProduct' */

      /* Assignment: '<S103>/Assignment1' */
      if (c_gmres_fdgmres_Initialize_mode == 1) {
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Assignment1_1
          [0] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_h_vector_initial_1
          [0];
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Assignment1_1
          [1] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_h_vector_initial_1
          [1];
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Assignment1_1
          [2] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_h_vector_initial_1
          [2];
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Assignment1_1
          [3] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_h_vector_initial_1
          [3];
      }

      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Assignment1_1
        [c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_ForIterator_1
        - 1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_DotProduct_1;
      for (i = 0; i < 40; i++) {
        /* Product: '<S103>/Product' */
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Product_1
          [i] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_DotProduct_1
          * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Selector_1
          [i];

        /* Sum: '<S103>/Subtract' */
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Subtract_1
          [i] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Switch_1
          [i] -
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Product_1
          [i];

        /* Update for UnitDelay: '<S103>/v_vector' */
        c_gmres_DWork.v_vector_DSTATE[i] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Subtract_1
          [i];
      }

      /* End of Assignment: '<S103>/Assignment1' */
    }

    /* End of Outputs for SubSystem: '<S98>/ForIteratorSubsystem' */

    /* DotProduct: '<S105>/DotProduct' */
    Bias = 0.0;
    for (i_0 = 0; i_0 < 40; i_0++) {
      x_idx_1 =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Subtract_1
        [i_0];
      y_idx_0 =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Subtract_1
        [i_0];
      Bias += x_idx_1 * y_idx_0;
    }

    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_two_norm_DotProduct_1
      = Bias;

    /* End of DotProduct: '<S105>/DotProduct' */

    /* Sqrt: '<S105>/Sqrt' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_two_norm_Sqrt_1
      = sqrt
      (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_two_norm_DotProduct_1);

    /* Width: '<S102>/Width1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_Width1_1
      = 4.0;

    /* Outputs for Iterator SubSystem: '<S102>/ForIteratorSubsystem' incorporates:
     *  ForIterator: '<S106>/ForIterator'
     */
    c_gmres_DWork.ForIterator_IterationMarker_gk = 1U;
    c_gmres_fdgmres_Initialize_mode = 1;
    do {
      exitg3 = 0;
      Bias =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_Width1_1;
      if (Bias < 0.0) {
        Bias = ceil(Bias);
      } else {
        Bias = floor(Bias);
      }

      if (rtIsNaN(Bias) || rtIsInf(Bias)) {
        Bias = 0.0;
      } else {
        Bias = fmod(Bias, 4.294967296E+9);
      }

      if (c_gmres_fdgmres_Initialize_mode <= (Bias < 0.0 ? -(int32_T)(uint32_T)
           -Bias : (int32_T)(uint32_T)Bias)) {
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_ForIterator_1
          = c_gmres_fdgmres_Initialize_mode;
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_RelationalOperator_1
          =
          (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_ForIterator_1
           == c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Add_1);
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Selector_1
          =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Assignment1_1
          [c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_ForIterator_1
          - 1];
        if (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_RelationalOperator_1)
        {
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Switch_1
            =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_two_norm_Sqrt_1;
        } else {
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Switch_1
            =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Selector_1;
        }

        if (c_gmres_fdgmres_Initialize_mode == 1) {
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Assignment_1
            [0] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Assignment1_1
            [0];
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Assignment_1
            [1] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Assignment1_1
            [1];
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Assignment_1
            [2] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Assignment1_1
            [2];
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Assignment_1
            [3] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Assignment1_1
            [3];
        }

        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Assignment_1
          [c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_ForIterator_1
          - 1] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Switch_1;
        c_gmres_fdgmres_Initialize_mode++;
      } else {
        exitg3 = 1;
      }
    } while (exitg3 == 0);

    /* End of Outputs for SubSystem: '<S102>/ForIteratorSubsystem' */

    /* Reshape: '<S102>/Reshape1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_Reshape1_1
      [0] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Assignment_1
      [0];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_Reshape1_1
      [1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Assignment_1
      [1];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_Reshape1_1
      [2] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Assignment_1
      [2];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_Reshape1_1
      [3] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Assignment_1
      [3];

    /* Assignment: '<S98>/Assignment_h_matrix' */
    if (c_gmres_fdgmres_Iterativvelymin == 1) {
      memcpy
        (&c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_matrix_1
         [0],
         &c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_h_matrix_1
         [0], 12U * sizeof(real_T));
    }

    i_0 = c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1;
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_matrix_1
      [(i_0 - 1) << 2] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_Reshape1_1
      [0];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_matrix_1
      [1 + ((i_0 - 1) << 2)] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_Reshape1_1
      [1];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_matrix_1
      [2 + ((i_0 - 1) << 2)] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_Reshape1_1
      [2];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_matrix_1
      [3 + ((i_0 - 1) << 2)] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_Reshape1_1
      [3];

    /* End of Assignment: '<S98>/Assignment_h_matrix' */

    /* Product: '<S98>/Divide' */
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Divide_1[
        i] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Subtract_1
        [i] /
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_two_norm_Sqrt_1;
    }

    /* End of Product: '<S98>/Divide' */

    /* Assignment: '<S98>/Assignment_q_matrix' */
    if (c_gmres_fdgmres_Iterativvelymin == 1) {
      memcpy
        (&c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_q_matrix_1
         [0], &c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Switch_1[0],
         160U * sizeof(real_T));
    }

    i_0 = (int32_T)c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Add_1;
    memcpy
      (&c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_q_matrix_1
       [40 * (i_0 - 1)],
       &c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Divide_1
       [0], 40U * sizeof(real_T));

    /* End of Assignment: '<S98>/Assignment_q_matrix' */

    /* Sum: '<S104>/Add' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Add_1
      = c_gmres_DigitalClock_1 + c_gmres_Constant1_1;

    /* Constant: '<S104>/zero2' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_zero2_1
      = c_gmres_P.zero2_Value;

    /* Product: '<S107>/Product2' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Product2_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_zero2_1
      * c_gmres_Constant1_1;

    /* Sum: '<S107>/Sum2' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Sum2_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Add_1
      + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Product2_1;

    /* Selector: '<S99>/Selector3' */
    i_0 = c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1;
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector3_1
      [0] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_matrix_1
      [(i_0 - 1) << 2];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector3_1
      [1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_matrix_1
      [((i_0 - 1) << 2) + 1];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector3_1
      [2] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_matrix_1
      [((i_0 - 1) << 2) + 2];

    /* Selector: '<S99>/Selector' */
    i_0 = c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1;
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector_1
      [0] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_matrix_1
      [(i_0 - 1) << 2];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector_1
      [1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_matrix_1
      [((i_0 - 1) << 2) + 1];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector_1
      [2] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_matrix_1
      [((i_0 - 1) << 2) + 2];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector_1
      [3] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_matrix_1
      [((i_0 - 1) << 2) + 3];

    /* UnitDelay: '<S9>/s_vector' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_s_vector_1[0] =
      c_gmres_DWork.s_vector_DSTATE[0];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_s_vector_1[1] =
      c_gmres_DWork.s_vector_DSTATE[1];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_s_vector_1[2] =
      c_gmres_DWork.s_vector_DSTATE[2];

    /* UnitDelay: '<S9>/c_vector' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_c_vector_1[0] =
      c_gmres_DWork.c_vector_DSTATE[0];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_c_vector_1[1] =
      c_gmres_DWork.c_vector_DSTATE[1];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_c_vector_1[2] =
      c_gmres_DWork.c_vector_DSTATE[2];

    /* Constant: '<S99>/one' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_one_1
      = c_gmres_P.one_Value_i;

    /* Sum: '<S99>/Subtract' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Subtract_1
      = (real_T)c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1
      - c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_one_1;

    /* Outputs for Iterator SubSystem: '<S143>/ForIteratorSubsystem' incorporates:
     *  ForIterator: '<S144>/ForIterator'
     */
    if (c_gmres_DWork.ForIterator_IterationMarker_d != 0) {
      /* InitializeConditions for UnitDelay: '<S144>/b_hat' */
      c_gmres_DWork.b_hat_DSTATE_d[0] = c_gmres_P.b_hat_InitialCondition[0];
      c_gmres_DWork.b_hat_DSTATE_d[1] = c_gmres_P.b_hat_InitialCondition[1];
      c_gmres_DWork.b_hat_DSTATE_d[2] = c_gmres_P.b_hat_InitialCondition[2];
    }

    c_gmres_DWork.ForIterator_IterationMarker_d = 1U;
    c_gmres_fdgmres_Initialize_mode = 1;
    do {
      exitg2 = 0;
      Bias =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Subtract_1;
      if (Bias < 0.0) {
        Bias = ceil(Bias);
      } else {
        Bias = floor(Bias);
      }

      if (rtIsNaN(Bias) || rtIsInf(Bias)) {
        Bias = 0.0;
      } else {
        Bias = fmod(Bias, 4.294967296E+9);
      }

      if (c_gmres_fdgmres_Initialize_mode <= (Bias < 0.0 ? -(int32_T)(uint32_T)
           -Bias : (int32_T)(uint32_T)Bias)) {
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_ForIterator_1
          = c_gmres_fdgmres_Initialize_mode;
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector1_c_j_1
          =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_c_vector_1[c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_ForIterator_1
          - 1];
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_b_hat_1
          [0] = c_gmres_DWork.b_hat_DSTATE_d[0];
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_b_hat_1
          [1] = c_gmres_DWork.b_hat_DSTATE_d[1];
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_b_hat_1
          [2] = c_gmres_DWork.b_hat_DSTATE_d[2];
        if (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_ForIterator_1
            > c_gmres_P.Switch1_Threshold) {
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Switch1_1
            [0] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_b_hat_1
            [0];
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Switch1_1
            [1] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_b_hat_1
            [1];
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Switch1_1
            [2] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_b_hat_1
            [2];
        } else {
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Switch1_1
            [0] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector3_1
            [0];
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Switch1_1
            [1] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector3_1
            [1];
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Switch1_1
            [2] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector3_1
            [2];
        }

        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector2_r_j_1n
          =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Switch1_1
          [c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_ForIterator_1
          - 1];
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Product_1
          =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector1_c_j_1
          * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector2_r_j_1n;
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector_s_j_1
          =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_s_vector_1[c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_ForIterator_1
          - 1];
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_one_1
          = c_gmres_P.one_Value_p;
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Add2_1
          = (real_T)
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_ForIterator_1
          + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_one_1;
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector3_h_jn
          =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector_1
          [(int32_T)
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Add2_1
          - 1];
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Product1_1
          =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector_s_j_1
          * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector3_h_jn;
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Add_1
          =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Product_1
          + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Product1_1;
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Product2_1
          =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector_s_j_1
          * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector2_r_j_1n;
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Product3_1
          =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector1_c_j_1
          * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector3_h_jn;
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Add1_1
          =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Product3_1
          - c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Product2_1;
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_VectorConcatenate_1
          [0] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Add_1;
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_VectorConcatenate_1
          [1] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Add1_1;
        if (c_gmres_fdgmres_Initialize_mode == 1) {
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Assignment_1
            [0] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Switch1_1
            [0];
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Assignment_1
            [1] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Switch1_1
            [1];
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Assignment_1
            [2] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Switch1_1
            [2];
        }

        i_0 =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_ForIterator_1;
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Assignment_1
          [i_0 - 1] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_VectorConcatenate_1
          [0];
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Assignment_1
          [i_0] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_VectorConcatenate_1
          [1];
        c_gmres_DWork.b_hat_DSTATE_d[0] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Assignment_1
          [0];
        c_gmres_DWork.b_hat_DSTATE_d[1] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Assignment_1
          [1];
        c_gmres_DWork.b_hat_DSTATE_d[2] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Assignment_1
          [2];
        c_gmres_fdgmres_Initialize_mode++;
      } else {
        exitg2 = 1;
      }
    } while (exitg2 == 0);

    /* End of Outputs for SubSystem: '<S143>/ForIteratorSubsystem' */

    /* Switch: '<S142>/Switch' */
    if (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1 >
        c_gmres_P.Switch_Threshold_fr) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Switch_Switch_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Assignment_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Switch_Switch_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Assignment_1
        [1];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Switch_Switch_1
        [2] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Assignment_1
        [2];
    } else {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Switch_Switch_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector3_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Switch_Switch_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector3_1
        [1];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Switch_Switch_1
        [2] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector3_1
        [2];
    }

    /* End of Switch: '<S142>/Switch' */

    /* Selector: '<S141>/Selector1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Selector1_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Switch_Switch_1
      [c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1 - 1];

    /* Math: '<S141>/Square' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Square_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Selector1_1
      * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Selector1_1;

    /* Selector: '<S141>/Selector2' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Selector2_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_matrix_1
      [(((c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1 - 1) <<
         2) + (int32_T)c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Add_1)
      - 1];

    /* Math: '<S141>/Square1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Square1_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Selector2_1
      * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Selector2_1;

    /* Sum: '<S141>/Add' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Add_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Square_1
      + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Square1_1;

    /* Sqrt: '<S141>/Sqrt' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Sqrt_1
      = sqrt
      (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Add_1);

    /* Product: '<S141>/Divide3' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Divide3_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Square_1
      /
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Sqrt_1;

    /* Product: '<S141>/Divide2' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Divide2_1
      = 1.0 /
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Sqrt_1
      * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Square1_1;

    /* Sum: '<S141>/Add1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Add1_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Divide3_1
      + c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Divide2_1;

    /* Product: '<S141>/Divide' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Divide_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Selector1_1
      /
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Sqrt_1;

    /* Assignment: '<S141>/Assignment1' */
    if (c_gmres_fdgmres_Iterativvelymin == 1) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment1_1
        [0] = c_gmres_fdgmres_Iterativvelyminimizeresidualvector_c_vector_1[0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment1_1
        [1] = c_gmres_fdgmres_Iterativvelyminimizeresidualvector_c_vector_1[1];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment1_1
        [2] = c_gmres_fdgmres_Iterativvelyminimizeresidualvector_c_vector_1[2];
    }

    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment1_1
      [c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1 - 1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Divide_1;

    /* End of Assignment: '<S141>/Assignment1' */

    /* Product: '<S141>/Divide1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Divide1_1
      = 1.0 /
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Sqrt_1
      * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Selector2_1;

    /* Assignment: '<S141>/Assignment2' */
    if (c_gmres_fdgmres_Iterativvelymin == 1) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment2_1
        [0] = c_gmres_fdgmres_Iterativvelyminimizeresidualvector_s_vector_1[0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment2_1
        [1] = c_gmres_fdgmres_Iterativvelyminimizeresidualvector_s_vector_1[1];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment2_1
        [2] = c_gmres_fdgmres_Iterativvelyminimizeresidualvector_s_vector_1[2];
    }

    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment2_1
      [c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1 - 1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Divide1_1;

    /* End of Assignment: '<S141>/Assignment2' */

    /* UnitDelay: '<S100>/b_hat' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_b_hat_1
      [0] = c_gmres_DWork.b_hat_DSTATE[0];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_b_hat_1
      [1] = c_gmres_DWork.b_hat_DSTATE[1];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_b_hat_1
      [2] = c_gmres_DWork.b_hat_DSTATE[2];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_b_hat_1
      [3] = c_gmres_DWork.b_hat_DSTATE[3];

    /* Switch: '<S100>/Switch1' */
    if (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1 >
        c_gmres_P.Switch1_Threshold_d) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Switch1_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_b_hat_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Switch1_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_b_hat_1
        [1];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Switch1_1
        [2] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_b_hat_1
        [2];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Switch1_1
        [3] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_b_hat_1
        [3];
    } else {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Switch1_1
        [0] = c_gmres_fdgmres_Initialize_model_Reshape1_1[0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Switch1_1
        [1] = c_gmres_fdgmres_Initialize_model_Reshape1_1[1];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Switch1_1
        [2] = c_gmres_fdgmres_Initialize_model_Reshape1_1[2];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Switch1_1
        [3] = c_gmres_fdgmres_Initialize_model_Reshape1_1[3];
    }

    /* End of Switch: '<S100>/Switch1' */

    /* Selector: '<S100>/Selector' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Selector_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Switch1_1
      [c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1 - 1];

    /* Product: '<S100>/Product1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Product1_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Selector_1
      * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Divide_1;

    /* Abs: '<S100>/Abs' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Abs_rho
      = fabs
      (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Product1_1);

    /* SignalConversion: '<S100>/ConcatBufferAtVectorConcatenateIn1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_VectorConcatenate_1
      [0] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Product1_1;

    /* Product: '<S100>/Product' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Product_1
      =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Selector_1
      * c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Divide1_1;

    /* Gain: '<S100>/Gain' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Gain_1
      = c_gmres_P.Gain_Gain_h *
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Product_1;

    /* SignalConversion: '<S100>/ConcatBufferAtVectorConcatenateIn2' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_VectorConcatenate_1
      [1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Gain_1;

    /* Assignment: '<S100>/Assignment_b_hat' */
    if (c_gmres_fdgmres_Iterativvelymin == 1) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Assignment_b_hat_1
        [0] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Switch1_1
        [0];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Assignment_b_hat_1
        [1] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Switch1_1
        [1];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Assignment_b_hat_1
        [2] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Switch1_1
        [2];
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Assignment_b_hat_1
        [3] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Switch1_1
        [3];
    }

    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Assignment_b_hat_1
      [c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1 - 1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_VectorConcatenate_1
      [0];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Assignment_b_hat_1
      [c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_VectorConcatenate_1
      [1];

    /* End of Assignment: '<S100>/Assignment_b_hat' */

    /* UnitDelay: '<S101>/r_matrix' */
    memcpy
      (&c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_r_matrix_1
       [0], &c_gmres_DWork.r_matrix_DSTATE[0], 9U * sizeof(real_T));

    /* Width: '<S146>/Width1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_Width1_1
      = 3.0;

    /* Outputs for Iterator SubSystem: '<S146>/ForIteratorSubsystem' incorporates:
     *  ForIterator: '<S147>/ForIterator'
     */
    c_gmres_DWork.ForIterator_IterationMarker = 1U;
    c_gmres_fdgmres_Initialize_mode = 1;
    do {
      exitg1 = 0;
      Bias =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_Width1_1;
      if (Bias < 0.0) {
        Bias = ceil(Bias);
      } else {
        Bias = floor(Bias);
      }

      if (rtIsNaN(Bias) || rtIsInf(Bias)) {
        Bias = 0.0;
      } else {
        Bias = fmod(Bias, 4.294967296E+9);
      }

      if (c_gmres_fdgmres_Initialize_mode <= (Bias < 0.0 ? -(int32_T)(uint32_T)
           -Bias : (int32_T)(uint32_T)Bias)) {
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_ForIterator_1
          = c_gmres_fdgmres_Initialize_mode;
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_RelationalOperator_1
          =
          (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_ForIterator_1
           == c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1);
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Selector_1
          =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Switch_Switch_1
          [c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_ForIterator_1
          - 1];
        if (c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_RelationalOperator_1)
        {
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Switch_1
            =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Add1_1;
        } else {
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Switch_1
            =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Selector_1;
        }

        if (c_gmres_fdgmres_Initialize_mode == 1) {
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Assignment_1
            [0] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Switch_Switch_1
            [0];
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Assignment_1
            [1] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Switch_Switch_1
            [1];
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Assignment_1
            [2] =
            c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Switch_Switch_1
            [2];
        }

        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Assignment_1
          [c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_ForIterator_1
          - 1] =
          c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Switch_1;
        c_gmres_fdgmres_Initialize_mode++;
      } else {
        exitg1 = 1;
      }
    } while (exitg1 == 0);

    /* End of Outputs for SubSystem: '<S146>/ForIteratorSubsystem' */

    /* Assignment: '<S101>/Assignment_r_matrix' */
    if (c_gmres_fdgmres_Iterativvelymin == 1) {
      memcpy
        (&c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_matrix_1
         [0],
         &c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_r_matrix_1
         [0], 9U * sizeof(real_T));
    }

    i_0 = c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1;
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_matrix_1
      [3 * (i_0 - 1)] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Assignment_1
      [0];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_matrix_1
      [1 + 3 * (i_0 - 1)] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Assignment_1
      [1];
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_matrix_1
      [2 + 3 * (i_0 - 1)] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Assignment_1
      [2];

    /* End of Assignment: '<S101>/Assignment_r_matrix' */

    /* Update for UnitDelay: '<S98>/h_matrix' */
    memcpy(&c_gmres_DWork.h_matrix_DSTATE[0],
           &c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_matrix_1
           [0], 12U * sizeof(real_T));

    /* Update for UnitDelay: '<S9>/q_matrix' */
    memcpy(&c_gmres_DWork.q_matrix_DSTATE[0],
           &c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_q_matrix_1
           [0], 160U * sizeof(real_T));

    /* Update for UnitDelay: '<S9>/s_vector' */
    c_gmres_DWork.s_vector_DSTATE[0] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment2_1
      [0];
    c_gmres_DWork.s_vector_DSTATE[1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment2_1
      [1];
    c_gmres_DWork.s_vector_DSTATE[2] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment2_1
      [2];

    /* Update for UnitDelay: '<S9>/c_vector' */
    c_gmres_DWork.c_vector_DSTATE[0] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment1_1
      [0];
    c_gmres_DWork.c_vector_DSTATE[1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment1_1
      [1];
    c_gmres_DWork.c_vector_DSTATE[2] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment1_1
      [2];

    /* Update for UnitDelay: '<S100>/b_hat' */
    c_gmres_DWork.b_hat_DSTATE[0] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Assignment_b_hat_1
      [0];
    c_gmres_DWork.b_hat_DSTATE[1] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Assignment_b_hat_1
      [1];
    c_gmres_DWork.b_hat_DSTATE[2] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Assignment_b_hat_1
      [2];
    c_gmres_DWork.b_hat_DSTATE[3] =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Assignment_b_hat_1
      [3];

    /* Update for UnitDelay: '<S101>/r_matrix' */
    memcpy(&c_gmres_DWork.r_matrix_DSTATE[0],
           &c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_matrix_1
           [0], 9U * sizeof(real_T));
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_ITERATIVVELYMINIMIZERESIDUALVECTOR,0,0,C_GMRES_FDGMRES_ITERATIVVELYMINIMIZERESIDUALVECTOR_PARAM);

  /* End of Outputs for SubSystem: '<S4>/Iterativvelyminimizeresidualvector' */

MEASUREMENT_START(C_GMRES_FDGMRES_CALCULATEY,0,0,C_GMRES_FDGMRES_CALCULATEY_PARAM);
  /* Outputs for Iterator SubSystem: '<S4>/Calculatey' incorporates:
   *  ForIterator: '<S7>/ForIterator_l'
   */
  /* InitializeConditions for UnitDelay: '<S7>/y' */
  c_gmres_DWork.y_DSTATE[0] = c_gmres_P.y_InitialCondition[0];
  c_gmres_DWork.y_DSTATE[1] = c_gmres_P.y_InitialCondition[1];
  c_gmres_DWork.y_DSTATE[2] = c_gmres_P.y_InitialCondition[2];
  for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
       c_gmres_P.ForIterator_l_IterationLimit; c_gmres_fdgmres_Initialize_mode++)
  {
    c_gmres_fdgmres_Calculatey_ForIterator_l_1 = c_gmres_fdgmres_Initialize_mode;

    /* Constant: '<S7>/iterations' */
    c_gmres_fdgmres_Calculatey_iterations_1 = c_gmres_P.iterations_Value;

    /* Constant: '<S7>/one' */
    c_gmres_fdgmres_Calculatey_one_1 = c_gmres_P.one_Value;

    /* Sum: '<S7>/Add' */
    c_gmres_fdgmres_Calculatey_Add_1 = (c_gmres_fdgmres_Calculatey_iterations_1
      - (real_T)c_gmres_fdgmres_Calculatey_ForIterator_l_1) +
      c_gmres_fdgmres_Calculatey_one_1;

    /* UnitDelay: '<S7>/y' */
    c_gmres_fdgmres_Calculatey_y_1[0] = c_gmres_DWork.y_DSTATE[0];
    c_gmres_fdgmres_Calculatey_y_1[1] = c_gmres_DWork.y_DSTATE[1];
    c_gmres_fdgmres_Calculatey_y_1[2] = c_gmres_DWork.y_DSTATE[2];

    /* Selector: '<S7>/Selector' */
    c_gmres_fdgmres_Calculatey_Selector_1 =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Assignment_b_hat_1
      [(int32_T)c_gmres_fdgmres_Calculatey_Add_1 - 1];

    /* Outputs for Iterator SubSystem: '<S7>/ForIteratorSubsystem' incorporates:
     *  ForIterator: '<S11>/ForIterator'
     */
    if (c_gmres_DWork.ForIterator_IterationMarker_jw != 0) {
      /* InitializeConditions for UnitDelay: '<S11>/UnitDelay' */
      c_gmres_DWork.UnitDelay_DSTATE_o = c_gmres_P.UnitDelay_InitialCondition_j;
    }

    c_gmres_DWork.ForIterator_IterationMarker_jw = 1U;
    for (c_gmres_fdgmres_Iterativvelymin = 1; c_gmres_fdgmres_Iterativvelymin <=
         c_gmres_P.ForIterator_IterationLimit_m; c_gmres_fdgmres_Iterativvelymin
         ++) {
      c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_ForIterator_1 =
        c_gmres_fdgmres_Iterativvelymin;

      /* Selector: '<S11>/Selector1' */
      c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_Selector1_1 =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_matrix_1
        [((c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_ForIterator_1 - 1) *
          3 + (int32_T)c_gmres_fdgmres_Calculatey_Add_1) - 1];

      /* Selector: '<S11>/Selector' */
      c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_Selector_1 =
        c_gmres_fdgmres_Calculatey_y_1[c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_ForIterator_1
        - 1];

      /* Product: '<S11>/Product' */
      c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_Product_1 =
        c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_Selector1_1 *
        c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_Selector_1;

      /* UnitDelay: '<S11>/UnitDelay' */
      c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_UnitDelay_1 =
        c_gmres_DWork.UnitDelay_DSTATE_o;

      /* Sum: '<S11>/Sum' */
      c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_Sum_1 =
        c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_Product_1 +
        c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_UnitDelay_1;

      /* Update for UnitDelay: '<S11>/UnitDelay' */
      c_gmres_DWork.UnitDelay_DSTATE_o =
        c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_Sum_1;
    }

    /* End of Outputs for SubSystem: '<S7>/ForIteratorSubsystem' */

    /* Constant: '<S7>/zero' */
    c_gmres_fdgmres_Calculatey_zero_1 = c_gmres_P.zero_Value;

    /* Switch: '<S7>/Switch' */
    if (c_gmres_fdgmres_Calculatey_ForIterator_l_1 > c_gmres_P.Switch_Threshold)
    {
      c_gmres_fdgmres_Calculatey_Switch_1 =
        c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_Sum_1;
    } else {
      c_gmres_fdgmres_Calculatey_Switch_1 = c_gmres_fdgmres_Calculatey_zero_1;
    }

    /* End of Switch: '<S7>/Switch' */

    /* Sum: '<S7>/Subtract' */
    c_gmres_fdgmres_Calculatey_Subtract_1 =
      c_gmres_fdgmres_Calculatey_Selector_1 -
      c_gmres_fdgmres_Calculatey_Switch_1;

    /* Selector: '<S7>/Selector1' */
    c_gmres_fdgmres_Calculatey_Selector1_1 =
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_matrix_1
      [(((int32_T)c_gmres_fdgmres_Calculatey_Add_1 - 1) * 3 + (int32_T)
        c_gmres_fdgmres_Calculatey_Add_1) - 1];

    /* Product: '<S7>/Divide' */
    c_gmres_fdgmres_Calculatey_Divide_1 = c_gmres_fdgmres_Calculatey_Subtract_1 /
      c_gmres_fdgmres_Calculatey_Selector1_1;

    /* Assignment: '<S7>/Assignment_y' */
    if (c_gmres_fdgmres_Initialize_mode == 1) {
      c_gmres_fdgmres_Calculatey_Assignment_y_1[0] =
        c_gmres_fdgmres_Calculatey_y_1[0];
      c_gmres_fdgmres_Calculatey_Assignment_y_1[1] =
        c_gmres_fdgmres_Calculatey_y_1[1];
      c_gmres_fdgmres_Calculatey_Assignment_y_1[2] =
        c_gmres_fdgmres_Calculatey_y_1[2];
    }

    c_gmres_fdgmres_Calculatey_Assignment_y_1[(int32_T)
      c_gmres_fdgmres_Calculatey_Add_1 - 1] =
      c_gmres_fdgmres_Calculatey_Divide_1;

    /* End of Assignment: '<S7>/Assignment_y' */

    /* Update for UnitDelay: '<S7>/y' */
    c_gmres_DWork.y_DSTATE[0] = c_gmres_fdgmres_Calculatey_Assignment_y_1[0];
    c_gmres_DWork.y_DSTATE[1] = c_gmres_fdgmres_Calculatey_Assignment_y_1[1];
    c_gmres_DWork.y_DSTATE[2] = c_gmres_fdgmres_Calculatey_Assignment_y_1[2];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_CALCULATEY,0,0,C_GMRES_FDGMRES_CALCULATEY_PARAM);

  /* End of Outputs for SubSystem: '<S4>/Calculatey' */

MEASUREMENT_START(C_GMRES_FDGMRES_CALCUKATEX,0,0,C_GMRES_FDGMRES_CALCUKATEX_PARAM);
  /* Outputs for Iterator SubSystem: '<S4>/Calcukatex' incorporates:
   *  ForIterator: '<S6>/ForIterator'
   */
  /* InitializeConditions for UnitDelay: '<S6>/UnitDelay' */
  for (i = 0; i < 40; i++) {
    c_gmres_DWork.UnitDelay_DSTATE_ca[i] = c_gmres_P.UnitDelay_InitialCondition;
  }

  /* End of InitializeConditions for UnitDelay: '<S6>/UnitDelay' */
  for (c_gmres_fdgmres_Initialize_mode = 1; c_gmres_fdgmres_Initialize_mode <=
       c_gmres_P.ForIterator_IterationLimit; c_gmres_fdgmres_Initialize_mode++)
  {
    c_gmres_fdgmres_Calcukatex_ForIterator_1 = c_gmres_fdgmres_Initialize_mode;

    /* Selector: '<S6>/Selector' */
    i_0 = c_gmres_fdgmres_Calcukatex_ForIterator_1;

    /* Selector: '<S6>/Selector1' */
    c_gmres_fdgmres_Calcukatex_Selector1_1 =
      c_gmres_fdgmres_Calculatey_Assignment_y_1[c_gmres_fdgmres_Calcukatex_ForIterator_1
      - 1];
    for (i = 0; i < 40; i++) {
      /* Selector: '<S6>/Selector' */
      c_gmres_fdgmres_Calcukatex_Selector_1[i] =
        c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_q_matrix_1
        [(i_0 - 1) * 40 + i];

      /* Product: '<S6>/Product' */
      c_gmres_fdgmres_Calcukatex_Product_1[i] =
        c_gmres_fdgmres_Calcukatex_Selector_1[i] *
        c_gmres_fdgmres_Calcukatex_Selector1_1;

      /* UnitDelay: '<S6>/UnitDelay' */
      c_gmres_fdgmres_Calcukatex_UnitDelay_1[i] =
        c_gmres_DWork.UnitDelay_DSTATE_ca[i];

      /* Sum: '<S6>/Add' */
      c_gmres_fdgmres_Calcukatex_Add_1[i] =
        c_gmres_fdgmres_Calcukatex_Product_1[i] +
        c_gmres_fdgmres_Calcukatex_UnitDelay_1[i];

      /* Update for UnitDelay: '<S6>/UnitDelay' */
      c_gmres_DWork.UnitDelay_DSTATE_ca[i] = c_gmres_fdgmres_Calcukatex_Add_1[i];
    }
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_CALCUKATEX,0,0,C_GMRES_FDGMRES_CALCUKATEX_PARAM);

  /* End of Outputs for SubSystem: '<S4>/Calcukatex' */
MEASUREMENT_START(C_GMRES_FDGMRES_RESHAPE,0,0,C_GMRES_FDGMRES_RESHAPE_PARAM);
  for (i = 0; i < 40; i++) {
    /* Reshape: '<S4>/Reshape' */
    c_gmres_fdgmres_Reshape_1[i] = c_gmres_fdgmres_Calcukatex_Add_1[i];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_RESHAPE,0,0,C_GMRES_FDGMRES_RESHAPE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_ADD,0,0,C_GMRES_FDGMRES_ADD_PARAM);
  for (i = 0; i < 40; i++) {
    /* Sum: '<S4>/Add' */
    c_gmres_fdgmres_Add_1[i] = c_gmres_fdgmres_Reshape_1[i] +
      c_gmres_UnitDelay2_1[i];
  }
MEASUREMENT_STOP(C_GMRES_FDGMRES_ADD,0,0,C_GMRES_FDGMRES_ADD_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ADD,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ADD_PARAM);
  /* Sum: '<S12>/Add' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Add_1 =
    c_gmres_DigitalClock_1 + c_gmres_Constant1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ADD,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ADD_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ZERO2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ZERO2_PARAM);
  /* Constant: '<S12>/zero2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_zero2_1 =
    c_gmres_P.zero2_Value_f;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ZERO2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ZERO2_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_PRODUCT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_PRODUCT2_PARAM);
  /* Product: '<S14>/Product2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Product2_1 =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_zero2_1 *
    c_gmres_Constant1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_PRODUCT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_PRODUCT2_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_SUM2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_SUM2_PARAM);
  /* Sum: '<S14>/Sum2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Sum2_1 =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Add_1 +
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Product2_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_SUM2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_MODEL_SUM2_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ONE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ONE_PARAM);
  /* Constant: '<S12>/one' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_one_1 =
    c_gmres_P.one_Value_j;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ONE,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_ONE_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_PRODUCT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_PRODUCT2_PARAM);
  /* Product: '<S50>/Product2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Product2_1 =
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_one_1 *
    c_gmres_Constant1_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_PRODUCT2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_PRODUCT2_PARAM);

MEASUREMENT_START(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_SUM2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_SUM2_PARAM);
  /* Sum: '<S50>/Sum2' */
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Sum2_1 =
    c_gmres_DigitalClock_1 +
    c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Product2_1;
MEASUREMENT_STOP(C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_SUM2,0,0,C_GMRES_FDGMRES_INITIALIZE_MODEL_CALCULATERESIDUALVECTOR_B_MODEL1_SUM2_PARAM);

MEASUREMENT_START(C_GMRES_PLANT_UNITDELAY2,1,0,C_GMRES_PLANT_UNITDELAY2_PARAM);
  /* Update for UnitDelay: '<S5>/UnitDelay2' */
  c_gmres_DWork.UnitDelay2_DSTATE[0] = c_gmres_plant_Add_1[0];
  c_gmres_DWork.UnitDelay2_DSTATE[1] = c_gmres_plant_Add_1[1];
MEASUREMENT_STOP(C_GMRES_PLANT_UNITDELAY2,1,0,C_GMRES_PLANT_UNITDELAY2_PARAM);

MEASUREMENT_START(C_GMRES_UNITDELAY1,1,0,C_GMRES_UNITDELAY1_PARAM);
  /* Update for UnitDelay: '<Root>/UnitDelay1' */
  c_gmres_DWork.UnitDelay1_DSTATE[0] = c_gmres_plant_Add_1[0];
  c_gmres_DWork.UnitDelay1_DSTATE[1] = c_gmres_plant_Add_1[1];
MEASUREMENT_STOP(C_GMRES_UNITDELAY1,1,0,C_GMRES_UNITDELAY1_PARAM);
MEASUREMENT_START(C_GMRES_DISCRETE_TIMEINTEGRATOR,1,0,C_GMRES_DISCRETE_TIMEINTEGRATOR_PARAM);
  for (i_0 = 0; i_0 < 40; i_0++) {
    /* Update for DiscreteIntegrator: '<Root>/Discrete_TimeIntegrator' */
    c_gmres_DWork.Discrete_TimeIntegrator_DSTATE[i_0] +=
      c_gmres_P.Discrete_TimeIntegrator_gainval * c_gmres_fdgmres_Add_1[i_0];
  }
MEASUREMENT_STOP(C_GMRES_DISCRETE_TIMEINTEGRATOR,1,0,C_GMRES_DISCRETE_TIMEINTEGRATOR_PARAM);

MEASUREMENT_START(C_GMRES_SUBSYSTEM_UNITDELAY,1,0,C_GMRES_SUBSYSTEM_UNITDELAY_PARAM);
  for (i_0 = 0; i_0 < 40; i_0++) {
    /* Update for UnitDelay: '<S3>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE[i_0] = c_gmres_Subsystem_Selector1_1[i_0];
  }
MEASUREMENT_STOP(C_GMRES_SUBSYSTEM_UNITDELAY,1,0,C_GMRES_SUBSYSTEM_UNITDELAY_PARAM);

MEASUREMENT_START(C_GMRES_UNITDELAY2,1,0,C_GMRES_UNITDELAY2_PARAM);
  for (i_0 = 0; i_0 < 40; i_0++) {
    /* Update for UnitDelay: '<Root>/UnitDelay2' */
    c_gmres_DWork.UnitDelay2_DSTATE_d[i_0] = c_gmres_fdgmres_Add_1[i_0];
  }
MEASUREMENT_STOP(C_GMRES_UNITDELAY2,1,0,C_GMRES_UNITDELAY2_PARAM);

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The resolution of this integer timer is 0.001, which is the step size
   * of the task. Size of "clockTick0" ensures timer will not overflow during the
   * application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  c_gmres_M->Timing.clockTick0++;
  if (!c_gmres_M->Timing.clockTick0) {
    c_gmres_M->Timing.clockTickH0++;
  }
MEASUREMENT_STEP_END();
}

/* Model initialize function */
void c_gmres_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  c_gmres_P.Saturation_UpperSat = rtInf;
  c_gmres_P.Saturation_UpperSat_j = rtInf;
  c_gmres_P.Saturation_UpperSat_a = rtInf;
  c_gmres_P.Saturation_UpperSat_b = rtInf;
  c_gmres_P.Saturation_UpperSat_o = rtInf;
  c_gmres_P.Saturation_UpperSat_bb = rtInf;
  c_gmres_P.Saturation_UpperSat_jw = rtInf;
  c_gmres_P.Saturation_UpperSat_i = rtInf;

  /* initialize real-time model */
  (void) memset((void *)c_gmres_M, 0,
                sizeof(RT_MODEL_c_gmres));

  /* block I/O */

  /* exported global signals */
  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_Discrete_TimeIntegrator_1[i] = 0.0;
    }
  }

  c_gmres_Selector_1[0] = 0.0;
  c_gmres_Selector_1[1] = 0.0;
  c_gmres_plant_Model1_Gain3_1[0] = 0.0;
  c_gmres_plant_Model1_Gain3_1[1] = 0.0;
  c_gmres_plant_UnitDelay2_1[0] = 0.0;
  c_gmres_plant_UnitDelay2_1[1] = 0.0;
  c_gmres_plant_Model1_Saturation_1[0] = 0.0;
  c_gmres_plant_Model1_Saturation_1[1] = 0.0;
  c_gmres_plant_Model1_Sqrt_1[0] = 0.0;
  c_gmres_plant_Model1_Sqrt_1[1] = 0.0;
  c_gmres_plant_Model1_Gain_1 = 0.0;
  c_gmres_plant_Model1_Gain1_1 = 0.0;
  c_gmres_plant_Model1_Gain2_1 = 0.0;
  c_gmres_plant_Model1_Add_1 = 0.0;
  c_gmres_plant_Model1_Add1_1[0] = 0.0;
  c_gmres_plant_Model1_Add1_1[1] = 0.0;
  c_gmres_plant_Gain_1[0] = 0.0;
  c_gmres_plant_Gain_1[1] = 0.0;
  c_gmres_plant_Add_1[0] = 0.0;
  c_gmres_plant_Add_1[1] = 0.0;
  c_gmres_Constant2_1 = 0.0;
  c_gmres_SliderGain_SliderGain_1 = 0.0;
  c_gmres_SliderGain1_SliderGain_1 = 0.0;
  c_gmres_Constant1_1 = 0.0;
  c_gmres_DigitalClock_1 = 0.0;
  c_gmres_UnitDelay1_1[0] = 0.0;
  c_gmres_UnitDelay1_1[1] = 0.0;
  c_gmres_Subtract_1[0] = 0.0;
  c_gmres_Subtract_1[1] = 0.0;
  c_gmres_Gain_1[0] = 0.0;
  c_gmres_Gain_1[1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_Subsystem_UnitDelay_1[i] = 0.0;
    }
  }

  c_gmres_VectorConcatenate_1[0] = 0.0;
  c_gmres_VectorConcatenate_1[1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 42; i++) {
      c_gmres_Subsystem_VectorConcatenate1_1[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_Subsystem_Selector1_1[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_UnitDelay2_1[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 160; i++) {
      c_gmres_fdgmres_Initialize_model_q_matrix_1[i] = 0.0;
    }
  }

  c_gmres_fdgmres_calculation_tau_Constant4_1 = 0.0;
  c_gmres_fdgmres_calculation_tau_Gain1_1 = 0.0;
  c_gmres_fdgmres_calculation_tau_Constant3_1 = 0.0;
  c_gmres_fdgmres_calculation_tau_Subtract1_1 = 0.0;
  c_gmres_fdgmres_calculation_tau_MathFunction_1 = 0.0;
  c_gmres_fdgmres_calculation_tau_Subtract2_1 = 0.0;
  c_gmres_fdgmres_calculation_tau_Gain2_1 = 0.0;

  {
    int32_T i;
    for (i = 0; i < 42; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_VectorConcatenate_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Selector_1[0]
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Selector_1[1]
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Selector1_1[0]
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Selector1_1[1]
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Subtract_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Subtract_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Constant_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Constant1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Product1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Constant2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Product2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Constant3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Product3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_Add1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculatelambda_N_VectorConcatenate_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_VectorConcatenate1_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Constant_1 = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Divide1_1[i] =
        0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 42; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_VectorConcatenate_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Subtract_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Subtract_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Constant_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Constant1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Product1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Constant2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Product2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Constant3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Product3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_Add1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculatelambda_N_VectorConcatenate_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_VectorConcatenate1_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_zero_1[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Product_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Sum_1[i]
        = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Product1_1[0]
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Product1_1[1]
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Sum1_1[0] =
    0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Sum1_1[1] =
    0.0;

  {
    int32_T i;
    for (i = 0; i < 42; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_VectorConcatenate_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Subtract_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Subtract_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Constant_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Constant1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Product1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Constant2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Product2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Constant3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Product3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_Add1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculatelambda_N_VectorConcatenate_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_VectorConcatenate1_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Subtract_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Divide_1[
        i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Subtract_1[i] =
        0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subsystem_Product_1[0]
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subsystem_Product_1[1]
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subsystem_Add_1[0] =
    0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subsystem_Add_1[1] =
    0.0;

  {
    int32_T i;
    for (i = 0; i < 42; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_VectorConcatenate_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Subtract_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Subtract_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Constant_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Constant1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Product1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Constant2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Product2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Constant3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Product3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_Add1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculatelambda_N_VectorConcatenate_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_VectorConcatenate1_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Product_1[i]
        = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Sum_1[i] =
        0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_zero1_1[0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_zero1_1[1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Product1_1[0] =
    0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Product1_1[1] =
    0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Sum1_1[0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Sum1_1[1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 42; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_VectorConcatenate_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Subtract_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Subtract_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Constant_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Constant1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Product1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Constant2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Product2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Constant3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Product3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_Add1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculatelambda_N_VectorConcatenate_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_VectorConcatenate1_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Subtract_1[i]
        = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Divide_1[i]
        = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Subtract_1[i] =
        0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Reshape_1[i] =
        0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_two_norm_DotProduct_1 = 0.0;
  c_gmres_fdgmres_Initialize_model_two_norm_Sqrt_1 = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Divide_1[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 160; i++) {
      c_gmres_fdgmres_Initialize_model_Assignment_1[i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_b_hat_init_1[0] = 0.0;
  c_gmres_fdgmres_Initialize_model_b_hat_init_1[1] = 0.0;
  c_gmres_fdgmres_Initialize_model_b_hat_init_1[2] = 0.0;
  c_gmres_fdgmres_Initialize_model_b_hat_init_1[3] = 0.0;
  c_gmres_fdgmres_Initialize_model_Product_1[0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Product_1[1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Product_1[2] = 0.0;
  c_gmres_fdgmres_Initialize_model_Product_1[3] = 0.0;
  c_gmres_fdgmres_Initialize_model_Reshape1_1[0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Reshape1_1[1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Reshape1_1[2] = 0.0;
  c_gmres_fdgmres_Initialize_model_Reshape1_1[3] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Reshape_1[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Add_1[i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Add_1 = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_zero2_1 = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Product2_1 =
    0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Sum2_1 = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_one_1 = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Product2_1 =
    0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Sum2_1 = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_one_1 = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Add_1 = 0.0;

  {
    int32_T i;
    for (i = 0; i < 12; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_h_matrix_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Subsystem_Product_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Subsystem_Product_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Subsystem_Add_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Subsystem_Add_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 42; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_VectorConcatenate_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Subtract_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Subtract_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Constant_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Product_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Constant1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Product1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Add_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Constant2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Product2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Constant3_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Product3_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_Add1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculatelambda_N_VectorConcatenate_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_VectorConcatenate1_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 160; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_q_matrix_1[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 160; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Switch_1[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Selector_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Reshape1_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Product_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Sum_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_zero1_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_zero1_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Product1_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Product1_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Sum1_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Sum1_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 42; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_VectorConcatenate_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Subtract_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Subtract_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Constant_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Product_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Constant1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Product1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Add_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Constant2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Product2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Constant3_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Product3_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_Add1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculatelambda_N_VectorConcatenate_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_VectorConcatenate1_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Subtract_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Divide_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Reshape_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_two_norm_DotProduct_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_two_norm_Sqrt_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_Width1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_Reshape1_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_Reshape1_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_Reshape1_1
    [2] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_Reshape1_1
    [3] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 12; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_matrix_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Divide_1[
        i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 160; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_q_matrix_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Add_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_zero2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Product2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Sum2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector3_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector3_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector3_1
    [2] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector_1
    [2] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Selector_1
    [3] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_s_vector_1[0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_s_vector_1[1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_s_vector_1[2] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_c_vector_1[0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_c_vector_1[1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_c_vector_1[2] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_one_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Subtract_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Switch_Switch_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Switch_Switch_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Switch_Switch_1
    [2] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Selector1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Square_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Selector2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Square1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Add_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Sqrt_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Divide3_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Divide2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Add1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Divide_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment1_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment1_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment1_1
    [2] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Divide1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment2_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment2_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_Calculateoutput_Assignment2_1
    [2] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_b_hat_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_b_hat_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_b_hat_1
    [2] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_b_hat_1
    [3] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Switch1_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Switch1_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Switch1_1
    [2] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Switch1_1
    [3] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Selector_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Product1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Abs_rho =
    0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Product_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Gain_1 =
    0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_VectorConcatenate_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Assignment_b_hat_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Assignment_b_hat_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Assignment_b_hat_1
    [2] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updateb_hat_vector_Assignment_b_hat_1
    [3] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 9; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_r_matrix_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_Width1_1
    = 0.0;

  {
    int32_T i;
    for (i = 0; i < 9; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_matrix_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Selector_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Switch_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Assignment_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Assignment_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_Assignment_1
    [2] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector1_c_j_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_b_hat_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_b_hat_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_b_hat_1
    [2] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Switch1_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Switch1_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Switch1_1
    [2] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector2_r_j_1n
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Product_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector_s_j_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_one_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Add2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Selector3_h_jn
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Product1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Add_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Product2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Product3_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Add1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_VectorConcatenate_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Assignment_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Assignment_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_Assignment_1
    [2] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Gain_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Reshape_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Reshape_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_r1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_Product_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_A1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_Divide_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u1_Add_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_r2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_Product_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_A2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_Divide_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_H_u2_Add_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 38; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Gain_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1
    = 0.0;

  {
    int32_T i;
    for (i = 0; i < 38; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Reshape1_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Reshape1_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Gain_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Reshape_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Reshape_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_r1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_Product_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_A1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_Divide_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u1_Add_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_r2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_Product_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_A2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_Divide_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_H_u2_Add_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_VectorConcatenate_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 38; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1
    = 0.0;

  {
    int32_T i;
    for (i = 0; i < 38; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_h_vector_initial_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_h_vector_initial_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_h_vector_initial_1
    [2] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_h_vector_initial_1
    [3] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_v_vector_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Switch_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Selector_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_DotProduct_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Assignment1_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Assignment1_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Assignment1_1
    [2] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Assignment1_1
    [3] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Product_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_Subtract_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Selector_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Switch_1
    = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Assignment_1
    [0] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Assignment_1
    [1] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Assignment_1
    [2] = 0.0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_Assignment_1
    [3] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Reshape_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Reshape_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_r1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_A1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_Divide_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u1_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_r2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_A2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_Divide_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_H_u2_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 38; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1
    = 0.0;

  {
    int32_T i;
    for (i = 0; i < 38; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Reshape1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Reshape1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Gain2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Reshape_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Reshape_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_r1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_A1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_Divide_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u1_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_r2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_A2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_Divide_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_H_u2_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_VectorConcatenate_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 38; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1
    = 0.0;

  {
    int32_T i;
    for (i = 0; i < 38; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Reshape1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Reshape1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Gain2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Reshape_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Reshape_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_r1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_A1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_Divide_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u1_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_r2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_A2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_Divide_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_H_u2_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_VectorConcatenate_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 38; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1
    = 0.0;

  {
    int32_T i;
    for (i = 0; i < 38; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Switch_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Reshape1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Gain2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_state_eq_Add1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Product_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Updatex_Add_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Reshape_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Reshape_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_r1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_A1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_Divide_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u1_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_r2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_A2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_Divide_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_H_u2_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_VectorConcatenate_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 38; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1
    = 0.0;

  {
    int32_T i;
    for (i = 0; i < 38; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain3_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_UnitDelay_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Switch_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Reshape1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Reshape1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Saturation_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Sqrt_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Gain2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_state_eq_Add1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Product_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Updatex_Add_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Reshape_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Reshape_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_r1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_A1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_Divide_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u1_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_r2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_A2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_Divide_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_H_u2_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_VectorConcatenate_1
    [1] = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 38; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q11_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Selector1_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Subtract_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Q12_Q21_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_UnitDelay_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Switch_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_C1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_A2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Product3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_Sqrt_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Divide2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x1_Add2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q22_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Q12_Q21_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product1_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_C2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_A2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Product3_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_H_x2_Add_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_H_x_VectorConcatenate_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Gain_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Product_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Updatelambda_Add_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
    [0] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Reshape_1
    [1] = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Constant2_1
    = 0.0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Subtract1_1
    = 0.0;

  {
    int32_T i;
    for (i = 0; i < 38; i++) {
      c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_Assignment_1
        [i] = 0.0;
    }
  }

  c_gmres_fdgmres_Calculatey_iterations_1 = 0.0;
  c_gmres_fdgmres_Calculatey_one_1 = 0.0;
  c_gmres_fdgmres_Calculatey_Add_1 = 0.0;
  c_gmres_fdgmres_Calculatey_y_1[0] = 0.0;
  c_gmres_fdgmres_Calculatey_y_1[1] = 0.0;
  c_gmres_fdgmres_Calculatey_y_1[2] = 0.0;
  c_gmres_fdgmres_Calculatey_Selector_1 = 0.0;
  c_gmres_fdgmres_Calculatey_zero_1 = 0.0;
  c_gmres_fdgmres_Calculatey_Switch_1 = 0.0;
  c_gmres_fdgmres_Calculatey_Subtract_1 = 0.0;
  c_gmres_fdgmres_Calculatey_Selector1_1 = 0.0;
  c_gmres_fdgmres_Calculatey_Divide_1 = 0.0;
  c_gmres_fdgmres_Calculatey_Assignment_y_1[0] = 0.0;
  c_gmres_fdgmres_Calculatey_Assignment_y_1[1] = 0.0;
  c_gmres_fdgmres_Calculatey_Assignment_y_1[2] = 0.0;
  c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_Selector1_1 = 0.0;
  c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_Selector_1 = 0.0;
  c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_Product_1 = 0.0;
  c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_UnitDelay_1 = 0.0;
  c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_Sum_1 = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Calcukatex_Selector_1[i] = 0.0;
    }
  }

  c_gmres_fdgmres_Calcukatex_Selector1_1 = 0.0;

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Calcukatex_Product_1[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Calcukatex_UnitDelay_1[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 40; i++) {
      c_gmres_fdgmres_Calcukatex_Add_1[i] = 0.0;
    }
  }

  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_ForIterator_1 = 0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_QRdecompositionusingGivensrotation_VariantSubsystem_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_predict_x_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_F_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_predict_x_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_F_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Multiply_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_predict_x_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_F_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model1_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_predict_x_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_F_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model1_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_predict_x_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_F_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_b_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_predict_x_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_F_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model1_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_predict_x_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_F_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Initialize_model_Calculateresidualvector_Model_Model_Calculate_lambda_i_ForIteratorSubsystem_ForIterator_1
    = 0;
  c_gmres_fdgmres_Calculatey_ForIterator_l_1 = 0;
  c_gmres_fdgmres_Calculatey_ForIteratorSubsystem_ForIterator_1 = 0;
  c_gmres_fdgmres_Calcukatex_ForIterator_1 = 0;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_ForIteratorSubsystem_RelationalOperator_1
    = false;
  c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_ForIteratorSubsystem_RelationalOperator_1
    = false;

  /* states (dwork) */
  (void) memset((void *)&c_gmres_DWork, 0,
                sizeof(D_Work_c_gmres));

  {
    int32_T i;

    /* InitializeConditions for Iterator SubSystem: '<S54>/ForIteratorSubsystem' */
    /* InitializeConditions for UnitDelay: '<S64>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_c[0] =
      c_gmres_P.UnitDelay_InitialCondition_ff;
    c_gmres_DWork.UnitDelay_DSTATE_c[1] =
      c_gmres_P.UnitDelay_InitialCondition_ff;

    /* End of InitializeConditions for SubSystem: '<S54>/ForIteratorSubsystem' */

    /* InitializeConditions for Iterator SubSystem: '<S51>/ForIteratorSubsystem' */
    /* InitializeConditions for UnitDelay: '<S57>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_gm[0] =
      c_gmres_P.UnitDelay_InitialCondition_o;
    c_gmres_DWork.UnitDelay_DSTATE_gm[1] =
      c_gmres_P.UnitDelay_InitialCondition_o;

    /* End of InitializeConditions for SubSystem: '<S51>/ForIteratorSubsystem' */

    /* InitializeConditions for Iterator SubSystem: '<S71>/ForIteratorSubsystem' */
    /* InitializeConditions for UnitDelay: '<S81>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_d5[0] =
      c_gmres_P.UnitDelay_InitialCondition_d1;
    c_gmres_DWork.UnitDelay_DSTATE_d5[1] =
      c_gmres_P.UnitDelay_InitialCondition_d1;

    /* End of InitializeConditions for SubSystem: '<S71>/ForIteratorSubsystem' */

    /* InitializeConditions for Iterator SubSystem: '<S68>/ForIteratorSubsystem' */
    /* InitializeConditions for UnitDelay: '<S74>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_b[0] = c_gmres_P.UnitDelay_InitialCondition_g;
    c_gmres_DWork.UnitDelay_DSTATE_b[1] = c_gmres_P.UnitDelay_InitialCondition_g;

    /* End of InitializeConditions for SubSystem: '<S68>/ForIteratorSubsystem' */

    /* InitializeConditions for Iterator SubSystem: '<S86>/ForIteratorSubsystem' */
    /* InitializeConditions for UnitDelay: '<S96>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_g[0] =
      c_gmres_P.UnitDelay_InitialCondition_ff3;
    c_gmres_DWork.UnitDelay_DSTATE_g[1] =
      c_gmres_P.UnitDelay_InitialCondition_ff3;

    /* End of InitializeConditions for SubSystem: '<S86>/ForIteratorSubsystem' */

    /* InitializeConditions for Iterator SubSystem: '<S83>/ForIteratorSubsystem' */
    /* InitializeConditions for UnitDelay: '<S89>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_h[0] = c_gmres_P.UnitDelay_InitialCondition_k;
    c_gmres_DWork.UnitDelay_DSTATE_h[1] = c_gmres_P.UnitDelay_InitialCondition_k;

    /* End of InitializeConditions for SubSystem: '<S83>/ForIteratorSubsystem' */

    /* InitializeConditions for Iterator SubSystem: '<S22>/ForIteratorSubsystem' */
    /* InitializeConditions for UnitDelay: '<S32>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_gz[0] =
      c_gmres_P.UnitDelay_InitialCondition_f;
    c_gmres_DWork.UnitDelay_DSTATE_gz[1] =
      c_gmres_P.UnitDelay_InitialCondition_f;

    /* End of InitializeConditions for SubSystem: '<S22>/ForIteratorSubsystem' */

    /* InitializeConditions for Iterator SubSystem: '<S19>/ForIteratorSubsystem' */
    /* InitializeConditions for UnitDelay: '<S25>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_cn[0] =
      c_gmres_P.UnitDelay_InitialCondition_a;
    c_gmres_DWork.UnitDelay_DSTATE_cn[1] =
      c_gmres_P.UnitDelay_InitialCondition_a;

    /* End of InitializeConditions for SubSystem: '<S19>/ForIteratorSubsystem' */

    /* InitializeConditions for Iterator SubSystem: '<S37>/ForIteratorSubsystem' */
    /* InitializeConditions for UnitDelay: '<S47>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_hu[0] =
      c_gmres_P.UnitDelay_InitialCondition_ab;
    c_gmres_DWork.UnitDelay_DSTATE_hu[1] =
      c_gmres_P.UnitDelay_InitialCondition_ab;

    /* End of InitializeConditions for SubSystem: '<S37>/ForIteratorSubsystem' */

    /* InitializeConditions for Iterator SubSystem: '<S34>/ForIteratorSubsystem' */
    /* InitializeConditions for UnitDelay: '<S40>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_al[0] =
      c_gmres_P.UnitDelay_InitialCondition_d;
    c_gmres_DWork.UnitDelay_DSTATE_al[1] =
      c_gmres_P.UnitDelay_InitialCondition_d;

    /* End of InitializeConditions for SubSystem: '<S34>/ForIteratorSubsystem' */

    /* Start for Iterator SubSystem: '<S4>/Iterativvelyminimizeresidualvector' */
    /* InitializeConditions for Iterator SubSystem: '<S114>/ForIteratorSubsystem' */
    /* InitializeConditions for UnitDelay: '<S124>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_n[0] = c_gmres_P.UnitDelay_InitialCondition_i;
    c_gmres_DWork.UnitDelay_DSTATE_n[1] = c_gmres_P.UnitDelay_InitialCondition_i;

    /* End of InitializeConditions for SubSystem: '<S114>/ForIteratorSubsystem' */

    /* InitializeConditions for Iterator SubSystem: '<S111>/ForIteratorSubsystem' */
    /* InitializeConditions for UnitDelay: '<S117>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_l[0] = c_gmres_P.UnitDelay_InitialCondition_h;
    c_gmres_DWork.UnitDelay_DSTATE_l[1] = c_gmres_P.UnitDelay_InitialCondition_h;

    /* End of InitializeConditions for SubSystem: '<S111>/ForIteratorSubsystem' */

    /* InitializeConditions for Iterator SubSystem: '<S129>/ForIteratorSubsystem' */
    /* InitializeConditions for UnitDelay: '<S139>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_d[0] =
      c_gmres_P.UnitDelay_InitialCondition_as;
    c_gmres_DWork.UnitDelay_DSTATE_d[1] =
      c_gmres_P.UnitDelay_InitialCondition_as;

    /* End of InitializeConditions for SubSystem: '<S129>/ForIteratorSubsystem' */

    /* InitializeConditions for Iterator SubSystem: '<S126>/ForIteratorSubsystem' */
    /* InitializeConditions for UnitDelay: '<S132>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_a[0] = c_gmres_P.UnitDelay_InitialCondition_m;
    c_gmres_DWork.UnitDelay_DSTATE_a[1] = c_gmres_P.UnitDelay_InitialCondition_m;

    /* End of InitializeConditions for SubSystem: '<S126>/ForIteratorSubsystem' */

    /* InitializeConditions for Iterator SubSystem: '<S98>/ForIteratorSubsystem' */
    /* InitializeConditions for UnitDelay: '<S103>/v_vector' */
    memcpy(&c_gmres_DWork.v_vector_DSTATE[0],
           &c_gmres_P.v_vector_InitialCondition[0], 40U * sizeof(real_T));

    /* End of InitializeConditions for SubSystem: '<S98>/ForIteratorSubsystem' */

    /* Start for Width: '<S102>/Width1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Arnoldiprocess_Assignment_h_vector_Width1_1
      = 4.0;

    /* InitializeConditions for Iterator SubSystem: '<S143>/ForIteratorSubsystem' */
    /* InitializeConditions for UnitDelay: '<S144>/b_hat' */
    c_gmres_DWork.b_hat_DSTATE_d[0] = c_gmres_P.b_hat_InitialCondition[0];
    c_gmres_DWork.b_hat_DSTATE_d[1] = c_gmres_P.b_hat_InitialCondition[1];
    c_gmres_DWork.b_hat_DSTATE_d[2] = c_gmres_P.b_hat_InitialCondition[2];

    /* End of InitializeConditions for SubSystem: '<S143>/ForIteratorSubsystem' */

    /* Start for Width: '<S146>/Width1' */
    c_gmres_fdgmres_Iterativvelyminimizeresidualvector_Updater_vector_Assignment_r_vector_Width1_1
      = 3.0;

    /* End of Start for SubSystem: '<S4>/Iterativvelyminimizeresidualvector' */

    /* InitializeConditions for Iterator SubSystem: '<S4>/Iterativvelyminimizeresidualvector' */
    /* InitializeConditions for UnitDelay: '<S98>/h_matrix' */
    memcpy(&c_gmres_DWork.h_matrix_DSTATE[0],
           &c_gmres_P.h_matrix_InitialCondition[0], 12U * sizeof(real_T));

    /* InitializeConditions for UnitDelay: '<S9>/q_matrix' */
    memcpy(&c_gmres_DWork.q_matrix_DSTATE[0],
           &c_gmres_P.q_matrix_InitialCondition[0], 160U * sizeof(real_T));

    /* InitializeConditions for UnitDelay: '<S9>/s_vector' */
    c_gmres_DWork.s_vector_DSTATE[0] = c_gmres_P.s_vector_InitialCondition[0];
    c_gmres_DWork.s_vector_DSTATE[1] = c_gmres_P.s_vector_InitialCondition[1];
    c_gmres_DWork.s_vector_DSTATE[2] = c_gmres_P.s_vector_InitialCondition[2];

    /* InitializeConditions for UnitDelay: '<S9>/c_vector' */
    c_gmres_DWork.c_vector_DSTATE[0] = c_gmres_P.c_vector_InitialCondition[0];
    c_gmres_DWork.c_vector_DSTATE[1] = c_gmres_P.c_vector_InitialCondition[1];
    c_gmres_DWork.c_vector_DSTATE[2] = c_gmres_P.c_vector_InitialCondition[2];

    /* InitializeConditions for UnitDelay: '<S100>/b_hat' */
    c_gmres_DWork.b_hat_DSTATE[0] = c_gmres_P.b_hat_InitialCondition_l[0];
    c_gmres_DWork.b_hat_DSTATE[1] = c_gmres_P.b_hat_InitialCondition_l[1];
    c_gmres_DWork.b_hat_DSTATE[2] = c_gmres_P.b_hat_InitialCondition_l[2];
    c_gmres_DWork.b_hat_DSTATE[3] = c_gmres_P.b_hat_InitialCondition_l[3];

    /* InitializeConditions for UnitDelay: '<S101>/r_matrix' */
    memcpy(&c_gmres_DWork.r_matrix_DSTATE[0],
           &c_gmres_P.r_matrix_InitialCondition[0], 9U * sizeof(real_T));

    /* End of InitializeConditions for SubSystem: '<S4>/Iterativvelyminimizeresidualvector' */

    /* Start for Iterator SubSystem: '<S4>/Calculatey' */
    /* InitializeConditions for Iterator SubSystem: '<S7>/ForIteratorSubsystem' */
    /* InitializeConditions for UnitDelay: '<S11>/UnitDelay' */
    c_gmres_DWork.UnitDelay_DSTATE_o = c_gmres_P.UnitDelay_InitialCondition_j;

    /* End of InitializeConditions for SubSystem: '<S7>/ForIteratorSubsystem' */
    /* End of Start for SubSystem: '<S4>/Calculatey' */

    /* InitializeConditions for Iterator SubSystem: '<S4>/Calculatey' */
    /* InitializeConditions for UnitDelay: '<S7>/y' */
    c_gmres_DWork.y_DSTATE[0] = c_gmres_P.y_InitialCondition[0];
    c_gmres_DWork.y_DSTATE[1] = c_gmres_P.y_InitialCondition[1];
    c_gmres_DWork.y_DSTATE[2] = c_gmres_P.y_InitialCondition[2];

    /* End of InitializeConditions for SubSystem: '<S4>/Calculatey' */

    /* InitializeConditions for Iterator SubSystem: '<S4>/Calcukatex' */
    /* InitializeConditions for UnitDelay: '<S6>/UnitDelay' */
    for (i = 0; i < 40; i++) {
      c_gmres_DWork.UnitDelay_DSTATE_ca[i] =
        c_gmres_P.UnitDelay_InitialCondition;
    }

    /* End of InitializeConditions for UnitDelay: '<S6>/UnitDelay' */
    /* End of InitializeConditions for SubSystem: '<S4>/Calcukatex' */

    /* InitializeConditions for UnitDelay: '<S5>/UnitDelay2' */
    c_gmres_DWork.UnitDelay2_DSTATE[0] = c_gmres_P.UnitDelay2_InitialCondition[0];
    c_gmres_DWork.UnitDelay2_DSTATE[1] = c_gmres_P.UnitDelay2_InitialCondition[1];

    /* InitializeConditions for UnitDelay: '<Root>/UnitDelay1' */
    c_gmres_DWork.UnitDelay1_DSTATE[0] = c_gmres_P.UnitDelay1_InitialCondition[0];
    c_gmres_DWork.UnitDelay1_DSTATE[1] = c_gmres_P.UnitDelay1_InitialCondition[1];
    for (i = 0; i < 40; i++) {
      /* InitializeConditions for DiscreteIntegrator: '<Root>/Discrete_TimeIntegrator' */
      c_gmres_DWork.Discrete_TimeIntegrator_DSTATE[i] =
        c_gmres_P.Discrete_TimeIntegrator_IC[i];

      /* InitializeConditions for UnitDelay: '<S3>/UnitDelay' */
      c_gmres_DWork.UnitDelay_DSTATE[i] =
        c_gmres_P.UnitDelay_InitialCondition_fz[i];

      /* InitializeConditions for UnitDelay: '<Root>/UnitDelay2' */
      c_gmres_DWork.UnitDelay2_DSTATE_d[i] =
        c_gmres_P.UnitDelay2_InitialCondition_o[i];
    }
  }
}

/* Model terminate function */
void c_gmres_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
