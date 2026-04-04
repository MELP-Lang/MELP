# Struct - MeLP Syntax

**Status:** ✅ COMPLETE (Stage1)
**Syntax Version:** mlp1 / Stage1

---

## 📖 BASIC SYNTAX

Struct is a data container only. Methods are defined **outside** the struct using `function StructName.method()` syntax. This is MeLP's STRUCT+FUNC principle (not OOP).

```mlp
struct Point
    numeric x
    numeric y
end struct
```

---

## ✅ COMPLETE EXAMPLES

### Example 1 — Basic Struct
```mlp
struct Point
    numeric x
    numeric y
end struct

Point p
p.x = 10
p.y = 20
print(p.x)
-- Output: 10
```

### Example 2 — Struct with External Methods
```mlp
struct Rectangle
    numeric width
    numeric height
end struct

-- Methods defined outside the struct
numeric function Rectangle.area()
    return this.width * this.height
end function

numeric function Rectangle.perimeter()
    return 2 * (this.width + this.height)
end function

function main()
    Rectangle r
    r.width = 10
    r.height = 5
    print(r.area())
    print(r.perimeter())
end function
-- Output: 50
-- Output: 30
```

### Example 3 — Nested Struct
```mlp
struct Address
    string street
    numeric zipcode
end struct

struct Person
    string name
    numeric age
    Address addr
end struct

Person p
p.name = "Ali"
p.age = 30
p.addr.street = "Main St"
print(p.addr.street)
-- Output: Main St
```

---

## ⚠️ CRITICAL RULES

1. Struct is a **data container only** — no methods inside struct body.
2. External methods: `numeric function Rectangle.area()`  — return type before `function`.
3. Block terminator: `end struct`
4. Access fields with `.` notation: `p.x`
5. `this` refers to the struct instance inside methods.

---

## 🚫 WRONG USAGE

```mlp
-- ❌ WRONG: method inside struct
struct Point
    numeric x
    function distance() -> numeric
        return this.x
    end function
end struct
```

```mlp
-- ✅ CORRECT: method outside struct
struct Point
    numeric x
end struct

numeric function Point.distance()
    return this.x
end function
```

---

## 🔗 RELATED CARDS

- [METHODS.md](METHODS.md) — external method definitions
- [STRUCT_CONSTRUCTOR.md](STRUCT_CONSTRUCTOR.md) — struct initialization
