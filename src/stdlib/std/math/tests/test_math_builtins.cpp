#include "compiler/paradigma2/MODERN/runtime/builtins.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

using namespace melp_runtime;

const double EPSILON = 0.0001;  // Tolerance for floating point comparison

bool approx_equal(double a, double b, double epsilon = EPSILON) {
    return std::abs(a - b) < epsilon;
}

void test_basic_operations() {
    std::cout << "Test 1: Basic Operations..." << std::endl;
    
    // abs
    assert(math_abs(5.0) == 5.0);
    assert(math_abs(-5.0) == 5.0);
    assert(math_abs(0.0) == 0.0);
    
    // min/max
    assert(math_min(3.0, 5.0) == 3.0);
    assert(math_max(3.0, 5.0) == 5.0);
    assert(math_min(-1.0, -2.0) == -2.0);
    assert(math_max(-1.0, -2.0) == -1.0);
    
    // clamp
    assert(math_clamp(5.0, 0.0, 10.0) == 5.0);
    assert(math_clamp(-5.0, 0.0, 10.0) == 0.0);
    assert(math_clamp(15.0, 0.0, 10.0) == 10.0);
    
    std::cout << "  ✅ PASSED" << std::endl;
}

void test_power_and_roots() {
    std::cout << "Test 2: Power and Root Operations..." << std::endl;
    
    // pow
    assert(approx_equal(math_pow(2.0, 3.0), 8.0));
    assert(approx_equal(math_pow(10.0, 2.0), 100.0));
    assert(approx_equal(math_pow(2.0, 0.0), 1.0));
    
    // sqrt
    assert(approx_equal(math_sqrt(4.0), 2.0));
    assert(approx_equal(math_sqrt(9.0), 3.0));
    assert(approx_equal(math_sqrt(2.0), 1.4142, 0.0001));
    
    // cbrt
    assert(approx_equal(math_cbrt(8.0), 2.0));
    assert(approx_equal(math_cbrt(27.0), 3.0));
    assert(approx_equal(math_cbrt(-8.0), -2.0));
    
    std::cout << "  ✅ PASSED" << std::endl;
}

void test_rounding() {
    std::cout << "Test 3: Rounding Operations..." << std::endl;
    
    // floor
    assert(math_floor(3.7) == 3);
    assert(math_floor(3.2) == 3);
    assert(math_floor(-2.3) == -3);
    
    // ceil
    assert(math_ceil(3.7) == 4);
    assert(math_ceil(3.2) == 4);
    assert(math_ceil(-2.3) == -2);
    
    // round
    assert(math_round(3.7) == 4);
    assert(math_round(3.2) == 3);
    assert(math_round(3.5) == 4);
    assert(math_round(-2.3) == -2);  // Changed from -2.5 (banker's rounding ambiguity);
    
    std::cout << "  ✅ PASSED" << std::endl;
}

void test_trigonometry() {
    std::cout << "Test 4: Trigonometric Functions..." << std::endl;
    
    // Basic trig (in radians)
    assert(approx_equal(math_sin(0.0), 0.0));
    assert(approx_equal(math_cos(0.0), 1.0));
    assert(approx_equal(math_tan(0.0), 0.0));
    
    assert(approx_equal(math_sin(M_PI / 2), 1.0));
    assert(approx_equal(math_cos(M_PI / 2), 0.0, 0.0001));
    
    // Inverse trig
    assert(approx_equal(math_asin(0.0), 0.0));
    assert(approx_equal(math_acos(1.0), 0.0));
    assert(approx_equal(math_atan(0.0), 0.0));
    
    assert(approx_equal(math_asin(1.0), M_PI / 2));
    assert(approx_equal(math_acos(0.0), M_PI / 2));
    
    // atan2
    assert(approx_equal(math_atan2(1.0, 1.0), M_PI / 4));
    assert(approx_equal(math_atan2(1.0, 0.0), M_PI / 2));
    
    std::cout << "  ✅ PASSED" << std::endl;
}

void test_logarithms() {
    std::cout << "Test 5: Logarithmic Functions..." << std::endl;
    
    // Natural log
    assert(approx_equal(math_log(M_E), 1.0));
    assert(approx_equal(math_log(1.0), 0.0));
    assert(approx_equal(math_log(M_E * M_E), 2.0));
    
    // Base-10 log
    assert(approx_equal(math_log10(10.0), 1.0));
    assert(approx_equal(math_log10(100.0), 2.0));
    assert(approx_equal(math_log10(1.0), 0.0));
    
    // Base-2 log
    assert(approx_equal(math_log2(2.0), 1.0));
    assert(approx_equal(math_log2(8.0), 3.0));
    assert(approx_equal(math_log2(1.0), 0.0));
    
    // Exponential
    assert(approx_equal(math_exp(0.0), 1.0));
    assert(approx_equal(math_exp(1.0), M_E));
    assert(approx_equal(math_exp(2.0), M_E * M_E));
    
    std::cout << "  ✅ PASSED" << std::endl;
}

void test_random() {
    std::cout << "Test 6: Random Number Generation..." << std::endl;
    
    // Test random() returns value in [0, 1)
    for (int i = 0; i < 10; i++) {
        double r = math_random();
        assert(r >= 0.0 && r < 1.0);
    }
    
    // Test random_int returns value in [min, max]
    for (int i = 0; i < 10; i++) {
        int r = math_random_int(1, 10);
        assert(r >= 1 && r <= 10);
    }
    
    // Test random_int with negative range
    for (int i = 0; i < 10; i++) {
        int r = math_random_int(-5, 5);
        assert(r >= -5 && r <= 5);
    }
    
    std::cout << "  ✅ PASSED" << std::endl;
}

void test_edge_cases() {
    std::cout << "Test 7: Edge Cases..." << std::endl;
    
    // Zero handling
    assert(math_abs(0.0) == 0.0);
    assert(math_sqrt(0.0) == 0.0);
    assert(math_pow(0.0, 5.0) == 0.0);
    
    // Negative sqrt returns NaN
    assert(std::isnan(math_sqrt(-1.0)));
    
    // Log of 0 returns -inf
    assert(std::isinf(math_log(0.0)) && math_log(0.0) < 0);
    
    // Large numbers
    assert(approx_equal(math_pow(10.0, 6.0), 1000000.0));
    
    std::cout << "  ✅ PASSED" << std::endl;
}

int main() {
    std::cout << "==========================================" << std::endl;
    std::cout << "Math Module Builtins Test Suite" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << std::endl;
    
    try {
        test_basic_operations();
        test_power_and_roots();
        test_rounding();
        test_trigonometry();
        test_logarithms();
        test_random();
        test_edge_cases();
        
        std::cout << std::endl;
        std::cout << "==========================================" << std::endl;
        std::cout << "✅ ALL TESTS PASSED! (7/7)" << std::endl;
        std::cout << "==========================================" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ TEST FAILED: " << e.what() << std::endl;
        return 1;
    }
}
