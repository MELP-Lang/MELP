# Test Coverage Strategy - MELP 1.0 Release

**Purpose:** Prioritize test development for maximum safety and quality  
**Goal:** 500+ tests by 1.0 release  
**Current:** 92 tests (Stage 1) → **Target:** 500 tests (18.5% → 100%)  
**Approach:** P0 (Critical) → P1 (Important) → P2 (Nice to have)

---

## 📊 Test Distribution Overview

| Priority | Category | Tests | % of Total | Rationale |
|----------|----------|-------|------------|-----------|
| 🔴 **P0** | **Safety-Critical** | **200** | **40%** | Memory safety, data races, security |
| 🟡 **P1** | **Functional** | **180** | **36%** | Core features, stdlib, FFI |
| 🟢 **P2** | **Quality** | **120** | **24%** | Edge cases, performance, tooling |
| | **Total** | **500** | **100%** | |

---

## 🔴 P0: Safety-Critical Tests (200 tests, 40%)

**Rule:** **P0 tests must pass 100% before moving to next phase**  
**Why P0?** Failures = crashes, security vulnerabilities, undefined behavior

### 1. Memory Safety (80 tests) - Phase 1 & 3

**When:** Phase 1 (30 tests) + Phase 3 (50 tests)

#### Phase 1: Heap Allocation (30 tests)
```
Heap Management:
- [ ] malloc/free basic (5 tests)
  - Allocate, use, free
  - Multiple allocations
  - Zero-size allocation
  - Large allocation (>1MB)
  - Allocation failure handling

- [ ] RAII Destructor (10 tests)
  - Automatic cleanup on scope exit
  - Early return with cleanup
  - Exception with cleanup
  - Nested scopes
  - Destructor call order (LIFO)
  - Destructor with exceptions
  - Move semantics (no double-free)
  - Copy semantics (deep copy)
  - Destructor chaining (member destructors)
  - Virtual destructors

- [ ] Memory Leaks (5 tests)
  - Detect leaks with valgrind
  - Leak on exception
  - Leak on early return
  - Circular reference leak
  - Leak in conditional branches

- [ ] Use-After-Free (5 tests)
  - Access after free (should fail at compile time in Phase 3)
  - Double free (should fail)
  - Dangling pointer
  - Dangling reference
  - Use after move

- [ ] Buffer Overflow (5 tests)
  - Vector bounds checking
  - String bounds checking
  - Array indexing
  - Negative index
  - Large index
```

**Validation:**
- **100% valgrind clean** (0 leaks, 0 errors)
- **ASan (AddressSanitizer)** clean
- **MSan (MemorySanitizer)** clean (uninitialized memory)

#### Phase 3: Ownership System (50 tests)
```
Ownership:
- [ ] Move semantics (10 tests)
  - Simple move
  - Move in function argument
  - Move in return value
  - Move in assignment
  - Conditional move (if/else)
  - Move in loop
  - Partial move (struct field)
  - Move with exceptions
  - Use-after-move detection (compile error)
  - Move-only types (no copy)

- [ ] Borrowing (20 tests)
  - Immutable borrow (10 tests):
    - Single borrow
    - Multiple borrows
    - Nested borrows
    - Borrow in function argument
    - Borrow in return value
    - Borrow with lifetimes
    - Borrow across scopes
    - Borrow in conditionals
    - Borrow in loops
    - Borrow with early return
  
  - Mutable borrow (10 tests):
    - Single mutable borrow
    - Mutable borrow exclusivity (compile error if violated)
    - Mutable + immutable conflict (compile error)
    - Multiple mutable conflicts (compile error)
    - Mutable borrow in function
    - Mutable borrow return
    - Mutable borrow with early return
    - Mutable borrow in loop
    - Mutable borrow reborrow
    - Mutable borrow with move

- [ ] Lifetimes (15 tests)
  - Function parameter lifetimes (5 tests):
    - Single borrow parameter
    - Multiple borrow parameters
    - Return borrowed reference
    - Return longest of two borrows
    - Lifetime elision
  
  - Struct lifetimes (5 tests):
    - Struct with borrowed field
    - Multiple borrowed fields
    - Struct lifetime annotations
    - Nested structs with lifetimes
    - Generic struct with lifetimes
  
  - Edge cases (5 tests):
    - Dangling reference (compile error)
    - Lifetime too short (compile error)
    - Lifetime outlives owner (compile error)
    - Circular references (compile error)
    - Self-referential struct (error or explicit Pin)

- [ ] Real-World Scenarios (5 tests)
  - Linked list (doubly-linked)
  - Binary tree (with parent pointers)
  - Graph (adjacency list)
  - String interning (shared references)
  - Object pool (reuse memory safely)
```

