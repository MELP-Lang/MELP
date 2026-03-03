# Concurrency - Preparation Reading List

**Purpose:** Early preparation for Phase 4 (Concurrency & Parallelism)  
**Timeline:** Week 10-15 (during Phase 3 - Ownership System)  
**Total Time:** 60-70 hours (8-9 days, spread over 4 weeks)  
**Goal:** Reduce Phase 4 risk from 70% → 40%, save 2-3 weeks

---

## 📚 Reading Schedule Overview

| Week | Days | Topic | Hours | Deliverable |
|------|------|-------|-------|-------------|
| **Week 10-11** | 1-3 | Event loop design | 15h | Design notes (1,500 words) |
| **Week 10-11** | 4-5 | libuv architecture | 10h | Architecture analysis |
| **Week 12-13** | 1-3 | State machine transformation | 12h | Algorithm sketch |
| **Week 12-13** | 4-5 | C++20 coroutines | 8h | LLVM IR patterns |
| **Week 14-15** | 1-3 | Thread safety (Crossbeam) | 15h | Lock-free patterns |
| **Week 14-15** | 4-5 | Prototype event loop | 15h | Working prototype (500 LOC) |

**Total:** ~75 hours = 9 days (parallel with Phase 3)

---

## 🎯 Phase 1: Event Loop Design (Week 10-11)

### 1. **"The Design of the Tokio Runtime"**

**Link:** https://tokio.rs/blog/2019-10-scheduler

**What is it?**
- Deep dive into Tokio's work-stealing scheduler
- Proven architecture (used in production by thousands of Rust projects)
- Explains trade-offs between single-threaded vs multi-threaded runtimes

**Why read this?**
- **Best event loop design reference** (battle-tested)
- Work-stealing = optimal load balancing
- Shows how to handle millions of concurrent tasks

**Time:** 2-3 hours

**Key Sections:**
1. **Work-Stealing Scheduler:** How tasks are distributed across threads
2. **Task Queue Design:** Lock-free queues (MPMC, SPSC)
3. **Reactor:** Epoll/kqueue/IOCP integration
4. **Executor:** How futures are polled

**Key Takeaways:**
- [ ] Work-stealing algorithm (Chase-Lev deque)
- [ ] Task queue design (global queue + per-thread queues)
- [ ] Reactor pattern (event notification)
- [ ] Executor pattern (task execution)

**Notes Template:**
```markdown
## Tokio Scheduler Notes

### Work-Stealing Algorithm:
1. Each thread has local queue
2. When idle, steal from other threads
3. Global queue as fallback

### Task Queue Design:
- Per-thread: LIFO (cache-friendly)
- Global: FIFO (fairness)
- Stealing: FIFO (reduce contention)

### Key Data Structures:
- ...
```

---

### 2. **libuv Design Overview**

**Link:** http://docs.libuv.org/en/v1.x/design.html

**What is it?**
- C-based event loop (used by Node.js)
- Cross-platform (Linux/macOS/Windows)
- Simpler than Tokio (single-threaded by default)

**Why study this?**
- **C-based = easier to port to MELP** (vs Rust Tokio)
- Production-proven (Node.js = billions of req/sec globally)
- Shows how to integrate with OS (epoll, kqueue, IOCP)

**Time:** 3-4 hours

**Key Concepts:**
1. **Event Loop:** Single-threaded, non-blocking I/O
2. **Handles:** Abstractions for I/O resources (TCP, UDP, timers)
3. **Requests:** Asynchronous operations
4. **Thread Pool:** For blocking operations (file I/O, DNS)

**Key Takeaways:**
- [ ] Event loop structure (loop.c analysis)
- [ ] Platform-specific backends (epoll vs kqueue vs IOCP)
- [ ] Handle lifecycle (init, start, stop, close)
- [ ] Thread pool design (for blocking ops)

**Action Items:**
- [ ] Clone repo: `git clone https://github.com/libuv/libuv`
- [ ] Read docs: `docs/src/design.rst`
- [ ] Browse code: `src/unix/core.c` (Linux/macOS), `src/win/core.c` (Windows)
- [ ] Trace example: Run `test/test-tcp-echo-server.c` with debugger

---

