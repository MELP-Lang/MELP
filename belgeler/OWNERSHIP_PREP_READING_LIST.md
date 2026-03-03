# Ownership System - Preparation Reading List

**Purpose:** Early preparation for Phase 3 (Ownership System)  
**Timeline:** Week 4-7 (during Phase 2 - SAO+FBO integration)  
**Total Time:** 40-50 hours (5-6 days)  
**Goal:** Reduce Phase 3 risk from 60% → 30%, save 2-4 weeks

---

## 📚 Reading Schedule Overview

| Week | Days | Topic | Hours | Deliverable |
|------|------|-------|-------|-------------|
| **Week 4** | 1-2 | NLL + Visual explanation | 8h | Notes (500 words) |
| **Week 4** | 3-4 | Polonius research | 10h | Design notes (1,000 words) |
| **Week 4** | 5 | rustc-dev-guide | 5h | Implementation notes |
| **Week 5** | 1-2 | Formal semantics | 8h | Algorithm sketch |
| **Week 5** | 3-4 | Lightweight approach | 6h | Simplified design |
| **Week 5** | 5 | Mini-Rust code review | 4h | Code notes |
| **Week 6-7** | | Prototype | 15h | Working prototype (500 LOC) |

**Total:** ~56 hours = 7 days (with breaks)

---

## 🎯 Phase 1: Core Concepts (Week 4, Days 1-2)

### 1. **RFC 2094: Non-Lexical Lifetimes (NLL)**

**Link:** https://rust-lang.github.io/rfcs/2094-nll.html

**What is it?**
- Rust RFC (Request for Comments) explaining lifetime inference improvements
- Moves from lexical scopes → control flow graph analysis
- Critical for understanding modern borrow checker design

**Why read this?**
- Foundation of Rust's current borrow checker
- Shows evolution from simple → sophisticated
- Explains constraint-based approach (key for MELP)

**Time:** 2-3 hours

**Key Takeaways to Extract:**
- [ ] How lifetimes are inferred (constraint generation)
- [ ] Difference between lexical vs non-lexical lifetimes
- [ ] Examples of false positives (old borrow checker) vs fixed (NLL)
- [ ] Constraint solver approach

**Notes Template:**
```markdown
## RFC 2094 Notes

### Core Idea:
- Old: Lifetimes tied to lexical scopes
- New: Lifetimes tied to control flow

### Constraint Generation:
1. ...
2. ...

### Examples I Should Remember:
- ...
```

---

### 2. **"Understanding Rust's Borrow Checker" (Felix Klock Video)**

**Link:** https://www.youtube.com/watch?v=_jTc1BTFdIo (RustConf talk)

**What is it?**
- Visual explanation of borrow checker internals
- By Felix Klock (core Rust team, borrow checker maintainer)
- 45 min video + Q&A

**Why watch this?**
- Visual diagrams > text (easier to understand)
- Real-world examples with step-by-step analysis
- Covers common pitfalls

**Time:** 1 hour (video + note-taking)

**Key Takeaways:**
- [ ] How borrows are tracked (lifetime regions)
- [ ] What "use after move" actually means internally
- [ ] How mutable/immutable aliasing is checked
- [ ] Examples of error messages + how checker generates them

**Action Items:**
- [ ] Screenshot key diagrams
- [ ] Reproduce examples in Rust Playground
- [ ] Note down questions for later research

---

## 🔬 Phase 2: Deep Dive (Week 4, Days 3-5)

### 3. **Polonius: The Next-Generation Borrow Checker**

**Links:**
- GitHub: https://github.com/rust-lang/polonius
- Blog: https://blog.rust-lang.org/inside-rust/2023/10/06/polonius-update.html
- Paper (Draft): https://github.com/rust-lang/polonius/blob/master/book/src/SUMMARY.md

**What is it?**
- Complete rewrite of Rust's borrow checker
- Uses Datalog (declarative logic programming)
- Constraint-based approach (simpler than current implementation)

**Why study this?**
- **Most important resource for MELP implementation**
- Cleaner design than current rustc (easier to port)
- Datalog = declarative = easier to reason about
- Already has formal specification

