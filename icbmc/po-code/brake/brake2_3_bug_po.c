/* 
 * Copyright 2012 The MathWorks, Inc.
 *
 * File: rt_main.c
 *
 * Abstract:
 *
 *   A real-time main that runs generated Simulink Coder code under most 
 *   operating systems.  Based on the definition of MULTITASKING, a single-task 
 *   or multitask step function is employed.
 *
 *   This file is a useful starting point for creating a custom main when 
 *   targeting a custom floating point processor or integer micro-controller.
 *
 *   Alternatively for ERT targets, you can generate a sample ert_main.c file 
 *   with the generated code by selecting the "Generate an example main program"
 *   option.  In this case, ert_main.c is precisely customized to the
 *   model requirements.  
 *
 * Required Defines:
 *
 *   MODEL - Model name
 *   NUMST - Number of sample times
 *
 */

#include <stdio.h>
#include <assert.h>

#ifdef CBMC
#include "brake_acc_nodiv_ctrl.c"
#include "brake_acc_nodiv_ctrl_data.c"
#include "rtGetNaN.c"
#include "rtGetInf.c"
#include "rt_nonfinite.c"
#include "cbmc-math.c"
#else
#include "brake_acc_nodiv_ctrl.h"
#endif

// Lihao
//int test;

#define MODEL brake_acc_nodiv_ctrl
#define NUMST 4

/*==================*
 * Required defines *
 *==================*/

#ifndef MODEL
# error Must specify a model name.  Define MODEL=name.
#else
/* create generic macros that work with any model */
# define EXPAND_CONCAT(name1,name2) name1 ## name2
# define CONCAT(name1,name2) EXPAND_CONCAT(name1,name2)
# define MODEL_INITIALIZE CONCAT(MODEL,_initialize)
# define MODEL_STEP       CONCAT(MODEL,_step)
# define MODEL_TERMINATE  CONCAT(MODEL,_terminate)
# define RT_MDL           CONCAT(MODEL,_M)
#endif

#if defined(TID01EQ) && TID01EQ == 1
#define FIRST_TID 1
#else 
#define FIRST_TID 0
#endif

/*====================*
 * External functions *
 *====================*/

extern void MODEL_INITIALIZE(void);
extern void MODEL_TERMINATE(void);
extern void MODEL_STEP(int_T tid);  /* multirate step function */


/*===================*
 * Visible functions *
 *===================*/

/* Function: rtOneStep ========================================================
 *
 * Abstract:
 *   Perform one step of the model.  This function is modeled such that
 *   it could be called from an interrupt service routine (ISR) with minor
 *   modifications.
 *
 *   Note that error checking is only necessary when this routine is
 *   attached to an interrupt.
 *
 *   Also, you may wish to unroll any or all of for and while loops to
 *   improve the real-time performance of this function.
 */
static void rt_OneStep(void)
{
    int_T i;

    /*******************************************
     * Step the model for the base sample time *
     *******************************************/
    MODEL_STEP(0);

    /*********************************************************
     * Step the model for any other sample times (subrates) *
     *********************************************************/
    for (i = FIRST_TID+1; i < NUMST; i++) {

      /******************************************
       * Step the model for sample time "i" *
       ******************************************/
      MODEL_STEP(i);
    }

} /* end rtOneStep */

/* Function: rt_InitModel ====================================================
 * 
 * Abstract: 
 *   Initialized the model and the overrun flags
 *
 */
void rt_InitModel(void)
{
    MODEL_INITIALIZE();
}

/* Function: rt_TermModel ====================================================
 * 
 * Abstract:
 *   Terminates the model and prints the error status
 *
 */
int_T rt_TermModel(void)
{
    MODEL_TERMINATE();
    return(0);
}

