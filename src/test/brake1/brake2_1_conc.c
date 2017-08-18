// Author: Chungha Sung
// This is a modified version of Brake2_1_conc.c which was used in DATE15 paper.
// The original source code can be found in http://www.cprover.org/interrupts/
/* 
 * Copyright 2012 The MathWorks, Inc_
 *
 * File: rt_main_c
 *
 * Abstract:
 *
 *   A real-time main that runs generated Simulink Coder code under most 
 *   operating systems_  Based on the definition of MULTITASKING, a single-task 
 *   or multitask step function is employed_
 *
 *   This file is a useful starting point for creating a custom main when 
 *   targeting a custom floating point processor or integer micro-controller_
 *
 *   Alternatively for ERT targets, you can generate a sample ert_main_c file 
 *   with the generated code by selecting the "Generate an example main program"
 *   option_  In this case, ert_main_c is precisely customized to the
 *   model requirements_  
 *
 * Required Defines:
 *
 *   MODEL - Model name
 *   NUMST - Number of sample times
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <assert.h>

//#include "brake_acc_nodiv_ctrl_h"

// Lihao
//int test;

#define MODEL brake_acc_nodiv_ctrl
#define NUMST 4

/*==================*
 * Required defines *
 *==================*/

#ifndef MODEL
# error Must specify a model name_  Define MODEL=name_
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
/*
extern void MODEL_INITIALIZE(void);
extern void MODEL_TERMINATE(void);
extern void MODEL_STEP(int_T tid);  // multirate step function
*/

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
//static void rt_OneStep(void)
//{
//    int_T i;
//    /*******************************************
//     * Step the model for the base sample time *
//     *******************************************/
//    MODEL_STEP(0);
//    /*********************************************************
//     * Step the model for any other sample times (subrates) *
//     *********************************************************/
//    for (i = FIRST_TID+1; i < NUMST; i++) {
//        /******************************************
//         * Step the model for sample time "i" *
//         ******************************************/
//        MODEL_STEP(i);
//    }
//
//} /* end rtOneStep */

/* Function: rt_InitModel ====================================================
 * 
 * Abstract: 
 *   Initialized the model and the overrun flags
 *
 */
/*
void rt_InitModel(void)
{
    MODEL_INITIALIZE();
}
*/

/* Function: rt_TermModel ====================================================
 * 
 * Abstract:
 *   Terminates the model and prints the error status
 *
 */
/*
int_T rt_TermModel(void)
{
    MODEL_TERMINATE();
    return(0);
}
*/

/*
void havocInputs(ExtU_brake_acc_nodiv_ctrl_T *_inputs) {
    ExtU_brake_acc_nodiv_ctrl_T inputs; 
    *_inputs = inputs;
}
*/

/* Split in separate tasks
   Lihao
 */

//Lihao
const int __VERIFIER_thread_priorities[] = {5, 5, 5, 5, 22};
const char* __VERIFIER_threads[] = {"c::task_RR_Wheel", "c::task_FR_Wheel",
    "c::task_FL_Wheel", "c::task_RL_Wheel", "c::task_compute"};

//void irq_err(void) {ERROR: goto ERROR;}

int brake_acc_nodiv_ctrl_B_local_RT_h;
int brake_acc_nodiv_ctrl_B_local_RTH;
int brake_acc_nodiv_ctrl_B_local_RT4;
int brake_acc_nodiv_ctrl_B_local_RT1;
int brake_acc_nodiv_ctrl_B_local_RT2;
int brake_acc_nodiv_ctrl_B_local_RT3;
int brake_acc_nodiv_ctrl_B_local_RT14;

int brake_acc_nodiv_ctrl_B_local_ABS_FL_Wheel_Threshold_10kmh;
int brake_acc_nodiv_ctrl_B_local_ABS_RL_Wheel_Threshold_10kmh;
int brake_acc_nodiv_ctrl_B_local_ABS_RR_Wheel_Threshold_10kmh;
int brake_acc_nodiv_ctrl_B_local_ABS_FR_Wheel_Threshold_10kmh;

int brake_acc_nodiv_ctrl_P_RT_X0;
int brake_acc_nodiv_ctrl_P_RT4_X0;
int brake_acc_nodiv_ctrl_P_RT1_X0;
int brake_acc_nodiv_ctrl_P_RT2_X0;
int brake_acc_nodiv_ctrl_P_RT3_X0;

int brake_acc_nodiv_ctrl_P_Gain1_Gain;

int brake_acc_nodiv_ctrl_P_Distribution_Gain0;
int brake_acc_nodiv_ctrl_P_Distribution_Gain1;
int brake_acc_nodiv_ctrl_P_Distribution_Gain2;
int brake_acc_nodiv_ctrl_P_Distribution_Gain3;


int brake_acc_nodiv_ctrl_P_average_rpm_Gain;
int brake_acc_nodiv_ctrl_P_wgrads_Gain;
int brake_acc_nodiv_ctrl_P_vkmh_Gain;

