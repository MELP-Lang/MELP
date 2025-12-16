; MELP println example - LLVM IR
; Source:
;   function main() returns numeric
;       numeric x = 42
;       numeric result = println(x)
;       return 0
;   end function

; External C library functions
declare i32 @printf(i8*, ...)

; String constant for printf
@.fmt_num = private unnamed_addr constant [4 x i8] c"%ld\00", align 1

; External MELP stdlib function
declare void @mlp_println_numeric(i8*, i8)

; Function: main
define i64 @main() {
entry:
    ; Allocate local variables
    %x = alloca i64, align 8
    %result = alloca i64, align 8
    
    ; numeric x = 42
    store i64 42, i64* %x, align 8
    
    ; Load x value
    %x_val = load i64, i64* %x, align 8
    
    ; Cast to void pointer for STO
    %x_ptr = inttoptr i64 %x_val to i8*
    
    ; Call println (STO type 0 = numeric integer)
    call void @mlp_println_numeric(i8* %x_ptr, i8 0)
    
    ; Store x_val as result (println returns void)
    store i64 %x_val, i64* %result, align 8
    
    ; Return 0
    ret i64 0
}
