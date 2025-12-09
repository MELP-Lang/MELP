# TTO String Operations - Implementation Guide (YZ_06)

## 📋 Overview

**Goal:** Add string concatenation and comparison to MELP using TTO principles.

**Current Status:**
- ✅ Runtime functions implemented (`mlp_string.c`)
- ✅ TTO type tracking exists (`is_numeric` flag)
- ⏳ Codegen integration needed

---

## 🎯 TTO Pattern for String Operations

### 1. Compile-Time Type Tracking (Already Works!)

**Variable Declaration:**
```mlp
numeric x = 42      → is_numeric = 1
text msg = "hello"  → is_numeric = 0
```

**Internal Representation:**
```c
// functions.h (Already implemented by YZ_05)
typedef struct LocalVariable {
    char* name;
    int stack_offset;
    int is_numeric;    // 1=numeric, 0=text
    struct LocalVariable* next;
} LocalVariable;
```

**Type Lookup:**
```c
// During codegen
int is_numeric = function_get_var_is_numeric(func, var_name);
```

---

## 🔧 Implementation: String Concatenation

### User Code:
```mlp
text greeting = "Hello"
text target = "World"
text message = greeting + " " + target
println(message)
```

### Parsing (Already Works):
```c
// variable_parser.c → Detects text type
// arithmetic_parser.c → Parses + operator
```

### Codegen Pattern:

**Location:** `modules/arithmetic/arithmetic_codegen.c`

**Current + operator codegen:**
```c
// generate_binary_op_code() - handles numeric arithmetic
case '+':
    fprintf(output, "    addq %%r9, %%r8\n");
    break;
```

**New: Type-aware + operator:**
```c
case '+': {
    // Check if operands are text or numeric
    int left_is_numeric = 1, right_is_numeric = 1;
    
    // Check left operand type
    if (left && !left->is_literal && left->value) {
        left_is_numeric = function_get_var_is_numeric(func, left->value);
    }
    
    // Check right operand type
    if (right && !right->is_literal && right->value) {
        right_is_numeric = function_get_var_is_numeric(func, right->value);
    }
    
    if (!left_is_numeric || !right_is_numeric) {
        // STRING CONCATENATION
        fprintf(output, "    # String concatenation\n");
        fprintf(output, "    movq %%r8, %%rdi   # arg1: left string\n");
        fprintf(output, "    movq %%r9, %%rsi   # arg2: right string\n");
        fprintf(output, "    call mlp_string_concat\n");
        fprintf(output, "    movq %%rax, %%r8   # result to r8\n");
    } else {
        // NUMERIC ADDITION
        fprintf(output, "    addq %%r9, %%r8\n");
    }
    break;
}
```

**Generated Assembly:**
```asm
# text message = greeting + target

# Load greeting
movq -8(%rbp), %r8      # greeting at offset -8

# Load target
movq -16(%rbp), %r9     # target at offset -16

# String concatenation
movq %r8, %rdi          # arg1: left string
movq %r9, %rsi          # arg2: right string
call mlp_string_concat  # Returns new string in %rax
movq %rax, %r8          # Move result to r8

# Store result
movq %r8, -24(%rbp)     # message at offset -24
```

---

## 🔧 Implementation: String Comparison

### User Code:
```mlp
text password = "admin123"
if password == "admin123"
    println("Access granted")
else
    println("Access denied")
end if
```

### Codegen Pattern:

**Location:** `modules/comparison/comparison_codegen.c`

**Current comparison codegen:**
```c
// generate_comparison_code() - handles numeric comparisons
fprintf(output, "    cmpq %%r9, %%r8\n");
fprintf(output, "    %s %%al\n", set_instruction);  // setl, sete, etc.
```

**New: Type-aware comparison:**
```c
void generate_comparison_code(FILE* output, ComparisonExpr* expr, 
                              int target_reg, FunctionDeclaration* func) {
    // ... load left and right operands to r8 and r9 ...
    
    // Check if operands are text
    int left_is_numeric = 1, right_is_numeric = 1;
    
    if (expr->left && !expr->left->is_literal && expr->left->value) {
        left_is_numeric = function_get_var_is_numeric(func, expr->left->value);
    }
    
    if (expr->right && !expr->right->is_literal && expr->right->value) {
        right_is_numeric = function_get_var_is_numeric(func, expr->right->value);
    }
    
    if (!left_is_numeric || !right_is_numeric) {
        // STRING COMPARISON
        fprintf(output, "    # String comparison\n");
        fprintf(output, "    movq %%r8, %%rdi   # arg1: left string\n");
        fprintf(output, "    movq %%r9, %%rsi   # arg2: right string\n");
        fprintf(output, "    call mlp_string_compare\n");
        fprintf(output, "    # Result in %%rax: <0, 0, >0\n");
        
        // Map comparison result to boolean
        const char* set_instruction;
        switch (expr->op) {
            case CMP_EQ:  set_instruction = "sete";  break;  // rax == 0
            case CMP_NE:  set_instruction = "setne"; break;  // rax != 0
            case CMP_LT:  set_instruction = "setl";  break;  // rax < 0
            case CMP_LE:  set_instruction = "setle"; break;  // rax <= 0
            case CMP_GT:  set_instruction = "setg";  break;  // rax > 0
            case CMP_GE:  set_instruction = "setge"; break;  // rax >= 0
        }
        
        fprintf(output, "    test %%rax, %%rax  # Test comparison result\n");
        fprintf(output, "    %s %%al\n", set_instruction);
        fprintf(output, "    movzbq %%al, %%r8  # Zero-extend to r8\n");
        
    } else {
        // NUMERIC COMPARISON (existing code)
        fprintf(output, "    cmpq %%r9, %%r8\n");
        fprintf(output, "    %s %%al\n", get_set_instruction(expr->op));
        fprintf(output, "    movzbq %%al, %%r8\n");
    }
}
```

