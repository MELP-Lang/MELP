#!/bin/bash

# LLVM vs Assembly Quick Regression Test
# Author: YZ_03
# Date: 20 Aralık 2025

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Paths
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
COMPILER="$PROJECT_ROOT/compiler/stage0/modules/functions/functions_compiler"

# Counters
TOTAL=0
ASM_OK=0
LLVM_OK=0
BOTH_OK=0
REGRESSION=0

echo -e "${BLUE}═══════════════════════════════════════════════════${NC}"
echo -e "${BLUE}   MELP Quick Regression Test${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════${NC}"
echo ""

# Test a simple file
test_simple() {
    local name="$1"
    local content="$2"
    
    TOTAL=$((TOTAL + 1))
    echo -ne "Test $name ... "
    
    # Create test file
    echo "$content" > "/tmp/test_${name}.mlp"
    
    # Test Assembly
    local asm_ok=0
    if "$COMPILER" "/tmp/test_${name}.mlp" "/tmp/test_${name}.s" &>/dev/null; then
        asm_ok=1
        ASM_OK=$((ASM_OK + 1))
    fi
    
    # Test LLVM
    local llvm_ok=0
    if "$COMPILER" --backend=llvm "/tmp/test_${name}.mlp" "/tmp/test_${name}.ll" &>/dev/null; then
        llvm_ok=1
        LLVM_OK=$((LLVM_OK + 1))
    fi
    
    # Compare
    if [ $asm_ok -eq 1 ] && [ $llvm_ok -eq 1 ]; then
        echo -e "${GREEN}✓ BOTH${NC}"
        BOTH_OK=$((BOTH_OK + 1))
    elif [ $asm_ok -eq 1 ] && [ $llvm_ok -eq 0 ]; then
        echo -e "${RED}✗ REGRESSION${NC}"
        REGRESSION=$((REGRESSION + 1))
    elif [ $asm_ok -eq 0 ] && [ $llvm_ok -eq 1 ]; then
        echo -e "${YELLOW}⚡ IMPROVEMENT${NC}"
    else
        echo -e "${RED}✗ BOTH FAIL${NC}"
    fi
    
    # Cleanup
    rm -f "/tmp/test_${name}".{mlp,s,ll}
}

# Simple tests
test_simple "return" "function main() returns numeric
    return 42
end function"

test_simple "arithmetic" "function main() returns numeric
    return 10 + 20
end function"

test_simple "variable" "function main() returns numeric
    numeric x = 88
    return x
end function"

test_simple "if_stmt" "function main() returns numeric
    numeric x = 10
    if (x > 5)
        return 99
    end if
    return 0
end function"

test_simple "while_loop" "function main() returns numeric
    numeric i = 0
    numeric sum = 0
    while (i < 5)
        sum = sum + 2
        i = i + 1
    end while
    return sum
end function"

test_simple "function_call" "function helper() returns numeric
    return 15
end function

function main() returns numeric
    return helper()
end function"

# Summary
echo ""
echo -e "${BLUE}═══════════════════════════════════════════════════${NC}"
echo -e "Total:          $TOTAL"
echo -e "ASM Success:    ${GREEN}$ASM_OK${NC}"
echo -e "LLVM Success:   ${GREEN}$LLVM_OK${NC}"
echo -e "Both Success:   ${GREEN}$BOTH_OK${NC}"
echo -e "Regressions:    ${RED}$REGRESSION${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════${NC}"

if [ $REGRESSION -eq 0 ]; then
    echo -e "${GREEN}✅ No regressions!${NC}"
    exit 0
else
    echo -e "${RED}⚠️  $REGRESSION regression(s) detected${NC}"
    exit 1
fi