**Validation:**
- **0 false positives** (legit code not rejected)
- **0 false negatives** (unsafe code accepted)
- **Manual security audit** (review all borrow checker logic)

---

### 2. Borrow Checker (50 tests) - Phase 3

**When:** Phase 3 (Ownership System)

```
Error Detection:
- [ ] Use-after-move (10 tests)
  - Simple use-after-move
  - Conditional move + use
  - Loop with move
  - Move in closure
  - Partial move + use other field
  - Move in match expression
  - Move in function call
  - Move in method call
  - Move with reborrow
  - Move with return

- [ ] Aliasing Violations (20 tests)
  - XOR Rule:
    - 1 mutable OR N immutable (base case)
    - Mutable + immutable conflict (error)
    - Multiple mutable conflict (error)
    - Nested borrows
    - Reborrowing (OK)
    - Borrow after move (error)
    - Borrow in function signature
    - Borrow with method calls
    - Interior mutability (Cell, RefCell)
    - Atomic types (special case)
  
  - Lifetime violations:
    - Return local reference (error)
    - Return borrow outliving owner (error)
    - Struct field lifetime (error)
    - Function signature lifetime mismatch
    - Generic lifetime constraint violation
    - Lifetime subtyping errors
    - Lifetime variance errors
    - Lifetime elision failures
    - Lifetime annotation missing
    - Lifetime too restrictive

- [ ] Edge Cases (20 tests)
  - Borrow in conditional branches
  - Borrow in loops (different iterations)
  - Borrow with early returns
  - Borrow with exceptions
  - Borrow with recursion
  - Borrow with closures
  - Borrow with iterators
  - Borrow with pattern matching
  - Borrow with enums (Option, Result)
  - Borrow with generics
  - Borrow with trait objects
  - Borrow with FFI (unsafe)
  - Borrow with inline assembly (unsafe)
  - Borrow with transmute (unsafe)
  - Borrow with pointer casts
  - Borrow with aliasing pointers
  - Borrow with shared state (Arc<Mutex<T>>)
  - Borrow with channels
  - Borrow with async/await
  - Borrow with Send/Sync bounds
```

**Validation:**
- **Borrow checker test suite:** 50/50 passing
- **No false positives:** Code review + user testing
- **Error message quality:** >90% "helpful" (user survey)

---

### 3. Concurrency Safety (40 tests) - Phase 4

**When:** Phase 4 (Concurrency & Parallelism)

```
Thread Safety:
- [ ] Data Race Detection (15 tests)
  - Shared mutable state (error without Mutex)
  - Mutex-protected state (OK)
  - RwLock (read-write lock)
  - Atomic operations
  - Channel send/receive
  - Arc<T> sharing (immutable)
  - Arc<Mutex<T>> sharing (mutable)
  - Send trait violations (compile error)
  - Sync trait violations (compile error)
  - Interior mutability (Cell, RefCell) not Send
  - Raw pointer not Send (unless marked unsafe)
  - Non-atomic reference counting (Rc) not Send
  - Multiple threads accessing shared data
  - False sharing (performance test)
  - Lock-free data structures

- [ ] Deadlock Prevention (10 tests)
  - Lock ordering (prevent circular wait)
  - Deadlock with 2 mutexes
  - Deadlock with 3+ mutexes
  - Deadlock detection (debug mode)
  - Livelock (spinning threads)
  - Starvation (unfair scheduling)
  - Priority inversion
  - Lock-free algorithms (no deadlock)
  - Channel deadlock (recv without send)
  - Thread pool deadlock

- [ ] Send/Sync Enforcement (15 tests)
  - Send types (can transfer ownership)
  - Sync types (can share references)
  - Not Send: Rc, Cell, raw pointers
  - Not Sync: RefCell, raw pointers
  - Send + Sync: Arc, Mutex, AtomicInt
  - Generic bounds (T: Send + Sync)
  - Trait object Send/Sync
  - Closure Send/Sync
  - Async task Send/Sync
  - Thread spawn requires Send
  - Arc<T> requires T: Send + Sync
  - Mutex<T> requires T: Send
  - RwLock<T> requires T: Send + Sync
  - Channel<T> requires T: Send
  - Custom types (derive Send/Sync)
```

