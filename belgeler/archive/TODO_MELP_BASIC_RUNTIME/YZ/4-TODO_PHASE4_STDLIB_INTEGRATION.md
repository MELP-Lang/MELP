# 📋 PHASE 4: STDLIB INTEGRATION

**Duration:** 2-3 saat  
**Priority:** 🔴 CRITICAL  
**Dependencies:** Phase 1-3 complete (full compilation working)  
**Owner:** YZ_BASIC_03

---

## 🎯 PHASE GOAL

Print wrappers gerçekten çalışmalı - programs execute and produce output!

**Current State:**
```bash
$ ./melp_compiler test.mlp -o test.ll  # ✅ Works (Phase 3)
$ llc test.ll -o test.o                 # ✅ Works (Phase 3)
$ clang test.o -o test                  # ❌ FAILS: undefined symbol __builtin_io_print_string
```

**Target State:**
```bash
$ ./melp_compiler test.mlp -o test.ll  # ✅ Compiles
$ llc test.ll -o test.o                 # ✅ Assembles
$ clang test.o -lmelp_runtime -o test   # ✅ Links!
$ ./test                                # ✅ RUNS and prints output!
merhaba dünya
42
```

---

## 📋 TASK BREAKDOWN

### Task 4.1: Choose Stdlib Strategy (30 min)

**Goal:** Decide how to provide runtime implementations

**Option A: Link Against Stage 0 Stdlib (RECOMMENDED)**

**Pros:**
- ✅ Proven code (31 FFI tests passing)
- ✅ Zero risk
- ✅ No code duplication
- ✅ Consistent semantics

