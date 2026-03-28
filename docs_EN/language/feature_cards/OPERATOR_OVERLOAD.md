# 📘 MLP Feature Card: Operator Overloading

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

```mlp
struct Vector
    numeric x
    numeric y
end struct

Vector function Vector.operator+(Vector other)
    Vector result
    result.x = this.x + other.x
    result.y = this.y + other.y
    return result
end function

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

```mlp
struct Point
    numeric x
    numeric y
end struct

boolean function Point.operator==(Point other)
    return this.x == other.x and this.y == other.y
end function

boolean function Point.operator<(Point other)
    if this.x != other.x then
        return this.x < other.x
    else
        return this.y < other.y
    end if
end function
```

### Arithmetic Operators

```mlp
struct Complex
    numeric real
    numeric imag
end struct

Complex function Complex.operator+(Complex other)
    Complex result
    result.real = this.real + other.real
    result.imag = this.imag + other.imag
    return result
end function

Complex function Complex.operator*(Complex other)
    Complex result
    result.real = this.real * other.real - this.imag * other.imag
    result.imag = this.real * other.imag + this.imag * other.real
    return result
end function
```

---

## 🔍 Detailed Specification

### 1. Operator Method Syntax

**Declaration:**
```mlp
return_type function StructName.operator@(param_type param)
    -- implementation
end function
```

**Where `@` is one of:**
- Arithmetic: `+`, `-`, `*`, `/`, `mod`
- Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`
- Unary: `-`, `not`

### 2. Supported Operators

**Binary Operators:**
```mlp
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
```mlp
operator-()   -- Negation
operator!()   -- Logical NOT
```

### 3. This Reference

**Access current instance:**
```mlp
Vector function Vector.operator+(Vector other)
    Vector result
    result.x = this.x + other.x  -- 'this' is current Vector
    result.y = this.y + other.y
    return result
end function
```

### 4. Chaining Operators

```mlp
Vector v1
Vector v2
Vector v3
Vector result = v1 + v2 + v3  -- Left-to-right: (v1 + v2) + v3
```

---

## 🧪 Test Cases

See: `GOLDEN_TEST_SUITE/operator_overload_tests/`

### Test 1: Arithmetic
```mlp
struct Vector
    numeric x
    numeric y
end struct

Vector function Vector.operator+(Vector other)
    Vector result
    result.x = this.x + other.x
    result.y = this.y + other.y
    return result
end function

function main()
    Vector v1
    v1.x = 10
    v1.y = 20
    
    Vector v2
    v2.x = 5
    v2.y = 15
    
    Vector v3 = v1 + v2
    print(v3.x)  -- 15
    print(v3.y)  -- 35
    
end function
```

### Test 2: Comparison
```mlp
struct Point
    numeric x
    numeric y
end struct

boolean function Point.operator==(Point other)
    return this.x == other.x and this.y == other.y
end function

function main()
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
    end if
    
end function
```

---

## ⚙️ Implementation Notes

**Name Mangling:**
```
Vector.operator+  →  _ZN6Vector8operatorPlE (C++ style)
```

**Compilation:**
```mlp
v1 + v2  →  Vector.operator+(v1, v2)
```

**LLVM IR:**
```llvm
%result = call %Vector @Vector.operator+(%Vector %v1, %Vector %v2)
```

---

## 🚨 MELP Syntax Rules

**✅ Correct:**
```mlp
Vector function Vector.operator+(Vector other)
    -- implementation
end function
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
- `end function` terminator

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