**Validation:**
- **ThreadSanitizer (TSan):** 0 errors
- **Helgrind (valgrind):** 0 data races
- **Manual review:** All concurrent code audited

---

### 4. Error Handling (30 tests) - Phase 6

**When:** Phase 6 (Error Handling Improvements)

```
Robustness:
- [ ] Result Type (10 tests)
  - Ok variant
  - Error variant
  - Pattern matching (match, if-let)
  - unwrap (panic on error)
  - unwrap_or (default value)
  - expect (panic with message)
  - is_ok, is_error
  - map, and_then (combinators)
  - Result chaining
  - Early return with ?

- [ ] Error Propagation (10 tests)
  - ? operator (simple case)
  - ? in nested functions
  - ? with different error types
  - ? with error conversion
  - ? in loops
  - ? with early returns
  - ? with async/await
  - ? with closures
  - Error context (error wrapping)
  - Error backtrace

- [ ] Stack Traces (10 tests)
  - Simple panic stack trace
  - Nested function stack trace
  - Recursive function stack trace
  - Stack trace with inlined functions
  - Stack trace with generic functions
  - Stack trace with closures
  - Stack trace with async/await
  - Stack trace with threads
  - Stack trace with FFI (C boundary)
  - Stack trace formatting
```

**Validation:**
- **All error scenarios tested** (10+ categories)
- **Stack traces accurate** (manual verification)
- **Error messages helpful** (user survey >90%)

---

## 🟡 P1: Functional Tests (180 tests, 36%)

**Rule:** P1 tests must pass before 1.0 release, but can defer to later phases  
**Why P1?** Core features, but failures don't cause crashes/security issues

### 5. Performance (40 tests) - Phase 2

**When:** Phase 2 (SAO+FBO Integration)

```
SAO Optimization:
- [ ] Baseline benchmarks (10 tests)
  - fibonacci (recursive)
  - ackermann (deep recursion)
  - prime sieve
  - matrix multiplication
  - sorting (quicksort, mergesort)
  - String operations
  - Hash table operations
  - Tree traversal
  - Graph search (BFS, DFS)
  - Vector operations

- [ ] SAO-optimized benchmarks (10 tests)
  - Same as baseline
  - Target: >2x speedup (90% of tests)
  - fibonacci: 84ms → <40ms (goal: <30ms)

- [ ] Regression tests (10 tests)
  - Ensure no slowdowns
  - 100% tests ≥ baseline
  - Check compile time overhead (<30%)
  - Check binary size overhead (<20%)

- [ ] Edge cases (10 tests)
  - Recursive with side effects (no SAO)
  - Impure functions (no SAO)
  - Functions with I/O (no SAO)
  - Functions with global state (no SAO)
  - Mixed pure/impure
  - Microbenchmarks (cache effects)
  - SIMD vectorization
  - Loop unrolling
  - Function inlining
  - Tail call optimization
```

**Validation:**
- **Performance targets met** (fibonacci <40ms)
- **90%+ benchmarks** show >2x improvement
- **No regressions** (100% ≥ baseline)

---

### 6. Stdlib Functionality (120 tests) - Phase 5

**When:** Phase 5 (Standard Library Integration)