**Cons:**
- ⚠️ Dependency on Stage 0
- ⚠️ Might have extra features (but that's fine)

**Implementation:**
```bash
# Stage 0 stdlib location
ls compiler/stage0/builtin/builtin_registry.cpp
ls compiler/stage0/codegen/string_ops.cpp

# Link against Stage 0's library
clang test.o -L./compiler/stage0/lib -lmelp_stdlib -o test
```

---

**Option B: Create Minimal Runtime (ALTERNATIVE)**

**Pros:**
- ✅ Stage 1 independent
- ✅ Minimal code (only what's needed)
- ✅ Easy to understand

**Cons:**
- ⚠️ Code duplication
- ⚠️ Must maintain two implementations

**Implementation:**
Create new file: `compiler/stage1/runtime/builtin_io.cpp`

---

**DECISION:** Choose Option A FIRST (link Stage 0). If linking fails, fallback to Option B (create minimal runtime).

---

### Task 4.2: Test Stage 0 Linking (Option A) (45 min)

**Goal:** Verify Stage 0 stdlib can be linked

**Step 1: Locate Stage 0 Library**
```bash
# Find Stage 0 stdlib object files
find compiler/stage0 -name "*.o" | grep -i builtin
find compiler/stage0 -name "libmelp*.a"

# If built:
ls compiler/stage0/build/*.o
ls compiler/stage0/build/*.a
```

**Step 2: Build Stage 0 Stdlib (if needed)**
```bash
cd compiler/stage0
mkdir -p build

# Compile builtin functions
clang++ -c builtin/builtin_registry.cpp -o build/builtin_registry.o -std=c++17
clang++ -c codegen/string_ops.cpp -o build/string_ops.o -std=c++17

# Create static library
ar rcs build/libmelp_stdlib.a build/builtin_registry.o build/string_ops.o

echo "✅ Stage 0 stdlib built"
```

**Step 3: Test Linking**
```bash
# Create test program
cat > /tmp/test_link.mlp << 'EOF'
function main()
  print("test linking")
  return 0
end_function
EOF

# Compile
./melp_compiler /tmp/test_link.mlp -o /tmp/test_link.ll
llc /tmp/test_link.ll -o /tmp/test_link.o

# Link against Stage 0 stdlib
clang /tmp/test_link.o -L./compiler/stage0/build -lmelp_stdlib -o /tmp/test_link

# Check for undefined symbols
nm /tmp/test_link | grep -i "U __builtin"
# Should be empty (all symbols resolved)

# Run!
/tmp/test_link
# Expected: test linking
```

**If Success:** Continue with Option A (document linking in build.sh)  
**If Failure:** Fallback to Option B (create minimal runtime)

---

### Task 4.3: Create Minimal Runtime (Option B - If Needed) (90 min)

**Goal:** Implement minimal print wrappers from scratch

**File: compiler/stage1/runtime/builtin_io.cpp** (NEW)

```cpp
/**
 * MELP Stage 1 Runtime Library
 * Minimal print implementations
 */

#include <cstdio>
#include <cstdint>
#include <cstring>

// Extern "C" prevents name mangling (allows LLVM to find functions)
extern "C" {

/**
 * Print a string (UTF-8)
 * @param str - Null-terminated C string
 */
void __builtin_io_print_string(const char* str) {
    if (str != nullptr) {
        printf("%s", str);
        fflush(stdout);  // Immediate output
    }
}

/**
 * Print a numeric value (i64)
 * @param num - 64-bit signed integer
 */
void __builtin_io_print_numeric(int64_t num) {
    printf("%lld", num);
    fflush(stdout);
}

/**
 * Print newline (helper for println)
 */
void __builtin_io_print_newline() {
    printf("\n");
    fflush(stdout);
}

/**
 * Print boolean (true/false)
 * @param val - Boolean value (0 = false, 1 = true)
 */
void __builtin_io_print_boolean(bool val) {
    printf("%s", val ? "true" : "false");
    fflush(stdout);
}

} // extern "C"
```

**Compile Runtime:**
```bash
mkdir -p compiler/stage1/runtime/build

clang++ -c compiler/stage1/runtime/builtin_io.cpp \
    -o compiler/stage1/runtime/build/builtin_io.o \
    -std=c++17 \
    -O2

# Create static library
ar rcs compiler/stage1/runtime/build/libmelp_runtime.a \
    compiler/stage1/runtime/build/builtin_io.o

echo "✅ Minimal runtime built"
```

**Test Linking:**
```bash
# Compile MELP program
./melp_compiler /tmp/test.mlp -o /tmp/test.ll
llc /tmp/test.ll -o /tmp/test.o

# Link against minimal runtime
clang /tmp/test.o \
    -L./compiler/stage1/runtime/build \
    -lmelp_runtime \
    -o /tmp/test

# Run
/tmp/test
```

---

### Task 4.4: Update Build System (60 min)

**Goal:** Automate stdlib compilation and linking

**Option 1: Update build.sh**

**File: build.sh** (modify existing)

```bash
#!/bin/bash
# MELP Stage 1 Build Script

set -e  # Exit on error

echo "🔨 Building MELP Stage 1 Compiler..."

# ========================================
# STEP 1: Build Runtime Library
# ========================================
echo "📦 Building runtime library..."

mkdir -p compiler/stage1/runtime/build

# Compile runtime
clang++ -c compiler/stage1/runtime/builtin_io.cpp \
    -o compiler/stage1/runtime/build/builtin_io.o \
    -std=c++17 -O2 -Wall -Wextra

# Create static library
ar rcs compiler/stage1/runtime/build/libmelp_runtime.a \
    compiler/stage1/runtime/build/builtin_io.o

echo "✅ Runtime library built: libmelp_runtime.a"

# ========================================
# STEP 2: Build Compiler
# ========================================
echo "🔧 Building compiler..."

# Your existing compiler build commands
# ...

echo "✅ Compiler built"

# ========================================
# STEP 3: Usage Instructions
# ========================================
echo ""
echo "✅ Build complete!"
echo ""
echo "Usage:"
echo "  ./melp_compiler <source.mlp> -o <output.ll>"
echo "  llc <output.ll> -o <output.o>"
echo "  clang <output.o> -L./compiler/stage1/runtime/build -lmelp_runtime -o <executable>"
echo ""
```

**Make Executable:**
```bash
chmod +x build.sh
./build.sh
```

---

**Option 2: Add to CMakeLists.txt**

**File: CMakeLists.txt** (if using CMake)

```cmake
# MELP Stage 1 Runtime Library

add_library(melp_runtime STATIC
    compiler/stage1/runtime/builtin_io.cpp
)

target_compile_features(melp_runtime PRIVATE cxx_std_17)
target_compile_options(melp_runtime PRIVATE -Wall -Wextra -O2)

# Install library
install(TARGETS melp_runtime
    ARCHIVE DESTINATION lib
)

# Usage note
message(STATUS "Runtime library: libmelp_runtime.a")
message(STATUS "Link with: -L\${CMAKE_INSTALL_PREFIX}/lib -lmelp_runtime")
```

**Build:**
```bash
mkdir -p build && cd build
cmake ..
make
sudo make install  # Installs to /usr/local/lib/libmelp_runtime.a
```

---

### Task 4.5: Create Compilation Helper Script (30 min)

**Goal:** Simplify MELP program compilation

**File: melpc (NEW)** - MELP Compiler Wrapper

```bash
#!/bin/bash
# MELP Compiler Wrapper Script
# Usage: ./melpc <source.mlp> [-o <output>]

set -e

if [ $# -lt 1 ]; then
    echo "Usage: ./melpc <source.mlp> [-o <output>]"
    exit 1
fi

SOURCE=$1
OUTPUT=${2:-a.out}

# Determine output name
if [ "$2" == "-o" ] && [ -n "$3" ]; then
    OUTPUT=$3
fi

BASENAME=$(basename "$SOURCE" .mlp)
TEMP_LL="/tmp/${BASENAME}.ll"
TEMP_O="/tmp/${BASENAME}.o"

echo "🔨 Compiling $SOURCE..."

# Step 1: MELP → LLVM IR
echo "  [1/4] MELP → LLVM IR"
./melp_compiler "$SOURCE" -o "$TEMP_LL"

# Step 2: LLVM IR → Assembly
echo "  [2/4] LLVM IR → Assembly"
llc "$TEMP_LL" -o "$TEMP_O"

# Step 3: Link runtime
echo "  [3/4] Linking runtime"
RUNTIME_LIB="./compiler/stage1/runtime/build/libmelp_runtime.a"

if [ ! -f "$RUNTIME_LIB" ]; then
    echo "❌ Error: Runtime library not found at $RUNTIME_LIB"
    echo "   Run ./build.sh first"
    exit 1
fi

clang "$TEMP_O" -L./compiler/stage1/runtime/build -lmelp_runtime -o "$OUTPUT"

# Step 4: Done
echo "  [4/4] Done!"
echo "✅ Compiled: $OUTPUT"
echo ""
echo "Run with: ./$OUTPUT"

# Cleanup temp files
rm -f "$TEMP_LL" "$TEMP_O"
```

**Make Executable:**
```bash
chmod +x melpc
```

**Usage:**
```bash
# Simple usage
./melpc test.mlp

# Custom output
./melpc test.mlp -o myprogram

# Run
./myprogram
```

---

### Task 4.6: Test Full Pipeline (45 min)

**Test Case 1: Hello World**
```bash
cat > /tmp/hello.mlp << 'EOF'
function main()
  print("merhaba dünya")
  return 0
end_function
EOF

./melpc /tmp/hello.mlp -o /tmp/hello
/tmp/hello
# Expected: merhaba dünya
```

**Test Case 2: Print Numeric**
```bash
cat > /tmp/number.mlp << 'EOF'
function main()
  numeric x = 42
  print(x)
  return 0
end_function
EOF

./melpc /tmp/number.mlp -o /tmp/number
/tmp/number
# Expected: 42
```

**Test Case 3: Mixed Output**
```bash
cat > /tmp/mixed.mlp << 'EOF'
function main()
  print("Sonuç: ")
  numeric result = 10 + 32
  print(result)
  print("\n")
  return 0
end_function
EOF

./melpc /tmp/mixed.mlp -o /tmp/mixed
/tmp/mixed
# Expected: Sonuç: 42\n
```

**Test Case 4: UTF-8 Turkish**
```bash
cat > /tmp/turkish.mlp << 'EOF'
function main()
  print("Merhaba Dünya! Türkçe: ğüşıöç")
  return 0
end_function
EOF

./melpc /tmp/turkish.mlp -o /tmp/turkish
/tmp/turkish
# Expected: Merhaba Dünya! Türkçe: ğüşıöç
```

---

## ✅ SUCCESS CRITERIA

**Phase 4 Complete When:**
- [ ] Runtime library exists (Stage 0 link OR minimal runtime)
- [ ] Runtime compiles to `.a` static library
- [ ] Programs link without undefined symbols
- [ ] Programs execute successfully
- [ ] `print("string")` produces output
- [ ] `print(number)` produces output
- [ ] UTF-8 strings display correctly
- [ ] Build system updated (build.sh OR CMake)
- [ ] Helper script created (melpc)
- [ ] All 4 test cases pass
- [ ] No runtime crashes
- [ ] Valgrind clean (test in Phase 5)

---

## 🚧 TROUBLESHOOTING

### Problem 1: Undefined Symbol Error

**Symptom:**
```
undefined reference to `__builtin_io_print_string'
```

**Solution:**
Ensure runtime is linked:
```bash
# Check runtime exists
ls -la compiler/stage1/runtime/build/libmelp_runtime.a

# Check link command includes runtime
clang test.o -L./compiler/stage1/runtime/build -lmelp_runtime -o test

# Verify symbols in library
nm compiler/stage1/runtime/build/libmelp_runtime.a | grep print
```

---

### Problem 2: Name Mangling (C++)

**Symptom:**
Linker can't find functions even though library has them

**Solution:**
Ensure `extern "C"` in runtime:
```cpp
extern "C" {
    void __builtin_io_print_string(const char* str) {
        // ...
    }
}
```

Check with:
```bash
nm builtin_io.o | grep print
# Should show: T __builtin_io_print_string (not mangled)
```

---

### Problem 3: Library Not Found

**Symptom:**
```
ld: library not found for -lmelp_runtime
```

**Solution:**
Use absolute path or -L flag:
```bash
# Option 1: Absolute path
clang test.o /full/path/to/libmelp_runtime.a -o test

# Option 2: -L flag
clang test.o -L./compiler/stage1/runtime/build -lmelp_runtime -o test
```

---

### Problem 4: UTF-8 Display Issues

**Symptom:**
Turkish characters show as `�` or garbage

**Solution:**
- Check terminal supports UTF-8: `locale`
- Ensure source files are UTF-8 encoded
- Runtime doesn't need special handling (just pass bytes through)

---

## 📚 REFERENCE CODE

**Stage 0 Builtin Registry:**
```bash
cat compiler/stage0/builtin/builtin_registry.cpp
# Study how Stage 0 implements print functions
```

**LLVM Static Library Linking:**
```bash
# Create library
ar rcs libmylib.a file1.o file2.o

# Link library
clang main.o -L./lib -lmylib -o program
```

---

## 🎯 NEXT PHASE

**When Phase 4 Complete:**
📄 **Move to [5-TODO_TESTING_VALIDATION.md](5-TODO_TESTING_VALIDATION.md)**

**You'll add:**
- Comprehensive test suite
- Memory leak testing (valgrind)
- LLVM IR validation
- Performance benchmarks
- **Final approval for production!**

---

## 📋 PHASE 4 CHECKLIST

Before moving to Phase 5:

**Runtime:**
- [ ] Runtime library compiled
- [ ] Static library (.a) created
- [ ] Functions exported (extern "C")
- [ ] No name mangling

**Build System:**
- [ ] build.sh updated OR CMake configured
- [ ] Runtime builds automatically
- [ ] Helper script (melpc) works
- [ ] Documentation updated

**Testing:**
- [ ] Hello world runs
- [ ] Print numeric runs
- [ ] Mixed output runs
- [ ] UTF-8 strings work
- [ ] No linker errors
- [ ] No runtime crashes

**Quality:**
- [ ] Clean build (no warnings)
- [ ] Library stripped (optional: strip -S)
- [ ] Helper script tested
- [ ] Usage docs written

---

**Ready? Start Task 4.1! 🚀**