int brake_acc_nodiv_ctrl_P_ABS_FL_Wheel_vms_Gain;
int brake_acc_nodiv_ctrl_P_ABS_FL_Wheel_wheSpdms_Gain;
int brake_acc_nodiv_ctrl_P_ABS_FL_Wheel_times_10_Gain;
int brake_acc_nodiv_ctrl_P_ABS_FL_Wheel_slip_abs_on_times_10_Gain;
int brake_acc_nodiv_ctrl_P_ABS_FL_Wheel_Threshold_10kmh_Threshold;
int brake_acc_nodiv_ctrl_P_ABS_FL_Wheel_ReleaseBrake_Value;
int brake_acc_nodiv_ctrl_P_ABS_FL_Wheel_positive_UpperSat;
int brake_acc_nodiv_ctrl_P_ABS_FL_Wheel_positive_LowerSat;

int brake_acc_nodiv_ctrl_P_ABS_RR_Wheel_vms_Gain;
int brake_acc_nodiv_ctrl_P_ABS_RR_Wheel_wheSpdms_Gain;
int brake_acc_nodiv_ctrl_P_ABS_RR_Wheel_times_10_Gain;
int brake_acc_nodiv_ctrl_P_ABS_RR_Wheel_slip_abs_on_times_10_Gain;
int brake_acc_nodiv_ctrl_P_ABS_RR_Wheel_Threshold_10kmh_Threshold;
int brake_acc_nodiv_ctrl_P_ABS_RR_Wheel_ReleaseBrake_Value;
int brake_acc_nodiv_ctrl_P_ABS_RR_Wheel_positive_UpperSat;
int brake_acc_nodiv_ctrl_P_ABS_RR_Wheel_positive_LowerSat;

int brake_acc_nodiv_ctrl_P_ABS_RL_Wheel_vms_Gain;
int brake_acc_nodiv_ctrl_P_ABS_RL_Wheel_wheSpdms_Gain;
int brake_acc_nodiv_ctrl_P_ABS_RL_Wheel_times_10_Gain;
int brake_acc_nodiv_ctrl_P_ABS_RL_Wheel_slip_abs_on_times_10_Gain;
int brake_acc_nodiv_ctrl_P_ABS_RL_Wheel_Threshold_10kmh_Threshold;
int brake_acc_nodiv_ctrl_P_ABS_RL_Wheel_ReleaseBrake_Value;
int brake_acc_nodiv_ctrl_P_ABS_RL_Wheel_positive_UpperSat;
int brake_acc_nodiv_ctrl_P_ABS_RL_Wheel_positive_LowerSat;

int brake_acc_nodiv_ctrl_P_ABS_FR_Wheel_vms_Gain;
int brake_acc_nodiv_ctrl_P_ABS_FR_Wheel_wheSpdms_Gain;
int brake_acc_nodiv_ctrl_P_ABS_FR_Wheel_times_10_Gain;
int brake_acc_nodiv_ctrl_P_ABS_FR_Wheel_slip_abs_on_times_10_Gain;
int brake_acc_nodiv_ctrl_P_ABS_FR_Wheel_Threshold_10kmh_Threshold;
int brake_acc_nodiv_ctrl_P_ABS_FR_Wheel_ReleaseBrake_Value;
int brake_acc_nodiv_ctrl_P_ABS_FR_Wheel_positive_UpperSat;
int brake_acc_nodiv_ctrl_P_ABS_FR_Wheel_positive_LowerSat;

int brake_acc_nodiv_ctrl_P_Pedal_map_UpperSat;
int brake_acc_nodiv_ctrl_P_Pedal_map_LowerSat;

int brake_acc_nodiv_ctrl_U_In_BrakePedal;
int brake_acc_nodiv_ctrl_U_In_FLRotation;
int brake_acc_nodiv_ctrl_U_In_RRRotation;
int brake_acc_nodiv_ctrl_U_In_FRRotation;

int vkmh;


int RT9;
int RT10;
int RT11;
int RT12;
int RT14;

int RT_Buffer0;
int RT0_Buffer0;
int RT1_Buffer0;
int RT2_Buffer0;
int RT3_Buffer0;
int RT4_Buffer0;

int Distribution_idx;
int Distribution_idx_0;
int Distribution_idx_1;


