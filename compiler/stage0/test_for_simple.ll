; MELP Program - Generated LLVM IR
; Compiler: MELP Stage 0 with LLVM Backend
; Date: 13 Aralık 2025

; Type definitions
%sto_value = type { i8, i64 }

; MLP Standard Library - I/O Functions
; void mlp_println_numeric(void* value, uint8_t sto_type)
declare void @mlp_println_numeric(i8*, i8)


; Function: main
define i64 @main() {
entry:
    %sum = alloca i64, align 8
    store i64 0, i64* %sum, align 8
    %i = alloca i64, align 8
    store i64 1, i64* %i, align 8
    %__for_end_1 = alloca i64, align 8
    store i64 5, i64* %__for_end_1, align 8
    br label %label1
label1:
    %tmp2 = load i64, i64* %i, align 8
    %tmp3 = load i64, i64* %__for_end_1, align 8
    %tmp4 = icmp sle i64 %tmp2, %tmp3
    br i1 %tmp4, label %label2, label %label4
label2:
    %tmp5 = load i64, i64* %sum, align 8
    %tmp6 = load i64, i64* %i, align 8
    %tmp7 = add nsw i64 %tmp5, %tmp6
    store i64 %tmp7, i64* %sum, align 8
    br label %label3
label3:
    %tmp8 = load i64, i64* %i, align 8
    %tmp9 = add nsw i64 %tmp8, 1
    store i64 %tmp9, i64* %i, align 8
    br label %label1
label4:
    %tmp10 = load i64, i64* %sum, align 8
    ret i64 %tmp10
}
