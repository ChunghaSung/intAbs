/*
 * brake_acc_nodiv_ctrl.h
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
#ifndef RTW_HEADER_brake_acc_nodiv_ctrl_h_
#define RTW_HEADER_brake_acc_nodiv_ctrl_h_
#ifndef brake_acc_nodiv_ctrl_COMMON_INCLUDES_
# define brake_acc_nodiv_ctrl_COMMON_INCLUDES_
#include <math.h>
#include <float.h>
#include <stddef.h>
#include <string.h>
#include "rtwtypes.h"
#include "builtin_typeid_types.h"
#include "multiword_types.h"
//#include "rtw_continuous.h"
//#include "rtw_solver.h"
//#include "rt_logging.h"
#include "rt_nonfinite.h"
#endif                                 /* brake_acc_nodiv_ctrl_COMMON_INCLUDES_ */

#include "brake_acc_nodiv_ctrl_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetFinalTime
# define rtmGetFinalTime(rtm)          ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetRTWLogInfo
# define rtmGetRTWLogInfo(rtm)         ((rtm)->rtwLogInfo)
#endif

#ifndef rtmCounterLimit
# define rtmCounterLimit(rtm, idx)     ((rtm)->Timing.TaskCounters.cLimit[(idx)])
#endif

#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmStepTask
# define rtmStepTask(rtm, idx)         ((rtm)->Timing.TaskCounters.TID[(idx)] == 0)
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

#ifndef rtmGetT
# define rtmGetT(rtm)                  ((rtm)->Timing.taskTime0)
#endif

#ifndef rtmGetTFinal
# define rtmGetTFinal(rtm)             ((rtm)->Timing.tFinal)
#endif

#ifndef rtmTaskCounter
# define rtmTaskCounter(rtm, idx)      ((rtm)->Timing.TaskCounters.TID[(idx)])
#endif

/* Block signals for system '<Root>/ABS_RR_Wheel' */
typedef struct {
  real_T Threshold_10kmh;              /* '<S4>/Threshold_10km//h' */
} B_ABS_RR_Wheel_brake_acc_nodi_T;

/* Block states (auto storage) for system '<Root>/ABS_RR_Wheel' */
typedef struct {
} DW_ABS_RR_Wheel_brake_acc_nod_T;

/* Block signals (auto storage) */
typedef struct {
  real_T RT_h;                         /* '<Root>/RT' */
  real_T RT4;                          /* '<Root>/RT4' */
  real_T RT1;                          /* '<Root>/RT1' */
  real_T RT2;                          /* '<Root>/RT2' */
  real_T RT3;                          /* '<Root>/RT3' */
  real_T RT14;                         /* '<Root>/RT14' */
  real_T RT9;                          /* '<Root>/RT9' */
  real_T RT10;                         /* '<Root>/RT10' */
  real_T RT11;                         /* '<Root>/RT11' */
  real_T RT12;                         /* '<Root>/RT12' */
  real_T vkmh;                         /* '<S25>/v (km//h)' */
  real_T Distribution[4];              /* '<S6>/Distribution' */
  real_T Gain1;                        /* '<S5>/Gain1' */
  B_ABS_RR_Wheel_brake_acc_nodi_T ABS_FL_Wheel;/* '<Root>/ABS_FL_Wheel' */
  B_ABS_RR_Wheel_brake_acc_nodi_T ABS_FR_Wheel;/* '<Root>/ABS_FR_Wheel' */
  B_ABS_RR_Wheel_brake_acc_nodi_T ABS_RL_Wheel;/* '<Root>/ABS_RL_Wheel' */
  B_ABS_RR_Wheel_brake_acc_nodi_T ABS_RR_Wheel;/* '<Root>/ABS_RR_Wheel' */
} B_brake_acc_nodiv_ctrl_T;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real_T RT_Buffer0;                   /* '<Root>/RT' */
  real_T RT4_Buffer0;                  /* '<Root>/RT4' */
  real_T RT1_Buffer0;                  /* '<Root>/RT1' */
  real_T RT2_Buffer0;                  /* '<Root>/RT2' */
  real_T RT3_Buffer0;                  /* '<Root>/RT3' */
  DW_ABS_RR_Wheel_brake_acc_nod_T ABS_FL_Wheel;/* '<Root>/ABS_FL_Wheel' */
  DW_ABS_RR_Wheel_brake_acc_nod_T ABS_FR_Wheel;/* '<Root>/ABS_FR_Wheel' */
  DW_ABS_RR_Wheel_brake_acc_nod_T ABS_RL_Wheel;/* '<Root>/ABS_RL_Wheel' */
  DW_ABS_RR_Wheel_brake_acc_nod_T ABS_RR_Wheel;/* '<Root>/ABS_RR_Wheel' */
} DW_brake_acc_nodiv_ctrl_T;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real_T In_BrakePedal;                /* '<Root>/In_BrakePedal' */
  real_T In_RRRotation;                /* '<Root>/In_RRRotation' */
  real_T In_RLRotation;                /* '<Root>/In_RLRotation' */
  real_T In_FRRotation;                /* '<Root>/In_FRRotation' */
  real_T In_FLRotation;                /* '<Root>/In_FLRotation' */
} ExtU_brake_acc_nodiv_ctrl_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T Out_TQ_RRBrake;               /* '<Root>/Out_TQ_RRBrake' */
  real_T Out_TQ_RLBrake;               /* '<Root>/Out_TQ_RLBrake' */
  real_T Out_TQ_FRBrake;               /* '<Root>/Out_TQ_FRBrake' */
  real_T Out_TQ_FLBrake;               /* '<Root>/Out_TQ_FLBrake' */
} ExtY_brake_acc_nodiv_ctrl_T;

