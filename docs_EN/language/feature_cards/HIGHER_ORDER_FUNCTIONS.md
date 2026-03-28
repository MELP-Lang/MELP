# Higher-Order Functions - MeLP Syntax

**Status:** ✅ COMPLETE (Stage1 — array map/filter/reduce + named fnptr)
**Syntax Version:** mlp1 / Stage1

---

## 📖 BASIC SYNTAX

```mlp
array.map(lambda(x) -> expression)
array.filter(lambda(x) -> condition)
array.reduce(lambda(acc; x) -> expression; initial)
```

---

## ✅ COMPLETE EXAMPLES

### Example 1 — Map
```mlp
function main()
    numeric[] numbers = [1; 2; 3; 4; 5]
    numeric[] doubled = numbers.map(lambda(x) -> x * 2)
    print(doubled[0])
    print(doubled[4])
end function
-- Output: 2
-- Output: 10
```

### Example 2 — Filter
```mlp
function main()
    numeric[] numbers = [1; 2; 3; 4; 5; 6]
    numeric[] evens = numbers.filter(lambda(x) -> x % 2 == 0)
    print(evens[0])
    print(evens.length)
end function
-- Output: 2
-- Output: 3
```

### Example 3 — Reduce
```mlp
function main()
    numeric[] numbers = [1; 2; 3; 4; 5]
    numeric sum = numbers.reduce(lambda(acc; x) -> acc + x; 0)
    print(sum)
end function
-- Output: 15
```

### Example 4 — Chaining
```mlp
function main()
    numeric[] numbers = [1; 2; 3; 4; 5]
    numeric result = numbers.filter(lambda(x) -> x % 2 == 0).map(lambda(x) -> x * x).reduce(lambda(acc; x) -> acc + x; 0)
    print(result)
end function
-- Output: 20
```

---

## ⚠️ CRITICAL RULES

1. Lambda parameter separator: `;` (semicolon)
2. `reduce` initial value separated by `;` from the lambda.
3. Array element separator: `;` (semicolon)

---

## 🚫 WRONG USAGE

```mlp
-- ❌ WRONG: comma separator in lambda parameters
numbers.reduce(lambda(acc, x) -> acc + x, 0)
```

```mlp
-- ✅ CORRECT: semicolon separator
numbers.reduce(lambda(acc; x) -> acc + x; 0)
```

---

## 🔗 RELATED CARDS

- [LAMBDA.md](LAMBDA.md) — lambda expressions
- [CLOSURE.md](CLOSURE.md) — closures
- [ARRAY.md](ARRAY.md) — arrays
- [FUNCREF.md](FUNCREF.md) — function references
