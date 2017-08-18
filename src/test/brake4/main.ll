; ModuleID = 'main2.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%union.pthread_attr_t = type { i64, [48 x i8] }

@__VERIFIER_thread_priorities = constant [5 x i32] [i32 5, i32 5, i32 5, i32 5, i32 22], align 16
@.str = private unnamed_addr constant [17 x i8] c"c::task_RR_Wheel\00", align 1
@.str.1 = private unnamed_addr constant [17 x i8] c"c::task_FR_Wheel\00", align 1
@.str.2 = private unnamed_addr constant [17 x i8] c"c::task_FL_Wheel\00", align 1
@.str.3 = private unnamed_addr constant [17 x i8] c"c::task_RL_Wheel\00", align 1
@.str.4 = private unnamed_addr constant [16 x i8] c"c::task_compute\00", align 1
@__VERIFIER_threads = global [5 x i8*] [i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.4, i32 0, i32 0)], align 16
@brake_acc_nodiv_ctrl_B_local_ABS_FL_Wheel_Threshold_10kmh = global i32 100, align 4
@brake_acc_nodiv_ctrl_B_local_ABS_RL_Wheel_Threshold_10kmh = global i32 100, align 4
@brake_acc_nodiv_ctrl_B_local_ABS_RR_Wheel_Threshold_10kmh = global i32 100, align 4
@brake_acc_nodiv_ctrl_B_local_ABS_FR_Wheel_Threshold_10kmh = global i32 100, align 4
@brake_acc_nodiv_ctrl_P_RT_X0 = common global i32 0, align 4
@brake_acc_nodiv_ctrl_B_local_RT_h = common global i32 0, align 4
@brake_acc_nodiv_ctrl_P_RT4_X0 = common global i32 0, align 4
@brake_acc_nodiv_ctrl_B_local_RT4 = common global i32 0, align 4
@brake_acc_nodiv_ctrl_P_RT1_X0 = common global i32 0, align 4
@brake_acc_nodiv_ctrl_B_local_RT1 = common global i32 0, align 4
@brake_acc_nodiv_ctrl_P_RT2_X0 = common global i32 0, align 4
@brake_acc_nodiv_ctrl_B_local_RT2 = common global i32 0, align 4
@brake_acc_nodiv_ctrl_P_RT3_X0 = common global i32 0, align 4
@brake_acc_nodiv_ctrl_B_local_RT3 = common global i32 0, align 4
@test = common global i32 0, align 4
@brake_acc_nodiv_ctrl_P_Distribution_Gain1 = common global i32 0, align 4
@brake_acc_nodiv_ctrl_B_local_RT14 = common global i32 0, align 4
@Distribution_idx = common global i32 0, align 4
@brake_acc_nodiv_ctrl_P_Distribution_Gain2 = common global i32 0, align 4
@Distribution_idx_0 = common global i32 0, align 4
@brake_acc_nodiv_ctrl_P_Distribution_Gain3 = common global i32 0, align 4
@Distribution_idx_1 = common global i32 0, align 4
@RT9 = common global i32 0, align 4
@RT10 = common global i32 0, align 4
@vkmh = common global i32 0, align 4
@RT11 = common global i32 0, align 4
@RT12 = common global i32 0, align 4
@brake_acc_nodiv_ctrl_P_average_rpm_Gain = common global i32 0, align 4
@brake_acc_nodiv_ctrl_P_wgrads_Gain = common global i32 0, align 4
@brake_acc_nodiv_ctrl_P_vkmh_Gain = common global i32 0, align 4
@.str.5 = private unnamed_addr constant [2 x i8] c"0\00", align 1
@.str.6 = private unnamed_addr constant [7 x i8] c"main.c\00", align 1
@__PRETTY_FUNCTION__.task_compute = private unnamed_addr constant [27 x i8] c"void *task_compute(void *)\00", align 1
@brake_acc_nodiv_ctrl_P_Distribution_Gain0 = common global i32 0, align 4
@RT_Buffer0 = common global i32 0, align 4
@RT4_Buffer0 = common global i32 0, align 4
@RT1_Buffer0 = common global i32 0, align 4
@RT2_Buffer0 = common global i32 0, align 4
@RT3_Buffer0 = common global i32 0, align 4
@brake_acc_nodiv_ctrl_P_Gain1_Gain = common global i32 0, align 4
@RT14 = common global i32 0, align 4
@brake_acc_nodiv_ctrl_B_local_RTH = common global i32 0, align 4
@RT0_Buffer0 = common global i32 0, align 4

