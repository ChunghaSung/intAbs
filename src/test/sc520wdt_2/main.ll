; ModuleID = 'main2.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%union.pthread_attr_t = type { i64, [48 x i8] }

@timeout = global i32 30, align 4
@nowayout = global i32 0, align 4
@wdt_spinlock_init = global i8 0, align 1
@dev_open = global i8 0, align 1
@.str = private unnamed_addr constant [9 x i8] c"dev_open\00", align 1
@.str.1 = private unnamed_addr constant [7 x i8] c"main.c\00", align 1
@__PRETTY_FUNCTION__.writer1 = private unnamed_addr constant [22 x i8] c"void *writer1(void *)\00", align 1
@.str.2 = private unnamed_addr constant [2 x i8] c"0\00", align 1
@jiffies = common global i64 0, align 8
@next_heartbeat = common global i64 0, align 8
@__PRETTY_FUNCTION__.writer2 = private unnamed_addr constant [22 x i8] c"void *writer2(void *)\00", align 1
@__PRETTY_FUNCTION__.writer3 = private unnamed_addr constant [22 x i8] c"void *writer3(void *)\00", align 1
@__PRETTY_FUNCTION__.writer4 = private unnamed_addr constant [22 x i8] c"void *writer4(void *)\00", align 1
@__PRETTY_FUNCTION__.writer5 = private unnamed_addr constant [22 x i8] c"void *writer5(void *)\00", align 1
@__PRETTY_FUNCTION__.writer6 = private unnamed_addr constant [22 x i8] c"void *writer6(void *)\00", align 1
@__PRETTY_FUNCTION__.writer7 = private unnamed_addr constant [22 x i8] c"void *writer7(void *)\00", align 1
@__PRETTY_FUNCTION__.writer8 = private unnamed_addr constant [22 x i8] c"void *writer8(void *)\00", align 1
@__PRETTY_FUNCTION__.writer9 = private unnamed_addr constant [22 x i8] c"void *writer9(void *)\00", align 1
@__PRETTY_FUNCTION__.writer10 = private unnamed_addr constant [23 x i8] c"void *writer10(void *)\00", align 1
@__PRETTY_FUNCTION__.writer11 = private unnamed_addr constant [23 x i8] c"void *writer11(void *)\00", align 1
@__PRETTY_FUNCTION__.writer12 = private unnamed_addr constant [23 x i8] c"void *writer12(void *)\00", align 1
@__PRETTY_FUNCTION__.writer13 = private unnamed_addr constant [23 x i8] c"void *writer13(void *)\00", align 1
@__PRETTY_FUNCTION__.writer14 = private unnamed_addr constant [23 x i8] c"void *writer14(void *)\00", align 1
@__PRETTY_FUNCTION__.writer15 = private unnamed_addr constant [23 x i8] c"void *writer15(void *)\00", align 1
@__PRETTY_FUNCTION__.writer16 = private unnamed_addr constant [23 x i8] c"void *writer16(void *)\00", align 1
@__PRETTY_FUNCTION__.writer17 = private unnamed_addr constant [23 x i8] c"void *writer17(void *)\00", align 1
@__PRETTY_FUNCTION__.writer18 = private unnamed_addr constant [23 x i8] c"void *writer18(void *)\00", align 1
@__PRETTY_FUNCTION__.writer19 = private unnamed_addr constant [23 x i8] c"void *writer19(void *)\00", align 1
@__PRETTY_FUNCTION__.writer20 = private unnamed_addr constant [23 x i8] c"void *writer20(void *)\00", align 1
@__PRETTY_FUNCTION__.writer21 = private unnamed_addr constant [23 x i8] c"void *writer21(void *)\00", align 1
@__PRETTY_FUNCTION__.writer22 = private unnamed_addr constant [23 x i8] c"void *writer22(void *)\00", align 1
@__PRETTY_FUNCTION__.writer23 = private unnamed_addr constant [23 x i8] c"void *writer23(void *)\00", align 1
@__PRETTY_FUNCTION__.writer24 = private unnamed_addr constant [23 x i8] c"void *writer24(void *)\00", align 1
@__PRETTY_FUNCTION__.writer25 = private unnamed_addr constant [23 x i8] c"void *writer25(void *)\00", align 1
@__PRETTY_FUNCTION__.writer26 = private unnamed_addr constant [23 x i8] c"void *writer26(void *)\00", align 1
@__PRETTY_FUNCTION__.writer27 = private unnamed_addr constant [23 x i8] c"void *writer27(void *)\00", align 1
@__PRETTY_FUNCTION__.writer28 = private unnamed_addr constant [23 x i8] c"void *writer28(void *)\00", align 1
@__PRETTY_FUNCTION__.writer29 = private unnamed_addr constant [23 x i8] c"void *writer29(void *)\00", align 1
@__PRETTY_FUNCTION__.writer30 = private unnamed_addr constant [23 x i8] c"void *writer30(void *)\00", align 1
@__PRETTY_FUNCTION__.writer31 = private unnamed_addr constant [23 x i8] c"void *writer31(void *)\00", align 1
@__PRETTY_FUNCTION__.writer32 = private unnamed_addr constant [23 x i8] c"void *writer32(void *)\00", align 1
@__PRETTY_FUNCTION__.writer33 = private unnamed_addr constant [23 x i8] c"void *writer33(void *)\00", align 1
@__PRETTY_FUNCTION__.writer34 = private unnamed_addr constant [23 x i8] c"void *writer34(void *)\00", align 1
@__PRETTY_FUNCTION__.writer35 = private unnamed_addr constant [23 x i8] c"void *writer35(void *)\00", align 1
@__PRETTY_FUNCTION__.writer36 = private unnamed_addr constant [23 x i8] c"void *writer36(void *)\00", align 1
@__PRETTY_FUNCTION__.writer37 = private unnamed_addr constant [23 x i8] c"void *writer37(void *)\00", align 1
@__PRETTY_FUNCTION__.writer38 = private unnamed_addr constant [23 x i8] c"void *writer38(void *)\00", align 1
@__PRETTY_FUNCTION__.writer39 = private unnamed_addr constant [23 x i8] c"void *writer39(void *)\00", align 1
@fops_write_buf = common global i8 0, align 1
@__PRETTY_FUNCTION__.closer = private unnamed_addr constant [21 x i8] c"void *closer(void *)\00", align 1
@wdt_expect_close = common global i8 0, align 1
@wdt_spinlock = common global i8 0, align 1
@wdtmrctl = common global i64 0, align 8
@wdt_is_open = common global i64 0, align 8

