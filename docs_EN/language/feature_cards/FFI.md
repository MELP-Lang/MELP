# 📘 MLP Feature Card: FFI (Foreign Function Interface)

**Stage:** Stage 1 (Aktif)
**Durum:** ✅ IMPLEMENT EDİLDİ — `external function` syntax ile çalışıyor
**Implementation:** `compiler/stage1/modules/ffi/ffi_codegen.cpp`

---

## ⚠️ ÖNEMLİ: DOĞRU SYNTAX

MELP FFI `external function` keyword'ü ile çalışır.
`DynamicLibrary.open()` syntax'ı **implement edilmemiş** — bu dosyanın alt bölümlerindeki örnekler eski/planlanan syntax, ÇALIŞMIYOR.

---

## 🎯 Feature Summary

**What:** C fonksiyonlarını `.mlp` kodundan çağır.

**Why:** Sistem çağrıları, C kütüphaneleri, native entegrasyon.

**How:** `external function` declare → LLVM `declare` IR → linker C sembolünü çözümler.

---

## 📖 Gerçek (Çalışan) Syntax

### Temel Kullanım

```mlp
-- C fonksiyonu declare et
external function strlen(string s)
external function puts(string s)

function main()
    numeric n = strlen("hello")
    print(n)
end function
```

### Tip Eşlemeleri

| MELP Tipi | C Tipi |
|---|---|
| `numeric` | `int64_t` / `long` |
| `string` | `const char*` |
| `boolean` | `int` (0/1) |
| `ptr` | `void*` / `i8*` |

### Çalışan Örnekler

```mlp
-- strlen: string uzunluğu
external function strlen(string s)

-- puts: stdout'a yaz (newline ekler)
external function puts(string s)

-- atoi: string → numeric
external function atoi(string s)
```

---

## 🚫 Çalışmayan (Planlanmış) Syntax

Aşağıdaki syntax **implement edilmemiş**. Bu dosyanın geri kalanı bu eski plana ait:

```
DynamicLibrary libc = DynamicLibrary.open("libc.so.6")  ← ÇALIŞMIYOR
libc.symbol<...>("strlen")                               ← ÇALIŞMIYOR
```

---

**Last Updated:** 2 Mart 2026 (gerçek durum güncellendi)
**Status:** ✅ `external function` çalışıyor | ❌ `DynamicLibrary` implement edilmemiş

---

## (Eski Döküman — Referans İçin)

### Loading a Library (PLANLANMIŞ, ÇALIŞMIYOR)

```mlp
import { DynamicLibrary } from "std/ffi"

function main()
    DynamicLibrary libc = DynamicLibrary.open("libc.so.6")

    if libc.is_valid() then
        print("✅ Library loaded")
        libc.close()
    else
        print("❌ Failed to load")
    end if

end function
```

**Platform-specific library names:**
- Linux: `libc.so.6`, `libm.so.6`
- macOS: `libc.dylib`, `libm.dylib`
- Windows: `msvcrt.dll`, `kernel32.dll`

### Resolving Symbols

```mlp
DynamicLibrary libc = DynamicLibrary.open("libc.so.6")

-- Resolve strlen function: function(string) -> numeric
function(string) -> numeric strlen_fn = libc.symbol<function(string) -> numeric>("strlen")

if strlen_fn != null then
    numeric len = strlen_fn("Hello World")
    print(len)  -- prints 11
end if
```

**Type Safety:** The generic type `<function(string) -> numeric>` ensures compile-time type checking.

### Multiple Symbol Resolution

```mlp
DynamicLibrary libm = DynamicLibrary.open("libm.so.6")

function(numeric) -> numeric sqrt_fn = libm.symbol<function(numeric) -> numeric>("sqrt")
function(numeric) -> numeric sin_fn = libm.symbol<function(numeric) -> numeric>("sin")
function(numeric) -> numeric cos_fn = libm.symbol<function(numeric) -> numeric>("cos")

print(sqrt_fn(16.0))    -- prints 4.0
print(sin_fn(1.5708))   -- prints 1.0 (π/2)
print(cos_fn(0.0))      -- prints 1.0
```

