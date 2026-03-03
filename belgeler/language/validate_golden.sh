#!/bin/bash

# MELP Golden Reference Validator (Stage-aware)
# Purpose: Ensure golden reference file is always syntactically valid
# Usage: ./validate_golden.sh [0|1|2]
#   ./validate_golden.sh 0  → Validate golden_reference0.mlp (Stage 0)
#   ./validate_golden.sh 1  → Validate golden_reference1.mlp (Stage 1)
#   ./validate_golden.sh 2  → Validate golden_reference2.mlp (Stage 2)
#   ./validate_golden.sh    → Validate all stages
# Run: Before any commit, in CI/CD pipeline

set -e  # Exit on first error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LEXER="$SCRIPT_DIR/../../compiler/paradigma2/MODERN/build/common/lexer/lexer"
TOKENS_OUTPUT="/tmp/golden_tokens.json"

# Parse stage argument
STAGE="${1:-all}"

# Validate stage argument
if [[ "$STAGE" != "0" && "$STAGE" != "1" && "$STAGE" != "2" && "$STAGE" != "all" ]]; then
    echo "❌ ERROR: Invalid stage argument!"
    echo "   Usage: ./validate_golden.sh [0|1|2|all]"
    echo "   Examples:"
    echo "     ./validate_golden.sh 0    → Validate Stage 0 only"
    echo "     ./validate_golden.sh all  → Validate all stages (default)"
    exit 1
fi

# Determine which files to validate
if [ "$STAGE" = "all" ]; then
    GOLDEN_FILES=("$SCRIPT_DIR/golden_reference0.mlp" "$SCRIPT_DIR/golden_reference1.mlp" "$SCRIPT_DIR/golden_reference2.mlp")
else
    GOLDEN_FILES=("$SCRIPT_DIR/golden_reference${STAGE}.mlp")
fi

echo "======================================================"
echo "  MELP Golden Reference Validation"
echo "======================================================"
echo ""
echo "🎯 Target: Stage $STAGE"
echo ""

# Check if lexer exists
if [ ! -f "$LEXER" ]; then
    echo "⚠️  WARNING: Lexer not built yet!"
    echo "   Expected: $LEXER"
    echo "   Run: cd compiler/paradigma2/MODERN && cmake -B build && cmake --build build"
    exit 1
fi

echo "✅ Lexer found: $(basename $LEXER)"
echo ""

# Validate each golden reference file
ALL_PASSED=true

for GOLDEN_REF in "${GOLDEN_FILES[@]}"; do
    echo "------------------------------------------------------"
    echo "📄 Validating: $(basename $GOLDEN_REF)"
    echo "------------------------------------------------------"
    echo ""
    
    # Check if golden reference exists
    if [ ! -f "$GOLDEN_REF" ]; then
        echo "❌ ERROR: Golden reference not found!"
        echo "   Expected: $GOLDEN_REF"
        ALL_PASSED=false
        continue
    fi
    
    echo "✅ Golden reference found: $(basename $GOLDEN_REF)"
    echo ""

    echo "✅ Golden reference found: $(basename $GOLDEN_REF)"
    echo ""
    
    # Run lexer on golden reference
    echo "🔍 Running lexer validation..."
    echo "   Command: $LEXER $GOLDEN_REF > $TOKENS_OUTPUT"
    echo ""
    
    if "$LEXER" "$GOLDEN_REF" > "$TOKENS_OUTPUT" 2>&1; then
        echo "✅ GOLDEN REFERENCE VALID!"
        echo ""
        
        # Show token count
        TOKEN_COUNT=$(cat "$TOKENS_OUTPUT" | wc -l)
        echo "📊 Statistics:"
        echo "   Tokens generated: $TOKEN_COUNT"
        echo "   Output saved: $TOKENS_OUTPUT"
        echo ""
        
        # Show first 10 tokens as sample
        echo "📋 Sample tokens (first 10):"
        head -10 "$TOKENS_OUTPUT" | sed 's/^/   /'
        echo ""
    else
        echo "❌ GOLDEN REFERENCE BROKEN!"
        echo ""
        echo "🔍 Lexer output:"
        cat "$TOKENS_OUTPUT" | sed 's/^/   /'
        echo ""
        ALL_PASSED=false
        
        echo "💡 Common issues:"
        echo "   - Syntax error in golden reference"
        echo "   - New keyword not added to lexer"
        echo "   - Token definition changed"
        echo ""
        echo "📚 Refer to: belgeler/language/feature_cards/"
        echo ""
    fi
done

echo "======================================================"
if [ "$ALL_PASSED" = true ]; then
    echo "  ✅ ALL VALIDATIONS PASSED"
    echo "======================================================"
    exit 0
else
    echo "  ❌ SOME VALIDATIONS FAILED - FIX IMMEDIATELY!"
    echo "======================================================"
    exit 1
fi