// Lihao
#if 0
void printState() {
  printf("state = {\n");
  printf("  RT2_Buffer0 = %f\n",brake_acc_nodiv_ctrl_DW.RT2_Buffer0);
  printf("  RT4_Buffer0 = %f\n",brake_acc_nodiv_ctrl_DW.RT4_Buffer0);
  printf("  RT1_Buffer0 = %f\n",brake_acc_nodiv_ctrl_DW.RT1_Buffer0);
  printf("  RT3_Buffer0 = %f\n",brake_acc_nodiv_ctrl_DW.RT3_Buffer0);
  printf("}\n");
}

void printOutputs() {
  printf("outputs = {\n");
  printf("  Out_TQ_RRBrake = %f\n",brake_acc_nodiv_ctrl_Y.Out_TQ_RRBrake);
  printf("  Out_TQ_RLBrake = %f\n",brake_acc_nodiv_ctrl_Y.Out_TQ_RLBrake);
  printf("  Out_TQ_FRBrake = %f\n",brake_acc_nodiv_ctrl_Y.Out_TQ_FRBrake);
  printf("  Out_TQ_FLBrake = %f\n",brake_acc_nodiv_ctrl_Y.Out_TQ_FLBrake);
  printf("}\n");
}

void printInputs() {
  printf("inputs = {\n");
  printf("  In_BrakePedal = %f\n",brake_acc_nodiv_ctrl_U.In_BrakePedal);
  printf("  In_RRRotation = %f\n",brake_acc_nodiv_ctrl_U.In_RRRotation);
  printf("  In_RLRotation = %f\n",brake_acc_nodiv_ctrl_U.In_RLRotation);
  printf("  In_FRRotation = %f\n",brake_acc_nodiv_ctrl_U.In_FRRotation);
  printf("  In_FLRotation = %f\n",brake_acc_nodiv_ctrl_U.In_FLRotation);
  printf("}\n");
}
#endif

void havocInputs(ExtU_brake_acc_nodiv_ctrl_T *_inputs) {
  ExtU_brake_acc_nodiv_ctrl_T inputs; 

#ifdef CBMC
  __CPROVER_assume(0<=inputs.In_BrakePedal && inputs.In_BrakePedal<=100 &&
              -10000<=inputs.In_RRRotation && inputs.In_RRRotation <= 10000 &&
              -10000<=inputs.In_RLRotation && inputs.In_RLRotation <= 10000 &&
              -10000<=inputs.In_FRRotation && inputs.In_FRRotation <= 10000 &&
              -10000<=inputs.In_FLRotation && inputs.In_FLRotation <= 10000);
#endif

  *_inputs = inputs;
}

/* Split in separate tasks
   Lihao
*/

//Lihao
const int __CPROVER_thread_priorities[] = {5, 5, 5, 5, 22};
const char* __CPROVER_threads[] = {"c::task_RR_Wheel", "c::task_FR_Wheel",
  "c::task_FL_Wheel", "c::task_RL_Wheel", "c::task_compute"};

void task_RR_Wheel(void);
void task_RL_Wheel(void);
void task_FR_Wheel(void);
void task_FL_Wheel(void);

