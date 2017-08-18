; ModuleID = 'main2.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%union.pthread_attr_t = type { i64, [48 x i8] }

@heartbeat = global i32 30, align 4
@nowayout = global i32 0, align 4
@tco_lock = common global i8 0, align 1
@tco_write_buf = common global i8 0, align 1
@tco_expect_close = common global i8 0, align 1
@tco1_rld = common global i8 0, align 1
@.str = private unnamed_addr constant [2 x i8] c"0\00", align 1
@.str.1 = private unnamed_addr constant [7 x i8] c"main.c\00", align 1
@__PRETTY_FUNCTION__.closer1 = private unnamed_addr constant [22 x i8] c"void *closer1(void *)\00", align 1
@tco1_cnt_b4 = common global i8 0, align 1
@timer_alive = common global i64 0, align 8
@__PRETTY_FUNCTION__.closer2 = private unnamed_addr constant [22 x i8] c"void *closer2(void *)\00", align 1
@__PRETTY_FUNCTION__.closer3 = private unnamed_addr constant [22 x i8] c"void *closer3(void *)\00", align 1
@ACPIBASE = common global i32 0, align 4
@tco1_cnt_b0 = common global i8 0, align 1
@tco1_cnt_b1 = common global i8 0, align 1
@tco1_cnt_b2 = common global i8 0, align 1
@tco1_cnt_b3 = common global i8 0, align 1
@tco1_cnt_b5 = common global i8 0, align 1
@tco1_cnt_b6 = common global i8 0, align 1
@tco1_cnt_b7 = common global i8 0, align 1
@cnt1 = common global i32 0, align 4
@cnt2 = common global i32 0, align 4
@cnt3 = common global i32 0, align 4
@cnt4 = common global i32 0, align 4
@cnt5 = common global i32 0, align 4
@cnt6 = common global i32 0, align 4
@cnt7 = common global i32 0, align 4
@cnt8 = common global i32 0, align 4

; Function Attrs: nounwind uwtable
define zeroext i8 @seconds_to_ticks(i32 %seconds) #0 {
entry:
  %mul = mul nsw i32 %seconds, 10
  %div = sdiv i32 %mul, 6
  %conv = trunc i32 %div to i8
  ret i8 %conv
}

; Function Attrs: nounwind uwtable
define i32 @tco_timer_set_heartbeat(i32 %t) #0 {
entry:
  %call = call zeroext i8 @seconds_to_ticks(i32 %t)
  %conv = zext i8 %call to i32
  %cmp = icmp sgt i32 %conv, 63
  br i1 %cmp, label %if.then, label %lor.lhs.false

lor.lhs.false:                                    ; preds = %entry
  %conv2 = zext i8 %call to i32
  %cmp3 = icmp slt i32 %conv2, 4
  br i1 %cmp3, label %if.then, label %if.end

if.then:                                          ; preds = %lor.lhs.false, %entry
  br label %return

if.end:                                           ; preds = %lor.lhs.false
  br label %do.body

do.body:                                          ; preds = %if.end
  br label %while.cond

while.cond:                                       ; preds = %while.body, %do.body
  %0 = load i8, i8* @tco_lock, align 1
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  br label %while.cond

while.end:                                        ; preds = %while.cond
  br label %do.end

do.end:                                           ; preds = %while.end
  %conv5 = zext i8 0 to i32
  %and = and i32 %conv5, 192
  %conv6 = trunc i32 %and to i8
  %conv7 = zext i8 %call to i32
  %conv8 = zext i8 %conv6 to i32
  %or = or i32 %conv8, %conv7
  %conv9 = trunc i32 %or to i8
  br label %do.body10

do.body10:                                        ; preds = %do.end
  store i8 0, i8* @tco_lock, align 1
  br label %do.end11

do.end11:                                         ; preds = %do.body10
  %conv12 = zext i8 0 to i32
  %and13 = and i32 %conv12, 63
  %conv14 = zext i8 %call to i32
  %cmp15 = icmp ne i32 %and13, %conv14
  br i1 %cmp15, label %if.then17, label %if.end18

if.then17:                                        ; preds = %do.end11
  br label %return

if.end18:                                         ; preds = %do.end11
  store i32 %t, i32* @heartbeat, align 4
  br label %return

return:                                           ; preds = %if.end18, %if.then17, %if.then
  %retval.0 = phi i32 [ -22, %if.then ], [ -22, %if.then17 ], [ 0, %if.end18 ]
  ret i32 %retval.0
}

; Function Attrs: nounwind uwtable
define i8* @closer1(i8* %unused) #0 {
entry:
  store i8 86, i8* @tco_write_buf, align 1
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i32, i32* @nowayout, align 4
  %tobool = icmp ne i32 %0, 0
  br i1 %tobool, label %if.end3, label %if.then

if.then:                                          ; preds = %do.body
  store i8 0, i8* @tco_expect_close, align 1
  %1 = load i8, i8* @tco_write_buf, align 1
  %conv = sext i8 %1 to i32
  %cmp = icmp eq i32 %conv, 86
  br i1 %cmp, label %if.then2, label %if.end