### Error Handling

```mlp
DynamicLibrary lib = DynamicLibrary.open("nonexistent.so")

if not lib.is_valid() then
    print("Error: ")
    print(lib.get_error())
end if

function(string) -> numeric fn = lib.symbol<function(string) -> numeric>("missing_symbol")

if fn == null then
    print("Symbol not found!")
end if
```

---

## ⚙️ Type Mappings

### MELP → C Type Conversion

| MELP Type | C Type | Notes |
|-----------|--------|-------|
| `numeric` | `double` | Default numeric representation |
| `string` | `const char*` | Null-terminated C string |
| `boolean` | `int` | 0 = false, 1 = true |
| `pointer` | `void*` | Opaque pointer type |
| `none` | `void` | No return value |

### Function Pointer Types

```mlp
-- Single parameter:
function(string) -> numeric

-- Multiple parameters:
function(numeric, string, boolean) -> pointer

-- No return:
function(string, numeric) -> none

-- No parameters:
function() -> numeric
```

### Pointer Operations

```mlp
-- Null pointer
pointer p = nullptr

-- Allocate memory
pointer buffer = malloc(1024)

-- Pointer arithmetic
pointer p2 = buffer + 8

-- Dereference (read)
numeric value = *p

-- Dereference (write)
*p = 42

-- Free memory
free(buffer)
```

---

## 🔌 DynamicLibrary API

### Constructor

```mlp
DynamicLibrary lib = DynamicLibrary.open(string path)
```

**Description:** Opens a shared library (.so on Linux, .dylib on macOS, .dll on Windows)

**Parameters:**
- `path`: Path to the library file (can be absolute or system library name)

**Returns:** Library handle (valid or invalid)

**Example:**
```mlp
DynamicLibrary libc = DynamicLibrary.open("libc.so.6")
DynamicLibrary libm = DynamicLibrary.open("/usr/lib/libm.so.6")
```

### Methods

#### `is_valid()`

```mlp
boolean lib.is_valid()
```

**Description:** Returns true if library loaded successfully

**Returns:** `boolean` - true if valid, false otherwise

**Example:**
```mlp
if lib.is_valid() then
    print("Library loaded successfully")
end if
```

#### `symbol<T>()`

```mlp
function<T> lib.symbol<T>(string name)
```

**Description:** Resolves a function symbol by name with type safety

**Type Parameter:**
- `T`: Function signature (e.g., `function(string) -> numeric`)

**Parameters:**
- `name`: Symbol name as it appears in the library

**Returns:** Function pointer or `null` if symbol not found

**Example:**
```mlp
function(numeric, numeric) -> numeric pow_fn = libm.symbol<function(numeric, numeric) -> numeric>("pow")

if pow_fn != null then
    numeric result = pow_fn(2.0, 8.0)  -- 256.0
end if
```

#### `close()`

```mlp
numeric lib.close()
```

**Description:** Closes the library and frees the handle

**Returns:** `0` on success, non-zero on error

**Example:**
```mlp
numeric status = lib.close()
if status == 0 then
    print("Library closed successfully")
end if
```

#### `get_error()`

```mlp
string lib.get_error()
```

**Description:** Returns the last error message

**Returns:** Error string or empty string if no error

**Example:**
```mlp
DynamicLibrary lib = DynamicLibrary.open("missing.so")

if not lib.is_valid() then
    print(lib.get_error())  -- "cannot open shared object file: No such file or directory"
end if
```

---

## 📚 Examples

### Example 1: Math Functions (libm)

```mlp
import { DynamicLibrary } from "std/ffi"

function main()
    DynamicLibrary libm = DynamicLibrary.open("libm.so.6")
    
    if not libm.is_valid() then
        print("Failed to load libm")
    end if
    
    function(numeric) -> numeric sqrt_fn = libm.symbol<function(numeric) -> numeric>("sqrt")
    function(numeric) -> numeric sin_fn = libm.symbol<function(numeric) -> numeric>("sin")
    function(numeric) -> numeric cos_fn = libm.symbol<function(numeric) -> numeric>("cos")
    
    print("sqrt(16) = ")
    println(sqrt_fn(16.0))
    
    print("sin(π/2) = ")
    println(sin_fn(1.5708))
    
    print("cos(0) = ")
    println(cos_fn(0.0))
    
    libm.close()
end function
```