**Time:** 4-5 hours (code + documentation)

**Key Takeaways:**
- [ ] Datalog rules (how constraints are expressed)
- [ ] Input facts (what information compiler provides)
- [ ] Output facts (what borrow checker produces)
- [ ] How to adapt Datalog → imperative code (for MELP)

**Action Items:**
- [ ] Clone repo: `git clone https://github.com/rust-lang/polonius`
- [ ] Read book: `polonius/book/`
- [ ] Run examples: `cargo test --all`
- [ ] Study key Datalog rules (in `polonius-engine/src/`)

**Code to Understand:**
```datalog
// Example Datalog rule from Polonius
// "A loan is live at a point if it's issued and not killed"
loan_live_at(L, P) :- 
    loan_issued_at(L, _),
    cfg_edge(P, _),
    !loan_killed_at(L, P).
```

**Translation to Imperative (for MELP):**
```cpp
bool is_loan_live_at(Loan loan, Point point) {
    return loan_issued_at(loan) && 
           !loan_killed_at(loan, point);
}
```

---

### 4. **rustc-dev-guide: Borrow Checker**

**Link:** https://rustc-dev-guide.rust-lang.org/borrow_check.html

**What is it?**
- Official Rust compiler development guide
- Explains current borrow checker implementation
- Code walkthroughs + architecture

**Why read this?**
- See real implementation (rustc source code)
- Understand data structures (Region, Loan, etc.)
- Learn from production-quality code

**Time:** 5-6 hours (guide + code browsing)

**Key Sections:**
1. **MIR (Mid-level IR):** How Rust represents code for analysis
2. **Region Inference:** How lifetimes are computed
3. **Constraint Generation:** How borrow rules become constraints
4. **Error Reporting:** How to generate helpful error messages

**Action Items:**
- [ ] Read guide: Focus on "How MIR Borrow Check Works"
- [ ] Browse code: `rustc_borrowck/src/` in Rust repo
- [ ] Trace one example: Pick simple Rust code, see MIR + constraints
- [ ] Note data structures: Region, Loan, Path, etc.

**Key Data Structures to Understand:**
```rust
// From rustc_borrowck
struct Loan {
    region: Region,
    kind: BorrowKind,  // Shared, Unique, Mut
    path: Path,        // What's being borrowed
}

struct Region {
    vid: RegionVid,    // Unique ID
    origin: RegionOrigin,
}
```

---

## 📖 Phase 3: Formal Foundation (Week 5, Days 1-4)

### 5. **"Oxide: The Essence of Rust" (Academic Paper)**

**Link:** https://arxiv.org/abs/1903.00982  
**Authors:** Aaron Weiss, Olek Gierczak, Daniel Patterson, Nicholas D. Matsakis, Amal Ahmed

**What is it?**
- Formal semantics of Rust's ownership system
- Mathematical proof of soundness (no data races)
- Core calculus (simplified Rust for proving theorems)

**Why read this?**
- Understand **why** borrow checker rules are correct
- See mathematical foundation (not just heuristics)
- Helps with edge cases (what if...?)

**Time:** 3-4 hours (dense paper)

**Key Sections:**
- Section 2: Informal overview (read this first!)
- Section 3: Oxide syntax (mini-Rust)
- Section 4: Typing rules (formal borrow checker)
- Section 7: Soundness theorem

**Don't Worry About:**
- Section 5-6: Operational semantics (too formal for implementation)
- Proofs (nice to have, not needed for MELP)

