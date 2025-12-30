#!/bin/bash
# Stage1 Build Script - P3.2 Linking
# 15 MLP modül + Stage0 lexer/codegen (hybrid)

set -e

STAGE1_DIR="/home/pardus/projeler/MLP/MLP-GCC/MELP/MLP/stage1"
STAGE0_DIR="/home/pardus/projeler/MLP/MLP-GCC/MELP/C/stage0"
RUNTIME_DIR="/home/pardus/projeler/MLP/MLP-GCC/MELP/runtime"

echo "🔨 Stage1 Build Starting..."
echo ""

# 1. Compile generated C files to object files
echo "📦 Step 1: Compiling generated C files..."
cd "$STAGE1_DIR/generated"
for f in *.c; do
    if [ -f "$f" ] && [ -s "$f" ]; then
        echo "  Compiling: $f"
        gcc -c "$f" -o "${f%.c}.o" \
            -I"$STAGE0_DIR" \
            -I"$STAGE0_DIR/modules" \
            -I"$RUNTIME_DIR" \
            -Wall -Wno-unused-variable -Wno-unused-function 2>&1 | head -5
    fi
done
echo ""

# 2. Copy Stage0 lexer and codegen (hybrid approach)
echo "🔗 Step 2: Adding Stage0 lexer and codegen (hybrid)..."
cp "$STAGE0_DIR/lexer.c" ./lexer_stage0.c
cp "$STAGE0_DIR/modules/codegen_emit/codegen_emit.c" ./codegen_emit_stage0.c

gcc -c lexer_stage0.c -o lexer.o -I"$STAGE0_DIR" -I"$RUNTIME_DIR"
gcc -c codegen_emit_stage0.c -o codegen_emit.o -I"$STAGE0_DIR" -I"$RUNTIME_DIR"
echo ""

# 3. Link all objects
echo "🔗 Step 3: Linking mlpc_stage1..."
cd "$STAGE1_DIR"
gcc generated/*.o \
    -o bin/mlpc_stage1 \
    -L"$RUNTIME_DIR" \
    -lm 2>&1 | head -10

if [ -f bin/mlpc_stage1 ]; then
    echo ""
    echo "✅ BUILD SUCCESS!"
    ls -lh bin/mlpc_stage1
    echo ""
    echo "🧪 Quick test:"
    ./bin/mlpc_stage1 --version 2>&1 || echo "(version flag not implemented yet)"
else
    echo ""
    echo "❌ BUILD FAILED!"
    exit 1
fi
