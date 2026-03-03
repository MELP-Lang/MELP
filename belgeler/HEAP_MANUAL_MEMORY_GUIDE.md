# Heap Manual Memory Guide - MELP Stage 0

**Manual Memory Management** - allocate/free ve Vector<T> kullanım kılavuzu.

---

## ⚠️ Important: Stage 0 = Manual Memory Only

Stage 0'da **RAII yok**, **ownership yok**, **garbage collection yok**.

**Siz sorumlusunuz:**
- Ayırdığınız her belleği serbest bırakmalısınız
- Double-free'den kaçınmalısınız
- Use-after-free'den kaçınmalısınız

---

## Quick Start: allocate/free

### Basic Usage

```mlp
function main() {
    -- Allocate 100 bytes
    var ptr = allocate(100)
    
    if ptr == null {
        print("Allocation failed!")
        return 1
    }
    
    -- Use memory...
    
    -- MUST free manually!
    free(ptr)
    
    return 0
}
```

### API

```mlp
allocate(size: numeric) -> pointer
```
- Returns pointer to zero-initialized memory
- Returns `null` on failure
- User MUST call `free()`

```mlp
free(ptr: pointer) -> void
```
- Releases memory
- Safe to call with `null` (no-op)
- **DO NOT** use pointer after free!

---

## Vector<T> - Dynamic Arrays

### Create Vector

```mlp
var vec = Vector<numeric>.new()
```

Supported types:
- `Vector<numeric>` - Floating point numbers
- `Vector<string>` - Strings
- `Vector<pointer>` - Generic pointers

### Append Elements

```mlp
vec.append(10)
vec.append(20)
vec.append(30)
```

**Auto-resizing:** Capacity doubles automatically (4 → 8 → 16 → 32...)

### Get Elements

```mlp
var value = vec.get(0)  -- First element
print(value)  -- 10
```

**Bounds checking:** Returns 0 for out-of-bounds (prints error)

### Query Length

```mlp
var len = vec.length()
print(len)  -- 3
```

### Free Vector

```mlp
vec.free()  -- MUST call manually!
```

**Important:** Frees both vector structure AND all elements.

---

## Complete Example

```mlp
function demonstrate_vector() {
    print("Creating Vector<numeric>...")
    var numbers = Vector<numeric>.new()
    
    -- Add some numbers
    numbers.append(1.5)
    numbers.append(2.5)
    numbers.append(3.5)
    
    -- Check length
    var count = numbers.length()
    print("Count: ")
    print(count)  -- 3
    
    -- Access elements
    var first = numbers.get(0)
    var second = numbers.get(1)
    var sum = first + second
    
    print("Sum: ")
    print(sum)  -- 4.0
    
    -- MUST free!
    numbers.free()
    print("Vector freed")
}
```

---

## Memory Leak Prevention

### Use Valgrind

```bash
valgrind --leak-check=full ./your_program
```

**Expected output (no leaks):**
```
==PID== HEAP SUMMARY:
==PID==     in use at exit: 0 bytes in 0 blocks
==PID== All heap blocks were freed -- no leaks are possible
```

### Common Patterns

#### ✅ CORRECT: Free before return

```mlp
function process_data() {
    var data = Vector<numeric>.new()
    
    data.append(10)
    data.append(20)
    
    -- Do work...
    
    data.free()  -- ✅ Freed before return
    return result
}
```

#### ❌ WRONG: Forgot to free

```mlp
function process_data() {
    var data = Vector<numeric>.new()
    
    data.append(10)
    data.append(20)
    
    return result  -- ❌ MEMORY LEAK!
}
```

#### ✅ CORRECT: Free in all paths

```mlp
function conditional_work() {
    var data = Vector<numeric>.new()
    
    if error_condition {
        data.free()  -- ✅ Free on error path
        return 1
    }
    
    -- Do work...
    
    data.free()  -- ✅ Free on success path
    return 0
}
```

---

## Common Mistakes

### 1. Double Free

```mlp
var ptr = allocate(100)
free(ptr)
free(ptr)  -- ❌ DOUBLE FREE! (undefined behavior)
```

**Solution:** Set to null after free (manual pattern):
```mlp
var ptr = allocate(100)
free(ptr)
ptr = null
```

### 2. Use After Free

```mlp
var vec = Vector<numeric>.new()
vec.append(10)
vec.free()

var val = vec.get(0)  -- ❌ USE AFTER FREE!
```

**Solution:** Don't use after free (Stage 0 can't detect this)

### 3. Forgot to Free

```mlp
function leak() {
    var vec = Vector<numeric>.new()
    vec.append(10)
    -- ❌ Forgot vec.free()!
}
```

**Solution:** Always free before function ends

---

## Vector Capacity Growth

Initial capacity: **4 elements**

