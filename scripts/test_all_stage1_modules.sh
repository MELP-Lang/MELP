#!/bin/bash
# YZ_22: Systematic Error Analysis - Test Harness
# Tests all Stage 1 modules and logs errors for pattern analysis

set -e

STAGE0_COMPILER="/home/pardus/projeler/MLP/MLP/compiler/stage0/modules/functions/functions_compiler"
STAGE1_MODULES="/home/pardus/projeler/MLP/MLP/compiler/stage1/modules"
LOG_DIR="/home/pardus/projeler/MLP/MLP/logs/stage1_errors"
SUMMARY_LOG="$LOG_DIR/summary.txt"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "🔬 YZ_22: Systematic Error Analysis"
echo "===================================="
echo ""

# Check if Stage 0 compiler exists
if [ ! -f "$STAGE0_COMPILER" ]; then
    echo -e "${RED}❌ Stage 0 compiler not found: $STAGE0_COMPILER${NC}"
    echo "Please build Stage 0 first."
    exit 1
fi

# Clear previous logs
rm -f "$LOG_DIR"/*.log
rm -f "$SUMMARY_LOG"

# Initialize counters
total=0
success=0
failed=0

echo "Testing Stage 1 modules..."
echo ""

# Find all .mlp files
while IFS= read -r module; do
    total=$((total + 1))
    module_name=$(basename "$module" .mlp)
    module_rel=$(realpath --relative-to="$STAGE1_MODULES" "$module")
    log_file="$LOG_DIR/${module_name}.log"
    
    echo -n "[$total] Testing: $module_rel ... "
    
    # Test compilation
    if "$STAGE0_COMPILER" -c "$module" /tmp/test_${module_name}.s > "$log_file" 2>&1; then
        echo -e "${GREEN}✓ PASS${NC}"
        echo "PASS: $module_rel" >> "$SUMMARY_LOG"
        success=$((success + 1))
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo "FAIL: $module_rel" >> "$SUMMARY_LOG"
        failed=$((failed + 1))
        
        # Extract first error for quick view
        first_error=$(grep -i "error" "$log_file" | head -1 || echo "Unknown error")
        echo "      → $first_error"
    fi
    
done < <(find "$STAGE1_MODULES" -name "*.mlp" -type f | sort)

echo ""
echo "===================================="
echo "📊 RESULTS:"
echo "   Total:   $total modules"
echo -e "   ${GREEN}Success: $success ($((success * 100 / total))%)${NC}"
echo -e "   ${RED}Failed:  $failed ($((failed * 100 / total))%)${NC}"
echo ""
echo "Logs saved to: $LOG_DIR"
echo "Summary: $SUMMARY_LOG"