void task_compute(void)
{
  // Lihao
  __CPROVER_ASYNC_1:
  task_RR_Wheel();
  __CPROVER_ASYNC_1:
  task_FL_Wheel();
  __CPROVER_ASYNC_1:
  task_FR_Wheel();

//#if 0
  B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B_local;

  /* Start for RateTransition: '<Root>/RT' */
  brake_acc_nodiv_ctrl_B_local.RT_h = brake_acc_nodiv_ctrl_P.RT_X0;

  /* Start for RateTransition: '<Root>/RT4' */
  brake_acc_nodiv_ctrl_B_local.RT4 = brake_acc_nodiv_ctrl_P.RT4_X0;

  /* Start for RateTransition: '<Root>/RT1' */
  brake_acc_nodiv_ctrl_B_local.RT1 = brake_acc_nodiv_ctrl_P.RT1_X0;

  /* Start for RateTransition: '<Root>/RT2' */
  brake_acc_nodiv_ctrl_B_local.RT2 = brake_acc_nodiv_ctrl_P.RT2_X0;

  /* Start for RateTransition: '<Root>/RT3' */
  brake_acc_nodiv_ctrl_B_local.RT3 = brake_acc_nodiv_ctrl_P.RT3_X0;
//#endif

  //B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B_local = brake_acc_nodiv_ctrl_B;
  //test = 0;

  //  brake_acc_nodiv_ctrl_step2(); //the following code is from this function
  real_T vkmh;
  real_T Distribution_idx;
  real_T Distribution_idx_0;
  real_T Distribution_idx_1;

  /* S-Function (fcncallgen): '<Root>/20ms' incorporates:
   *  SubSystem: '<Root>/Global Brake Controller'
   */
  /* Gain: '<S6>/Distribution' */
  Distribution_idx = brake_acc_nodiv_ctrl_P.Distribution_Gain[1] *
    brake_acc_nodiv_ctrl_B_local.RT14;
  Distribution_idx_0 = brake_acc_nodiv_ctrl_P.Distribution_Gain[2] *
    brake_acc_nodiv_ctrl_B_local.RT14;
  Distribution_idx_1 = brake_acc_nodiv_ctrl_P.Distribution_Gain[3] *
    brake_acc_nodiv_ctrl_B_local.RT14;

  /* S-Function (fcncallgen): '<Root>/20ms1' incorporates:
   *  SubSystem: '<Root>/Veh_Speed_Estimator'
   */
  /* Gain: '<S25>/v (km//h)' incorporates:
   *  Gain: '<S25>/average_rpm'
   *  Gain: '<S25>/w (grad//s)'
   *  Sum: '<S25>/Add'
   */

  vkmh = (((RT9 + RT10) + RT11) + RT12) *
    brake_acc_nodiv_ctrl_P.average_rpm_Gain * brake_acc_nodiv_ctrl_P.wgrads_Gain
    * brake_acc_nodiv_ctrl_P.vkmh_Gain;

  // Lihao
  {
    real_T tmp = (((RT9 + RT10) + RT11) + RT12) *
    brake_acc_nodiv_ctrl_P.average_rpm_Gain * brake_acc_nodiv_ctrl_P.wgrads_Gain
    * brake_acc_nodiv_ctrl_P.vkmh_Gain;

    assert(fabs(vkmh-tmp)<=0.001);
    //assert(test == 0);
  }


  /* S-Function (fcncallgen): '<Root>/20ms' incorporates:
   *  SubSystem: '<Root>/Global Brake Controller'
   */
  /* Update for RateTransition: '<Root>/RT' incorporates:
   *  Gain: '<S6>/Distribution'
   */
  RT_Buffer0 = brake_acc_nodiv_ctrl_P.Distribution_Gain
    [0] * brake_acc_nodiv_ctrl_B_local.RT14;

  /* Update for RateTransition: '<Root>/RT4' */
  RT4_Buffer0 = vkmh;

  /* Update for RateTransition: '<Root>/RT1' */
  RT1_Buffer0 = Distribution_idx;

  /* Update for RateTransition: '<Root>/RT2' */
  RT2_Buffer0 = Distribution_idx_0;

  /* Update for RateTransition: '<Root>/RT3' */
  RT3_Buffer0 = Distribution_idx_1;
}

