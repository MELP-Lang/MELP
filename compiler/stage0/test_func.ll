; MELP Program - Generated LLVM IR
; Compiler: MELP Stage 0 with LLVM Backend
; Date: 13 Aralık 2025

; Type definitions
%sto_value = type { i8, i64 }

; External C library function
declare i32 @printf(i8*, ...)

; Format string for numeric println
@.fmt_num = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1


; Function: add
define i64 @add(i64 %a, i64 %b) {
entry:
    %tmp1 = add nsw i64 %a, %b
    ret i64 %tmp1
}

; Function: main
define i64 @main() {
entry:
    %x = alloca i64, align 8
    store i64 15, i64* %x, align 8
    %y = alloca i64, align 8
    store i64 27, i64* %y, align 8
    %sum = alloca i64, align 8
    %tmp2 = load i64, i64* %x, align 8
    %tmp3 = load i64, i64* %y, align 8
    %tmp4 = call i64 @add(i64 %tmp2, i64 %tmp3)
    store i64 %tmp4, i64* %sum, align 8
    %tmp5 = load i64, i64* %sum, align 8
    ret i64 %tmp5
}
