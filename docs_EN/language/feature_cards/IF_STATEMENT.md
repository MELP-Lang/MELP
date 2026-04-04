# If Statement - MELP Syntax

**Durum:** ✅ COMPLETE (Stage1)  
**Sözdizimi Sürümü:** mlp1 / Stage1  

**Stage:** mlp1 (Stage1)  
**Module:** Conditional control structure

---

## 📖 BASIC SYNTAX

```mlp
if condition then
    -- body
end if
```

---

## ✅ WITH ELSE

```mlp
if condition then
    -- true branch
else
    -- false branch
end if
```

---

## ✅ WITH ELSE IF

```mlp
if condition1 then
    -- first branch
else
    if condition2 then
        -- second branch
    else
        if condition3 then
            -- third branch
        else
            -- default branch
        end if
    end if
end if
```

---

## ⚠️ CRITICAL RULES

1. **`if` MUST have `then`!**
   - ✅ `if x > 5 then`
   - ❌ `if x > 5` (missing `then` - WRONG!)
   - ❌ `if x > 5:` (colon instead of `then` - WRONG!)

2. **Block terminator: `end if`**
    - ✅ `end if` (two words, with space)

3. **Else-if uses space: `else if`**
    - ✅ `else if condition then`
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
```mlp
-- WRONG!
if x > 5
    print("large")
end if
```

### ✅ CORRECT MELP
```mlp
if x > 5 then
    print("large")
else
    if x > 2 then
        print("medium")
    else
        print("small")
    end if
end if
```

---

## 📚 COMPLETE EXAMPLE

```mlp
numeric score = 85

if score >= 90 then
    print("Grade: A")
else
    if score >= 80 then
        print("Grade: B")
    else
        if score >= 70 then
            print("Grade: C")
        else
            if score >= 60 then
                print("Grade: D")
            else
                print("Grade: F")
            end if
        end if
    end if
end if
```

---

## 📚 GOLDEN REFERENCE

**File:** [test_mlp_golden_reference.mlp](../../test_mlp_golden_reference.mlp)  
**Search for:** "if" examples in golden reference

---

**Reference:** [mlp0_syntax.md](../mlp0_syntax.md) | [mlp_sozdizimi.md](../../mlp_sozdizimi.md)
