# Operators - MELP Syntax

**Durum:** ⚠️ PARTIAL (Stage1/1 — temel var; xor/power L-10'da eklendi, operator overloading eksik)  
**Sözdizimi Sürümü:** mlp1 / Stage1  

**Stage:** mlp1 (Stage1)  
**Module:** Arithmetic, comparison, and logical operators

---

## ⚙️ ARITHMETIC OPERATORS

| Operator | Meaning | Example | Result |
|----------|---------|---------|--------|
| `+` | Addition | `10 + 5` | `15` |
| `-` | Subtraction | `10 - 5` | `5` |
| `*` | Multiplication | `10 * 5` | `50` |
| `/` | Division | `10 / 5` | `2` |
| `mod` | Modulo | `17 mod 5` | `2` |
| `**` | Power | `2 ** 8` | `256` |

### Examples
```mlp
numeric a = 10 + 5           -- 15
numeric b = 20 - 8           -- 12
numeric c = 6 * 7            -- 42
numeric d = 100 / 4          -- 25
numeric e = 17 mod 5         -- 2
numeric f = 2 ** 10          -- 1024
```

---

## 🔍 COMPARISON OPERATORS

| Operator | Meaning | Example | Result |
|----------|---------|---------|--------|
| `==` | Equal | `5 == 5` | `true` |
| `!=` | Not equal | `5 != 3` | `true` |
| `<` | Less than | `3 < 5` | `true` |
| `>` | Greater than | `5 > 3` | `true` |
| `<=` | Less or equal | `3 <= 5` | `true` |
| `>=` | Greater or equal | `5 >= 5` | `true` |

### Examples
```mlp
boolean b1 = 5 == 5          -- true
boolean b2 = 5 != 3          -- true
boolean b3 = 3 < 5           -- true
boolean b4 = 5 > 3           -- true
boolean b5 = 3 <= 5          -- true
boolean b6 = 5 >= 5          -- true
```

---

## 🔀 LOGICAL OPERATORS

| Operator | Meaning | Example | Result |
|----------|---------|---------|--------|
| `and` | Logical AND | `true and false` | `false` |
| `or` | Logical OR | `true or false` | `true` |
| `not` | Logical NOT | `not true` | `false` |

### Examples
```mlp
boolean b1 = true and true   -- true
boolean b2 = true and false  -- false
boolean b3 = true or false   -- true
boolean b4 = false or false  -- false
boolean b5 = not true        -- false
boolean b6 = not false       -- true
```

---

## ⚠️ CRITICAL RULES

1. **Use `mod` (NOT `%`)**
   - ✅ `17 mod 5`
   - ❌ `17 % 5` (deprecated)

2. **Use words for logical operators**
   - ✅ `and`, `or`, `not`
   - ❌ `&&`, `||`, `!` (C-style - WRONG!)

3. **No semicolon at end!**
   - ✅ `numeric x = 10 + 5`
   - ❌ `numeric x = 10 + 5;`

---

## 🚫 ANTI-PATTERNS

### ❌ C-style operators
```c
// WRONG - This is C, not MELP!
int x = 17 % 5;
bool b = true && false;
bool c = !true;
```

### ❌ Python-style operators
```python
# WRONG - This is Python, not MELP!
x = 17 % 5
b = True and False
c = not True
```

### ✅ CORRECT MELP
```mlp
numeric x = 17 mod 5
boolean b = true and false
boolean c = not true
```

---

## 📚 OPERATOR PRECEDENCE (High to Low)

1. `**` (Power)
2. `*`, `/`, `mod` (Multiplication, Division, Modulo)
3. `+`, `-` (Addition, Subtraction)
4. `<`, `>`, `<=`, `>=` (Comparison)
5. `==`, `!=` (Equality)
6. `not` (Logical NOT)
7. `and` (Logical AND)
8. `or` (Logical OR)

### Example
```mlp
numeric result = 2 + 3 * 4    -- 14 (not 20)
numeric power = 2 ** 3 + 1    -- 9 (not 16)
boolean complex = 5 > 3 and 10 < 20  -- true
```

---

## 📚 PRACTICAL EXAMPLES

### Complex Expression
```mlp
numeric x = (10 + 5) * 2 - 3    -- 27
numeric y = 2 ** (3 + 1)        -- 16
```

### Conditional Logic
```mlp
numeric age = 25
boolean is_adult = age >= 18 and age < 65

if is_adult then
    print("Adult")
end if
```

### Range Check
```mlp
numeric score = 85
boolean passed = score >= 60 and score <= 100
```

---

## 📚 GOLDEN REFERENCE

**File:** [test_mlp_golden_reference.mlp](../../test_mlp_golden_reference.mlp)  
**Search for:** Operator usage examples in golden reference

---

**Reference:** [mlp0_syntax.md](../mlp0_syntax.md) | [mlp_sozdizimi.md](../../mlp_sozdizimi.md)
