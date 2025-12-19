#!/bin/bash
# YZ_102: Batch convert comma to semicolon in Stage 1 modules
# Converts PMPL function call syntax: func(a, b) → func(a; b)

# Find all .mlp files and convert commas in function calls to semicolons
find archive/stage1_api_attempt/modules -name "*.mlp" -type f | while read file; do
    echo "Processing: $file"
    
    # Create backup
    cp "$file" "$file.backup"
    
    # Replace comma with semicolon in function parameter declarations
    # Pattern: function name(type param1, type param2)
    sed -i 's/function \([a-zA-Z_][a-zA-Z0-9_]*\)(\([^)]*\),\([^)]*\))/function \1(\2;\3)/g' "$file"
    
    # Replace comma with semicolon in function calls (simple cases)
    # Pattern: func(arg1, arg2)
    # Note: This is a simple regex, may need manual review for complex cases
    sed -i 's/\([a-zA-Z_][a-zA-Z0-9_]*\)(\([^,;)]*\),\s*\([^,;)]*\))/\1(\2; \3)/g' "$file"
    
    # Check if file changed
    if diff -q "$file" "$file.backup" > /dev/null; then
        echo "  No changes"
        rm "$file.backup"
    else
        echo "  ✅ Converted"
    fi
done

echo ""
echo "Conversion complete! Please review changes manually."
echo "Backups saved as *.backup"
