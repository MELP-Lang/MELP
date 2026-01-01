#!/bin/bash

# Test Runner for Thread Module
# YZ_02 - TODO #2 Task 2: Threading & Concurrency

set -e  # Exit on error

cd "$(dirname "$0")"
RUNTIME_PATH="../../MELP/runtime/thread"

echo "======================================"
echo "  MLP Threading Module - Test Suite"
echo "======================================"
echo ""

# Compile and run test_thread_basic
echo "▶ Test 1: Basic Thread Operations"
echo "  Compiling..."
gcc -o test_thread_basic test_thread_basic.c \
    $RUNTIME_PATH/thread.c \
    -I$RUNTIME_PATH \
    -lpthread \
    -Wall -Wextra

echo "  Running..."
./test_thread_basic
echo ""

# Compile and run test_mutex_basic
echo "▶ Test 2: Basic Mutex Operations"
echo "  Compiling..."
gcc -o test_mutex_basic test_mutex_basic.c \
    $RUNTIME_PATH/mutex.c \
    $RUNTIME_PATH/thread.c \
    -I$RUNTIME_PATH \
    -lpthread \
    -Wall -Wextra

echo "  Running..."
./test_mutex_basic
echo ""

# Compile and run test_channel_basic
echo "▶ Test 3: Basic Channel Operations"
echo "  Compiling..."
gcc -o test_channel_basic test_channel_basic.c \
    $RUNTIME_PATH/channel.c \
    $RUNTIME_PATH/thread.c \
    -I$RUNTIME_PATH \
    -lpthread \
    -Wall -Wextra

echo "  Running..."
./test_channel_basic
echo ""

echo "======================================"
echo "  ✅ ALL TESTS PASSED SUCCESSFULLY"
echo "======================================"
echo ""
echo "Summary:"
echo "  - Thread: create, join, sleep, current_id ✓"
echo "  - Mutex: lock, unlock, key registry ✓"
echo "  - Channel: send, receive, buffering ✓"
echo ""
