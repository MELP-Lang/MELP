#!/bin/bash
# YZ_113: Convert 'returns' keyword to 'as' in Stage 1 modules
# PMPL uses 'as' for return type declaration, not 'returns'

echo "🔧 Converting 'returns' → 'as' in Stage 1 modules..."
echo ""

count=0
find archive/stage1_api_attempt/modules -name "*.mlp" -type f | while read file; do
    # Check if file contains 'returns'
    if grep -q " returns " "$file"; then
        echo "Processing: $file"
        
        # Replace 'returns' with 'as' in function declarations
        # Pattern: ') returns type' → ') as type'
        sed -i 's/) returns /) as /g' "$file"
        
        count=$((count + 1))
        echo "  ✅ Converted"
    fi
done

echo ""
echo "✅ Conversion complete!"
echo "Note: Please test Stage 1 modules after this change."