if.then2:                                         ; preds = %if.then
  store i8 42, i8* @tco_expect_close, align 1
  br label %if.end

if.end:                                           ; preds = %if.then2, %if.then
  br label %if.end3

if.end3:                                          ; preds = %if.end, %do.body
  br label %do.body4

do.body4:                                         ; preds = %if.end3
  br label %do.body5

do.body5:                                         ; preds = %do.body4
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body5
  br label %do.end6

do.end6:                                          ; preds = %do.end
  br label %do.end7

do.end7:                                          ; preds = %do.end6
  store i8 42, i8* @tco_expect_close, align 1
  br label %do.body8

do.body8:                                         ; preds = %do.end7
  %2 = load i8, i8* @tco_expect_close, align 1
  %conv9 = sext i8 %2 to i32
  %cmp10 = icmp ne i32 %conv9, 42
  br i1 %cmp10, label %if.then12, label %if.else

if.then12:                                        ; preds = %do.body8
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 721, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.closer1, i32 0, i32 0)) #4
  unreachable

if.else:                                          ; preds = %do.body8
  br label %do.body13

do.body13:                                        ; preds = %if.else
  br label %do.body14

do.body14:                                        ; preds = %do.body13
  br label %while.cond

while.cond:                                       ; preds = %while.body, %do.body14
  %3 = load i8, i8* @tco_lock, align 1
  %tobool15 = trunc i8 %3 to i1
  br i1 %tobool15, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  br label %while.cond

while.end:                                        ; preds = %while.cond
  br label %do.end16

do.end16:                                         ; preds = %while.end
  store i8 1, i8* @tco1_cnt_b4, align 1
  br label %do.body17

do.body17:                                        ; preds = %do.end16
  store i8 0, i8* @tco_lock, align 1
  br label %do.end18

do.end18:                                         ; preds = %do.body17
  br label %do.end19

do.end19:                                         ; preds = %do.end18
  br label %if.end20

if.end20:                                         ; preds = %do.end19
  br label %do.body21

do.body21:                                        ; preds = %if.end20
  store i64 0, i64* @timer_alive, align 8
  br label %do.end22

do.end22:                                         ; preds = %do.body21
  store i8 0, i8* @tco_expect_close, align 1
  br label %do.end23

do.end23:                                         ; preds = %do.end22
  ret i8* null
}

; Function Attrs: noreturn nounwind
declare void @__assert_fail(i8*, i8*, i32, i8*) #1

; Function Attrs: nounwind uwtable
define i8* @closer2(i8* %unused) #0 {
entry:
  store i8 86, i8* @tco_write_buf, align 1
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i32, i32* @nowayout, align 4
  %tobool = icmp ne i32 %0, 0
  br i1 %tobool, label %if.end3, label %if.then

if.then:                                          ; preds = %do.body
  store i8 0, i8* @tco_expect_close, align 1
  %1 = load i8, i8* @tco_write_buf, align 1
  %conv = sext i8 %1 to i32
  %cmp = icmp eq i32 %conv, 86
  br i1 %cmp, label %if.then2, label %if.end

if.then2:                                         ; preds = %if.then
  store i8 42, i8* @tco_expect_close, align 1
  br label %if.end

if.end:                                           ; preds = %if.then2, %if.then
  br label %if.end3

if.end3:                                          ; preds = %if.end, %do.body
  br label %do.body4

do.body4:                                         ; preds = %if.end3
  br label %do.body5

do.body5:                                         ; preds = %do.body4
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body5
  br label %do.end6

do.end6:                                          ; preds = %do.end
  br label %do.end7

do.end7:                                          ; preds = %do.end6
  store i8 42, i8* @tco_expect_close, align 1
  br label %do.body8

do.body8:                                         ; preds = %do.end7
  %2 = load i8, i8* @tco_expect_close, align 1
  %conv9 = sext i8 %2 to i32
  %cmp10 = icmp ne i32 %conv9, 42
  br i1 %cmp10, label %if.then12, label %if.else

if.then12:                                        ; preds = %do.body8
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 732, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.closer2, i32 0, i32 0)) #4
  unreachable

if.else:                                          ; preds = %do.body8
  br label %do.body13

do.body13:                                        ; preds = %if.else
  br label %do.body14

do.body14:                                        ; preds = %do.body13
  br label %while.cond

while.cond:                                       ; preds = %while.body, %do.body14
  %3 = load i8, i8* @tco_lock, align 1
  %tobool15 = trunc i8 %3 to i1
  br i1 %tobool15, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  br label %while.cond

while.end:                                        ; preds = %while.cond
  br label %do.end16

do.end16:                                         ; preds = %while.end
  store i8 1, i8* @tco1_cnt_b4, align 1
  br label %do.body17

do.body17:                                        ; preds = %do.end16
  store i8 0, i8* @tco_lock, align 1
  br label %do.end18

