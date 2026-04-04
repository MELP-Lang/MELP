# Lambda - MeLP Syntax

**Status:** ✅ PARTIAL (inline lambda works; block lambda not yet implemented)
**Syntax Version:** mlp1 / Stage1

---

## 📖 BASIC SYNTAX

```mlp
-- Inline lambda: lambda(params) -> expression
numeric add = lambda(x; y) -> x + y
```

---

## ✅ COMPLETE EXAMPLES

### Example 1 — Basic Lambda
```mlp
function main()
    numeric add = lambda(x; y) -> x + y
    numeric result = add(10; 20)
    print(result)
end function
-- Output: 30
```

### Example 2 — Single Parameter
```mlp
function main()
    numeric double = lambda(x) -> x * 2
    print(double(7))
end function
-- Output: 14
```

### Example 3 — Closure (Captures Outer Variable)
```mlp
function main()
    numeric multiplier = 3
    numeric mult = lambda(x) -> x * multiplier
    print(mult(5))
    print(mult(10))
end function
-- Output: 15
-- Output: 30
```

### Example 4 — Lambda in Condition
```mlp
function main()
    numeric double = lambda(x) -> x * 2
    numeric base = 5
    numeric add_base = lambda(x) -> x + base

    if double(5) == 10 then
        print("double ok")
    end if
    if add_base(3) == 8 then
        print("closure ok")
    end if
end function
-- Output: double ok
-- Output: closure ok
```

---

## ⚠️ CRITICAL RULES

1. Lambda syntax: `lambda(params) -> expression`
2. Parameter separator: `;` (semicolon)
3. Lambda result is stored in a `numeric` variable.
4. Closures capture outer variables by value.
5. Block lambda (`end lambda`) is **not yet implemented**.

---

## 🚫 WRONG USAGE

```mlp
-- ❌ WRONG: block lambda (not implemented)
function add = lambda(numeric a; numeric b) -> numeric
    return a + b
end lambda
```

```mlp
-- ✅ CORRECT: inline lambda
numeric add = lambda(a; b) -> a + b
```

---

## 🔗 RELATED CARDS

- [CLOSURE.md](CLOSURE.md) — closure details
- [HIGHER_ORDER_FUNCTIONS.md](HIGHER_ORDER_FUNCTIONS.md) — passing functions
- [FUNCREF.md](FUNCREF.md) — function references
