# Function References (FUNCREF) — MELP Syntax

**Durum:** 🔴 TODO (Phase 3 — funcref token lexer'da yok)  
**Sözdizimi Sürümü:** mlp1 / Stage1  

**Stage:** mlp1 (Stage1)  
**Module:** First-class functions and function references

---

## 📖 BASIC SYNTAX

```mlp
-- Declare a function reference variable
funcref<(ParamType) -> ReturnType> name = existing_function

-- Call through a reference
numeric result = name(arg)
```

---

## ✅ COMPLETE EXAMPLES

### Basic function reference
```mlp
numeric function double(numeric x)
    return x * 2
end function

function main()
    funcref<(numeric) -> numeric> fn = double
    numeric result = fn(5)
    print(result)
    -- Output: 10
end function
```

### Pass as argument (higher-order function)
```mlp
numeric function apply(funcref<(numeric) -> numeric> f; numeric v)
    return f(v)
end function

numeric function triple(numeric n)
    return n * 3
end function

numeric r = apply(triple; 4)
print(r)
-- Prints: 12
```

### Store in variable and reassign
```mlp
numeric function add1(numeric x)
    return x + 1
end function

numeric function add2(numeric x)
    return x + 2
end function

funcref<(numeric) -> numeric> op = add1
print(op(10))
-- Prints: 11
op = add2
print(op(10))
-- Prints: 12
```

---

## ⚠️ CRITICAL RULES

- `funcref<(P1; P2; ...) -> R>` is the type syntax for function references.
- The signature must exactly match the referred function.
- Function references are not closures — they do not capture surrounding variables.
- For closures (captured variables), see [CLOSURE.md](CLOSURE.md).
- Void return functions: use `void function name()` syntax.

---

---

## 🧪 GOLDEN TEST

```mlp
numeric function double(numeric x)
    return x * 2
end function

numeric function inc(numeric x)
    return x + 1
end function

numeric function apply(funcref<(numeric) -> numeric> f; numeric v)
    return f(v)
end function

function main()
    funcref<(numeric) -> numeric> op = double
    numeric r1 = apply(op; 5)
    -- Output: 10
    if r1 == 10 then
        op = inc
        numeric r2 = apply(op; 10)
        -- Output: 11
        if r2 == 11 then return 1 end if
    end if
end function
```

---

**Bağımlılıklar:** FUNCTION_DEF, CLOSURE  
**Kanonik Kaynak:** `mlp1_sozdizimi.md` — Fonksiyon tipleri

## 🔗 RELATED CARDS

- [FUNCTION_DEF.md](FUNCTION_DEF.md) — defining functions
- [CLOSURE.md](CLOSURE.md) — closures with capture
- [GENERICS.md](GENERICS.md) — generic function signatures