; Function Attrs: nounwind uwtable
define i8* @task_compute(i8* %unused) #0 {
entry:
  %0 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT_X0, align 4
  store i32 %0, i32* @brake_acc_nodiv_ctrl_B_local_RT_h, align 4
  %1 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT4_X0, align 4
  store i32 %1, i32* @brake_acc_nodiv_ctrl_B_local_RT4, align 4
  %2 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT1_X0, align 4
  store i32 %2, i32* @brake_acc_nodiv_ctrl_B_local_RT1, align 4
  %3 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT2_X0, align 4
  store i32 %3, i32* @brake_acc_nodiv_ctrl_B_local_RT2, align 4
  %4 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT3_X0, align 4
  store i32 %4, i32* @brake_acc_nodiv_ctrl_B_local_RT3, align 4
  store i32 0, i32* @test, align 4
  %5 = load i32, i32* @brake_acc_nodiv_ctrl_P_Distribution_Gain1, align 4
  %6 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT14, align 4
  %add = add nsw i32 %5, %6
  store i32 %add, i32* @Distribution_idx, align 4
  %7 = load i32, i32* @brake_acc_nodiv_ctrl_P_Distribution_Gain2, align 4
  %8 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT14, align 4
  %add1 = add nsw i32 %7, %8
  store i32 %add1, i32* @Distribution_idx_0, align 4
  %9 = load i32, i32* @brake_acc_nodiv_ctrl_P_Distribution_Gain3, align 4
  %10 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT14, align 4
  %add2 = add nsw i32 %9, %10
  store i32 %add2, i32* @Distribution_idx_1, align 4
  %11 = load i32, i32* @RT9, align 4
  %12 = load i32, i32* @RT10, align 4
  %add3 = add nsw i32 %11, %12
  store i32 %add3, i32* @vkmh, align 4
  %13 = load i32, i32* @vkmh, align 4
  %14 = load i32, i32* @RT11, align 4
  %add4 = add nsw i32 %13, %14
  store i32 %add4, i32* @vkmh, align 4
  %15 = load i32, i32* @vkmh, align 4
  %16 = load i32, i32* @RT12, align 4
  %add5 = add nsw i32 %15, %16
  store i32 %add5, i32* @vkmh, align 4
  %17 = load i32, i32* @vkmh, align 4
  %18 = load i32, i32* @brake_acc_nodiv_ctrl_P_average_rpm_Gain, align 4
  %add6 = add nsw i32 %17, %18
  store i32 %add6, i32* @vkmh, align 4
  %19 = load i32, i32* @vkmh, align 4
  %20 = load i32, i32* @brake_acc_nodiv_ctrl_P_wgrads_Gain, align 4
  %add7 = add nsw i32 %19, %20
  store i32 %add7, i32* @vkmh, align 4
  %21 = load i32, i32* @vkmh, align 4
  %22 = load i32, i32* @brake_acc_nodiv_ctrl_P_vkmh_Gain, align 4
  %add8 = add nsw i32 %21, %22
  store i32 %add8, i32* @vkmh, align 4
  %23 = load i32, i32* @test, align 4
  %cmp = icmp ne i32 %23, 0
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.6, i32 0, i32 0), i32 317, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @__PRETTY_FUNCTION__.task_compute, i32 0, i32 0)) #3
  unreachable

if.end:                                           ; preds = %entry
  %24 = load i32, i32* @test, align 4
  %cmp9 = icmp ne i32 %24, 1
  br i1 %cmp9, label %if.then10, label %if.end11

if.then10:                                        ; preds = %if.end
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.6, i32 0, i32 0), i32 322, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @__PRETTY_FUNCTION__.task_compute, i32 0, i32 0)) #3
  unreachable

if.end11:                                         ; preds = %if.end
  %25 = load i32, i32* @test, align 4
  %cmp12 = icmp ne i32 %25, 2
  br i1 %cmp12, label %if.then13, label %if.end14

if.then13:                                        ; preds = %if.end11
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.6, i32 0, i32 0), i32 327, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @__PRETTY_FUNCTION__.task_compute, i32 0, i32 0)) #3
  unreachable

