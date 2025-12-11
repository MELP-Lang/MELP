# Smart Type Optimization (STO)

**Version:** 1.0  
**Last Updated:** December 12, 2025  
**Status:** Design Document

---

## Overview

STO (Smart Type Optimization) is MELP's approach to providing a simple user experience while maintaining high performance. Users see only two basic types (`numeric` and `string`), but the compiler automatically selects optimal internal representations.

---

## Philosophy

### The Problem

Traditional languages force users to choose between many numeric types:
- `int8`, `int16`, `int32`, `int64`
- `float`, `double`
- `BigInteger`, `BigDecimal`

This creates cognitive overhead and potential bugs (overflow, precision loss).

### MELP's Solution

**User perspective:** Only `numeric` and `string` exist.

**Compiler perspective:** Automatically select optimal storage:
- Small integers → `int64` (register)
- Decimals → `double` (XMM register)
- Large numbers → `BigDecimal` (heap)

---

## Type Mapping

### Numeric Internal Representations

| User Writes | Value Range | Internal Type | Storage | Performance |
|-------------|-------------|---------------|---------|-------------|
| `numeric x = 42` | -2^63 to 2^63-1 | `int64` | Register/Stack | ⚡ Fastest |
| `numeric y = 3.14` | ~15 digit precision | `double` | XMM Register | ⚡ Fast |
| `numeric z = 10^100` | Unlimited | `BigDecimal` | Heap | 🐢 Slower |

### String Internal Representations

| User Writes | Length | Internal Type | Storage | Performance |
|-------------|--------|---------------|---------|-------------|
| `string s = "Ali"` | ≤23 bytes | SSO (inline) | Stack | ⚡ Fastest |
| `string t = "Long text..."` | >23 bytes | Heap pointer | Heap | 🔄 Normal |
| `string c = "Constant"` | Any (literal) | `.rodata` | Read-only | ⚡ Shared |

**SSO = Small String Optimization:** Short strings stored directly on stack without heap allocation.

---

## Compile-Time Analysis

### Numeric Type Selection Algorithm

```
function analyze_numeric(value):
    if value is_integer AND -2^63 ≤ value ≤ 2^63-1:
        return INT64          -- Store in register
    else if value is_decimal AND digits ≤ 15:
        return DOUBLE         -- Store in XMM register
    else:
        return BIGDECIMAL     -- Store on heap
```

### String Type Selection Algorithm

```
function analyze_string(value):
    if is_literal_constant(value):
        return RODATA_STRING  -- .rodata section
    else if length(value) ≤ 23:
        return SSO_STRING     -- Stack inline
    else:
        return HEAP_STRING    -- Heap allocation
```

---

## Runtime Behavior

### Automatic Overflow Handling

```mlp
numeric x = 9223372036854775807  -- Max int64
x = x + 1                        -- OVERFLOW!

-- What happens internally:
-- 1. Overflow detected
-- 2. x promoted to BigDecimal
-- 3. Operation continues with BigDecimal
-- 4. User sees correct result (no crash!)
```

### Type Promotion Rules

| Operation | Types | Result |
|-----------|-------|--------|
| `int64 + int64` | Same | `int64` |
| `int64 + double` | Mixed | `double` |
| `int64 overflow` | - | `BigDecimal` |
| `double + BigDecimal` | Mixed | `BigDecimal` |

---

## Memory Layout

```
┌─────────────────────────────────────────────────────────────┐
│                         STACK                                │
├─────────────────────────────────────────────────────────────┤
│  int64 values (8 bytes)                                     │
│  double values (8 bytes, aligned)                           │
│  SSO strings (≤24 bytes, inline)                            │
│  Heap pointers (8 bytes, point to heap data)                │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                          HEAP                                │
├─────────────────────────────────────────────────────────────┤
│  BigDecimal structures                                      │
│  Long string data (>23 bytes)                               │
│  Dynamic arrays                                             │
│  Struct instances                                           │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                     .RODATA (Read-only)                      │
├─────────────────────────────────────────────────────────────┤
│  String literals                                            │
│  Constant numeric values                                    │
└─────────────────────────────────────────────────────────────┘
```

---

## Code Generation Examples

### User Code

```mlp
numeric small = 42
numeric decimal = 3.14159
numeric huge = 10 ^ 1000

string short = "Ali"
string long = read_file("book.txt")
```

### Generated Assembly (Conceptual)

```asm
; small = 42 → int64, stored in register
mov rax, 42
mov [rbp-8], rax

; decimal = 3.14159 → double, stored in XMM
movsd xmm0, [.LC0]
movsd [rbp-16], xmm0

; huge = 10^1000 → BigDecimal, heap allocated
call bigdec_pow
mov [rbp-24], rax      ; heap pointer

; short = "Ali" → SSO, inline on stack
mov qword [rbp-48], "Ali\0"  ; 24-byte area, inline

; long = read_file(...) → heap string
call read_file
mov [rbp-56], rax      ; heap pointer
```

---

## Advantages

1. **User Simplicity:** Only `numeric` and `string` - no type confusion
2. **Automatic Performance:** Small values = fast, large values = safe
3. **Memory Efficiency:** No unnecessary heap allocations
4. **Backward Compatible:** Existing MELP code works unchanged
5. **Transparent:** Users don't need to know about optimization

---

## Implementation Status

### Phase 1: Basic STO (In Progress)

**Numeric:**
- [ ] int64 support (small integers)
- [ ] double support (decimals)
- [ ] BigDecimal fallback (large/precise)
- [ ] Overflow detection and auto-promote

**String:**
- [ ] SSO implementation (≤23 bytes inline)
- [ ] Heap string (>23 bytes)
- [ ] Constant string → .rodata

**Codegen:**
- [ ] Type-aware register allocation
- [ ] Optimized assembly patterns

### Phase 2: Advanced STO (Future)

- [ ] Copy-on-write strings
- [ ] String interning
- [ ] int32 usage for smaller values
- [ ] SIMD optimizations

---

## Considerations

### Arithmetic Operations

| Operation | Note |
|-----------|------|
| `int64 + double` | Result is `double` |
| Overflow | Auto-promote to `BigDecimal` |
| Division | May produce `double` from `int64` |

### String Operations

| Operation | Note |
|-----------|------|
| Concatenation | SSO + SSO may become heap |
| Substring | May reference original |
| Comparison | Works across all internal types |

---

## For AI Agents

- STO should be implemented BEFORE self-hosting
- C runtime provides base functions
- Codegen generates type-aware assembly
- Test: Every type combination needs testing
- User API NEVER CHANGES - only backend optimizes

---

*This document describes MELP's internal optimization strategy. Users are not expected to know these details.*
