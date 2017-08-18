/*
 * brake_acc_nodiv_ctrl.c
 *
 * Code generation for model "brake_acc_nodiv_ctrl".
 *
 * Model version              : 1.775
 * Simulink Coder version : 8.5 (R2013b) 08-Aug-2013
 * C source code generated on : Tue Mar  4 08:34:46 2014
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */
#include "brake_acc_nodiv_ctrl.h"
#include "brake_acc_nodiv_ctrl_private.h"

/* Block signals (auto storage) */
//B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B;
// Lihao
real_T RT9;
real_T RT10;
real_T RT11;
real_T RT12;
real_T RT14;

/* Block states (auto storage) */
//DW_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_DW;
// Lihao
real_T RT_Buffer0;
real_T RT1_Buffer0;
real_T RT2_Buffer0;
real_T RT3_Buffer0;
real_T RT4_Buffer0;

/* External inputs (root inport signals with auto storage) */
ExtU_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_U;

/* External outputs (root outports fed by signals with auto storage) */
//ExtY_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_Y;

// Lihao
/* Real-time model */
//RT_MODEL_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_M_;
//RT_MODEL_brake_acc_nodiv_ctrl_T *const brake_acc_nodiv_ctrl_M =
//  &brake_acc_nodiv_ctrl_M_;

/*
 * Output and update for function-call system:
 *    '<Root>/ABS_RR_Wheel'
 *    '<Root>/ABS_RL_Wheel'
 *    '<Root>/ABS_FR_Wheel'
 *    '<Root>/ABS_FL_Wheel'

#ifdef CBMC
  __CPROVER_assume(-300<=rtu_vVehicleVelocitykmh && rtu_vVehicleVelocitykmh<=300 &&
             0<=rtu_TRequestedTorqueNm && rtu_TRequestedTorqueNm<= 10000 &&
          -10000<=rtu_wwheelRotationrpm && rtu_wwheelRotationrpm <= 10000 &&
          -10000<=localB->Threshold_10kmh && localB->Threshold_10kmh <= 10000);
#endif


 */

// Lihao
void brake_acc_nodiv_ct_ABS_RR_Wheel(real_T rtu_vVehicleVelocitykmh, real_T
  rtu_TRequestedTorqueNm, real_T rtu_wwheelRotationrpm,
  B_ABS_RR_Wheel_brake_acc_nodi_T *localB, P_ABS_RR_Wheel_brake_acc_nodi_T
  localP)
{

  /* local block i/o variables */
  boolean_T rtb_RelationalOperator;
  real_T rtb_vms;
  real_T u;

  /* Outputs for Atomic SubSystem: '<S4>/If v>=10 km//h' */
  /* Gain: '<S29>/v (m//s)' */
  rtb_vms = localP.vms_Gain * rtu_vVehicleVelocitykmh;

  /* Sum: '<S29>/Subtract' incorporates:
   *  Gain: '<S29>/wheSpd (m//s)'
   */
  u = rtb_vms - localP.wheSpdms_Gain * rtu_wwheelRotationrpm;

  /* Saturate: '<S29>/positive' */
  if (u >= localP.positive_UpperSat) {
    u = localP.positive_UpperSat;
  } else {
    if (u <= localP.positive_LowerSat) {
      u = localP.positive_LowerSat;
    }
  }

  /* RelationalOperator: '<S29>/Relational Operator' incorporates:
   *  Gain: '<S29>/slip_abs_on_times_10'
   *  Gain: '<S29>/times_10'
   *  Saturate: '<S29>/positive'
   */
  rtb_RelationalOperator = (localP.times_10_Gain * u >
    localP.slip_abs_on_times_10_Gain * rtb_vms);

  /* Switch: '<S4>/Threshold_10km//h' */
  if (rtu_vVehicleVelocitykmh >= localP.Threshold_10kmh_Threshold) {
    /* Outputs for Atomic SubSystem: '<S4>/If v>=10 km//h' */
    /* Switch: '<S29>/LockDetect' incorporates:
     *  Constant: '<S29>/ReleaseBrake'
     */
    if (rtb_RelationalOperator) {
      localB->Threshold_10kmh = localP.ReleaseBrake_Value;
    } else {
      localB->Threshold_10kmh = rtu_TRequestedTorqueNm;
    }

    /* End of Outputs for SubSystem: '<S4>/If v>=10 km//h' */
  } else {
    /* Outputs for Atomic SubSystem: '<S4>/If v>=10 km//h' */
    /* Switch: '<S29>/LockDetect' */
    localB->Threshold_10kmh = rtu_TRequestedTorqueNm;

    /* End of Outputs for SubSystem: '<S4>/If v>=10 km//h' */
  }

  /* End of Switch: '<S4>/Threshold_10km//h' */
}