if.end14:                                         ; preds = %if.end11
  %26 = load i32, i32* @test, align 4
  %cmp15 = icmp ne i32 %26, 3
  br i1 %cmp15, label %if.then16, label %if.end17

if.then16:                                        ; preds = %if.end14
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.6, i32 0, i32 0), i32 332, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @__PRETTY_FUNCTION__.task_compute, i32 0, i32 0)) #3
  unreachable

if.end17:                                         ; preds = %if.end14
  %27 = load i32, i32* @test, align 4
  %cmp18 = icmp sle i32 %27, 0
  br i1 %cmp18, label %if.then19, label %if.end20

if.then19:                                        ; preds = %if.end17
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.6, i32 0, i32 0), i32 337, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @__PRETTY_FUNCTION__.task_compute, i32 0, i32 0)) #3
  unreachable

if.end20:                                         ; preds = %if.end17
  %28 = load i32, i32* @test, align 4
  %inc = add nsw i32 %28, 1
  store i32 %inc, i32* @test, align 4
  %29 = load i32, i32* @test, align 4
  %cmp21 = icmp ne i32 %29, 0
  br i1 %cmp21, label %if.then22, label %if.end23

if.then22:                                        ; preds = %if.end20
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.6, i32 0, i32 0), i32 344, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @__PRETTY_FUNCTION__.task_compute, i32 0, i32 0)) #3
  unreachable

if.end23:                                         ; preds = %if.end20
  %30 = load i32, i32* @test, align 4
  %cmp24 = icmp ne i32 %30, 1
  br i1 %cmp24, label %if.then25, label %if.end26

if.then25:                                        ; preds = %if.end23
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.6, i32 0, i32 0), i32 349, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @__PRETTY_FUNCTION__.task_compute, i32 0, i32 0)) #3
  unreachable

if.end26:                                         ; preds = %if.end23
  %31 = load i32, i32* @test, align 4
  %cmp27 = icmp ne i32 %31, 2
  br i1 %cmp27, label %if.then28, label %if.end29

if.then28:                                        ; preds = %if.end26
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.6, i32 0, i32 0), i32 354, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @__PRETTY_FUNCTION__.task_compute, i32 0, i32 0)) #3
  unreachable

if.end29:                                         ; preds = %if.end26
  %32 = load i32, i32* @test, align 4
  %cmp30 = icmp ne i32 %32, 3
  br i1 %cmp30, label %if.then31, label %if.end32

if.then31:                                        ; preds = %if.end29
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.6, i32 0, i32 0), i32 359, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @__PRETTY_FUNCTION__.task_compute, i32 0, i32 0)) #3
  unreachable

if.end32:                                         ; preds = %if.end29
  %33 = load i32, i32* @test, align 4
  %cmp33 = icmp sle i32 %33, 0
  br i1 %cmp33, label %if.then34, label %if.end35

if.then34:                                        ; preds = %if.end32
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.6, i32 0, i32 0), i32 364, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @__PRETTY_FUNCTION__.task_compute, i32 0, i32 0)) #3
  unreachable

if.end35:                                         ; preds = %if.end32
  %34 = load i32, i32* @brake_acc_nodiv_ctrl_P_Distribution_Gain0, align 4
  %35 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT14, align 4
  %add36 = add nsw i32 %34, %35
  store i32 %add36, i32* @RT_Buffer0, align 4
  %36 = load i32, i32* @vkmh, align 4
  store i32 %36, i32* @RT4_Buffer0, align 4
  %37 = load i32, i32* @Distribution_idx, align 4
  store i32 %37, i32* @RT1_Buffer0, align 4
  %38 = load i32, i32* @Distribution_idx_0, align 4
  store i32 %38, i32* @RT2_Buffer0, align 4
  %39 = load i32, i32* @Distribution_idx_1, align 4
  store i32 %39, i32* @RT3_Buffer0, align 4
  ret i8* undef
}

; Function Attrs: noreturn nounwind
declare void @__assert_fail(i8*, i8*, i32, i8*) #1

