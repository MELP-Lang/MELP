; MELP Program - Generated LLVM IR
; Compiler: MELP Stage 0 with LLVM Backend
; Date: 13 Aralık 2025

; Type definitions
%sto_value = type { i8, i64 }

; MLP Standard Library - I/O Functions
; void mlp_println_numeric(void* value, uint8_t sto_type)
declare void @mlp_println_numeric(i8*, i8)


; Function: test_if
define i64 @test_if(i64 %x) {
entry:
    %result = alloca i64, align 8
    store i64 0, i64* %result, align 8
    %tmp1 = icmp sgt i64 %x, 10
    br i1 %tmp1, label %label1, label %label2
label1:
    store i64 1, i64* %result, align 8
    br label %label3
label2:
    store i64 0, i64* %result, align 8
    br label %label3
label3:
    %tmp2 = load i64, i64* %result, align 8
    ret i64 %tmp2
}

; Function: main
define i64 @main() {
entry:
    %a = alloca i64, align 8
    %tmp3 = call i64 @test_if(i64 15)
    store i64 %tmp3, i64* %a, align 8
    %b = alloca i64, align 8
    %tmp4 = call i64 @test_if(i64 5)
    store i64 %tmp4, i64* %b, align 8
    %tmp5 = load i64, i64* %a, align 8
    ret i64 %tmp5
}