### 3. **Async/Await Comparison: Rust vs JavaScript vs C++20**

**Resources:**
- Rust: https://rust-lang.github.io/async-book/
- JavaScript: MDN Web Docs (Promises + async/await)
- C++20: https://en.cppreference.com/w/cpp/language/coroutines

**Why compare?**
- Understand different approaches (trade-offs)
- Rust = zero-cost abstraction (best performance)
- JavaScript = simple API (best ergonomics)
- C++20 = low-level (closest to LLVM IR)

**Time:** 2-3 hours

**Comparison Matrix:**

| Feature | Rust | JavaScript | C++20 | MELP (Goal) |
|---------|------|------------|-------|-------------|
| **Syntax** | `async fn`, `await` | `async function`, `await` | Coroutines | TBD (similar to Rust) |
| **Runtime** | Explicit (Tokio) | Implicit (V8) | None (library) | Custom (like Tokio) |
| **Overhead** | Zero-cost | V8 engine | Minimal | Zero-cost (goal) |
| **Cancellation** | Drop future | AbortController | co_return | TBD |

**Key Takeaways:**
- [ ] State machine transformation (async → state machine)
- [ ] Future/Promise representation
- [ ] Polling vs callback (Rust = polling, JS = callback)
- [ ] Cancellation mechanisms

---

## 🔬 Phase 2: State Machine Transformation (Week 12-13)

### 4. **"Async/Await in C++20 Coroutines"**

**Link:** https://en.cppreference.com/w/cpp/language/coroutines

**What is it?**
- C++20 coroutines = compiler-generated state machines
- Low-level API (closest to LLVM IR)
- Shows how compilers implement async/await

**Why study this?**
- **Most relevant for MELP implementation** (C++ → LLVM IR)
- See compiler-generated code (co_await = state machine)
- Understand suspend points (where to save state)

**Time:** 4-5 hours

**Key Concepts:**
1. **Coroutine Frame:** State storage (local variables, resume point)
2. **Suspend Points:** `co_await`, `co_yield`, `co_return`
3. **Awaitable Type:** What can be `co_await`ed
4. **Promise Type:** Coroutine lifecycle management

**Key Takeaways:**
- [ ] State machine structure (frame layout)
- [ ] Suspend/resume mechanism (setjmp/longjmp-like)
- [ ] Memory management (frame allocation/deallocation)
- [ ] LLVM IR patterns (coroutine intrinsics)

**Action Items:**
- [ ] Write simple C++20 coroutine
- [ ] Compile with `-emit-llvm`: `clang++ -std=c++20 -S -emit-llvm test.cpp`
- [ ] Study LLVM IR output (see `@llvm.coro.*` intrinsics)
- [ ] Note suspend points (where state is saved)

**Example:**
```cpp
// C++20 coroutine
std::task<int> compute() {
    int x = 1;
    co_await some_async_op();  // Suspend point 1
    int y = 2;
    co_return x + y;
}

// Pseudo-LLVM IR (simplified)
// State machine:
// State 0: Initial (compute x)
// State 1: After suspend (compute y)
define @compute_state_machine(i32 %state, %frame* %f) {
    switch %state {
        case 0: goto initial
        case 1: goto after_suspend
    }
initial:
    store i32 1, %f.x
    call @some_async_op()
    store i32 1, %f.state  // Save resume point
    ret suspend
after_suspend:
    load i32 %f.x
    store i32 2, %f.y
    add %f.x, %f.y
    ret result
}
```

---

### 5. **"Rust Async Book: Async in Depth"**

**Link:** https://rust-lang.github.io/async-book/02_execution/01_chapter.html