/* Parameters for system: '<Root>/ABS_RR_Wheel' */
struct P_ABS_RR_Wheel_brake_acc_nodi_T_ {
  real_T wheSpdms_Gain;                /* Expression: R*pi/30
                                        * Referenced by: '<S29>/wheSpd (m//s)'
                                        */
  real_T vms_Gain;                     /* Expression: 10/36
                                        * Referenced by: '<S29>/v (m//s)'
                                        */
  real_T positive_UpperSat;            /* Expression: inf
                                        * Referenced by: '<S29>/positive'
                                        */
  real_T positive_LowerSat;            /* Expression: 0
                                        * Referenced by: '<S29>/positive'
                                        */
  real_T times_10_Gain;                /* Expression: 10
                                        * Referenced by: '<S29>/times_10'
                                        */
  real_T slip_abs_on_times_10_Gain;    /* Expression: 10*slip_abs_on
                                        * Referenced by: '<S29>/slip_abs_on_times_10'
                                        */
  real_T Threshold_10kmh_Threshold;    /* Expression: 10
                                        * Referenced by: '<S4>/Threshold_10km//h'
                                        */
  uint8_T ReleaseBrake_Value;          /* Computed Parameter: ReleaseBrake_Value
                                        * Referenced by: '<S29>/ReleaseBrake'
                                        */
};

/* Parameters (auto storage) */
struct P_brake_acc_nodiv_ctrl_T_ {
  real_T Gain1_Gain;                   /* Expression: maxBrakeTorque/100
                                        * Referenced by: '<S5>/Gain1'
                                        */
  real_T Distribution_Gain[4];         /* Expression: distrib
                                        * Referenced by: '<S6>/Distribution'
                                        */
  real_T average_rpm_Gain;             /* Expression: 1/4
                                        * Referenced by: '<S25>/average_rpm'
                                        */
  real_T wgrads_Gain;                  /* Expression: pi/30
                                        * Referenced by: '<S25>/w (grad//s)'
                                        */
  real_T vkmh_Gain;                    /* Expression: 3.6*R
                                        * Referenced by: '<S25>/v (km//h)'
                                        */
  real_T RT_X0;                        /* Expression: 0
                                        * Referenced by: '<Root>/RT'
                                        */
  real_T RT4_X0;                       /* Expression: v0*3.6
                                        * Referenced by: '<Root>/RT4'
                                        */
  real_T RT1_X0;                       /* Expression: 0
                                        * Referenced by: '<Root>/RT1'
                                        */
  real_T RT2_X0;                       /* Expression: 0
                                        * Referenced by: '<Root>/RT2'
                                        */
  real_T RT3_X0;                       /* Expression: 0
                                        * Referenced by: '<Root>/RT3'
                                        */
  real_T Pedal_map_UpperSat;           /* Expression: 100
                                        * Referenced by: '<S7>/Pedal_map'
                                        */
  real_T Pedal_map_LowerSat;           /* Expression: 0
                                        * Referenced by: '<S7>/Pedal_map'
                                        */
  P_ABS_RR_Wheel_brake_acc_nodi_T ABS_FL_Wheel;/* '<Root>/ABS_FL_Wheel' */
  P_ABS_RR_Wheel_brake_acc_nodi_T ABS_FR_Wheel;/* '<Root>/ABS_FR_Wheel' */
  P_ABS_RR_Wheel_brake_acc_nodi_T ABS_RL_Wheel;/* '<Root>/ABS_RL_Wheel' */
  P_ABS_RR_Wheel_brake_acc_nodi_T ABS_RR_Wheel;/* '<Root>/ABS_RR_Wheel' */
};

