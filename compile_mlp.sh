#!/bin/bash
# ============================================================================
# MELP Stage 1 Compiler - Build and Test Script (YZ_117)
# ============================================================================
# Purpose: Compile .mlp files to executable binaries
# Usage: ./compile_mlp.sh <input.mlp> <output_binary>
# ============================================================================

set -e

if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <input.mlp> <output_binary>"
    echo ""
    echo "Example:"
    echo "  $0 test.mlp test_binary"
    exit 1
fi

INPUT_MLP="$1"
OUTPUT_BIN="$2"
TEMP_ASM="/tmp/$(basename ${INPUT_MLP%.mlp}).s"

# Colors
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  MELP Stage 1 Compiler - YZ_117${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# Step 1: Compile .mlp to assembly
echo -e "${GREEN}[1/3] Compiling .mlp to assembly...${NC}"
echo "  Input:  $INPUT_MLP"
echo "  Output: $TEMP_ASM"

cd /home/pardus/projeler/MLP/MLP/compiler/stage0/modules/functions
./functions_compiler "$INPUT_MLP" "$TEMP_ASM"

if [ ! -f "$TEMP_ASM" ]; then
    echo "ERROR: Assembly generation failed"
    exit 1
fi

echo ""

# Step 2: Link with runtime
echo -e "${GREEN}[2/3] Linking with runtime...${NC}"
RUNTIME_DIR="/home/pardus/projeler/MLP/MLP/runtime"

gcc -no-pie \
    "$TEMP_ASM" \
    "$RUNTIME_DIR/sto/runtime_sto.c" \
    "$RUNTIME_DIR/sto/bigdecimal.c" \
    "$RUNTIME_DIR/sto/sso_string.c" \
    "$RUNTIME_DIR/string_helpers.c" \
    -o "$OUTPUT_BIN" 2>&1 | grep -v "missing .note.GNU-stack" | grep -v "deprecated" || true

if [ ! -f "$OUTPUT_BIN" ]; then
    echo "ERROR: Linking failed"
    exit 1
fi

echo "  Binary: $OUTPUT_BIN"
echo ""

# Step 3: Success
echo -e "${GREEN}[3/3] Build successful!${NC}"
echo ""
echo "Run with:"
echo "  $OUTPUT_BIN"
echo ""
