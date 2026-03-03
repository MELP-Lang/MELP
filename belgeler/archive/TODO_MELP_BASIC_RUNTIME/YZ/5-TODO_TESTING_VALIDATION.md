# 📋 PHASE 5: TESTING & VALIDATION

**Duration:** 2-3 saat  
**Priority:** 🔴 CRITICAL  
**Dependencies:** Phase 1-4 complete (full pipeline working)  
**Owner:** YZ_BASIC_03

---

## 🎯 PHASE GOAL

Comprehensive testing and validation - ensure production readiness!

**Quality Gates:**
- ✅ All MVP test cases pass
- ✅ No memory leaks (valgrind clean)
- ✅ LLVM IR validates
- ✅ No warnings or errors
- ✅ Performance acceptable
- ✅ Documentation complete

---

## 📋 TASK BREAKDOWN

### Task 5.1: Create Test Suite (60 min)

**Goal:** Comprehensive test coverage

**Test Suite Structure:**
```
tests/phase8/
├── test_print_string.mlp           # Basic string print
├── test_print_numeric.mlp          # Basic numeric print
├── test_print_variable.mlp         # Variable print
├── test_mixed_io.mlp               # Mixed string + numeric
├── test_math_print.mlp             # Math expression + print
├── test_utf8_strings.mlp           # Turkish characters
├── test_multiple_prints.mlp        # Many print calls
├── test_newlines.mlp               # \n, \t escapes
├── test_empty_string.mlp           # Edge case: ""
├── test_large_number.mlp           # Edge case: large int
└── run_all_tests.sh                # Test runner script
```

---

**Test 1: test_print_string.mlp**
```mlp
function main()
  print("hello world")
  return 0
end_function
```
**Expected:** `hello world`

---

**Test 2: test_print_numeric.mlp**
```mlp
function main()
  print(42)
  return 0
end_function
```
**Expected:** `42`

---

**Test 3: test_print_variable.mlp**
```mlp
function main()
  numeric x = 123
  print(x)
  return 0
end_function
```
**Expected:** `123`

---

**Test 4: test_mixed_io.mlp**
```mlp
function main()
  print("Result: ")
  numeric x = 42
  print(x)
  print("\n")
  return 0
end_function
```
**Expected:** `Result: 42\n`

---

**Test 5: test_math_print.mlp**
```mlp
function main()
  numeric x = 10
  numeric y = 32
  numeric sum = x + y
  print("10 + 32 = ")
  print(sum)
  return 0
end_function
```
**Expected:** `10 + 32 = 42`

---

**Test 6: test_utf8_strings.mlp**
```mlp
function main()
  print("Türkçe karakter testi: ğüşıöçĞÜŞİÖÇ")
  return 0
end_function
```
**Expected:** `Türkçe karakter testi: ğüşıöçĞÜŞİÖÇ`

---

**Test 7: test_multiple_prints.mlp**
```mlp
function main()
  print("Line 1")
  print("\n")
  print("Line 2")
  print("\n")
  print("Line 3")
  return 0
end_function
```
**Expected:**
```
Line 1
Line 2
Line 3
```

---

**Test 8: test_newlines.mlp**
```mlp
function main()
  print("Tab:\there\n")
  print("Newline:\nhere")
  return 0
end_function
```
**Expected:**
```
Tab:	here
Newline:
here
```

---

**Test 9: test_empty_string.mlp** (Edge Case)
```mlp
function main()
  print("")
  print("after empty")
  return 0
end_function
```
**Expected:** `after empty`

---

**Test 10: test_large_number.mlp** (Edge Case)
```mlp
function main()
  numeric big = 9223372036854775807
  print(big)
  return 0
end_function
```
**Expected:** `9223372036854775807` (i64 max value)

---

### Task 5.2: Create Test Runner Script (30 min)

**File: tests/phase8/run_all_tests.sh**

