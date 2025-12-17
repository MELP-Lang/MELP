# Variables Module - MELP Stage 1

**Created by:** YZ_05  
**Date:** December 18, 2025  
**Status:** ✅ Complete

---

## Overview

The Variables module provides parsing and code generation for variable declarations and assignments in MELP Stage 1 compiler. This module is a pure MELP implementation based on the Stage 0 C version.

**Key Features:**
- Variable declaration parsing (numeric, string, boolean, pointer, array)
- Variable assignment parsing
- LLVM IR code generation (alloca, store, load)
- Full stateless architecture (no global state)
- Support for multiple storage locations (stack, data, bss)

---

## Architecture

### Design Principles

✅ **Stateless**: All functions use parameter passing, no mutable globals  
✅ **Modular**: Self-contained module with parser + codegen  
✅ **Import-based**: Uses core utilities from YZ_03  
✅ **LLVM Backend**: Generates standard LLVM IR

### Dependencies

```
variables/
├── variables_parser.mlp     → Uses: core/token_types.mlp
│
├── variables_codegen.mlp    → Uses: variables_parser.mlp (constants)
│
├── test_variables.mlp       → Tests both parser and codegen
└── README.md                → This file
```

---

## Files

### 1. `variables_parser.mlp`

**Purpose:** Parse variable declarations and assignments

**Constants:**

```melp
-- Variable Types
const numeric VAR_NUMERIC = 0
const numeric VAR_STRING = 1
const numeric VAR_BOOLEAN = 2
const numeric VAR_POINTER = 3
const numeric VAR_ARRAY = 4
const numeric VAR_LIST = 5
const numeric VAR_TUPLE = 6

-- Storage Locations
const numeric STORAGE_REGISTER = 0
const numeric STORAGE_STACK = 1
const numeric STORAGE_DATA = 2
const numeric STORAGE_BSS = 3
const numeric STORAGE_HEAP = 4
```

**Functions:**

#### `parse_variable_declaration(tokens, pos) → [decl_info, new_pos]`
Parse variable declaration:
```melp
numeric x = 42
string name = "Alice"
boolean flag = true
numeric* ptr
numeric[10] arr
```

**Returns:**
- `decl_info`: List with 9 elements
  - `[0]`: Variable name (text)
  - `[1]`: Variable type (numeric - VAR_*)
  - `[2]`: Initial value (text)
  - `[3]`: Storage location (numeric - STORAGE_*)
  - `[4]`: Is pointer (boolean)
  - `[5]`: Is array (boolean)
  - `[6]`: Array size (numeric)
  - `[7]`: Base type (numeric)
  - `[8]`: Has initializer (boolean)
- `new_pos`: Updated token position

#### `parse_variable_assignment(tokens, pos) → [assign_info, new_pos]`
Parse variable assignment:
```melp
x = 100
name = "Bob"
flag = false
```

**Returns:**
- `assign_info`: List with 3 elements
  - `[0]`: Variable name (text)
  - `[1]`: Value (text)
  - `[2]`: Is expression (boolean)
- `new_pos`: Updated token position

**Helper Functions:**

- `token_to_var_type(token_type) → var_type` - Map token to variable type
- `is_type_token(token_type) → boolean` - Check if token is a type keyword
- `get_var_type_name(var_type) → text` - Get type name for display
- `get_storage_name(storage) → text` - Get storage name for display

---

### 2. `variables_codegen.mlp`

**Purpose:** Generate LLVM IR for variables

**Functions:**

#### `codegen_variable_declaration(decl_info, indent) → string`
Generate LLVM IR for variable declaration with optional initialization:

**Input:**
```melp
numeric count = 10
```

**Output:**
```llvm
  ; Variable: count (type: numeric)
  %count = alloca i64, align 8
  ; Initialize count
  store i64 10, i64* %count, align 8
```

#### `codegen_variable_initialization(var_name, var_type, init_value, indent) → string`
Generate initialization code for a variable:

**Input:** `var_name="count"`, `var_type=VAR_NUMERIC`, `init_value="42"`

**Output:**
```llvm
  ; Initialize count
  store i64 42, i64* %count, align 8
```

