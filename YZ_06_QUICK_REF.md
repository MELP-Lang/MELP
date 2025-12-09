# YZ_06 Quick Reference - String Operations

## 🎯 What Was Done

### ✅ Runtime Functions (Complete)
```c
// runtime/stdlib/mlp_string.c
char* mlp_string_concat(const char* str1, const char* str2);
int mlp_string_compare(const char* str1, const char* str2);
```

### ✅ TTO Pattern (Documented)
```c
// Type checking during codegen
int is_numeric = function_get_var_is_numeric(func, var_name);

if (!is_numeric) {
    // String operation
    call mlp_string_concat
} else {
    // Numeric operation
    addq %r9, %r8
}
```

---

## 🚀 Next Steps for YZ_07

### 1. String Concatenation Codegen
**File:** `modules/arithmetic/arithmetic_codegen.c`  
**Line:** ~200 (case '+' in generate_binary_op_code)

```c
case '+': {
    // Check operand types
    int left_is_numeric = 1, right_is_numeric = 1;
    
    if (left && !left->is_literal && left->value) {
        left_is_numeric = function_get_var_is_numeric(func, left->value);
    }
    if (right && !right->is_literal && right->value) {
        right_is_numeric = function_get_var_is_numeric(func, right->value);
    }
    
    if (!left_is_numeric || !right_is_numeric) {
        // STRING CONCAT
        fprintf(output, "    movq %%r8, %%rdi\n");
        fprintf(output, "    movq %%r9, %%rsi\n");
        fprintf(output, "    call mlp_string_concat\n");
        fprintf(output, "    movq %%rax, %%r8\n");
    } else {
        // NUMERIC ADD
        fprintf(output, "    addq %%r9, %%r8\n");
    }
    break;
}
```

### 2. String Comparison Codegen
**File:** `modules/comparison/comparison_codegen.c`  
**Function:** `generate_comparison_code()` or similar

```c
// After loading operands to r8 and r9
int left_is_numeric = function_get_var_is_numeric(func, left_var);
int right_is_numeric = function_get_var_is_numeric(func, right_var);

if (!left_is_numeric || !right_is_numeric) {
    // STRING COMPARE
    fprintf(output, "    movq %%r8, %%rdi\n");
    fprintf(output, "    movq %%r9, %%rsi\n");
    fprintf(output, "    call mlp_string_compare\n");
    fprintf(output, "    test %%rax, %%rax\n");
    fprintf(output, "    %s %%al\n", set_instruction);  // sete, setl, etc.
    fprintf(output, "    movzbq %%al, %%r8\n");
} else {
    // NUMERIC COMPARE (existing code)
    fprintf(output, "    cmpq %%r9, %%r8\n");
    fprintf(output, "    %s %%al\n", set_instruction);
    fprintf(output, "    movzbq %%al, %%r8\n");
}
```

---

## 🧪 Test Programs

### Test 1: Basic Concat
```mlp
function main() returns numeric
    text a = "Hello"
    text b = "World"
    text result = a + b
    println(result)
    return 0
end function
```
**Expected:** `HelloWorld`

### Test 2: String Equality
```mlp
function main() returns numeric
    text password = "secret"
    if password == "secret"
        println("Match!")
    end if
    return 0
end function
```
**Expected:** `Match!`

---

## 📚 Key Files

1. **Runtime:** `runtime/stdlib/mlp_string.c` ✅
2. **Guide:** `compiler/stage0/docs/TTO_STRING_OPERATIONS.md` ✅
3. **Handoff:** `YZ/YZ_06.md` ✅
4. **Codegen Targets:**
   - `modules/arithmetic/arithmetic_codegen.c` ⏳
   - `modules/comparison/comparison_codegen.c` ⏳

---

## ⚡ Quick Commands

```bash
# Check runtime
cd runtime/stdlib && ls -la libmlp_stdlib.a

# Read guide
cat compiler/stage0/docs/TTO_STRING_OPERATIONS.md

# Start coding
cd compiler/stage0/modules/arithmetic
vim arithmetic_codegen.c  # Add string concat

cd ../comparison
vim comparison_codegen.c  # Add string compare
```

---

**YZ_06 Status:** Runtime ✅ | Docs ✅ | Codegen ⏳ (YZ_07)
