# Function Definition - MELP Syntax

**Durum:** ✅ COMPLETE (Stage1)  
**Sözdizimi Sürümü:** mlp1 / Stage1  

**Stage:** mlp1 (Stage1)  
**Module:** Function declaration and definition

---

## 📖 BASIC SYNTAX

```mlp
return_type function name(type param1; type param2)
    -- body
    return value
end function
```

---

## ✅ COMPLETE EXAMPLES

### Simple Function
```mlp
numeric function add(numeric x; numeric y)
    return x + y
end function

numeric result = add(10; 20)  -- 30
```

### String Function
```mlp
string function greet(string name)
    return "Hello, " + name
end function

string message = greet("Ali")  -- "Hello, Ali"
```

### No Return Value
```mlp
function print_greeting(string name)
    print("Hello, " + name)
end function

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

3. **Return type: before `function` keyword**
   - ✅ `numeric function foo()`
   - ❌ `function foo() -> numeric` (wrong position)
   - ❌ `function foo(): numeric` (colon - WRONG!)

4. **Block terminator: `end function`**
   - ✅ `end function` (two words, with space)

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
```mlp
-- WRONG!
function add(numeric x, numeric y) -> numeric
    return x + y
end function
```

### ✅ CORRECT MELP
```mlp
numeric function add(numeric x; numeric y)
    return x + y
end function
```

---

## 📚 PRACTICAL EXAMPLES

### Multiple Operations
```mlp
numeric function calculate(numeric a; numeric b)
    numeric sum = a + b
    numeric product = a * b
    return sum + product
end function
```

### Boolean Function
```mlp
boolean function is_positive(numeric x)
    if x > 0 then
        return true
    else
        return false
    end if
end function
```

### Complex Logic
```mlp
numeric function max(numeric a; numeric b; numeric c)
    if a > b and a > c then
        return a
    else
        if b > c then
            return b
        else
            return c
        end if
    end if
end function
```

---

## 📚 GOLDEN REFERENCE

**File:** [test_mlp_golden_reference.mlp](../../test_mlp_golden_reference.mlp)  
**Search for:** "function" examples in golden reference

---

**Reference:** [mlp0_syntax.md](../mlp0_syntax.md) | [mlp_sozdizimi.md](../../mlp_sozdizimi.md)