**Expected Output:**
```
sqrt(16) = 4
sin(π/2) = 1
cos(0) = 1
```

### Example 2: String Operations (libc)

```mlp
import { DynamicLibrary } from "std/ffi"

function main()
    DynamicLibrary libc = DynamicLibrary.open("libc.so.6")
    
    function(string) -> numeric strlen_fn = libc.symbol<function(string) -> numeric>("strlen")
    function(string, string) -> numeric strcmp_fn = libc.symbol<function(string, string) -> numeric>("strcmp")
    
    numeric len = strlen_fn("Hello World")
    print("Length: ")
    println(len)  -- prints 11
    
    numeric cmp = strcmp_fn("abc", "def")
    print("Compare: ")
    println(cmp)  -- prints negative number (abc < def)
    
    libc.close()
end function
```

### Example 3: Memory Management

```mlp
import { DynamicLibrary } from "std/ffi"

function main()
    DynamicLibrary libc = DynamicLibrary.open("libc.so.6")
    
    function(numeric) -> pointer malloc_fn = libc.symbol<function(numeric) -> pointer>("malloc")
    function(pointer) -> none free_fn = libc.symbol<function(pointer) -> none>("free")
    
    -- Allocate 100 bytes
    pointer buffer = malloc_fn(100)
    
    if buffer != nullptr then
        print("Allocated 100 bytes")
        
        -- Use buffer...
        
        -- Free memory
        free_fn(buffer)
        print("Memory freed")
    end if
    
    libc.close()
end function
```

### Example 4: File Operations

```mlp
import { DynamicLibrary } from "std/ffi"

function main()
    DynamicLibrary libc = DynamicLibrary.open("libc.so.6")
    
    function(string, string) -> pointer fopen_fn = libc.symbol<function(string, string) -> pointer>("fopen")
    function(pointer) -> numeric fclose_fn = libc.symbol<function(pointer) -> numeric>("fclose")
    function(string, numeric, numeric, pointer) -> numeric fwrite_fn = libc.symbol<function(string, numeric, numeric, pointer) -> numeric>("fwrite")
    
    pointer file = fopen_fn("output.txt", "w")
    
    if file != nullptr then
        string text = "Hello from FFI!"
        fwrite_fn(text, 1, 15, file)
        fclose_fn(file)
        print("File written successfully")
    end if
    
    libc.close()
end function
```

### Example 5: Complex Network Operation (HTTP GET)

```mlp
import { DynamicLibrary } from "std/ffi"

function main()
    DynamicLibrary libc = DynamicLibrary.open("libc.so.6")
    
    -- Socket functions
    function(numeric, numeric, numeric) -> numeric socket_fn = libc.symbol<function(numeric, numeric, numeric) -> numeric>("socket")
    function(numeric, pointer, numeric) -> numeric connect_fn = libc.symbol<function(numeric, pointer, numeric) -> numeric>("connect")
    function(numeric, string, numeric, numeric) -> numeric send_fn = libc.symbol<function(numeric, string, numeric, numeric) -> numeric>("send")
    function(numeric, pointer, numeric, numeric) -> numeric recv_fn = libc.symbol<function(numeric, pointer, numeric, numeric) -> numeric>("recv")
    function(numeric) -> numeric close_fn = libc.symbol<function(numeric) -> numeric>("close")
    
    -- Create socket (AF_INET=2, SOCK_STREAM=1)
    numeric sock = socket_fn(2, 1, 0)
    
    if sock >= 0 then
        print("Socket created")
        -- Connect, send HTTP request, receive response...
        close_fn(sock)
    end if
    
    libc.close()
end function
```

---

## 🚫 Stage 2 Limitations

### Cannot Use Directly

- ❌ **Variadic functions** (`printf`, `scanf`) - Limited support
- ❌ **C++ name mangling** - Only `extern "C"` functions work
- ❌ **Struct by value** - Pass pointers instead
- ❌ **Callback closures** - Use simple callbacks only
- ❌ **Thread safety** - Responsibility of the C library

