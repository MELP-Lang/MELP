# Function References (FUNCREF) — MELP Syntax

**Durum:** 🔴 TODO (Phase 3 — funcref token lexer'da yok)  
**Sözdizimi Sürümü:** pmlp0 / Stage0  

**Stage:** pmlp0 (Stage 0)  
**Module:** First-class functions and function references

---

## 📖 BASIC SYNTAX

```melp
-- Declare a function reference variable
funcref<(ParamType) -> ReturnType> name = existing_function

-- Call through a reference
numeric result = name(arg)
```

---

## ✅ COMPLETE EXAMPLES

### Basic function reference
```melp
function double(x: numeric) -> numeric
    return x * 2
end_function

funcref<(numeric) -> numeric> fn = double
numeric result = fn(5)
print(result)
-- Prints: 10
```

### Pass as argument (higher-order function)
```melp
function apply(f: funcref<(numeric) -> numeric>, v: numeric) -> numeric
    return f(v)
end_function

function triple(n: numeric) -> numeric
    return n * 3
end_function

numeric r = apply(triple, 4)
print(r)
-- Prints: 12
```

### Store in variable and reassign
```melp
function add1(x: numeric) -> numeric
    return x + 1
end_function

function add2(x: numeric) -> numeric
    return x + 2
end_function

funcref<(numeric) -> numeric> op = add1
print(op(10))
-- Prints: 11
op = add2
print(op(10))
-- Prints: 12
```

---

## ⚠️ CRITICAL RULES

- `funcref<(P1, P2, ...) -> R>` is the type syntax for function references.
- The signature must exactly match the referred function.
- Function references are not closures — they do not capture surrounding variables.
- For closures (captured variables), see [CLOSURE.md](CLOSURE.md).
- `void` return is written as `-> void`.

---

## 🔗 RELATED CARDS

- [FUNCTION_DEF.md](FUNCTION_DEF.md) — defining functions
- [CLOSURE.md](CLOSURE.md) — closures with capture
- [GENERICS.md](GENERICS.md) — generic function signatures
