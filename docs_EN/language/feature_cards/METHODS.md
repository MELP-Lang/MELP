# 📘 MLP Feature Card: Methods (Struct Methods)

**Durum:** ⚠️ PARTIAL (Stage 1 — temel methods var, virtual eksik)  
**Sözdizimi Sürümü:** mlp1 / Stage1  

**Stage:** Stage 1 Phase 1.5  
**Complexity:** ⭐⭐ Medium  
**Estimated Time:** 2-3 days

---

## 🎯 Feature Summary

**What:** Methods associated with structs.

**Why:** Object-oriented programming, better code organization.

**How:** `StructName.method_name()` syntax.

---

## 📖 Syntax Reference

### Basic Method

```mlp
struct Counter
    numeric value
end struct

numeric function Counter.increment()
    this.value = this.value + 1
    return this.value
end function

numeric function Counter.get_value()
    return this.value
end function

-- Usage
Counter c
c.value = 0
c.increment()
c.increment()
print(c.get_value())  -- prints 2
```

**MELP Format:** `StructName.method_name()` for definition.

### Method with Parameters

```mlp
struct Rectangle
    numeric width
    numeric height
end struct

numeric function Rectangle.set_size(numeric w; numeric h)
    this.width = w
    this.height = h
    return 0
end function

numeric function Rectangle.area()
    return this.width * this.height
end function

-- Usage
Rectangle rect
rect.set_size(10; 20)
print(rect.area())  -- prints 200
```

### Method Chaining

```mlp
struct Builder
    string result
end struct

Builder function Builder.append(string text)
    this.result = this.result + text
    return this
end function

-- Usage
Builder b
b.result = ""
b.append("Hello").append(" ").append("World")
print(b.result)  -- prints "Hello World"
```

---

## 🔍 Detailed Specification

### 1. Method Definition

**Syntax:**
```mlp
return_type function StructName.method_name(params)
    -- implementation using 'this'
end function
```

**Rules:**
- Method name after struct name and dot
- `this` refers to current instance
- Can access and modify fields via `this.field`

### 2. This Reference

**Implicit Parameter:**
```mlp
numeric function Counter.increment()
    this.value = this.value + 1  -- 'this' is Counter instance
    return this.value
end function
```

**This Type:** Same as struct type.

### 3. Method Calling

**Syntax:**
```mlp
instance.method_name(arguments)
```

**Example:**
```mlp
Counter c
c.value = 10
numeric result = c.increment()  -- Calls Counter.increment(c)
```

**Desugaring:**
```mlp
c.increment()  →  Counter.increment(c)
```

### 4. Static Methods (Future)

**Stage 1:** Not supported
```mlp
-- Future (Stage 2+):
Counter static function Counter.create()
    Counter c
    c.value = 0
    return c
end function

Counter c = Counter.create()  -- No instance needed
```

---

## 🧪 Test Cases

See: `GOLDEN_TEST_SUITE/methods_tests/`

### Test 1: Basic Methods
```mlp
struct Point
    numeric x
    numeric y
end struct

numeric function Point.move(numeric dx; numeric dy)
    this.x = this.x + dx
    this.y = this.y + dy
    return 0
end function

numeric function Point.distance_from_origin()
    return (this.x * this.x + this.y * this.y) ** 0.5
end function

function main()
    Point p
    p.x = 3
    p.y = 4
    
    print(p.distance_from_origin())  -- 5.0
    
    p.move(1; 1)
    print(p.x)  -- 4
    print(p.y)  -- 5
    
end function
```

### Test 2: Method Chaining
```mlp
struct StringBuilder
    string content
end struct

StringBuilder function StringBuilder.append(string text)
    this.content = this.content + text
    return this
end function

string function StringBuilder.to_string()
    return this.content
end function

function main()
    StringBuilder sb
    sb.content = ""
    
    string result = sb.append("Hello")
                      .append(" ")
                      .append("World")
                      .to_string()
    
    print(result)  -- "Hello World"
end function
```

---

## ⚙️ Implementation Notes

**Method Compilation:**

1. **Desugar to function:**
   ```mlp
   c.increment()  →  Counter.increment(c)
   ```

2. **Name mangling:**
   ```
   Counter.increment  →  _ZN7Counter9incrementEv
   ```

3. **LLVM IR:**
   ```llvm
   %result = call i32 @_ZN7Counter9incrementEv(%Counter* %c)
   ```

**This Reference:**
- First parameter to method function
- Passed by pointer
- Allows modification of fields

---

## 🚨 MELP Syntax Rules

**✅ Correct:**
```mlp
struct Counter
    numeric value
end struct

numeric function Counter.increment()
    this.value = this.value + 1
    return this.value
end function
```

**❌ Wrong (C++ style):**
```cpp
class Counter {
    int value;
public:
    int increment() {
        return ++value;
    }
};
```

**❌ Wrong (Python style):**
```python
class Counter:
    def increment(self):
        self.value += 1
        return self.value
```

**MELP way:**
- Separate struct and method definitions
- `StructName.method_name()` syntax
- `this` keyword (not `self`)
- `function` keyword required
- `end function` terminator

---

## 📋 Limitations (Stage 1)

**Supported:**
- ✅ Instance methods
- ✅ Method chaining
- ✅ `this` reference

**Not Supported Yet:**
- ❌ Static methods
- ❌ Constructor methods
- ❌ Destructor methods
- ❌ Method overloading

---

## 📋 Success Criteria

- ✅ Method definitions parse
- ✅ Method calls work
- ✅ `this` reference works
- ✅ Method chaining works
- ✅ All golden tests pass

---

**Reference:** GOLDEN_TEST_SUITE/methods_tests/
