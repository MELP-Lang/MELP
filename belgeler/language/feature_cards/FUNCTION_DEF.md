# Function Definition - MELP Syntax

**Durum:** ✅ COMPLETE (Stage 0)  
**Sözdizimi Sürümü:** pmlp0 / Stage0  

**Stage:** pmlp0 (Stage 0)  
**Module:** Function declaration and definition

---

## 📖 BASIC SYNTAX

```pmpl
function name(type param1; type param2) as return_type
    -- body
    return value
end_function
```

---

## ✅ COMPLETE EXAMPLES

### Simple Function
```pmpl
function add(numeric x; numeric y) as numeric
    return x + y
end_function

numeric result = add(10; 20)  -- 30
```

### String Function
```pmpl
function greet(string name) as string
    return "Hello, " + name
end_function

string message = greet("Ali")  -- "Hello, Ali"
```

### No Return Value
```pmpl
function print_greeting(string name) as none
    print("Hello, " + name)
end_function

print_greeting("Veli")
```

---

## ⚠️ CRITICAL RULES

1. **Parameter format: `type name`**
   - ✅ `function add(numeric x; numeric y)`
   - ❌ `function add(x: numeric, y: numeric)` (wrong order)

2. **Parameter separator: `;` (semicolon)**
   - ✅ `(numeric x; numeric y)`
   - ❌ `(numeric x, numeric y)` (comma - WRONG!)

3. **Return type: `as type`**
   - ✅ `function foo() as numeric`
   - ❌ `function foo() returns numeric` (deprecated)
   - ❌ `function foo(): numeric` (colon - WRONG!)

4. **Block terminator: `end_function`**
   - ✅ `end_function` (underscore, single token)
   - ❌ `end function` (two tokens - WRONG!)

5. **Return statement required (except `as none`)**
   - ✅ Must have `return` if return type is not `none`

---

## 🚫 ANTI-PATTERNS

### ❌ Python-style
```python
# WRONG - This is Python, not MELP!
def add(x, y):
    return x + y
```

### ❌ C-style
```c
// WRONG - This is C, not MELP!
int add(int x, int y) {
    return x + y;
}
```

### ❌ JavaScript-style
```javascript
// WRONG - This is JavaScript, not MELP!
function add(x, y) {
    return x + y;
}
```

### ❌ Comma separator
```pmpl
-- WRONG!
function add(numeric x, numeric y) as numeric
    return x + y
end_function
```

### ✅ CORRECT MELP
```pmpl
function add(numeric x; numeric y) as numeric
    return x + y
end_function
```

---

## 📚 PRACTICAL EXAMPLES

### Multiple Operations
```pmpl
function calculate(numeric a; numeric b) as numeric
    numeric sum = a + b
    numeric product = a * b
    return sum + product
end_function
```

### Boolean Function
```pmpl
function is_positive(numeric x) as boolean
    if x > 0 then
        return true
    else
        return false
    end_if
end_function
```

### Complex Logic
```pmpl
function max(numeric a; numeric b; numeric c) as numeric
    if a > b and a > c then
        return a
    else_if b > c then
        return b
    else
        return c
    end_if
end_function
```

---

## 📚 GOLDEN REFERENCE

**File:** [test_pmlp_golden_reference.mlp](../../test_pmlp_golden_reference.mlp)  
**Search for:** "function" examples in golden reference

---

**Reference:** [pmlp0_syntax.md](../pmlp0_syntax.md) | [pmlp_sozdizimi.md](../../pmlp_sozdizimi.md)
