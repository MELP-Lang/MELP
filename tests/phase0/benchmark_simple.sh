#!/bin/bash
# Phase 0 Performance Benchmark - Simple Version
# Measures compilation time for Phase 0 tests

COMPILER="./compiler/stage0/modules/functions/functions_compiler"
TEST_DIR="tests/phase0"

echo "========================================"
echo "Phase 0 Performance Benchmark"
echo "========================================"
echo ""

# Benchmark function
benchmark() {
    local file=$1
    local name=$(basename "$file" .mlp)
    echo -n "Compiling $name ... "
    
    local start=$(date +%s%N)
    $COMPILER "$file" "/tmp/bench_$name.ll" --backend=llvm >/dev/null 2>&1
    local end=$(date +%s%N)
    
    local elapsed=$(( (end - start) / 1000000 ))
    echo "${elapsed}ms"
    
    echo $elapsed
}

# Run benchmarks
t1=$(benchmark "$TEST_DIR/test_variables_simple.mlp")
t2=$(benchmark "$TEST_DIR/test_variables_arithmetic.mlp")
t3=$(benchmark "$TEST_DIR/test_if_then.mlp")
t4=$(benchmark "$TEST_DIR/test_comparison.mlp")
t5=$(benchmark "$TEST_DIR/test_nested_if.mlp")
t6=$(benchmark "$TEST_DIR/test_function_call.mlp")
t7=$(benchmark "$TEST_DIR/test_arithmetic.mlp")
t8=$(benchmark "$TEST_DIR/test_integration.mlp")

# Calculate average
total=$((t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8))
avg=$((total / 8))

echo ""
echo "========================================"
echo "Summary"
echo "========================================"
echo "Tests: 8"
echo "Total time: ${total}ms"
echo "Average time: ${avg}ms per file"
echo ""

# Memory usage (single measurement)
echo "Memory usage test:"
/usr/bin/time -v $COMPILER "$TEST_DIR/test_integration.mlp" "/tmp/bench_mem.ll" --backend=llvm >/dev/null 2>&1 | grep "Maximum resident"
