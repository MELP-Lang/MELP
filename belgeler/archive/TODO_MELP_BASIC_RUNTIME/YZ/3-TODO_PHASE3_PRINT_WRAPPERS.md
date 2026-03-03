# 📋 PHASE 3: PRINT WRAPPERS

**Duration:** 2-3 saat  
**Priority:** 🔴 CRITICAL  
**Dependencies:** Phase 1-2 complete (strings + external declarations)  
**Owner:** YZ_BASIC_02

---

## 🎯 PHASE GOAL

`print()` function calls compile edilip gerçek LLVM call instructions'a dönüşmeli.

**Input:**
```mlp
function main()
  print("hello")
  print(42)
  return 0
end_function
```

**Expected LLVM IR:**
```llvm
@.str.0 = private unnamed_addr constant [6 x i8] c"hello\00"

declare void @__builtin_io_print_string(i8*)
declare void @__builtin_io_print_numeric(i64)

define i32 @main() {
entry:
  ; print("hello")
  %str_ptr = getelementptr inbounds [6 x i8], [6 x i8]* @.str.0, i32 0, i32 0
  call void @__builtin_io_print_string(i8* %str_ptr)
  
  ; print(42)
  call void @__builtin_io_print_numeric(i64 42)
  
  ret i32 0
}
```

---

## 📋 TASK BREAKDOWN

### Task 3.1: Add/Verify CALL_EXPR AST Node (30 min)

**File:** `compiler/stage1/common/ast.h`

**Goal:** AST can represent function calls

**Check NodeType Enum:**
```cpp
enum NodeType {
    // Literals
    NUMERIC_LITERAL,
    STRING_LITERAL,
    BOOLEAN_LITERAL,
    
    // Expressions
    BINARY_OP,
    UNARY_OP,
    CALL_EXPR,        // ← Must exist
    
    // Statements
    ASSIGNMENT,
    RETURN_STMT,
    // ...
};
```

**Check/Add CallExprNode:**
```cpp
struct CallExprNode : public ASTNode {
    NodeType type = CALL_EXPR;
    std::string functionName;           // "print", "println", etc.
    std::vector<ASTNode*> arguments;    // Argument expressions
    
    ~CallExprNode() {
        // Clean up arguments
        for (auto* arg : arguments) {
            delete arg;
        }
    }
};
```

**If Missing, Add Both:**
- Add `CALL_EXPR` to enum
- Add `CallExprNode` struct
- Update parser to create CALL_EXPR nodes (if needed)

---

### Task 3.2: Check Parser Handles Function Calls (30 min)

**File:** `compiler/stage0/parser/parser.cpp`

**Goal:** Verify parser creates CALL_EXPR for `function_name(...)`

**Search for Function Call Parsing:**
```bash
cd compiler/stage0/parser
grep -r "parseCall" .
grep -r "CALL_EXPR" .
grep -r "functionCall" .
```

**Expected Parser Logic:**
```cpp
// Pseudocode - check if similar exists
ASTNode* Parser::parseFunctionCall() {
    std::string funcName = currentToken.value;  // "print"
    expect(TOKEN_IDENTIFIER);
    expect(TOKEN_LPAREN);  // (
    
    CallExprNode* call = new CallExprNode();
    call->functionName = funcName;
    
    // Parse arguments
    if (currentToken.type != TOKEN_RPAREN) {
        do {
            ASTNode* arg = parseExpression();
            call->arguments.push_back(arg);
        } while (match(TOKEN_COMMA));
    }
    
    expect(TOKEN_RPAREN);  // )
    return call;
}
```

**If Parser Missing:**
- Add `parseFunctionCall()` method
- Integrate into expression parser
- **Time:** +2 hours

---

### Task 3.3: Implement compileCallExpr() Method (60 min)

**File:** `compiler/stage1/modules/function_compiler/function_compiler.cpp`

**Goal:** Compile CALL_EXPR to LLVM call instruction

**Header Addition (function_compiler.h):**
```cpp
class FunctionCompiler {
public:
    llvm::Value* compileCallExpr(ASTNode* node);
};
```