### Workarounds

#### C++ Libraries

**Problem:** C++ function names are mangled.

```cpp
// C++ function (mangled name: _ZN3std6strlenEPKc)
namespace std {
    size_t strlen(const char* s);
}
```

**Solution:** Create a C wrapper.

```cpp
// wrapper.h
extern "C" {
    size_t my_strlen(const char* s);
}

// wrapper.cpp
extern "C" size_t my_strlen(const char* s) {
    return std::strlen(s);
}
```

**MELP Usage:**
```mlp
function(string) -> numeric my_strlen_fn = lib.symbol<function(string) -> numeric>("my_strlen")
```

See `examples/cpp_bridge_demo/` for complete C++ integration patterns.

#### Complex Structs

**Problem:** C struct layout must match exactly.

**Solution:** Use opaque pointers.

```cpp
// C library
typedef struct Database* DatabaseHandle;

extern "C" {
    DatabaseHandle db_create();
    void db_query(DatabaseHandle db, const char* sql);
    void db_destroy(DatabaseHandle db);
}
```

```mlp
function() -> pointer db_create = lib.symbol<function() -> pointer>("db_create")
function(pointer, string) -> none db_query = lib.symbol<function(pointer, string) -> none>("db_query")
function(pointer) -> none db_destroy = lib.symbol<function(pointer) -> none>("db_destroy")

pointer db = db_create()
db_query(db, "SELECT * FROM users")
db_destroy(db)
```

#### Callbacks

**Problem:** C libraries expect function pointers, but .mlp lambdas may capture variables.

**Solution:** Use simple functions without captures.

```mlp
-- Simple callback (no closure)
numeric function compare_numbers(pointer a; pointer b)
    numeric val_a = *a
    numeric val_b = *b
    return val_a - val_b
end function

-- Register callback
function(pointer, numeric, numeric, pointer) -> none qsort_fn = libc.symbol<...>("qsort")
qsort_fn(array_ptr, count, size, &compare_numbers)
```

---

## ⚡ Performance Considerations

### Overhead

- **Library loading:** One-time cost (~1-5ms)
- **Symbol resolution:** One-time per symbol (~0.1-0.5ms)
- **Function call:** Negligible overhead (~1-2 CPU cycles)

### Best Practices

1. **Cache symbol lookups:**
   ```mlp
   -- ✅ GOOD: Resolve once
   function(string) -> numeric strlen_fn = libc.symbol<...>("strlen")
   loop i = 0 to 1000 do
       numeric len = strlen_fn(strings[i])
   end loop
   
   -- ❌ BAD: Resolve in loop
   loop i = 0 to 1000 do
       function(string) -> numeric strlen_fn = libc.symbol<...>("strlen")
       numeric len = strlen_fn(strings[i])
   end loop
   ```

2. **Minimize library opens:**
   ```mlp
   -- ✅ GOOD: Open once
   DynamicLibrary libc = DynamicLibrary.open("libc.so.6")
   -- Use multiple functions...
   libc.close()
   
   -- ❌ BAD: Open multiple times
   DynamicLibrary libc1 = DynamicLibrary.open("libc.so.6")
   DynamicLibrary libc2 = DynamicLibrary.open("libc.so.6")
   ```

3. **Close libraries when done:**
   ```mlp
   DynamicLibrary lib = DynamicLibrary.open("...")
   -- Use library...
   lib.close()  -- Free resources
   ```

---

## 🔒 Safety Considerations

### Type Safety

**Compile-time checks:**
- ✅ Function signature must match symbol type
- ✅ Argument count and types verified
- ✅ Return type enforced

**Runtime checks:**
- ⚠️ No bounds checking on pointers
- ⚠️ No null pointer protection
- ⚠️ Memory safety depends on C library

### Memory Management

**Rules:**
1. Memory allocated by C functions (malloc) must be freed by C (free)
2. Strings passed to C functions must be null-terminated
3. Pointers returned by C functions are opaque - treat as black boxes
4. Always check for null pointers before dereferencing