; Function Attrs: nounwind uwtable
define i8* @task_RR_Wheel(i8* %unused) #0 {
entry:
  %0 = load i32, i32* @test, align 4
  %inc = add nsw i32 %0, 1
  store i32 %inc, i32* @test, align 4
  %1 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT_X0, align 4
  store i32 %1, i32* @brake_acc_nodiv_ctrl_B_local_RT_h, align 4
  %2 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT4_X0, align 4
  store i32 %2, i32* @brake_acc_nodiv_ctrl_B_local_RT4, align 4
  %3 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT1_X0, align 4
  store i32 %3, i32* @brake_acc_nodiv_ctrl_B_local_RT1, align 4
  %4 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT2_X0, align 4
  store i32 %4, i32* @brake_acc_nodiv_ctrl_B_local_RT2, align 4
  %5 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT3_X0, align 4
  store i32 %5, i32* @brake_acc_nodiv_ctrl_B_local_RT3, align 4
  %6 = load i32, i32* @RT_Buffer0, align 4
  store i32 %6, i32* @brake_acc_nodiv_ctrl_B_local_RT_h, align 4
  %7 = load i32, i32* @RT4_Buffer0, align 4
  store i32 %7, i32* @brake_acc_nodiv_ctrl_B_local_RT4, align 4
  %8 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT4, align 4
  %add = add nsw i32 10, %8
  %add1 = add nsw i32 %add, 10
  %add2 = add nsw i32 %add1, 10
  %cmp = icmp sge i32 %add2, 100
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  br label %if.end5

if.else:                                          ; preds = %entry
  %cmp3 = icmp sle i32 %add2, 5
  br i1 %cmp3, label %if.then4, label %if.end

if.then4:                                         ; preds = %if.else
  br label %if.end

if.end:                                           ; preds = %if.then4, %if.else
  br label %if.end5

if.end5:                                          ; preds = %if.end, %if.then
  %add6 = add nsw i32 10, %add
  %cmp7 = icmp sgt i32 10, %add6
  br i1 %cmp7, label %if.then8, label %if.else9

if.then8:                                         ; preds = %if.end5
  br label %if.end10

if.else9:                                         ; preds = %if.end5
  br label %if.end10

if.end10:                                         ; preds = %if.else9, %if.then8
  %rtb_RelationalOperator.0 = phi i8 [ 1, %if.then8 ], [ 0, %if.else9 ]
  %9 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT4, align 4
  %cmp11 = icmp sge i32 %9, 100
  br i1 %cmp11, label %if.then12, label %if.else16

if.then12:                                        ; preds = %if.end10
  %tobool = trunc i8 %rtb_RelationalOperator.0 to i1
  br i1 %tobool, label %if.then13, label %if.else14

if.then13:                                        ; preds = %if.then12
  store i32 8, i32* @brake_acc_nodiv_ctrl_B_local_ABS_RR_Wheel_Threshold_10kmh, align 4
  br label %if.end15

if.else14:                                        ; preds = %if.then12
  %10 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT_h, align 4
  store i32 %10, i32* @brake_acc_nodiv_ctrl_B_local_ABS_RR_Wheel_Threshold_10kmh, align 4
  br label %if.end15

if.end15:                                         ; preds = %if.else14, %if.then13
  br label %if.end17

if.else16:                                        ; preds = %if.end10
  %11 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT_h, align 4
  store i32 %11, i32* @brake_acc_nodiv_ctrl_B_local_ABS_RR_Wheel_Threshold_10kmh, align 4
  br label %if.end17

if.end17:                                         ; preds = %if.else16, %if.end15
  %12 = load i32, i32* @RT1_Buffer0, align 4
  store i32 %12, i32* @brake_acc_nodiv_ctrl_B_local_RT1, align 4
  store i32 10, i32* @RT9, align 4
  ret i8* undef
}

; Function Attrs: nounwind uwtable
define i8* @task_FL_Wheel(i8* %unused) #0 {
entry:
  %0 = load i32, i32* @test, align 4
  %inc = add nsw i32 %0, 1
  store i32 %inc, i32* @test, align 4
  %1 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT_X0, align 4
  store i32 %1, i32* @brake_acc_nodiv_ctrl_B_local_RT_h, align 4
  %2 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT4_X0, align 4
  store i32 %2, i32* @brake_acc_nodiv_ctrl_B_local_RT4, align 4
  %3 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT1_X0, align 4
  store i32 %3, i32* @brake_acc_nodiv_ctrl_B_local_RT1, align 4
  %4 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT2_X0, align 4
  store i32 %4, i32* @brake_acc_nodiv_ctrl_B_local_RT2, align 4
  %5 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT3_X0, align 4
  store i32 %5, i32* @brake_acc_nodiv_ctrl_B_local_RT3, align 4
  %6 = load i32, i32* @RT_Buffer0, align 4
  store i32 %6, i32* @brake_acc_nodiv_ctrl_B_local_RT_h, align 4
  %7 = load i32, i32* @RT4_Buffer0, align 4
  store i32 %7, i32* @brake_acc_nodiv_ctrl_B_local_RT4, align 4
  %8 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT4, align 4
  %add = add nsw i32 10, %8
  %add1 = add nsw i32 %add, 10
  %add2 = add nsw i32 %add1, 10
  %cmp = icmp sge i32 %add2, 100
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  br label %if.end5