```
Module Tests:
- [ ] std/math (30 tests)
  - Basic ops (abs, min, max, clamp)
  - Power/root (pow, sqrt, cbrt)
  - Trig (sin, cos, tan, asin, acos, atan)
  - Exponential (exp, log, log2, log10)
  - Rounding (floor, ceil, round, trunc)
  - Random (Random::new, range)
  - Constants (PI, E, TAU)
  - Edge cases (NaN, Infinity, -0)
  - Precision (floating point accuracy)
  - FFI to libm (all functions)

- [ ] std/string (30 tests)
  - Basic (length, char_at, concat, equals)
  - Split/join (split, join, trim)
  - Search (contains, starts_with, ends_with)
  - Replace (replace, replace_all)
  - Substring (substring, slice)
  - Case (to_uppercase, to_lowercase)
  - Repeat (repeat)
  - UTF-8 (multi-byte characters)
  - Edge cases (empty string, very long string)
  - Performance (large strings)

- [ ] std/collections (30 tests)
  - Vector (10 tests):
    - new, append, get, set, length, capacity
    - push_front, pop_front, push_back, pop_back
    - sort, reverse, dedup
    - binary_search, contains
    - iter, map, filter, fold
    - Edge: empty, single element, large (10k+ elements)
  
  - HashMap (10 tests):
    - new, put, get, contains, remove, size
    - iter, keys, values
    - Collision handling
    - Resize (dynamic growth)
    - Edge: empty, full, hash collisions
  
  - Other (10 tests):
    - Set (union, intersection, difference)
    - LinkedList (push, pop, reverse)
    - Queue/Stack (FIFO/LIFO)
    - Deque (double-ended queue)

- [ ] std/fs (20 tests)
  - File I/O (read_file, write_file, append_file)
  - Directory (list_dir, mkdir, rmdir)
  - Path (join, basename, dirname, extension)
  - Metadata (size, modified_time, is_file, is_dir)
  - Permissions (chmod, chown)
  - Symlinks (readlink, symlink)
  - Error handling (file not found, permission denied)
  - Edge cases (empty file, large file >1GB)

- [ ] std/net (10 tests)
  - TCP (listen, connect, send, receive)
  - UDP (bind, send_to, recv_from)
  - Error handling (connection refused, timeout)
  - IPv4/IPv6
  - Edge: large data (>1MB), many connections
```

**Validation:**
- **150+ stdlib tests passing**
- **API coverage:** 100% (all functions tested)
- **Real-world demo app** (500+ LOC, uses 5+ modules)

---

### 7. FFI Integration (20 tests) - Phase 1 (parallel)

**When:** Week 1 (FFI parser integration, parallel with Phase 1)

```
C Library Integration:
- [ ] dlopen/dlclose (4 tests)
  - Open libc (standard library)
  - Open missing library (error handling)
  - Multiple libraries
  - Close library

- [ ] dlsym (4 tests)
  - Resolve strlen symbol
  - Resolve missing symbol (error)
  - Multiple symbols
  - Type safety (function pointer cast)

- [ ] Type Marshalling (5 tests)
  - Numeric types (int, long, double)
  - String types (char*, string)
  - Boolean types (int → bool)
  - Pointer types (void*)
  - Struct types (padding, alignment)

- [ ] Callbacks (4 tests)
  - C function calls MELP function
  - Callback with arguments
  - Callback with return value
  - Multiple callbacks

- [ ] Real-World Examples (3 tests)
  - HTTP server (libc sockets)
  - SQLite database (libsqlite3)
  - C++ wrapper (std::vector via opaque pointer)
```

**Validation:**
- **20/20 FFI tests passing**
- **3 real-world examples working** (HTTP, SQLite, C++)

---

## 🟢 P2: Quality Tests (120 tests, 24%)

**Rule:** P2 tests nice to have but not blocking 1.0 release  
**Why P2?** Edge cases, tooling, advanced features

### 8. Package Manager (60 tests) - Phase 7

**When:** Phase 7 (Package Manager)

```
melp CLI:
- [ ] Package operations (20 tests)
  - melp init
  - melp add <package>
  - melp remove <package>
  - melp install
  - melp update
  - melp publish
  - melp search
  - Error handling (network failure, auth failure)

- [ ] Dependency Resolution (20 tests)
  - Simple dependency (A → B)
  - Transitive dependency (A → B → C)
  - Diamond dependency (A → B,C → D)
  - Conflict resolution
  - Version constraints (^1.0, ~1.2, =1.2.3)
  - Lock file generation
  - Lock file validation
  - Dependency cycles (error)

- [ ] Build Integration (20 tests)
  - Compile with dependencies
  - Incremental builds
  - Cache validation
  - Package path resolution
  - Import from packages
```

---

### 9. IDE Support (40 tests) - Phase 8 (Post-1.0)

**When:** Phase 8 (IDE Support, after 1.0)

```
LSP Features:
- [ ] Syntax highlighting (5 tests)
- [ ] Autocomplete (10 tests)
- [ ] Go-to-definition (10 tests)
- [ ] Find references (5 tests)
- [ ] Rename refactoring (5 tests)
- [ ] Error diagnostics (5 tests)
```

