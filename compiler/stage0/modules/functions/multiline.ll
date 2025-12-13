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


; Function: main
define i64 @main() {
entry:
  %tmp1 = getelementptr inbounds [7 x i8], [7 x i8]* @.str.1, i64 0, i64 0
  call void @mlp_println_string(i8* %tmp1)
  %tmp2 = getelementptr inbounds [7 x i8], [7 x i8]* @.str.2, i64 0, i64 0
  call void @mlp_println_string(i8* %tmp2)
  %tmp3 = getelementptr inbounds [14 x i8], [14 x i8]* @.str.3, i64 0, i64 0
  call void @mlp_println_string(i8* %tmp3)
    ret i64 0
}

; String Constants
@.str.1 = private unnamed_addr constant [7 x i8] c"Line 1\00", align 1
@.str.2 = private unnamed_addr constant [7 x i8] c"Line 2\00", align 1
@.str.3 = private unnamed_addr constant [14 x i8] c"Merhaba MELP!\00", align 1

