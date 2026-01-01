#!/bin/bash
# JSON Library Test Runner
# Compiles and runs all JSON tests

set -e  # Exit on error

echo "========================================="
echo "JSON Library Test Suite"
echo "========================================="
echo ""

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

TEST_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
RUNTIME_DIR="$TEST_DIR/../../MELP/runtime/json"

# Compile flags
CFLAGS="-Wall -Wextra -I$TEST_DIR/../.. -g"
SOURCES="$RUNTIME_DIR/parser.c $RUNTIME_DIR/stringify.c"

TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

run_test() {
    local test_name=$1
    local test_file=$2
    
    echo "----------------------------------------"
    echo "Running: $test_name"
    echo "----------------------------------------"
    
    # Compile
    if gcc $CFLAGS -o "$TEST_DIR/$test_name" "$test_file" $SOURCES 2>&1; then
        # Run
        if "$TEST_DIR/$test_name"; then
            echo -e "${GREEN}✓ $test_name PASSED${NC}"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}✗ $test_name FAILED (runtime error)${NC}"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    else
        echo -e "${RED}✗ $test_name FAILED (compile error)${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    echo ""
}

# Run all tests
run_test "test_parse_basic" "$TEST_DIR/test_parse_basic.c"
run_test "test_parse_metadata" "$TEST_DIR/test_parse_metadata.c"
run_test "test_stringify" "$TEST_DIR/test_stringify.c"
run_test "test_invalid" "$TEST_DIR/test_invalid.c"
run_test "test_large" "$TEST_DIR/test_large.c"

# Summary
echo "========================================="
echo "TEST SUMMARY"
echo "========================================="
echo "Total Tests:  $TOTAL_TESTS"
echo -e "Passed:       ${GREEN}$PASSED_TESTS${NC}"
echo -e "Failed:       ${RED}$FAILED_TESTS${NC}"
echo ""

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}✅ ALL TESTS PASSED! 🎉${NC}"
    echo ""
    echo "JSON Parser/Serializer is ready for:"
    echo "  ✓ Module metadata parsing (import system)"
    echo "  ✓ Configuration files"
    echo "  ✓ Data interchange"
    echo ""
    exit 0
else
    echo -e "${RED}❌ SOME TESTS FAILED${NC}"
    exit 1
fi
