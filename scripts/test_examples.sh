#!/bin/bash
# Stage1 examples/en/base/ Test Script
# Created: 11 Mart 2026 by Stage1 MM
# Purpose: Test all folders in examples/en/base/ and generate report

set -u

STAGE1_ROOT="/home/pardus/PROJELER/MELP/LLVM/STAGE1"
EXAMPLES_DIR="$STAGE1_ROOT/examples/en/base"
NORMALIZER_BIN="/home/pardus/PROJELER/MELP/ORTAK/normalizer/melp_normalizer_bin"
COMPILER="$STAGE1_ROOT/bin/melp_compiler_bin"
OUTPUT_DIR="/tmp/melp_test_$$"
REPORT_FILE="$STAGE1_ROOT/test_report_11mart.txt"
FAILED_FILE="$STAGE1_ROOT/failed_tests.txt"

# Cleanup function
cleanup() {
    rm -rf "$OUTPUT_DIR"
}

trap cleanup EXIT

# Initialize
mkdir -p "$OUTPUT_DIR"
echo "=== MeLP Stage1 examples/en/base/ Test Report ===" > "$REPORT_FILE"
echo "Date: $(date)" >> "$REPORT_FILE"
echo "Compiler: $COMPILER" >> "$REPORT_FILE"
echo "" >> "$REPORT_FILE"
echo "BAŞARISIZ TESTLER:" > "$FAILED_FILE"
echo "" >> "$FAILED_FILE"

total=0
passed=0
failed=0
skipped=0

cd "$EXAMPLES_DIR" || exit 1

# Find all .mlp files
for folder in */; do
    folder="${folder%/}"
    
    # Skip if not a directory
    [[ ! -d "$folder" ]] && continue
    
    # Find .mlp file in folder
    mlp_file=$(find "$folder" -maxdepth 1 -name "*.mlp" | head -n 1)
    
    if [[ -z "$mlp_file" ]]; then
        echo "⊘ SKIP: $folder (no .mlp file)" | tee -a "$REPORT_FILE"
        ((skipped++))
        continue
    fi
    
    ((total++))
    
    echo -n "Testing $folder ... "
    
    # Normalize: MLP → PMLP (Stage1/Stage0 boundary)
    norm_file="$OUTPUT_DIR/${folder}.pmlp"
    cp "$mlp_file" /tmp/.melp_norm_src
    printf 'english' > /tmp/.melp_norm_lang
    "$NORMALIZER_BIN" > "$norm_file" 2>> "$OUTPUT_DIR/${folder}.log"

    # Compile: PMLP → LLVM IR
    output_ll="$OUTPUT_DIR/${folder}.ll"
    if timeout 30s "$COMPILER" "$norm_file" -o "$output_ll" &> "$OUTPUT_DIR/${folder}.log"; then
        # Check if .ll file is valid (non-empty, and either bitcode or text LLVM IR)
        if [[ -s "$output_ll" ]] && { file "$output_ll" | grep -qi "LLVM\|bitcode" || grep -q "define" "$output_ll"; }; then
            echo "✓ PASS" | tee -a "$REPORT_FILE"
            ((passed++))
        else
            echo "✗ FAIL (invalid .ll)" | tee -a "$REPORT_FILE"
            echo "━━━ $folder ━━━" >> "$FAILED_FILE"
            echo "Reason: Generated .ll file invalid or empty" >> "$FAILED_FILE"
            cat "$OUTPUT_DIR/${folder}.log" >> "$FAILED_FILE"
            echo "" >> "$FAILED_FILE"
            ((failed++))
        fi
    else
        echo "✗ FAIL (compilation error)" | tee -a "$REPORT_FILE"
        echo "━━━ $folder ━━━" >> "$FAILED_FILE"
        echo "Reason: Compilation failed or timeout" >> "$FAILED_FILE"
        cat "$OUTPUT_DIR/${folder}.log" >> "$FAILED_FILE"
        echo "" >> "$FAILED_FILE"
        ((failed++))
    fi
done

# Summary
echo "" | tee -a "$REPORT_FILE"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" | tee -a "$REPORT_FILE"
echo "ÖZET (SUMMARY)" | tee -a "$REPORT_FILE"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" | tee -a "$REPORT_FILE"
echo "Total Folders: $total" | tee -a "$REPORT_FILE"
echo "Passed: $passed" | tee -a "$REPORT_FILE"
echo "Failed: $failed" | tee -a "$REPORT_FILE"
echo "Skipped: $skipped (no .mlp file)" | tee -a "$REPORT_FILE"

if [[ $total -gt 0 ]]; then
    success_rate=$(awk "BEGIN {printf \"%.1f\", ($passed/$total)*100}")
    echo "Success Rate: $success_rate%" | tee -a "$REPORT_FILE"
    
    if (( $(echo "$success_rate >= 85.0" | bc -l) )); then
        echo "Status: ✓ BAŞARILI (>= 85%)" | tee -a "$REPORT_FILE"
    else
        echo "Status: ⚠ UYARI (< 85%)" | tee -a "$REPORT_FILE"
    fi
else
    echo "Status: ERROR (no tests found)" | tee -a "$REPORT_FILE"
fi

echo "" | tee -a "$REPORT_FILE"
echo "Full report: $REPORT_FILE" | tee -a "$REPORT_FILE"
echo "Failed tests: $FAILED_FILE" | tee -a "$REPORT_FILE"
