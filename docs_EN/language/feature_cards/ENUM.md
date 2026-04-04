# Enum - MeLP Syntax

**Status:** ✅ COMPLETE (Stage1)
**Syntax Version:** mlp1 / Stage1

---

## 📖 BASIC SYNTAX

Enum defines a group of named constants. Values start at 0 and auto-increment. Access with `EnumName.VALUE` notation.

```mlp
enum Color
    RED
    GREEN
    BLUE
end enum

-- RED=0, GREEN=1, BLUE=2
```

---

## ✅ COMPLETE EXAMPLES

### Example 1 — Basic Enum
```mlp
enum Color
    RED
    GREEN
    BLUE
end enum

numeric c = Color.RED

if c == Color.GREEN then
    print("green")
else
    print("not green")
end if
-- Output: not green
```

### Example 2 — Enum with Match
```mlp
enum Status
    PENDING
    ACTIVE
    COMPLETED
end enum

string function describe(numeric s)
    match s
        case Status.PENDING then return "Pending"
        case Status.ACTIVE then return "Active"
        case Status.COMPLETED then return "Completed"
        else return "Unknown"
    end match
end function

function main()
    print(describe(Status.ACTIVE))
end function
-- Output: Active
```

### Example 3 — Enum in Loop
```mlp
enum Day
    MON
    TUE
    WED
    THU
    FRI
end enum

function main()
    numeric d = Day.MON
    loop d <= Day.FRI
        print(d)
        d = d + 1
    end loop
end function
-- Output: 0 1 2 3 4
```

---

## ⚠️ CRITICAL RULES

1. Enum values start at 0 and auto-increment by 1.
2. Access with `EnumName.VALUE` notation.
3. Block terminator: `end enum`
4. Enum values are numeric — use with `match` or `if`.

---

## 🚫 WRONG USAGE

```mlp
-- ❌ WRONG: accessing without enum prefix
numeric c = RED
```

```mlp
-- ✅ CORRECT: always use enum prefix
numeric c = Color.RED
```

---

## 🔗 RELATED CARDS

- [PATTERN_MATCHING.md](PATTERN_MATCHING.md) — using enums with match
- [SWITCH_CASE.md](SWITCH_CASE.md) — switch/match control flow