**String initialization:**
```llvm
  ; String constant: "Hello"
  @.str.message = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
  %str_ptr_message = getelementptr inbounds [6 x i8], [6 x i8]* @.str.message, i32 0, i32 0
  store i8* %str_ptr_message, i8** %message, align 8
```

#### `codegen_variable_assignment(assign_info, var_type, indent) → string`
Generate LLVM IR for variable assignment:

**Input:**
```melp
count = 20
```

**Output:**
```llvm
  ; Assignment: count = 20
  store i64 20, i64* %count, align 8
```

#### `codegen_variable_load(var_name, var_type, result_reg, indent) → string`
Generate LLVM IR to load variable value:

**Input:** `var_name="count"`, `result_reg="temp1"`

**Output:**
```llvm
  ; Load count
  %temp1 = load i64, i64* %count, align 8
```

**Utility Functions:**

- `codegen_complete_variable(decl_info) → string` - Complete declaration with init
- `codegen_simple_assignment(var_name, var_type, value) → string` - Simple assignment
- `codegen_simple_load(var_name, var_type, result_reg) → string` - Simple load

---

### 3. `test_variables.mlp`

**Purpose:** Comprehensive test suite

**Tests:**
1. Parse numeric variable declaration
2. Parse string variable declaration
3. Parse boolean variable declaration
4. Parse variable with initializer
5. Parse variable assignment
6. CodeGen variable declaration
7. CodeGen variable assignment
8. CodeGen variable load

**Run:**
```bash
# When Stage 1 compiler is ready
./melp_compiler test_variables.mlp
```

---

## Type System

### LLVM Type Mapping

| MELP Type | LLVM Type | Description |
|-----------|-----------|-------------|
| `numeric` | `i64` | 64-bit integer (STO optimizes) |
| `string` | `i8*` | Pointer to null-terminated string |
| `boolean` | `i1` | 1-bit boolean |
| `numeric*` | `i64*` | Pointer to numeric |
| `string*` | `i8**` | Pointer to string pointer |
| `numeric[10]` | `[10 x i64]*` | Fixed-size array pointer |
| `numeric[]` | `i64*` | Dynamic array pointer |

### Alignment

- `i64` (numeric): 8-byte alignment
- `i8*` (string/pointer): 8-byte alignment
- `i1` (boolean): 1-byte alignment
- Arrays: Element alignment

---

## Usage Examples

### Example 1: Simple Variable

```melp
numeric count = 0
```

**Parser Output:**
```melp
[
  "count",        -- name
  VAR_NUMERIC,    -- type
  "0",            -- init value
  STORAGE_DATA,   -- storage
  false,          -- is pointer
  false,          -- is array
  0,              -- array size
  VAR_NUMERIC,    -- base type
  true            -- has initializer
]
```

**CodeGen Output:**
```llvm
  ; Variable: count (type: numeric)
  %count = alloca i64, align 8
  ; Initialize count
  store i64 0, i64* %count, align 8
```

---

### Example 2: String Variable

```melp
string message = "Hello, MELP!"
```

**Parser Output:**
```melp
[
  "message",
  VAR_STRING,
  "Hello, MELP!",
  STORAGE_DATA,
  false, false, 0,
  VAR_STRING,
  true
]
```

**CodeGen Output:**
```llvm
  ; Variable: message (type: string)
  %message = alloca i8*, align 8
  ; String constant: "Hello, MELP!"
  @.str.message = private unnamed_addr constant [13 x i8] c"Hello, MELP!\00", align 1
  %str_ptr_message = getelementptr inbounds [13 x i8], [13 x i8]* @.str.message, i32 0, i32 0
  store i8* %str_ptr_message, i8** %message, align 8
```

---

### Example 3: Boolean Variable

```melp
boolean is_ready = true
```

**CodeGen Output:**
```llvm
  ; Variable: is_ready (type: boolean)
  %is_ready = alloca i1, align 1
  ; Initialize is_ready
  store i1 1, i1* %is_ready, align 1
```

---

### Example 4: Variable Assignment

```melp
count = 42
```

**CodeGen Output:**
```llvm
  ; Assignment: count = 42
  store i64 42, i64* %count, align 8
```

---

### Example 5: Variable Load

```melp
-- Load count into temp register
```

**CodeGen Output:**
```llvm
  ; Load count
  %temp1 = load i64, i64* %count, align 8
```

---

## Stateless Pattern

