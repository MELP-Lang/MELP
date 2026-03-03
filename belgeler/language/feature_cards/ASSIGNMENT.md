# Assignment — MELP Syntax

**Durum:** ✅ COMPLETE (Stage 0)  
**Sözdizimi Sürümü:** pmlp0 / Stage0  

**Stage:** pmlp0 (Stage 0)  
**Module:** Variables — assignment and reassignment

---

## 📖 BASIC SYNTAX

```melp
-- Initial declaration + assignment
TypeName varName = expression

-- Reassignment (type already known)
varName = expression
```

---

## ✅ COMPLETE EXAMPLES

### Declare and assign
```melp
numeric x = 10
print(x)
-- Prints: 10
```

### Reassign
```melp
numeric count = 0
count = 5
count = count + 1
print(count)
-- Prints: 6
```

### Assign inside loop
```melp
numeric last = 0
for i = 1 to 5
    last = i
end_for
print(last)
-- Prints: 5
```

### String assignment
```melp
string name = "Alice"
name = "Bob"
print(name)
-- Prints: Bob
```

### Assign from function return
```melp
function square(n: numeric) -> numeric
    return n * n
end_function

numeric result = square(7)
print(result)
-- Prints: 49
```

---

## ⚠️ CRITICAL RULES

- Declaration requires the type keyword (`numeric`, `string`, etc.).
- Reassignment uses the bare variable name — **no** type keyword.
- The type of a variable cannot change after declaration.
- Assignments inside loops correctly update the declared variable in the outer scope.
- Compound operators (`+=`, `-=`, etc.) are **not** currently supported; use `x = x + n`.

---

## 🔗 RELATED CARDS

- [VARIABLE.md](VARIABLE.md) — variable declarations
- [BASIC_TYPES.md](BASIC_TYPES.md) — available types
- [FOR_LOOP.md](FOR_LOOP.md) — loops with assignment
- [OPERATORS.md](OPERATORS.md) — arithmetic operators