```bash
#!/bin/bash
# MELP Basic Runtime Test Suite Runner

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test counter
TOTAL=0
PASSED=0
FAILED=0

# Test definitions: "name:expected_output"
TESTS=(
    "test_print_string:hello world"
    "test_print_numeric:42"
    "test_print_variable:123"
    "test_mixed_io:Result: 42"
    "test_math_print:10 + 32 = 42"
    "test_utf8_strings:Türkçe karakter testi: ğüşıöçĞÜŞİÖÇ"
    "test_empty_string:after empty"
    "test_large_number:9223372036854775807"
)

echo "🧪 MELP Basic Runtime Test Suite"
echo "================================="
echo ""

# Create temp directory
TEMP_DIR="/tmp/melp_tests_$$"
mkdir -p "$TEMP_DIR"

for test_case in "${TESTS[@]}"; do
    IFS=':' read -r name expected <<< "$test_case"
    
    TOTAL=$((TOTAL + 1))
    echo -n "[$TOTAL] Testing $name... "
    
    TEST_FILE="tests/phase8/${name}.mlp"
    
    if [ ! -f "$TEST_FILE" ]; then
        echo -e "${RED}SKIP${NC} (file not found)"
        continue
    fi
    
    # Compile
    if ! ./melpc "$TEST_FILE" -o "$TEMP_DIR/$name" &> "$TEMP_DIR/${name}_compile.log"; then
        echo -e "${RED}FAIL${NC} (compilation error)"
        cat "$TEMP_DIR/${name}_compile.log"
        FAILED=$((FAILED + 1))
        continue
    fi
    
    # Run
    output=$("$TEMP_DIR/$name" 2>&1)
    
    # Compare output
    if [[ "$output" == *"$expected"* ]]; then
        echo -e "${GREEN}PASS${NC}"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}FAIL${NC}"
        echo "  Expected: $expected"
        echo "  Got:      $output"
        FAILED=$((FAILED + 1))
    fi
done

# Cleanup
rm -rf "$TEMP_DIR"

# Summary
echo ""
echo "================================="
echo "Test Summary:"
echo "  Total:  $TOTAL"
echo -e "  Passed: ${GREEN}$PASSED${NC}"
if [ $FAILED -gt 0 ]; then
    echo -e "  Failed: ${RED}$FAILED${NC}"
else
    echo "  Failed: 0"
fi
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✅ All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}❌ Some tests failed${NC}"
    exit 1
fi
```

**Make Executable:**
```bash
chmod +x tests/phase8/run_all_tests.sh
```

**Run Tests:**
```bash
./tests/phase8/run_all_tests.sh
```

---

### Task 5.3: Memory Leak Testing (Valgrind) (30 min)

**Goal:** Ensure no memory leaks

**Install Valgrind (if needed):**
```bash
# Ubuntu/Debian
sudo apt-get install valgrind

# Check version
valgrind --version
```

**Test Script: tests/phase8/test_memory_leaks.sh**

```bash
#!/bin/bash
# Memory Leak Testing with Valgrind

echo "🔍 Memory Leak Testing"
echo "====================="
echo ""

TESTS=(
    "test_print_string"
    "test_print_numeric"
    "test_mixed_io"
    "test_math_print"
)

TEMP_DIR="/tmp/melp_valgrind_$$"
mkdir -p "$TEMP_DIR"

ALL_CLEAN=true

for test in "${TESTS[@]}"; do
    echo "Testing $test..."
    
    # Compile
    ./melpc "tests/phase8/${test}.mlp" -o "$TEMP_DIR/$test" &> /dev/null
    
    # Run with valgrind
    valgrind --leak-check=full \
             --show-leak-kinds=all \
             --track-origins=yes \
             --error-exitcode=1 \
             "$TEMP_DIR/$test" &> "$TEMP_DIR/${test}_valgrind.log"
    
    EXIT_CODE=$?
    
    if [ $EXIT_CODE -eq 0 ]; then
        echo "  ✅ No leaks detected"
    else
        echo "  ❌ Memory leak detected!"
        cat "$TEMP_DIR/${test}_valgrind.log"
        ALL_CLEAN=false
    fi
done

rm -rf "$TEMP_DIR"

echo ""
if $ALL_CLEAN; then
    echo "✅ All tests clean (no memory leaks)"
    exit 0
else
    echo "❌ Memory leaks detected"
    exit 1
fi
```

**Expected Output:**
```
==12345== HEAP SUMMARY:
==12345==     in use at exit: 0 bytes in 0 blocks
==12345==   total heap usage: X allocs, X frees, Y bytes allocated
==12345==
==12345== All heap blocks were freed -- no leaks are possible
```

---

### Task 5.4: LLVM IR Validation (30 min)

**Goal:** Ensure generated IR is valid

**Test Script: tests/phase8/validate_llvm_ir.sh**

