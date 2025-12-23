#!/bin/bash
# Phase 1, Task 1.1 Test Suite - String Type & Concatenation

COMPILER="./compiler/stage0/modules/functions/functions_compiler"
TOTAL=0
PASSED=0
FAILED=0

echo "========================================"
echo "Phase 1, Task 1.1 Test Suite"
echo "String Type & Concatenation"
echo "========================================"
echo ""

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

run_test() {
    local test_file=$1
    local expected_exit=$2
    local test_name=$3
    
    TOTAL=$((TOTAL + 1))
    
    echo -n "Test $TOTAL: $test_name ... "
    
    # Compile
    $COMPILER "$test_file" /tmp/test_$TOTAL.ll > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "${RED}FAIL${NC} (compilation error)"
        FAILED=$((FAILED + 1))
        return
    fi
    
    # Compile LLVM IR to executable (instead of using lli)
    clang /tmp/test_$TOTAL.ll -L./runtime/stdlib -lmlp_stdlib -o /tmp/test_$TOTAL.bin > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "${RED}FAIL${NC} (linking error)"
        FAILED=$((FAILED + 1))
        return
    fi
    
    # Run
    /tmp/test_$TOTAL.bin > /dev/null 2>&1
    actual_exit=$?
    
    if [ $actual_exit -eq $expected_exit ]; then
        echo -e "${GREEN}PASS${NC} (exit: $actual_exit)"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}FAIL${NC} (expected: $expected_exit, got: $actual_exit)"
        FAILED=$((FAILED + 1))
    fi
}

# Test 1: String variable declaration
run_test "tests/phase1/test_string_variable.mlp" 5 "String variable declaration"

# Test 2: String concatenation with variables
run_test "tests/phase1/test_string_concat_vars.mlp" 0 "String concatenation (vars)"

# Test 3: String comparison (==)
run_test "tests/phase1/test_string_comparison_eq.mlp" 1 "String comparison (==)"

# Test 4: String comparison (!=)
run_test "tests/phase1/test_string_comparison_neq.mlp" 1 "String comparison (!=)"

# Test 5: String comparison (<)
run_test "tests/phase1/test_string_comparison_lt.mlp" 1 "String comparison (<)"

# Test 6: String comparison (>)
run_test "tests/phase1/test_string_comparison_gt.mlp" 1 "String comparison (>)"

# Test 7: String comparison (<=)
run_test "tests/phase1/test_string_comparison_lte.mlp" 1 "String comparison (<=)"

# Test 8: String comparison (>=)
run_test "tests/phase1/test_string_comparison_gte.mlp" 1 "String comparison (>=)"

echo ""
echo "========================================"
echo "Results: $PASSED/$TOTAL tests passed"
if [ $FAILED -gt 0 ]; then
    echo -e "${RED}$FAILED tests failed${NC}"
    exit 1
else
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
fi
