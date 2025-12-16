; MELP Program - Generated LLVM IR
; Compiler: MELP Stage 0 with LLVM Backend
; Date: 13 Aralık 2025

; Type definitions
%sto_value = type { i8, i64 }

; External C library function
declare i32 @printf(i8*, ...)

; Format string for numeric println
@.fmt_num = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1


; Function: main
define i64 @main() {
entry:
    %a = alloca i64, align 8
    store i64 1, i64* %a, align 8
    %b = alloca i64, align 8
    store i64 1, i64* %b, align 8
    %result = alloca i64, align 8
    %tmp1 = load i64, i64* %a, align 8
    %tmp2 = load i64, i64* %b, align 8
    %tmp3 = and i64 %tmp1, %tmp2
    store i64 %tmp3, i64* %result, align 8
    %output = alloca i64, align 8
    store i64 0, i64* %output, align 8
    %tmp4 = load i64, i64* %result, align 8
    %tmp5 = icmp eq i64 %tmp4, 1
    br i1 %tmp5, label %label1, label %label2
label1:
    store i64 1, i64* %output, align 8
    br label %label2
label2:
    %tmp6 = load i64, i64* %output, align 8
    ret i64 %tmp6
}
