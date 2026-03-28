# Interfaces (Trait-like) - MeLP Syntax

**Status:** ⚠️ PARTIAL (Stage1 — parsed, dispatch incomplete)
**Syntax Version:** mlp1 / Stage1

---

## 📖 BASIC SYNTAX

```mlp
interface InterfaceName
    numeric function method1(params)
    string function method2(params)
end interface
```

---

## ✅ COMPLETE EXAMPLES

### Example 1 — Basic Interface
```mlp
interface Drawable
    numeric function draw()
    numeric function get_area()
end interface
```

### Example 2 — Struct Implementing Interface
```mlp
struct Circle implements Drawable
    numeric radius
end struct

-- Implementation (methods outside struct)
numeric function Circle.draw()
    print("Drawing circle")
    return 0
end function

numeric function Circle.get_area()
    return 3 * this.radius * this.radius
end function
```

### Example 3 — Multiple Interfaces
```mlp
interface Printable
    string function to_string()
end interface

interface Comparable
    numeric function compare(Comparable other)
end interface

struct Person implements Printable; Comparable
    string name
    numeric age
end struct
```

### Example 4 — Polymorphic Function
```mlp
interface Shape
    numeric function area()
end interface

struct Rectangle implements Shape
    numeric width
    numeric height
end struct

numeric function Rectangle.area()
    return this.width * this.height
end function

function print_area(Shape s)
    print(s.area())
end function

function main()
    Rectangle r
    r.width = 10
    r.height = 5
    print_area(r)
end function
-- Output: 50
```

---

## ⚠️ CRITICAL RULES

1. Interface declares method signatures only — no implementation.
2. Return type comes **before** `function` keyword: `numeric function draw()`
3. Struct implementation: `struct Name implements Interface1; Interface2`
4. Methods implemented outside struct: `numeric function Circle.draw()`
5. `this` refers to the struct instance inside methods.
6. Block terminator: `end interface`

---

## 🚫 WRONG USAGE

```mlp
-- ❌ WRONG: Java style
interface Drawable {
    int draw();
}
```

```mlp
-- ❌ WRONG: -> return type syntax
function Circle.draw() -> numeric
    return 0
end function
```

```mlp
-- ✅ CORRECT: return type before function keyword
numeric function Circle.draw()
    return 0
end function
```

---

## 🔗 RELATED CARDS

- [STRUCT.md](STRUCT.md) — struct definitions
- [METHODS.md](METHODS.md) — external method definitions
- [GENERICS.md](GENERICS.md) — generic interfaces