void *task_compute(void *unused) {

    //B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B_local;
    /* Start for RateTransition: '<Root>/RT' */
    //brake_acc_nodiv_ctrl_B_local_RT_h = brake_acc_nodiv_ctrl_P_RT_X0;
    brake_acc_nodiv_ctrl_B_local_RT_h = brake_acc_nodiv_ctrl_P_RT_X0;
    /* Start for RateTransition: '<Root>/RT4' */
    //brake_acc_nodiv_ctrl_B_local_RT4 = brake_acc_nodiv_ctrl_P_RT4_X0;
    brake_acc_nodiv_ctrl_B_local_RT4 = brake_acc_nodiv_ctrl_P_RT4_X0;
    /* Start for RateTransition: '<Root>/RT1' */
    //brake_acc_nodiv_ctrl_B_local_RT1 = brake_acc_nodiv_ctrl_P_RT1_X0;
    brake_acc_nodiv_ctrl_B_local_RT1 = brake_acc_nodiv_ctrl_P_RT1_X0;
    /* Start for RateTransition: '<Root>/RT2' */
    //brake_acc_nodiv_ctrl_B_local_RT2 = brake_acc_nodiv_ctrl_P_RT2_X0;
    brake_acc_nodiv_ctrl_B_local_RT2 = brake_acc_nodiv_ctrl_P_RT2_X0;
    /* Start for RateTransition: '<Root>/RT3' */
    //brake_acc_nodiv_ctrl_B_local_RT3 = brake_acc_nodiv_ctrl_P_RT3_X0;
    brake_acc_nodiv_ctrl_B_local_RT3 = brake_acc_nodiv_ctrl_P_RT3_X0;
    //B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B_local = brake_acc_nodiv_ctrl_B;

    //test = 0;
    //  brake_acc_nodiv_ctrl_step2(); //the following code is from this function


    /* S-Function (fcncallgen): '<Root>/20ms' incorporates:
     *  SubSystem: '<Root>/Global Brake Controller'
     */
    /* Gain: '<S6>/Distribution' */
    Distribution_idx = brake_acc_nodiv_ctrl_P_Distribution_Gain1 *
        brake_acc_nodiv_ctrl_B_local_RT14;
    Distribution_idx_0 = brake_acc_nodiv_ctrl_P_Distribution_Gain2 *
        brake_acc_nodiv_ctrl_B_local_RT14;
    Distribution_idx_1 = brake_acc_nodiv_ctrl_P_Distribution_Gain3 *
        brake_acc_nodiv_ctrl_B_local_RT14;

    /* S-Function (fcncallgen): '<Root>/20ms1' incorporates:
     *  SubSystem: '<Root>/Veh_Speed_Estimator'
     */
    /* Gain: '<S25>/v (km//h)' incorporates:
     *  Gain: '<S25>/average_rpm'
     *  Gain: '<S25>/w (grad//s)'
     *  Sum: '<S25>/Add'
     */

    vkmh = (((RT9 + RT10) + RT11) + RT12) *
        brake_acc_nodiv_ctrl_P_average_rpm_Gain * brake_acc_nodiv_ctrl_P_wgrads_Gain
        * brake_acc_nodiv_ctrl_P_vkmh_Gain;


    //if(!(fabs(vkmh-tmp)<=0_001)) irq_err();
    //__VERIFIER_assert(fabs(vkmh-tmp)<=0_001);
    //assert(fabs(vkmh-tmp)<=0_001);
    //assert(test == 0);

    /* S-Function (fcncallgen): '<Root>/20ms' incorporates:
     *  SubSystem: '<Root>/Global Brake Controller'
     */
    /* Update for RateTransition: '<Root>/RT' incorporates:
     *  Gain: '<S6>/Distribution'
     */
    RT_Buffer0 = brake_acc_nodiv_ctrl_P_Distribution_Gain0 * brake_acc_nodiv_ctrl_B_local_RT14;

    /* Update for RateTransition: '<Root>/RT4' */
    RT4_Buffer0 = vkmh;

    /* Update for RateTransition: '<Root>/RT1' */
    RT1_Buffer0 = Distribution_idx;

    /* Update for RateTransition: '<Root>/RT2' */
    RT2_Buffer0 = Distribution_idx_0;

    /* Update for RateTransition: '<Root>/RT3' */
    RT3_Buffer0 = Distribution_idx_1;


}