**Example:**
```mlp
pointer buf = malloc_fn(100)

if buf != nullptr then
    -- Safe to use
    *buf = 42
    free_fn(buf)
else
    print("Allocation failed!")
end if
```

---

## 🧪 Testing Strategy

### Unit Tests

Test individual FFI operations:
```mlp
-- Test library loading
numeric function test_dlopen()
    DynamicLibrary lib = DynamicLibrary.open("libc.so.6")
    assert(lib.is_valid())
    lib.close()
    return 0
end function

-- Test symbol resolution
numeric function test_symbol()
    DynamicLibrary lib = DynamicLibrary.open("libc.so.6")
    function(string) -> numeric strlen_fn = lib.symbol<...>("strlen")
    assert(strlen_fn != null)
    lib.close()
    return 0
end function

-- Test function call
numeric function test_call()
    DynamicLibrary lib = DynamicLibrary.open("libc.so.6")
    function(string) -> numeric strlen_fn = lib.symbol<...>("strlen")
    numeric len = strlen_fn("Hello")
    assert(len == 5)
    lib.close()
    return 0
end function
```

### Integration Tests

Test real-world scenarios:
```mlp
-- HTTP server
-- SQLite database
-- OpenGL graphics
-- File compression (zlib)
```

---

## 📦 Standard Library Integration

### Planned Modules

Future standard library modules using FFI:

- **std/net** - Network operations (socket, HTTP, TLS)
- **std/db** - Database drivers (PostgreSQL, MySQL, SQLite)
- **std/crypto** - Cryptography (OpenSSL)
- **std/compress** - Compression (zlib, gzip)
- **std/regex** - Regular expressions (PCRE)
- **std/image** - Image processing (libpng, libjpeg)
- **std/audio** - Audio processing (PortAudio)
- **std/graphics** - OpenGL/Vulkan bindings

---

## 🔍 Debugging

### Common Errors

**Error:** `"cannot open shared object file: No such file or directory"`
```mlp
-- Solution: Check library path
DynamicLibrary lib = DynamicLibrary.open("/usr/lib/x86_64-linux-gnu/libc.so.6")
```

**Error:** Symbol not found
```mlp
-- Solution: Verify symbol exists with `nm` command
-- $ nm -D /lib/x86_64-linux-gnu/libc.so.6 | grep strlen
```

**Error:** Segmentation fault
```mlp
-- Solution: Check function signature matches exactly
-- Wrong: function(string) -> none
-- Right: function(string, numeric) -> none
```

### Debugging Tools

**Linux:**
```bash
# List symbols in library
nm -D /lib/libc.so.6

# Check library dependencies
ldd /lib/libc.so.6

# Trace library loading
LD_DEBUG=libs ./my_program
```

---

## 📚 Further Reading

- [dlopen(3) man page](https://man7.org/linux/man-pages/man3/dlopen.3.html)
- [System V ABI (calling conventions)](https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf)

### Real-World Examples

1. **[HTTP Server Example](../../examples/ffi_http_server/README.md)** (~200 LOC)
   - POSIX socket API integration
   - TCP server on port 8080
   - Demonstrates socket(), bind(), listen(), accept(), send()
   - Manual sockaddr_in structure marshalling
   - Complete with error handling and resource cleanup

2. **[SQLite Database Demo](../../examples/ffi_sqlite_demo/README.md)** (~190 LOC)
   - SQLite3 C library integration
   - CREATE TABLE, INSERT, SELECT operations
   - Prepared statement execution
   - Result iteration with sqlite3_step()
   - Proper handle management and cleanup

### See Also

- [FFI Test Suite](../../GOLDEN_TEST_SUITE/ffi_tests/) - Comprehensive test coverage
- [FFI Runtime Source](../../compiler/paradigma2/MODERN/runtime/) - C implementation details

---

**Last Updated:** 3 Şubat 2026  
**Status:** Stage 2 - In Development  
**Progress:** YZ_FFI_00-06 Complete (Runtime + Examples)  
**Next Steps:** YZ_FFI_07 (C++ Bridge)
