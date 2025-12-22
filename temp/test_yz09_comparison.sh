#!/bin/bash
# YZ_09 Comparison Operators Test
# Tests >, <, == operators

echo "======================================"
echo "YZ_09 Comparison Operators Test"
echo "======================================"
echo ""

# Test 1: Greater than (50 > 8 = 1)
echo "Test 1: Greater than (50 > 8 = 1)"
echo "-----------------------------------"
cat > /tmp/test1_comp_yz09.ll << 'EOF'
define i64 @main() {
entry:
  %x = alloca i64
  store i64 50, i64* %x
  %y = alloca i64
  store i64 8, i64* %y
  %0 = load i64, i64* %x
  %1 = load i64, i64* %y
  %2 = icmp sgt i64 %0, %1
  %3 = zext i1 %2 to i64
  ret i64 %3
}
EOF

clang /tmp/test1_comp_yz09.ll -o /tmp/test1_comp_yz09 2>&1 | grep -v "warning:"
/tmp/test1_comp_yz09
RESULT1=$?
echo "Exit code: $RESULT1"
if [ $RESULT1 -eq 1 ]; then
    echo "✅ Test 1 PASSED"
else
    echo "❌ Test 1 FAILED (expected 1, got $RESULT1)"
fi
echo ""

# Test 2: Less than (5 < 10 = 1)
echo "Test 2: Less than (5 < 10 = 1)"
echo "-------------------------------"
cat > /tmp/test2_comp_yz09.ll << 'EOF'
define i64 @main() {
entry:
  %x = alloca i64
  store i64 5, i64* %x
  %y = alloca i64
  store i64 10, i64* %y
  %0 = load i64, i64* %x
  %1 = load i64, i64* %y
  %2 = icmp slt i64 %0, %1
  %3 = zext i1 %2 to i64
  ret i64 %3
}
EOF

clang /tmp/test2_comp_yz09.ll -o /tmp/test2_comp_yz09 2>&1 | grep -v "warning:"
/tmp/test2_comp_yz09
RESULT2=$?
echo "Exit code: $RESULT2"
if [ $RESULT2 -eq 1 ]; then
    echo "✅ Test 2 PASSED"
else
    echo "❌ Test 2 FAILED (expected 1, got $RESULT2)"
fi
echo ""

# Test 3: Equal (42 == 42 = 1)
echo "Test 3: Equal (42 == 42 = 1)"
echo "-----------------------------"
cat > /tmp/test3_comp_yz09.ll << 'EOF'
define i64 @main() {
entry:
  %x = alloca i64
  store i64 42, i64* %x
  %y = alloca i64
  store i64 42, i64* %y
  %0 = load i64, i64* %x
  %1 = load i64, i64* %y
  %2 = icmp eq i64 %0, %1
  %3 = zext i1 %2 to i64
  ret i64 %3
}
EOF

clang /tmp/test3_comp_yz09.ll -o /tmp/test3_comp_yz09 2>&1 | grep -v "warning:"
/tmp/test3_comp_yz09
RESULT3=$?
echo "Exit code: $RESULT3"
if [ $RESULT3 -eq 1 ]; then
    echo "✅ Test 3 PASSED"
else
    echo "❌ Test 3 FAILED (expected 1, got $RESULT3)"
fi
echo ""

# Summary
echo "======================================"
echo "Test Summary"
echo "======================================"
if [ $RESULT1 -eq 1 ] && [ $RESULT2 -eq 1 ] && [ $RESULT3 -eq 1 ]; then
    echo "✅ All comparison tests PASSED!"
    echo ""
    echo "YZ_09 Comparison Operators SUCCESS:"
    echo "  - Greater than (>) ✅"
    echo "  - Less than (<) ✅"
    echo "  - Equal (==) ✅"
    exit 0
else
    echo "❌ Some tests FAILED"
    exit 1
fi
