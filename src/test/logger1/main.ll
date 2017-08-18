; ModuleID = 'main2.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%union.pthread_attr_t = type { i64, [48 x i8] }

@__CPROVER_thread_priorities = constant [2 x i32] [i32 5, i32 2], align 4
@.str = private unnamed_addr constant [20 x i8] c"c::task_communicate\00", align 1
@.str.1 = private unnamed_addr constant [16 x i8] c"c::task_measure\00", align 1
@__CPROVER_threads = global [2 x i8*] [i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.1, i32 0, i32 0)], align 16
@systemState = common global i32 0, align 4
@numberOfRecords = common global i32 0, align 4
@intervalCounter = common global i32 0, align 4
@tickCounter = common global i32 0, align 4
@startTime = common global i32 0, align 4
@interval = common global i32 0, align 4
@records = common global i32 0, align 4
@cmd = common global i32 0, align 4
@rspStatus = common global i32 0, align 4
@decodingStatus = common global i32 0, align 4
@sendit = common global i32 0, align 4
@.str.2 = private unnamed_addr constant [2 x i8] c"0\00", align 1
@.str.3 = private unnamed_addr constant [7 x i8] c"main.c\00", align 1
@__PRETTY_FUNCTION__.task_communicate = private unnamed_addr constant [31 x i8] c"void *task_communicate(void *)\00", align 1
@msgBuffer = common global [64 x i32] zeroinitializer, align 16

; Function Attrs: nounwind uwtable
define void @stop() #0 {
entry:
  store i32 0, i32* @systemState, align 4
  ret void
}

; Function Attrs: nounwind uwtable
define void @start(i32 %_startTime, i32 %_interval) #0 {
entry:
  %0 = load i32, i32* @numberOfRecords, align 4
  %add = add nsw i32 %0, 1
  %cmp = icmp sge i32 %add, 64
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 0, i32* @systemState, align 4
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  store i32 0, i32* @intervalCounter, align 4
  store i32 0, i32* @tickCounter, align 4
  store i32 0, i32* @numberOfRecords, align 4
  store i32 %_startTime, i32* @startTime, align 4
  store i32 %_interval, i32* @interval, align 4
  store i32 1, i32* @systemState, align 4
  ret void
}

; Function Attrs: nounwind uwtable
define i8* @task_measure(i8* %unused) #0 {
entry:
  %0 = load i32, i32* @tickCounter, align 4
  %inc = add nsw i32 %0, 1
  store i32 %inc, i32* @tickCounter, align 4
  %1 = load i32, i32* @tickCounter, align 4
  %2 = load i32, i32* @interval, align 4
  store i32 0, i32* @tickCounter, align 4
  %3 = load i32, i32* @intervalCounter, align 4
  %inc1 = add nsw i32 %3, 1
  store i32 %inc1, i32* @intervalCounter, align 4
  store i32 1, i32* @records, align 4
  %4 = load i32, i32* @numberOfRecords, align 4
  %inc2 = add nsw i32 %4, 1
  store i32 %inc2, i32* @numberOfRecords, align 4
  ret i8* undef
}

; Function Attrs: nounwind uwtable
define i8* @task_communicate(i8* %unused) #0 {
entry:
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 4
  %add = add nsw i32 %rem, 1
  store i32 %add, i32* @cmd, align 4
  %call1 = call i32 @rand() #3
  %rem2 = srem i32 %call1, 2
  %add3 = add nsw i32 %rem2, 1
  store i32 %add3, i32* @rspStatus, align 4
  store i32 0, i32* @decodingStatus, align 4
  %0 = load i32, i32* @cmd, align 4
  %cmp = icmp eq i32 %0, 2
  br i1 %cmp, label %if.then, label %lor.lhs.false

lor.lhs.false:                                    ; preds = %entry
  %1 = load i32, i32* @cmd, align 4
  %cmp4 = icmp eq i32 %1, 3
  br i1 %cmp4, label %if.then, label %lor.lhs.false5

lor.lhs.false5:                                   ; preds = %lor.lhs.false
  %2 = load i32, i32* @cmd, align 4
  %cmp6 = icmp eq i32 %2, 4
  br i1 %cmp6, label %if.then, label %lor.lhs.false7

lor.lhs.false7:                                   ; preds = %lor.lhs.false5
  %3 = load i32, i32* @cmd, align 4
  %cmp8 = icmp eq i32 %3, 1
  br i1 %cmp8, label %if.then, label %if.end

if.then:                                          ; preds = %lor.lhs.false7, %lor.lhs.false5, %lor.lhs.false, %entry
  store i32 1, i32* @decodingStatus, align 4
  br label %if.end

