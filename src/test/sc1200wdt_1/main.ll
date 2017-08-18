; ModuleID = 'main2.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%union.pthread_attr_t = type { i64, [48 x i8] }

@timeout = global i32 1, align 4
@io = global i32 0, align 4
@io_len = global i32 2, align 4
@nowayout = global i32 0, align 4
@fops_write_buf = common global i8 0, align 1
@expect_close = common global i8 0, align 1
@.str = private unnamed_addr constant [2 x i8] c"0\00", align 1
@.str.1 = private unnamed_addr constant [7 x i8] c"main.c\00", align 1
@__PRETTY_FUNCTION__.closer = private unnamed_addr constant [21 x i8] c"void *closer(void *)\00", align 1
@sc1200wdt_lock = common global i8 0, align 1
@pmir_port = common global i8 0, align 1
@pmdr_port = common global i8 0, align 1
@open_sem = common global i32 0, align 4
@jiffies = common global i64 0, align 8

; Function Attrs: nounwind uwtable
define i8* @writer1(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer2(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer3(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer4(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer5(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer6(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer7(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer8(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer9(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer10(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer11(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer12(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer13(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer14(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer15(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer16(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer17(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer18(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer19(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer20(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer21(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer22(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @closer(i8* %unused) #0 {
entry:
  store i8 86, i8* @fops_write_buf, align 1
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i32, i32* @nowayout, align 4
  %tobool = icmp ne i32 %0, 0
  br i1 %tobool, label %if.end3, label %if.then

if.then:                                          ; preds = %do.body
  store i8 0, i8* @expect_close, align 1
  %1 = load i8, i8* @fops_write_buf, align 1
  %conv = sext i8 %1 to i32
  %cmp = icmp eq i32 %conv, 86
  br i1 %cmp, label %if.then2, label %if.end

if.then2:                                         ; preds = %if.then
  store i8 42, i8* @expect_close, align 1
  br label %if.end

if.end:                                           ; preds = %if.then2, %if.then
  br label %if.end3

if.end3:                                          ; preds = %if.end, %do.body
  br label %do.end

do.end:                                           ; preds = %if.end3
  br label %do.body4

do.body4:                                         ; preds = %do.end
  %2 = load i8, i8* @expect_close, align 1
  %conv5 = sext i8 %2 to i32
  %cmp6 = icmp ne i32 %conv5, 42
  br i1 %cmp6, label %if.then8, label %if.else

if.then8:                                         ; preds = %do.body4
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 682, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @__PRETTY_FUNCTION__.closer, i32 0, i32 0)) #3
  unreachable

if.else:                                          ; preds = %do.body4
  br label %do.body9

do.body9:                                         ; preds = %if.else
  br label %do.body10

do.body10:                                        ; preds = %do.body9
  br label %do.body11

do.body11:                                        ; preds = %do.body10
  br label %while.cond

while.cond:                                       ; preds = %while.body, %do.body11
  %3 = load i8, i8* @sc1200wdt_lock, align 1
  %tobool12 = trunc i8 %3 to i1
  br i1 %tobool12, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  br label %while.cond

while.end:                                        ; preds = %while.cond
  br label %do.end13

do.end13:                                         ; preds = %while.end
  br label %do.body14

do.body14:                                        ; preds = %do.end13
  store i8 5, i8* @pmir_port, align 1
  br label %do.end15

do.end15:                                         ; preds = %do.body14
  br label %do.body16

do.body16:                                        ; preds = %do.end15
  store i8 0, i8* @pmdr_port, align 1
  br label %do.end17

do.end17:                                         ; preds = %do.body16
  br label %do.body18

do.body18:                                        ; preds = %do.end17
  store i8 0, i8* @sc1200wdt_lock, align 1
  br label %do.end19

do.end19:                                         ; preds = %do.body18
  br label %do.end20

do.end20:                                         ; preds = %do.end19
  br label %do.end21

do.end21:                                         ; preds = %do.end20
  br label %if.end22

if.end22:                                         ; preds = %do.end21
  br label %do.body23

do.body23:                                        ; preds = %if.end22
  %4 = load i32, i32* @open_sem, align 4
  %add = add nsw i32 %4, 1
  store i32 %add, i32* @open_sem, align 4
  br label %do.end24

do.end24:                                         ; preds = %do.body23
  store i8 0, i8* @expect_close, align 1
  br label %do.end25

do.end25:                                         ; preds = %do.end24
  ret i8* null
}

; Function Attrs: noreturn nounwind
declare void @__assert_fail(i8*, i8*, i32, i8*) #1

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %t1 = alloca i64, align 8
  %t2 = alloca i64, align 8
  store i8 0, i8* @expect_close, align 1
  store i32 0, i32* @open_sem, align 4
  %call = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer1, i8* null) #4
  %call1 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer2, i8* null) #4
  %call2 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer3, i8* null) #4
  %call3 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer4, i8* null) #4
  %call4 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer5, i8* null) #4
  %call5 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer6, i8* null) #4
  %call6 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer7, i8* null) #4
  %call7 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer8, i8* null) #4
  %call8 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer9, i8* null) #4
  %call9 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer10, i8* null) #4
  %call10 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer11, i8* null) #4
  %call11 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer12, i8* null) #4
  %call12 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer13, i8* null) #4
  %call13 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer14, i8* null) #4
  %call14 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer15, i8* null) #4
  %call15 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer16, i8* null) #4
  %call16 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer17, i8* null) #4
  %call17 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer18, i8* null) #4
  %call18 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer19, i8* null) #4
  %call19 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer20, i8* null) #4
  %call20 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer21, i8* null) #4
  %call21 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer22, i8* null) #4
  %call22 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @closer, i8* null) #4
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
