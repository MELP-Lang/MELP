# MLP Testing Guide

**Version:** 1.0  
**Author:** YZ_STAGE1_07  
**Date:** 27 Ocak 2026

---

## Table of Contents

1. [Introduction](#introduction)
2. [Getting Started](#getting-started)
3. [Writing Your First Test](#writing-your-first-test)
4. [Assertions Reference](#assertions-reference)
5. [Test Organization](#test-organization)
6. [Test Suites and Hooks](#test-suites-and-hooks)
7. [Best Practices](#best-practices)
8. [Common Patterns](#common-patterns)
9. [Testing Strategies](#testing-strategies)
10. [Performance Testing](#performance-testing)
11. [Debugging Failed Tests](#debugging-failed-tests)
12. [CI/CD Integration](#cicd-integration)
13. [Advanced Topics](#advanced-topics)
14. [FAQ](#faq)

---

## Introduction

### What is Testing?

Testing is the practice of verifying that your code works as expected. Tests help you:

- **Catch bugs early** before they reach production
- **Document behavior** through executable examples
- **Enable refactoring** with confidence
- **Prevent regressions** when adding new features
- **Improve design** by writing testable code

### The MLP Testing Framework

MLP provides a built-in testing framework (`std.test`) that supports:

- ✅ **Assertions** - Verify expected outcomes
- ✅ **Test suites** - Group related tests
- ✅ **Hooks** - Setup and teardown
- ✅ **Statistics** - Track test results
- ✅ **Clear output** - Understand failures quickly

---

## Getting Started

### Installation

The test framework is part of the standard library. No installation needed!

```mlp
import std.test
```

### Your First Test File

Create a file named `test_math.pmpl`:

```mlp
import std.test

func test_addition() {
    test.assert_eq_numeric(2 + 2, 4)
}

func main() {
    test.test("Addition works", test_addition)
    test.run_tests()
}
```

### Running Tests

```bash
mlp test_math.pmpl
```

Expected output:
```
======================================================================
Running Tests...
======================================================================

🔵 Test 1/1: Addition works
  ✅ PASSED

======================================================================
📊 Results: 1 passed, 0 failed (Duration: 0ms)
======================================================================
```

---

## Writing Your First Test

### The Test Anatomy

A test consists of three parts:

1. **Arrange** - Set up test data
2. **Act** - Execute the code under test
3. **Assert** - Verify the outcome

```mlp
func test_string_concatenation() {
    -- Arrange: Prepare test data
    let first: string = "Hello"
    let second: string = "World"
    
    -- Act: Execute the operation
    let result: string = first + " " + second
    
    -- Assert: Verify the result
    test.assert_eq_string(result, "Hello World")
}
```

### Naming Conventions

**Good test names:**
- `test_addition_with_positive_numbers()`
- `test_string_split_with_comma_delimiter()`
- `test_user_validation_rejects_empty_name()`

**Poor test names:**
- `test1()`
- `check()`
- `it_works()`

The name should clearly describe **what** is being tested and **what** the expected behavior is.

---

## Assertions Reference

### Numeric Assertions

#### `assert_eq_numeric(actual, expected)`

Asserts two numbers are equal (within epsilon precision).

```mlp
test.assert_eq_numeric(5 + 3, 8)
test.assert_eq_numeric(0.1 + 0.2, 0.3)  // Handles floating-point precision
```

#### `assert_ne_numeric(actual, expected)`

Asserts two numbers are NOT equal.

```mlp
test.assert_ne_numeric(5, 6)
test.assert_ne_numeric(10 / 3, 3)
```

### String Assertions

#### `assert_eq_string(actual, expected)`

Asserts two strings are equal (case-sensitive).

```mlp
test.assert_eq_string("hello", "hello")
test.assert_eq_string(concat("foo", "bar"), "foobar")
```

#### `assert_ne_string(actual, expected)`

Asserts two strings are NOT equal.

```mlp
test.assert_ne_string("Hello", "hello")  // Case matters
test.assert_ne_string("test", "TEST")
```

### Boolean Assertions

#### `assert_true(condition)`

Asserts a condition is true.

```mlp
test.assert_true(5 > 3)
test.assert_true(len("test") == 4)
test.assert_true(is_valid_user(user))
```

#### `assert_false(condition)`

Asserts a condition is false.

```mlp
test.assert_false(5 < 3)
test.assert_false(is_empty("hello"))
```

### Custom Assertions

#### `assert(condition, message)`

Assert with a custom error message.

```mlp
let age: numeric = get_user_age()
test.assert(age >= 18, "User must be 18 or older")
test.assert(age <= 120, "Age must be realistic")
```

**Use custom messages when:**
- The assertion logic is complex
- You want to provide context for failures
- The default error message isn't clear enough

---

## Test Organization

### Single Test File

For small projects, keep all tests in one file:

```mlp
import std.test

func test_feature1() { ... }
func test_feature2() { ... }
func test_feature3() { ... }

func main() {
    test.test("Feature 1", test_feature1)
    test.test("Feature 2", test_feature2)
    test.test("Feature 3", test_feature3)
    test.run_tests()
}
```

### Multiple Test Files

For larger projects, organize by module:

```
tests/
├── test_math.pmpl
├── test_strings.pmpl
├── test_collections.pmpl
└── test_user.pmpl
```

Run all tests:
```bash
mlp tests/test_*.pmpl
```

### Test Directory Structure

Recommended structure:

```
my_project/
├── src/
│   ├── math.pmpl
│   ├── strings.pmpl
│   └── user.pmpl
├── tests/
│   ├── test_math.pmpl
│   ├── test_strings.pmpl
│   └── test_user.pmpl
└── examples/
    └── demo.pmpl
```

---

## Test Suites and Hooks

### Basic Test Suite

Group related tests with `describe()`:

```mlp
import std.test

func main() {
    test.describe("Math Operations")
    
    func test_add() {
        test.assert_eq_numeric(2 + 2, 4)
    }
    test.test("Addition", test_add)
    
    func test_sub() {
        test.assert_eq_numeric(5 - 3, 2)
    }
    test.test("Subtraction", test_sub)
    
    test.end_describe()
    test.run_tests()
}
```

Output:
```
📦 Test Suite: Math Operations
   └─ 2 tests registered
```

### Setup and Teardown Hooks

#### before_each / after_each

Run before/after **each** test in the suite:

```mlp
let counter: numeric = 0

func setup() {
    counter = 0
    print("Setup: counter reset")
}

func cleanup() {
    print("Cleanup: test completed")
}

test.describe("Counter Tests")
test.before_each(setup)
test.after_each(cleanup)

func test1() {
    counter = counter + 1
    test.assert_eq_numeric(counter, 1)
}
test.test("Test 1", test1)

func test2() {
    counter = counter + 1
    test.assert_eq_numeric(counter, 1)  // Counter was reset!
}
test.test("Test 2", test2)

test.end_describe()
```

#### before_all / after_all

Run once before/after **all** tests in the suite:

```mlp
let database: string = ""

func connect_db() {
    database = "Connected"
    print("Database connected")
}

func disconnect_db() {
    database = ""
    print("Database disconnected")
}

test.describe("Database Tests")
test.before_all(connect_db)
test.after_all(disconnect_db)

func test1() {
    test.assert_eq_string(database, "Connected")
}
test.test("Connection test", test1)

test.end_describe()
```

### Hook Execution Order

```
before_all()
    before_each()
        test_1()
    after_each()
    
    before_each()
        test_2()
    after_each()
after_all()
```

---

## Best Practices

### 1. One Assertion Per Test (Ideally)

**Good:**
```mlp
func test_addition_positive() {
    test.assert_eq_numeric(2 + 3, 5)
}

func test_addition_negative() {
    test.assert_eq_numeric(-2 + -3, -5)
}
```

**Acceptable:**
```mlp
func test_addition_edge_cases() {
    test.assert_eq_numeric(0 + 0, 0)
    test.assert_eq_numeric(-5 + 5, 0)
    test.assert_eq_numeric(1 + 0, 1)
}
```

### 2. Test Edge Cases

Always test:
- **Zero values**: `0`, `""`, empty arrays
- **Negative values**: `-1`, `-100`
- **Boundaries**: Min/max values
- **Special cases**: `null`, `undefined` (if applicable)

```mlp
func test_division_edge_cases() {
    test.assert_eq_numeric(10 / 2, 5)        // Normal case
    test.assert_eq_numeric(0 / 5, 0)         // Zero numerator
    test.assert_eq_numeric(10 / 1, 10)       // Divide by 1
    // Note: Division by zero should be handled separately
}
```

### 3. Test Error Conditions

Don't just test the "happy path":

```mlp
func test_user_validation() {
    -- Happy path
    test.assert_true(is_valid_email("user@example.com"))
    
    -- Error cases
    test.assert_false(is_valid_email(""))
    test.assert_false(is_valid_email("invalid"))
    test.assert_false(is_valid_email("@example.com"))
}
```

### 4. Keep Tests Independent

Each test should be runnable in isolation:

**Bad:**
```mlp
let shared_state: numeric = 0

func test1() {
    shared_state = 10
}

func test2() {
    test.assert_eq_numeric(shared_state, 10)  // Depends on test1!
}
```

**Good:**
```mlp
func test1() {
    let state: numeric = 10
    test.assert_eq_numeric(state, 10)
}

func test2() {
    let state: numeric = 10
    test.assert_eq_numeric(state, 10)  // Independent
}
```

### 5. Use Descriptive Variable Names

```mlp
-- Bad
func test_calc() {
    let x = 5
    let y = 3
    let z = x + y
    test.assert_eq_numeric(z, 8)
}

-- Good
func test_addition_returns_sum() {
    let first_number: numeric = 5
    let second_number: numeric = 3
    let sum: numeric = first_number + second_number
    test.assert_eq_numeric(sum, 8)
}
```

### 6. Fast Tests Are Good Tests

- Avoid slow operations (network, disk I/O)
- Use test doubles/mocks for dependencies
- Keep setup/teardown minimal

---

## Common Patterns

### Pattern 1: Testing Return Values

```mlp
func add(a: numeric, b: numeric) -> numeric {
    return a + b
}

func test_add_returns_correct_sum() {
    let result: numeric = add(5, 3)
    test.assert_eq_numeric(result, 8)
}
```

### Pattern 2: Testing State Changes

```mlp
let counter: numeric = 0

func increment() {
    counter = counter + 1
}

func test_increment_increases_counter() {
    counter = 0
    increment()
    test.assert_eq_numeric(counter, 1)
}
```

### Pattern 3: Testing String Operations

```mlp
func test_string_operations() {
    -- Concatenation
    test.assert_eq_string("foo" + "bar", "foobar")
    
    -- Case sensitivity
    test.assert_ne_string("Hello", "hello")
    
    -- Length
    test.assert_true(len("test") == 4)
}
```

### Pattern 4: Testing Collections

```mlp
func test_array_operations() {
    let arr: Array<numeric> = [1, 2, 3, 4, 5]
    
    -- Length
    test.assert_eq_numeric(len(arr), 5)
    
    -- Access
    test.assert_eq_numeric(arr[0], 1)
    test.assert_eq_numeric(arr[4], 5)
    
    -- Contains
    test.assert_true(contains(arr, 3))
    test.assert_false(contains(arr, 10))
}
```

### Pattern 5: Testing with Test Doubles

```mlp
-- Real function (slow, uses network)
func fetch_user_from_api(id: numeric) -> string {
    -- Makes HTTP request...
    return "User " + str(id)
}

-- Test double (fast, no network)
func mock_fetch_user(id: numeric) -> string {
    return "Test User " + str(id)
}

func test_user_processing() {
    let user: string = mock_fetch_user(123)
    test.assert_eq_string(user, "Test User 123")
}
```

---

## Testing Strategies

### Unit Testing

Test individual functions in isolation:

```mlp
func multiply(a: numeric, b: numeric) -> numeric {
    return a * b
}

func test_multiply() {
    test.assert_eq_numeric(multiply(2, 3), 6)
    test.assert_eq_numeric(multiply(0, 5), 0)
    test.assert_eq_numeric(multiply(-2, 3), -6)
}
```

### Integration Testing

Test multiple components working together:

```mlp
func calculate_total(price: numeric, quantity: numeric, tax_rate: numeric) -> numeric {
    let subtotal: numeric = multiply(price, quantity)
    let tax: numeric = multiply(subtotal, tax_rate)
    return add(subtotal, tax)
}

func test_calculate_total_integration() {
    let total: numeric = calculate_total(10, 2, 0.1)
    test.assert_eq_numeric(total, 22)  // (10 * 2) + (20 * 0.1) = 22
}
```

### Regression Testing

Prevent old bugs from coming back:

```mlp
-- Bug: divide_by_zero crashed the program
-- Fix: Now returns 0 instead

func test_divide_by_zero_returns_zero() {
    let result: numeric = safe_divide(10, 0)
    test.assert_eq_numeric(result, 0)
}
```

### Test-Driven Development (TDD)

1. Write test first (it fails)
2. Write minimal code to make it pass
3. Refactor

```mlp
-- Step 1: Write failing test
func test_square() {
    test.assert_eq_numeric(square(5), 25)
}

-- Step 2: Implement
func square(x: numeric) -> numeric {
    return x * x
}

-- Step 3: Test passes, refactor if needed
```

---

## Performance Testing

### Basic Performance Test

```mlp
import std.test

func test_performance() {
    let start_time: numeric = get_current_time_ms()
    
    -- Operation to test
    let result: numeric = 0
    let i: numeric = 0
    while i < 1000000 {
        result = result + 1
        i = i + 1
    }
    
    let end_time: numeric = get_current_time_ms()
    let duration: numeric = end_time - start_time
    
    print("Operation took: " + str(duration) + "ms")
    test.assert_true(duration < 1000)  // Should complete in < 1 second
}
```

### Benchmark Comparison

```mlp
func test_performance_comparison() {
    -- Method 1
    let start1: numeric = get_current_time_ms()
    method1()
    let time1: numeric = get_current_time_ms() - start1
    
    -- Method 2
    let start2: numeric = get_current_time_ms()
    method2()
    let time2: numeric = get_current_time_ms() - start2
    
    print("Method 1: " + str(time1) + "ms")
    print("Method 2: " + str(time2) + "ms")
    
    -- Assert method2 is faster
    test.assert_true(time2 < time1)
}
```

---

## Debugging Failed Tests

### Understanding Error Messages

```
❌ Test: Addition test
   Error: Expected 5, got 4
```

This tells you:
- **Which test failed**: "Addition test"
- **What went wrong**: Expected 5, got 4
- **Where to look**: The assertion that failed

### Adding Debug Output

```mlp
func test_complex_calculation() {
    let x: numeric = 10
    let y: numeric = 20
    let result: numeric = complex_calc(x, y)
    
    print("Debug: x=" + str(x) + ", y=" + str(y) + ", result=" + str(result))
    
    test.assert_eq_numeric(result, 30)
}
```

### Isolating the Problem

If a test fails:

1. **Run it alone** - Does it still fail?
2. **Check dependencies** - Are other tests affecting it?
3. **Simplify** - Remove code until it passes, then add back
4. **Add assertions** - Test intermediate values

---

## CI/CD Integration

### Running Tests in CI

Example GitHub Actions workflow:

```yaml
name: Run Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - name: Run tests
        run: mlp tests/test_*.pmpl
```

### Exit Codes

Tests return:
- `0` - All tests passed
- `1` - Some tests failed

Use in scripts:
```bash
mlp tests/test_all.pmpl
if [ $? -eq 0 ]; then
    echo "✅ All tests passed!"
else
    echo "❌ Tests failed!"
    exit 1
fi
```

---

## Advanced Topics

### Parameterized Tests

Test multiple inputs with the same logic:

```mlp
func test_is_even() {
    let test_cases: Array<numeric> = [0, 2, 4, 6, 8, 10]
    
    let i: numeric = 0
    while i < len(test_cases) {
        let value: numeric = test_cases[i]
        test.assert_true(is_even(value))
        i = i + 1
    }
}
```

### Testing Async Code

(Future feature - planned for Day 4+)

```mlp
func test_async_operation() {
    test.describe("Async Tests")
    
    func test_fetch() {
        let result = await fetch_data()
        test.assert_eq_string(result, "data")
    }
    test.test_async("Fetch data", test_fetch)
    
    test.end_describe()
}
```

### Custom Matchers

Create reusable assertion helpers:

```mlp
func assert_is_positive(value: numeric) {
    test.assert_true(value > 0)
}

func assert_is_email(value: string) {
    test.assert_true(contains(value, "@"))
    test.assert_true(contains(value, "."))
}

func test_custom_matchers() {
    assert_is_positive(42)
    assert_is_email("user@example.com")
}
```

---

## FAQ

### Q: How many tests should I write?

**A:** Enough to give you confidence. Start with:
- One test per public function
- Additional tests for edge cases
- Tests for any bugs you fix

### Q: Should I test private functions?

**A:** Generally, no. Test the public API. Private functions are tested indirectly.

### Q: How do I test code with side effects?

**A:** Use test doubles, mocks, or dependency injection to isolate the code.

### Q: My tests are slow. What should I do?

**A:**
1. Avoid I/O operations (network, disk)
2. Use smaller test data
3. Run tests in parallel (future feature)
4. Profile to find slow tests

### Q: Should tests have assertions?

**A:** Yes! A test without assertions doesn't verify anything.

### Q: How do I test randomness?

**A:**
- Set a fixed seed for reproducibility
- Test the distribution/range instead of exact values
- Use multiple runs to verify randomness

```mlp
func test_random_in_range() {
    let i: numeric = 0
    while i < 100 {
        let value: numeric = random_int(1, 10)
        test.assert_true(value >= 1)
        test.assert_true(value <= 10)
        i = i + 1
    }
}
```

---

## Summary

### Key Takeaways

1. ✅ **Write tests** for all important code
2. ✅ **Use descriptive names** so tests document behavior
3. ✅ **Keep tests independent** so they can run in any order
4. ✅ **Test edge cases** not just the happy path
5. ✅ **Use suites and hooks** to organize tests
6. ✅ **Run tests frequently** catch bugs early
7. ✅ **Fix failing tests** don't let them accumulate

### Quick Reference Card

```mlp
import std.test

-- Assertions
test.assert_eq_numeric(actual, expected)
test.assert_eq_string(actual, expected)
test.assert_ne_numeric(actual, expected)
test.assert_ne_string(actual, expected)
test.assert_true(condition)
test.assert_false(condition)
test.assert(condition, "message")

-- Test registration
test.test("test name", test_function)

-- Test suites
test.describe("Suite name")
test.end_describe()

-- Hooks
test.before_all(setup_function)
test.after_all(teardown_function)
test.before_each(setup_function)
test.after_each(teardown_function)

-- Run tests
let stats = test.run_tests()
```

---

## Additional Resources

- **API Reference**: See [README.md](README.md)
- **Examples**: See `examples/` directory
- **Test Suite**: See `tests/` directory for real-world examples

---

**Happy Testing!** 🎉

If you have questions or suggestions, please file an issue or contribute to the documentation.