void *task_RR_Wheel(void *unused) {  

    /* Start for RateTransition: '<Root>/RT' */
    brake_acc_nodiv_ctrl_B_local_RT_h = brake_acc_nodiv_ctrl_P_RT_X0;
    /* Start for RateTransition: '<Root>/RT4' */
    brake_acc_nodiv_ctrl_B_local_RT4 = brake_acc_nodiv_ctrl_P_RT4_X0;
    /* Start for RateTransition: '<Root>/RT1' */
    brake_acc_nodiv_ctrl_B_local_RT1 = brake_acc_nodiv_ctrl_P_RT1_X0;
    /* Start for RateTransition: '<Root>/RT2' */
    brake_acc_nodiv_ctrl_B_local_RT2 = brake_acc_nodiv_ctrl_P_RT2_X0;
    /* Start for RateTransition: '<Root>/RT3' */
    brake_acc_nodiv_ctrl_B_local_RT3 = brake_acc_nodiv_ctrl_P_RT3_X0;

    //B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B_local = brake_acc_nodiv_ctrl_B;
    //brake_acc_nodiv_ctrl_step1 // the following code is from this function

    /* local block i/o variables */
    int rtb_to_int;
    /* RateTransition: '<Root>/RT' */
    brake_acc_nodiv_ctrl_B_local_RT_h = RT_Buffer0;
    /* RateTransition: '<Root>/RT4' */
    brake_acc_nodiv_ctrl_B_local_RT4 = RT4_Buffer0;
    //rtb_to_int = rt_roundd_snf(brake_acc_nodiv_ctrl_U_In_RRRotation);
	rtb_to_int = brake_acc_nodiv_ctrl_U_In_RRRotation;

    bool rtb_RelationalOperator;
    int rtb_vms;
    int u;
    rtb_vms = brake_acc_nodiv_ctrl_P_ABS_RR_Wheel_vms_Gain * brake_acc_nodiv_ctrl_B_local_RT4;

    u = rtb_vms - brake_acc_nodiv_ctrl_P_ABS_RR_Wheel_wheSpdms_Gain * rtb_to_int;

    int positive_UpperSat_local = brake_acc_nodiv_ctrl_P_ABS_RR_Wheel_positive_UpperSat;

    if (u >= positive_UpperSat_local) {
        u = brake_acc_nodiv_ctrl_P_ABS_RR_Wheel_positive_UpperSat;
    } else {
        int positive_LowerSat_local = brake_acc_nodiv_ctrl_P_ABS_RR_Wheel_positive_LowerSat;
        if (u <= positive_LowerSat_local) {
            u = brake_acc_nodiv_ctrl_P_ABS_RR_Wheel_positive_LowerSat;
        }
    }
    /* RelationalOperator: '<S29>/Relational Operator' incorporates:
     *  Gain: '<S29>/slip_abs_on_times_10'
     *  Gain: '<S29>/times_10'
     *  Saturate: '<S29>/positive'
     */
    rtb_RelationalOperator = (brake_acc_nodiv_ctrl_P_ABS_RR_Wheel_times_10_Gain * u >
            brake_acc_nodiv_ctrl_P_ABS_RR_Wheel_slip_abs_on_times_10_Gain * rtb_vms);
    /* Switch: '<S4>/Threshold_10km//h' */
    // Lihao: lift
    int Threshold_10kmh_Threshold_local = brake_acc_nodiv_ctrl_P_ABS_RR_Wheel_Threshold_10kmh_Threshold;
    if (brake_acc_nodiv_ctrl_B_local_RT4 >= Threshold_10kmh_Threshold_local) {
        /* Outputs for Atomic SubSystem: '<S4>/If v>=10 km//h' */
        /* Switch: '<S29>/LockDetect' incorporates:
         *  Constant: '<S29>/ReleaseBrake'
         */
        if (rtb_RelationalOperator) {
            brake_acc_nodiv_ctrl_B_local_ABS_RR_Wheel_Threshold_10kmh = brake_acc_nodiv_ctrl_P_ABS_RR_Wheel_ReleaseBrake_Value;
        } else {
            brake_acc_nodiv_ctrl_B_local_ABS_RR_Wheel_Threshold_10kmh = brake_acc_nodiv_ctrl_B_local_RT_h;
        }
        /* End of Outputs for SubSystem: '<S4>/If v>=10 km//h' */
    } else {
        /* Outputs for Atomic SubSystem: '<S4>/If v>=10 km//h' */
        /* Switch: '<S29>/LockDetect' */
        brake_acc_nodiv_ctrl_B_local_ABS_RR_Wheel_Threshold_10kmh = brake_acc_nodiv_ctrl_B_local_RT_h;
        /* End of Outputs for SubSystem: '<S4>/If v>=10 km//h' */
    }
    /* End of Switch: '<S4>/Threshold_10km//h' */
    // end of inline
    /* Outport: '<Root>/Out_TQ_RRBrake' */
    //brake_acc_nodiv_ctrl_Y_Out_TQ_RRBrake =
    //  brake_acc_nodiv_ctrl_B_local_ABS_RR_Wheel_Threshold_10kmh;
    /* RateTransition: '<Root>/RT1' */
    brake_acc_nodiv_ctrl_B_local_RT1 = RT1_Buffer0;
    /* End of RateTransition: '<Root>/RT1' */
    /* RateTransition: '<Root>/RT9' incorporates:
     *  Gain: '<S5>/Gain1'
     *  Rounding: '<S16>/round_to_int'
     *  Saturate: '<S7>/Pedal_map'
     */
    RT9 = rtb_to_int;
}

