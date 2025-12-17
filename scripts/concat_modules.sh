#!/bin/bash
# ============================================================================
# MELP Module Concatenation Script
# ============================================================================
# Purpose: Combine all modular source files into single-file for Stage 0
# Author: YZ_27
# Date: 17 Aralık 2025
# Usage: ./scripts/concat_modules.sh > temp/compiler_monolithic.mlp
# ============================================================================

set -e  # Exit on error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

echo "-- ============================================================================"
echo "-- MELP Self-Hosting Compiler - Monolithic Build"
echo "-- ============================================================================"
echo "-- Generated: $(date)"
echo "-- Source: Modular architecture (69 modules)"
echo "-- Build: Stage 0 single-file workaround"
echo "-- Purpose: Self-hosting bootstrap"
echo "--"
echo "-- Module Order:"
echo "--   1. Lexer (tokenization)"
echo "--   2. Parser (AST construction)"
echo "--   3. CodeGen (LLVM IR generation)"
echo "--   4. Compiler (orchestration)"
echo "-- ============================================================================"
echo ""

# ============================================================================
# Helper: Add module with header
# ============================================================================
add_module() {
    local module_path="$1"
    local module_name="$(basename "$module_path")"
    
    if [[ ! -f "$module_path" ]]; then
        echo "ERROR: Module not found: $module_path" >&2
        return 1
    fi
    
    echo ""
    echo "-- ============================================================================"
    echo "-- MODULE: $module_name"
    echo "-- ============================================================================"
    echo ""
    
    # Filter out global constants and variables (Stage 0 limitation)
    # Keep only: comments, blank lines, and function definitions
    cat "$module_path" | awk '
        # Keep comments and blank lines
        /^--/ || /^$/ { print; next }
        
        # Skip global constant declarations (uppercase names)
        /^numeric [A-Z_]+ =/ { next }
        /^string [A-Z_]+ =/ { next }
        /^boolean [A-Z_]+ =/ { next }
        
        # Skip global variable declarations (both with and without comments)
        /^list [a-z_]+/ { next }
        /^numeric [a-z_]+/ { next }
        /^string [a-z_]+/ { next }
        /^boolean [a-z_]+/ { next }
        
        # Keep everything else (function definitions)
        { print }
    '
}

# ============================================================================
# PHASE 1: LEXER MODULES (no dependencies)
# ============================================================================
echo "-- PHASE 1: LEXER"
echo ""

# Core lexer (must be first - defines token types)
add_module "$PROJECT_ROOT/modules/lexer_mlp/lexer.mlp"

# Lexer API wrapper
add_module "$PROJECT_ROOT/modules/lexer_mlp/lexer_api.mlp"

# ============================================================================
# PHASE 2: PARSER MODULES (depends on lexer token types)
# ============================================================================
echo ""
echo "-- PHASE 2: PARSER"
echo ""

# Parser core (defines parser state)
add_module "$PROJECT_ROOT/modules/parser_mlp/parser.mlp"

# Parser API wrapper
add_module "$PROJECT_ROOT/modules/parser_mlp/parser_api.mlp"

# ============================================================================
# PHASE 3: CODEGEN MODULES (depends on parser AST)
# ============================================================================
echo ""
echo "-- PHASE 3: CODEGEN"
echo ""

# CodeGen integration (main dispatcher)
add_module "$PROJECT_ROOT/modules/codegen_mlp/codegen_integration.mlp"

# CodeGen API wrapper
add_module "$PROJECT_ROOT/modules/codegen_mlp/codegen_api.mlp"

# ============================================================================
# PHASE 4: COMPILER ORCHESTRATION (depends on all)
# ============================================================================
echo ""
echo "-- PHASE 4: COMPILER"
echo ""

# Main compiler (has main() - keep it!)
add_module "$PROJECT_ROOT/modules/compiler_full.mlp"

echo ""
echo "-- ============================================================================"
echo "-- End of monolithic build"
echo "-- ============================================================================"
