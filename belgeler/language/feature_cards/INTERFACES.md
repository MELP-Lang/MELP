# 📘 PMLP Feature Card: Interfaces (Trait-like)

**Durum:** ⚠️ PARTIAL (Stage 1 — parse edilir, dispatch eksik)  
**Sözdizimi Sürümü:** pmlp1 / Stage1  

**Stage:** Stage 1 Phase 1.5  
**Complexity:** ⭐⭐⭐ High  
**Estimated Time:** 4-5 days

---

## 🎯 Feature Summary

**What:** Interface definitions for polymorphism and abstraction.

**Why:** Define contracts that types must implement.

**How:** `interface` declaration + `implements` keyword for structs.

---

## 📖 Syntax Reference

### Basic Interface

```pmlp
interface Drawable
    function draw() as numeric
    function get_area() as numeric
end_interface
```

### Struct Implementing Interface

```pmlp
struct Circle implements Drawable
    numeric radius
end_struct

-- Implementation
function Circle.draw() as numeric
    print("Drawing circle")
    return 0
end_function

function Circle.get_area() as numeric
    return 3.14 * this.radius * this.radius
end_function
```

**MELP Format:** `StructName.method_name()` for implementation.

### Multiple Interfaces

```pmlp
interface Printable
    function to_string() as string
end_interface

interface Comparable
    function compare(Comparable other) as numeric
end_interface

struct Person implements Printable; Comparable
    string name
    numeric age
end_struct
```

**Note:** Semicolon `;` separates multiple interfaces.

### Interface as Parameter

```pmlp
function render(Drawable obj) as numeric
    return obj.draw()
end_function

-- Usage
Circle c
c.radius = 5
render(c)  -- Polymorphism!
```

---

## 🔍 Detailed Specification

### 1. Interface Declaration

**Syntax:**
```pmlp
interface InterfaceName
    function method1(params) as return_type
    function method2(params) as return_type
end_interface
```

**Rules:**
- Interface name: PascalCase
- Only method signatures (no implementation)
- Methods use `function` keyword

### 2. Interface Implementation

**Syntax:**
```pmlp
struct StructName implements Interface1; Interface2
    -- fields
end_struct

-- Method implementations
function StructName.method1(params) as return_type
    -- implementation
end_function
```

### 3. Polymorphic Functions

**Example:**
```pmlp
interface Shape
    function area() as numeric
end_interface

struct Rectangle implements Shape
    numeric width
    numeric height
end_struct

function Rectangle.area() as numeric
    return this.width * this.height
end_function

function print_area(Shape s) as numeric
    print("Area: ")
    print(s.area())
    return 0
end_function
```

---

## 🧪 Test Cases

See: `GOLDEN_TEST_SUITE/interfaces_tests/`

### Test 1: Basic Interface
```pmlp
interface Printable
    function print_self() as numeric
end_interface

struct Person implements Printable
    string name
end_struct

function Person.print_self() as numeric
    print(this.name)
    return 0
end_function

function main() as numeric
    Person p
    p.name = "Alice"
    p.print_self()
    return 0
end_function
```

### Test 2: Polymorphism
```pmlp
interface Drawable
    function draw() as string
end_interface

struct Circle implements Drawable
    numeric radius
end_struct

function Circle.draw() as string
    return "Circle"
end_function

function render(Drawable d) as numeric
    print(d.draw())
    return 0
end_function
```

---

## ⚙️ Implementation Notes

**Static Dispatch:** Preferred for Stage 1
- Compile-time method resolution
- No vtable overhead

**Dynamic Dispatch:** Stage 1.5+
- Runtime polymorphism
- Requires vtable/interface pointers

**Method Resolution:**
```pmlp
Circle c
c.draw()  -- Static: Circle.draw()

Drawable d = c
d.draw()  -- Dynamic: lookup Circle.draw() via interface
```

---

## 🚨 MELP Syntax Rules

**✅ Correct:**
```pmlp
interface Drawable
    function draw() as numeric
end_interface

struct Circle implements Drawable
    numeric radius
end_struct

function Circle.draw() as numeric
    print("Drawing")
    return 0
end_function
```

**❌ Wrong (Java style):**
```pmlp
interface Drawable {
    int draw();
}

class Circle implements Drawable {
    public int draw() { }
}
```

**❌ Wrong (Rust style):**
```pmlp
trait Drawable {
    fn draw(&self) -> i32;
}

impl Drawable for Circle {
    fn draw(&self) -> i32 { }
}
```

**MELP way:**
- `interface` keyword (not `trait`)
- `implements` in struct declaration
- `StructName.method()` for implementation
- `function` keyword for methods
- `this` for self reference

---

## 📋 Success Criteria

- ✅ Interface declarations work
- ✅ Struct implements interface
- ✅ Multiple interfaces supported
- ✅ Polymorphic functions work
- ✅ All golden tests pass

---

**Reference:** GOLDEN_TEST_SUITE/interfaces_tests/
