; ModuleID = 'main2.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%union.pthread_attr_t = type { i64, [48 x i8] }

@timeout = global i32 30, align 4
@nowayout = global i32 0, align 4
@jiffies = common global i64 0, align 8
@next_heartbeat = common global i64 0, align 8
@fops_write_buf = common global i8 0, align 1
@wdt_expect_close = common global i8 0, align 1
@.str = private unnamed_addr constant [2 x i8] c"0\00", align 1
@.str.1 = private unnamed_addr constant [7 x i8] c"main.c\00", align 1
@__PRETTY_FUNCTION__.closer = private unnamed_addr constant [21 x i8] c"void *closer(void *)\00", align 1
@wdt_spinlock = common global i8 0, align 1
@wdtmrctl = common global i64 0, align 8
@wdt_is_open = common global i64 0, align 8

; Function Attrs: nounwind uwtable
define i8* @writer1(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer2(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer3(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer4(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer5(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer6(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer7(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer8(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer9(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer10(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer11(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer12(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer13(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer14(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer15(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer16(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer17(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer18(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer19(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer20(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer21(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer22(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer23(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer24(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer25(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer26(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer27(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer28(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
  ret i8* null
}

; Function Attrs: nounwind uwtable
define i8* @writer29(i8* %unused) #0 {
entry:
  br label %do.body

do.body:                                          ; preds = %entry
  br label %do.body1

do.body1:                                         ; preds = %do.body
  %0 = load i64, i64* @jiffies, align 8
  %1 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %1, 1
  %conv = sext i32 %mul to i64
  %add = add nsw i64 %0, %conv
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body1
  br label %do.end2

do.end2:                                          ; preds = %do.end
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
  store i8 0, i8* @wdt_expect_close, align 1
  %1 = load i8, i8* @fops_write_buf, align 1
  %conv = sext i8 %1 to i32
  %cmp = icmp eq i32 %conv, 86
  br i1 %cmp, label %if.then2, label %if.end

if.then2:                                         ; preds = %if.then
  store i8 42, i8* @wdt_expect_close, align 1
  br label %if.end

if.end:                                           ; preds = %if.then2, %if.then
  br label %if.end3

if.end3:                                          ; preds = %if.end, %do.body
  br label %do.body4

do.body4:                                         ; preds = %if.end3
  %2 = load i64, i64* @jiffies, align 8
  %3 = load i32, i32* @timeout, align 4
  %mul = mul nsw i32 %3, 1
  %conv5 = sext i32 %mul to i64
  %add = add nsw i64 %2, %conv5
  store i64 %add, i64* @next_heartbeat, align 8
  br label %do.end

do.end:                                           ; preds = %do.body4
  br label %do.end6

do.end6:                                          ; preds = %do.end
  br label %do.body7

do.body7:                                         ; preds = %do.end6
  %4 = load i8, i8* @wdt_expect_close, align 1
  %conv8 = sext i8 %4 to i32
  %cmp9 = icmp ne i32 %conv8, 42
  br i1 %cmp9, label %if.then11, label %if.else

if.then11:                                        ; preds = %do.body7
  call void @__assert_fail(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.1, i32 0, i32 0), i32 760, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @__PRETTY_FUNCTION__.closer, i32 0, i32 0)) #4
  unreachable

if.else:                                          ; preds = %do.body7
  br label %do.body12

do.body12:                                        ; preds = %if.else
  br label %do.body13

do.body13:                                        ; preds = %do.body12
  br label %do.body14

do.body14:                                        ; preds = %do.body13
  br label %while.cond

while.cond:                                       ; preds = %while.body, %do.body14
  %5 = load i8, i8* @wdt_spinlock, align 1
  %tobool15 = trunc i8 %5 to i1
  br i1 %tobool15, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  br label %while.cond

while.end:                                        ; preds = %while.cond
  br label %do.end16

do.end16:                                         ; preds = %while.end
  %6 = load i64, i64* @wdtmrctl, align 8
  br label %do.body17

do.body17:                                        ; preds = %do.end16
  store i64 43690, i64* @wdtmrctl, align 8
  br label %do.end18

do.end18:                                         ; preds = %do.body17
  br label %do.body19

do.body19:                                        ; preds = %do.end18
  store i64 21845, i64* @wdtmrctl, align 8
  br label %do.end20

do.end20:                                         ; preds = %do.body19
  br label %do.body21

do.body21:                                        ; preds = %do.end20
  store i64 13107, i64* @wdtmrctl, align 8
  br label %do.end22

do.end22:                                         ; preds = %do.body21
  br label %do.body23

do.body23:                                        ; preds = %do.end22
  store i64 52428, i64* @wdtmrctl, align 8
  br label %do.end24

do.end24:                                         ; preds = %do.body23
  br label %do.body25

do.body25:                                        ; preds = %do.end24
  store i64 0, i64* @wdtmrctl, align 8
  br label %do.end26

do.end26:                                         ; preds = %do.body25
  br label %do.body27

do.body27:                                        ; preds = %do.end26
  store i8 0, i8* @wdt_spinlock, align 1
  br label %do.end28

do.end28:                                         ; preds = %do.body27
  br label %do.end29

do.end29:                                         ; preds = %do.end28
  br label %do.end30

do.end30:                                         ; preds = %do.end29
  br label %if.end31

if.end31:                                         ; preds = %do.end30
  br label %do.body32

do.body32:                                        ; preds = %if.end31
  store i64 0, i64* @wdt_is_open, align 8
  br label %do.end33

do.end33:                                         ; preds = %do.body32
  store i8 0, i8* @wdt_expect_close, align 1
  br label %do.end34

do.end34:                                         ; preds = %do.end33
  ret i8* null
}

; Function Attrs: noreturn nounwind
declare void @__assert_fail(i8*, i8*, i32, i8*) #1

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %t1 = alloca i64, align 8
  %t2 = alloca i64, align 8
  store i8 0, i8* @fops_write_buf, align 1
  store i64 0, i64* @jiffies, align 8
  store i8 0, i8* @wdt_expect_close, align 1
  %call = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer1, i8* null) #5
  %call1 = call i32 @pthread_create(i64* %t2, %union.pthread_attr_t* null, i8* (i8*)* @writer2, i8* null) #5
  %call2 = call i32 @pthread_create(i64* %t1, %union.pthread_attr_t* null, i8* (i8*)* @closer, i8* null) #5
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
