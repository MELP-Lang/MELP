# 📘 PMLP Feature Card: Operator Overloading

**Stage:** Stage 1 Phase 1.5  
**Complexity:** ⭐⭐⭐ High  
**Estimated Time:** 3-4 days

---

## 🎯 Feature Summary

**What:** Define custom behavior for operators on user types.

**Why:** Natural syntax for custom types (e.g., `vector1 + vector2`).

**How:** `operator+`, `operator==` etc. as special methods.

---

## 📖 Syntax Reference

### Basic Operator Overload

```pmlp
struct Vector
    numeric x
    numeric y
end_struct

function Vector.operator+(Vector other) as Vector
    Vector result
    result.x = this.x + other.x
    result.y = this.y + other.y
    return result
end_function

-- Usage
Vector v1
v1.x = 10
v1.y = 20

Vector v2
v2.x = 5
v2.y = 15

Vector v3 = v1 + v2  -- Calls Vector.operator+
```

**MELP Format:** `StructName.operator+` for method definition.

### Comparison Operators

```pmlp
struct Point
    numeric x
    numeric y
end_struct

function Point.operator==(Point other) as boolean
    return this.x == other.x and this.y == other.y
end_function

function Point.operator<(Point other) as boolean
    if this.x != other.x then
        return this.x < other.x
    else
        return this.y < other.y
    end_if
end_function
```

### Arithmetic Operators

```pmlp
struct Complex
    numeric real
    numeric imag
end_struct

function Complex.operator+(Complex other) as Complex
    Complex result
    result.real = this.real + other.real
    result.imag = this.imag + other.imag
    return result
end_function

function Complex.operator*(Complex other) as Complex
    Complex result
    result.real = this.real * other.real - this.imag * other.imag
    result.imag = this.real * other.imag + this.imag * other.real
    return result
end_function
```

---

## 🔍 Detailed Specification

### 1. Operator Method Syntax

**Declaration:**
```pmlp
function StructName.operator@(param_type param) as return_type
    -- implementation
end_function
```

**Where `@` is one of:**
- Arithmetic: `+`, `-`, `*`, `/`, `mod`
- Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`
- Unary: `-`, `not`

### 2. Supported Operators

**Binary Operators:**
```pmlp
operator+(other)   -- Addition
operator-(other)   -- Subtraction
operator*(other)   -- Multiplication
operator/(other)   -- Division
operator==(other)  -- Equality
operator!=(other)  -- Inequality
operator<(other)   -- Less than
operator>(other)   -- Greater than
```

**Unary Operators:**
```pmlp
operator-()   -- Negation
operator!()   -- Logical NOT
```

### 3. This Reference

**Access current instance:**
```pmlp
function Vector.operator+(Vector other) as Vector
    Vector result
    result.x = this.x + other.x  -- 'this' is current Vector
    result.y = this.y + other.y
    return result
end_function
```

### 4. Chaining Operators

```pmlp
Vector v1
Vector v2
Vector v3
Vector result = v1 + v2 + v3  -- Left-to-right: (v1 + v2) + v3
```

---

## 🧪 Test Cases

See: `GOLDEN_TEST_SUITE/operator_overload_tests/`

### Test 1: Arithmetic
```pmlp
struct Vector
    numeric x
    numeric y
end_struct

function Vector.operator+(Vector other) as Vector
    Vector result
    result.x = this.x + other.x
    result.y = this.y + other.y
    return result
end_function

function main() as numeric
    Vector v1
    v1.x = 10
    v1.y = 20
    
    Vector v2
    v2.x = 5
    v2.y = 15
    
    Vector v3 = v1 + v2
    print(v3.x)  -- 15
    print(v3.y)  -- 35
    
    return 0
end_function
```

### Test 2: Comparison
```pmlp
struct Point
    numeric x
    numeric y
end_struct

function Point.operator==(Point other) as boolean
    return this.x == other.x and this.y == other.y
end_function

function main() as numeric
    Point p1
    p1.x = 5
    p1.y = 10
    
    Point p2
    p2.x = 5
    p2.y = 10
    
    if p1 == p2 then
        print("Equal")
    else
        print("Not equal")
    end_if
    
    return 0
end_function
```

---

## ⚙️ Implementation Notes

**Name Mangling:**
```
Vector.operator+  →  _ZN6Vector8operatorPlE (C++ style)
```

**Compilation:**
```pmlp
v1 + v2  →  Vector.operator+(v1, v2)
```

**LLVM IR:**
```llvm
%result = call %Vector @Vector.operator+(%Vector %v1, %Vector %v2)
```

---

## 🚨 MELP Syntax Rules

**✅ Correct:**
```pmlp
function Vector.operator+(Vector other) as Vector
    -- implementation
end_function
```

**❌ Wrong (C++ style):**
```cpp
Vector operator+(const Vector& other) const {
    // implementation
}
```

**❌ Wrong (Python style):**
```python
def __add__(self, other):
    # implementation
```

**MELP way:**
- `StructName.operator+` format
- `function` keyword
- `this` for self reference
- Parameters: `Type name` format
- `as Type` for return type
- `end_function` terminator

---

## 📋 Limitations (Stage 1)

**Supported:**
- ✅ Binary arithmetic operators
- ✅ Comparison operators
- ✅ Unary operators

**Not Supported Yet:**
- ❌ `[]` subscript operator
- ❌ `()` function call operator
- ❌ Assignment operators (`+=`, `-=`)
- ❌ Conversion operators

---

## 📋 Success Criteria

- ✅ Arithmetic operators work
- ✅ Comparison operators work
- ✅ Operator chaining works
- ✅ `this` reference works
- ✅ All golden tests pass

---

**Reference:** GOLDEN_TEST_SUITE/operator_overload_tests/