All functions follow the stateless pattern:

```melp
-- ❌ BAD: Global state
list g_tokens
numeric g_pos

function parse() returns list
    -- Uses global state
end_function

-- ✅ GOOD: Stateless
function parse_variable_declaration(list tokens, numeric pos) returns list
    -- All state passed as parameters
    -- Returns [result, new_position]
    return [decl_info, new_pos]
end_function
```

**Benefits:**
- Thread-safe
- Testable
- Composable
- No side effects

---

## Future Enhancements

**Phase 2 (STO Integration):**
- Smart Type Optimization for numeric types
- Automatic int64 ↔ double ↔ bigdecimal promotion
- Small String Optimization (SSO) for strings ≤23 bytes
- Overflow detection at runtime

**Phase 3 (Advanced Features):**
- Array literal initialization: `numeric[3] arr = [1, 2, 3]`
- List initialization: `list items = (1; "hello"; true)`
- Tuple initialization: `tuple pair = <42, "answer">`
- Complex expressions in initializers

---

## Testing

Run test suite:
```bash
cd compiler/stage1/modules/variables/
# When compiler ready:
../../../melpc test_variables.mlp -o test_variables
./test_variables
```

Expected output:
```
=== MELP Stage 1 - Variables Module Tests ===

Test 1: Parse numeric variable (numeric x)
  ✅ PASS

Test 2: Parse string variable (string name)
  ✅ PASS

Test 3: Parse boolean variable (boolean flag)
  ✅ PASS

Test 4: Parse variable with initializer (numeric x = 42)
  ✅ PASS

Test 5: Parse variable assignment (x = 100)
  ✅ PASS

Test 6: CodeGen variable declaration (LLVM IR)
  ✅ PASS

Test 7: CodeGen variable assignment (LLVM IR)
  ✅ PASS

Test 8: CodeGen variable load (LLVM IR)
  ✅ PASS

===================
Tests Summary:
  Passed: 8
  Failed: 0
===================
✅ All tests passed!
```

---

## Integration

### With Other Modules

**Core Utilities (YZ_03):**
- Uses `TOKEN_*` constants from `token_types.mlp`

**Functions Module (YZ_04):**
- Variables can be function parameters
- Functions can declare local variables

**Future Modules:**
- **Operators:** Variables in expressions
- **Control Flow:** Variables in conditions
- **Arrays:** Array variable declarations

### Import Example

```melp
import "compiler/stage1/modules/variables/variables_parser.mlp"
import "compiler/stage1/modules/variables/variables_codegen.mlp"

function compile_variable(list tokens, numeric pos) returns text
    list result = parse_variable_declaration(tokens, pos)
    list decl_info = result[0]
    text ir_code = codegen_variable_declaration(decl_info, "  ")
    return ir_code
end_function
```

---

## Notes

### Storage Locations

- **STORAGE_STACK**: Local variables (most common)
- **STORAGE_DATA**: Initialized global variables
- **STORAGE_BSS**: Uninitialized global variables
- **STORAGE_HEAP**: Dynamically allocated (future)
- **STORAGE_REGISTER**: Temporary values (optimization)

### Limitations (Current Phase)

- Complex expressions not fully supported (parsed as runtime init)
- Array literals require array module integration
- No type inference yet (explicit types required)
- No pointer arithmetic

---

## Source Files

**Stage 0 Reference:**
- `compiler/stage0/modules/variable/variable.h` - Type definitions
- `compiler/stage0/modules/variable/variable_parser.c` - C parser
- `compiler/stage0/modules/variable/variable_codegen.c` - C codegen

**Stage 1 Implementation:**
- `compiler/stage1/modules/variables/variables_parser.mlp` - MELP parser (450 lines)
- `compiler/stage1/modules/variables/variables_codegen.mlp` - MELP codegen (400 lines)
- `compiler/stage1/modules/variables/test_variables.mlp` - Test suite (480 lines)

**Total Lines:** ~1,330 lines of MELP code

---

## Credits

**Module:** Variables  
**Author:** YZ_05  
**Date:** December 18, 2025  
**Based on:** Stage 0 C implementation by MELP Team  
**Previous Work:** YZ_03 (Core Utilities), YZ_04 (Functions Module)

---

## License

Part of the MELP (Multi-Expression Language Platform) project.