#if 0
void brake_acc_nodiv_ct_ABS_RR_Wheel(real_T rtu_vVehicleVelocitykmh, real_T
  rtu_TRequestedTorqueNm, real_T rtu_wwheelRotationrpm,
  B_ABS_RR_Wheel_brake_acc_nodi_T *localB, P_ABS_RR_Wheel_brake_acc_nodi_T
  *localP)
{

  /* local block i/o variables */
  boolean_T rtb_RelationalOperator;
  real_T rtb_vms;
  real_T u;

  /* Outputs for Atomic SubSystem: '<S4>/If v>=10 km//h' */
  /* Gain: '<S29>/v (m//s)' */
  rtb_vms = localP->vms_Gain * rtu_vVehicleVelocitykmh;

  /* Sum: '<S29>/Subtract' incorporates:
   *  Gain: '<S29>/wheSpd (m//s)'
   */
  u = rtb_vms - localP->wheSpdms_Gain * rtu_wwheelRotationrpm;

  /* Saturate: '<S29>/positive' */
  if (u >= localP->positive_UpperSat) {
    u = localP->positive_UpperSat;
  } else {
    if (u <= localP->positive_LowerSat) {
      u = localP->positive_LowerSat;
    }
  }

  /* RelationalOperator: '<S29>/Relational Operator' incorporates:
   *  Gain: '<S29>/slip_abs_on_times_10'
   *  Gain: '<S29>/times_10'
   *  Saturate: '<S29>/positive'
   */
  rtb_RelationalOperator = (localP->times_10_Gain * u >
    localP->slip_abs_on_times_10_Gain * rtb_vms);

  /* Switch: '<S4>/Threshold_10km//h' */
  if (rtu_vVehicleVelocitykmh >= localP->Threshold_10kmh_Threshold) {
    /* Outputs for Atomic SubSystem: '<S4>/If v>=10 km//h' */
    /* Switch: '<S29>/LockDetect' incorporates:
     *  Constant: '<S29>/ReleaseBrake'
     */
    if (rtb_RelationalOperator) {
      localB->Threshold_10kmh = localP->ReleaseBrake_Value;
    } else {
      localB->Threshold_10kmh = rtu_TRequestedTorqueNm;
    }

    /* End of Outputs for SubSystem: '<S4>/If v>=10 km//h' */
  } else {
    /* Outputs for Atomic SubSystem: '<S4>/If v>=10 km//h' */
    /* Switch: '<S29>/LockDetect' */
    localB->Threshold_10kmh = rtu_TRequestedTorqueNm;

    /* End of Outputs for SubSystem: '<S4>/If v>=10 km//h' */
  }

  /* End of Switch: '<S4>/Threshold_10km//h' */
}
#endif

