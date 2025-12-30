#!/bin/bash
# Stage1 Single-File Compiler Builder
# Merges all MLP-generated C files into one unified file

set -e

GENERATED_DIR="$(cd "$(dirname "$0")/generated" && pwd)"
OUTPUT_FILE="stage1_unified.c"

echo "🔨 Building Stage1 Unified Compiler..."
echo ""

# Start with header
cat > "$OUTPUT_FILE" << 'EOF'
// MLP-GCC Stage1 Unified Compiler
// Generated: $(date)
// Build: Single-file compilation (Bootstrap pattern)

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// STO Runtime types
typedef struct { /* bigdecimal */ } mlp_bigdecimal_t;
typedef struct { char* data; size_t len; } mlp_string_t;

// Forward declarations (Stage0 simple translator - missing types)
typedef int Boolean;
typedef void* HashMap;
typedef void* ASTArena;
typedef void* FunctionRegistry;


EOF

echo "📦 Merging modules in dependency order..."

# Dependency order (manual, based on type usage)
MODULES=(
    "token_types"
    "gc_integration"
    "function_registry"
    "scope_manager"
    "import_handler"
    "parser"
    "parser_functions"
    "parser_structures"
    "parser_advanced"
    "codegen"
    "codegen_control"
    "codegen_functions"
    "codegen_lambda"
    "codegen_structures"
    # codegen_async skipped - duplicate functions
    "mlp_compiler_main"
)

for module in "${MODULES[@]}"; do
    file="$GENERATED_DIR/${module}.c"
    if [ -f "$file" ]; then
        echo "  ✅ $module"
        # Skip #include, STO types (duplicates), comment blocks, and main() in non-main modules
        if [ "$module" = "mlp_compiler_main" ]; then
            grep -v "^#include" "$file" | \
            grep -v "^// MLP-GCC Generated" | \
            grep -v "^// Target:" | \
            grep -v "^// STO:" | \
            grep -v "^typedef struct { /\* bigdecimal \*/ }" | \
            grep -v "^typedef struct { char\* data; size_t len; }" | \
            sed '/^\/\/ STO Runtime types$/,/^$/d' >> "$OUTPUT_FILE"
        else
            # Remove main() function from non-main modules
            grep -v "^#include" "$file" | \
            grep -v "^// MLP-GCC Generated" | \
            grep -v "^// Target:" | \
            grep -v "^// STO:" | \
            grep -v "^typedef struct { /\* bigdecimal \*/ }" | \
            grep -v "^typedef struct { char\* data; size_t len; }" | \
            sed '/^\/\/ STO Runtime types$/,/^$/d' | \
            awk '/^int main\(void\) \{/,/^\}$/ {next} {print}' >> "$OUTPUT_FILE"
        fi
        echo "" >> "$OUTPUT_FILE"
    else
        echo "  ⚠️  $module (not found)"
    fi
done

echo ""
echo "✅ Unified file created: $OUTPUT_FILE"
echo "📊 Size: $(wc -l < "$OUTPUT_FILE") lines"
echo ""
echo "🔨 Compiling..."
gcc -c "$OUTPUT_FILE" -o stage1_unified.o -Wno-implicit-function-declaration -Wno-unused-variable 2>&1 | head -20

if [ -f stage1_unified.o ]; then
    echo "✅ Compilation successful!"
    ls -lh stage1_unified.o
else
    echo "❌ Compilation failed"
    exit 1
fi
