; MELP function call example - LLVM IR
; Source:
;   function add(numeric a, numeric b) returns numeric
;       return a + b
;   end function
;   
;   function main() returns numeric
;       numeric result = add(10, 20)
;       return result
;   end function

; Type definitions
%sto_value = type { i8, i64 }

; Function: add
define i64 @add(i64 %a, i64 %b) {
entry:
    ; Perform addition
    %result = add nsw i64 %a, %b
    ret i64 %result
}

; Function: main
define i64 @main() {
entry:
    ; Call add(10, 20)
    %result = call i64 @add(i64 10, i64 20)
    
    ; Return result
    ret i64 %result
}
