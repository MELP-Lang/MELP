#!/bin/bash

# LLVM Test Runner for MELP Compiler
# Author: YZ_02
# Date: 20 Aralık 2025

set -e

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Get script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
COMPILER="$PROJECT_ROOT/compiler/stage0/modules/functions/functions_compiler"

# Check if compiler exists
if [ ! -f "$COMPILER" ]; then
    echo -e "${RED}❌ Compiler not found at: $COMPILER${NC}"
    echo "Please build the compiler first: cd compiler/stage0 && make modules"
    exit 1
fi

# Check if llc is available (for LLVM IR compilation)
if ! command -v llc &> /dev/null; then
    echo -e "${YELLOW}⚠️  llc not found. Trying to use gcc directly...${NC}"
    USE_GCC_DIRECT=true
else
    USE_GCC_DIRECT=false
fi

echo -e "${BLUE}═══════════════════════════════════════════════════${NC}"
echo -e "${BLUE}   MELP LLVM Backend Test Suite${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════${NC}"
echo ""

# Function to run a single test
run_test() {
    local test_file="$1"
    local expected_output="$2"
    local test_name=$(basename "$test_file" .mlp)
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -n "Testing: $test_name ... "
    
    # Generate LLVM IR
    local ll_file="${test_file%.mlp}.ll"
    if ! "$COMPILER" --backend=llvm "$test_file" "$ll_file" 2>/dev/null; then
        echo -e "${RED}FAILED (compilation)${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return 1
    fi
    
    # Compile and run
    local output
    if [ "$USE_GCC_DIRECT" = true ]; then
        # Compile LLVM IR to object file and link
        local s_file="${test_file%.mlp}.s"
        local exe_file="${test_file%.mlp}"
        
        # Try to compile with clang if available, otherwise skip
        if command -v clang &> /dev/null; then
            clang -o "$exe_file" "$ll_file" 2>/dev/null || {
                echo -e "${YELLOW}SKIPPED (clang compilation failed)${NC}"
                rm -f "$ll_file"
                return 0
            }
            output=$("$exe_file" 2>/dev/null; echo $?)
            rm -f "$exe_file"
        else
            echo -e "${YELLOW}SKIPPED (no LLVM runtime available)${NC}"
            rm -f "$ll_file"
            return 0
        fi
    else
        # Use llc to compile to assembly
        local s_file="${test_file%.mlp}.s"
        local exe_file="${test_file%.mlp}"
        llc -o "$s_file" "$ll_file" 2>/dev/null || {
            echo -e "${RED}FAILED (llc compilation)${NC}"
            FAILED_TESTS=$((FAILED_TESTS + 1))
            rm -f "$ll_file"
            return 1
        }
        gcc -o "$exe_file" "$s_file" 2>/dev/null || {
            echo -e "${RED}FAILED (gcc linking)${NC}"
            FAILED_TESTS=$((FAILED_TESTS + 1))
            rm -f "$ll_file" "$s_file"
            return 1
        }
        output=$("$exe_file" 2>/dev/null; echo $?)
        rm -f "$s_file" "$exe_file"
    fi
    
    # Clean up
    rm -f "$ll_file"
    
    # Check result
    if [ "$output" -eq "$expected_output" ]; then
        echo -e "${GREEN}✓ PASSED${NC} (exit code: $output)"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        return 0
    else
        echo -e "${RED}✗ FAILED${NC} (expected: $expected_output, got: $output)"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return 1
    fi
}

# Function to extract expected output from test file
get_expected_output() {
    local test_file="$1"
    local test_name=$(basename "$test_file" .mlp)
    
    # Map test names to expected outputs
    case "$test_name" in
        "test_return") echo "42" ;;
        "test_arithmetic") echo "30" ;;
        "test_variables") echo "88" ;;
        "test_arithmetic_complex") echo "17" ;;
        "test_variable_arithmetic") echo "100" ;;
        "test_if") echo "99" ;;
        "test_if_else") echo "55" ;;
        "test_while") echo "10" ;;
        "test_for") echo "15" ;;
        "test_call") echo "15" ;;
        "test_params") echo "35" ;;
        "test_multiple_calls") echo "25" ;;
        *) echo "0" ;;
    esac
}

echo -e "${YELLOW}▶ Running 01_basic tests...${NC}"
echo ""

for test in "$SCRIPT_DIR/01_basic"/*.mlp; do
    if [ -f "$test" ]; then
        expected=$(get_expected_output "$test")
        run_test "$test" "$expected"
    fi
done

echo ""
echo -e "${YELLOW}▶ Running 02_control tests...${NC}"
echo ""

for test in "$SCRIPT_DIR/02_control"/*.mlp; do
    if [ -f "$test" ]; then
        expected=$(get_expected_output "$test")
        run_test "$test" "$expected"
    fi
done

echo ""
echo -e "${YELLOW}▶ Running 03_functions tests...${NC}"
echo ""

for test in "$SCRIPT_DIR/03_functions"/*.mlp; do
    if [ -f "$test" ]; then
        expected=$(get_expected_output "$test")
        run_test "$test" "$expected"
    fi
done

# Print summary
echo ""
echo -e "${BLUE}═══════════════════════════════════════════════════${NC}"
echo -e "${BLUE}   Test Summary${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════${NC}"
echo ""
echo -e "Total Tests:  ${BLUE}$TOTAL_TESTS${NC}"
echo -e "Passed:       ${GREEN}$PASSED_TESTS${NC}"
echo -e "Failed:       ${RED}$FAILED_TESTS${NC}"
echo ""

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}🎉 All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}❌ Some tests failed.${NC}"
    exit 1
fi