do.end18:                                         ; preds = %do.body17
  br label %do.end19

do.end19:                                         ; preds = %do.end18
  br label %if.end20

if.end20:                                         ; preds = %do.end19
  br label %do.body21

do.body21:                                        ; preds = %if.end20
  store i64 0, i64* @timer_alive, align 8
  br label %do.end22

do.end22:                                         ; preds = %do.body21
  store i8 0, i8* @tco_expect_close, align 1
  br label %do.end23

do.end23:                                         ; preds = %do.end22
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @closer3(i8* %unused) #0 {
entry:
  store i8 86, i8* @tco_write_buf, align 1
  br label %do.body

do.body:                                          ; preds = %entry
  %0 = load i32, i32* @nowayout, align 4
  %tobool = icmp ne i32 %0, 0
  br i1 %tobool, label %if.end3, label %if.then

if.then:                                          ; preds = %do.body
  store i8 0, i8* @tco_expect_close, align 1
  %1 = load i8, i8* @tco_write_buf, align 1
  %conv = sext i8 %1 to i32
  %cmp = icmp eq i32 %conv, 86
  br i1 %cmp, label %if.then2, label %if.end

if.then2:                                         ; preds = %if.then
  store i8 42, i8* @tco_expect_close, align 1
  br label %if.end

if.end:                                           ; preds = %if.then2, %if.then
  br label %if.end3

if.end3:                                          ; preds = %if.end, %do.body
  br label %do.body4

do.body4:                                         ; preds = %if.end3
  br label %do.body5

do.body5:                                         ; preds = %do.body4
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body5
  br label %do.end6

do.end6:                                          ; preds = %do.end
  br label %do.end7

do.end7:                                          ; preds = %do.end6
  store i8 42, i8* @tco_expect_close, align 1
  br label %do.body8

do.body8:                                         ; preds = %do.end7
  %2 = load i8, i8* @tco_expect_close, align 1
  %conv9 = sext i8 %2 to i32
  %cmp10 = icmp ne i32 %conv9, 42
  br i1 %cmp10, label %if.then12, label %if.else

if.then12:                                        ; preds = %do.body8
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 743, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @__PRETTY_FUNCTION__.closer3, i32 0, i32 0)) #4
  unreachable

if.else:                                          ; preds = %do.body8
  br label %do.body13

do.body13:                                        ; preds = %if.else
  br label %do.body14

do.body14:                                        ; preds = %do.body13
  br label %while.cond

while.cond:                                       ; preds = %while.body, %do.body14
  %3 = load i8, i8* @tco_lock, align 1
  %tobool15 = trunc i8 %3 to i1
  br i1 %tobool15, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  br label %while.cond

while.end:                                        ; preds = %while.cond
  br label %do.end16

do.end16:                                         ; preds = %while.end
  store i8 1, i8* @tco1_cnt_b4, align 1
  br label %do.body17

do.body17:                                        ; preds = %do.end16
  store i8 0, i8* @tco_lock, align 1
  br label %do.end18

do.end18:                                         ; preds = %do.body17
  br label %do.end19

do.end19:                                         ; preds = %do.end18
  br label %if.end20

if.end20:                                         ; preds = %do.end19
  br label %do.body21

do.body21:                                        ; preds = %if.end20
  store i64 0, i64* @timer_alive, align 8
  br label %do.end22

do.end22:                                         ; preds = %do.body21
  store i8 0, i8* @tco_expect_close, align 1
  br label %do.end23

do.end23:                                         ; preds = %do.end22
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer1(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer2(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer3(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer4(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  br label %do.body5

do.body5:                                         ; preds = %do.end4
  br label %do.body6

do.body6:                                         ; preds = %do.body5
  br label %do.body7

do.body7:                                         ; preds = %do.body6
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end8

do.end8:                                          ; preds = %do.body7
  br label %do.end9

do.end9:                                          ; preds = %do.end8
  br label %do.end10

do.end10:                                         ; preds = %do.end9
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer5(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer6(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer7(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer8(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer9(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer10(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer11(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer12(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer13(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer14(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer15(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer16(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer17(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer18(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer19(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer20(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer21(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer22(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer23(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer24(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer25(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer26(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer27(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer28(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer29(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer30(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  br label %do.body2

do.body2:                                         ; preds = %do.body1
  store i8 1, i8* @tco1_rld, align 1
  br label %do.end

do.end:                                           ; preds = %do.body2
  br label %do.end3

do.end3:                                          ; preds = %do.end
  br label %do.end4

do.end4:                                          ; preds = %do.end3
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %t1 = alloca i64, align 8
  %t2 = alloca i64, align 8
  store i8 0, i8* @tco_expect_close, align 1
  %call = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @closer1, i8* null) #5
  %call1 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @closer2, i8* null) #5
  %call2 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @closer3, i8* null) #5
  %call3 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer1, i8* null) #5
  %call4 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer2, i8* null) #5
  %call5 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @writer3, i8* null) #5
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
