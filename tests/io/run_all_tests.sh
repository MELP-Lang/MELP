#!/bin/bash
# Test runner for File I/O module tests

echo "========================================="
echo "MELP File I/O Module Test Suite"
echo "Task 5: File I/O Complete (YZ_04)"
echo "========================================="
echo ""

# Color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

PASSED=0
FAILED=0

# Function to compile and run a test
run_test() {
    test_name=$1
    source_file="${test_name}.c"
    
    echo -n "Compiling ${test_name}... "
    
    # Compile
    gcc -o "$test_name" "$source_file" \
        ../../MELP/runtime/io/path.c \
        ../../MELP/runtime/file_io.c \
        -I../../MELP/runtime \
        -I../../MELP/runtime/io \
        2> compile_error.log
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}COMPILE FAILED${NC}"
        cat compile_error.log
        ((FAILED++))
        return 1
    fi
    
    echo -e "${GREEN}OK${NC}"
    
    # Run test
    echo "Running ${test_name}..."
    echo "---"
    ./"$test_name"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ ${test_name} PASSED${NC}"
        ((PASSED++))
        echo ""
        return 0
    else
        echo -e "${RED}✗ ${test_name} FAILED${NC}"
        ((FAILED++))
        echo ""
        return 1
    fi
}

# Test 1: Path Join
run_test "test_path_join"

# Test 2: Path Normalize
run_test "test_path_normalize"

# Test 3: Path Components (basename, dirname, extension)
run_test "test_path_components"

# Test 4: Path Checks (exists, is_file, is_directory, is_absolute)
run_test "test_path_checks"

# Test 5: Directory Listing
run_test "test_directory_list"

# Test 6: File Metadata
run_test "test_file_metadata"

# Test 7: File Operations (read, write, append)
run_test "test_file_operations"

# Summary
echo "========================================="
echo "TEST SUMMARY"
echo "========================================="
echo -e "Total Tests:  $((PASSED + FAILED))"
echo -e "${GREEN}Passed:       ${PASSED}${NC}"
echo -e "${RED}Failed:       ${FAILED}${NC}"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}✅ ALL TESTS PASSED! 🎉${NC}"
    echo ""
    echo "File I/O module is ready for:"
    echo "  ✓ Module loading (path operations)"
    echo "  ✓ File reading/writing"
    echo "  ✓ Directory discovery"
    echo "  ✓ File metadata (cache invalidation)"
    echo ""
    exit 0
else
    echo -e "${RED}❌ SOME TESTS FAILED${NC}"
    exit 1
fi
