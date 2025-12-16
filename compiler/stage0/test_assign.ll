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
    %x = alloca i64, align 8
    store i64 10, i64* %x, align 8
    %y = alloca i64, align 8
    store i64 20, i64* %y, align 8
    %tmp1 = load i64, i64* %x, align 8
    %tmp2 = load i64, i64* %y, align 8
    %tmp3 = add nsw i64 %tmp1, %tmp2
    store i64 %tmp3, i64* %x, align 8
    %tmp4 = load i64, i64* %x, align 8
    %tmp5 = sub nsw i64 %tmp4, 5
    store i64 %tmp5, i64* %y, align 8
    %tmp6 = load i64, i64* %y, align 8
    ret i64 %tmp6
}
