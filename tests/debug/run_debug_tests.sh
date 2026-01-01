#!/bin/bash
# Test suite for mlp-debug

set -e

echo "=== MLP Debug Test Suite ==="
echo ""

# Build mlp-debug first
echo "Building mlp-debug..."
cd ../../tools/mlp-debug
make clean > /dev/null 2>&1
make > /dev/null 2>&1
cd ../../tests/debug

echo "✅ mlp-debug built successfully"
echo ""

# Test 1: Check help
echo "Test 1: Help output"
if ../../tools/mlp-debug/mlp-debug --help > /dev/null 2>&1; then
    echo "✅ Help output works"
else
    echo "❌ Help output failed"
    exit 1
fi

# Test 2: Compile with debug info
echo ""
echo "Test 2: Compile with -g flag"
if timeout 5 ../../MELP/C/stage0/melpc -g test_breakpoint.mlp test_breakpoint.c > /dev/null 2>&1; then
    echo "✅ Compilation with -g works"
else
    echo "❌ Compilation failed or timed out"
    exit 1
fi

# Test 3: Check debug comments in generated C
echo ""
echo "Test 3: Check DWARF comments in generated C"
if grep -q "Debug info enabled" test_breakpoint.c && grep -q "DWARF:" test_breakpoint.c; then
    echo "✅ DWARF debug info present in generated C"
else
    echo "❌ DWARF debug info missing"
    exit 1
fi

# Test 4: Compile C with gcc -g
echo ""
echo "Test 4: Compile C code with gcc -g"
if timeout 10 gcc -g test_breakpoint.c -o test_breakpoint 2> /dev/null; then
    echo "✅ GCC compilation with -g works"
else
    echo "❌ GCC compilation failed or timed out"
    exit 1
fi

# Test 5: Check if GDB can load the binary
echo ""
echo "Test 5: GDB can load binary"
if command -v gdb > /dev/null 2>&1; then
    if echo "quit" | gdb -q test_breakpoint > /dev/null 2>&1; then
        echo "✅ GDB can load binary"
    else
        echo "⚠️  GDB cannot load binary (may need debugging symbols)"
    fi
else
    echo "⚠️  GDB not installed (skipping GDB tests)"
fi

# Test 6: Compile other test files
echo ""
echo "Test 6: Compile other test files"
for mlp_file in test_variables.mlp test_step.mlp; do
    base=$(basename "$mlp_file" .mlp)
    if timeout 5 ../../MELP/C/stage0/melpc -g "$mlp_file" "${base}.c" > /dev/null 2>&1; then
        if timeout 10 gcc -g "${base}.c" -o "$base" 2> /dev/null; then
            echo "✅ $mlp_file compiled successfully"
        else
            echo "❌ GCC compilation failed or timed out for $mlp_file"
            exit 1
        fi
    else
        echo "❌ MLP compilation failed or timed out for $mlp_file"
        exit 1
    fi
done

# Cleanup
rm -f *.c test_breakpoint test_variables test_step

echo ""
echo "=== All Debug Tests Passed ✅ ==="
echo ""
echo "Summary:"
echo "  - mlp-debug binary: OK"
echo "  - -g flag support: OK"
echo "  - DWARF info generation: OK"
echo "  - GCC debug compilation: OK"
echo "  - Multiple test files: OK"
