/*
 * builtin_typeid_types.h
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

#ifndef __BUILTIN_TYPEID_TYPES_H__
#define __BUILTIN_TYPEID_TYPES_H__
#include "rtwtypes.h"
#ifndef __BUILTIN_TYPEID_TYPES__
#define __BUILTIN_TYPEID_TYPES__

/* Enumeration of built-in data types */
typedef enum {
  SS_DOUBLE = 0,                       /* real_T    */
  SS_SINGLE = 1,                       /* real32_T  */
  SS_INT8 = 2,                         /* int8_T    */
  SS_UINT8 = 3,                        /* uint8_T   */
  SS_INT16 = 4,                        /* int16_T   */
  SS_UINT16 = 5,                       /* uint16_T  */
  SS_INT32 = 6,                        /* int32_T   */
  SS_UINT32 = 7,                       /* uint32_T  */
  SS_BOOLEAN = 8                       /* boolean_T */
} BuiltInDTypeId;

#define SS_NUM_BUILT_IN_DTYPE          ((int_T)SS_BOOLEAN+1)

/* Enumeration for MAT-file logging code */
typedef int_T DTypeId;

#endif                                 /* __BUILTIN_TYPEID_TYPES__ */
#endif                                 /* __BUILTIN_TYPEID_TYPES_H__ */
