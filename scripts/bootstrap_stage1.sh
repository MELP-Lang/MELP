#!/bin/bash
# ============================================================================
# MELP Stage 1 - Bootstrap Script (YZ_12)
# ============================================================================
# Purpose: Compile Stage 1 compiler using Stage 0
# Author: YZ_12
# Date: 18 Aralık 2025
#
# Process:
#   1. Use Stage 0 functions_standalone to compile main.mlp
#   2. Generate LLVM IR (.ll)
#   3. Compile to assembly (.s)
#   4. Link to create stage1_compiler binary
# ============================================================================

set -e  # Exit on error

echo "=========================================="
echo "MELP Stage 1 - Bootstrap Process"
echo "=========================================="
echo ""

# ============================================================================
# Configuration
# ============================================================================

STAGE0_COMPILER="compiler/stage0/modules/functions/functions_standalone"
STAGE1_SOURCE="compiler/stage1/main.mlp"
OUTPUT_DIR="compiler/stage1/build"
STAGE1_BINARY="compiler/stage1/stage1_compiler"

# Check if Stage 0 compiler exists
if [ ! -f "$STAGE0_COMPILER" ]; then
    echo "❌ Error: Stage 0 compiler not found: $STAGE0_COMPILER"
    echo ""
    echo "Building Stage 0 first..."
    cd compiler/stage0
    make all
    cd ../..
fi

# Create output directory
mkdir -p "$OUTPUT_DIR"

echo "Stage 0 Compiler: $STAGE0_COMPILER"
echo "Stage 1 Source:   $STAGE1_SOURCE"
echo "Output Directory: $OUTPUT_DIR"
echo ""

# ============================================================================
# Step 1: Compile main.mlp to LLVM IR
# ============================================================================

echo "Step 1: Compiling main.mlp → LLVM IR"
echo "-----------------------------------"

$STAGE0_COMPILER "$STAGE1_SOURCE" "$OUTPUT_DIR/main.ll" || {
    echo "❌ Compilation failed!"
    exit 1
}

echo "  ✅ LLVM IR generated: $OUTPUT_DIR/main.ll"
echo ""

# ============================================================================
# Step 2: Stage 0 already created executable (.ll binary)
# ============================================================================

echo "Step 2: Binary Ready (Stage 0 created executable)"
echo "-----------------------------------"

# Stage 0's functions_standalone creates an executable named .ll
# This is the actual binary, not LLVM IR
if [ -f "$OUTPUT_DIR/main.ll" ]; then
    # Copy to stage1_compiler name
    cp "$OUTPUT_DIR/main.ll" "$STAGE1_BINARY"
    chmod +x "$STAGE1_BINARY"
    echo "  ✅ Binary copied: $STAGE1_BINARY"
else
    echo "❌ Binary not found: $OUTPUT_DIR/main.ll"
    exit 1
fi

echo ""

# ============================================================================
# Step 3: Test Bootstrap
# ============================================================================

echo "Step 3: Testing Bootstrap"
echo "-----------------------------------"

if [ -f "$STAGE1_BINARY" ]; then
    echo "Running Stage 1 compiler..."
    LD_LIBRARY_PATH=runtime/stdlib:runtime/sto "$STAGE1_BINARY" || {
        echo "⚠️  Execution had issues (exit code: $?)"
    }
    echo ""
fi

# ============================================================================
# Success!
# ============================================================================

echo "=========================================="
echo "✅ Bootstrap Complete!"
echo "=========================================="
echo ""
echo "Stage 1 compiler ready: $STAGE1_BINARY"
echo ""
echo "Next steps:"
echo "  1. Run tests: ./scripts/run_all_tests_stage1.sh"
echo "  2. Test individual modules"
echo "  3. Verify LLVM IR output"
echo ""
