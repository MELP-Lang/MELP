# Lambda/Anonymous Functions Test Suite
## YZ_208: First-Class Functions

**Test Coverage:**
- ✅ test_01_simple_lambda.mlp - Basic lambda expression
- ✅ test_02_lambda_add.mlp - Lambda with multiple parameters
- ✅ test_03_higher_order.mlp - Higher-order functions (map)
- ✅ test_04_closure.mlp - Closures (variable capture)

**Syntax:**
```pmpl
-- Lambda type annotation
lambda<ReturnType, ParamType1, ParamType2, ...> name

-- Lambda definition
lambda (ParamType1 param1, ParamType2 param2) returns ReturnType
    -- body
end_lambda

-- Lambda call
result = lambda_var(arg1, arg2)
```

**Features:**
- ✅ Anonymous functions
- ✅ First-class functions (assign to variables)
- ✅ Higher-order functions (functions that take/return lambdas)
- ✅ Closures (capture outer scope variables)
- ⏸️ Lambda type inference (Stage 1)

**Expected Results:**
- test_01: Returns 42 (21 * 2)
- test_02: Returns 42 (10 + 32)
- test_03: Returns 30 (sum of doubled list)
- test_04: Returns 42 (closure adds 10 to 32)

**Module Integration:**
- Lambda module: `compiler/stage0/modules/lambda/`
- Runtime support: Function pointers, closure structs
- LLVM IR: Indirect calls, closure context passing
