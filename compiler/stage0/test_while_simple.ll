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
    store i64 0, i64* %x, align 8
    br label %label1
label1:
    %tmp1 = load i64, i64* %x, align 8
    %tmp2 = icmp slt i64 %tmp1, 5
    br i1 %tmp2, label %label2, label %label3
label2:
    %tmp3 = load i64, i64* %x, align 8
    %tmp4 = add nsw i64 %tmp3, 1
    store i64 %tmp4, i64* %x, align 8
    br label %label1
label3:
    %tmp5 = load i64, i64* %x, align 8
    ret i64 %tmp5
}
