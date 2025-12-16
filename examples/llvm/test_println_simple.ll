; MELP println simple example - LLVM IR
; Source:
;   function main() returns numeric
;       numeric x = 42
;       numeric dummy = println(x)
;       return 0
;   end function

; External C library function
declare i32 @printf(i8*, ...)

; String constant for printf
@.fmt_num = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1

; Function: main
define i64 @main() {
entry:
    ; Allocate local variable
    %x = alloca i64, align 8
    
    ; numeric x = 42
    store i64 42, i64* %x, align 8
    
    ; Load x value
    %x_val = load i64, i64* %x, align 8
    
    ; Call printf to print the value
    %fmt_ptr = getelementptr inbounds [5 x i8], [5 x i8]* @.fmt_num, i64 0, i64 0
    call i32 (i8*, ...) @printf(i8* %fmt_ptr, i64 %x_val)
    
    ; Return 0
    ret i64 0
}
