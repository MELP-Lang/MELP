# For Loop - MELP Syntax

**Durum:** ⚠️ PARTIAL (Stage 1 — downto/step L-04'te eklenecek)  
**Sözdizimi Sürümü:** mlp1 / Stage1  

**Stage:** mlp1 (Stage1)  
**Module:** For loop control structure

---

## 📖 BASIC SYNTAX (Ascending)

```mlp
loop variable = start to end
    -- body
end loop
```

---

## 📖 DESCENDING SYNTAX

```mlp
loop variable = start downto end
    -- body
end loop
```

---

## ✅ COMPLETE EXAMPLES

### Ascending Loop
```mlp
loop i = 0 to 10
    print(i)
end loop
-- Prints: 0, 1, 2, ..., 10
```

### Descending Loop
```mlp
loop i = 10 downto 0
    print(i)
end loop
-- Prints: 10, 9, 8, ..., 0
```

### With Step (Optional)
```mlp
loop i = 0 to 100 step 10
    print(i)
end loop
-- Prints: 0, 10, 20, ..., 100
```

---

## ⚠️ CRITICAL RULES

1. **Use `to` or `downto` keywords**
   - ✅ `for i = 0 to 10`
   - ✅ `for i = 10 downto 0`
   - ❌ `for i in range(0, 10)` (Python-style - WRONG!)

2. **Block terminator: `end for`**
   - ✅ `end for` (two words, with space)
   - ❌ `end for` (underscore form — WRONG!)

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
loop i in range(10):
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
```mlp
-- WRONG!
loop i = 0, 10
    print(i)
end loop
```

### ✅ CORRECT MELP
```mlp
loop i = 0 to 10
    print(i)
end loop
```

---

## 📚 PRACTICAL EXAMPLES

### Array Iteration
```mlp
numeric[] numbers = [1; 2; 3; 4; 5]
loop i = 0 to 4
    print(numbers[i])
end loop
```

### Sum Calculation
```mlp
numeric sum = 0
loop i = 1 to 100
    sum = sum + i
end loop
print(sum)  -- 5050
```

### Countdown
```mlp
loop count = 10 downto 1
    print(count)
end loop
print("Liftoff!")
```

---

## 📚 GOLDEN REFERENCE

**File:** [test_mlp_golden_reference.mlp](../../test_mlp_golden_reference.mlp)  
**Search for:** "for" examples in golden reference

---

**Reference:** [mlp0_syntax.md](../mlp0_syntax.md) | [mlp_sozdizimi.md](../../mlp_sozdizimi.md)
