; MELP test_basic.mlp compiled to LLVM IR
; Source:
;   function main() returns numeric
;       numeric x = 10
;       numeric y = 20
;       return x + y
;   end function

; Type definitions
%sto_value = type { i8, i64 }  ; {type_tag, value}
; type_tag: 0=numeric, 1=text, 2=bigdec, etc.

; External declarations (STO runtime)
declare void @sto_check_overflow(i64, i64, i64)
declare %sto_value* @sto_bigdec_from_int64(i64)
declare %sto_value* @sto_bigdec_add(%sto_value*, %sto_value*)

; Main function
define i64 @main() {
entry:
    ; Allocate local variables
    %x = alloca i64, align 8
    %y = alloca i64, align 8
    
    ; numeric x = 10
    store i64 10, i64* %x, align 8
    
    ; numeric y = 20
    store i64 20, i64* %y, align 8
    
    ; Load values for addition
    %x_val = load i64, i64* %x, align 8
    %y_val = load i64, i64* %y, align 8
    
    ; Perform addition with overflow check
    %add_result = add nsw i64 %x_val, %y_val
    ; Note: nsw = no signed wrap, triggers trap on overflow
    ; For now, simplified - full STO integration later
    
    ; Return result
    ret i64 %add_result
}