void *task_FL_Wheel(void *unused) {  
    //B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B_local;
    /* Start for RateTransition: '<Root>/RT' */
    brake_acc_nodiv_ctrl_B_local_RT_h = brake_acc_nodiv_ctrl_P_RT_X0;
    /* Start for RateTransition: '<Root>/RT4' */
    brake_acc_nodiv_ctrl_B_local_RT4 = brake_acc_nodiv_ctrl_P_RT4_X0;
    /* Start for RateTransition: '<Root>/RT1' */
    brake_acc_nodiv_ctrl_B_local_RT1 = brake_acc_nodiv_ctrl_P_RT1_X0;
    /* Start for RateTransition: '<Root>/RT2' */
    brake_acc_nodiv_ctrl_B_local_RT2 = brake_acc_nodiv_ctrl_P_RT2_X0;
    /* Start for RateTransition: '<Root>/RT3' */
    brake_acc_nodiv_ctrl_B_local_RT3 = brake_acc_nodiv_ctrl_P_RT3_X0;
    int rtb_to_int1;
    int tmp;
    /* Rounding: '<S9>/to_int1' incorporates:
     *  Inport: '<Root>/In_FLRotation'
     */
    //rtb_to_int1 = rt_roundd_snf(brake_acc_nodiv_ctrl_U_In_FLRotation);
    rtb_to_int1 = brake_acc_nodiv_ctrl_U_In_FLRotation;
        /* RateTransition: '<Root>/RT' */
        brake_acc_nodiv_ctrl_B_local_RT_h = RT_Buffer0;
    /* RateTransition: '<Root>/RT4' */
    brake_acc_nodiv_ctrl_B_local_RT4 = RT4_Buffer0;
    // inline
    /* S-Function (fcncallgen): '<Root>/10ms_4' incorporates:
     *  SubSystem: '<Root>/ABS_FL_Wheel'
     */
    //brake_acc_nodiv_ct_ABS_RR_Wheel(brake_acc_nodiv_ctrl_B_local_RT4,
    //  brake_acc_nodiv_ctrl_B_local_RT3, rtb_to_int1,
    //  &brake_acc_nodiv_ctrl_B_local_ABS_FL_Wheel, //(P_ABS_RR_Wheel_brake_acc_nodi_T *)
    //  brake_acc_nodiv_ctrl_P_ABS_FL_Wheel);

    /* local block i/o variables */
    bool rtb_RelationalOperator;
    int rtb_vms;
    int u;
    /* Outputs for Atomic SubSystem: '<S4>/If v>=10 km//h' */
    /* Gain: '<S29>/v (m//s)' */
    rtb_vms = brake_acc_nodiv_ctrl_P_ABS_FL_Wheel_vms_Gain * brake_acc_nodiv_ctrl_B_local_RT4;
    /* Sum: '<S29>/Subtract' incorporates:
     *  Gain: '<S29>/wheSpd (m//s)'
     */
    u = rtb_vms - brake_acc_nodiv_ctrl_P_ABS_FL_Wheel_wheSpdms_Gain * rtb_to_int1;
    /* Saturate: '<S29>/positive' */
    // Lihao: lift
    int positive_UpperSat_local = brake_acc_nodiv_ctrl_P_ABS_FL_Wheel_positive_UpperSat;
    if (u >= positive_UpperSat_local) {
        u = brake_acc_nodiv_ctrl_P_ABS_FL_Wheel_positive_UpperSat;
    } else {
        // Lihao: lift
        int positive_LowerSat_local = brake_acc_nodiv_ctrl_P_ABS_FL_Wheel_positive_LowerSat;
        if (u <= positive_LowerSat_local) {
            u = brake_acc_nodiv_ctrl_P_ABS_FL_Wheel_positive_LowerSat;
        }
    }
    /* RelationalOperator: '<S29>/Relational Operator' incorporates:
     *  Gain: '<S29>/slip_abs_on_times_10'
     *  Gain: '<S29>/times_10'
     *  Saturate: '<S29>/positive'
     */
    rtb_RelationalOperator = (brake_acc_nodiv_ctrl_P_ABS_FL_Wheel_times_10_Gain * u >
            brake_acc_nodiv_ctrl_P_ABS_FL_Wheel_slip_abs_on_times_10_Gain * rtb_vms);
    /* Switch: '<S4>/Threshold_10km//h' */
    // Lihao: lift
    int Threshold_10kmh_Threshold_local = brake_acc_nodiv_ctrl_P_ABS_FL_Wheel_Threshold_10kmh_Threshold;
    if (brake_acc_nodiv_ctrl_B_local_RT4 >= Threshold_10kmh_Threshold_local) {
        /* Outputs for Atomic SubSystem: '<S4>/If v>=10 km//h' */
        /* Switch: '<S29>/LockDetect' incorporates:
         *  Constant: '<S29>/ReleaseBrake'
         */
        if (rtb_RelationalOperator) {
            brake_acc_nodiv_ctrl_B_local_ABS_FL_Wheel_Threshold_10kmh = brake_acc_nodiv_ctrl_P_ABS_FL_Wheel_ReleaseBrake_Value;
        } else {
            brake_acc_nodiv_ctrl_B_local_ABS_FL_Wheel_Threshold_10kmh = brake_acc_nodiv_ctrl_B_local_RT3;
        }
        /* End of Outputs for SubSystem: '<S4>/If v>=10 km//h' */
    } else {
        /* Outputs for Atomic SubSystem: '<S4>/If v>=10 km//h' */
        /* Switch: '<S29>/LockDetect' */
        brake_acc_nodiv_ctrl_B_local_ABS_FL_Wheel_Threshold_10kmh = brake_acc_nodiv_ctrl_B_local_RT3;
        /* End of Outputs for SubSystem: '<S4>/If v>=10 km//h' */
    }
    /* End of Switch: '<S4>/Threshold_10km//h' */
    // end of inline
    /* Outport: '<Root>/Out_TQ_FLBrake' */
    //brake_acc_nodiv_ctrl_Y_Out_TQ_FLBrake =
    //  brake_acc_nodiv_ctrl_B_local_ABS_FL_Wheel_Threshold_10kmh;
    /* Saturate: '<S7>/Pedal_map' incorporates:
     *  Inport: '<Root>/In_BrakePedal'
     */
    // Lihao: lift
    int In_BrakePedal = brake_acc_nodiv_ctrl_U_In_BrakePedal;
    int Pedal_map_UpperSat = brake_acc_nodiv_ctrl_P_Pedal_map_UpperSat;
    if (In_BrakePedal >= Pedal_map_UpperSat) {
        tmp = brake_acc_nodiv_ctrl_P_Pedal_map_UpperSat;
    } else {
        // Lihao: lift
        In_BrakePedal = brake_acc_nodiv_ctrl_U_In_BrakePedal;
        int Pedal_map_LowerSat = brake_acc_nodiv_ctrl_P_Pedal_map_LowerSat;
        if (In_BrakePedal <= Pedal_map_LowerSat) {
            tmp = brake_acc_nodiv_ctrl_P_Pedal_map_LowerSat;
        } else {
            tmp = brake_acc_nodiv_ctrl_U_In_BrakePedal;
        }
    }
    /* S-Function (fcncallgen): '<Root>/10ms' incorporates:
     *  SubSystem: '<Root>/Brake_Torq_Calculation'
     */
    //RT14 = brake_acc_nodiv_ctrl_P_Gain1_Gain * rt_roundd_snf(tmp);
    RT14 = brake_acc_nodiv_ctrl_P_Gain1_Gain * tmp;
    /* RateTransition: '<Root>/RT12' */
    RT12 = rtb_to_int1;
}