void task_RR_Wheel(void)
{  
  // Lihao
  //__CPROVER_ASYNC_1:
  //task_compute();
  //test = 1;
 
//#if 0
  B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B_local;

  /* Start for RateTransition: '<Root>/RT' */
  brake_acc_nodiv_ctrl_B_local.RT_h = brake_acc_nodiv_ctrl_P.RT_X0;

  /* Start for RateTransition: '<Root>/RT4' */
  brake_acc_nodiv_ctrl_B_local.RT4 = brake_acc_nodiv_ctrl_P.RT4_X0;

  /* Start for RateTransition: '<Root>/RT1' */
  brake_acc_nodiv_ctrl_B_local.RT1 = brake_acc_nodiv_ctrl_P.RT1_X0;

  /* Start for RateTransition: '<Root>/RT2' */
  brake_acc_nodiv_ctrl_B_local.RT2 = brake_acc_nodiv_ctrl_P.RT2_X0;

  /* Start for RateTransition: '<Root>/RT3' */
  brake_acc_nodiv_ctrl_B_local.RT3 = brake_acc_nodiv_ctrl_P.RT3_X0;
//#endif

  //B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B_local = brake_acc_nodiv_ctrl_B;

  //brake_acc_nodiv_ctrl_step1 // the following code is from this function

  /* local block i/o variables */
  real_T rtb_to_int;

  /* RateTransition: '<Root>/RT' */
  brake_acc_nodiv_ctrl_B_local.RT_h = RT_Buffer0;

  /* RateTransition: '<Root>/RT4' */
  brake_acc_nodiv_ctrl_B_local.RT4 = RT4_Buffer0;

  /* Rounding: '<S15>/to_int' incorporates:
   *  Inport: '<Root>/In_RRRotation'
   */
  rtb_to_int = rt_roundd_snf(brake_acc_nodiv_ctrl_U.In_RRRotation);

  /* S-Function (fcncallgen): '<Root>/10ms_1' incorporates:
   *  SubSystem: '<Root>/ABS_RR_Wheel'
   */
  brake_acc_nodiv_ct_ABS_RR_Wheel(brake_acc_nodiv_ctrl_B_local.RT4,
    brake_acc_nodiv_ctrl_B_local.RT_h, rtb_to_int,
    &brake_acc_nodiv_ctrl_B_local.ABS_RR_Wheel, //(P_ABS_RR_Wheel_brake_acc_nodi_T *)
    brake_acc_nodiv_ctrl_P.ABS_RR_Wheel);

  /* Outport: '<Root>/Out_TQ_RRBrake' */
  //brake_acc_nodiv_ctrl_Y.Out_TQ_RRBrake =
  //  brake_acc_nodiv_ctrl_B_local.ABS_RR_Wheel.Threshold_10kmh;

  /* RateTransition: '<Root>/RT1' */
  brake_acc_nodiv_ctrl_B_local.RT1 = RT1_Buffer0;

  /* End of RateTransition: '<Root>/RT1' */

  /* RateTransition: '<Root>/RT9' incorporates:
     *  Gain: '<S5>/Gain1'
     *  Rounding: '<S16>/round_to_int'
     *  Saturate: '<S7>/Pedal_map'
   */
  RT9 = rtb_to_int;

}

void task_FL_Wheel(void)
{  
  // Lihao
  //__CPROVER_ASYNC_1:
  //task_compute();

//#if 0
  B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B_local;

  /* Start for RateTransition: '<Root>/RT' */
  brake_acc_nodiv_ctrl_B_local.RT_h = brake_acc_nodiv_ctrl_P.RT_X0;

  /* Start for RateTransition: '<Root>/RT4' */
  brake_acc_nodiv_ctrl_B_local.RT4 = brake_acc_nodiv_ctrl_P.RT4_X0;

  /* Start for RateTransition: '<Root>/RT1' */
  brake_acc_nodiv_ctrl_B_local.RT1 = brake_acc_nodiv_ctrl_P.RT1_X0;

  /* Start for RateTransition: '<Root>/RT2' */
  brake_acc_nodiv_ctrl_B_local.RT2 = brake_acc_nodiv_ctrl_P.RT2_X0;

  /* Start for RateTransition: '<Root>/RT3' */
  brake_acc_nodiv_ctrl_B_local.RT3 = brake_acc_nodiv_ctrl_P.RT3_X0;
//#endif

  //B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B_local = brake_acc_nodiv_ctrl_B;

  //brake_acc_nodiv_ctrl_step1 // the following code is from this function

  real_T rtb_to_int1;
  real_T tmp;

  /* Rounding: '<S9>/to_int1' incorporates:
   *  Inport: '<Root>/In_FLRotation'
   */
  rtb_to_int1 = rt_roundd_snf(brake_acc_nodiv_ctrl_U.In_FLRotation);

  /* RateTransition: '<Root>/RT' */
  brake_acc_nodiv_ctrl_B_local.RT_h = RT_Buffer0;

  /* RateTransition: '<Root>/RT4' */
  brake_acc_nodiv_ctrl_B_local.RT4 = RT4_Buffer0;

  /* S-Function (fcncallgen): '<Root>/10ms_4' incorporates:
   *  SubSystem: '<Root>/ABS_FL_Wheel'
   */
  brake_acc_nodiv_ct_ABS_RR_Wheel(brake_acc_nodiv_ctrl_B_local.RT4,
    brake_acc_nodiv_ctrl_B_local.RT3, rtb_to_int1,
    &brake_acc_nodiv_ctrl_B_local.ABS_FL_Wheel, //(P_ABS_RR_Wheel_brake_acc_nodi_T *)
    brake_acc_nodiv_ctrl_P.ABS_FL_Wheel);

  /* Outport: '<Root>/Out_TQ_FLBrake' */
  //brake_acc_nodiv_ctrl_Y.Out_TQ_FLBrake =
  //  brake_acc_nodiv_ctrl_B_local.ABS_FL_Wheel.Threshold_10kmh;

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
  RT14 = brake_acc_nodiv_ctrl_P.Gain1_Gain *
    rt_roundd_snf(tmp);

  /* RateTransition: '<Root>/RT12' */
  RT12 = rtb_to_int1;
}

