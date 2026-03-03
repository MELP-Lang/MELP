# If Statement - MELP Syntax

**Durum:** ✅ COMPLETE (Stage 0)  
**Sözdizimi Sürümü:** pmlp0 / Stage0  

**Stage:** pmlp0 (Stage 0)  
**Module:** Conditional control structure

---

## 📖 BASIC SYNTAX

```pmpl
if condition then
    -- body
end_if
```

---

## ✅ WITH ELSE

```pmpl
if condition then
    -- true branch
else
    -- false branch
end_if
```

---

## ✅ WITH ELSE_IF

```pmpl
if condition1 then
    -- first branch
else_if condition2 then
    -- second branch
else_if condition3 then
    -- third branch
else
    -- default branch
end_if
```

---

## ⚠️ CRITICAL RULES

1. **`if` MUST have `then`!**
   - ✅ `if x > 5 then`
   - ❌ `if x > 5` (missing `then` - WRONG!)
   - ❌ `if x > 5:` (colon instead of `then` - WRONG!)

2. **Block terminator: `end_if`**
   - ✅ `end_if` (underscore, single token)
   - ❌ `end if` (two tokens - WRONG!)

3. **Else-if is single token: `else_if`**
   - ✅ `else_if condition then`
   - ❌ `else if` (two tokens - WRONG!)
   - ❌ `elif` (Python-style - WRONG!)

4. **No parentheses required**
   - ✅ `if x > 5 then`
   - ⚠️ `if (x > 5) then` (works but unnecessary)

---

## 🚫 ANTI-PATTERNS

### ❌ Python-style
```python
# WRONG - This is Python, not MELP!
if x > 5:
    print("large")
elif x > 2:
    print("medium")
else:
    print("small")
```

### ❌ C-style
```c
// WRONG - This is C, not MELP!
if (x > 5) {
    printf("large");
} else if (x > 2) {
    printf("medium");
} else {
    printf("small");
}
```

### ❌ Missing `then`
```pmpl
-- WRONG!
if x > 5
    print("large")
end_if
```

### ✅ CORRECT MELP
```pmpl
if x > 5 then
    print("large")
else_if x > 2 then
    print("medium")
else
    print("small")
end_if
```

---

## 📚 COMPLETE EXAMPLE

```pmpl
numeric score = 85

if score >= 90 then
    print("Grade: A")
else_if score >= 80 then
    print("Grade: B")
else_if score >= 70 then
    print("Grade: C")
else_if score >= 60 then
    print("Grade: D")
else
    print("Grade: F")
end_if
```

---

## 📚 GOLDEN REFERENCE

**File:** [test_pmlp_golden_reference.mlp](../../test_pmlp_golden_reference.mlp)  
**Search for:** "if" examples in golden reference

---

**Reference:** [pmlp0_syntax.md](../pmlp0_syntax.md) | [pmlp_sozdizimi.md](../../pmlp_sozdizimi.md)
