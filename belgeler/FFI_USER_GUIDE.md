# FFI User Guide - MELP Stage 0

**Foreign Function Interface (FFI)** - C kütüphanelerini MELP'ten çağırma kılavuzu.

---

## Quick Start

### 1. Kütüphane Açma

```mlp
var lib = DynamicLibrary.open("libc.so.6")
```

### 2. Fonksiyon Çözümleme

```mlp
var strlen = lib.symbol<function(pointer) -> numeric>("strlen")
```

### 3. Fonksiyon Çağırma

```mlp
var my_string = "Hello, World!"
var length = strlen(my_string)
print(length)  -- 13
```

### 4. Kütüphane Kapatma

```mlp
lib.close()
```

---

## Type Marshalling

### MELP → C Type Mapping

| MELP Type | C Type | Notes |
|-----------|--------|-------|
| `numeric` | `double` | 64-bit floating point |
| `string` | `char*` | Null-terminated |
| `pointer` | `void*` | Generic pointer |
| `boolean` | `int` | 0 = false, 1 = true |

### Function Signatures

```mlp
-- No parameters, returns numeric
var getpid = lib.symbol<function() -> numeric>("getpid")

-- One parameter, returns pointer
var malloc = lib.symbol<function(numeric) -> pointer>("malloc")

-- Two parameters, no return
var memcpy = lib.symbol<function(pointer, pointer, numeric) -> void>("memcpy")
```

---

## Real-World Examples

### Example 1: strlen (String Length)

```mlp
function main() {
    var lib = DynamicLibrary.open("libc.so.6")
    var strlen = lib.symbol<function(pointer) -> numeric>("strlen")
    
    var text = "MELP Language"
    var len = strlen(text)
    
    print("Length: ")
    print(len)
    
    lib.close()
    return 0
}
```

### Example 2: File Operations

```mlp
function main() {
    -- Using built-in file I/O (built on FFI)
    var content = read_file("input.txt")
    print(content)
    
    var success = write_file("output.txt", "Hello from MELP!")
    
    if success == 1 {
        print("File written successfully")
    }
    
    return 0
}
```

### Example 3: Math Functions

```mlp
function calculate_sqrt() {
    var lib = DynamicLibrary.open("libm.so.6")
    var sqrt_fn = lib.symbol<function(numeric) -> numeric>("sqrt")
    
    var result = sqrt_fn(16)
    print(result)  -- 4.0
    
    lib.close()
}
```

---

## Error Handling

### Library Not Found

```mlp
var lib = DynamicLibrary.open("nonexistent.so")
-- Runtime error: Library not found
```

**Best Practice:** Check library exists before use (Stage 0 limitation: no exception handling)

### Symbol Not Found

```mlp
var lib = DynamicLibrary.open("libc.so.6")
var fn = lib.symbol<function() -> numeric>("nonexistent_function")
-- Runtime error: Symbol not resolved
```

**Best Practice:** Verify symbol names with `nm` command:
```bash
nm -D /lib/x86_64-linux-gnu/libc.so.6 | grep strlen
```

---

## Limitations (Stage 0)

### ❌ NOT Supported

- **Complex structs** - Only primitive types (numeric, string, pointer)
- **Callbacks** - Cannot pass MELP functions to C
- **Variadic functions** - printf, scanf not directly usable
- **Exception handling** - No try/catch for FFI errors

### ✅ Supported

- **Primitive types** - numeric, string, pointer, boolean
- **Simple functions** - Fixed parameter count
- **Multiple libraries** - Open multiple .so files simultaneously
- **Manual memory** - Use with allocate()/free()

---

## Advanced: Working with Pointers

### Allocate Memory for C Functions

```mlp
function use_c_memory() {
    var lib = DynamicLibrary.open("libc.so.6")
    var malloc_fn = lib.symbol<function(numeric) -> pointer>("malloc")
    var free_fn = lib.symbol<function(pointer) -> void>("free")
    
    -- Allocate 100 bytes
    var buffer = malloc_fn(100)
    
    -- Use buffer with other C functions
    -- ...
    
    -- Free memory
    free_fn(buffer)
    
    lib.close()
}
```

Or use MELP's built-in allocation:

```mlp
var buffer = allocate(100)
-- Use with C functions
free(buffer)
```

---

## Performance Notes

- **Library loading** - O(1) dlopen call
- **Symbol resolution** - O(1) dlsym call (cached internally)
- **Function calls** - Near-native C performance (no overhead)

---

## Best Practices

1. **Always close libraries:**
   ```mlp
   lib.close()  -- Release resources
   ```

2. **Check return values:**
   ```mlp
   var result = c_function()
   if result == null {
       print("Function failed")
   }
   ```

3. **Use correct types:**
   ```mlp
   -- WRONG: string for numeric parameter
   var result = sqrt_fn("16")  -- Error!
   
   -- RIGHT:
   var result = sqrt_fn(16)
   ```

4. **Free allocated memory:**
   ```mlp
   var ptr = malloc_fn(100)
   -- ... use ptr ...
   free_fn(ptr)  -- Don't forget!
   ```

---

## Testing FFI Code

Run FFI tests:
```bash
cd tests/heap
../../compiler/paradigma2/MODERN/melp_coordinator.sh test_ffi_*.pmpl
```

---

## Troubleshooting

### "Library not found" Error

**Solution:** Provide full path or add to LD_LIBRARY_PATH
```mlp
var lib = DynamicLibrary.open("/usr/lib/x86_64-linux-gnu/libc.so.6")
```

### "Symbol not found" Error

**Solution:** Check symbol exists with `nm`:
```bash
nm -D /lib/libc.so.6 | grep your_function
```

### Segmentation Fault

**Causes:**
- Wrong function signature
- Passing wrong type
- Using freed pointer

**Debug:** Use Valgrind:
```bash
valgrind --leak-check=full ./your_program
```

---

## Next Steps

- **Heap Management:** See [HEAP_MANUAL_MEMORY_GUIDE.md](HEAP_MANUAL_MEMORY_GUIDE.md)
- **Stage 1:** Learn about self-hosting in [STAGE0_TO_STAGE1_GUIDE.md](STAGE0_TO_STAGE1_GUIDE.md)
- **Examples:** See `tests/heap/` for more FFI examples

---

**Version:** Stage 0  
**Date:** 3 Şubat 2026  
**Status:** Production Ready ✅