void task_FR_Wheel(void)
{  
  // Lihao
  //__CPROVER_ASYNC_1:
  //task_compute();
  
//#if 0 
  B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B_local;

  /* Start for RateTransition: '<Root>/RT' */
  brake_acc_nodiv_ctrl_B_local.RT_h = brake_acc_nodiv_ctrl_P.RT_X0;

  /* Start for RateTransition: '<Root>/RT4' */
  brake_acc_nodiv_ctrl_B_local.RT4 = brake_acc_nodiv_ctrl_P.RT4_X0;

  /* Start for RateTransition: '<Root>/RT1' */
  brake_acc_nodiv_ctrl_B_local.RT1 = brake_acc_nodiv_ctrl_P.RT1_X0;

  /* Start for RateTransition: '<Root>/RT2' */
  brake_acc_nodiv_ctrl_B_local.RT2 = brake_acc_nodiv_ctrl_P.RT2_X0;

  /* Start for RateTransition: '<Root>/RT3' */
  brake_acc_nodiv_ctrl_B_local.RT3 = brake_acc_nodiv_ctrl_P.RT3_X0;
//#endif

  //B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B_local = brake_acc_nodiv_ctrl_B;

  //brake_acc_nodiv_ctrl_step1 // the following code is from this function

  real_T rtb_to_int_k;

  /* RateTransition: '<Root>/RT4' */
  brake_acc_nodiv_ctrl_B_local.RT4 = RT4_Buffer0;

  /* Rounding: '<S11>/to_int' incorporates:
   *  Inport: '<Root>/In_FRRotation'
   */
  rtb_to_int_k = rt_roundd_snf(brake_acc_nodiv_ctrl_U.In_FRRotation);

  /* S-Function (fcncallgen): '<Root>/10ms_3' incorporates:
   *  SubSystem: '<Root>/ABS_FR_Wheel'
   */
  brake_acc_nodiv_ct_ABS_RR_Wheel(brake_acc_nodiv_ctrl_B_local.RT4,
    brake_acc_nodiv_ctrl_B_local.RT2, rtb_to_int_k,
    &brake_acc_nodiv_ctrl_B_local.ABS_FR_Wheel, //(P_ABS_RR_Wheel_brake_acc_nodi_T *)
    brake_acc_nodiv_ctrl_P.ABS_FR_Wheel);

  /* Outport: '<Root>/Out_TQ_FRBrake' */
  //brake_acc_nodiv_ctrl_Y.Out_TQ_FRBrake =
  //  brake_acc_nodiv_ctrl_B_local.ABS_FR_Wheel.Threshold_10kmh;

  /* RateTransition: '<Root>/RT3' */
  brake_acc_nodiv_ctrl_B_local.RT3 = RT3_Buffer0;

  /* End of RateTransition: '<Root>/RT3' */

  /* RateTransition: '<Root>/RT11' */
  RT11 = rtb_to_int_k;
}