**Implementation (function_compiler.cpp):**
```cpp
llvm::Value* FunctionCompiler::compileCallExpr(ASTNode* node) {
    CallExprNode* call = static_cast<CallExprNode*>(node);
    
    // 1. Get the function from module
    llvm::Function* func = module->getFunction(call->functionName);
    if (!func) {
        throw CompileError(
            "Undefined function: " + call->functionName
        );
    }
    
    // 2. Compile arguments
    std::vector<llvm::Value*> args;
    for (ASTNode* argNode : call->arguments) {
        llvm::Value* argVal = compileExpression(argNode);
        args.push_back(argVal);
    }
    
    // 3. Type check (ensure arg count matches)
    if (args.size() != func->arg_size()) {
        throw CompileError(
            "Function " + call->functionName + 
            " expects " + std::to_string(func->arg_size()) +
            " arguments, got " + std::to_string(args.size())
        );
    }
    
    // 4. Create call instruction
    llvm::Value* result = builder.CreateCall(func, args);
    
    return result;
}
```

**Key Points:**
- `module->getFunction()` retrieves declared function
- `compileExpression()` handles each argument
- `builder.CreateCall()` creates LLVM call instruction
- Type checking ensures args match signature

---

### Task 3.4: Add print() Type Dispatcher (45 min)

**File:** `compiler/stage1/modules/function_compiler/function_compiler.cpp`

**Goal:** Route print() to correct wrapper based on argument type

**Modify compileCallExpr():**
```cpp
llvm::Value* FunctionCompiler::compileCallExpr(ASTNode* node) {
    CallExprNode* call = static_cast<CallExprNode*>(node);
    
    // ========================================
    // SPECIAL HANDLING FOR print()
    // ========================================
    if (call->functionName == "print") {
        if (call->arguments.size() != 1) {
            throw CompileError("print() expects exactly 1 argument");
        }
        
        ASTNode* argNode = call->arguments[0];
        llvm::Value* argVal = compileExpression(argNode);
        
        // Type-based dispatch
        llvm::Function* printFunc = nullptr;
        
        if (argNode->type == STRING_LITERAL) {
            // Call __builtin_io_print_string(i8*)
            printFunc = module->getFunction("__builtin_io_print_string");
        }
        else if (argNode->type == NUMERIC_LITERAL || 
                 argNode->type == VARIABLE) {
            // Call __builtin_io_print_numeric(i64)
            // TODO: Check variable type is numeric
            printFunc = module->getFunction("__builtin_io_print_numeric");
        }
        else if (argNode->type == BOOLEAN_LITERAL) {
            // Future: __builtin_io_print_boolean
            throw CompileError("print(boolean) not yet implemented");
        }
        else {
            throw CompileError(
                "print() unsupported type: " + 
                std::string(getNodeTypeName(argNode->type))
            );
        }
        
        if (!printFunc) {
            throw CompileError("Print wrapper not declared");
        }
        
        return builder.CreateCall(printFunc, {argVal});
    }
    
    // ========================================
    // GENERIC FUNCTION CALL (other functions)
    // ========================================
    llvm::Function* func = module->getFunction(call->functionName);
    if (!func) {
        throw CompileError("Undefined function: " + call->functionName);
    }
    
    std::vector<llvm::Value*> args;
    for (ASTNode* argNode : call->arguments) {
        args.push_back(compileExpression(argNode));
    }
    
    return builder.CreateCall(func, args);
}
```

**Important:**
- Type dispatch happens at compile time (zero runtime overhead)
- String → `__builtin_io_print_string`
- Numeric → `__builtin_io_print_numeric`
- Future: Boolean, arrays, structs, etc.

---

### Task 3.5: Integrate into compileExpression() (15 min)

**File:** `compiler/stage1/modules/function_compiler/function_compiler.cpp`

**Goal:** Handle CALL_EXPR in expression compiler

