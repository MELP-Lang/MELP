# YZ_207 GÖREVI: Try-Catch Exception Handling

**Tarih:** 21 Aralık 2025  
**Öncelik:** 🟢 Düşük (Result pattern zaten var, opsiyonel)  
**Süre Tahmini:** 5 gün  
**Bağımlılık:** YZ_206 (Result Type Pattern) ✅ TAMAMLANDI  
**Branch:** `try-catch_YZ_207`

---

## 📋 GÖREV TANIMI

Try-catch-finally exception handling mekanizmasını MELP diline ekle. Bu, Result pattern'e **alternatif** bir error handling yöntemidir. Result pattern zaten tamamlandığı için bu görev **opsiyonel**dır, ancak bazı geliştiriciler imperative exception handling'i tercih edebilir.

**Not:** Result pattern (YZ_206) zero-cost error handling sağlar (compile-time). Try-catch runtime overhead içerir (stack unwinding). Kullanıcı tercihine bağlı olarak ikisi de desteklenebilir.

---

## 🎯 HEDEFLER

### Phase 1: Try-Catch-Finally Syntax [2 gün]

**Amaç:** Try-catch-finally bloklarını parse et

**Yapılacaklar:**
- [ ] Try block parser: `try ... end_try`
- [ ] Catch clause parser: `catch identifier as exception_type ... end_catch`
- [ ] Finally block parser: `finally ... end_finally`
- [ ] Exception type definition
- [ ] AST structures:
  - `TryBlock`: body, catch_clauses[], finally_block
  - `CatchClause`: exception_var, exception_type, body
  - `FinallyBlock`: body

**Syntax:**
```pmpl
try
    -- code that may throw
    result<numeric, string> res = divide(10, 0)
    if is_error(res) then
        throw exception("Division error")
    end_if
catch e as DivisionException
    -- handle specific exception
    print("Division error: " + e.message)
catch e as Exception
    -- handle general exception
    print("Error: " + e.message)
finally
    -- cleanup code (always runs)
    cleanup_resources()
end_try
```

**Dosyalar:**
- `compiler/stage0/modules/exception/exception.h`
- `compiler/stage0/modules/exception/exception_parser.c`
- `compiler/stage0/modules/exception/try_catch_parser.c`

---

### Phase 2: Exception Types [1 gün]

**Amaç:** Exception type hierarchy oluştur

**Yapılacaklar:**
- [ ] Base Exception type
- [ ] Built-in exception types:
  - `DivisionByZeroException`
  - `NullPointerException`
  - `IndexOutOfBoundsException`
  - `TypeMismatchException`
  - `FileNotFoundException`
- [ ] Exception struct: message, stack_trace
- [ ] Exception inheritance (base class)

**Syntax:**
```pmpl
-- Define custom exception
exception CustomException extends Exception
    string custom_field
end_exception

-- Throw exception
throw DivisionByZeroException("Cannot divide by zero")
```

**Dosyalar:**
- `compiler/stage0/modules/exception/exception_types.h`
- `compiler/stage0/modules/exception/exception_types.c`

---

### Phase 3: Throw Statement [1 gün]

**Amaç:** Exception throwing mekanizması

**Yapılacaklar:**
- [ ] Throw statement parser: `throw exception_value`
- [ ] Exception object creation
- [ ] Stack unwinding preparation
- [ ] Control flow: Jump to nearest catch block
- [ ] AST structure: `ThrowStatement`

**Syntax:**
```pmpl
function divide(numeric a, numeric b) returns numeric
    if b == 0 then
        throw DivisionByZeroException("Division by zero")
    end_if
    return a / b
end_function
```

**Dosyalar:**
- `compiler/stage0/modules/exception/throw_parser.c`
- `compiler/stage0/modules/exception/exception.h` (update)

---

### Phase 4: Stack Unwinding [1 gün]

**Amaç:** Exception propagation ve stack cleanup

**Yapılacaklar:**
- [ ] Stack frame tracking
- [ ] Automatic cleanup (finally blocks)
- [ ] Resource cleanup (RAII-style)
- [ ] Exception propagation through call stack
- [ ] Catch matching: Find first matching catch clause

**Design:**
- Runtime maintains exception handler stack
- On throw: Walk up call stack
- Execute finally blocks in reverse order
- Match exception type with catch clauses
- If no match: Propagate to caller

**Dosyalar:**
- `compiler/stage0/modules/exception/stack_unwinding.c`
- `runtime/sto/exception_handler.c`

---

### Phase 5: LLVM Exception Handling [2 gün]

**Amaç:** LLVM IR codegen for exceptions

**Yapılacaklar:**
- [ ] LLVM `invoke` instruction (instead of call)
- [ ] LLVM `landingpad` instruction (catch handler)
- [ ] LLVM `resume` instruction (rethrow)
- [ ] Personality function: `__melp_personality_v0`
- [ ] Exception info struct: `{ i8*, i32 }`
- [ ] Type info for exception matching

