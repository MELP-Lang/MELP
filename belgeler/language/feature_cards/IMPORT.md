# 📘 PMLP Feature Card: Import System

**Durum:** ✅ COMPLETE (Stage 1)  
**Sözdizimi Sürümü:** pmlp1 / Stage1  

**Stage:** Stage 0 (Unblock for Stage 1)  
**Complexity:** ⭐⭐⭐ High  
**Estimated Time:** 2-3 days

---

## 🎯 Feature Summary

**What:** Multi-file compilation with module imports.

**Why:** Enable splitting code into multiple `.mlp` files.

**How:** `import` statement to bring in other modules.

---

## 📖 Syntax Reference

### Basic Import

**File: math_utils.mlp**
```pmlp
function add(numeric a; numeric b) as numeric
    return a + b
end_function

function multiply(numeric a; numeric b) as numeric
    return a * b
end_function
```

**File: main.mlp**
```pmlp
import math_utils

function main() as numeric
    numeric result = math_utils.add(5; 3)
    print(result)  -- prints 8
    
    result = math_utils.multiply(4; 7)
    print(result)  -- prints 28
    
    return 0
end_function
```

**Syntax:** `import module_name` (without `.mlp` extension)

### Import with Structs

**File: geometry.mlp**
```pmlp
struct Point
    numeric x
    numeric y
end_struct

function Point.distance_from_origin() as numeric
    return this.x * this.x + this.y * this.y
end_function
```

**File: app.mlp**
```pmlp
import geometry

function main() as numeric
    geometry.Point p
    p.x = 3
    p.y = 4
    
    numeric dist = p.distance_from_origin()
    print(dist)  -- prints 25
    
    return 0
end_function
```

### Multiple Imports

```pmlp
import string_utils
import math_utils
import data_structures

function main() as numeric
    string text = string_utils.concat("Hello"; " World")
    numeric sum = math_utils.add(10; 20)
    
    data_structures.List list
    -- use list...
    
    return 0
end_function
```

---

## 🧪 Test Scenarios

### Test 1: Basic Import
**File: utils.mlp**
```pmlp
function greet(string name) as string
    return "Hello, " + name
end_function
```

**File: test_01_basic_import.mlp**
```pmlp
import utils

function main() as numeric
    string greeting = utils.greet("World")
    print(greeting)
    return 0
end_function
```

### Test 2: Struct Import
**File: shapes.mlp**
```pmlp
struct Circle
    numeric radius
end_struct

function Circle.area() as numeric
    return 3.14 * this.radius * this.radius
end_function
```

**File: test_02_struct_import.mlp**
```pmlp
import shapes

function main() as numeric
    shapes.Circle c
    c.radius = 5
    print(c.area())
    return 0
end_function
```

### Test 3: Multiple Imports
**File: math.mlp**
```pmlp
function add(numeric a; numeric b) as numeric
    return a + b
end_function
```

**File: string.mlp**
```pmlp
function concat(string a; string b) as string
    return a + b
end_function
```

**File: test_03_multiple_imports.mlp**
```pmlp
import math
import string

function main() as numeric
    numeric sum = math.add(5; 10)
    string text = string.concat("Sum: "; "15")
    print(text)
    return 0
end_function
```

### Test 4: Nested Structs Import
**File: data.mlp**
```pmlp
struct Container
    numeric value
end_struct

struct Wrapper
    Container inner
end_struct
```

**File: test_04_nested_import.mlp**
```pmlp
import data

function main() as numeric
    data.Wrapper w
    w.inner.value = 42
    print(w.inner.value)
    return 0
end_function
```

### Test 5: Cross-Module Function Calls
**File: calculator.mlp**
```pmlp
function compute(numeric x) as numeric
    return x * 2 + 10
end_function
```

**File: test_05_cross_module.mlp**
```pmlp
import calculator

function wrapper(numeric input) as numeric
    return calculator.compute(input)
end_function

function main() as numeric
    numeric result = wrapper(15)
    print(result)  -- prints 40
    return 0
end_function
```

---

## ⚠️ Important Notes

### Syntax Rules
- **Import statement:** `import module_name` (one per line)
- **Module access:** `module_name.function()` or `module_name.StructName`
- **File names:** Must match module name (e.g., `math_utils.mlp` → `import math_utils`)
- **No circular imports:** Module A cannot import Module B if B imports A

### Compiler Behavior
- Imports are resolved at compile time
- Each module compiled separately, then linked
- Module symbols prefixed with namespace

### File Organization
```
project/
├── main.mlp          # Entry point
├── utils.mlp         # Utility module
├── data.mlp          # Data structures module
└── helpers.mlp       # Helper functions module
```

**Compilation:** 
```bash
# Compiler must process all .mlp files in dependency order
./compiler main.mlp utils.mlp data.mlp helpers.mlp -o output
```

---

## 🔗 Related Features

- **struct** - Can be imported and used across modules
- **function_def** - Functions can be imported
- **generics** - Generic types work with imports (Stage 1)

---

## 📚 Implementation Reference

**Module System:** `compiler/paradigma2/MODERN/modules/import/`  
**Parser:** `import_parser.cpp` (to be created)  
**Codegen:** `import_codegen.cpp` (to be created)  
**Linking:** Multi-file LLVM IR linking

**Archaeological Evidence:** 
- `/home/pardus/projeler/MLP/Arşiv/MLP_selfhosting_yedek/compiler/` (import module exists)
- Multi-file support proven in Stage 0 backup

---

## 🎓 Design Philosophy

### Namespace Isolation
- Each module has its own namespace
- No global pollution
- Explicit import required

### Compilation Model
- **Separate compilation:** Each `.mlp` file → `.ll` LLVM IR
- **Linking phase:** LLVM linker combines modules
- **Symbol resolution:** Qualified names (`module.function`)

### Dependency Resolution
- Import statements declare dependencies
- Compiler builds dependency graph
- Topological sort for compilation order

---

**Last Updated:** 2 Şubat 2026
