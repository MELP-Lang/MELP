#!/bin/bash

# LLVM vs Assembly Regression Test Script
# Author: YZ_03
# Date: 20 Aralık 2025

set -e

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

# Paths
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
COMPILER="$PROJECT_ROOT/compiler/stage0/modules/functions/functions_compiler"
STAGE0_DIR="$PROJECT_ROOT/compiler/stage0"

# Test directories
TEST_DIRS=(
    "$STAGE0_DIR"
    "$PROJECT_ROOT/tests/manual"
)

# Counters
TOTAL_TESTS=0
ASSEMBLY_SUCCESS=0
ASSEMBLY_FAILED=0
LLVM_SUCCESS=0
LLVM_FAILED=0
BOTH_SUCCESS=0
BOTH_FAILED=0
REGRESSION=0

# Results storage
declare -A ASM_RESULTS
declare -A LLVM_RESULTS

# Check compiler
if [ ! -f "$COMPILER" ]; then
    echo -e "${RED}❌ Compiler not found: $COMPILER${NC}"
    exit 1
fi

echo -e "${BLUE}═══════════════════════════════════════════════════${NC}"
echo -e "${BLUE}   MELP Regression Test: Assembly vs LLVM${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════${NC}"
echo ""

# Function to test a single file
test_file() {
    local mlp_file="$1"
    local test_name=$(basename "$mlp_file" .mlp)
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -ne "${CYAN}Testing:${NC} $test_name ... "
    
    # Test with Assembly backend (default)
    local asm_file="/tmp/${test_name}_asm.s"
    local asm_exe="/tmp/${test_name}_asm"
    local asm_result="FAIL"
    local asm_exitcode=999
    
    if "$COMPILER" "$mlp_file" "$asm_file" &>/dev/null; then
        if gcc "$asm_file" -o "$asm_exe" -L"$PROJECT_ROOT/runtime/stdlib" -lmlp_stdlib -L"$PROJECT_ROOT/runtime/sto" -lsto_runtime -lm &>/dev/null; then
            if "$asm_exe" &>/dev/null; then
                asm_exitcode=$?
                asm_result="PASS"
                ASSEMBLY_SUCCESS=$((ASSEMBLY_SUCCESS + 1))
            else
                asm_exitcode=$?
                ASSEMBLY_FAILED=$((ASSEMBLY_FAILED + 1))
            fi
        else
            ASSEMBLY_FAILED=$((ASSEMBLY_FAILED + 1))
        fi
    else
        ASSEMBLY_FAILED=$((ASSEMBLY_FAILED + 1))
    fi
    rm -f "$asm_file" "$asm_exe"
    
    # Test with LLVM backend
    local ll_file="/tmp/${test_name}_llvm.ll"
    local llvm_exe="/tmp/${test_name}_llvm"
    local llvm_result="FAIL"
    local llvm_exitcode=999
    
    if "$COMPILER" --backend=llvm "$mlp_file" "$ll_file" &>/dev/null; then
        if command -v clang &>/dev/null; then
            if clang "$ll_file" -o "$llvm_exe" &>/dev/null; then
                if "$llvm_exe" &>/dev/null; then
                    llvm_exitcode=$?
                    llvm_result="PASS"
                    LLVM_SUCCESS=$((LLVM_SUCCESS + 1))
                else
                    llvm_exitcode=$?
                    LLVM_FAILED=$((LLVM_FAILED + 1))
                fi
            else
                LLVM_FAILED=$((LLVM_FAILED + 1))
            fi
        else
            llvm_result="SKIP"
        fi
    else
        LLVM_FAILED=$((LLVM_FAILED + 1))
    fi
    rm -f "$ll_file" "$llvm_exe"
    
    # Store results
    ASM_RESULTS["$test_name"]="$asm_result:$asm_exitcode"
    LLVM_RESULTS["$test_name"]="$llvm_result:$llvm_exitcode"
    
    # Compare and report
    if [ "$asm_result" = "PASS" ] && [ "$llvm_result" = "PASS" ]; then
        if [ "$asm_exitcode" -eq "$llvm_exitcode" ]; then
            echo -e "${GREEN}✓ BOTH PASS${NC} (exit: $asm_exitcode)"
            BOTH_SUCCESS=$((BOTH_SUCCESS + 1))
        else
            echo -e "${YELLOW}⚠ MISMATCH${NC} (ASM: $asm_exitcode, LLVM: $llvm_exitcode)"
            REGRESSION=$((REGRESSION + 1))
        fi
    elif [ "$asm_result" = "PASS" ] && [ "$llvm_result" = "FAIL" ]; then
        echo -e "${RED}✗ REGRESSION${NC} (ASM works, LLVM fails)"
        REGRESSION=$((REGRESSION + 1))
    elif [ "$asm_result" = "FAIL" ] && [ "$llvm_result" = "PASS" ]; then
        echo -e "${YELLOW}⚡ IMPROVEMENT${NC} (ASM fails, LLVM works)"
    elif [ "$llvm_result" = "SKIP" ]; then
        if [ "$asm_result" = "PASS" ]; then
            echo -e "${BLUE}○ ASM ONLY${NC} (exit: $asm_exitcode)"
        else
            echo -e "${RED}✗ BOTH FAIL${NC}"
            BOTH_FAILED=$((BOTH_FAILED + 1))
        fi
    else
        echo -e "${RED}✗ BOTH FAIL${NC}"
        BOTH_FAILED=$((BOTH_FAILED + 1))
    fi
}