/* Real-time Model Data Structure */
struct tag_RTM_brake_acc_nodiv_ctrl_T {
  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    time_T taskTime0;
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    struct {
      uint8_T TID[3];
      uint8_T cLimit[3];
    } TaskCounters;

    struct {
      uint8_T TID1_2;
    } RateInteraction;

    time_T tFinal;
    boolean_T stopRequestedFlag;
  } Timing;
};

// Lihao
/* Block parameters (auto storage) */
extern P_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_P;

/* Block signals (auto storage) */
//extern B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B;

/* Block states (auto storage) */
//extern DW_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_DW;

/* External inputs (root inport signals with auto storage) */
extern ExtU_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_U;

/* External outputs (root outports fed by signals with auto storage) */
//extern ExtY_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_Y;

/* Model entry point functions */
extern void brake_acc_nodiv_ctrl_initialize(void);
//extern void brake_acc_nodiv_ctrl_step(int_T tid);
extern void brake_acc_nodiv_ctrl_terminate(void);

/* Real-time Model object */
//extern RT_MODEL_brake_acc_nodiv_ctrl_T *const brake_acc_nodiv_ctrl_M;

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
 * '<Root>' : 'brake_acc_nodiv_ctrl'
 * '<S1>'   : 'brake_acc_nodiv_ctrl/ABS_FL_Wheel'
 * '<S2>'   : 'brake_acc_nodiv_ctrl/ABS_FR_Wheel'
 * '<S3>'   : 'brake_acc_nodiv_ctrl/ABS_RL_Wheel'
 * '<S4>'   : 'brake_acc_nodiv_ctrl/ABS_RR_Wheel'
 * '<S5>'   : 'brake_acc_nodiv_ctrl/Brake_Torq_Calculation'
 * '<S6>'   : 'brake_acc_nodiv_ctrl/Global Brake Controller'
 * '<S7>'   : 'brake_acc_nodiv_ctrl/HW_BrakePedal'
 * '<S8>'   : 'brake_acc_nodiv_ctrl/HW_FLBrake'
 * '<S9>'   : 'brake_acc_nodiv_ctrl/HW_FLEncoder'
 * '<S10>'  : 'brake_acc_nodiv_ctrl/HW_FRBrake'
 * '<S11>'  : 'brake_acc_nodiv_ctrl/HW_FREncoder'
 * '<S12>'  : 'brake_acc_nodiv_ctrl/HW_RLBrake'
 * '<S13>'  : 'brake_acc_nodiv_ctrl/HW_RLEncoder'
 * '<S14>'  : 'brake_acc_nodiv_ctrl/HW_RRBrake'
 * '<S15>'  : 'brake_acc_nodiv_ctrl/HW_RREncoder'
 * '<S16>'  : 'brake_acc_nodiv_ctrl/LDM_BrakePedal'
 * '<S17>'  : 'brake_acc_nodiv_ctrl/LDM_FLBrake'
 * '<S18>'  : 'brake_acc_nodiv_ctrl/LDM_FLSensor'
 * '<S19>'  : 'brake_acc_nodiv_ctrl/LDM_FRBrake'
 * '<S20>'  : 'brake_acc_nodiv_ctrl/LDM_FRSensor'
 * '<S21>'  : 'brake_acc_nodiv_ctrl/LDM_RLBrake'
 * '<S22>'  : 'brake_acc_nodiv_ctrl/LDM_RLSensor'
 * '<S23>'  : 'brake_acc_nodiv_ctrl/LDM_RRBrake'
 * '<S24>'  : 'brake_acc_nodiv_ctrl/LDM_RRSensor'
 * '<S25>'  : 'brake_acc_nodiv_ctrl/Veh_Speed_Estimator'
 * '<S26>'  : 'brake_acc_nodiv_ctrl/ABS_FL_Wheel/If v>=10 km//h'
 * '<S27>'  : 'brake_acc_nodiv_ctrl/ABS_FR_Wheel/If v>=10 km//h'
 * '<S28>'  : 'brake_acc_nodiv_ctrl/ABS_RL_Wheel/If v>=10 km//h'
 * '<S29>'  : 'brake_acc_nodiv_ctrl/ABS_RR_Wheel/If v>=10 km//h'
 */
#endif                                 /* RTW_HEADER_brake_acc_nodiv_ctrl_h_ */
