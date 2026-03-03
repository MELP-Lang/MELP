# Variable Declaration - MELP Syntax

**Durum:** ✅ COMPLETE (Stage 0)  
**Sözdizimi Sürümü:** pmlp0 / Stage0  

**Stage:** pmlp0 (Stage 0)  
**Module:** Variable declaration and assignment

---

## 📖 BASIC SYNTAX

```pmpl
type name = value
```

---

## ✅ BASIC TYPES

### Numeric
```pmpl
numeric x = 42
numeric pi = 3,14159
numeric million = 1.000.000
```

### String
```pmpl
string name = "Ali"
string greeting = "Hello, World!"
string empty = ""
```

### Boolean
```pmpl
boolean flag = true
boolean is_valid = false
```

---

## ⚠️ CRITICAL RULES

1. **NO semicolon at end of statement!**
   - ✅ `numeric x = 42`
   - ❌ `numeric x = 42;` (semicolon - WRONG!)

2. **Type comes first**
   - ✅ `numeric x = 42`
   - ❌ `x: numeric = 42` (wrong order)

3. **Turkish number format**
   - `,` (comma) = decimal separator: `3,14`
   - `.` (dot) = thousands separator: `1.000`

4. **String type name: `string` (not `text`)**
   - ✅ `string name = "Ali"`
   - ❌ `text name = "Ali"` (deprecated)

---

## 🚫 ANTI-PATTERNS

### ❌ Python-style
```python
# WRONG - This is Python, not MELP!
x = 42
name = "Ali"
flag = True
```

### ❌ JavaScript/TypeScript-style
```javascript
// WRONG - This is JavaScript, not MELP!
let x = 42;
const name = "Ali";
var flag = true;
```

### ❌ C-style
```c
// WRONG - This is C, not MELP!
int x = 42;
char* name = "Ali";
bool flag = true;
```

### ✅ CORRECT MELP
```pmpl
numeric x = 42
string name = "Ali"
boolean flag = true
```

---

## 📚 ASSIGNMENT (After Declaration)

### Simple Assignment
```pmpl
numeric x = 42
x = 100  -- Assignment (no type needed)
```

### Expression Assignment
```pmpl
numeric a = 10
numeric b = 20
numeric sum = a + b
sum = sum + 5
```

---

## 📚 TURKISH NUMBER FORMAT

### Decimal Numbers
```pmpl
numeric pi = 3,14159          -- 3.14159
numeric e = 2,71828           -- 2.71828
numeric half = 0,5            -- 0.5
```

### Large Numbers
```pmpl
numeric thousand = 1.000      -- 1,000
numeric million = 1.000.000   -- 1,000,000
numeric big = 126.555.852,36  -- 126,555,852.36
```

**Rule:** 
- Comma `,` = decimal point
- Dot `.` = thousands separator

---

## 📚 PRACTICAL EXAMPLES

### Multiple Variables
```pmpl
numeric x = 10
numeric y = 20
numeric z = 30
```

### Calculated Values
```pmpl
numeric width = 100
numeric height = 50
numeric area = width * height
```

### String Operations
```pmpl
string first = "Ali"
string last = "Veli"
string full = first + " " + last
```

---

## 📚 GOLDEN REFERENCE

**File:** [test_pmlp_golden_reference.mlp](../../test_pmlp_golden_reference.mlp)  
**Search for:** Variable declaration examples in golden reference

---

**Reference:** [pmlp0_syntax.md](../pmlp0_syntax.md) | [pmlp_sozdizimi.md](../../pmlp_sozdizimi.md)