**Key Takeaways:**
- [ ] Core typing rules (how borrow checker formalizes rules)
- [ ] Lifetime annotations (how they're inferred)
- [ ] Uniqueness vs sharing (mutable vs immutable borrows)

**Notes Template:**
```markdown
## Oxide Paper Notes

### Core Typing Rule (Borrow):
- Γ ⊢ e : &'a T  means...
- Conditions for safe borrow: ...

### Soundness Theorem:
- "If program type-checks, then no data races"
- Key insight: ...
```

---

### 6. **"A Lightweight Formalism for Reference Lifetimes" (Paper)**

**Link:** Search on Google Scholar: "lightweight reference lifetimes" + Ralf Jung  
**Authors:** Ralf Jung, et al. (Appears in PLDI/POPL proceedings)

**What is it?**
- Simplified version of Rust's lifetime system
- Focuses on practical implementation
- Less formal than Oxide, more implementable

**Why read this?**
- **Perfect for MELP** - balance between theory and practice
- Shows simplified approach (good for MVP)
- Explains how to skip complex cases (for now)

**Time:** 2-3 hours

**Key Takeaways:**
- [ ] Simplified lifetime inference algorithm
- [ ] What to implement first (MVP)
- [ ] What to defer (advanced features)

---

## 🛠️ Phase 4: Implementation Guidance (Week 5, Day 5)

### 7. **Mini-Rust: A Tiny Rust Interpreter**

**Link:** https://github.com/Alexander-Shukaev/mini-rust

**What is it?**
- Minimal Rust implementation (~2,000 LOC total)
- Includes simple borrow checker (~500 LOC)
- Educational project (easy to understand)

**Why study this?**
- See complete implementation (small enough to read in 1 day)
- Working code > theory (can test immediately)
- Good starting point for MELP prototype

**Time:** 3-4 hours (code review)

**What to Focus On:**
- `src/borrow_checker.rs` (if Rust-based)
- Or equivalent file (if C++ implementation)
- How borrows are tracked (data structures)
- How errors are reported

**Action Items:**
- [ ] Clone repo: `git clone https://github.com/Alexander-Shukaev/mini-rust`
- [ ] Build and run: Follow README instructions
- [ ] Trace example: Step through borrow checker with debugger
- [ ] Extract pattern: Note algorithm flow for MELP

**Code Pattern to Extract:**
```cpp
// Typical borrow checker structure (from mini-rust)
class BorrowChecker {
    map<Variable, Ownership> ownership_map;
    set<Borrow> active_borrows;
    
    void check_expression(Expr* expr) {
        if (MoveExpr* move = dynamic_cast<MoveExpr*>(expr)) {
            // Check ownership, mark variable as moved
        } else if (BorrowExpr* borrow = ...) {
            // Check aliasing rules, add to active_borrows
        }
    }
};
```

---

### 8. **C++ Lifetime Checker (Herb Sutter)**

**Link:** https://herbsutter.com/2018/09/20/lifetime-profile-v1-0-posted/

**What is it?**
- Proposal for C++ lifetime checker (similar to Rust)
- Different approach (C++ doesn't have ownership by default)
- Shows alternative design

**Why read this?**
- See how lifetime checking works without ownership types
- Learn from C++ community (large codebase constraints)
- Alternative algorithms (compare with Rust's approach)

**Time:** 2 hours

**Key Differences:**
| Aspect | Rust | C++ (Herb's Proposal) |
|--------|------|----------------------|
| Ownership | Explicit (core language) | Implicit (analysis) |
| Borrows | &T, &mut T syntax | Inferred from usage |
| Lifetimes | Part of type system | Analysis-time only |

**Key Takeaways:**
- [ ] How to infer ownership without explicit syntax
- [ ] Heuristics for detecting lifetime bugs
- [ ] Trade-offs: precision vs compile time

---

## 🔨 Phase 5: Prototype (Week 6-7)

### Goal: Simple Borrow Checker Prototype (500 LOC)

**Objectives:**
- Prove constraint-based approach works
- Test on 10+ simple examples
- Validate design before Phase 3

**What to Implement:**
1. **Ownership Tracking (200 LOC)**
   - Data structure: `map<Variable, OwnershipState>`
   - States: `Owned`, `Moved`, `Borrowed`, `MutBorrowed`
   - Operations: `move`, `borrow`, `borrow_mut`

2. **Borrow Rules (150 LOC)**
   - XOR rule: 1 mutable borrow OR N immutable borrows
   - No use-after-move
   - No use-after-free (scope-based)

3. **Lifetime Inference (100 LOC)**
   - Simple constraint generation
   - Constraint solving (union-find or graph-based)

4. **Error Reporting (50 LOC)**
   - Basic error messages
   - Point to conflicting borrows

**Test Cases (10 minimum):**
```rust
// Test 1: Simple move
fn test_move() {
    let x = vec![1, 2, 3];
    let y = x;  // move
    // println!("{:?}", x);  // ERROR: use after move
}

// Test 2: Immutable borrow
fn test_borrow() {
    let x = vec![1, 2, 3];
    let y = &x;  // borrow
    let z = &x;  // OK: multiple immutable borrows
    println!("{:?} {:?}", y, z);
}

// Test 3: Mutable borrow
fn test_borrow_mut() {
    let mut x = vec![1, 2, 3];
    let y = &mut x;  // mutable borrow
    // let z = &x;  // ERROR: cannot borrow as immutable while mutable borrow exists
    y.push(4);
}

// ... 7 more tests (lifetimes, nested borrows, etc.)
```

**Deliverable:**
- [ ] Working prototype (~500 LOC)
- [ ] 10+ tests passing
- [ ] Design document (1,000 words):
  - Algorithm overview
  - Data structures
  - Constraint generation approach
  - Known limitations (what's NOT implemented)

---

## 📊 Success Metrics

### Research Phase (Week 4-5)
- [ ] 8 resources read/studied (100%)
- [ ] Notes written (3,000+ words total)
- [ ] Key algorithms identified (constraint generation, solving)
- [ ] Design document draft (1,000 words)

### Prototype Phase (Week 6-7)
- [ ] Prototype working (500 LOC)
- [ ] 10+ tests passing
- [ ] Algorithm validated (constraint-based approach works)
- [ ] Known limitations documented

### Overall Goal
- [ ] **Phase 3 risk reduced from 60% → 30%**
- [ ] **2-4 weeks saved** (prototype = head start)
- [ ] Team confidence high (proven approach)

---

## 🚨 Red Flags (When to Ask for Help)

**If you encounter:**
1. **Datalog too complex** → Skip Polonius, focus on rustc-dev-guide
2. **Math too formal** → Skip Oxide proofs, focus on examples
3. **Prototype not working after 3 days** → Review Mini-Rust more carefully
4. **Constraint solver stuck** → Use union-find (simpler than graph-based)

**Don't hesitate to:**
- Ask ÜA for clarification
- Post questions on Rust forums (they're helpful!)
- Simplify scope (MVP borrow checker = good enough)

---

## 📝 Deliverables Checklist

**Week 4-5 (Research):**
- [ ] `belgeler/ownership_prep/nll_notes.md` (500 words)
- [ ] `belgeler/ownership_prep/polonius_notes.md` (1,000 words)
- [ ] `belgeler/ownership_prep/rustc_analysis.md` (800 words)
- [ ] `belgeler/ownership_prep/formal_semantics_notes.md` (700 words)
- [ ] `belgeler/ownership_prep/design_document.md` (1,500 words)

**Week 6-7 (Prototype):**
- [ ] `tools/borrow_checker_prototype/` (source code)
- [ ] `tools/borrow_checker_prototype/tests/` (10+ tests)
- [ ] `tools/borrow_checker_prototype/README.md` (usage + limitations)

**Final Report:**
- [ ] `belgeler/OWNERSHIP_PREP_REPORT.md` (2,000 words):
  - What was learned
  - Algorithm chosen
  - Prototype results
  - Confidence level for Phase 3
  - Estimated timeline adjustment (if any)

---

## 🎯 Expected Outcome

**Before Prep:**
- Phase 3 risk: 60% (high uncertainty)
- Estimated time: 6-8 weeks (could stretch to 10-12)
- Approach: "Figure it out as we go"

**After Prep:**
- Phase 3 risk: 30% (design validated)
- Estimated time: 6-8 weeks (confident, buffer for edge cases)
- Approach: "Constraint-based, Polonius-inspired"
- **Bonus:** Working prototype = 2-4 weeks head start

**Total ROI:** 5-6 days prep = 2-4 weeks saved = **300-400% return on investment** 🎉

---

**Document Status:** Active  
**Owner:** YZ Agent 3  
**Timeline:** Week 4-7 (during Phase 2)  
**Next Review:** End of Week 7 (before Phase 3 starts)
