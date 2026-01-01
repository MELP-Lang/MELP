#!/bin/bash
# MLP Preprocessor - Inline imports for Stage0 compilation
# Usage: ./mlp_preprocess.sh input.mlp output_merged.mlp

set -e

INPUT_FILE="$1"
OUTPUT_FILE="$2"

if [ -z "$INPUT_FILE" ] || [ -z "$OUTPUT_FILE" ]; then
    echo "Usage: $0 <input.mlp> <output_merged.mlp>"
    exit 1
fi

echo "MLP Preprocessor - Inlining imports"
echo "Input: $INPUT_FILE"
echo "Output: $OUTPUT_FILE"

# Get directory of input file
INPUT_DIR="$(cd "$(dirname "$INPUT_FILE")" && pwd)"

# Temporary file for processing
TEMP_FILE=$(mktemp)

# Function to process file recursively
process_file() {
    local file="$1"
    local indent="$2"
    
    echo "${indent}Processing: $file" >&2
    
    while IFS= read -r line; do
        # Check if line is import statement
        if echo "$line" | grep -q "^import"; then
            import_file=$(echo "$line" | sed 's/import[[:space:]]*"\([^"]*\)".*/\1/')
            import_path="$INPUT_DIR/$import_file"
            
            echo "${indent}  Found import: $import_file" >&2
            
            if [ -f "$import_path" ]; then
                echo "-- BEGIN IMPORT: $import_file"
                process_file "$import_path" "$indent  "
                echo "-- END IMPORT: $import_file"
            else
                echo "-- WARNING: Import not found: $import_file"
            fi
        else
            # Output line as-is
            echo "$line"
        fi
    done < "$file"
}

# Process main file
process_file "$INPUT_FILE" "" > "$OUTPUT_FILE"

echo "✅ Preprocessing complete: $OUTPUT_FILE"
echo "Lines: $(wc -l < "$OUTPUT_FILE")"
