#!/bin/bash
# Comprehensive Test Suite for Phase 12 STO Refactoring
# YZ_52 - Part 4: Integration & Testing

set -e

COMPILER="./modules/functions/functions_compiler"
RUNTIME_LIB="-L../../runtime/stdlib -lmlp_stdlib -L../../runtime/sto -lsto_runtime -lm"
PASSED=0
FAILED=0
TOTAL=0

echo "╔════════════════════════════════════════════════════════╗"
echo "║  MELP Phase 12 - Comprehensive Test Suite (STO)       ║"
echo "╚════════════════════════════════════════════════════════╝"
echo ""

# Color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

run_test() {
    local test_file=$1
    local expected_exit=${2:-0}
    local description=$3
    
    TOTAL=$((TOTAL + 1))
    
    echo -n "Testing: $test_file ... "
    
    # Compile and link from modules/functions directory (where compiler expects to run)
    # Pass relative paths from there
    if ! (cd modules/functions && ./functions_compiler "../../$test_file" "../../${test_file%.mlp}") &>/dev/null; then
        echo -e "${RED}FAIL${NC} (compilation/linking error)"
        FAILED=$((FAILED + 1))
        return 1
    fi
    
    # Execute with LD_LIBRARY_PATH
    LD_LIBRARY_PATH="../../runtime/stdlib:../../runtime/sto" ./"${test_file%.mlp}" &>/dev/null
    local exit_code=$?
    
    # Check result
    if [ $exit_code -eq $expected_exit ]; then
        echo -e "${GREEN}PASS${NC} (exit: $exit_code)"
        PASSED=$((PASSED + 1))
        # Clean up
        rm -f "${test_file%.mlp}.s" "${test_file%.mlp}"
        return 0
    else
        echo -e "${RED}FAIL${NC} (expected: $expected_exit, got: $exit_code)"
        FAILED=$((FAILED + 1))
        return 1
    fi
}

echo "═══════════════════════════════════════════════════════════"
echo "1. BASIC SYNTAX TESTS"
echo "═══════════════════════════════════════════════════════════"

run_test "test_basic.mlp" 42 "Basic return value"
run_test "test_all_syntax.mlp" 0 "All syntax features"

echo ""
echo "═══════════════════════════════════════════════════════════"
echo "2. ARITHMETIC & EXPRESSIONS"
echo "═══════════════════════════════════════════════════════════"

run_test "test_sto_final.mlp" 150 "STO arithmetic (100+50)"

echo ""
echo "═══════════════════════════════════════════════════════════"
echo "3. CONTROL FLOW - IF/ELSE"
echo "═══════════════════════════════════════════════════════════"

run_test "test_if_boolean_false.mlp" 10 "If-else false branch"
run_test "test_if_boolean_true.mlp" 20 "If-else true branch"
run_test "test_if_multiline.mlp" 0 "Multi-line if statements"
run_test "test_if_nested.mlp" 0 "Nested if statements"
run_test "test_if_simple.mlp" 0 "Simple if statement"

echo ""
echo "═══════════════════════════════════════════════════════════"
echo "4. CONTROL FLOW - FOR LOOPS"
echo "═══════════════════════════════════════════════════════════"

run_test "test_for_simple.mlp" 5 "Simple for loop (1 to 5)"
run_test "test_for_simpler.mlp" 6 "Simpler for loop counter"
run_test "test_for_count.mlp" 10 "For loop count"
run_test "test_for_downto.mlp" 0 "For loop downto"

echo ""
echo "═══════════════════════════════════════════════════════════"
echo "5. BOOLEAN OPERATIONS"
echo "═══════════════════════════════════════════════════════════"

run_test "test_boolean_and.mlp" 0 "Boolean AND operation"
run_test "test_boolean_and_true.mlp" 0 "Boolean AND true"
run_test "test_boolean_or.mlp" 0 "Boolean OR operation"
run_test "test_boolean_not.mlp" 0 "Boolean NOT operation"

echo ""
echo "═══════════════════════════════════════════════════════════"
echo "6. ARRAY OPERATIONS"
echo "═══════════════════════════════════════════════════════════"

run_test "test_array_syntax.mlp" 0 "Array syntax"
run_test "test_array_expr_assign.mlp" 0 "Array expression assignment"
run_test "test_array_expr_index.mlp" 0 "Array expression indexing"
run_test "test_bounds_ok.mlp" 0 "Array bounds check OK"
run_test "test_expr_idx_read.mlp" 0 "Expression index read"
run_test "test_expr_idx_write.mlp" 0 "Expression index write"
run_test "test_expr_idx_full.mlp" 0 "Expression index full"

echo ""
echo "═══════════════════════════════════════════════════════════"
echo "7. COLLECTION TESTS"
echo "═══════════════════════════════════════════════════════════"

run_test "test_all_collections.mlp" 0 "All collection types"
run_test "test_list_syntax.mlp" 0 "List syntax"
run_test "test_tuple_syntax.mlp" 0 "Tuple syntax"

echo ""
echo "═══════════════════════════════════════════════════════════"
echo "8. STRING OPERATIONS"
echo "═══════════════════════════════════════════════════════════"

run_test "test_string_assign.mlp" 0 "String assignment"
run_test "test_string_concat.mlp" 0 "String concatenation"
run_test "test_string_literal.mlp" 0 "String literals"

echo ""
echo "═══════════════════════════════════════════════════════════"
echo "9. PRINTLN & OUTPUT"
echo "═══════════════════════════════════════════════════════════"

run_test "test_basic_println.mlp" 0 "Basic println"
run_test "test_println_basic.mlp" 0 "Println basic"
run_test "test_println_boolean.mlp" 0 "Println boolean"
run_test "test_println_simple.mlp" 0 "Println simple"
run_test "test_println_string.mlp" 0 "Println string"

echo ""
echo "═══════════════════════════════════════════════════════════"
echo "10. MODULE SYSTEM"
echo "═══════════════════════════════════════════════════════════"

if [ -f "test_module_import.mlp" ]; then
    run_test "test_module_import.mlp" 0 "Module import"
fi

if [ -f "test_module_export.mlp" ]; then
    run_test "test_module_export.mlp" 0 "Module export"
fi

echo ""
echo "═══════════════════════════════════════════════════════════"
echo "11. STATE MANAGEMENT"
echo "═══════════════════════════════════════════════════════════"

if [ -f "test_state_basic.mlp" ]; then
    run_test "test_state_basic.mlp" 0 "Basic state management"
fi

if [ -f "test_state_counter.mlp" ]; then
    run_test "test_state_counter.mlp" 0 "State counter"
fi

echo ""
echo "╔════════════════════════════════════════════════════════╗"
echo "║  TEST SUMMARY                                          ║"
echo "╚════════════════════════════════════════════════════════╝"
echo ""
echo "Total Tests: $TOTAL"
echo -e "${GREEN}Passed:${NC} $PASSED"
echo -e "${RED}Failed:${NC} $FAILED"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✅ ALL TESTS PASSED!${NC}"
    echo "STO refactoring successful - no regressions detected!"
    exit 0
else
    echo -e "${RED}❌ SOME TESTS FAILED!${NC}"
    echo "Review failures and check for regressions."
    exit 1
fi