---

### 10. Edge Cases (20 tests) - Ongoing

**When:** Throughout all phases

```
Corner Cases:
- [ ] Empty inputs (5 tests)
- [ ] Very large inputs (5 tests)
- [ ] Unicode edge cases (5 tests)
- [ ] Platform-specific (5 tests)
```

---

## 📈 Test Development Timeline

| Phase | Week | P0 Tests | P1 Tests | P2 Tests | Total New | Cumulative |
|-------|------|----------|----------|----------|-----------|------------|
| **Baseline** | 0 | 0 | 92 | 0 | 0 | 92 |
| **FFI Parser** | 1 | 0 | 20 | 0 | 20 | 112 |
| **Phase 1** | 1-3 | 30 | 0 | 0 | 30 | 142 |
| **Phase 2** | 4-7 | 0 | 40 | 0 | 40 | 182 |
| **Phase 3** | 8-17 | 100 | 0 | 0 | 100 | 282 |
| **Phase 4** | 18-25 | 40 | 0 | 0 | 40 | 322 |
| **Phase 5** | 26-31 | 0 | 120 | 0 | 120 | 442 |
| **Phase 6** | 32-33 | 30 | 0 | 0 | 30 | 472 |
| **Phase 7** | 34-41 | 0 | 0 | 60 | 60 | 532 |
| **Polish** | 42-45 | 0 | 0 | 20 | 20 | 552 |

**Final Count:** 552 tests (exceeded 500 target ✅)

---

## 🎯 Validation Rules

### P0 (Safety-Critical) - MUST PASS 100%
```
Rule: If ANY P0 test fails → BLOCK phase progression
Exception: None
Reason: Memory safety, data races, security = non-negotiable
```

### P1 (Functional) - MUST PASS >95%
```
Rule: If >5% P1 tests fail → REVIEW phase progression
Exception: Known issues documented + workaround available
Reason: Core functionality = critical for usability
```

### P2 (Quality) - SHOULD PASS >80%
```
Rule: P2 tests can have known issues for 1.0
Exception: Defer to post-1.0 if timeline critical
Reason: Edge cases, tooling = nice to have
```

---

## 🔍 Test Quality Metrics

### Code Coverage (gcov/lcov)
```
Target: >75% overall
- Phase 1 (Heap): >80% (memory-critical)
- Phase 3 (Ownership): >85% (safety-critical)
- Phase 4 (Concurrency): >80% (thread-safety-critical)
- Other phases: >70%
```

### Error Message Quality
```
Target: >90% "helpful" (user survey)
- Test method: 20+ developers review error messages
- Rating scale: 1-5 (helpful, actionable, clear)
- Threshold: Average ≥4.5
```

### Performance Regression
```
Target: 0% regressions
- All benchmarks must be ≥ baseline
- Compile time overhead <30%
- Binary size overhead <20%
```

---

## 📊 Test Automation

### CI/CD Pipeline
```
On every commit:
1. Run P0 tests (MUST pass)
2. Run P1 tests (SHOULD pass >95%)
3. Run P2 tests (nice to have)
4. Generate coverage report
5. Run sanitizers (ASan, TSan, MSan)
6. Check performance (no regressions)
```

### Nightly Tests
```
Every night:
1. Run full test suite (500+ tests)
2. Run long-running tests (>1 hour each)
3. Run stress tests (10k+ iterations)
4. Run fuzzing (AFL, libFuzzer)
5. Generate test report
```

---

## 🚀 Next Steps

1. **Create test infrastructure:**
   - [ ] tests/p0_safety/ (200 tests)
   - [ ] tests/p1_functional/ (180 tests)
   - [ ] tests/p2_quality/ (120 tests)

2. **Automate test runners:**
   - [ ] scripts/run_p0_tests.sh
   - [ ] scripts/run_p1_tests.sh
   - [ ] scripts/run_p2_tests.sh
   - [ ] scripts/run_all_tests.sh

3. **Set up CI/CD:**
   - [ ] GitHub Actions workflow
   - [ ] Test result dashboard
   - [ ] Coverage reporting

---

**Document Status:** Active  
**Owner:** QA Team  
**Next Review:** After each phase completion  
**Version:** 1.0
