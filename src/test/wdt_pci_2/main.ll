; ModuleID = 'main2.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%union.pthread_attr_t = type { i64, [48 x i8] }

@heartbeat = global i32 60, align 4
@nowayout = global i32 0, align 4
@wdtpci_write_buf = common global i8 0, align 1
@expect_close = common global i32 0, align 4
@count = common global i32 0, align 4
@.str = private unnamed_addr constant [2 x i8] c"0\00", align 1
@.str.1 = private unnamed_addr constant [7 x i8] c"main.c\00", align 1
@__PRETTY_FUNCTION__.closer1 = private unnamed_addr constant [22 x i8] c"void *closer1(void *)\00", align 1
@wdtpci_lock = common global i8 0, align 1
@wdt_dc_port = common global i8 0, align 1
@__PRETTY_FUNCTION__.closer2 = private unnamed_addr constant [22 x i8] c"void *closer2(void *)\00", align 1
@__PRETTY_FUNCTION__.closer3 = private unnamed_addr constant [22 x i8] c"void *closer3(void *)\00", align 1
@jiffies = common global i64 0, align 8
@dev_count = common global i32 0, align 4
@open_sem = common global i32 0, align 4
@io = common global i32 0, align 4
@irq = common global i32 0, align 4
@wd_heartbeat = common global i32 0, align 4
@cnt1 = common global i32 0, align 4
@cnt2 = common global i32 0, align 4
@cnt3 = common global i32 0, align 4
@cnt4 = common global i32 0, align 4
@cnt5 = common global i32 0, align 4
@cnt6 = common global i32 0, align 4
@buf = common global i32 0, align 4

; Function Attrs: nounwind uwtable
define i8* @closer1(i8* %unused) #0 {
entry:
  store i8 86, i8* @wdtpci_write_buf, align 1
  store i32 42, i32* @expect_close, align 4
  store i32 1, i32* @count, align 4
  %0 = load i32, i32* @count, align 4
  %tobool = icmp ne i32 %0, 0
  br i1 %tobool, label %if.then, label %if.end6

if.then:                                          ; preds = %entry
  %1 = load i32, i32* @nowayout, align 4
  %tobool1 = icmp ne i32 %1, 0
  br i1 %tobool1, label %if.end5, label %if.then2

if.then2:                                         ; preds = %if.then
  %2 = load i8, i8* @wdtpci_write_buf, align 1
  %conv = sext i8 %2 to i32
  %cmp = icmp ne i32 %conv, 86
  br i1 %cmp, label %if.then4, label %if.end

if.then4:                                         ; preds = %if.then2
  store i32 0, i32* @expect_close, align 4
  br label %if.end

if.end:                                           ; preds = %if.then4, %if.then2
  br label %if.end5

if.end5:                                          ; preds = %if.end, %if.then
  br label %if.end6

if.end6:                                          ; preds = %if.end5, %entry
  %3 = load i32, i32* @expect_close, align 4
  %cmp7 = icmp ne i32 %3, 42
  br i1 %cmp7, label %if.then9, label %if.else

if.then9:                                         ; preds = %if.end6
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 969, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.closer1, i32 0, i32 0)) #4
  unreachable

if.else:                                          ; preds = %if.end6
  br label %do.body

do.body:                                          ; preds = %if.else
  br label %do.body10

do.body10:                                        ; preds = %do.body
  br label %while.cond

while.cond:                                       ; preds = %while.body, %do.body10
  %4 = load i8, i8* @wdtpci_lock, align 1
  %tobool11 = trunc i8 %4 to i1
  br i1 %tobool11, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  br label %while.cond

while.end:                                        ; preds = %while.cond
  br label %do.end

do.end:                                           ; preds = %while.end
  %5 = load i8, i8* @wdt_dc_port, align 1
  br label %do.body12

do.body12:                                        ; preds = %do.end
  br label %do.end13

do.end13:                                         ; preds = %do.body12
  br label %do.body14

do.body14:                                        ; preds = %do.end13
  store i8 0, i8* @wdtpci_lock, align 1
  br label %do.end15

do.end15:                                         ; preds = %do.body14
  br label %do.end16

do.end16:                                         ; preds = %do.end15
  br label %if.end17

if.end17:                                         ; preds = %do.end16
  store i32 0, i32* @expect_close, align 4
  ret i8* null
}

; Function Attrs: noreturn nounwind
declare void @__assert_fail(i8*, i8*, i32, i8*) #1

