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
; YZ_66: char* mlp_string_concat(const char* str1, const char* str2)
declare i8* @mlp_string_concat(i8*, i8*)


; Function: main
define i64 @main() {
entry:
  %message_ptr = alloca i8*, align 8
  %tmp1 = getelementptr inbounds [11 x i8], [11 x i8]* @.str.1, i64 0, i64 0
  store i8* %tmp1, i8** %message_ptr, align 8
  %tmp3 = load i8*, i8** %message_ptr, align 8
  call void @mlp_println_string(i8* %tmp3)
    ret i64 0
}

; String Constants
@.str.1 = private unnamed_addr constant [11 x i8] c"HelloWorld\00", align 1

