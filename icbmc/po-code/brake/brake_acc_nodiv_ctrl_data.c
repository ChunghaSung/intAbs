/*
 * brake_acc_nodiv_ctrl_data.c
 *
 * Code generation for model "brake_acc_nodiv_ctrl".
 *
 * Model version              : 1.774
 * Simulink Coder version : 8.5 (R2013b) 08-Aug-2013
 * C source code generated on : Mon Feb 24 16:38:19 2014
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

//#include "brake_acc_nodiv_ctrl.h"
//#include "brake_acc_nodiv_ctrl_private.h"

/* Block parameters (auto storage) */
// Lihao
P_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_P = {
  30.0,                                /* Expression: maxBrakeTorque/100
                                        * Referenced by: '<S5>/Gain1'
                                        */

  /*  Expression: distrib
   * Referenced by: '<S6>/Distribution'
   */
  { 0.31, 0.29, 0.21, 0.19 },
  0.25,                                /* Expression: 1/4
                                        * Referenced by: '<S25>/average_rpm'
                                        */
  0.10471975511965977,                 /* Expression: pi/30
                                        * Referenced by: '<S25>/w (grad//s)'
                                        */
  1.8,                                 /* Expression: 3.6*R
                                        * Referenced by: '<S25>/v (km//h)'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<Root>/RT'
                                        */
  0.0,                                 /* Expression: v0*3.6
                                        * Referenced by: '<Root>/RT4'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<Root>/RT1'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<Root>/RT2'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<Root>/RT3'
                                        */
  100.0,                               /* Expression: 100
                                        * Referenced by: '<S7>/Pedal_map'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S7>/Pedal_map'
                                        */

  /* Start of '<Root>/ABS_FL_Wheel' */
  {
    0.052359877559829883,              /* Expression: R*pi/30
                                        * Referenced by: '<S26>/wheSpd (m//s)'
                                        */
    0.27777777777777779,               /* Expression: 10/36
                                        * Referenced by: '<S26>/v (m//s)'
                                        */
    0.0,                               /* Expression: inf
                                        * Referenced by: '<S26>/positive'
                                        */
    0.0,                               /* Expression: 0
                                        * Referenced by: '<S26>/positive'
                                        */
    10.0,                              /* Expression: 10
                                        * Referenced by: '<S26>/times_10'
                                        */
    1.0,                               /* Expression: 10*slip_abs_on
                                        * Referenced by: '<S26>/slip_abs_on_times_10'
                                        */
    10.0,                              /* Expression: 10
                                        * Referenced by: '<S1>/Threshold_10km//h'
                                        */
    0U                                 /* Computed Parameter: ReleaseBrake_Value
                                        * Referenced by: '<S26>/ReleaseBrake'
                                        */
  }
  /* End of '<Root>/ABS_FL_Wheel' */
  ,

  /* Start of '<Root>/ABS_FR_Wheel' */
  {
    0.052359877559829883,              /* Expression: R*pi/30
                                        * Referenced by: '<S27>/wheSpd (m//s)'
                                        */
    0.27777777777777779,               /* Expression: 10/36
                                        * Referenced by: '<S27>/v (m//s)'
                                        */
    0.0,                               /* Expression: inf
                                        * Referenced by: '<S27>/positive'
                                        */
    0.0,                               /* Expression: 0
                                        * Referenced by: '<S27>/positive'
                                        */
    10.0,                              /* Expression: 10
                                        * Referenced by: '<S27>/times_10'
                                        */
    1.0,                               /* Expression: 10*slip_abs_on
                                        * Referenced by: '<S27>/slip_abs_on_times_10'
                                        */
    10.0,                              /* Expression: 10
                                        * Referenced by: '<S2>/Threshold_10km//h'
                                        */
    0U                                 /* Computed Parameter: ReleaseBrake_Value
                                        * Referenced by: '<S27>/ReleaseBrake'
                                        */
  }
  /* End of '<Root>/ABS_FR_Wheel' */
  ,

  /* Start of '<Root>/ABS_RL_Wheel' */
  {
    0.052359877559829883,              /* Expression: R*pi/30
                                        * Referenced by: '<S28>/wheSpd (m//s)'
                                        */
    0.27777777777777779,               /* Expression: 10/36
                                        * Referenced by: '<S28>/v (m//s)'
                                        */
    0.0,                               /* Expression: inf
                                        * Referenced by: '<S28>/positive'
                                        */
    0.0,                               /* Expression: 0
                                        * Referenced by: '<S28>/positive'
                                        */
    10.0,                              /* Expression: 10
                                        * Referenced by: '<S28>/times_10'
                                        */
    1.0,                               /* Expression: 10*slip_abs_on
                                        * Referenced by: '<S28>/slip_abs_on_times_10'
                                        */
    10.0,                              /* Expression: 10
                                        * Referenced by: '<S3>/Threshold_10km//h'
                                        */
    0U                                 /* Computed Parameter: ReleaseBrake_Value
                                        * Referenced by: '<S28>/ReleaseBrake'
                                        */
  }
  /* End of '<Root>/ABS_RL_Wheel' */
  ,

  /* Start of '<Root>/ABS_RR_Wheel' */
  {
    0.052359877559829883,              /* Expression: R*pi/30
                                        * Referenced by: '<S29>/wheSpd (m//s)'
                                        */
    0.27777777777777779,               /* Expression: 10/36
                                        * Referenced by: '<S29>/v (m//s)'
                                        */
    0.0,                               /* Expression: inf
                                        * Referenced by: '<S29>/positive'
                                        */
    0.0,                               /* Expression: 0
                                        * Referenced by: '<S29>/positive'
                                        */
    10.0,                              /* Expression: 10
                                        * Referenced by: '<S29>/times_10'
                                        */
    1.0,                               /* Expression: 10*slip_abs_on
                                        * Referenced by: '<S29>/slip_abs_on_times_10'
                                        */
    10.0,                              /* Expression: 10
                                        * Referenced by: '<S4>/Threshold_10km//h'
                                        */
    0U                                 /* Computed Parameter: ReleaseBrake_Value
                                        * Referenced by: '<S29>/ReleaseBrake'
                                        */
  }
  /* End of '<Root>/ABS_RR_Wheel' */
};
