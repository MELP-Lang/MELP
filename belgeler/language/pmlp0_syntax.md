# PMLP0 SYNTAX - Stage 0 Minimal

**Stage:** Stage 0 (Currently Implemented)  
**Purpose:** Minimal syntax for basic programs  
**Target:** Parser implementation, YZ training  
**Lines:** ~100 (need-to-know basis only!)  
**Son Güncelleme:** 19 Şubat 2026  
**Kanonik Kaynak:** [`belgeler/pmlp0_sozdizimi.md`](../pmlp0_sozdizimi.md) (677 satır)

---

## ⚠️ CRITICAL RULES

1. **Semicolon (`;`)** - ONLY for separators (params, array/list elements)
   - ✅ `func(a; b; c)` - Parameters
   - ✅ `[1; 2; 3]` - Array elements
   - ❌ `x = 42;` - NO statement terminator!

2. **Block Terminators** - Underscore, single token
   - ✅ `end_if`, `end_while`, `end_for`, `end_function`
   - ❌ `end if` (two tokens - WRONG!)

3. **Exit/Continue** - `exit` (NOT `break`!)
   - ✅ `exit` - Exit current block
   - ✅ `continue` - Continue loop
   - ❌ `break` - Doesn't exist!

4. **Return Type** - Use `as` (NOT `returns`)
   - ✅ `function foo() as numeric`
   - ❌ `function foo() returns numeric`

5. **String Type** - Use `string` (NOT `text`)
   - ✅ `string name = "Ali"`
   - ❌ `text name = "Ali"`

---

## 🔤 BASIC TYPES

```pmpl
numeric x = 42
string name = "Ali"
boolean flag = true
```

---/
String literal içinde desteklenen kaçış karakterleri:
- `\n` → yeni satır
- `\t` → yatay sekme (tab)
- `\"` → çift tırnak karakteri
- `\\` → ters bölü işareti

Not: Bu kaçışlar string tokenizasyonunda çözülür.
(GPT-5.3_14 Şubat 2026)
/---

**Turkish Number Format:**
```pmpl
numeric pi = 3,14159        -- 3.14159 (comma = decimal)
numeric million = 1.000.000  -- 1,000,000 (dot = thousands)
```

---

## 🔄 CONTROL FLOW

### If Statement
```pmpl
if x > 5 then
    print("large")
else_if x > 2 then
    print("medium")
else
    print("small")
end_if
```

**Critical:** `if` MUST have `then`!

### While Loop
```pmpl
while i < 10
    print(i)
    i = i + 1
end_while
```

**Critical:** NO `do` keyword! `while condition` only!

### For Loop
```pmpl
-- Ascending
for i = 0 to 10
    print(i)
end_for

-- Descending
for i = 10 downto 0
    print(i)
end_for
```

---

## 🔧 FUNCTIONS

```pmpl
function add(numeric x; numeric y) as numeric
    return x + y
end_function

function greet(string name) as string
    return "Hello, " + name
end_function
```

**Stage 1 Notu:** `main()` fonksiyonunda `return 0` opsiyoneldir.

```pmpl
function main()
    print("hello")
end_function
```

Derleyici `main()` sonunda implicit `ret i64 0` ekler.
Non-main fonksiyonlarda explicit `return` zorunludur.

**Parameter Format:** `type name` (e.g., `numeric x`)  
**Separator:** `;` (semicolon between parameters)

---

## ⚙️ OPERATORS

### Arithmetic
```pmpl
+ - * / mod **
```

**Example:**
```pmpl
numeric result = (10 + 5) * 2
numeric remainder = 17 mod 5
numeric power = 2 ** 8
```

### Comparison
```pmpl
== != < > <= >=
```

### Logical
```pmpl
and or not
```

**Example:**
```pmpl
if x > 0 and x < 100 then
    print("valid")
end_if
```

---

## 📋 COLLECTIONS (Basic)

### Array (Homogeneous)
```pmpl
numeric[] numbers = [1; 2; 3; 4; 5]
numbers[0] = 100
```

### List (Heterogeneous)
```pmpl
list data = ("Ali"; 25; true)
data[0] = "Veli"
```

**Critical:** Separator is `;` (semicolon), NOT comma!

---

## � STRUCT (Minimal — AST İçin)

```pmpl
struct ASTNode
    string node_type
    numeric value
    list children
end_struct

struct Token
    string type
    string value
    numeric line
end_struct
```

**Oluşturma ve Kullanım:**
```pmpl
-- Instance oluşturma
Token tok = Token{"NUMBER"; "42"; 1}

-- Field erişimi (dot notation)
print(tok.type)    -- "NUMBER"

-- Field güncelleme
tok.value = "99"
```

**MELP Compliance:**
- ✔️ Blok sonlandırıcı: `end_struct`
- ✔️ Sadece data container (method YOK Stage 0'da)
- ✔️ Parametre ayırıcı `;` (struct literal içinde)

---

## �💬 COMMENTS

```pmpl
-- Single line comment

---
Multi-line comment
Can span multiple lines
---
```

---

## 🚫 ANTI-PATTERNS (Common Mistakes)

### ❌ Python-style while
```pmpl
-- WRONG!
while i < 10:
    print(i)
```

### ❌ C-style parentheses
```pmpl
-- WRONG!
while (i < 10) {
    print(i);
}
```

### ❌ Statement terminator semicolon
```pmpl
-- WRONG!
numeric x = 42;
string name = "Ali";
```

### ❌ Comma as parameter separator
```pmpl
-- WRONG!
function add(numeric x, numeric y)
    return x + y
end_function
```

### ✅ CORRECT MELP Style
```pmpl
function add(numeric x; numeric y) as numeric
    return x + y
end_function

numeric result = add(10; 20)
```

---

**Reference:** [pmlp_sozdizimi.md](../pmlp_sozdizimi.md) (Full specification - 1994 lines)  
**Feature Cards:** See `feature_cards/` directory for detailed syntax per feature
