#!/bin/bash
# Phase 0 Performance Benchmark
# Measures compilation time and memory usage

COMPILER="./compiler/stage0/modules/functions/functions_compiler"
TEST_DIR="tests/phase0"
TEMP_DIR="/tmp/melp_benchmark"
ITERATIONS=5

# Colors
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

mkdir -p "$TEMP_DIR"

echo "========================================"
echo "Phase 0 Performance Benchmark"
echo "========================================"
echo ""
echo "Compiler: $COMPILER"
echo "Iterations per test: $ITERATIONS"
echo ""

# Function to benchmark compilation
benchmark_compile() {
    local test_file=$1
    local test_name=$(basename "$test_file" .mlp)
    
    echo -n "Benchmarking: $test_name ... "
    
    local total_time=0
    local max_mem=0
    
    for i in $(seq 1 $ITERATIONS); do
        # Measure time and memory
        local start=$(date +%s%N)
        /usr/bin/time -f "%M" "$COMPILER" "$test_file" "$TEMP_DIR/$test_name.ll" --backend=llvm >/dev/null 2>/tmp/bench_mem.txt
        local end=$(date +%s%N)
        
        # Calculate time in milliseconds
        local elapsed=$(( (end - start) / 1000000 ))
        total_time=$((total_time + elapsed))
        
        # Get max memory (in KB)
        local mem=$(cat /tmp/bench_mem.txt | tail -1)
        if [ $mem -gt $max_mem ]; then
            max_mem=$mem
        fi
    done
    
    # Calculate average
    local avg_time=$((total_time / ITERATIONS))
    
    echo -e "${GREEN}Done${NC}"
    echo "  Average time: ${avg_time}ms"
    echo "  Max memory: ${max_mem}KB"
    echo ""
    
    # Return values for summary
    echo "$avg_time $max_mem"
}

# Benchmark all tests
echo "=== Compilation Benchmarks ==="
echo ""

result1=$(benchmark_compile "$TEST_DIR/test_variables_simple.mlp")
result2=$(benchmark_compile "$TEST_DIR/test_variables_arithmetic.mlp")
result3=$(benchmark_compile "$TEST_DIR/test_if_then.mlp")
result4=$(benchmark_compile "$TEST_DIR/test_comparison.mlp")
result5=$(benchmark_compile "$TEST_DIR/test_nested_if.mlp")
result6=$(benchmark_compile "$TEST_DIR/test_function_call.mlp")
result7=$(benchmark_compile "$TEST_DIR/test_arithmetic.mlp")
result8=$(benchmark_compile "$TEST_DIR/test_integration.mlp")

# Calculate totals
time1=$(echo $result1 | cut -d' ' -f1)
time2=$(echo $result2 | cut -d' ' -f1)
time3=$(echo $result3 | cut -d' ' -f1)
time4=$(echo $result4 | cut -d' ' -f1)
time5=$(echo $result5 | cut -d' ' -f1)
time6=$(echo $result6 | cut -d' ' -f1)
time7=$(echo $result7 | cut -d' ' -f1)
time8=$(echo $result8 | cut -d' ' -f1)

total_time=$((time1 + time2 + time3 + time4 + time5 + time6 + time7 + time8))
avg_total=$((total_time / 8))

mem1=$(echo $result1 | cut -d' ' -f2)
mem2=$(echo $result2 | cut -d' ' -f2)
mem3=$(echo $result3 | cut -d' ' -f2)
mem4=$(echo $result4 | cut -d' ' -f2)
mem5=$(echo $result5 | cut -d' ' -f2)
mem6=$(echo $result6 | cut -d' ' -f2)
mem7=$(echo $result7 | cut -d' ' -f2)
mem8=$(echo $result8 | cut -d' ' -f2)

# Find max memory
max_mem=$mem1
for m in $mem2 $mem3 $mem4 $mem5 $mem6 $mem7 $mem8; do
    if [ $m -gt $max_mem ]; then
        max_mem=$m
    fi
done

echo "========================================"
echo "Summary"
echo "========================================"
echo "Tests benchmarked: 8"
echo -e "Average compile time: ${BLUE}${avg_total}ms${NC}"
echo -e "Peak memory usage: ${BLUE}${max_mem}KB${NC} ($(($max_mem / 1024))MB)"
echo ""
echo "All benchmarks complete!"
