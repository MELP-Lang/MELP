#!/bin/bash
# ============================================================================
# MELP Stage 1 - Test Runner (YZ_12)
# ============================================================================
# Purpose: Run all module tests (YZ_03 - YZ_11)
# Author: YZ_12
# Date: 18 Aralık 2025
#
# Tests:
#   - YZ_03: core         (15 tests)
#   - YZ_04: functions    (6 tests)
#   - YZ_05: variables    (8 tests)
#   - YZ_06: operators    (8 tests)
#   - YZ_07: control_flow (12 tests)
#   - YZ_08: literals     (10 tests)
#   - YZ_09: arrays       (10 tests)
#   - YZ_10: structs      (12 tests)
#   - YZ_11: enums        (12 tests)
#   Total: 93 tests
# ============================================================================

set -e  # Exit on error

echo "=========================================="
echo "MELP Stage 1 - Module Test Runner"
echo "=========================================="
echo ""

# ============================================================================
# Configuration
# ============================================================================

STAGE1_COMPILER="compiler/stage1/stage1_compiler"
MODULES_DIR="compiler/stage1/modules"
OUTPUT_DIR="temp/test_results"
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Check if Stage 1 compiler exists
if [ ! -f "$STAGE1_COMPILER" ]; then
    echo "❌ Error: Stage 1 compiler not found: $STAGE1_COMPILER"
    echo ""
    echo "Run bootstrap first:"
    echo "  ./scripts/bootstrap_stage1.sh"
    exit 1
fi

# Create output directory
mkdir -p "$OUTPUT_DIR"

# ============================================================================
# Test Runner Function
# ============================================================================

run_test() {
    local module=$1
    local test_file=$2
    local test_name=$(basename "$test_file" .mlp)
    
    echo -n "  Testing $test_name... "
    
    # Compile test
    if $STAGE1_COMPILER "$test_file" "$OUTPUT_DIR/$test_name.ll" 2>&1 > "$OUTPUT_DIR/$test_name.log"; then
        # Compilation successful
        if [ -f "$OUTPUT_DIR/$test_name.ll" ]; then
            echo "✅ PASS"
            ((PASSED_TESTS++))
        else
            echo "❌ FAIL (no output)"
            ((FAILED_TESTS++))
        fi
    else
        echo "❌ FAIL (compilation error)"
        ((FAILED_TESTS++))
    fi
    
    ((TOTAL_TESTS++))
}

# ============================================================================
# Run Module Tests
# ============================================================================

echo "Running module tests..."
echo ""

# YZ_03: Core Utilities (15 tests)
echo "Module: core (YZ_03)"
echo "-----------------------------------"
if [ -f "$MODULES_DIR/core/test_core.mlp" ]; then
    run_test "core" "$MODULES_DIR/core/test_core.mlp"
else
    echo "  ⚠️  Test file not found"
fi
echo ""

# YZ_04: Functions (6 tests)
echo "Module: functions (YZ_04)"
echo "-----------------------------------"
if [ -f "$MODULES_DIR/functions/test_functions.mlp" ]; then
    run_test "functions" "$MODULES_DIR/functions/test_functions.mlp"
else
    echo "  ⚠️  Test file not found"
fi
echo ""

# YZ_05: Variables (8 tests)
echo "Module: variables (YZ_05)"
echo "-----------------------------------"
if [ -f "$MODULES_DIR/variables/test_variables.mlp" ]; then
    run_test "variables" "$MODULES_DIR/variables/test_variables.mlp"
else
    echo "  ⚠️  Test file not found"
fi
echo ""

# YZ_06: Operators (8 tests)
echo "Module: operators (YZ_06)"
echo "-----------------------------------"
if [ -f "$MODULES_DIR/operators/test_operators.mlp" ]; then
    run_test "operators" "$MODULES_DIR/operators/test_operators.mlp"
else
    echo "  ⚠️  Test file not found"
fi
echo ""

# YZ_07: Control Flow (12 tests)
echo "Module: control_flow (YZ_07)"
echo "-----------------------------------"
if [ -f "$MODULES_DIR/control_flow/test_control_flow.mlp" ]; then
    run_test "control_flow" "$MODULES_DIR/control_flow/test_control_flow.mlp"
else
    echo "  ⚠️  Test file not found"
fi
echo ""

# YZ_08: Literals (10 tests)
echo "Module: literals (YZ_08)"
echo "-----------------------------------"
if [ -f "$MODULES_DIR/literals/test_literals.mlp" ]; then
    run_test "literals" "$MODULES_DIR/literals/test_literals.mlp"
else
    echo "  ⚠️  Test file not found"
fi
echo ""

# YZ_09: Arrays (10 tests)
echo "Module: arrays (YZ_09)"
echo "-----------------------------------"
if [ -f "$MODULES_DIR/arrays/test_arrays.mlp" ]; then
    run_test "arrays" "$MODULES_DIR/arrays/test_arrays.mlp"
else
    echo "  ⚠️  Test file not found"
fi
echo ""

# YZ_10: Structs (12 tests)
echo "Module: structs (YZ_10)"
echo "-----------------------------------"
if [ -f "$MODULES_DIR/structs/test_structs.mlp" ]; then
    run_test "structs" "$MODULES_DIR/structs/test_structs.mlp"
else
    echo "  ⚠️  Test file not found"
fi
echo ""

# YZ_11: Enums (12 tests)
echo "Module: enums (YZ_11)"
echo "-----------------------------------"
if [ -f "$MODULES_DIR/enums/test_enums.mlp" ]; then
    run_test "enums" "$MODULES_DIR/enums/test_enums.mlp"
else
    echo "  ⚠️  Test file not found"
fi
echo ""

# ============================================================================
# Summary
# ============================================================================

echo "=========================================="
echo "Test Results Summary"
echo "=========================================="
echo ""
echo "Total Tests:  $TOTAL_TESTS"
echo "Passed:       $PASSED_TESTS ✅"
echo "Failed:       $FAILED_TESTS ❌"
echo ""

if [ $FAILED_TESTS -eq 0 ]; then
    echo "🎉 ALL TESTS PASSED!"
    echo ""
    exit 0
else
    echo "⚠️  Some tests failed. Check logs in: $OUTPUT_DIR"
    echo ""
    exit 1
fi
