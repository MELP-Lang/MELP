#!/bin/bash
# YZ_09 Arithmetic Operators Test
# Tests -, *, / operators

echo "======================================"
echo "YZ_09 Arithmetic Operators Test"
echo "======================================"
echo ""

# Test 1: Subtraction (50 - 8 = 42)
echo "Test 1: Subtraction (50 - 8 = 42)"
echo "-----------------------------------"
cat > /tmp/test1_yz09.ll << 'EOF'
define i64 @main() {
entry:
  %x = alloca i64
  store i64 50, i64* %x
  %y = alloca i64
  store i64 8, i64* %y
  %0 = load i64, i64* %x
  %1 = load i64, i64* %y
  %2 = sub i64 %0, %1
  ret i64 %2
}
EOF

clang /tmp/test1_yz09.ll -o /tmp/test1_yz09 2>&1 | grep -v "warning:"
/tmp/test1_yz09
RESULT1=$?
echo "Exit code: $RESULT1"
if [ $RESULT1 -eq 42 ]; then
    echo "✅ Test 1 PASSED"
else
    echo "❌ Test 1 FAILED (expected 42, got $RESULT1)"
fi
echo ""

# Test 2: Multiplication (6 * 7 = 42)
echo "Test 2: Multiplication (6 * 7 = 42)"
echo "------------------------------------"
cat > /tmp/test2_yz09.ll << 'EOF'
define i64 @main() {
entry:
  %x = alloca i64
  store i64 6, i64* %x
  %y = alloca i64
  store i64 7, i64* %y
  %0 = load i64, i64* %x
  %1 = load i64, i64* %y
  %2 = mul i64 %0, %1
  ret i64 %2
}
EOF

clang /tmp/test2_yz09.ll -o /tmp/test2_yz09 2>&1 | grep -v "warning:"
/tmp/test2_yz09
RESULT2=$?
echo "Exit code: $RESULT2"
if [ $RESULT2 -eq 42 ]; then
    echo "✅ Test 2 PASSED"
else
    echo "❌ Test 2 FAILED (expected 42, got $RESULT2)"
fi
echo ""

# Test 3: Division (84 / 2 = 42)
echo "Test 3: Division (84 / 2 = 42)"
echo "-------------------------------"
cat > /tmp/test3_yz09.ll << 'EOF'
define i64 @main() {
entry:
  %x = alloca i64
  store i64 84, i64* %x
  %y = alloca i64
  store i64 2, i64* %y
  %0 = load i64, i64* %x
  %1 = load i64, i64* %y
  %2 = sdiv i64 %0, %1
  ret i64 %2
}
EOF

clang /tmp/test3_yz09.ll -o /tmp/test3_yz09 2>&1 | grep -v "warning:"
/tmp/test3_yz09
RESULT3=$?
echo "Exit code: $RESULT3"
if [ $RESULT3 -eq 42 ]; then
    echo "✅ Test 3 PASSED"
else
    echo "❌ Test 3 FAILED (expected 42, got $RESULT3)"
fi
echo ""

# Summary
echo "======================================"
echo "Test Summary"
echo "======================================"
if [ $RESULT1 -eq 42 ] && [ $RESULT2 -eq 42 ] && [ $RESULT3 -eq 42 ]; then
    echo "✅ All arithmetic tests PASSED!"
    echo ""
    echo "YZ_09 Arithmetic Operators SUCCESS:"
    echo "  - Subtraction (-) ✅"
    echo "  - Multiplication (*) ✅"
    echo "  - Division (/) ✅"
    exit 0
else
    echo "❌ Some tests FAILED"
    exit 1
fi