; Function Attrs: nounwind uwtable
define i8* @closer2(i8* %unused) #0 {
entry:
  store i8 86, i8* @wdtpci_write_buf, align 1
  store i32 42, i32* @expect_close, align 4
  store i32 1, i32* @count, align 4
  %0 = load i32, i32* @count, align 4
  %tobool = icmp ne i32 %0, 0
  br i1 %tobool, label %if.then, label %if.end6

if.then:                                          ; preds = %entry
  %1 = load i32, i32* @nowayout, align 4
  %tobool1 = icmp ne i32 %1, 0
  br i1 %tobool1, label %if.end5, label %if.then2

if.then2:                                         ; preds = %if.then
  %2 = load i8, i8* @wdtpci_write_buf, align 1
  %conv = sext i8 %2 to i32
  %cmp = icmp ne i32 %conv, 86
  br i1 %cmp, label %if.then4, label %if.end

if.then4:                                         ; preds = %if.then2
  store i32 0, i32* @expect_close, align 4
  br label %if.end

if.end:                                           ; preds = %if.then4, %if.then2
  br label %if.end5

if.end5:                                          ; preds = %if.end, %if.then
  br label %if.end6

if.end6:                                          ; preds = %if.end5, %entry
  %3 = load i32, i32* @expect_close, align 4
  %cmp7 = icmp ne i32 %3, 42
  br i1 %cmp7, label %if.then9, label %if.else

if.then9:                                         ; preds = %if.end6
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 1002, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.closer2, i32 0, i32 0)) #4
  unreachable

if.else:                                          ; preds = %if.end6
  br label %do.body

do.body:                                          ; preds = %if.else
  br label %do.body10

do.body10:                                        ; preds = %do.body
  br label %while.cond

while.cond:                                       ; preds = %while.body, %do.body10
  %4 = load i8, i8* @wdtpci_lock, align 1
  %tobool11 = trunc i8 %4 to i1
  br i1 %tobool11, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  br label %while.cond

while.end:                                        ; preds = %while.cond
  br label %do.end

do.end:                                           ; preds = %while.end
  %5 = load i8, i8* @wdt_dc_port, align 1
  br label %do.body12

do.body12:                                        ; preds = %do.end
  br label %do.end13

do.end13:                                         ; preds = %do.body12
  br label %do.body14

do.body14:                                        ; preds = %do.end13
  store i8 0, i8* @wdtpci_lock, align 1
  br label %do.end15

do.end15:                                         ; preds = %do.body14
  br label %do.end16

do.end16:                                         ; preds = %do.end15
  br label %if.end17

if.end17:                                         ; preds = %do.end16
  store i32 0, i32* @expect_close, align 4
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @closer3(i8* %unused) #0 {
entry:
  store i8 86, i8* @wdtpci_write_buf, align 1
  store i32 42, i32* @expect_close, align 4
  store i32 1, i32* @count, align 4
  %0 = load i32, i32* @count, align 4
  %tobool = icmp ne i32 %0, 0
  br i1 %tobool, label %if.then, label %if.end6

if.then:                                          ; preds = %entry
  %1 = load i32, i32* @nowayout, align 4
  %tobool1 = icmp ne i32 %1, 0
  br i1 %tobool1, label %if.end5, label %if.then2

if.then2:                                         ; preds = %if.then
  %2 = load i8, i8* @wdtpci_write_buf, align 1
  %conv = sext i8 %2 to i32
  %cmp = icmp ne i32 %conv, 86
  br i1 %cmp, label %if.then4, label %if.end

if.then4:                                         ; preds = %if.then2
  store i32 0, i32* @expect_close, align 4
  br label %if.end

if.end:                                           ; preds = %if.then4, %if.then2
  br label %if.end5

if.end5:                                          ; preds = %if.end, %if.then
  br label %if.end6

if.end6:                                          ; preds = %if.end5, %entry
  %3 = load i32, i32* @expect_close, align 4
  %cmp7 = icmp ne i32 %3, 42
  br i1 %cmp7, label %if.then9, label %if.else

if.then9:                                         ; preds = %if.end6
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 1036, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.closer3, i32 0, i32 0)) #4
  unreachable

if.else:                                          ; preds = %if.end6
  br label %do.body

do.body:                                          ; preds = %if.else
  br label %do.body10

do.body10:                                        ; preds = %do.body
  br label %while.cond

while.cond:                                       ; preds = %while.body, %do.body10
  %4 = load i8, i8* @wdtpci_lock, align 1
  %tobool11 = trunc i8 %4 to i1
  br i1 %tobool11, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  br label %while.cond

