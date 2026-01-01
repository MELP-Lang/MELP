#!/bin/bash
# Test suite for mlp-errors

set -e

echo "🧪 MELP Error Message System - Test Suite"
echo "=========================================="
echo ""

# Test counter
TESTS_PASSED=0
TESTS_FAILED=0

# Helper function
run_test() {
    local test_name="$1"
    local code="$2"
    local file="$3"
    local line="$4"
    local col="$5"
    shift 5
    local args="$@"
    
    echo "Test: $test_name"
    if ../../build/mlp-errors "$code" "$file" "$line" "$col" $args > /dev/null 2>&1; then
        echo "✅ PASS"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo "❌ FAIL"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
    echo ""
}

# Create test file
cat > test_errors.mlp << 'EOF'
let x = 42
print(y)
func add(a, b) { return a + b }
EOF

echo "Phase 1: Basic Error Tests"
echo "----------------------------"

run_test "E0001 - Undefined Variable (TR)" \
    E0001 test_errors.mlp 2 7 y

run_test "E0001 - Undefined Variable (EN)" \
    E0001 test_errors.mlp 2 7 myVar

run_test "E0002 - Type Mismatch" \
    E0002 test_errors.mlp 1 10 int string

run_test "E0003 - Syntax Error" \
    E0003 test_errors.mlp 1 15 ";"

run_test "E0004 - Function Not Found" \
    E0004 test_errors.mlp 3 5 calculate

run_test "E0005 - Wrong Number of Arguments" \
    E0005 test_errors.mlp 3 10 add 2 3

echo ""
echo "Phase 2: Warning Tests"
echo "----------------------"

run_test "W0001 - Unused Variable" \
    W0001 test_errors.mlp 1 5 x

run_test "W0002 - Unused Import" \
    W0002 test_errors.mlp 1 1 math

run_test "W0003 - Unused Function" \
    W0003 test_errors.mlp 3 6 helper

run_test "W0004 - Shadowing" \
    W0004 test_errors.mlp 2 9 x

echo ""
echo "Phase 3: Type System Tests"
echo "--------------------------"

run_test "E0100 - Invalid Type Conversion" \
    E0100 test_errors.mlp 1 10 string int

run_test "E0101 - Type Inference Failed" \
    E0101 test_errors.mlp 1 5 value

run_test "E0102 - Invalid Operation" \
    E0102 test_errors.mlp 1 10 "/" string

run_test "E0103 - Incompatible Types" \
    E0103 test_errors.mlp 1 10 "+" int string

echo ""
echo "Phase 4: Module System Tests"
echo "----------------------------"

run_test "E0200 - Module Not Found" \
    E0200 test_errors.mlp 1 1 unknown_module

run_test "E0201 - Name Collision" \
    E0201 test_errors.mlp 2 1 log

run_test "E0202 - Private Access" \
    E0202 test_errors.mlp 5 10 _private

run_test "E0204 - Circular Import" \
    E0204 test_errors.mlp 1 1 "a.mlp→b.mlp→a.mlp"

echo ""
echo "Phase 5: Control Flow Tests"
echo "---------------------------"

run_test "E0400 - Unreachable Code" \
    E0400 test_errors.mlp 10 5

run_test "E0401 - Break Misuse" \
    E0401 test_errors.mlp 15 5

run_test "E0402 - Continue Misuse" \
    E0402 test_errors.mlp 16 5

run_test "E0404 - If Condition Error" \
    E0404 test_errors.mlp 20 8 int

echo ""
echo "Phase 6: Language Tests"
echo "----------------------"

echo "Testing Turkish (default):"
../../build/mlp-errors E0001 test_errors.mlp 2 7 y | head -1
echo ""

echo "Testing English:"
MLP_LANG=en ../../build/mlp-errors E0001 test_errors.mlp 2 7 y | head -1
echo ""

echo ""
echo "=========================================="
echo "Test Results:"
echo "  Passed: $TESTS_PASSED"
echo "  Failed: $TESTS_FAILED"
echo "=========================================="

if [ $TESTS_FAILED -eq 0 ]; then
    echo "✅ All tests passed!"
    exit 0
else
    echo "❌ Some tests failed!"
    exit 1
fi
