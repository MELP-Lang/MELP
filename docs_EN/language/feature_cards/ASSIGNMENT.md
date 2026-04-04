# Assignment — MELP Syntax

**Durum:** ✅ COMPLETE (Stage1)  
**Sözdizimi Sürümü:** mlp1 / Stage1  

**Stage:** mlp1 (Stage1)  
**Module:** Variables — assignment and reassignment

---

## 📖 BASIC SYNTAX

```mlp
-- Initial declaration + assignment
TypeName varName = expression

-- Reassignment (type already known)
varName = expression
```

---

## ✅ COMPLETE EXAMPLES

### Declare and assign
```mlp
numeric x = 10
print(x)
-- Prints: 10
```

### Reassign
```mlp
numeric count = 0
count = 5
count = count + 1
print(count)
-- Prints: 6
```

### Assign inside loop
```mlp
numeric last = 0
loop i = 1 to 5
    last = i
end loop
print(last)
-- Prints: 5
```

### String assignment
```mlp
string name = "Alice"
name = "Bob"
print(name)
-- Prints: Bob
```

### Assign from function return
```mlp
numeric function square(numeric n)
    return n * n
end function

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