**Add to Switch:**
```cpp
llvm::Value* FunctionCompiler::compileExpression(ASTNode* node) {
    switch (node->type) {
        case NUMERIC_LITERAL:
            return compileNumericLiteral(node);
            
        case STRING_LITERAL:
            return createStringLiteral(node->value);
            
        case BOOLEAN_LITERAL:
            return compileBooleanLiteral(node);
            
        case VARIABLE:
            return compileVariableRef(node);
            
        case BINARY_OP:
            return compileBinaryOp(node);
            
        case CALL_EXPR:  // NEW: Handle function calls
            return compileCallExpr(node);
            
        default:
            throw CompileError(
                "Unknown expression type: " + 
                std::string(getNodeTypeName(node->type))
            );
    }
}
```

---

### Task 3.6: Handle CALL_EXPR as Statement (15 min)

**File:** `compiler/stage1/modules/function_compiler/function_compiler.cpp`

**Goal:** Allow `print()` as standalone statement (not just expression)

**Add to compileStatement():**
```cpp
void FunctionCompiler::compileStatement(ASTNode* node) {
    switch (node->type) {
        case ASSIGNMENT:
            compileAssignment(node);
            break;
            
        case RETURN_STMT:
            compileReturn(node);
            break;
            
        case IF_STMT:
            compileIfStatement(node);
            break;
            
        case WHILE_STMT:
            compileWhileStatement(node);
            break;
            
        case FOR_STMT:
            compileForStatement(node);
            break;
            
        case CALL_EXPR:  // NEW: Function call as statement
            compileCallExpr(node);
            break;
            
        default:
            throw CompileError("Unknown statement type");
    }
}
```

---

### Task 3.7: Test Phase 3 (45 min)

**Test Case 1: Print String**
```mlp
# File: tests/phase3/test_print_string.mlp
function main()
  print("hello world")
  return 0
end_function
```

**Expected IR:**
```llvm
@.str.0 = private unnamed_addr constant [12 x i8] c"hello world\00"

declare void @__builtin_io_print_string(i8*)

define i32 @main() {
entry:
  %str_ptr = getelementptr inbounds [12 x i8], [12 x i8]* @.str.0, i32 0, i32 0
  call void @__builtin_io_print_string(i8* %str_ptr)
  ret i32 0
}
```

**Test Case 2: Print Numeric**
```mlp
function main()
  print(42)
  return 0
end_function
```

**Expected IR:**
```llvm
declare void @__builtin_io_print_numeric(i64)

define i32 @main() {
entry:
  call void @__builtin_io_print_numeric(i64 42)
  ret i32 0
}
```

**Test Case 3: Print Variable**
```mlp
function main()
  numeric x = 123
  print(x)
  return 0
end_function
```

**Expected IR:**
```llvm
define i32 @main() {
entry:
  %x = alloca i64
  store i64 123, i64* %x
  %1 = load i64, i64* %x
  call void @__builtin_io_print_numeric(i64 %1)
  ret i32 0
}
```

**Test Case 4: Multiple Print Calls**
```mlp
function main()
  print("x = ")
  numeric x = 42
  print(x)
  return 0
end_function
```

**Expected IR:**
```llvm
@.str.0 = private unnamed_addr constant [5 x i8] c"x = \00"

declare void @__builtin_io_print_string(i8*)
declare void @__builtin_io_print_numeric(i64)

define i32 @main() {
entry:
  %str_ptr = getelementptr inbounds [5 x i8], [5 x i8]* @.str.0, i32 0, i32 0
  call void @__builtin_io_print_string(i8* %str_ptr)
  %x = alloca i64
  store i64 42, i64* %x
  %1 = load i64, i64* %x
  call void @__builtin_io_print_numeric(i64 %1)
  ret i32 0
}
```

**Validation:**
```bash
# Test all cases
for test in test_print_string test_print_numeric test_print_variable test_multiple_prints; do
    echo "Testing $test..."
    ./melp_compiler tests/phase3/$test.mlp -o /tmp/$test.ll
    
    # Check for call instructions
    grep "call void @__builtin_io_print" /tmp/$test.ll
    
    # Validate IR
    llvm-as /tmp/$test.ll -o /tmp/$test.bc
    opt -verify /tmp/$test.bc -o /dev/null
    
    # Compile to object
    llc /tmp/$test.ll -o /tmp/$test.o
    
    echo "✅ $test passed"
done
```