```bash
#!/bin/bash
# LLVM IR Validation

echo "🔧 LLVM IR Validation"
echo "===================="
echo ""

TESTS=(
    "test_print_string"
    "test_print_numeric"
    "test_mixed_io"
    "test_math_print"
)

TEMP_DIR="/tmp/melp_ir_validation_$$"
mkdir -p "$TEMP_DIR"

ALL_VALID=true

for test in "${TESTS[@]}"; do
    echo "Validating $test..."
    
    TEST_FILE="tests/phase8/${test}.mlp"
    IR_FILE="$TEMP_DIR/${test}.ll"
    BC_FILE="$TEMP_DIR/${test}.bc"
    
    # Step 1: Compile to IR
    if ! ./melp_compiler "$TEST_FILE" -o "$IR_FILE" &> "$TEMP_DIR/${test}_compile.log"; then
        echo "  ❌ Compilation failed"
        cat "$TEMP_DIR/${test}_compile.log"
        ALL_VALID=false
        continue
    fi
    
    # Step 2: Assemble to bitcode
    if ! llvm-as "$IR_FILE" -o "$BC_FILE" 2> "$TEMP_DIR/${test}_llvm_as.log"; then
        echo "  ❌ llvm-as failed (invalid IR syntax)"
        cat "$TEMP_DIR/${test}_llvm_as.log"
        ALL_VALID=false
        continue
    fi
    
    # Step 3: Verify IR semantics
    if ! opt -verify "$BC_FILE" -o /dev/null 2> "$TEMP_DIR/${test}_verify.log"; then
        echo "  ❌ opt -verify failed (invalid IR semantics)"
        cat "$TEMP_DIR/${test}_verify.log"
        ALL_VALID=false
        continue
    fi
    
    # Step 4: Compile to object
    if ! llc "$IR_FILE" -o "$TEMP_DIR/${test}.o" 2> "$TEMP_DIR/${test}_llc.log"; then
        echo "  ❌ llc failed (codegen error)"
        cat "$TEMP_DIR/${test}_llc.log"
        ALL_VALID=false
        continue
    fi
    
    echo "  ✅ Valid IR"
done

rm -rf "$TEMP_DIR"

echo ""
if $ALL_VALID; then
    echo "✅ All IR valid"
    exit 0
else
    echo "❌ Some IR invalid"
    exit 1
fi
```

---

### Task 5.5: Performance Benchmarks (30 min)

**Goal:** Ensure compilation is reasonably fast

**Test Script: tests/phase8/benchmark_compilation.sh**

```bash
#!/bin/bash
# Compilation Performance Benchmark

echo "⏱️  Compilation Performance"
echo "========================="
echo ""

# Create test programs of varying sizes
TEMP_DIR="/tmp/melp_benchmark_$$"
mkdir -p "$TEMP_DIR"

# Small program (1 print)
cat > "$TEMP_DIR/small.mlp" << 'EOF'
function main()
  print("test")
  return 0
end_function
EOF

# Medium program (100 prints)
cat > "$TEMP_DIR/medium.mlp" << 'EOF'
function main()
EOF
for i in {1..100}; do
    echo "  print(\"Line $i\")" >> "$TEMP_DIR/medium.mlp"
done
echo "  return 0" >> "$TEMP_DIR/medium.mlp"
echo "end_function" >> "$TEMP_DIR/medium.mlp"

# Large program (1000 prints)
cat > "$TEMP_DIR/large.mlp" << 'EOF'
function main()
EOF
for i in {1..1000}; do
    echo "  print(\"Line $i\")" >> "$TEMP_DIR/large.mlp"
done
echo "  return 0" >> "$TEMP_DIR/large.mlp"
echo "end_function" >> "$TEMP_DIR/large.mlp"

# Benchmark compilation time
for size in small medium large; do
    echo "Testing $size program..."
    
    START=$(date +%s.%N)
    ./melp_compiler "$TEMP_DIR/$size.mlp" -o "$TEMP_DIR/$size.ll" &> /dev/null
    END=$(date +%s.%N)
    
    DURATION=$(echo "$END - $START" | bc)
    echo "  Compilation time: ${DURATION}s"
done

rm -rf "$TEMP_DIR"

echo ""
echo "✅ Benchmark complete"
```

**Expected:**
- Small: <0.1s
- Medium: <0.5s
- Large: <2s

---

### Task 5.6: Integration Test (Full Pipeline) (30 min)

**Goal:** End-to-end pipeline test

**Test: tests/phase8/test_full_pipeline.sh**