**What is it?**
- Official Rust async programming guide
- Explains Future trait (Rust's async primitive)
- Covers executors, wakers, pinning

**Why read this?**
- Rust = best async/await design (zero-cost)
- Future trait = simple, composable
- Shows how to build executor from scratch

**Time:** 3-4 hours

**Key Chapters:**
1. **Chapter 2:** Execution (Executor, Waker, Future)
2. **Chapter 3:** Async/Await syntax
3. **Chapter 4:** Pinning (advanced, optional)

**Key Concepts:**
1. **Future Trait:**
   ```rust
   trait Future {
       type Output;
       fn poll(self: Pin<&mut Self>, cx: &mut Context) -> Poll<Self::Output>;
   }
   ```
   - `poll`: Try to make progress, return `Ready` or `Pending`
   - `Context`: Contains Waker (for notification)

2. **Executor:**
   - Manages task queue
   - Polls futures when ready
   - Handles waker notifications

3. **Waker:**
   - Notification mechanism
   - Tells executor "future is ready to make progress"

**Key Takeaways:**
- [ ] Future trait design (simple API)
- [ ] Polling model (vs callback hell)
- [ ] Waker mechanism (event notification)
- [ ] Executor architecture (task scheduling)

**Action Items:**
- [ ] Build simple executor (from scratch, ~200 LOC)
- [ ] Test with basic futures (timer, channel)
- [ ] Understand waker flow (when/how waker is called)

---

## 🧵 Phase 3: Thread Safety (Week 14-15)

### 6. **"Crossbeam: Concurrent Data Structures" (Rust)**

**Link:** https://github.com/crossbeam-rs/crossbeam

**What is it?**
- Lock-free data structures library (Rust)
- Channels, queues, epoch-based memory reclamation
- Production-quality implementations

**Why study this?**
- **Best reference for lock-free algorithms**
- Shows how to implement Send/Sync correctly
- Patterns for safe concurrency

**Time:** 4-5 hours (code review)

**Key Modules:**
1. **crossbeam-channel:** MPMC channels (lock-free)
2. **crossbeam-queue:** Lock-free queues (SPSC, MPMC)
3. **crossbeam-epoch:** Memory reclamation (avoid ABA problem)
4. **crossbeam-utils:** Thread scope, parker/unparker

**Key Algorithms:**
- [ ] MPMC channel (lock-free, based on Dmitry Vyukov's design)
- [ ] Chase-Lev deque (work-stealing queue)
- [ ] Epoch-based reclamation (safe memory management)

**Action Items:**
- [ ] Clone repo: `git clone https://github.com/crossbeam-rs/crossbeam`
- [ ] Read code: `crossbeam-channel/src/flavors/array.rs` (bounded channel)
- [ ] Read paper: "Simple, Fast, and Practical Non-Blocking and Blocking Concurrent Queue Algorithms" (Michael & Scott)
- [ ] Extract patterns for MELP

**Code Pattern to Extract:**
```rust
// Lock-free push (from crossbeam-queue)
pub fn push(&self, value: T) -> Result<(), T> {
    let backoff = Backoff::new();
    loop {
        let tail = self.tail.load(Ordering::Acquire);
        let slot = &self.buffer[tail & self.mask];
        
        let stamp = slot.stamp.load(Ordering::Acquire);
        if stamp == tail {
            // Try to claim this slot
            if self.tail.compare_exchange_weak(
                tail, tail + 1, Ordering::SeqCst, Ordering::Relaxed
            ).is_ok() {
                // Successfully claimed, write value
                slot.value.store(Some(value), Ordering::Release);
                slot.stamp.store(tail + 1, Ordering::Release);
                return Ok(());
            }
        } else {
            backoff.spin();
        }
    }
}
```

---

### 7. **"The Art of Multiprocessor Programming" (Chapters 7-10)**

**Book:** Herlihy & Shavit, ISBN: 978-0123705914

**What is it?**
- Comprehensive textbook on concurrent programming
- Theory + practice (algorithms + correctness proofs)
- Covers mutexes, semaphores, monitors, lock-free structures

**Why read this?**
- **Theoretical foundation** (understand WHY algorithms work)
- Correctness conditions (linearizability, serializability)
- Design patterns for concurrent data structures

**Time:** 10-15 hours (dense textbook)

**Key Chapters:**
- **Chapter 7:** Spin Locks and Contention
- **Chapter 8:** Monitors and Blocking Synchronization
- **Chapter 9:** Linked Lists: Lock-Based vs Lock-Free
- **Chapter 10:** Concurrent Queues and Stacks

**Key Concepts:**
1. **Linearizability:** Operations appear atomic
2. **Lock-free:** At least one thread makes progress
3. **Wait-free:** All threads make progress (strongest guarantee)
4. **ABA Problem:** Memory reuse causing false CAS success

**Key Takeaways:**
- [ ] Lock design (test-and-set, test-and-test-and-set, backoff)
- [ ] Lock-free algorithms (linked list, queue, stack)
- [ ] Memory ordering (sequential consistency vs relaxed)
- [ ] Correctness arguments (linearization points)

**Notes Template:**
```markdown
## Multiprocessor Programming Notes

### Lock Design:
- Spin lock: Fast for short critical sections
- Backoff: Reduce contention (exponential backoff)
- Queue locks (MCS): Fairness + scalability

### Lock-Free Queue (Michael & Scott):
1. Two pointers: head, tail
2. CAS on head for dequeue
3. CAS on tail for enqueue
4. Handle race conditions: ...

### ABA Problem:
- Problem: Pointer reuse breaks CAS
- Solution: Epoch-based reclamation OR tagged pointers
```

---

### 8. **"Rust Nomicon: Send and Sync"**

**Link:** https://doc.rust-lang.org/nomicon/send-and-sync.html

**What is it?**
- Explains Rust's Send and Sync marker traits
- Compile-time thread safety enforcement
- Rules for safe concurrency

**Why read this?**
- **Critical for MELP's thread safety design**
- Send = can transfer ownership between threads
- Sync = can share references between threads
- Compiler-enforced = zero runtime cost

**Time:** 1-2 hours

**Key Concepts:**
1. **Send Trait:**
   - Types that can be moved to another thread
   - Most types are Send (except raw pointers, Rc<T>)
   - Rule: `T: Send` if all fields are `Send`

2. **Sync Trait:**
   - Types that can be shared between threads (&T is Send)
   - Most types are Sync if immutable
   - Not Sync: Cell<T>, RefCell<T> (interior mutability without locks)

3. **Auto Traits:**
   - Compiler automatically implements Send/Sync if safe
   - Manual implementation requires `unsafe`

**Key Takeaways:**
- [ ] Send/Sync rules (what types are thread-safe)
- [ ] How to check Send/Sync in MELP compiler
- [ ] Error messages for thread safety violations

**Example:**
```rust
// Rc<T> is NOT Send (reference counting without atomics)
let rc = Rc::new(5);
thread::spawn(move || {
    println!("{}", rc);  // ERROR: Rc<i32> is not Send
});

// Arc<T> IS Send (atomic reference counting)
let arc = Arc::new(5);
thread::spawn(move || {
    println!("{}", arc);  // OK: Arc<i32> is Send
});
```

**For MELP:**
```
Rule: Type T is Send if:
1. All fields are Send
2. No raw pointers (or marked unsafe)
3. No non-atomic shared state (Rc, Cell)

Rule: Type T is Sync if:
1. &T is Send (can share references)
2. No interior mutability without synchronization
```

---

## 🔨 Phase 4: Prototype (Week 14-15, Days 4-5)

### Goal: Simple Event Loop Prototype (500 LOC)

**Objectives:**
- Prove work-stealing scheduler works
- Test with 10+ async tasks
- Validate design before Phase 4

**What to Implement:**
1. **Task Queue (150 LOC)**
   - Global queue (MPMC, lock-based for simplicity)
   - Per-thread queue (SPSC, lock-free)
   - Work-stealing (Chase-Lev deque)

2. **Executor (200 LOC)**
   - Thread pool (4 threads)
   - Task scheduling (poll ready futures)
   - Waker implementation (notify when ready)

3. **Future Trait (100 LOC)**
   - Simple Future<T> trait
   - poll(context) -> Ready(T) | Pending
   - Example futures (timer, channel recv)

4. **Reactor (50 LOC)**
   - Epoll/kqueue wrapper (single-threaded)
   - Register I/O events
   - Notify executor when ready

**Test Cases (10 minimum):**
```cpp
// Test 1: Simple async task
Future<int> compute() {
    co_await sleep(100ms);
    co_return 42;
}

// Test 2: Parallel tasks
async fn main() {
    auto fut1 = compute();
    auto fut2 = compute();
    auto [a, b] = co_await join(fut1, fut2);
    assert(a == 42 && b == 42);
}

// Test 3: Work-stealing
async fn main() {
    // Spawn 1000 tasks, ensure all complete
    vector<Future<int>> tasks;
    for (int i = 0; i < 1000; i++) {
        tasks.push_back(compute());
    }
    auto results = co_await join_all(tasks);
    assert(results.size() == 1000);
}

// ... 7 more tests (channels, timers, I/O)
```

**Deliverable:**
- [ ] Working prototype (~500 LOC)
- [ ] 10+ tests passing
- [ ] Benchmark: 10,000 tasks/sec throughput
- [ ] Design document (1,500 words):
  - Event loop architecture
  - Work-stealing algorithm
  - Task queue design
  - Known limitations

---

## 📊 Success Metrics

### Research Phase (Week 10-13)
- [ ] 8 resources read/studied (100%)
- [ ] Notes written (4,000+ words total)
- [ ] Key algorithms identified (work-stealing, state machine, lock-free)
- [ ] Design document draft (1,500 words)

### Prototype Phase (Week 14-15)
- [ ] Prototype working (500 LOC)
- [ ] 10+ tests passing
- [ ] Work-stealing validated (benchmark shows even load distribution)
- [ ] Known limitations documented

### Overall Goal
- [ ] **Phase 4 risk reduced from 70% → 40%**
- [ ] **2-3 weeks saved** (prototype = head start)
- [ ] Team confidence high (proven architecture)

---

## 🚨 Red Flags (When to Ask for Help)

**If you encounter:**
1. **Lock-free algorithms too complex** → Use lock-based for MVP
2. **State machine transformation unclear** → Study C++20 LLVM IR more
3. **Prototype not working after 3 days** → Simplify to single-threaded first
4. **Work-stealing not balanced** → Check global queue fallback

**Don't hesitate to:**
- Ask ÜA for clarification
- Post questions on Rust/C++ forums
- Simplify scope (single-threaded runtime = good enough for MVP)

---

## 📝 Deliverables Checklist

**Week 10-11 (Event Loop Research):**
- [ ] `belgeler/concurrency_prep/tokio_analysis.md` (1,000 words)
- [ ] `belgeler/concurrency_prep/libuv_analysis.md` (800 words)
- [ ] `belgeler/concurrency_prep/async_comparison.md` (700 words)

**Week 12-13 (State Machine):**
- [ ] `belgeler/concurrency_prep/cpp20_coroutines.md` (1,000 words)
- [ ] `belgeler/concurrency_prep/rust_async_notes.md` (800 words)
- [ ] `belgeler/concurrency_prep/design_document.md` (1,500 words)

**Week 14-15 (Thread Safety + Prototype):**
- [ ] `belgeler/concurrency_prep/crossbeam_analysis.md` (1,000 words)
- [ ] `belgeler/concurrency_prep/multiprocessor_notes.md` (1,500 words)
- [ ] `belgeler/concurrency_prep/send_sync_rules.md` (500 words)
- [ ] `tools/event_loop_prototype/` (source code)
- [ ] `tools/event_loop_prototype/tests/` (10+ tests)
- [ ] `tools/event_loop_prototype/README.md` (usage + limitations)

**Final Report:**
- [ ] `belgeler/CONCURRENCY_PREP_REPORT.md` (2,500 words):
  - What was learned
  - Architecture chosen (work-stealing vs single-threaded)
  - Prototype results (benchmark data)
  - Confidence level for Phase 4
  - Estimated timeline adjustment (if any)

---

## 🎯 Expected Outcome

**Before Prep:**
- Phase 4 risk: 70% (high uncertainty)
- Estimated time: 6-8 weeks (could stretch to 10-12)
- Approach: "Pick a runtime library and integrate"

**After Prep:**
- Phase 4 risk: 40% (architecture validated)
- Estimated time: 6-8 weeks (confident, buffer for edge cases)
- Approach: "Custom work-stealing runtime, Tokio-inspired"
- **Bonus:** Working prototype = 2-3 weeks head start

**Total ROI:** 8-9 days prep = 2-3 weeks saved = **200-250% return on investment** 🎉

---

**Document Status:** Active  
**Owner:** YZ Agent 4  
**Timeline:** Week 10-15 (during Phase 3)  
**Next Review:** End of Week 15 (before Phase 4 starts)