if.else:                                          ; preds = %entry
  %cmp3 = icmp sle i32 %add2, 5
  br i1 %cmp3, label %if.then4, label %if.end

if.then4:                                         ; preds = %if.else
  br label %if.end

if.end:                                           ; preds = %if.then4, %if.else
  br label %if.end5

if.end5:                                          ; preds = %if.end, %if.then
  %add6 = add nsw i32 10, %add
  %cmp7 = icmp sgt i32 10, %add6
  br i1 %cmp7, label %if.then8, label %if.else9

if.then8:                                         ; preds = %if.end5
  br label %if.end10

if.else9:                                         ; preds = %if.end5
  br label %if.end10

if.end10:                                         ; preds = %if.else9, %if.then8
  %rtb_RelationalOperator.0 = phi i8 [ 1, %if.then8 ], [ 0, %if.else9 ]
  %9 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT4, align 4
  %cmp11 = icmp sge i32 %9, 100
  br i1 %cmp11, label %if.then12, label %if.else16

if.then12:                                        ; preds = %if.end10
  %tobool = trunc i8 %rtb_RelationalOperator.0 to i1
  br i1 %tobool, label %if.then13, label %if.else14

if.then13:                                        ; preds = %if.then12
  store i32 8, i32* @brake_acc_nodiv_ctrl_B_local_ABS_FL_Wheel_Threshold_10kmh, align 4
  br label %if.end15

if.else14:                                        ; preds = %if.then12
  %10 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT3, align 4
  store i32 %10, i32* @brake_acc_nodiv_ctrl_B_local_ABS_FL_Wheel_Threshold_10kmh, align 4
  br label %if.end15

if.end15:                                         ; preds = %if.else14, %if.then13
  br label %if.end17

if.else16:                                        ; preds = %if.end10
  %11 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT3, align 4
  store i32 %11, i32* @brake_acc_nodiv_ctrl_B_local_ABS_FL_Wheel_Threshold_10kmh, align 4
  br label %if.end17

if.end17:                                         ; preds = %if.else16, %if.end15
  %tobool18 = trunc i8 %rtb_RelationalOperator.0 to i1
  br i1 %tobool18, label %if.then19, label %if.else20

if.then19:                                        ; preds = %if.end17
  br label %if.end21

if.else20:                                        ; preds = %if.end17
  br label %if.end21

if.end21:                                         ; preds = %if.else20, %if.then19
  %In_BrakePedal.0 = phi i32 [ 15, %if.then19 ], [ 65, %if.else20 ]
  %cmp22 = icmp sge i32 %In_BrakePedal.0, 50
  br i1 %cmp22, label %if.then23, label %if.end24

if.then23:                                        ; preds = %if.end21
  br label %if.end24

if.end24:                                         ; preds = %if.then23, %if.end21
  %cmp25 = icmp sle i32 %In_BrakePedal.0, 20
  br i1 %cmp25, label %if.then26, label %if.else27

if.then26:                                        ; preds = %if.end24
  br label %if.end28

if.else27:                                        ; preds = %if.end24
  br label %if.end28

if.end28:                                         ; preds = %if.else27, %if.then26
  %tmp.0 = phi i32 [ 20, %if.then26 ], [ 15, %if.else27 ]
  %12 = load i32, i32* @brake_acc_nodiv_ctrl_P_Gain1_Gain, align 4
  %add29 = add nsw i32 %12, %tmp.0
  store i32 %add29, i32* @RT14, align 4
  store i32 10, i32* @RT12, align 4
  ret i8* undef
}

