; Simple LLVM IR test - Hello World
@.str = private unnamed_addr constant [13 x i8] c"Hello LLVM!\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str, i64 0, i64 0))
  ret i32 0
}
