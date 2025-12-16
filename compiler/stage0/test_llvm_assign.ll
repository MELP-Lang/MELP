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