if.end:                                           ; preds = %if.then, %lor.lhs.false7
  %4 = load i32, i32* @rspStatus, align 4
  %cmp9 = icmp eq i32 %4, 1
  br i1 %cmp9, label %if.then10, label %if.end37

if.then10:                                        ; preds = %if.end
  %5 = load i32, i32* @cmd, align 4
  %cmp11 = icmp eq i32 %5, 2
  br i1 %cmp11, label %if.then12, label %if.end16

if.then12:                                        ; preds = %if.then10
  %6 = load i32, i32* @systemState, align 4
  %cmp13 = icmp ne i32 %6, 1
  br i1 %cmp13, label %if.then14, label %if.else

if.then14:                                        ; preds = %if.then12
  store i32 0, i32* @rspStatus, align 4
  br label %if.end15

if.else:                                          ; preds = %if.then12
  store i32 0, i32* @systemState, align 4
  br label %if.end15

if.end15:                                         ; preds = %if.else, %if.then14
  store i32 1, i32* @sendit, align 4
  br label %if.end16

if.end16:                                         ; preds = %if.end15, %if.then10
  %7 = load i32, i32* @cmd, align 4
  %cmp17 = icmp eq i32 %7, 3
  br i1 %cmp17, label %if.then18, label %if.end19

if.then18:                                        ; preds = %if.end16
  store i32 1, i32* @sendit, align 4
  br label %if.end19

if.end19:                                         ; preds = %if.then18, %if.end16
  %8 = load i32, i32* @cmd, align 4
  %cmp20 = icmp eq i32 %8, 4
  br i1 %cmp20, label %if.then21, label %if.end22

if.then21:                                        ; preds = %if.end19
  store i32 1, i32* @sendit, align 4
  br label %if.end22

if.end22:                                         ; preds = %if.then21, %if.end19
  %9 = load i32, i32* @cmd, align 4
  %cmp23 = icmp eq i32 %9, 0
  br i1 %cmp23, label %if.then24, label %if.end36

if.then24:                                        ; preds = %if.end22
  %10 = load i32, i32* @systemState, align 4
  %cmp25 = icmp ne i32 %10, 0
  br i1 %cmp25, label %if.then26, label %if.else27

if.then26:                                        ; preds = %if.then24
  store i32 0, i32* @rspStatus, align 4
  br label %if.end35

if.else27:                                        ; preds = %if.then24
  %11 = load i32, i32* @numberOfRecords, align 4
  %add28 = add nsw i32 %11, 1
  %cmp29 = icmp sge i32 %add28, 64
  br i1 %cmp29, label %if.then30, label %if.end31

if.then30:                                        ; preds = %if.else27
  store i32 0, i32* @systemState, align 4
  br label %if.end31

if.end31:                                         ; preds = %if.then30, %if.else27
  store i32 0, i32* @intervalCounter, align 4
  store i32 0, i32* @tickCounter, align 4
  store i32 0, i32* @numberOfRecords, align 4
  store i32 1, i32* @startTime, align 4
  store i32 1, i32* @interval, align 4
  store i32 1, i32* @systemState, align 4
  %12 = load i32, i32* @numberOfRecords, align 4
  %cmp32 = icmp ne i32 %12, 0
  br i1 %cmp32, label %if.then33, label %if.end34

if.then33:                                        ; preds = %if.end31
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.3, i32 0, i32 0), i32 142, i8* getelementptr inbounds ([31 x i8], [31 x i8]* @__PRETTY_FUNCTION__.task_communicate, i32 0, i32 0)) #4
  unreachable

if.end34:                                         ; preds = %if.end31
  store i32 1, i32* @sendit, align 4
  br label %if.end35

if.end35:                                         ; preds = %if.end34, %if.then26
  br label %if.end36

if.end36:                                         ; preds = %if.end35, %if.end22
  br label %if.end37

if.end37:                                         ; preds = %if.end36, %if.end
  ret i8* undef
}

; Function Attrs: nounwind
declare i32 @rand() #1

; Function Attrs: noreturn nounwind
declare void @__assert_fail(i8*, i8*, i32, i8*) #2

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %t1 = alloca i64, align 8
  %t2 = alloca i64, align 8
  store i32 0, i32* @systemState, align 4
  %call = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @task_communicate, i8* null) #3
  %call1 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @task_measure, i8* null) #3
  ret i32 0
}

; Function Attrs: nounwind
declare i32 @pthread_create(i64*, %union.pthread_attr_t*, i8* (i8*)*, i8*) #1

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { noreturn nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }
attributes #4 = { noreturn nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0 (tags/RELEASE_380/final)"}