; Function Attrs: nounwind uwtable
define i8* @task_FR_Wheel(i8* %unused) #0 {
entry:
  %0 = load i32, i32* @test, align 4
  %inc = add nsw i32 %0, 1
  store i32 %inc, i32* @test, align 4
  %1 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT_X0, align 4
  store i32 %1, i32* @brake_acc_nodiv_ctrl_B_local_RT_h, align 4
  %2 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT4_X0, align 4
  store i32 %2, i32* @brake_acc_nodiv_ctrl_B_local_RT4, align 4
  %3 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT1_X0, align 4
  store i32 %3, i32* @brake_acc_nodiv_ctrl_B_local_RT1, align 4
  %4 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT2_X0, align 4
  store i32 %4, i32* @brake_acc_nodiv_ctrl_B_local_RT2, align 4
  %5 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT3_X0, align 4
  store i32 %5, i32* @brake_acc_nodiv_ctrl_B_local_RT3, align 4
  %6 = load i32, i32* @RT4_Buffer0, align 4
  store i32 %6, i32* @brake_acc_nodiv_ctrl_B_local_RT4, align 4
  %7 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT4, align 4
  %add = add nsw i32 10, %7
  %add1 = add nsw i32 %add, 10
  %add2 = add nsw i32 %add1, 10
  %cmp = icmp sge i32 %add2, 100
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  br label %if.end5

if.else:                                          ; preds = %entry
  %cmp3 = icmp sle i32 %add2, 5
  br i1 %cmp3, label %if.then4, label %if.end

if.then4:                                         ; preds = %if.else
  br label %if.end

if.end:                                           ; preds = %if.then4, %if.else
  br label %if.end5

if.end5:                                          ; preds = %if.end, %if.then
  %add6 = add nsw i32 10, %add
  %cmp7 = icmp sgt i32 10, %add6
  br i1 %cmp7, label %if.then8, label %if.else9

if.then8:                                         ; preds = %if.end5
  br label %if.end10

if.else9:                                         ; preds = %if.end5
  br label %if.end10

if.end10:                                         ; preds = %if.else9, %if.then8
  %rtb_RelationalOperator.0 = phi i8 [ 1, %if.then8 ], [ 0, %if.else9 ]
  %8 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT4, align 4
  %cmp11 = icmp sge i32 %8, 100
  br i1 %cmp11, label %if.then12, label %if.else16

if.then12:                                        ; preds = %if.end10
  %tobool = trunc i8 %rtb_RelationalOperator.0 to i1
  br i1 %tobool, label %if.then13, label %if.else14

if.then13:                                        ; preds = %if.then12
  store i32 8, i32* @brake_acc_nodiv_ctrl_B_local_ABS_FR_Wheel_Threshold_10kmh, align 4
  br label %if.end15

if.else14:                                        ; preds = %if.then12
  %9 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT2, align 4
  store i32 %9, i32* @brake_acc_nodiv_ctrl_B_local_ABS_FR_Wheel_Threshold_10kmh, align 4
  br label %if.end15

if.end15:                                         ; preds = %if.else14, %if.then13
  br label %if.end17

if.else16:                                        ; preds = %if.end10
  %10 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT2, align 4
  store i32 %10, i32* @brake_acc_nodiv_ctrl_B_local_ABS_FR_Wheel_Threshold_10kmh, align 4
  br label %if.end17

if.end17:                                         ; preds = %if.else16, %if.end15
  %11 = load i32, i32* @RT3_Buffer0, align 4
  store i32 %11, i32* @brake_acc_nodiv_ctrl_B_local_RT3, align 4
  store i32 10, i32* @RT11, align 4
  ret i8* undef
}

