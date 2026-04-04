# Closure - MeLP Syntax

**Status:** ✅ PARTIAL (inline closure works; mutable capture not yet implemented)
**Syntax Version:** mlp1 / Stage1

---

## 📖 BASIC SYNTAX

A closure is a lambda that captures variables from its enclosing scope.

```mlp
-- Lambda captures outer variable
numeric multiplier = 3
numeric mult = lambda(x) -> x * multiplier
```

---

## ✅ COMPLETE EXAMPLES

### Example 1 — Basic Closure (Single Capture)
```mlp
function main()
    numeric base = 5
    numeric add_base = lambda(x) -> x + base
    print(add_base(10))
    print(add_base(3))
end function
-- Output: 15
-- Output: 8
```

### Example 2 — Closure from Function Return
```mlp
numeric function makeAdder(numeric x)
    return lambda(y) -> x + y
end function

function main()
    numeric add5 = makeAdder(5)
    print(add5(10))
end function
-- Output: 15
```

### Example 3 — Multiple Closures
```mlp
function main()
    numeric base = 10
    numeric add_base = lambda(x) -> x + base
    numeric mul_base = lambda(x) -> x * base

    print(add_base(5))
    print(mul_base(3))
end function
-- Output: 15
-- Output: 30
```

---

## ⚠️ CRITICAL RULES

1. Closures capture outer variables by value (immutable capture).
2. Mutable capture (`count = count + 1` pattern) is not yet implemented.
3. Block lambda (`end lambda`) is not yet implemented — use inline `-> expr` form.
4. Parameter separator: `;` (semicolon)

---

## 🚫 WRONG USAGE

```mlp
-- ❌ WRONG: block lambda (not implemented)
return lambda(y)
    return x + y
end lambda
```

```mlp
-- ✅ CORRECT: inline lambda
return lambda(y) -> x + y
```

---

## 🔗 RELATED CARDS

- [LAMBDA.md](LAMBDA.md) — lambda expressions
- [HIGHER_ORDER_FUNCTIONS.md](HIGHER_ORDER_FUNCTIONS.md) — passing functions
- [FUNCREF.md](FUNCREF.md) — function references
