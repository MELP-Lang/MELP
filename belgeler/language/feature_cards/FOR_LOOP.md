# For Loop - MELP Syntax

**Durum:** ⚠️ PARTIAL (Stage 1 — downto/step L-04'te eklenecek)  
**Sözdizimi Sürümü:** pmlp0 / Stage0  

**Stage:** pmlp0 (Stage 0)  
**Module:** For loop control structure

---

## 📖 BASIC SYNTAX (Ascending)

```pmpl
for variable = start to end
    -- body
end_for
```

---

## 📖 DESCENDING SYNTAX

```pmpl
for variable = start downto end
    -- body
end_for
```

---

## ✅ COMPLETE EXAMPLES

### Ascending Loop
```pmpl
for i = 0 to 10
    print(i)
end_for
-- Prints: 0, 1, 2, ..., 10
```

### Descending Loop
```pmpl
for i = 10 downto 0
    print(i)
end_for
-- Prints: 10, 9, 8, ..., 0
```

### With Step (Optional)
```pmpl
for i = 0 to 100 step 10
    print(i)
end_for
-- Prints: 0, 10, 20, ..., 100
```

---

## ⚠️ CRITICAL RULES

1. **Use `to` or `downto` keywords**
   - ✅ `for i = 0 to 10`
   - ✅ `for i = 10 downto 0`
   - ❌ `for i in range(0, 10)` (Python-style - WRONG!)

2. **Block terminator: `end_for`**
   - ✅ `end_for` (underscore, single token)
   - ❌ `end for` (two tokens - WRONG!)

3. **No parentheses**
   - ✅ `for i = 0 to 10`
   - ❌ `for (i = 0; i < 10; i++)` (C-style - WRONG!)

4. **Step is optional**
   - Default step: 1 (ascending) or -1 (descending)

---

## 🚫 ANTI-PATTERNS

### ❌ Python-style
```python
# WRONG - This is Python, not MELP!
for i in range(10):
    print(i)
```

### ❌ C-style
```c
// WRONG - This is C, not MELP!
for (int i = 0; i < 10; i++) {
    printf("%d\n", i);
}
```

### ❌ Missing `to`/`downto`
```pmpl
-- WRONG!
for i = 0, 10
    print(i)
end_for
```

### ✅ CORRECT MELP
```pmpl
for i = 0 to 10
    print(i)
end_for
```

---

## 📚 PRACTICAL EXAMPLES

### Array Iteration
```pmpl
numeric[] numbers = [1; 2; 3; 4; 5]
for i = 0 to 4
    print(numbers[i])
end_for
```

### Sum Calculation
```pmpl
numeric sum = 0
for i = 1 to 100
    sum = sum + i
end_for
print(sum)  -- 5050
```

### Countdown
```pmpl
for count = 10 downto 1
    print(count)
end_for
print("Liftoff!")
```

---

## 📚 GOLDEN REFERENCE

**File:** [test_pmlp_golden_reference.mlp](../../test_pmlp_golden_reference.mlp)  
**Search for:** "for" examples in golden reference

---

**Reference:** [pmlp0_syntax.md](../pmlp0_syntax.md) | [pmlp_sozdizimi.md](../../pmlp_sozdizimi.md)