**LLVM IR Example:**
```llvm
define i32 @test_try_catch() personality i8* @__melp_personality_v0 {
entry:
  %result = invoke i32 @divide(i32 10, i32 0)
    to label %normal unwind label %exception

normal:
  ret i32 %result

exception:
  %exc = landingpad { i8*, i32 }
    catch i8* @_ZTI14DivisionException
    catch i8* null
  %exc_ptr = extractvalue { i8*, i32 } %exc, 0
  %type_id = extractvalue { i8*, i32 } %exc, 1
  ; Handle exception
  br label %finally

finally:
  ; Cleanup code
  ret i32 -1
}
```

**Dosyalar:**
- `compiler/stage0/modules/exception/exception_codegen.c`
- `runtime/sto/personality.c`

---

### Phase 6: Integration & Testing [1 gün]

**Amaç:** Sistemi entegre et ve test et

**Yapılacaklar:**
- [ ] Makefile güncelle (exception modülü)
- [ ] Parser'a entegre et (statement parsing)
- [ ] Type checker entegrasyonu
- [ ] LLVM codegen entegrasyonu
- [ ] Test suite: 8-10 test dosyası
- [ ] Documentation: README + examples

**Test Cases:**
```pmpl
-- Test 1: Basic try-catch
function test_basic_try_catch() returns numeric
    try
        return divide(10, 0)  -- Throws
    catch e as DivisionByZeroException
        return -1
    end_try
end_function

-- Test 2: Multiple catch clauses
function test_multiple_catch() returns numeric
    try
        throw CustomException("test")
    catch e as CustomException
        return 1
    catch e as Exception
        return 2
    end_try
end_function

-- Test 3: Finally block
function test_finally() returns numeric
    numeric result = 0
    try
        result = 10
        throw Exception("error")
    catch e as Exception
        result = 20
    finally
        result = result + 5  -- Always runs
    end_try
    return result  -- Should return 25
end_function

-- Test 4: Nested try-catch
function test_nested() returns numeric
    try
        try
            throw Exception("inner")
        catch e as CustomException
            return 1
        end_try
        return 2  -- Not reached
    catch e as Exception
        return 3  -- Caught by outer
    end_try
end_function

-- Test 5: Rethrow
function test_rethrow() returns numeric
    try
        try
            throw Exception("error")
        catch e as Exception
            print("Caught, rethrowing")
            throw e  -- Rethrow same exception
        end_try
    catch e as Exception
        return 1
    end_try
end_function

-- Test 6: Exception propagation
function inner() returns numeric
    throw Exception("from inner")
    return 0
end_function

function outer() returns numeric
    try
        return inner()  -- Exception propagates
    catch e as Exception
        return -1
    end_try
end_function

-- Test 7: Finally without catch
function test_finally_only() returns numeric
    numeric result = 0
    try
        result = 10
        return result
    finally
        result = result + 5  -- Runs before return
    end_try
    -- Return value: 10 (finally can't change return)
end_function

-- Test 8: Exception with custom fields
exception CustomException extends Exception
    numeric error_code
end_exception

function test_custom_exception() returns numeric
    try
        CustomException e = CustomException("Error", 404)
        throw e
    catch e as CustomException
        return e.error_code  -- Should return 404
    end_try
end_function
```

**Dosyalar:**
- `tests/llvm/10_exception/test_basic_try_catch.mlp`
- `tests/llvm/10_exception/test_multiple_catch.mlp`
- `tests/llvm/10_exception/test_finally.mlp`
- `tests/llvm/10_exception/test_nested.mlp`
- `tests/llvm/10_exception/test_rethrow.mlp`
- `tests/llvm/10_exception/test_propagation.mlp`
- `tests/llvm/10_exception/test_finally_only.mlp`
- `tests/llvm/10_exception/test_custom_exception.mlp`
- `tests/llvm/10_exception/README.md`

---

## 🎯 TEST KRİTERLERİ

### Başarı Kriterleri

**Parser Tests:**
- [ ] Try-catch-finally blokları parse ediliyor
- [ ] Catch clauses exception type ile eşleşiyor
- [ ] Throw statements parse ediliyor
- [ ] Nested try-catch blokları destekleniyor
- [ ] Exception type definitions çalışıyor

**Codegen Tests:**
- [ ] LLVM invoke instruction üretiliyor
- [ ] Landingpad doğru exception türlerini yakalıyor
- [ ] Finally block her durumda çalışıyor
- [ ] Stack unwinding doğru çalışıyor
- [ ] Exception propagation doğru

