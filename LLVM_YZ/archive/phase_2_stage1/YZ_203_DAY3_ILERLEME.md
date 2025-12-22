# YZ_203 Day 3 Progress Report - Generic Call Parsing

## ✅ Completed Tasks

### 1. Generic Call Syntax Parsing
**Objective:** Parse `function_name<type1, type2>(args)` syntax in expressions

**Implementation:**
- **Files Modified:**
  - `compiler/stage0/modules/arithmetic/arithmetic_parser.c` (both stateful and stateless paths)
  - `compiler/stage0/modules/arithmetic/arithmetic.c` (cleanup)

**Changes:**
1. **Type Argument Parsing Logic** (after identifier, before `(`):
   ```c
   // After: char* identifier = strdup(...); advance(...);
   
   // Check for '<' or TOKEN_LANGLE (whitespace-aware)
   if (parser->current_token && 
       (parser->current_token->type == TOKEN_LESS || 
        parser->current_token->type == TOKEN_LANGLE)) {
       
       // Parse: <type1, type2, type3>
       // Store in: type_arguments[], type_arg_count
   }
   ```

2. **FunctionCallExpr Population:**
   ```c
   func_call->type_arguments = type_arguments;  // char** array
   func_call->type_arg_count = type_arg_count;  // int
   ```

3. **Memory Management:**
   - Added type_arguments cleanup in all error paths
   - Updated `arithmetic_expr_free()` to free type_arguments array
   - Proper cleanup in both stateful and stateless parser paths

### 2. Test File Created
**File:** `tests/llvm/11_generics/test_generic_call.mlp`
```melp
function identity<T>(T value) returns T
    return value
end_function

function main() returns numeric
    numeric result = identity<numeric>(42)
    return result
end_function
```

**Status:** Syntax ready to parse, codegen not yet implemented

## Technical Details

### Token Handling
- Supports both `TOKEN_LESS` and `TOKEN_LANGLE` for `<` symbol
- Lexer uses whitespace heuristic to distinguish:
  - `max<numeric>` → TOKEN_LANGLE (generic)
  - `x < y` → TOKEN_LESS (comparison)

### Parser Paths Updated
1. **Stateful Path** (line ~187 in arithmetic_parser.c):
   - `arithmetic_parse_expression()` → calls `parse_primary()`
   - Identifier parsed → Type args parsed → Function call

2. **Stateless Path** (line ~874 in arithmetic_parser.c):
   - `parse_primary_stateless()` 
   - Same logic, different advance mechanism

### Error Handling
All error paths now include type_arguments cleanup:
```c
for (int i = 0; i < type_arg_count; i++) {
    free(type_arguments[i]);
}
free(type_arguments);
```

## Integration Status

### ✅ Working
- Generic template parsing: `function max<T>`
- Generic call parsing: `max<numeric>(10, 20)`
- Type argument storage in FunctionCallExpr
- Memory cleanup in all paths

### ⏳ Pending
- **Monomorphization trigger** in codegen (Day 4-5)
- **Generic instantiation** when processing FunctionCallExpr
- **LLVM IR emission** for specialized instances
- **Type inference** for calls without explicit types (Day 6-7)

## Next Steps (Day 4-5)

### Monomorphization in Codegen
1. **Detect Generic Calls:**
   - In `generate_expression_llvm()` when processing `EXPR_FUNC_CALL`
   - Check if `func_call->type_arg_count > 0`

2. **Trigger Instantiation:**
   ```c
   if (func_call->type_arg_count > 0) {
       // Look up template in registry
       GenericTemplate* template = generic_find_template(
           registry, func_call->function_name
       );
       
       // Check for existing instance
       GenericInstance* instance = generic_find_instance(
           template, func_call->type_arguments, func_call->type_arg_count
       );
       
       if (!instance) {
           // Create new specialized version
           instance = generic_instantiate(
               template, func_call->type_arguments, func_call->type_arg_count
           );
       }
       
       // Use instance->mangled_name for call instruction
   }
   ```

3. **LLVM Emission:**
   - Emit specialized function definition first
   - Use mangled name in call instruction
   - Store instances to avoid duplicate specializations

### Implementation Plan
- **File:** `compiler/stage0/modules/functions/functions_codegen_llvm.c`
- **Function:** `generate_expression_llvm()`
- **Location:** Around EXPR_FUNC_CALL case

## Commit
```
Commit: a39e5fea
Message: YZ_203 Day 3: Generic call parsing
Branch: generic-types_YZ_203
```

## Progress Summary
- **Total Days:** 14 (2 weeks)
- **Completed:** Day 1-3 (21%)
- **Current Focus:** Day 4-5 Monomorphization
- **Pattern:** Following YZ_200/201/202 incremental approach

---
**Date:** 2025-01-XX  
**Task:** YZ_203 - Generic Types (Type Parameters)  
**Status:** ON TRACK ✅
