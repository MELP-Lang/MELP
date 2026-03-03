#!/bin/bash

# MELP Stage 1 - Tasks 11-15 Compiler and Runner Script
# Usage: ./RUN_TASKS_11_15.sh

MELP="/home/pardus/projeler/MLP/LLVM/stage0/v2/build/compiler/stage1/modules/orchestrator/melp_compiler"
BASE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Tasks to run
TASKS=(
    "11_string_interpolation:string_interpolation.mlp"
    "12_nested_functions:nested_functions.mlp"
    "13_higher_order:higher_order.mlp"
    "14_lambda:lambda.mlp"
    "15_struct:struct.mlp"
)

# Color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "=========================================="
echo "MELP Stage 1 - Tasks 11-15"
echo "=========================================="
echo ""

for task in "${TASKS[@]}"; do
    IFS=':' read -r task_dir file_name <<< "$task"
    task_path="$BASE_DIR/$task_dir/$file_name"
    task_num=$(echo "$task_dir" | cut -d_ -f1)

    echo -n "Task $task_num ($task_dir)... "

    if [ ! -f "$task_path" ]; then
        echo -e "${RED}❌ FILE NOT FOUND${NC}: $task_path"
        continue
    fi

    # Compile
    output_ll="/tmp/out_task${task_num}.ll"
    output_elf="/tmp/out_task${task_num}.elf"

    if ! "$MELP" "$task_path" -o "$output_ll" 2>/dev/null; then
        echo -e "${RED}❌ COMPILE FAILED${NC}"
        continue
    fi

    # Link
    if ! clang -O2 -x ir "$output_ll" -o "$output_elf" 2>/dev/null; then
        echo -e "${RED}❌ LINK FAILED${NC}"
        continue
    fi

    # Run
    if output=$("$output_elf" 2>&1); then
        echo -e "${GREEN}✅ PASS${NC}"
        echo "Output:"
        echo "$output" | sed 's/^/  /'
    else
        echo -e "${RED}❌ RUNTIME ERROR${NC}"
    fi

    echo ""
done

echo "=========================================="
echo "Done!"
echo "=========================================="