Growth pattern:
```
Elements:  1-4   → Capacity: 4
Elements:  5-8   → Capacity: 8 (doubled)
Elements:  9-16  → Capacity: 16 (doubled)
Elements: 17-32  → Capacity: 32 (doubled)
```

**Performance:**
- Append: O(1) amortized
- Get: O(1)
- Growth: O(n) only when resizing

---

## Generic Types

### Vector<numeric>

```mlp
var numbers = Vector<numeric>.new()
numbers.append(1.5)
numbers.append(2.5)
var sum = numbers.get(0) + numbers.get(1)
numbers.free()
```

### Vector<string>

```mlp
var words = Vector<string>.new()
words.append("hello")
words.append("world")
var first = words.get(0)
print(first)  -- "hello"
words.free()
```

### Vector<pointer>

```mlp
var ptrs = Vector<pointer>.new()
var p1 = allocate(10)
var p2 = allocate(20)
ptrs.append(p1)
ptrs.append(p2)

-- Don't forget to free elements!
free(ptrs.get(0))
free(ptrs.get(1))
ptrs.free()
```

---

## Limitations (Stage 0)

### ❌ NOT Supported

- **RAII** - No automatic cleanup (Stage 2)
- **Smart pointers** - No shared_ptr/unique_ptr (Stage 2)
- **Ownership** - No borrow checker (Stage 2)
- **Garbage collection** - Manual only (Stage 3+)
- **Use-after-free detection** - Compiler can't detect (Stage 2)

### ✅ Supported

- **Manual allocation/deallocation**
- **Vector<T> with generics**
- **Automatic resizing**
- **Bounds checking** (runtime error)
- **Valgrind integration** (leak detection)

---

## Testing Your Code

### Run Heap Tests

```bash
cd tests/heap
for test in test_*.pmpl; do
    ../../compiler/paradigma2/MODERN/melp_coordinator.sh "$test"
done
```

### Check for Leaks

```bash
# Compile your program
./melp_coordinator.sh your_program.pmpl

# Run with Valgrind
valgrind --leak-check=full ./your_program
```

---

## Best Practices

### 1. Free Early, Free Often

```mlp
var data = allocate(1000)
-- Use data...
free(data)  -- Free as soon as done
data = null  -- Optional: prevent use-after-free
```

### 2. One Allocation = One Free

```mlp
-- ✅ CORRECT
var p1 = allocate(10)
var p2 = allocate(20)
free(p1)
free(p2)

-- ❌ WRONG
var p1 = allocate(10)
var p2 = p1  -- Same pointer
free(p1)
free(p2)  -- DOUBLE FREE!
```

### 3. Free in Reverse Order (Optional Style)

```mlp
var a = allocate(10)
var b = allocate(20)
var c = allocate(30)

-- Free in reverse (LIFO)
free(c)
free(b)
free(a)
```

### 4. Check Null Before Use

```mlp
var ptr = allocate(1000000000)  -- Large allocation
if ptr == null {
    print("Allocation failed!")
    return 1
}

-- Use ptr...
free(ptr)
```

---

## Migration Path (Stage 2+)

When Stage 2 adds RAII:

**Stage 0 (Manual):**
```mlp
var vec = Vector<numeric>.new()
vec.append(10)
vec.free()  -- Manual!
```

**Stage 2 (Automatic):**
```mlp
{
    var vec = Vector<numeric>.new()
    vec.append(10)
    -- Automatic free on scope exit!
}
```

---

## Troubleshooting

### "Allocation failed" Error

**Cause:** Out of memory or size too large

**Solution:** Reduce allocation size or check available memory

### Segmentation Fault

**Common causes:**
1. Use-after-free
2. Double-free
3. Buffer overflow
4. Null pointer dereference

**Debug:** Use Valgrind to identify exact issue

### Memory Leak

**Detect:** Run Valgrind
```bash
valgrind --leak-check=full ./program
```

**Fix:** Add missing `free()` calls

---

## Quick Reference

```mlp
-- Allocation
var ptr = allocate(size)      -- Allocate memory
free(ptr)                     -- Free memory

-- Vector
var vec = Vector<T>.new()     -- Create vector
vec.append(value)             -- Add element
var val = vec.get(index)      -- Get element
var len = vec.length()        -- Get length
vec.free()                    -- Free vector

-- Null check
if ptr == null { ... }        -- Check allocation failure
```

---

## Next Steps

- **FFI Integration:** See [FFI_USER_GUIDE.md](FFI_USER_GUIDE.md)
- **Stage 1 Planning:** See [STAGE0_TO_STAGE1_GUIDE.md](STAGE0_TO_STAGE1_GUIDE.md)
- **Examples:** See `tests/heap/` for comprehensive examples

---

**Version:** Stage 0  
**Date:** 3 Şubat 2026  
**Status:** Production Ready ✅  
**Warning:** Manual memory management only - RAII in Stage 2
