#!/bin/bash
# ============================================================================
# MELP Stage 1 - YZ_116 Integration Test Suite
# ============================================================================
# Purpose: Run all YZ_116 integration tests
# Date: 20 December 2025
# ============================================================================

echo "=============================================="
echo "MELP Stage 1 - YZ_116 Test Suite"
echo "=============================================="
echo ""

COMPILER="/home/pardus/projeler/MLP/MLP/compiler/stage0/modules/functions/functions_compiler"
TEST_DIR="/home/pardus/projeler/MLP/MLP/archive/stage1_api_attempt/modules/test"
RUNTIME_STDLIB="/home/pardus/projeler/MLP/MLP/runtime/stdlib"
RUNTIME_STO="/home/pardus/projeler/MLP/MLP/runtime/sto"
OUTPUT_DIR="/tmp"

TESTS=(
    "test_char_utils_direct:116:Lexer (char_utils)"
    "test_parser_integration:117:Parser (token processing)"
    "test_codegen_integration:118:Codegen (assembly gen)"
    "test_pipeline_e2e:116:E2E Pipeline"
)

PASSED=0
FAILED=0

for test_spec in "${TESTS[@]}"; do
    IFS=':' read -r test_name expected_code test_desc <<< "$test_spec"
    
    echo "----------------------------------------"
    echo "TEST: $test_desc"
    echo "File: $test_name.mlp"
    echo "----------------------------------------"
    
    # Compile .mlp -> .s
    if ! $COMPILER "$TEST_DIR/$test_name.mlp" "$OUTPUT_DIR/$test_name.s" > /dev/null 2>&1; then
        echo "✗ COMPILATION FAILED"
        FAILED=$((FAILED + 1))
        continue
    fi
    
    # Link .s -> binary
    if ! gcc -no-pie "$OUTPUT_DIR/$test_name.s" \
        -L"$RUNTIME_STDLIB" -L"$RUNTIME_STO" \
        -lmlp_stdlib -lsto_runtime \
        -o "$OUTPUT_DIR/$test_name" 2>/dev/null; then
        echo "✗ LINKING FAILED"
        FAILED=$((FAILED + 1))
        continue
    fi
    
    # Run binary
    "$OUTPUT_DIR/$test_name" > /dev/null 2>&1
    actual_code=$?
    
    if [ $actual_code -eq $expected_code ]; then
        echo "✓ PASSED (exit code: $actual_code)"
        PASSED=$((PASSED + 1))
    else
        echo "✗ FAILED (expected: $expected_code, got: $actual_code)"
        FAILED=$((FAILED + 1))
    fi
    
    echo ""
done

echo "=============================================="
echo "TEST SUMMARY"
echo "=============================================="
echo "Passed: $PASSED"
echo "Failed: $FAILED"
echo "Total:  $((PASSED + FAILED))"
echo ""

if [ $FAILED -eq 0 ]; then
    echo "🎉 ALL TESTS PASSED! YZ_116 COMPLETE!"
    exit 0
else
    echo "❌ SOME TESTS FAILED"
    exit 1
fi
