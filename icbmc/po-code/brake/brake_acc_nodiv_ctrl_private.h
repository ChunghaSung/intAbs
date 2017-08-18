/*
 * brake_acc_nodiv_ctrl_private.h
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
#ifndef RTW_HEADER_brake_acc_nodiv_ctrl_private_h_
#define RTW_HEADER_brake_acc_nodiv_ctrl_private_h_
#include "rtwtypes.h"
#include "builtin_typeid_types.h"
#include "multiword_types.h"

/* Private macros used by the generated code to access rtModel */
#ifndef rtmSetTFinal
# define rtmSetTFinal(rtm, val)        ((rtm)->Timing.tFinal = (val))
#endif

#ifndef rtmGetTPtr
# define rtmGetTPtr(rtm)               (&(rtm)->Timing.taskTime0)
#endif

#ifndef __RTWTYPES_H__
#error This file requires rtwtypes.h to be included
#else
#ifdef TMWTYPES_PREVIOUSLY_INCLUDED
#error This file requires rtwtypes.h to be included before tmwtypes.h
#endif                                 /* TMWTYPES_PREVIOUSLY_INCLUDED */
#endif                                 /* __RTWTYPES_H__ */

//#if 0
extern real_T rt_roundd_snf(real_T u);
extern void brake_acc_nodiv_ct_ABS_RR_Wheel(real_T rtu_vVehicleVelocitykmh,
  real_T rtu_TRequestedTorqueNm, real_T rtu_wwheelRotationrpm,
  B_ABS_RR_Wheel_brake_acc_nodi_T *localB, P_ABS_RR_Wheel_brake_acc_nodi_T
  localP);
  //*localP); // Lihao
extern void brake_acc_nodiv_ctrl_step0(void);
extern void brake_acc_nodiv_ctrl_step1(void);
extern void brake_acc_nodiv_ctrl_step2(void);
//#endif

#endif                                 /* RTW_HEADER_brake_acc_nodiv_ctrl_private_h_ */