# Select basic tests from stage0
echo -e "${YELLOW}▶ Testing Stage 0 basic files...${NC}"
echo ""

cd "$STAGE0_DIR"
for test in test_basic.mlp test_func.mlp test_simple_main.mlp test_if_boolean.mlp test_while_simple.mlp test_for_simple.mlp; do
    if [ -f "$test" ]; then
        test_file "$STAGE0_DIR/$test"
    fi
done

# Test LLVM specific tests
echo ""
echo -e "${YELLOW}▶ Testing LLVM specific files...${NC}"
echo ""

for test in test_llvm_assign.mlp test_llvm_if.mlp test_llvm_while.mlp test_llvm_functions.mlp; do
    if [ -f "$STAGE0_DIR/$test" ]; then
        test_file "$STAGE0_DIR/$test"
    fi
done

# Print summary
echo ""
echo -e "${BLUE}═══════════════════════════════════════════════════${NC}"
echo -e "${BLUE}   Regression Test Summary${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════${NC}"
echo ""
echo -e "Total Tests:        ${BLUE}$TOTAL_TESTS${NC}"
echo ""
echo -e "${CYAN}Assembly Backend:${NC}"
echo -e "  Success:          ${GREEN}$ASSEMBLY_SUCCESS${NC}"
echo -e "  Failed:           ${RED}$ASSEMBLY_FAILED${NC}"
echo ""
echo -e "${CYAN}LLVM Backend:${NC}"
echo -e "  Success:          ${GREEN}$LLVM_SUCCESS${NC}"
echo -e "  Failed:           ${RED}$LLVM_FAILED${NC}"
echo ""
echo -e "${CYAN}Comparison:${NC}"
echo -e "  Both Success:     ${GREEN}$BOTH_SUCCESS${NC}"
echo -e "  Both Failed:      ${RED}$BOTH_FAILED${NC}"
echo -e "  Regressions:      ${RED}$REGRESSION${NC}"
echo ""

if [ $REGRESSION -eq 0 ] && [ $BOTH_SUCCESS -gt 0 ]; then
    echo -e "${GREEN}✅ No regressions detected!${NC}"
    exit 0
elif [ $REGRESSION -gt 0 ]; then
    echo -e "${RED}⚠️  $REGRESSION regression(s) detected!${NC}"
    exit 1
else
    echo -e "${YELLOW}⚠️  Tests completed with issues${NC}"
    exit 1
fi