void *task_FR_Wheel(void *unused)
{

    //B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B_local;
    /* Start for RateTransition: '<Root>/RT' */
    brake_acc_nodiv_ctrl_B_local_RT_h = brake_acc_nodiv_ctrl_P_RT_X0;
    /* Start for RateTransition: '<Root>/RT4' */
    brake_acc_nodiv_ctrl_B_local_RT4 = brake_acc_nodiv_ctrl_P_RT4_X0;
    /* Start for RateTransition: '<Root>/RT1' */
    brake_acc_nodiv_ctrl_B_local_RT1 = brake_acc_nodiv_ctrl_P_RT1_X0;
    /* Start for RateTransition: '<Root>/RT2' */
    brake_acc_nodiv_ctrl_B_local_RT2 = brake_acc_nodiv_ctrl_P_RT2_X0;
    /* Start for RateTransition: '<Root>/RT3' */
    brake_acc_nodiv_ctrl_B_local_RT3 = brake_acc_nodiv_ctrl_P_RT3_X0;
    //B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B_local = brake_acc_nodiv_ctrl_B;

    //brake_acc_nodiv_ctrl_step1 // the following code is from this function
    int rtb_to_int_k;
    /* RateTransition: '<Root>/RT4' */
    brake_acc_nodiv_ctrl_B_local_RT4 = RT4_Buffer0;
    /* Rounding: '<S11>/to_int' incorporates:
     *  Inport: '<Root>/In_FRRotation'
     */
    //rtb_to_int_k = rt_roundd_snf(brake_acc_nodiv_ctrl_U_In_FRRotation);
	rtb_to_int_k = brake_acc_nodiv_ctrl_U_In_FRRotation;
    // inline
    /* S-Function (fcncallgen): '<Root>/10ms_3' incorporates:
     *  SubSystem: '<Root>/ABS_FR_Wheel'
     */
    //brake_acc_nodiv_ct_ABS_RR_Wheel(brake_acc_nodiv_ctrl_B_local_RT4,
    //  brake_acc_nodiv_ctrl_B_local_RT2, rtb_to_int_k,
    //  &brake_acc_nodiv_ctrl_B_local_ABS_FR_Wheel, //(P_ABS_RR_Wheel_brake_acc_nodi_T *)
    //  brake_acc_nodiv_ctrl_P_ABS_FR_Wheel);

    /* local block i/o variables */
    bool rtb_RelationalOperator;
    int rtb_vms;
    int u;
    /* Outputs for Atomic SubSystem: '<S4>/If v>=10 km//h' */
    /* Gain: '<S29>/v (m//s)' */
    rtb_vms = brake_acc_nodiv_ctrl_P_ABS_FR_Wheel_vms_Gain * brake_acc_nodiv_ctrl_B_local_RT4;
    /* Sum: '<S29>/Subtract' incorporates:
     *  Gain: '<S29>/wheSpd (m//s)'
     */
    u = rtb_vms - brake_acc_nodiv_ctrl_P_ABS_FR_Wheel_wheSpdms_Gain * rtb_to_int_k;
    /* Saturate: '<S29>/positive' */
    // Lihao: lift
    int positive_UpperSat_local = brake_acc_nodiv_ctrl_P_ABS_FR_Wheel_positive_UpperSat;
    if (u >= positive_UpperSat_local) {
        u = brake_acc_nodiv_ctrl_P_ABS_FR_Wheel_positive_UpperSat;
    } else {
        // Lihao: lift
        int positive_LowerSat_local = brake_acc_nodiv_ctrl_P_ABS_FR_Wheel_positive_LowerSat;
        if (u <= positive_LowerSat_local) {
            u = brake_acc_nodiv_ctrl_P_ABS_FR_Wheel_positive_LowerSat;
        }
    }
    /* RelationalOperator: '<S29>/Relational Operator' incorporates:
     *  Gain: '<S29>/slip_abs_on_times_10'
     *  Gain: '<S29>/times_10'
     *  Saturate: '<S29>/positive'
     */
    rtb_RelationalOperator = (brake_acc_nodiv_ctrl_P_ABS_FR_Wheel_times_10_Gain * u >
            brake_acc_nodiv_ctrl_P_ABS_FR_Wheel_slip_abs_on_times_10_Gain * rtb_vms);
    /* Switch: '<S4>/Threshold_10km//h' */
    // Lihao: lift
    int Threshold_10kmh_Threshold_local = brake_acc_nodiv_ctrl_P_ABS_FR_Wheel_Threshold_10kmh_Threshold;
    if (brake_acc_nodiv_ctrl_B_local_RT4 >= Threshold_10kmh_Threshold_local) {
        /* Outputs for Atomic SubSystem: '<S4>/If v>=10 km//h' */
        /* Switch: '<S29>/LockDetect' incorporates:
         *  Constant: '<S29>/ReleaseBrake'
         */
        if (rtb_RelationalOperator) {
            brake_acc_nodiv_ctrl_B_local_ABS_FR_Wheel_Threshold_10kmh = brake_acc_nodiv_ctrl_P_ABS_FR_Wheel_ReleaseBrake_Value;
        } else {
            brake_acc_nodiv_ctrl_B_local_ABS_FR_Wheel_Threshold_10kmh = brake_acc_nodiv_ctrl_B_local_RT2;
        }
        /* End of Outputs for SubSystem: '<S4>/If v>=10 km//h' */
    } else {
        /* Outputs for Atomic SubSystem: '<S4>/If v>=10 km//h' */
        /* Switch: '<S29>/LockDetect' */
        brake_acc_nodiv_ctrl_B_local_ABS_FR_Wheel_Threshold_10kmh = brake_acc_nodiv_ctrl_B_local_RT2;
        /* End of Outputs for SubSystem: '<S4>/If v>=10 km//h' */
    }
    /* End of Switch: '<S4>/Threshold_10km//h' */
    // end of inline

    /* Outport: '<Root>/Out_TQ_FRBrake' */
    //brake_acc_nodiv_ctrl_Y_Out_TQ_FRBrake =
    //  brake_acc_nodiv_ctrl_B_local_ABS_FR_Wheel_Threshold_10kmh;
    /* RateTransition: '<Root>/RT3' */
    brake_acc_nodiv_ctrl_B_local_RT3 = RT3_Buffer0;
    /* End of RateTransition: '<Root>/RT3' */
    /* RateTransition: '<Root>/RT11' */
    RT11 = rtb_to_int_k;
}

