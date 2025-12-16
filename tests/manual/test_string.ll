; MELP Program - Generated LLVM IR
; Compiler: MELP Stage 0 with LLVM Backend
; Date: 13 Aralık 2025

; Type definitions
%sto_value = type { i8, i64 }

; MLP Standard Library - I/O Functions
; void mlp_println_numeric(void* value, uint8_t sto_type)
declare void @mlp_println_numeric(i8*, i8)
; void mlp_println_string(const char* str)
declare void @mlp_println_string(i8*)


; Function: greet
define i64 @greet(i8* %msg) {
entry:
  call void @mlp_println_string(i8* %msg)
    ret i64 0
}

; Function: main
define i64 @main() {
entry:
    %result = alloca i64, align 8
    %tmp1 = getelementptr inbounds [11 x i8], [11 x i8]* @.str.1, i64 0, i64 0
    %tmp2 = call i64 @greet(i8* %tmp1)
    store i64 %tmp2, i64* %result, align 8
    ret i64 0
}

; String Constants
@.str.1 = private unnamed_addr constant [11 x i8] c"Hello MELP\00", align 1