; Function Attrs: nounwind uwtable
define i8* @task_RL_Wheel(i8* %unused) #0 {
entry:
  %0 = load i32, i32* @test, align 4
  %inc = add nsw i32 %0, 1
  store i32 %inc, i32* @test, align 4
  %1 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT_X0, align 4
  store i32 %1, i32* @brake_acc_nodiv_ctrl_B_local_RT_h, align 4
  %2 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT4_X0, align 4
  store i32 %2, i32* @brake_acc_nodiv_ctrl_B_local_RT4, align 4
  %3 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT1_X0, align 4
  store i32 %3, i32* @brake_acc_nodiv_ctrl_B_local_RT1, align 4
  %4 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT2_X0, align 4
  store i32 %4, i32* @brake_acc_nodiv_ctrl_B_local_RT2, align 4
  %5 = load i32, i32* @brake_acc_nodiv_ctrl_P_RT3_X0, align 4
  store i32 %5, i32* @brake_acc_nodiv_ctrl_B_local_RT3, align 4
  %6 = load i32, i32* @RT_Buffer0, align 4
  store i32 %6, i32* @brake_acc_nodiv_ctrl_B_local_RT_h, align 4
  %7 = load i32, i32* @RT4_Buffer0, align 4
  store i32 %7, i32* @brake_acc_nodiv_ctrl_B_local_RT4, align 4
  %8 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT4, align 4
  %add = add nsw i32 10, %8
  %add1 = add nsw i32 %add, 10
  %add2 = add nsw i32 %add1, 10
  %cmp = icmp sge i32 %add2, 100
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  br label %if.end5

if.else:                                          ; preds = %entry
  %cmp3 = icmp sle i32 %add2, 5
  br i1 %cmp3, label %if.then4, label %if.end

if.then4:                                         ; preds = %if.else
  br label %if.end

if.end:                                           ; preds = %if.then4, %if.else
  br label %if.end5

if.end5:                                          ; preds = %if.end, %if.then
  %add6 = add nsw i32 10, %add
  %cmp7 = icmp sgt i32 10, %add6
  br i1 %cmp7, label %if.then8, label %if.else9

if.then8:                                         ; preds = %if.end5
  br label %if.end10

if.else9:                                         ; preds = %if.end5
  br label %if.end10

if.end10:                                         ; preds = %if.else9, %if.then8
  %rtb_RelationalOperator.0 = phi i8 [ 1, %if.then8 ], [ 0, %if.else9 ]
  %9 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT4, align 4
  %cmp11 = icmp sge i32 %9, 100
  br i1 %cmp11, label %if.then12, label %if.else16

if.then12:                                        ; preds = %if.end10
  %tobool = trunc i8 %rtb_RelationalOperator.0 to i1
  br i1 %tobool, label %if.then13, label %if.else14

if.then13:                                        ; preds = %if.then12
  store i32 8, i32* @brake_acc_nodiv_ctrl_B_local_ABS_RL_Wheel_Threshold_10kmh, align 4
  br label %if.end15

if.else14:                                        ; preds = %if.then12
  %10 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT1, align 4
  store i32 %10, i32* @brake_acc_nodiv_ctrl_B_local_ABS_RL_Wheel_Threshold_10kmh, align 4
  br label %if.end15

if.end15:                                         ; preds = %if.else14, %if.then13
  br label %if.end17

if.else16:                                        ; preds = %if.end10
  %11 = load i32, i32* @brake_acc_nodiv_ctrl_B_local_RT1, align 4
  store i32 %11, i32* @brake_acc_nodiv_ctrl_B_local_ABS_RL_Wheel_Threshold_10kmh, align 4
  br label %if.end17

if.end17:                                         ; preds = %if.else16, %if.end15
  %12 = load i32, i32* @RT2_Buffer0, align 4
  store i32 %12, i32* @brake_acc_nodiv_ctrl_B_local_RT2, align 4
  store i32 10, i32* @RT10, align 4
  ret i8* undef
}

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %t0 = alloca i64, align 8
  %t1 = alloca i64, align 8
  %t2 = alloca i64, align 8
  %t3 = alloca i64, align 8
  %t4 = alloca i64, align 8
  store i32 0, i32* @test, align 4
  %call = call i32 @pthread_create(i64* %t0, %union.pthread_attr_t* null, i8* (i8*)* @task_compute, i8* null) #4
  %call1 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @task_RR_Wheel, i8* null) #4
  %call2 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @task_FL_Wheel, i8* null) #4
  %call3 = call i32 @pthread_create(i64* %t3, %union.pthread_attr_t* null, i8* (i8*)* @task_FR_Wheel, i8* null) #4
  %call4 = call i32 @pthread_create(i64* %t4, %union.pthread_attr_t* null, i8* (i8*)* @task_RL_Wheel, i8* null) #4
  ret i32 0
}

; Function Attrs: nounwind
declare i32 @pthread_create(i64*, %union.pthread_attr_t*, i8* (i8*)*, i8*) #2

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noreturn nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { noreturn nounwind }
attributes #4 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0 (tags/RELEASE_380/final)"}
