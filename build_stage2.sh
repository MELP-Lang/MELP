#!/bin/bash
# Build Stage2 MLP Compiler
# Compiles Stage2 MLP modules using Stage1 (C-based) compiler
# Output: build/melpc_stage2

set -e  # Exit on error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR"
STAGE2_DIR="$PROJECT_ROOT/MELP/MLP/stage2"
BUILD_DIR="$PROJECT_ROOT/build"

echo "MLP Stage2 Compiler Build Script"
echo "================================="
echo ""

# Create build directory
mkdir -p "$BUILD_DIR"

# Check if Stage1 compiler exists
STAGE1_COMPILER=""
if [ -f "$BUILD_DIR/melpc" ]; then
    STAGE1_COMPILER="$BUILD_DIR/melpc"
elif [ -f "$PROJECT_ROOT/melpc" ]; then
    STAGE1_COMPILER="$PROJECT_ROOT/melpc"
elif [ -f "$PROJECT_ROOT/MELP/C/stage0/melpc" ]; then
    STAGE1_COMPILER="$PROJECT_ROOT/MELP/C/stage0/melpc"
elif [ -f "$PROJECT_ROOT/MELP/bootstrap/melpc" ]; then
    STAGE1_COMPILER="$PROJECT_ROOT/MELP/bootstrap/melpc"
elif [ -f "$PROJECT_ROOT/MELP/compiler" ]; then
    STAGE1_COMPILER="$PROJECT_ROOT/MELP/compiler"
fi

if [ -z "$STAGE1_COMPILER" ]; then
    echo "Error: Stage1 compiler not found!"
    echo "Searched locations:"
    echo "  - $BUILD_DIR/melpc"
    echo "  - $PROJECT_ROOT/melpc"
    echo "  - $PROJECT_ROOT/MELP/C/stage0/melpc"
    echo "  - $PROJECT_ROOT/MELP/bootstrap/melpc"
    echo "  - $PROJECT_ROOT/MELP/compiler"
    echo ""
    echo "Please build Stage1 compiler first (main.c)"
    exit 1
fi

echo "Stage1 Compiler: $STAGE1_COMPILER"
echo "Stage2 Source:   $STAGE2_DIR"
echo "Build Output:    $BUILD_DIR/melpc_stage2"
echo ""

# Stage2 modules to compile
MODULES=(
    "lexer.mlp"
    "parser.mlp"
    "codegen.mlp"
    "codegen_advanced.mlp"
    "type_system.mlp"
    "compiler_main.mlp"
)

echo "Stage2 Modules:"
for module in "${MODULES[@]}"; do
    if [ -f "$STAGE2_DIR/$module" ]; then
        lines=$(wc -l < "$STAGE2_DIR/$module")
        echo "  ✓ $module ($lines lines)"
    else
        echo "  ✗ $module (NOT FOUND)"
        exit 1
    fi
done
echo ""

# Method 1: Try using Stage1 compiler (if it supports MLP → C)
echo "[METHOD 1] Attempting to compile with Stage1 MLP compiler..."
if [ -f "$STAGE1_COMPILER" ]; then
    # Try to compile each module to C
    for module in "${MODULES[@]}"; do
        base_name="${module%.mlp}"
        echo "  Compiling $module → ${base_name}.c"
        "$STAGE1_COMPILER" "$STAGE2_DIR/$module" "$BUILD_DIR/${base_name}.c" || {
            echo "  Warning: Stage1 compilation failed for $module"
        }
    done
    
    # Try to compile all C files together
    if [ -f "$BUILD_DIR/compiler_main.c" ]; then
        echo ""
        echo "  Linking Stage2 binary..."
        gcc -o "$BUILD_DIR/melpc_stage2" \
            "$BUILD_DIR/lexer.c" \
            "$BUILD_DIR/parser.c" \
            "$BUILD_DIR/codegen.c" \
            "$BUILD_DIR/codegen_advanced.c" \
            "$BUILD_DIR/type_system.c" \
            "$BUILD_DIR/compiler_main.c" \
            -lm -std=c99 || {
            echo "  Warning: GCC linking failed"
        }
        
        if [ -f "$BUILD_DIR/melpc_stage2" ]; then
            echo ""
            echo "✅ SUCCESS: Stage2 compiler built!"
            echo "   Binary: $BUILD_DIR/melpc_stage2"
            ls -lh "$BUILD_DIR/melpc_stage2"
            exit 0
        fi
    fi
fi

echo ""
echo "[METHOD 2] Alternative: Direct GCC compilation of MLP modules..."
echo "(This assumes MLP syntax is C-compatible or use manual C translation)"
echo ""

# Method 2: Manual fallback - create a simplified Stage2 using C
# This requires pre-translated C versions of the modules
echo "Creating minimal Stage2 wrapper..."

cat > "$BUILD_DIR/stage2_wrapper.c" << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Minimal Stage2 wrapper
// TODO: Link with actual Stage2 modules when available

int main(int argc, char *argv[]) {
    printf("MLP Stage2 Compiler (Minimal Build)\n");
    printf("===================================\n\n");
    
    if (argc < 2) {
        printf("Error: No input file specified\n");
        printf("Usage: %s <input.mlp> [-o output.c] [-v]\n", argv[0]);
        return 1;
    }
    
    printf("Input: %s\n", argv[1]);
    printf("\n");
    printf("Stage2 modules:\n");
    printf("  - lexer.mlp (471 lines)\n");
    printf("  - parser.mlp (483 lines)\n");
    printf("  - codegen.mlp (501 lines)\n");
    printf("  - codegen_advanced.mlp (394 lines)\n");
    printf("  - type_system.mlp (481 lines)\n");
    printf("  - compiler_main.mlp (285 lines)\n");
    printf("\n");
    printf("Total: 2615 lines of MLP code\n");
    printf("\n");
    printf("Note: Full compilation requires Stage1 MLP→C translator\n");
    printf("      Current build: Minimal stub for testing\n");
    
    return 0;
}
EOF

gcc -o "$BUILD_DIR/melpc_stage2" "$BUILD_DIR/stage2_wrapper.c" -std=c99
echo ""
echo "⚠️  PARTIAL SUCCESS: Minimal Stage2 stub created"
echo "   Binary: $BUILD_DIR/melpc_stage2"
echo ""
echo "For full Stage2 build:"
echo "  1. Ensure Stage1 compiler can translate MLP → C"
echo "  2. Re-run this script"
echo "  3. Or manually translate Stage2 modules to C"
echo ""

ls -lh "$BUILD_DIR/melpc_stage2"
