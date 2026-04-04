# Loop Statement - MELP Syntax

**Stage:** mlp1 (Stage1)  
**Module:** Loop control structure (replaces while/do-while)

---

## 📖 BASIC SYNTAX

```mlp
-- Condition at start (pre-check)
loop condition
    -- body
end loop

-- Condition at end (post-check, like do-loop)
loop
    -- body
end loop condition

-- Infinite loop
loop
    -- body (use exit to break)
end loop
```

---

## ✅ COMPLETE EXAMPLES

### Pre-check Loop (like while)
```mlp
numeric i = 0
loop i < 10
    print(i)
    i = i + 1
end loop
```

### Post-check Loop (runs at least once)
```mlp
numeric i = 0
loop
    print(i)
    i = i + 1
end loop i < 10
```

### Infinite Loop
```mlp
loop
    if done then exit
    process()
end loop
```

### Until Semantics (with not)
```mlp
-- "repeat UNTIL i >= 10"
loop not i >= 10
    print(i)
    i = i + 1
end loop
-- Same as: loop i < 10
```

---

## ⚠️ CRITICAL RULES

1. **Condition position determines semantics:**
   - Start: pre-check (like while)
   - End: post-check (like do-while)
   - None: infinite loop

2. **Block terminator: `end loop`**
   - ✅ `end loop` (two words, with space)
   - ❌ `end loop` (underscore form — WRONG!)
   - ❌ `end while` (old keyword — WRONG!)

3. **`not` operator creates until semantics**
   - ✅ `loop not done` = "until done"
   - No special `until` keyword needed

4. **Use `loop`, not `while`**
   - ✅ `loop condition`
   - ❌ `while condition` (old keyword — WRONG!)

---

## 🚫 ANTI-PATTERNS

### ❌ Python-style
```python
# WRONG - This is Python, not MELP!
loop i < 10:
    print(i)
```

### ❌ C-style
```c
// WRONG - This is C, not MELP!
loop (i < 10) {
    print(i);
}
```

### ❌ Using old `while` keyword
```mlp
-- WRONG! (use loop instead of while)
loop i < 10
    print(i)
end loop
```

### ✅ CORRECT MELP
```mlp
loop i < 10
    print(i)
    i = i + 1
end loop
```

---

## 📚 GOLDEN REFERENCE

**File:** [golden_reference0.mlp](../golden_reference0.mlp)  
**Search for:** "loop" examples in golden reference

---

**Reference:** [mlp0_syntax.md](../mlp0_syntax.md) | [mlp_sozdizimi.md](../../mlp_sozdizimi.md)