---

## ✅ SUCCESS CRITERIA

**Phase 3 Complete When:**
- [ ] CALL_EXPR node exists in AST
- [ ] Parser creates CALL_EXPR nodes (or added if missing)
- [ ] `compileCallExpr()` method works
- [ ] Type-based print() dispatcher works
- [ ] `print("string")` generates call instruction
- [ ] `print(number)` generates call instruction
- [ ] `print(variable)` generates call instruction
- [ ] Multiple print() calls work
- [ ] LLVM IR validates (`opt -verify` passes)
- [ ] Object files compile (`llc` succeeds)
- [ ] No compilation warnings

---

## 🚧 TROUBLESHOOTING

### Problem 1: Function Not Found

**Symptom:**
```
Error: Undefined function: __builtin_io_print_string
```

**Solution:**
Ensure Phase 2 declarations are called:
```cpp
void FunctionCompiler::compile() {
    // MUST declare functions first!
    declarePrintWrappers();  // Or inline declarations
    
    // Then compile body
    compileBody(root);
}
```

---

### Problem 2: Type Mismatch in Call

**Symptom:**
```
error: call argument type 'i32' does not match function signature 'i64'
```

**Solution:**
Ensure numeric values are i64:
```cpp
// WRONG:
llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 42);

// CORRECT:
llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), 42);
```

---

### Problem 3: Wrong Argument Count

**Symptom:**
```
error: call has incorrect number of arguments
```

**Solution:**
Check argument count matches function signature:
```cpp
if (args.size() != func->arg_size()) {
    throw CompileError("Argument count mismatch");
}
```

---

### Problem 4: Variable Type Unknown

**Symptom:**
Type dispatcher doesn't know if variable is string or numeric

**Solution:**
Add symbol table lookup:
```cpp
if (argNode->type == VARIABLE) {
    // Look up variable type in symbol table
    VariableInfo* varInfo = symbolTable->lookup(argNode->name);
    if (varInfo->type == TYPE_NUMERIC) {
        printFunc = module->getFunction("__builtin_io_print_numeric");
    }
    else if (varInfo->type == TYPE_STRING) {
        printFunc = module->getFunction("__builtin_io_print_string");
    }
}
```

**Note:** If symbol table doesn't exist yet, assume numeric for MVP.

---

## 📚 REFERENCE CODE (Stage 0)

**Stage 0 Function Calls:**
```bash
grep -A 20 "compileCallExpr" compiler/stage0/codegen/*.cpp
```

**LLVM Call Instruction API:**
```cpp
// Example
llvm::Function* func = module->getFunction("printf");
std::vector<llvm::Value*> args = {formatStr, value};
llvm::Value* result = builder.CreateCall(func, args);
```

---

## 🎯 NEXT PHASE

**When Phase 3 Complete:**
📄 **Move to [4-TODO_PHASE4_STDLIB_INTEGRATION.md](4-TODO_PHASE4_STDLIB_INTEGRATION.md)**

**You'll add:**
- Runtime library (libmelp_runtime.a)
- Implement print wrappers (__builtin_io_print_*)
- Update build system for linking
- **Programs will actually RUN!**

---

## 📋 PHASE 3 CHECKLIST

Before moving to Phase 4:

**Code:**
- [ ] CALL_EXPR in AST
- [ ] Parser creates CALL_EXPR
- [ ] compileCallExpr() implemented
- [ ] Type dispatcher works
- [ ] CALL_EXPR handled in expressions
- [ ] CALL_EXPR handled in statements

**Testing:**
- [ ] print("string") compiles
- [ ] print(number) compiles
- [ ] print(variable) compiles
- [ ] Multiple prints work
- [ ] IR has call instructions
- [ ] IR validates
- [ ] Object files compile

**Quality:**
- [ ] No compiler warnings
- [ ] Type checking works
- [ ] Error messages clear
- [ ] Code documented

---

**Ready? Start Task 3.1! 🚀**