void *task_RL_Wheel(void *unused)
{  
    //#if 0
    //B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B_local;
    /* Start for RateTransition: '<Root>/RT' */
    brake_acc_nodiv_ctrl_B_local_RT_h = brake_acc_nodiv_ctrl_P_RT_X0;

    /* Start for RateTransition: '<Root>/RT4' */
    brake_acc_nodiv_ctrl_B_local_RT4 = brake_acc_nodiv_ctrl_P_RT4_X0;

    /* Start for RateTransition: '<Root>/RT1' */
    brake_acc_nodiv_ctrl_B_local_RT1 = brake_acc_nodiv_ctrl_P_RT1_X0;

    /* Start for RateTransition: '<Root>/RT2' */
    brake_acc_nodiv_ctrl_B_local_RT2 = brake_acc_nodiv_ctrl_P_RT2_X0;

    /* Start for RateTransition: '<Root>/RT3' */
    brake_acc_nodiv_ctrl_B_local_RT3 = brake_acc_nodiv_ctrl_P_RT3_X0;
    //#endif
    //B_brake_acc_nodiv_ctrl_T brake_acc_nodiv_ctrl_B_local = brake_acc_nodiv_ctrl_B;
    //brake_acc_nodiv_ctrl_step1 // the following code is from this function
    int rtb_to_int_g;
    /* RateTransition: '<Root>/RT4' */
    brake_acc_nodiv_ctrl_B_local_RT4 = RT4_Buffer0;
    // inline
    /* S-Function (fcncallgen): '<Root>/10ms_2' incorporates:
     *  SubSystem: '<Root>/ABS_RL_Wheel'
     */
    //brake_acc_nodiv_ct_ABS_RR_Wheel(brake_acc_nodiv_ctrl_B_local_RT4,
    //  brake_acc_nodiv_ctrl_B_local_RT1, rtb_to_int_g,
    //  &brake_acc_nodiv_ctrl_B_local_ABS_RL_Wheel, //(P_ABS_RR_Wheel_brake_acc_nodi_T *)
    //  brake_acc_nodiv_ctrl_P_ABS_RL_Wheel);
    /* local block i/o variables */
    bool rtb_RelationalOperator;
    int rtb_vms;
    int u;
    /* Outputs for Atomic SubSystem: '<S4>/If v>=10 km//h' */
    /* Gain: '<S29>/v (m//s)' */
    rtb_vms = brake_acc_nodiv_ctrl_P_ABS_RL_Wheel_vms_Gain * brake_acc_nodiv_ctrl_B_local_RT4;
    /* Sum: '<S29>/Subtract' incorporates:
     *  Gain: '<S29>/wheSpd (m//s)'
     */
    u = rtb_vms - brake_acc_nodiv_ctrl_P_ABS_RL_Wheel_wheSpdms_Gain * rtb_to_int_g;
    /* Saturate: '<S29>/positive' */
    // Lihao: lift
    int positive_UpperSat_local = brake_acc_nodiv_ctrl_P_ABS_RL_Wheel_positive_UpperSat;
    if (u >= positive_UpperSat_local) {
        u = brake_acc_nodiv_ctrl_P_ABS_RL_Wheel_positive_UpperSat;
    } else {
        // Lihao: lift
        int positive_LowerSat_local = brake_acc_nodiv_ctrl_P_ABS_RL_Wheel_positive_LowerSat;
        if (u <= positive_LowerSat_local) {
            u = brake_acc_nodiv_ctrl_P_ABS_RL_Wheel_positive_LowerSat;
        }
    }
    /* RelationalOperator: '<S29>/Relational Operator' incorporates:
     *  Gain: '<S29>/slip_abs_on_times_10'
     *  Gain: '<S29>/times_10'
     *  Saturate: '<S29>/positive'
     */
    rtb_RelationalOperator = (brake_acc_nodiv_ctrl_P_ABS_RL_Wheel_times_10_Gain * u >
            brake_acc_nodiv_ctrl_P_ABS_RL_Wheel_slip_abs_on_times_10_Gain * rtb_vms);

    /* Switch: '<S4>/Threshold_10km//h' */
    // Lihao: lift
    int Threshold_10kmh_Threshold_local = brake_acc_nodiv_ctrl_P_ABS_RL_Wheel_Threshold_10kmh_Threshold;
    if (brake_acc_nodiv_ctrl_B_local_RT4 >= Threshold_10kmh_Threshold_local) {
        /* Outputs for Atomic SubSystem: '<S4>/If v>=10 km//h' */
        /* Switch: '<S29>/LockDetect' incorporates:
         *  Constant: '<S29>/ReleaseBrake'
         */
        if (rtb_RelationalOperator) {
            brake_acc_nodiv_ctrl_B_local_ABS_RL_Wheel_Threshold_10kmh = brake_acc_nodiv_ctrl_P_ABS_RL_Wheel_ReleaseBrake_Value;
        } else {
            brake_acc_nodiv_ctrl_B_local_ABS_RL_Wheel_Threshold_10kmh = brake_acc_nodiv_ctrl_B_local_RT1;
        }

        /* End of Outputs for SubSystem: '<S4>/If v>=10 km//h' */
    } else {
        /* Outputs for Atomic SubSystem: '<S4>/If v>=10 km//h' */
        /* Switch: '<S29>/LockDetect' */
        brake_acc_nodiv_ctrl_B_local_ABS_RL_Wheel_Threshold_10kmh = brake_acc_nodiv_ctrl_B_local_RT1;

        /* End of Outputs for SubSystem: '<S4>/If v>=10 km//h' */
    }

    /* End of Switch: '<S4>/Threshold_10km//h' */
    // end of inline
    /* Outport: '<Root>/Out_TQ_RLBrake' */
    //brake_acc_nodiv_ctrl_Y_Out_TQ_RLBrake =
    //  brake_acc_nodiv_ctrl_B_local_ABS_RL_Wheel_Threshold_10kmh;
    /* RateTransition: '<Root>/RT2' */
    brake_acc_nodiv_ctrl_B_local_RT2 = RT2_Buffer0;
    /* End of RateTransition: '<Root>/RT2' */
    /* RateTransition: '<Root>/RT10' */
    RT10 = rtb_to_int_g;
}


/* Function: main =============================================================
 *
 * Abstract:
 *   Execute model on a generic target such as a workstation_
 */
int main(void)
{
    //rt_InitModel();
    //havocInputs(&brake_acc_nodiv_ctrl_U);
    //Lihao
    pthread_t t0, t1, t2, t3, t4;
    pthread_create(&t0, 0, task_compute, 0);
    pthread_create(&t1, 0, task_RR_Wheel, 0);
    pthread_create(&t2, 0, task_FL_Wheel, 0);
    pthread_create(&t3, 0, task_FR_Wheel, 0);
    pthread_create(&t4, 0, task_RL_Wheel, 0);
    return 0;
    //const int_T result = rt_TermModel();
    //return result;
}

/* [EOF] rt_main_c */
