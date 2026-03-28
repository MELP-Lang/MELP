# Pattern Matching - MeLP Syntax

**Status:** ⚠️ WIP (Stage1)
**Syntax Version:** mlp1 / Stage1

---

## 📖 BASIC SYNTAX

```mlp
match expression
    case value then -- body
    case value then -- body
    else -- default body
end match
```

---

## ✅ COMPLETE EXAMPLES

### Example 1 — Basic Match
```mlp
function main()
    numeric x = 5
    match x
        case 1 then print("one")
        case 2 then print("two")
        case 5 then print("five")
        else print("other")
    end match
end function
-- Output: five
```

### Example 2 — Match as Expression
```mlp
function main()
    numeric x = 2
    string msg = match x
        case 1 then "one"
        case 2 then "two"
        else "other"
    end match
    print(msg)
end function
-- Output: two
```

### Example 3 — Guard Clause
```mlp
function main()
    numeric n = 15
    match n
        case x if x < 10 then print("small")
        case x if x < 20 then print("medium")
        else print("large")
    end match
end function
-- Output: medium
```

### Example 4 — Classify Function
```mlp
string function classify(numeric n)
    return match n
        case 0 then "zero"
        case x if x < 0 then "negative"
        case x if x < 100 then "small"
        else "large"
    end match
end function

function main()
    print(classify(0))
    print(classify(-5))
    print(classify(50))
end function
-- Output: zero
-- Output: negative
-- Output: small
```

---

## ⚠️ CRITICAL RULES

1. `match` requires `end match` terminator.
2. `else` is the default/wildcard branch.
3. Guard clauses: `case x if condition then`
4. Match can be used as an expression to return a value.
5. `function main()` should NOT have `return 0` or `return 1`.

---

## 🚫 WRONG USAGE

```mlp
-- ❌ WRONG: -> syntax for return type
function classify(numeric n) -> string
    return "x"
end function
```

```mlp
-- ✅ CORRECT: return type before function keyword
string function classify(numeric n)
    return "x"
end function
```

---

## 🔗 RELATED CARDS

- [ENUM.md](ENUM.md) — enums for pattern matching
- [SWITCH_CASE.md](SWITCH_CASE.md) — switch/case alternative
- [IF_STATEMENT.md](IF_STATEMENT.md) — conditional control flow
