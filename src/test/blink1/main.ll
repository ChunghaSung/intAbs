; ModuleID = 'main2.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%union.pthread_attr_t = type { i64, [48 x i8] }

@timerCount = global i32 0, align 4
@.str = private unnamed_addr constant [2 x i8] c"0\00", align 1
@.str.1 = private unnamed_addr constant [7 x i8] c"main.c\00", align 1
@__PRETTY_FUNCTION__.Timer_A = private unnamed_addr constant [22 x i8] c"void *Timer_A(void *)\00", align 1
@LED_0 = common global i32 0, align 4
@LED_1 = common global i32 0, align 4
@P1OUT = common global i32 0, align 4
@__PRETTY_FUNCTION__.Timer_B = private unnamed_addr constant [22 x i8] c"void *Timer_B(void *)\00", align 1
@__PRETTY_FUNCTION__.Timer_Force = private unnamed_addr constant [26 x i8] c"void *Timer_Force(void *)\00", align 1
@WDTPW = common global i32 0, align 4
@WDTHOLD = common global i32 0, align 4
@WDTCTL = common global i32 0, align 4
@LED_DIR = common global i32 0, align 4
@LED_OUT = common global i32 0, align 4
@CCIE = common global i32 0, align 4
@CCTL0 = common global i32 0, align 4
@TASSEL_2 = common global i32 0, align 4
@MC_2 = common global i32 0, align 4
@TACTL = common global i32 0, align 4
@WDTCLT = common global i32 0, align 4
@cnt1 = common global i32 0, align 4
@cnt2 = common global i32 0, align 4
@cnt3 = common global i32 0, align 4

; Function Attrs: nounwind uwtable
define i8* @Timer_A(i8* %unused) #0 {
entry:
  store i32 1, i32* @timerCount, align 4
  %0 = load i32, i32* @timerCount, align 4
  %cmp = icmp ne i32 %0, 0
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 100, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.Timer_A, i32 0, i32 0)) #3
  unreachable

if.end:                                           ; preds = %entry
  %1 = load i32, i32* @timerCount, align 4
  %cmp1 = icmp eq i32 %1, 0
  br i1 %cmp1, label %if.then2, label %if.end3

if.then2:                                         ; preds = %if.end
  %2 = load i32, i32* @LED_0, align 4
  %3 = load i32, i32* @LED_1, align 4
  %add = add nsw i32 %2, %3
  store i32 %add, i32* @P1OUT, align 4
  br label %if.end3

if.end3:                                          ; preds = %if.then2, %if.end
  ret i8* undef
}

; Function Attrs: noreturn nounwind
declare void @__assert_fail(i8*, i8*, i32, i8*) #1

; Function Attrs: nounwind uwtable
define i8* @Timer_B(i8* %unused) #0 {
entry:
  store i32 1, i32* @timerCount, align 4
  %0 = load i32, i32* @timerCount, align 4
  %cmp = icmp ne i32 %0, 0
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 119, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.Timer_B, i32 0, i32 0)) #3
  unreachable

if.end:                                           ; preds = %entry
  %1 = load i32, i32* @timerCount, align 4
  %cmp1 = icmp eq i32 %1, 0
  br i1 %cmp1, label %if.then2, label %if.end3

if.then2:                                         ; preds = %if.end
  %2 = load i32, i32* @LED_0, align 4
  store i32 %2, i32* @P1OUT, align 4
  br label %if.end3

if.end3:                                          ; preds = %if.then2, %if.end
  ret i8* undef
}

; Function Attrs: nounwind uwtable
define i8* @Timer_Force(i8* %unused) #0 {
entry:
  store i32 0, i32* @timerCount, align 4
  %0 = load i32, i32* @timerCount, align 4
  %cmp = icmp ne i32 %0, 0
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 136, i8* getelementptr inbounds ([26 x i8], [26 x i8]* @__PRETTY_FUNCTION__.Timer_Force, i32 0, i32 0)) #3
  unreachable

if.end:                                           ; preds = %entry
  store i32 1, i32* @timerCount, align 4
  ret i8* undef
}

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %t1 = alloca i64, align 8
  %t2 = alloca i64, align 8
  %t3 = alloca i64, align 8
  %0 = load i32, i32* @WDTPW, align 4
  %1 = load i32, i32* @WDTHOLD, align 4
  %add = add nsw i32 %0, %1
  store i32 %add, i32* @WDTCTL, align 4
  %2 = load i32, i32* @LED_0, align 4
  %3 = load i32, i32* @LED_1, align 4
  %add1 = add nsw i32 %2, %3
  store i32 %add1, i32* @LED_DIR, align 4
  %4 = load i32, i32* @LED_0, align 4
  %5 = load i32, i32* @LED_1, align 4
  %add2 = add nsw i32 %4, %5
  %add3 = add nsw i32 1, %add2
  store i32 %add3, i32* @LED_OUT, align 4
  %6 = load i32, i32* @CCIE, align 4
  store i32 %6, i32* @CCTL0, align 4
  %7 = load i32, i32* @TASSEL_2, align 4
  %8 = load i32, i32* @MC_2, align 4
  %add4 = add nsw i32 %7, %8
  store i32 %add4, i32* @TACTL, align 4
  store i32 0, i32* @timerCount, align 4
  %call = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @Timer_A, i8* null) #4
  %call5 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @Timer_B, i8* null) #4
  %call6 = call i32 @pthread_create(i64* %t3, %union.pthread_attr_t* null, i8* (i8*)* @Timer_Force, i8* null) #4
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
