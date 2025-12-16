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
  %greeting_ptr = alloca i8*, align 8
  %tmp1 = getelementptr inbounds [6 x i8], [6 x i8]* @.str.1, i64 0, i64 0
  store i8* %tmp1, i8** %greeting_ptr, align 8
  %target_ptr = alloca i8*, align 8
  %tmp2 = getelementptr inbounds [6 x i8], [6 x i8]* @.str.2, i64 0, i64 0
  store i8* %tmp2, i8** %target_ptr, align 8
    %message = alloca i64, align 8
    %tmp3 = load i8*, i8** %greeting_ptr, align 8
    %tmp4 = load i8*, i8** %target_ptr, align 8
  ; String concatenation
  %tmp5 = call i8* @mlp_string_concat(i8* %tmp3, i8* %tmp4)
    store i64 %tmp5, i64* %message, align 8
  %tmp7 = load i8*, i8** %message_ptr, align 8
  call void @mlp_println_string(i8* %tmp7)
    ret i64 0
}

; String Constants
@.str.1 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@.str.2 = private unnamed_addr constant [6 x i8] c"World\00", align 1