real_T rt_roundd_snf(real_T u)
{
  real_T y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

// Lihao
#if 0
/* Model step function for TID0 */
void brake_acc_nodiv_ctrl_step0(void)  /* Sample time: [0.005s, 0.0s] */
{
  /* Update absolute time */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  /*if (!(++brake_acc_nodiv_ctrl_M->Timing.clockTick0)) {
    ++brake_acc_nodiv_ctrl_M->Timing.clockTickH0;
  }

  brake_acc_nodiv_ctrl_M->Timing.taskTime0 =
    brake_acc_nodiv_ctrl_M->Timing.clockTick0 *
    brake_acc_nodiv_ctrl_M->Timing.stepSize0 +
    brake_acc_nodiv_ctrl_M->Timing.clockTickH0 *
    brake_acc_nodiv_ctrl_M->Timing.stepSize0 * 4294967296.0;*/
}

/* Model step function for TID1 */
void brake_acc_nodiv_ctrl_step1(void)  /* Sample time: [0.01s, 0.0s] */
{
  /* local block i/o variables */
  real_T rtb_to_int;
  real_T rtb_to_int_g;
  real_T rtb_to_int_k;
  real_T rtb_to_int1;
  real_T tmp;

  /* Update the flag to indicate when data transfers from
   *  Sample time: [0.01s, 0.0s] to Sample time: [0.02s, 0.0s]  */
  (brake_acc_nodiv_ctrl_M->Timing.RateInteraction.TID1_2)++;
  if ((brake_acc_nodiv_ctrl_M->Timing.RateInteraction.TID1_2) > 1) {
    brake_acc_nodiv_ctrl_M->Timing.RateInteraction.TID1_2 = 0;
  }

  /* RateTransition: '<Root>/RT' */
  if (brake_acc_nodiv_ctrl_M->Timing.RateInteraction.TID1_2 == 1) {
    brake_acc_nodiv_ctrl_B.RT_h = brake_acc_nodiv_ctrl_DW.RT_Buffer0;

    /* RateTransition: '<Root>/RT4' */
    brake_acc_nodiv_ctrl_B.RT4 = brake_acc_nodiv_ctrl_DW.RT4_Buffer0;
  }

  /* End of RateTransition: '<Root>/RT' */

  /* Rounding: '<S15>/to_int' incorporates:
   *  Inport: '<Root>/In_RRRotation'
   */
  rtb_to_int = rt_roundd_snf(brake_acc_nodiv_ctrl_U.In_RRRotation);

  /* S-Function (fcncallgen): '<Root>/10ms_1' incorporates:
   *  SubSystem: '<Root>/ABS_RR_Wheel'
   */
  brake_acc_nodiv_ct_ABS_RR_Wheel(brake_acc_nodiv_ctrl_B.RT4,
    brake_acc_nodiv_ctrl_B.RT_h, rtb_to_int,
    &brake_acc_nodiv_ctrl_B.ABS_RR_Wheel, (P_ABS_RR_Wheel_brake_acc_nodi_T *)
    &brake_acc_nodiv_ctrl_P.ABS_RR_Wheel);

  /* Outport: '<Root>/Out_TQ_RRBrake' */
  brake_acc_nodiv_ctrl_Y.Out_TQ_RRBrake =
    brake_acc_nodiv_ctrl_B.ABS_RR_Wheel.Threshold_10kmh;

  /* RateTransition: '<Root>/RT1' */
  if (brake_acc_nodiv_ctrl_M->Timing.RateInteraction.TID1_2 == 1) {
    brake_acc_nodiv_ctrl_B.RT1 = brake_acc_nodiv_ctrl_DW.RT1_Buffer0;
  }

  /* End of RateTransition: '<Root>/RT1' */

  /* Rounding: '<S13>/to_int' incorporates:
   *  Inport: '<Root>/In_RLRotation'
   */
  rtb_to_int_g = rt_roundd_snf(brake_acc_nodiv_ctrl_U.In_RLRotation);

  /* S-Function (fcncallgen): '<Root>/10ms_2' incorporates:
   *  SubSystem: '<Root>/ABS_RL_Wheel'
   */
  brake_acc_nodiv_ct_ABS_RR_Wheel(brake_acc_nodiv_ctrl_B.RT4,
    brake_acc_nodiv_ctrl_B.RT1, rtb_to_int_g,
    &brake_acc_nodiv_ctrl_B.ABS_RL_Wheel, (P_ABS_RR_Wheel_brake_acc_nodi_T *)
    &brake_acc_nodiv_ctrl_P.ABS_RL_Wheel);

  /* Outport: '<Root>/Out_TQ_RLBrake' */
  brake_acc_nodiv_ctrl_Y.Out_TQ_RLBrake =
    brake_acc_nodiv_ctrl_B.ABS_RL_Wheel.Threshold_10kmh;

  /* RateTransition: '<Root>/RT2' */
  if (brake_acc_nodiv_ctrl_M->Timing.RateInteraction.TID1_2 == 1) {
    brake_acc_nodiv_ctrl_B.RT2 = brake_acc_nodiv_ctrl_DW.RT2_Buffer0;
  }

  /* End of RateTransition: '<Root>/RT2' */

  /* Rounding: '<S11>/to_int' incorporates:
   *  Inport: '<Root>/In_FRRotation'
   */
  rtb_to_int_k = rt_roundd_snf(brake_acc_nodiv_ctrl_U.In_FRRotation);

  /* S-Function (fcncallgen): '<Root>/10ms_3' incorporates:
   *  SubSystem: '<Root>/ABS_FR_Wheel'
   */
  brake_acc_nodiv_ct_ABS_RR_Wheel(brake_acc_nodiv_ctrl_B.RT4,
    brake_acc_nodiv_ctrl_B.RT2, rtb_to_int_k,
    &brake_acc_nodiv_ctrl_B.ABS_FR_Wheel, (P_ABS_RR_Wheel_brake_acc_nodi_T *)
    &brake_acc_nodiv_ctrl_P.ABS_FR_Wheel);

  /* Outport: '<Root>/Out_TQ_FRBrake' */
  brake_acc_nodiv_ctrl_Y.Out_TQ_FRBrake =
    brake_acc_nodiv_ctrl_B.ABS_FR_Wheel.Threshold_10kmh;

  /* RateTransition: '<Root>/RT3' */
  if (brake_acc_nodiv_ctrl_M->Timing.RateInteraction.TID1_2 == 1) {
    brake_acc_nodiv_ctrl_B.RT3 = brake_acc_nodiv_ctrl_DW.RT3_Buffer0;
  }

  /* End of RateTransition: '<Root>/RT3' */

  /* Rounding: '<S9>/to_int1' incorporates:
   *  Inport: '<Root>/In_FLRotation'
   */
  rtb_to_int1 = rt_roundd_snf(brake_acc_nodiv_ctrl_U.In_FLRotation);

  /* S-Function (fcncallgen): '<Root>/10ms_4' incorporates:
   *  SubSystem: '<Root>/ABS_FL_Wheel'
   */
  brake_acc_nodiv_ct_ABS_RR_Wheel(brake_acc_nodiv_ctrl_B.RT4,
    brake_acc_nodiv_ctrl_B.RT3, rtb_to_int1,
    &brake_acc_nodiv_ctrl_B.ABS_FL_Wheel, (P_ABS_RR_Wheel_brake_acc_nodi_T *)
    &brake_acc_nodiv_ctrl_P.ABS_FL_Wheel);

  /* Outport: '<Root>/Out_TQ_FLBrake' */
  brake_acc_nodiv_ctrl_Y.Out_TQ_FLBrake =
    brake_acc_nodiv_ctrl_B.ABS_FL_Wheel.Threshold_10kmh;

  /* RateTransition: '<Root>/RT14' incorporates:
   *  Gain: '<S5>/Gain1'
   *  Rounding: '<S16>/round_to_int'
   *  Saturate: '<S7>/Pedal_map'
   */
  if (brake_acc_nodiv_ctrl_M->Timing.RateInteraction.TID1_2 == 1) {
    /* Saturate: '<S7>/Pedal_map' incorporates:
     *  Inport: '<Root>/In_BrakePedal'
     */
    if (brake_acc_nodiv_ctrl_U.In_BrakePedal >=
        brake_acc_nodiv_ctrl_P.Pedal_map_UpperSat) {
      tmp = brake_acc_nodiv_ctrl_P.Pedal_map_UpperSat;
    } else if (brake_acc_nodiv_ctrl_U.In_BrakePedal <=
               brake_acc_nodiv_ctrl_P.Pedal_map_LowerSat) {
      tmp = brake_acc_nodiv_ctrl_P.Pedal_map_LowerSat;
    } else {
      tmp = brake_acc_nodiv_ctrl_U.In_BrakePedal;
    }

    /* S-Function (fcncallgen): '<Root>/10ms' incorporates:
     *  SubSystem: '<Root>/Brake_Torq_Calculation'
     */
    brake_acc_nodiv_ctrl_B.RT14 = brake_acc_nodiv_ctrl_P.Gain1_Gain *
      rt_roundd_snf(tmp);

    /* RateTransition: '<Root>/RT9' incorporates:
     *  Gain: '<S5>/Gain1'
     *  Rounding: '<S16>/round_to_int'
     *  Saturate: '<S7>/Pedal_map'
     */
    brake_acc_nodiv_ctrl_B.RT9 = rtb_to_int;

    /* RateTransition: '<Root>/RT10' */
    brake_acc_nodiv_ctrl_B.RT10 = rtb_to_int_g;

    /* RateTransition: '<Root>/RT11' */
    brake_acc_nodiv_ctrl_B.RT11 = rtb_to_int_k;

    /* RateTransition: '<Root>/RT12' */
    brake_acc_nodiv_ctrl_B.RT12 = rtb_to_int1;
  }

  /* End of RateTransition: '<Root>/RT14' */
}

/* Model step function for TID2 */
void brake_acc_nodiv_ctrl_step2(void)  /* Sample time: [0.02s, 0.0s] */
{
  real_T vkmh;
  real_T Distribution_idx;
  real_T Distribution_idx_0;
  real_T Distribution_idx_1;

  /* S-Function (fcncallgen): '<Root>/20ms' incorporates:
   *  SubSystem: '<Root>/Global Brake Controller'
   */
  /* Gain: '<S6>/Distribution' */
  Distribution_idx = brake_acc_nodiv_ctrl_P.Distribution_Gain[1] *
    brake_acc_nodiv_ctrl_B.RT14;
  Distribution_idx_0 = brake_acc_nodiv_ctrl_P.Distribution_Gain[2] *
    brake_acc_nodiv_ctrl_B.RT14;
  Distribution_idx_1 = brake_acc_nodiv_ctrl_P.Distribution_Gain[3] *
    brake_acc_nodiv_ctrl_B.RT14;

  /* S-Function (fcncallgen): '<Root>/20ms1' incorporates:
   *  SubSystem: '<Root>/Veh_Speed_Estimator'
   */
  /* Gain: '<S25>/v (km//h)' incorporates:
   *  Gain: '<S25>/average_rpm'
   *  Gain: '<S25>/w (grad//s)'
   *  Sum: '<S25>/Add'
   */
  vkmh = (((brake_acc_nodiv_ctrl_B.RT9 + brake_acc_nodiv_ctrl_B.RT10) +
           brake_acc_nodiv_ctrl_B.RT11) + brake_acc_nodiv_ctrl_B.RT12) *
    brake_acc_nodiv_ctrl_P.average_rpm_Gain * brake_acc_nodiv_ctrl_P.wgrads_Gain
    * brake_acc_nodiv_ctrl_P.vkmh_Gain;

  /* S-Function (fcncallgen): '<Root>/20ms' incorporates:
   *  SubSystem: '<Root>/Global Brake Controller'
   */
  /* Update for RateTransition: '<Root>/RT' incorporates:
   *  Gain: '<S6>/Distribution'
   */
  brake_acc_nodiv_ctrl_DW.RT_Buffer0 = brake_acc_nodiv_ctrl_P.Distribution_Gain
    [0] * brake_acc_nodiv_ctrl_B.RT14;

  /* Update for RateTransition: '<Root>/RT4' */
  brake_acc_nodiv_ctrl_DW.RT4_Buffer0 = vkmh;

  /* Update for RateTransition: '<Root>/RT1' */
  brake_acc_nodiv_ctrl_DW.RT1_Buffer0 = Distribution_idx;

  /* Update for RateTransition: '<Root>/RT2' */
  brake_acc_nodiv_ctrl_DW.RT2_Buffer0 = Distribution_idx_0;

  /* Update for RateTransition: '<Root>/RT3' */
  brake_acc_nodiv_ctrl_DW.RT3_Buffer0 = Distribution_idx_1;
}

/* Model step wrapper function for compatibility with a static main program */
void brake_acc_nodiv_ctrl_step(int_T tid)
{
  switch (tid) {
   case 0 :
    brake_acc_nodiv_ctrl_step0();
    break;

   case 1 :
    brake_acc_nodiv_ctrl_step1();
    break;

   case 2 :
    brake_acc_nodiv_ctrl_step2();
    break;

   default :
    break;
  }
}
#endif

/* Model initialize function */
void brake_acc_nodiv_ctrl_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  // Lihao
  brake_acc_nodiv_ctrl_P.ABS_FL_Wheel.positive_UpperSat = rtInf;
  brake_acc_nodiv_ctrl_P.ABS_FR_Wheel.positive_UpperSat = rtInf;
  brake_acc_nodiv_ctrl_P.ABS_RL_Wheel.positive_UpperSat = rtInf;
  brake_acc_nodiv_ctrl_P.ABS_RR_Wheel.positive_UpperSat = rtInf;

/* // Peter
  (void) memset(((void *) &brake_acc_nodiv_ctrl_B), 0,
                sizeof(B_brake_acc_nodiv_ctrl_T));
  (void) memset((void *)&brake_acc_nodiv_ctrl_DW, 0,
                sizeof(DW_brake_acc_nodiv_ctrl_T));
  (void) memset((void *)&brake_acc_nodiv_ctrl_U, 0,
                sizeof(ExtU_brake_acc_nodiv_ctrl_T));
  (void) memset((void *)&brake_acc_nodiv_ctrl_Y, 0,
                sizeof(ExtY_brake_acc_nodiv_ctrl_T));
*/

#if 0
  /* Start for RateTransition: '<Root>/RT' */
  brake_acc_nodiv_ctrl_B.RT_h = brake_acc_nodiv_ctrl_P.RT_X0;

  /* Start for RateTransition: '<Root>/RT4' */
  brake_acc_nodiv_ctrl_B.RT4 = brake_acc_nodiv_ctrl_P.RT4_X0;

  /* Start for RateTransition: '<Root>/RT1' */
  brake_acc_nodiv_ctrl_B.RT1 = brake_acc_nodiv_ctrl_P.RT1_X0;

  /* Start for RateTransition: '<Root>/RT2' */
  brake_acc_nodiv_ctrl_B.RT2 = brake_acc_nodiv_ctrl_P.RT2_X0;

  /* Start for RateTransition: '<Root>/RT3' */
  brake_acc_nodiv_ctrl_B.RT3 = brake_acc_nodiv_ctrl_P.RT3_X0;

  /* InitializeConditions for RateTransition: '<Root>/RT' */
  brake_acc_nodiv_ctrl_DW.RT_Buffer0 = brake_acc_nodiv_ctrl_P.RT_X0;

  /* InitializeConditions for RateTransition: '<Root>/RT4' */
  brake_acc_nodiv_ctrl_DW.RT4_Buffer0 = brake_acc_nodiv_ctrl_P.RT4_X0;

  /* InitializeConditions for RateTransition: '<Root>/RT1' */
  brake_acc_nodiv_ctrl_DW.RT1_Buffer0 = brake_acc_nodiv_ctrl_P.RT1_X0;

  /* InitializeConditions for RateTransition: '<Root>/RT2' */
  brake_acc_nodiv_ctrl_DW.RT2_Buffer0 = brake_acc_nodiv_ctrl_P.RT2_X0;

  /* InitializeConditions for RateTransition: '<Root>/RT3' */
  brake_acc_nodiv_ctrl_DW.RT3_Buffer0 = brake_acc_nodiv_ctrl_P.RT3_X0;
#endif

  /* InitializeConditions for RateTransition: '<Root>/RT' */
  RT_Buffer0 = brake_acc_nodiv_ctrl_P.RT_X0;

  /* InitializeConditions for RateTransition: '<Root>/RT4' */
  RT4_Buffer0 = brake_acc_nodiv_ctrl_P.RT4_X0;

  /* InitializeConditions for RateTransition: '<Root>/RT1' */
  RT1_Buffer0 = brake_acc_nodiv_ctrl_P.RT1_X0;

  /* InitializeConditions for RateTransition: '<Root>/RT2' */
  RT2_Buffer0 = brake_acc_nodiv_ctrl_P.RT2_X0;

  /* InitializeConditions for RateTransition: '<Root>/RT3' */
  RT3_Buffer0 = brake_acc_nodiv_ctrl_P.RT3_X0;

}

/* Model terminate function */
void brake_acc_nodiv_ctrl_terminate(void)
{
  /* (no terminate code required) */
}
