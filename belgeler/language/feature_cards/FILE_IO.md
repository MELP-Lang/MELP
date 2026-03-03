# 📘 PMLP Feature Card: File I/O

**Durum:** ✅ COMPLETE (Stage 1)  
**Sözdizimi Sürümü:** pmlp1 / Stage1  

**Stage:** Stage 0 (Unblock for Stage 1)  
**Complexity:** ⭐ Easy  
**Estimated Time:** 2-3 hours

---

## 🎯 Feature Summary

**What:** Built-in file operations (read, write, append).

**Why:** Enable file-based I/O for Stage 1 modules.

**How:** Built-in functions: `read_file()`, `write_file()`, `append_file()`

---

## 📖 Syntax Reference

### Read File

```pmlp
function test_read() as numeric
    string content = read_file("data.txt")
    print(content)
    return 0
end_function
```

**Returns:** `string` (file contents) or empty string on error.

### Write File

```pmlp
function test_write() as numeric
    string data = "Hello, World!"
    numeric success = write_file("output.txt"; data)
    
    if success == 1 then
        print("File written successfully")
    else
        print("Write failed")
    end_if
    
    return 0
end_function
```

**Returns:** `1` (success) or `0` (failure).

### Append File

```pmlp
function test_append() as numeric
    string log = "New log entry\n"
    numeric success = append_file("log.txt"; log)
    return success
end_function
```

**Returns:** `1` (success) or `0` (failure).

### Combined Example

```pmlp
function process_file() as numeric
    -- Read input
    string content = read_file("input.txt")
    
    if content == "" then
        print("Error: Could not read file")
        return 1
    end_if
    
    -- Process content
    string processed = content + "\nProcessed!"
    
    -- Write output
    numeric result = write_file("output.txt"; processed)
    
    return result
end_function
```

---

## 🧪 Test Scenarios

### Test 1: Basic Read
```pmlp
function test_01_read_file() as numeric
    string content = read_file("test_data.txt")
    return 0
end_function
```

### Test 2: Basic Write
```pmlp
function test_02_write_file() as numeric
    string data = "Test content"
    numeric success = write_file("test_output.txt"; data)
    return success
end_function
```

### Test 3: Append
```pmlp
function test_03_append_file() as numeric
    string line1 = "Line 1\n"
    string line2 = "Line 2\n"
    
    write_file("test.txt"; line1)
    append_file("test.txt"; line2)
    
    return 0
end_function
```

### Test 4: Read-Process-Write
```pmlp
function test_04_read_write() as numeric
    string input = read_file("input.txt")
    numeric wrote = write_file("output.txt"; input)
    return wrote
end_function
```

### Test 5: Error Handling
```pmlp
function test_05_error_handling() as numeric
    string content = read_file("nonexistent.txt")
    
    if content == "" then
        print("File not found (expected)")
        return 0
    end_if
    
    return 1
end_function
```

---

## ⚠️ Important Notes

### Syntax Rules
- **Function names:** `read_file`, `write_file`, `append_file` (built-in keywords)
- **Parameters:** Semicolon (`;`) separated
- **Return types:** Declared with `as numeric` or `as string`
- **Variable declarations:** Type before name (`string content`)

### Error Handling
- `read_file()` returns empty string on error
- `write_file()` and `append_file()` return `0` on error, `1` on success
- No exceptions - check return values!

### File Paths
- Relative to executable location
- Use forward slashes: `"data/input.txt"`

---

## 🔗 Related Features

- **string** - For content manipulation
- **if_statement** - For error checking
- **variable** - For storing file contents

---

## 📚 Implementation Reference

**Runtime:** `compiler/paradigma2/MODERN/runtime/builtins.cpp`  
**LLVM Declarations:** `io_codegen.cpp`  
**Source Evidence:** `/home/pardus/projeler/MLP/Arşiv/MLP_selfhosting_yedek/runtime/stdlib/mlp_io.c`

---

**Last Updated:** 2 Şubat 2026
