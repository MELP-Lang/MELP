# MLP Code Formatter (mlp-fmt)

**Task:** TODO #1 - Task 4 (Code Formatter)  
**Görevli:** YZ_04  
**Tarih:** 1 Ocak 2026  
**Süre:** 3-4 gün

---

## 📋 Overview

Code formatter for MLP language (like Prettier/Black/gofmt).

**Features:**
- Automatic code formatting
- Indent normalization (4 spaces)
- Spacing around operators
- In-place formatting (-i flag)
- Check mode (--check)

**Architecture:** Standalone binary (`mlp-fmt`) - NO main.c modification!

---

## 🏗️ Components

### Lexer (fmt_lexer.c/h)
- Tokenization (keywords, identifiers, operators, etc.)
- Token stream generation
- Source position tracking

### Formatter (fmt_formatter.c/h)
- Token-based formatting
- Indentation management
- Spacing rules
- Output generation

### Main (main.c)
- CLI interface
- File I/O
- Option parsing

---

## 📏 Code Size

```
fmt_lexer.c       : 338 satır
fmt_formatter.c   : 236 satır
main.c            : 152 satır
fmt_lexer.h       :  23 satır
fmt_formatter.h   :  18 satır
fmt_types.h       :  64 satır
Makefile          :  16 satır
-----------------------------------
TOPLAM            : 847 satır
```

**Hedef:** < 1000 satır ✅  
**Gerçek:** 847 satır ✅

---

## 🚀 Usage

### Basic formatting (stdout)
```bash
./mlp-fmt file.mlp
```

### In-place formatting
```bash
./mlp-fmt -i file.mlp
```

### Check mode (exit 1 if needs formatting)
```bash
./mlp-fmt --check file.mlp
```

### Custom options
```bash
./mlp-fmt --indent 2 --width 100 file.mlp
```

---

## 🎨 Formatting Rules

### 1. Indentation
- 4 spaces per level (configurable with --indent)
- Consistent nesting

**Before:**
```mlp
fn test():
let x=1
    return x
```

**After:**
```mlp
fn test():
    let x = 1
    return x
```

### 2. Operator Spacing
- Spaces around binary operators (+, -, *, /, =, ==, etc.)

**Before:**
```mlp
let x=1+2*3
```

**After:**
```mlp
let x = 1 + 2 * 3
```

### 3. Function Definitions
- Space after keyword
- Space after comma in parameters
- Space before colon

**Before:**
```mlp
fn calculate(x:Integer,y:Integer)->Integer:
```

**After:**
```mlp
fn calculate(x: Integer, y: Integer)-> Integer:
```

### 4. Comments
- Preserved in place
- Double space before inline comments

**Before:**
```mlp
let x=42#comment
```

**After:**
```mlp
let x = 42  # comment
```

---

## 🧪 Testing

### Run Tests
```bash
make test
# or
./test_fmt.sh
```

### Test Results
```
Test 1: Basic formatting              ✅
Test 2: In-place formatting (-i)      ✅
Test 3: Check mode (unformatted)      ✅
Test 4: Check mode (formatted)        ✅
Test 5: Complex formatting            ✅
```

**5/5 tests passing** ✅

---

## 🔧 Building

```bash
make clean
make
```

**Output:** `mlp-fmt` binary

---

## 📖 Examples

### Example 1: Function Formatting
**Input:**
```mlp
fn   calculate(  x:Integer,y:Integer )->Integer:
    let result=x+y
    return result
```

**Output:**
```mlp
fn calculate(x: Integer, y: Integer)-> Integer:
    let result = x + y
    return result
```

### Example 2: Control Flow
**Input:**
```mlp
if x>10:
print("big")
else:
print("small")
```

**Output:**
```mlp
if x > 10:
    print("big")
else:
    print("small")
```

### Example 3: Loops
**Input:**
```mlp
while counter<10:
counter=counter+1
```

**Output:**
```mlp
while counter < 10:
    counter = counter + 1
```

---

## 🎯 Features

- ✅ Keyword spacing
- ✅ Operator spacing
- ✅ Indentation
- ✅ Function formatting
- ✅ Comment preservation
- ✅ String literal preservation
- ✅ In-place mode
- ✅ Check mode
- ✅ Configurable indent size
- ✅ File I/O

---

## 🔮 Future Enhancements

- [ ] Line wrapping (80 chars)
- [ ] Import sorting
- [ ] Blank line management
- [ ] Multi-line expression formatting
- [ ] Configuration file support (.mlpfmt)
- [ ] IDE integration
- [ ] Batch formatting (multiple files)

---

## 📝 Notes

- **main.c:** UNTOUCHED (687 lines) ✅
- **Modular:** Standalone binary ✅
- **Test-driven:** 5/5 tests passing ✅
- **Small:** 847 lines (< 1000 target) ✅

---

**Date:** 1 Ocak 2026  
**Status:** ✅ PRODUCTION READY  
**Next:** Code formatter documentation and integration
