#!/bin/bash
# ============================================
# MELP Time Library - Test Runner
# ============================================
# Runs all time library tests and reports results

echo "============================================"
echo "MELP TIME LIBRARY - TEST SUITE"
echo "============================================"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

PASSED=0
FAILED=0
TOTAL=0

# Compile and run a test
run_test() {
    local test_name=$1
    local test_file=$2
    
    echo "Running: $test_name"
    echo "----------------------------------------"
    
    # Compile
    gcc -o "$test_file.out" "$test_file" ../../MELP/runtime/time/time.c -I../../MELP/runtime/time
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}❌ COMPILATION FAILED${NC}"
        echo ""
        FAILED=$((FAILED + 1))
        TOTAL=$((TOTAL + 1))
        return 1
    fi
    
    # Run
    ./"$test_file.out"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✅ TEST PASSED${NC}"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}❌ TEST FAILED${NC}"
        FAILED=$((FAILED + 1))
    fi
    
    TOTAL=$((TOTAL + 1))
    echo ""
    
    # Clean up
    rm -f "$test_file.out"
}

# Run all tests
run_test "Test 1: Time Now" "test_now.c"
run_test "Test 2: Sleep Operations" "test_sleep.c"
run_test "Test 3: Datetime UTC" "test_datetime_utc.c"
run_test "Test 4: Datetime Local" "test_datetime_local.c"
run_test "Test 5: Date Formatting" "test_format.c"
run_test "Test 6: Date Parsing" "test_parse.c"
run_test "Test 7: Timezone" "test_timezone.c"
run_test "Test 8: Round-trip" "test_roundtrip.c"
run_test "Test 9: Edge Cases" "test_edge_cases.c"
run_test "Test 10: Performance" "test_performance.c"

# Summary
echo "============================================"
echo "TEST SUMMARY"
echo "============================================"
echo "Total Tests: $TOTAL"
echo -e "Passed: ${GREEN}$PASSED${NC}"
echo -e "Failed: ${RED}$FAILED${NC}"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✅ ALL TESTS PASSED!${NC}"
    exit 0
else
    echo -e "${RED}❌ SOME TESTS FAILED${NC}"
    exit 1
fi