while.end:                                        ; preds = %while.cond
  br label %do.end

do.end:                                           ; preds = %while.end
  %5 = load i8, i8* @wdt_dc_port, align 1
  br label %do.body12

do.body12:                                        ; preds = %do.end
  br label %do.end13

do.end13:                                         ; preds = %do.body12
  br label %do.body14

do.body14:                                        ; preds = %do.end13
  store i8 0, i8* @wdtpci_lock, align 1
  br label %do.end15

do.end15:                                         ; preds = %do.body14
  br label %do.end16

do.end16:                                         ; preds = %do.end15
  br label %if.end17

if.end17:                                         ; preds = %do.end16
  store i32 0, i32* @expect_close, align 4
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer1(i8* %unused) #0 {
entry:
  store i32 0, i32* @count, align 4
  store i32 0, i32* @expect_close, align 4
  %0 = load i32, i32* @count, align 4
  %tobool = icmp ne i32 %0, 0
  br i1 %tobool, label %if.then, label %if.end6

if.then:                                          ; preds = %entry
  %1 = load i32, i32* @nowayout, align 4
  %tobool1 = icmp ne i32 %1, 0
  br i1 %tobool1, label %if.end5, label %if.then2

if.then2:                                         ; preds = %if.then
  %2 = load i8, i8* @wdtpci_write_buf, align 1
  %conv = sext i8 %2 to i32
  %cmp = icmp ne i32 %conv, 86
  br i1 %cmp, label %if.then4, label %if.end

if.then4:                                         ; preds = %if.then2
  store i32 0, i32* @expect_close, align 4
  br label %if.end

if.end:                                           ; preds = %if.then4, %if.then2
  br label %if.end5

if.end5:                                          ; preds = %if.end, %if.then
  br label %if.end6

if.end6:                                          ; preds = %if.end5, %entry
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer2(i8* %unused) #0 {
entry:
  store i32 0, i32* @count, align 4
  store i32 0, i32* @expect_close, align 4
  %0 = load i32, i32* @count, align 4
  %tobool = icmp ne i32 %0, 0
  br i1 %tobool, label %if.then, label %if.end6

if.then:                                          ; preds = %entry
  %1 = load i32, i32* @nowayout, align 4
  %tobool1 = icmp ne i32 %1, 0
  br i1 %tobool1, label %if.end5, label %if.then2

if.then2:                                         ; preds = %if.then
  %2 = load i8, i8* @wdtpci_write_buf, align 1
  %conv = sext i8 %2 to i32
  %cmp = icmp ne i32 %conv, 86
  br i1 %cmp, label %if.then4, label %if.end

if.then4:                                         ; preds = %if.then2
  store i32 0, i32* @expect_close, align 4
  br label %if.end

if.end:                                           ; preds = %if.then4, %if.then2
  br label %if.end5

if.end5:                                          ; preds = %if.end, %if.then
  br label %if.end6

if.end6:                                          ; preds = %if.end5, %entry
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer3(i8* %unused) #0 {
entry:
  store i32 0, i32* @count, align 4
  store i32 0, i32* @expect_close, align 4
  %0 = load i32, i32* @count, align 4
  %tobool = icmp ne i32 %0, 0
  br i1 %tobool, label %if.then, label %if.end6

if.then:                                          ; preds = %entry
  %1 = load i32, i32* @nowayout, align 4
  %tobool1 = icmp ne i32 %1, 0
  br i1 %tobool1, label %if.end5, label %if.then2

if.then2:                                         ; preds = %if.then
  %2 = load i8, i8* @wdtpci_write_buf, align 1
  %conv = sext i8 %2 to i32
  %cmp = icmp ne i32 %conv, 86
  br i1 %cmp, label %if.then4, label %if.end

if.then4:                                         ; preds = %if.then2
  store i32 0, i32* @expect_close, align 4
  br label %if.end

if.end:                                           ; preds = %if.then4, %if.then2
  br label %if.end5

if.end5:                                          ; preds = %if.end, %if.then
  br label %if.end6

if.end6:                                          ; preds = %if.end5, %entry
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
define i8* @writer23(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer24(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer25(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer26(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer27(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer28(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer29(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.end

do.end:                                           ; preds = %do.body
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %t1 = alloca i64, align 8
  %call = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer1, i8* null) #5
  %call1 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer2, i8* null) #5
  %call2 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer3, i8* null) #5
  %call3 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @closer1, i8* null) #5
  %call4 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @closer2, i8* null) #5
  %call5 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @closer3, i8* null) #5
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