void task_RL_Wheel(void)
{  
  // Lihao
  //__CPROVER_ASYNC_1:
  //task_compute();
 
//#if 0
  B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B_local;

  /* Start for RateTransition: '<Root>/RT' */
  brake_acc_nodiv_ctrl_B_local.RT_h = brake_acc_nodiv_ctrl_P.RT_X0;

  /* Start for RateTransition: '<Root>/RT4' */
  brake_acc_nodiv_ctrl_B_local.RT4 = brake_acc_nodiv_ctrl_P.RT4_X0;

  /* Start for RateTransition: '<Root>/RT1' */
  brake_acc_nodiv_ctrl_B_local.RT1 = brake_acc_nodiv_ctrl_P.RT1_X0;

  /* Start for RateTransition: '<Root>/RT2' */
  brake_acc_nodiv_ctrl_B_local.RT2 = brake_acc_nodiv_ctrl_P.RT2_X0;

  /* Start for RateTransition: '<Root>/RT3' */
  brake_acc_nodiv_ctrl_B_local.RT3 = brake_acc_nodiv_ctrl_P.RT3_X0;
//#endif

  //B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B_local = brake_acc_nodiv_ctrl_B;

  //brake_acc_nodiv_ctrl_step1 // the following code is from this function

  real_T rtb_to_int_g;

  /* RateTransition: '<Root>/RT4' */
  brake_acc_nodiv_ctrl_B_local.RT4 = RT4_Buffer0;

  /* S-Function (fcncallgen): '<Root>/10ms_2' incorporates:
   *  SubSystem: '<Root>/ABS_RL_Wheel'
   */
  brake_acc_nodiv_ct_ABS_RR_Wheel(brake_acc_nodiv_ctrl_B_local.RT4,
    brake_acc_nodiv_ctrl_B_local.RT1, rtb_to_int_g,
    &brake_acc_nodiv_ctrl_B_local.ABS_RL_Wheel, //(P_ABS_RR_Wheel_brake_acc_nodi_T *)
    brake_acc_nodiv_ctrl_P.ABS_RL_Wheel);

  /* Outport: '<Root>/Out_TQ_RLBrake' */
  //brake_acc_nodiv_ctrl_Y.Out_TQ_RLBrake =
  //  brake_acc_nodiv_ctrl_B_local.ABS_RL_Wheel.Threshold_10kmh;

  /* RateTransition: '<Root>/RT2' */
  brake_acc_nodiv_ctrl_B_local.RT2 = RT2_Buffer0;

  /* End of RateTransition: '<Root>/RT2' */

  /* RateTransition: '<Root>/RT10' */
  RT10 = rtb_to_int_g;
}


/* Function: main =============================================================
 *
 * Abstract:
 *   Execute model on a generic target such as a workstation.
 */
int_T main(int_T argc, const char *argv[])
{
  rt_InitModel();

#if 0
  while (1) 
  {
#endif

    havocInputs(&brake_acc_nodiv_ctrl_U);

    //Lihao
    //__CPROVER_ASYNC_1:
    //task_RR_Wheel();
    //__CPROVER_ASYNC_1:
    //task_FL_Wheel();
    //__CPROVER_ASYNC_1:
    //task_FR_Wheel();
    //__CPROVER_ASYNC_1:
    //task_RL_Wheel();
    __CPROVER_ASYNC_1:
    task_compute();


#if 0
    printInputs();

    rt_OneStep();

    printState();
    printOutputs();
  }
#endif

  const int_T result = rt_TermModel();
  return result;
}

/* [EOF] rt_main.c */
