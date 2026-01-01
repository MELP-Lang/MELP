#!/bin/bash
# Run all networking tests
# YZ_01 - TODO #2 Task 1

echo "======================================"
echo "  MELP Networking Library Test Suite"
echo "======================================"
echo ""

TESTS_PASSED=0
TESTS_FAILED=0
TEST_DIR="tests/net"

# Compile and run test function
run_test() {
    local test_name=$1
    local test_file=$2
    local extra_args=$3
    
    echo "----------------------------------------"
    echo "Running: $test_name"
    echo "----------------------------------------"
    
    # Compile
    gcc -o "$TEST_DIR/$test_name" "$TEST_DIR/$test_file" \
        MELP/runtime/net/socket.c MELP/runtime/net/http.c \
        -IMELP/runtime/net $extra_args 2>&1
    
    local compile_result=$?
    if [ $compile_result -ne 0 ]; then
        echo "✗ FAILED: Compilation error"
        TESTS_FAILED=$((TESTS_FAILED + 1))
        return 1
    fi
    
    # Run
    "$TEST_DIR/$test_name"
    local exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        echo "✓ PASSED"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo "✗ FAILED: Exit code $exit_code"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
    
    echo ""
    return $exit_code
}

# Test 1: Socket - TCP Connection
run_test "test_socket" "test_socket_simple.c"

# Test 2: Socket - UDP
run_test "test_udp" "test_udp.c"

# Test 3: Socket - TCP Server
echo "Note: Server test will timeout after 5 seconds"
run_test "test_server" "test_server.c"

# Test 4: HTTP - GET Request
run_test "test_http" "test_http_simple.c"

# Test 5: HTTP - POST Request
run_test "test_http_post" "test_http_post.c"

# Summary
echo "======================================"
echo "  Test Summary"
echo "======================================"
echo "Tests Passed: $TESTS_PASSED"
echo "Tests Failed: $TESTS_FAILED"
echo "Total Tests:  $((TESTS_PASSED + TESTS_FAILED))"
echo ""

if [ $TESTS_FAILED -eq 0 ]; then
    echo "✓ ALL TESTS PASSED ✓"
    exit 0
else
    echo "✗ SOME TESTS FAILED ✗"
    exit 1
fi