**Generated Assembly:**
```asm
# if password == "admin123"

# Load password variable
movq -8(%rbp), %r8          # password at offset -8

# Load string literal
leaq .str_0(%rip), %r9      # "admin123" in .rodata

# String comparison
movq %r8, %rdi              # arg1: password
movq %r9, %rsi              # arg2: "admin123"
call mlp_string_compare     # Returns int in %rax

# Check equality (rax == 0)
test %rax, %rax             # Test result
sete %al                    # Set if equal (zero)
movzbq %al, %r8             # Zero-extend to 64-bit

# Use result in if condition
test %r8, %r8
jz .else_label
```

---

## 📝 Files to Modify

### 1. **arithmetic_codegen.c** (String Concatenation)
**Path:** `compiler/stage0/modules/arithmetic/arithmetic_codegen.c`

**Function:** `generate_binary_op_code()`

**Change:** Add type check for `+` operator, dispatch to `mlp_string_concat`

---

### 2. **comparison_codegen.c** (String Comparison)
**Path:** `compiler/stage0/modules/comparison/comparison_codegen.c`

**Function:** `generate_comparison_code()` or `comparison_generate_code()`

**Change:** Add type check for all comparison operators, dispatch to `mlp_string_compare`

---

### 3. **Include Header in Codegen** (Optional)
If codegen files need to know about string functions:

```c
// At top of codegen files
#include "../../runtime/stdlib/mlp_string.h"  // For function signatures
```

**Note:** Not strictly needed since we're just generating `call` instructions.

---

## 🧪 Test Programs

### Test 1: Basic Concatenation
```mlp
function main() returns numeric
    text a = "Hello"
    text b = "World"
    text result = a + b
    println(result)
    return 0
end function
```

**Expected Output:** `HelloWorld`

---

### Test 2: Multi-operand Concatenation
```mlp
function main() returns numeric
    text greeting = "Hello"
    text space = " "
    text target = "MELP"
    text message = greeting + space + target
    println(message)
    return 0
end function
```

**Expected Output:** `Hello MELP`

**Note:** `a + b + c` becomes:
1. `temp1 = a + b` → call mlp_string_concat(a, b)
2. `result = temp1 + c` → call mlp_string_concat(temp1, c)

---

### Test 3: String Equality
```mlp
function main() returns numeric
    text password = "secret"
    if password == "secret"
        println("Match!")
        return 1
    else
        println("No match")
        return 0
    end if
end function
```

**Expected Output:** `Match!`

---

### Test 4: String Ordering
```mlp
function main() returns numeric
    text a = "apple"
    text b = "banana"
    if a < b
        println("apple < banana")
        return 1
    end if
    return 0
end function
```

**Expected Output:** `apple < banana`

---

## ⚠️ Important Notes

### Memory Management
- `mlp_string_concat()` returns heap-allocated string
- **Problem:** No automatic cleanup yet!
- **Future:** Garbage collection or reference counting
- **For now:** Memory leak is acceptable (proof of concept)

### String Literals vs Variables
- **Literals:** Stored in `.rodata` (read-only)
- **Variables:** Pointer to string (stack or heap)
- **Both:** Handled the same way (just pointers)

### Type Checking
- Compiler tracks `is_numeric` flag per variable
- Literals: Check token type during parsing
- Mixed operations: Not yet supported (numeric + text → error?)

---

## 🎯 Success Criteria

**Minimum:**
- [x] Runtime functions compile ✅
- [ ] Codegen for `text c = a + b`
- [ ] Codegen for `if str == "literal"`
- [ ] At least one test passing

**Complete:**
- [ ] All 6 comparison operators work
- [ ] Multi-operand concatenation
- [ ] All test programs passing
- [ ] No crashes or segfaults

---

## 📚 Reference

**TTO Principles:**
- User sees only `numeric` and `text`
- Compiler tracks 1 bit: `is_numeric`
- Runtime optimizes transparently

**Key Functions:**
```c
// Type tracking (functions.c)
int function_get_var_is_numeric(FunctionDeclaration* func, const char* name);

// Runtime (mlp_string.c)
char* mlp_string_concat(const char* str1, const char* str2);
int mlp_string_compare(const char* str1, const char* str2);
```

**Assembly Pattern:**
```asm
# Load operands
movq <left>, %r8
movq <right>, %r9

# Dispatch based on type
movq %r8, %rdi      # arg1
movq %r9, %rsi      # arg2
call <function>     # Returns in %rax
movq %rax, %r8      # Move to result register
```

---

**Created:** 9 Aralık 2025, 21:00  
**Author:** YZ_06  
**Status:** Implementation guide complete, ready for codegen work