```bash
#!/bin/bash
# Full Pipeline Integration Test

echo "🚀 Full Pipeline Integration Test"
echo "================================="
echo ""

TEMP_DIR="/tmp/melp_integration_$$"
mkdir -p "$TEMP_DIR"

# Create test program
cat > "$TEMP_DIR/full_test.mlp" << 'EOF'
function main()
  print("MELP Basic Runtime Test")
  print("\n")
  
  numeric x = 10
  numeric y = 32
  numeric sum = x + y
  
  print("Testing variables: x = ")
  print(x)
  print(", y = ")
  print(y)
  print("\n")
  
  print("Testing math: x + y = ")
  print(sum)
  print("\n")
  
  print("Testing UTF-8: Türkçe ğüşıöç")
  print("\n")
  
  print("✅ All features working!")
  print("\n")
  
  return 0
end_function
EOF

echo "Step 1: Compiling MELP → LLVM IR..."
if ! ./melp_compiler "$TEMP_DIR/full_test.mlp" -o "$TEMP_DIR/full_test.ll"; then
    echo "❌ Compilation failed"
    exit 1
fi
echo "✅ Compiled to IR"

echo ""
echo "Step 2: Assembling LLVM IR → Object..."
if ! llc "$TEMP_DIR/full_test.ll" -o "$TEMP_DIR/full_test.o"; then
    echo "❌ Assembly failed"
    exit 1
fi
echo "✅ Assembled to object"

echo ""
echo "Step 3: Linking with runtime..."
if ! clang "$TEMP_DIR/full_test.o" \
    -L./compiler/stage1/runtime/build \
    -lmelp_runtime \
    -o "$TEMP_DIR/full_test"; then
    echo "❌ Linking failed"
    exit 1
fi
echo "✅ Linked successfully"

echo ""
echo "Step 4: Running program..."
echo "--- OUTPUT START ---"
"$TEMP_DIR/full_test"
EXIT_CODE=$?
echo "--- OUTPUT END ---"
echo ""

if [ $EXIT_CODE -eq 0 ]; then
    echo "✅ Program executed successfully"
else
    echo "❌ Program exited with code $EXIT_CODE"
    exit 1
fi

# Cleanup
rm -rf "$TEMP_DIR"

echo ""
echo "✅ Full pipeline integration test PASSED"
```

---

## ✅ SUCCESS CRITERIA

**Phase 5 Complete When:**
- [ ] Test suite created (10 test cases)
- [ ] Test runner script works
- [ ] All tests pass (10/10)
- [ ] Valgrind clean (no leaks)
- [ ] LLVM IR validates (opt -verify)
- [ ] Performance acceptable (<2s for large programs)
- [ ] Integration test passes
- [ ] Documentation complete
- [ ] Known issues documented (if any)

---

## 🎯 FINAL APPROVAL

**When Phase 5 Complete:**
- [ ] Run full test suite: `./tests/phase8/run_all_tests.sh`
- [ ] Run valgrind: `./tests/phase8/test_memory_leaks.sh`
- [ ] Run IR validation: `./tests/phase8/validate_llvm_ir.sh`
- [ ] Run integration test: `./tests/phase8/test_full_pipeline.sh`
- [ ] Update ROADMAP.md (Phase 8 COMPLETE)
- [ ] Update CHANGELOG.md (Add Phase 8 entry)
- [ ] Create completion report (optional)

**Approval Checklist:**
- [ ] All tests pass
- [ ] No memory leaks
- [ ] No warnings
- [ ] Documentation updated
- [ ] Build system tested on clean checkout
- [ ] README updated with new capabilities

---

## 🎉 PROJECT COMPLETE!

**When All Phases Done:**

```bash
$ cat victory.mlp
function main()
  print("🎉 MELP Basic Runtime: COMPLETE!")
  print("\n")
  print("✅ String literals working")
  print("\n")
  print("✅ print() working")
  print("\n")
  print("✅ Stdlib integrated")
  print("\n")
  print("Stage 1 is now Minimum Viable Compiler!")
  return 0
end_function

$ ./melpc victory.mlp -o victory
$ ./victory
🎉 MELP Basic Runtime: COMPLETE!
✅ String literals working
✅ print() working
✅ Stdlib integrated
Stage 1 is now Minimum Viable Compiler!
```

---

## 📋 PHASE 5 CHECKLIST

**Testing:**
- [ ] 10 test cases created
- [ ] Test runner script works
- [ ] All tests pass
- [ ] Edge cases tested

**Validation:**
- [ ] Valgrind clean
- [ ] LLVM IR validates
- [ ] Performance benchmarked
- [ ] Integration test passes

**Documentation:**
- [ ] Test suite documented
- [ ] Known issues listed
- [ ] Usage examples added
- [ ] CHANGELOG updated

**Quality:**
- [ ] No warnings
- [ ] Clean build
- [ ] All scripts executable
- [ ] Code reviewed

---

**Ready? Start Task 5.1! 🚀**

**Target: MELP Minimum Viable Compiler! 🎯**
