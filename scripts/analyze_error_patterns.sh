#!/bin/bash
# YZ_22: Error Pattern Analysis Script

LOG_DIR="/home/pardus/projeler/MLP/MLP/logs/stage1_errors"
ANALYSIS_FILE="/home/pardus/projeler/MLP/MLP/logs/YZ_22_ERROR_PATTERNS.md"

echo "# YZ_22: Error Pattern Analysis" > "$ANALYSIS_FILE"
echo "" >> "$ANALYSIS_FILE"
echo "**Date:** $(date)" >> "$ANALYSIS_FILE"
echo "**Total Modules Tested:** 42" >> "$ANALYSIS_FILE"
echo "**Success Rate:** 17/42 (40%)" >> "$ANALYSIS_FILE"
echo "**Failure Rate:** 25/42 (60%)" >> "$ANALYSIS_FILE"
echo "" >> "$ANALYSIS_FILE"

echo "## 🔍 Top Error Patterns (Grouped)" >> "$ANALYSIS_FILE"
echo "" >> "$ANALYSIS_FILE"

# Pattern 1: Expected 'function' keyword
echo "### 1️⃣ Parser: Expected 'function' keyword (90 occurrences)" >> "$ANALYSIS_FILE"
echo "" >> "$ANALYSIS_FILE"
grep -r "Expected 'function' keyword" "$LOG_DIR"/*.log | head -10 >> "$ANALYSIS_FILE"
echo "" >> "$ANALYSIS_FILE"

# Pattern 2: Expected '(' after 'print'
echo "### 2️⃣ Print Statement: Expected '(' after 'print' (22 occurrences)" >> "$ANALYSIS_FILE"
echo "" >> "$ANALYSIS_FILE"
grep -r "Expected '(' after 'print'" "$LOG_DIR"/*.log | head -10 >> "$ANALYSIS_FILE"
echo "" >> "$ANALYSIS_FILE"

# Pattern 3: Expected ')' after list index
echo "### 3️⃣ Array Access: Expected ')' after list index (20 occurrences)" >> "$ANALYSIS_FILE"
echo "" >> "$ANALYSIS_FILE"
grep -r "Expected ')' after list index" "$LOG_DIR"/*.log | head -10 >> "$ANALYSIS_FILE"
echo "" >> "$ANALYSIS_FILE"

# Pattern 4: Expected 'then' after if condition
echo "### 4️⃣ If Statement: Expected 'then' after if condition (17 occurrences)" >> "$ANALYSIS_FILE"
echo "" >> "$ANALYSIS_FILE"
grep -r "Expected 'then' after if condition" "$LOG_DIR"/*.log | head -10 >> "$ANALYSIS_FILE"
echo "" >> "$ANALYSIS_FILE"

# Pattern 5: Module not found
echo "### 5️⃣ Import System: Module not found (multiple)" >> "$ANALYSIS_FILE"
echo "" >> "$ANALYSIS_FILE"
grep -r "Module.*not found in search paths" "$LOG_DIR"/*.log | head -15 >> "$ANALYSIS_FILE"
echo "" >> "$ANALYSIS_FILE"

# Pattern 6: Unexpected token in arithmetic
echo "### 6️⃣ Arithmetic: Unexpected token in arithmetic expression (7 occurrences)" >> "$ANALYSIS_FILE"
echo "" >> "$ANALYSIS_FILE"
grep -r "Unexpected token in arithmetic expression" "$LOG_DIR"/*.log >> "$ANALYSIS_FILE"
echo "" >> "$ANALYSIS_FILE"

echo "## 📊 Failed Modules Breakdown" >> "$ANALYSIS_FILE"
echo "" >> "$ANALYSIS_FILE"
grep "FAIL:" "$LOG_DIR/summary.txt" >> "$ANALYSIS_FILE"
echo "" >> "$ANALYSIS_FILE"

echo "## ✅ Passing Modules" >> "$ANALYSIS_FILE"
echo "" >> "$ANALYSIS_FILE"
grep "PASS:" "$LOG_DIR/summary.txt" >> "$ANALYSIS_FILE"
echo "" >> "$ANALYSIS_FILE"

echo "Analysis saved to: $ANALYSIS_FILE"