; Function Attrs: nounwind uwtable
define i8* @writer1(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 635, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.writer1, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 635, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.writer1, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: noreturn nounwind
declare void @__assert_fail(i8*, i8*, i32, i8*) #1

; Function Attrs: nounwind uwtable
define i8* @writer2(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 640, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.writer2, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 640, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.writer2, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer3(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 645, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.writer3, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 645, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.writer3, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer4(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 650, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.writer4, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 650, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.writer4, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer5(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 655, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.writer5, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 655, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.writer5, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer6(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 660, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.writer6, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 660, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.writer6, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer7(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 665, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.writer7, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 665, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.writer7, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer8(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 670, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.writer8, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 670, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.writer8, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer9(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 675, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.writer9, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 675, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.writer9, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer10(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 680, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer10, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 680, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer10, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer11(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 685, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer11, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 685, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer11, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer12(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 690, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer12, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 690, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer12, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer13(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 695, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer13, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 695, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer13, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer14(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 700, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer14, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 700, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer14, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer15(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 705, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer15, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 705, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer15, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer16(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 710, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer16, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 710, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer16, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer17(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 715, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer17, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 715, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer17, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer18(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 720, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer18, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 720, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer18, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer19(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 725, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer19, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 725, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer19, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer20(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 730, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer20, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 730, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer20, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer21(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 735, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer21, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 735, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer21, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer22(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 740, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer22, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 740, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer22, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer23(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 745, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer23, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 745, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer23, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer24(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 750, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer24, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 750, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer24, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer25(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 755, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer25, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 755, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer25, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer26(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 760, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer26, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 760, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer26, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer27(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 765, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer27, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 765, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer27, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer28(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 770, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer28, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 770, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer28, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer29(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 775, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer29, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 775, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer29, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer30(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 780, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer30, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 780, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer30, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer31(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 785, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer31, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 785, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer31, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer32(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 790, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer32, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 790, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer32, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer33(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 795, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer33, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 795, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer33, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer34(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 800, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer34, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 800, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer34, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer35(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 805, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer35, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 805, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer35, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer36(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 810, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer36, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 810, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer36, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer37(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 815, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer37, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 815, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer37, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer38(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 820, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer38, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 820, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer38, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer39(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 825, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer39, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  br label %do.body1

do.body1:                                         ; preds = %cond.end
  %2 = load i8, i8* @dev_open, align 1
  %tobool2 = trunc i8 %2 to i1
  br i1 %tobool2, label %if.end, label %if.then

if.then:                                          ; preds = %do.body1
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 825, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @__PRETTY_FUNCTION__.writer39, i32 0, i32 0)) #4
  unreachable

if.end:                                           ; preds = %do.body1
  %3 = load i64, i64* @jiffies, align 8
  %4 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %4, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %3, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end
  br label %do.end3

do.end3:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @closer(i8* %unused) #0 {
entry:
  store i8 86, i8* @fops_write_buf, align 1
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i8, i8* @dev_open, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %cond.true, label %cond.false

cond.true:                                        ; preds = %do.body
  br label %cond.end

cond.false:                                       ; preds = %do.body
  call void @__assert_fail(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 831, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @__PRETTY_FUNCTION__.closer, i32 0, i32 0)) #4
  unreachable
                                                  ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %1, %cond.true
  %2 = load i32, i32* @nowayout, align 4
  %tobool1 = icmp ne i32 %2, 0
  br i1 %tobool1, label %if.end4, label %if.then

if.then:                                          ; preds = %cond.end
  store i8 0, i8* @wdt_expect_close, align 1
  %3 = load i8, i8* @fops_write_buf, align 1
  %conv = sext i8 %3 to i32
  %cmp = icmp eq i32 %conv, 86
  br i1 %cmp, label %if.then3, label %if.end

if.then3:                                         ; preds = %if.then
  store i8 42, i8* @wdt_expect_close, align 1
  br label %if.end

if.end:                                           ; preds = %if.then3, %if.then
  br label %if.end4

if.end4:                                          ; preds = %if.end, %cond.end
  br label %do.body5

do.body5:                                         ; preds = %if.end4
  %4 = load i8, i8* @dev_open, align 1
  %tobool6 = trunc i8 %4 to i1
  br i1 %tobool6, label %if.end8, label %if.then7

if.then7:                                         ; preds = %do.body5
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 831, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @__PRETTY_FUNCTION__.closer, i32 0, i32 0)) #4
  unreachable

if.end8:                                          ; preds = %do.body5
  %5 = load i64, i64* @jiffies, align 8
  %6 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %6, 1
  %conv9 = sext i32 %mul to i64
  %add = add nsw i64 %5, %conv9
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %if.end8
  br label %do.end10

do.end10:                                         ; preds = %do.end
  br label %do.body11

do.body11:                                        ; preds = %do.end10
  %7 = load i8, i8* @wdt_expect_close, align 1
  %conv12 = sext i8 %7 to i32
  %cmp13 = icmp ne i32 %conv12, 42
  br i1 %cmp13, label %if.then15, label %if.else

if.then15:                                        ; preds = %do.body11
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 832, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @__PRETTY_FUNCTION__.closer, i32 0, i32 0)) #4
  unreachable

if.else:                                          ; preds = %do.body11
  br label %do.body16

do.body16:                                        ; preds = %if.else
  br label %do.body17

do.body17:                                        ; preds = %do.body16
  br label %do.body18

do.body18:                                        ; preds = %do.body17
  br label %while.cond

while.cond:                                       ; preds = %while.body, %do.body18
  %8 = load i8, i8* @wdt_spinlock, align 1
  %tobool19 = trunc i8 %8 to i1
  br i1 %tobool19, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  br label %while.cond

while.end:                                        ; preds = %while.cond
  br label %do.end20

do.end20:                                         ; preds = %while.end
  %9 = load i64, i64* @wdtmrctl, align 8
  br label %do.body21

do.body21:                                        ; preds = %do.end20
  store i64 43690, i64* @wdtmrctl, align 8
  br label %do.end22

do.end22:                                         ; preds = %do.body21
  br label %do.body23

do.body23:                                        ; preds = %do.end22
  store i64 21845, i64* @wdtmrctl, align 8
  br label %do.end24

do.end24:                                         ; preds = %do.body23
  br label %do.body25

do.body25:                                        ; preds = %do.end24
  store i64 13107, i64* @wdtmrctl, align 8
  br label %do.end26

do.end26:                                         ; preds = %do.body25
  br label %do.body27

do.body27:                                        ; preds = %do.end26
  store i64 52428, i64* @wdtmrctl, align 8
  br label %do.end28

do.end28:                                         ; preds = %do.body27
  br label %do.body29

do.body29:                                        ; preds = %do.end28
  store i64 0, i64* @wdtmrctl, align 8
  br label %do.end30

do.end30:                                         ; preds = %do.body29
  br label %do.body31

do.body31:                                        ; preds = %do.end30
  br label %do.end32

do.end32:                                         ; preds = %do.body31
  br label %do.end33

do.end33:                                         ; preds = %do.end32
  br label %do.end34

do.end34:                                         ; preds = %do.end33
  br label %if.end35

if.end35:                                         ; preds = %do.end34
  br label %do.body36

do.body36:                                        ; preds = %if.end35
  store i64 0, i64* @wdt_is_open, align 8
  br label %do.end37

do.end37:                                         ; preds = %do.body36
  store i8 0, i8* @wdt_expect_close, align 1
  br label %do.end38

do.end38:                                         ; preds = %do.end37
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %t1 = alloca i64, align 8
  %t2 = alloca i64, align 8
  br label %do.body

do.body:                                          ; preds = %entry
  store i8 0, i8* @fops_write_buf, align 1
  store i64 0, i64* @jiffies, align 8
  store i8 0, i8* @wdt_expect_close, align 1
  store i8 1, i8* @dev_open, align 1
  store i8 1, i8* @wdt_spinlock_init, align 1
  br label %do.end

do.end:                                           ; preds = %do.body
  %call = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer1, i8* null) #5
  %call1 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer2, i8* null) #5
  %call2 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer3, i8* null) #5
  %call3 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer4, i8* null) #5
  %call4 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer5, i8* null) #5
  %call5 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer6, i8* null) #5
  %call6 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer7, i8* null) #5
  %call7 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer8, i8* null) #5
  %call8 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer9, i8* null) #5
  %call9 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer10, i8* null) #5
  %call10 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer11, i8* null) #5
  %call11 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer12, i8* null) #5
  %call12 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer13, i8* null) #5
  %call13 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer14, i8* null) #5
  %call14 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer15, i8* null) #5
  %call15 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer16, i8* null) #5
  %call16 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer17, i8* null) #5
  %call17 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer18, i8* null) #5
  %call18 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer19, i8* null) #5
  %call19 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer20, i8* null) #5
  %call20 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer21, i8* null) #5
  %call21 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer22, i8* null) #5
  %call22 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer23, i8* null) #5
  %call23 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer24, i8* null) #5
  %call24 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer25, i8* null) #5
  %call25 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer26, i8* null) #5
  %call26 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer27, i8* null) #5
  %call27 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer28, i8* null) #5
  %call28 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer29, i8* null) #5
  %call29 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer30, i8* null) #5
  %call30 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer31, i8* null) #5
  %call31 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer32, i8* null) #5
  %call32 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer33, i8* null) #5
  %call33 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer34, i8* null) #5
  %call34 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer35, i8* null) #5
  %call35 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer36, i8* null) #5
  %call36 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer37, i8* null) #5
  %call37 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer38, i8* null) #5
  %call38 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer39, i8* null) #5
  %call39 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @closer, i8* null) #5
  call void @pthread_exit(i8* null) #6
  unreachable

return:                                           ; No predecessors!
  ret i32 0
}

; Function Attrs: nounwind
declare i32 @pthread_create(i64*, %union.pthread_attr_t*, i8* (i8*)*, i8*) #2

; Function Attrs: noreturn
declare void @pthread_exit(i8*) #3

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noreturn nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { noreturn "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { noreturn nounwind }
attributes #5 = { nounwind }
attributes #6 = { noreturn }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0 (tags/RELEASE_380/final)"}