**Runtime Tests:**
- [ ] Exception throw ve catch çalışıyor
- [ ] Multiple catch clauses doğru sırayla test ediliyor
- [ ] Finally block her durumda execute ediliyor
- [ ] Rethrow mekanizması çalışıyor
- [ ] Custom exception types çalışıyor

**Integration Tests:**
- [ ] Result pattern ile birlikte çalışabiliyor
- [ ] Memory leaks yok (Valgrind)
- [ ] Performance overhead kabul edilebilir
- [ ] Error messages anlaşılır

**Build Tests:**
- [ ] Clean compile (no warnings)
- [ ] All tests pass
- [ ] Documentation complete
- [ ] Examples working

---

## 📊 DOSYA YAPISI

```
compiler/stage0/modules/exception/
├── exception.h                  # Exception types & structures
├── exception_types.h            # Built-in exception types
├── exception_types.c            # Exception type implementations
├── exception_parser.c           # Exception definition parser
├── try_catch_parser.c           # Try-catch-finally parser
├── throw_parser.c               # Throw statement parser
├── stack_unwinding.c            # Stack unwinding logic
├── exception_codegen.c          # LLVM codegen
└── README.md                    # Module documentation

runtime/sto/
├── exception_handler.c          # Runtime exception handler
└── personality.c                # LLVM personality function

tests/llvm/10_exception/
├── test_basic_try_catch.mlp
├── test_multiple_catch.mlp
├── test_finally.mlp
├── test_nested.mlp
├── test_rethrow.mlp
├── test_propagation.mlp
├── test_finally_only.mlp
├── test_custom_exception.mlp
└── README.md
```

---

## 🚨 ÖNEMLİ NOTLAR

### Result Pattern vs Try-Catch

**Result Pattern (YZ_206):**
- ✅ Zero-cost (compile-time)
- ✅ Explicit error handling (forced by type system)
- ✅ No runtime overhead
- ✅ Functional style
- ❌ Verbose (explicit match/propagation)

**Try-Catch (YZ_207):**
- ✅ Imperative style (familiar to Java/C++ devs)
- ✅ Automatic propagation (no explicit ?)
- ✅ Finally block for cleanup
- ❌ Runtime overhead (stack unwinding)
- ❌ Can be forgotten (no type system enforcement)

**Öneri:** Her iki yöntemi de destekle, kullanıcı tercihe göre seçsin.

### LLVM Exception Handling

LLVM exception handling complex bir konudur:
- `invoke` instruction: Call with exception handling
- `landingpad`: Exception catch point
- `resume`: Rethrow exception
- Personality function: Language-specific exception handler

**Referans:**
- LLVM Language Reference: Exception Handling
- Itanium C++ ABI (exception handling model)
- Clang exception codegen (reference implementation)

### Performance Considerations

Try-catch overhead:
- **Happy path:** Minimal (invoke ≈ call)
- **Exception thrown:** Significant (stack unwinding)
- **Finally block:** Always runs (overhead)

**Optimize:**
- Avoid exceptions in hot paths
- Use Result pattern for performance-critical code
- Cache personality function

---

## 📚 KAYNAKLAR

**LLVM Exception Handling:**
- https://llvm.org/docs/ExceptionHandling.html
- https://llvm.org/docs/LangRef.html#invoke-instruction
- https://llvm.org/docs/LangRef.html#landingpad-instruction

**Exception Handling Models:**
- Itanium C++ ABI: https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html
- Zero-cost exceptions: https://www.iar.com/knowledge/learn/programming/advanced-c-exception-handling/

**Similar Implementations:**
- Swift error handling: try-catch + Result
- Rust: Result/Option (no exceptions)
- Java: try-catch-finally
- C++: try-catch + RAII

---

## ✅ TAMAMLANMA KOŞULLARI

YZ_207 tamamlandı sayılır eğer:

1. **Parser Complete:**
   - Try-catch-finally syntax parse ediliyor
   - Exception types tanımlanıyor
   - Throw statements çalışıyor

2. **Codegen Complete:**
   - LLVM invoke/landingpad üretiliyor
   - Stack unwinding çalışıyor
   - Finally blocks execute ediliyor

3. **Tests Pass:**
   - 8 test dosyası geçiyor
   - Memory leaks yok
   - Integration sorunsuz

4. **Documentation:**
   - README.md complete
   - Examples working
   - API documented

5. **Build Success:**
   - Clean compile
   - No warnings
   - Makefile updated

---

## 🎯 NEXT STEPS

**Sonraki Görev:** YZ_208 - Lambda/Anonymous Functions

**Hazırlık:**
- YZ_207 tamamlandıktan sonra
- Lambda module zaten yazılmış (entegrasyon only)
- Functional programming foundation

**Öncelik:** 🟡 Yüksek (lambda kritik feature)

---

**Good luck, YZ_207! 🚀**
