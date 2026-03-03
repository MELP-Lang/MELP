# While Loop - MELP Syntax

**Stage:** pmlp0 (Stage 0)  
**Module:** While loop control structure

---

## 📖 BASIC SYNTAX

```pmpl
while condition
    -- body
end_while
```

---

## ✅ COMPLETE EXAMPLE

```pmpl
numeric i = 0
while i < 10
    print(i)
    i = i + 1
end_while
```

---

## ⚠️ CRITICAL RULES

1. **NO `do` keyword!**
   - ✅ `while condition` 
   - ❌ `while condition do` (WRONG!)

2. **Block terminator: `end_while`**
   - ✅ `end_while` (underscore, single token)
   - ❌ `end while` (two tokens - WRONG!)

3. **Condition is boolean expression**
   - No parentheses required
   - ✅ `while i < 10`
   - ❌ `while (i < 10)` (unnecessary parentheses)

---

## 🚫 ANTI-PATTERNS

### ❌ Python-style
```python
# WRONG - This is Python, not MELP!
while i < 10:
    print(i)
```

### ❌ C-style
```c
// WRONG - This is C, not MELP!
while (i < 10) {
    print(i);
}
```

### ❌ Using `do`
```pmpl
-- WRONG!
while i < 10 do
    print(i)
end_while
```

### ✅ CORRECT MELP
```pmpl
while i < 10
    print(i)
    i = i + 1
end_while
```

---

## 📚 GOLDEN REFERENCE

**File:** [test_pmlp_golden_reference.mlp](../../test_pmlp_golden_reference.mlp)  
**Search for:** "while" examples in golden reference

---

**Reference:** [pmlp0_syntax.md](../pmlp0_syntax.md) | [pmlp_sozdizimi.md](../../pmlp_sozdizimi.md)
