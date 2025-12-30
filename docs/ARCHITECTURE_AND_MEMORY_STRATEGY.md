# MLP Mimari ve Bellek Yönetimi Stratejisi

**Tarih:** 27 Aralık 2025  
**Durum:** 🎯 Stratejik Karar Belgesi  
**Son Güncelleme:** 30 Aralık 2025 (Module Registry Pattern eklendi)

---

## 🏗️ MİMARİ KARAR: MODULE REGISTRY PATTERN (30 Aralık 2025)

**Karar Tarihi:** 30 Aralık 2025  
**Kapsam:** Stage1+ compiler orchestration  
**Durum:** ✅ Onaylandı - Bu model üzerinden devam edilecek

### Motivasyon

**Sorun:** `melpc.mlp` (compiler orchestrator) naive yaklaşımda her yeni TODO ile büyüyecekti:

```mlp
// ❌ NAİVE YAKLAŞIM - Monolitik büyüme!
func compile_file(input: string, output: string) -> i32 {
    let ast = run_parser(run_lexer(input));
    
    // Her TODO yeni satırlar ekler:
    ast = run_async_transformer(ast);        // TODO #8
    ast = run_macro_expander(ast);           // TODO #3
    ast = run_generic_instantiator(ast);     // TODO #3
    ast = run_trait_resolver(ast);           // TODO #4
    // ... 100+ pass eklenebilir!
    
    return run_codegen(ast, output);
}
```

**Sonuç:** 6 Temel Esas ihlali (500 satır limiti aşımı), monolitik orchestrator.

### Çözüm: Module Registry + Auto-Dispatch

**melpc.mlp asla büyümez (20 satır sabit):**

```mlp
// ✅ THIN ORCHESTRATOR - Asla büyümez!
func compile_file(input: string, output: string) -> i32 {
    let ast = run_parser(run_lexer(input));
    
    // Tek satır - tüm pass'ler otomatik çalışır
    ast = CompilerPipeline.run_all_passes(ast);
    
    return run_codegen(ast, output);
}
```

**CompilerPipeline Module (Core):**

```mlp
// modules/compiler_pipeline/compiler_pipeline.mlp
module CompilerPipeline {
    // Pass registry - modüller buraya kayıt eder
    let pass_registry: HashMap<string, PassHandler> = {}
    
    struct PassHandler {
        name: string
        pass_type: PassType      // Transform, Optimize, etc.
        priority: i32            // Execution order
        handler: func(ASTNode) -> ASTNode
    }
    
    func register_pass(name, type, priority, handler) {
        pass_registry[name] = PassHandler { ... }
    }
    
    func run_all_passes(ast: ASTNode) -> ASTNode {
        let passes = pass_registry.values().sort_by_priority()
        for pass in passes {
            ast = pass.handler(ast)
        }
        return ast
    }
}
```

**Her modül kendini kayıt eder:**

```mlp
// modules/async/async.mlp (TODO #8)
init {
    CompilerPipeline.register_pass("async_transform",
                                   PassType.AST_TRANSFORM,
                                   priority: 10,
                                   handler: async_transform_ast)
}

// modules/generics/generics.mlp (TODO #3)
init {
    CompilerPipeline.register_pass("generic_instantiate",
                                   PassType.AST_TRANSFORM,
                                   priority: 20,
                                   handler: instantiate_generics)
}
```

### Karşılaştırma

| Özellik | Naive | Module Registry |
|---------|-------|------------------|
| melpc.mlp boyutu | ❌ Büyür (TODO başına +5 satır) | ✅ Sabit (20 satır) |
| Yeni modül eklemek | ❌ melpc.mlp düzenle | ✅ init() çağır |
| Pass sırası | ❌ Manuel hardcode | ✅ Priority ile otomatik |
| Modül aktif/pasif | ❌ Kod değişikliği | ✅ Registry operasyonu |
| 6 Temel Esas | ❌ İhlal riski | ✅ Uyumlu |

### Referanslar

- **GCC:** Plugin Architecture (callback registration)
- **LLVM:** Pass Manager (dynamic pass registration)
- **Rust:** Compiler plugins (deprecated but similar idea)

**Bu mimari model üzerinden devam edilecek!** ✅

---

## ⚠️ ÖNEMLİ GÜNCELLEME (27 Aralık 2025)

**MLP'DE GC (Garbage Collection) YOK! ✅**

MLP, RAII (Resource Acquisition Is Initialization) benzeri pattern kullanır:
- **Scope-based bellek yönetimi:** Değişkenler scope çıkışında otomatik free edilir
- **Compiler tarafından:** Derleyici otomatik olarak cleanup kodlarını ekler
- **Sıfır runtime overhead:** GC pause yok, mark-sweep yok, reference counting yok
- **Öngörülebilir performans:** Bellek temizliği deterministik

**İmplementasyon Durumu:**
- ✅ Phase 1: gc_integration.mlp stub'ları RAII yorumları ile güncellendi
- ✅ Phase 2: STO (Smart Type Optimization) AST entegrasyonu tamamlandı
- ✅ Phase 3: Scope tracker implementasyonu (otomatik free enjeksiyonu)
- ✅ Phase 4: `state` keyword eklendi (modül-scoped değişkenler)

**Detaylar:** [`TODO_ARCHITECTURE/ARCH_YZ/completed/`](../TODO_ARCHITECTURE/ARCH_YZ/completed/)

---

## 📋 İçindekiler

1. [Şu Anki Mimari](#şu-anki-mimari)
2. [Bellek Yönetimi Analizi](#bellek-yönetimi-analizi)
3. [Alternatif Strateji: MLP → C → Binary](#alternatif-strateji-mlp--c--binary)
4. [Karşılaştırma ve Öneri](#karşılaştırma-ve-öneri)
5. [Uygulama Planı](#uygulama-planı)

---

## 🏗️ Şu Anki Mimari

### Derleme Pipeline

```
MLP Kaynak Kodu (.mlp)
    ↓
Preprocessor (dil_cevirici.py) - Çok dilli destek
    ↓
İngilizce MLP (.preprocessed.mlp)
    ↓
C Derleyici (c_lexer.c, c_parser.c, c_generator.c)
    ↓
x86-64 Assembly (NASM) (.asm)
    ↓
NASM Assembler
    ↓
Object File (.o)
    ↓
GCC Linker (runtime.o ile)
    ↓
Native Binary
```

**Kritik Nokta:** `c_generator.c` Assembly üretiyor, C kodu değil! ❌

### Neden "c_generator" İsmi?

Tarihsel sebepler:
- Başlangıçta C üretme planı vardı
- Sonra performans için doğrudan Assembly'ye geçildi
- İsim değiştirilmedi (teknik borç 😅)

**Doğru isim olmalıydı:** `asm_generator.c`

---

## 🧠 Bellek Yönetimi Analizi

### Mevcut Durum

#### 1. Stack Değişkenler (Otomatik) ✅

**MLP Kodu:**
```mlp
int x = 42;
string name = "Alice";
```

**Üretilen Assembly:**
```nasm
; int x = 42
sub rsp, 8          ; Stack'te yer aç
mov qword [rbp-8], 42

; string name = "Alice"
sub rsp, 8
lea rax, [msg_0]    ; String literal pointer
mov [rbp-16], rax
```

**Sonuç:** 
- Stack'te tutuluyor ✅
- Fonksiyon bitince otomatik temizleniyor ✅
- Sıfır bellek sızıntısı riski ✅

#### 2. Dinamik Yapılar (List<T>) ✅ ÇÖZÜLDÜ!

**MLP Kodu:**
```mlp
list<int> numbers = list<int>();
numbers.add(42);
numbers.add(100);
-- Fonksiyon bitince ne olacak?
```

**Üretilen Assembly (27 Aralık 2025 - Phase 3 Sonrası):**
```nasm
; list<int> oluşturma
call mlp_list_create     ; malloc ile Heap'te yer açıyor
mov [rbp-8], rax         ; Pointer'ı stack'te tut

; add çağrıları
...

; ✅ SCOPE TRACKER OTOMATİK EKLEDİ (Phase 3):
mov rdi, [rbp-8]
call mlp_list_free       ; <-- Compiler otomatik ekledi!
```

**Sonuç:**
- Heap'te allocate ediliyor (malloc) ✅
- **Scope çıkışında otomatik free ediliyor** ✅ (Phase 3 - Scope Tracker)
- **Bellek sızıntısı ÇÖZÜLDÜ** ✅✅✅
- **RAII pattern:** Compiler otomatik cleanup kodu ekler

### ✅ İmplementasyon Tamamlandı: Scope Çıkışında Otomatik Free (Phase 3)

**İstenen Davranış (ŞİMDİ ÇALIŞIYOR):**
```nasm
; Scope başlangıcı
call mlp_list_create
mov [rbp-8], rax

; ... işlemler ...

; Scope sonu - DERLEYİCİ OTOMATİK EKLİYOR! ✨
mov rdi, [rbp-8]
call mlp_list_free        ; <-- Compiler Phase 3'te ekledi!

; Stack temizliği
add rsp, 8
```

**Nasıl Çalışıyor?**
- Compiler her scope'u track ediyor (Phase 3 - Scope Tracker)
- Heap allocate edilen her değişkeni kaydediyor
- Scope çıkışında otomatik `free()` çağrısı enjekte ediyor
- RAII pattern: Resource Acquisition Is Initialization
- GC'ye gerek yok: Deterministik, öngörülebilir, sıfır overhead

---

## 🔄 Alternatif Strateji: MLP → C → Binary

### Gemini'nin Önerdiği Mimari

```
MLP Kaynak Kodu (.mlp)
    ↓
MLP Derleyici
    ↓
C Kaynak Kodu (.c)  ⬅️ YENİ AŞAMA
    ↓
GCC/Clang
    ↓
Native Binary
```

### Örnek Dönüşüm

#### MLP Kodu:
```mlp
func test()
    int x = 5;
    list<int> numbers = list<int>();
    numbers.add(x);
    print numbers.get(0)
end func
```

#### Üretilecek C Kodu:
```c
void test() {
    // Stack değişken - C halleder ✅
    int x = 5;
    
    // Heap değişken - Malloc
    MLPList* numbers = mlp_list_create(sizeof(int));
    mlp_list_add(numbers, &x);
    
    int value = *(int*)mlp_list_get(numbers, 0);
    mlp_print_int(value);
    
    // ✨ DERLEYİCİ OTOMATİK EKLEDİ:
    mlp_list_free(numbers);  // <-- SORUN ÇÖZÜLDÜ!
} // x otomatik temizlendi (Stack)
```

### Avantajlar

#### 1. Bellek Yönetimi Basitleşir ✅

**Stack Değişkenler:**
- C'nin otomatik scope yönetimi
- `{` ile başlar, `}` ile biter
- Hiçbir şey yapmanıza gerek yok

**Heap Değişkenler:**
- MLP derleyicisi scope çıkışını tespit eder
- C koduna `free()` çağrısı ekler
- GC'ye gerek kalmaz

#### 2. Optimizasyon Bedava 🚀

```bash
gcc -O3 -march=native output.c -o program
```

GCC/Clang:
- Register allocation
- Loop unrolling
- Inline expansion
- Dead code elimination
- Constant propagation

**Assembly yazarken bunları manuel yapıyorsunuz!**

#### 3. Taşınabilirlik 🌍

Aynı C kodu:
- x86-64 (Linux, Windows, macOS)
- ARM64 (Apple Silicon, Raspberry Pi)
- RISC-V
- WebAssembly (Emscripten ile)

**Şu anki NASM kodu sadece x86-64 Linux!**

#### 4. Debugging Kolaylaşır 🐛

```bash
gcc -g output.c -o program
gdb program
(gdb) break test
(gdb) print x
(gdb) print numbers->size
```

GDB, Valgrind, AddressSanitizer hepsi çalışır!

#### 5. Daha Az Kod 📉

**Assembly Generator:** 2500+ satır C kodu  
**C Generator:** ~500 satır C kodu (tahmin)

Çünkü:
- Register allocation yok (GCC halleder)
- Stack frame yönetimi yok (GCC halleder)
- Calling convention yok (GCC halleder)

---

## ⚖️ Karşılaştırma ve Öneri

### Mevcut Mimari: MLP → Assembly (27 Aralık 2025 - RAII İmplementasyonu)

| Artılar | Eksiler |
|---------|---------|
| ✅ Tam kontrol | ❌ 2500+ satır generator kodu |
| ✅ Performans (teoride) | ❌ Sadece x86-64 Linux |
| ✅ Öğretici (Assembly bilgisi) | ❌ Debugging zor |
| ✅ **Bellek yönetimi çözüldü (RAII)** | ❌ Optimizasyon manuel |
| ✅ **Scope-based free (Phase 3)** | |

### Önerilen Mimari: MLP → C (Gelecek Planı)

| Artılar | Eksiler |
|---------|---------|
| ✅ Bellek yönetimi kolay (RAII zaten var) | ❌ GCC'ye bağımlılık |
| ✅ GCC optimizasyonları | ❌ Biraz daha yavaş derleme |
| ✅ Taşınabilirlik (ARM, RISC-V, WASM) | ❌ Üretilen C kodu şişkin olabilir |
| ✅ Debugging araçları | |
| ✅ Daha az kod (~500 satır) | |
| ✅ **GC'ye gerek yok (RAII pattern)** | |

**Not:** MLP'de GC YOK! Her iki backend de RAII pattern kullanır.

### Karar: Hibrit Yaklaşım 🎯

**Öneri:** Her iki backend'i de destekleyin!

```
MLP Parser/AST
    ├─→ C Backend (Varsayılan) → gcc → binary
    └─→ ASM Backend (Legacy) → nasm → binary
```

**Kullanım:**
```bash
# C backend (varsayılan)
mlpc program.mlp -o output

# Assembly backend (eski yöntem)
mlpc program.mlp -o output --backend=asm

# C kodu göster (debugging)
mlpc program.mlp --emit-c -o output.c
```

---

## 🚀 Uygulama Planı

### Faz 1: C Backend Temel Yapı (1-2 gün)

**Dosyalar:**
```
c_compiler/
├── c_backend.c          ⬅️ YENİ
├── c_backend.h          ⬅️ YENİ
├── c_generator.c        (ASM backend - kalsın)
└── main.c               (Backend seçimi)
```

**Test:**
```mlp
int x = 5;
print x
```

**Üretilecek C:**
```c
#include "runtime.h"
int main() {
    int x = 5;
    mlp_print_int(x);
    return 0;
}
```

### Faz 2: Scope-Based Free Injection (2-3 gün)

**Amaç:** `list<T>` için otomatik free

**MLP:**
```mlp
func test()
    list<int> nums = list<int>();
    nums.add(42);
end func
```

**C:**
```c
void test() {
    MLPList* nums = mlp_list_create(sizeof(int));
    int temp = 42;
    mlp_list_add(nums, &temp);
    
    // ✨ Otomatik eklendi:
    mlp_list_free(nums);
}
```

**Algoritma:**
```python
def generate_function_body(ast_node):
    # 1. Scope başlangıcı
    emit("void " + ast_node.name + "() {")
    
    # 2. Heap değişkenleri takip et
    heap_vars = []
    for stmt in ast_node.body:
        if stmt.type == "LIST_DECLARATION":
            heap_vars.append(stmt.var_name)
        generate_statement(stmt)
    
    # 3. Scope sonu - Free hepsini!
    for var in heap_vars:
        emit(f"    mlp_list_free({var});")
    
    emit("}")
```

### Faz 3: Control Flow Free (2-3 gün)

**Zorluk:** Erken return/break durumları

**MLP:**
```mlp
func search(x)
    list<int> temp = list<int>();
    
    if x < 0 then
        return -1  -- ⚠️ temp leak ediyor!
    end
    
    return 0
end func
```

**Yanlış C:**
```c
int search(int x) {
    MLPList* temp = mlp_list_create(sizeof(int));
    
    if (x < 0) {
        return -1;  // ❌ temp leak!
    }
    
    return 0;       // ❌ temp leak!
}
```

**Doğru C:**
```c
int search(int x) {
    MLPList* temp = mlp_list_create(sizeof(int));
    int __return_value;
    
    if (x < 0) {
        __return_value = -1;
        goto __cleanup;  // ✅ Free'ye git
    }
    
    __return_value = 0;
    
__cleanup:
    mlp_list_free(temp);
    return __return_value;
}
```

**Algoritma:** Cleanup Label Pattern
```python
def generate_function_with_cleanup(ast_node):
    has_heap_vars = has_dynamic_allocations(ast_node)
    
    if has_heap_vars:
        # Return'leri goto'ya çevir
        transform_returns_to_goto(ast_node)
        
        # Cleanup label ekle
        emit("__cleanup:")
        for var in heap_vars:
            emit(f"    mlp_list_free({var});")
        emit("    return __return_value;")
```

### Faz 4: Nested Scopes (2-3 gün)

**MLP:**
```mlp
func test()
    list<int> outer = list<int>();
    
    if true then
        list<int> inner = list<int>();
        -- inner burada ölmeli
    end
    
    -- outer burada ölmeli
end func
```

**C:**
```c
void test() {
    MLPList* outer = mlp_list_create(sizeof(int));
    
    if (1) {
        MLPList* inner = mlp_list_create(sizeof(int));
        
        // Scope sonu
        mlp_list_free(inner);  // ✅
    }
    
    // Function sonu
    mlp_list_free(outer);  // ✅
}
```

### Faz 5: Test ve Benchmark (1-2 gün)

**Test Senaryoları:**
```mlp
-- 1. Basit scope
-- 2. Nested scope
-- 3. Early return
-- 4. Loop içinde break
-- 5. Exception gibi durumlar
```

**Benchmark:**
```bash
# Assembly backend
time ./mlpc_asm test.mlp && time ./test

# C backend
time ./mlpc_c test.mlp && time ./test

# Karşılaştır
```

### Faz 6: Dokümantasyon (1 gün)

**Güncellenecek:**
- SPECS.md → Backend seçenekleri
- README.md → Yeni derleme bayrakları
- ARCHITECTURE.md → Bu doküman

---

## 📝 Örnek: Fibonacci Karşılaştırması

### MLP Kaynak Kodu
```mlp
func fibonacci(n)
    if n <= 1 then
        return n
    end
    return fibonacci(n - 1) + fibonacci(n - 2)
end func

int result = fibonacci(10);
print result
```

### Assembly Backend (Mevcut)
```nasm
; 50+ satır assembly
global fibonacci
fibonacci:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    
    ; if n <= 1
    mov rax, [rbp+16]
    cmp rax, 1
    jg .L1
    
    ; return n
    mov rax, [rbp+16]
    jmp .L2
    
.L1:
    ; fibonacci(n-1)
    mov rax, [rbp+16]
    sub rax, 1
    push rax
    call fibonacci
    add rsp, 8
    push rax
    
    ; fibonacci(n-2)
    mov rax, [rbp+16]
    sub rax, 2
    push rax
    call fibonacci
    add rsp, 8
    
    ; add
    pop rbx
    add rax, rbx
    
.L2:
    mov rsp, rbp
    pop rbp
    ret
```

### C Backend (Önerilen)
```c
#include "runtime.h"

int64_t fibonacci(int64_t n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    int64_t result = fibonacci(10);
    mlp_print_int(result);
    return 0;
}
```

**Sonuç:**
- C: 8 satır, okunabilir, maintainable
- ASM: 50+ satır, karmaşık, hata riskli
- GCC -O3: Assembly'den daha hızlı optimizasyon!

---

## 🎯 Sonuç ve Tavsiye

### Gemini Haklı! ✅

**Bellek yönetimi için C backend şart değil ama çok mantıklı:**

1. **Stack değişkenler:** C'nin scope sistemi otomatik hallediyor
2. **Heap değişkenler:** Scope çıkışında free enjeksiyonu kolay
3. **GC'ye gerek yok:** RAII benzeri pattern yeterli
4. **Optimizasyon:** GCC/Clang > El yazısı assembly

### Uygulama Stratejisi

**Kısa Vadede (1-2 hafta):**
1. C backend prototype oluştur
2. Basit programları test et
3. Scope-based free injection ekle
4. Performans karşılaştır

**Orta Vadede (1-2 ay):**
1. C backend'i varsayılan yap
2. Assembly backend'i legacy olarak tut
3. List<T> için otomatik free çalışsın
4. Dokümantasyon tamamla

**Uzun Vadede (3-6 ay):**
1. Assembly backend'i kaldır (veya sadece öğretim için tut)
2. Çoklu platform desteği (ARM, RISC-V)
3. WebAssembly backend (Emscripten)
4. LLVM backend araştır

### Final Yorum

**Şu an:** `MLP → Assembly` → 2500 satır kod, manuel bellek yönetimi, sadece x86-64

**Gelecek:** `MLP → C → Binary` → 500 satır kod, otomatik bellek yönetimi, her platform

**Self-hosting için bonus:** C backend'i MLP ile yazabilirsiniz! 🚀

```mlp
-- c_backend.mlp
func generate_c_code(ast)
    list<string> output = list<string>();
    
    for node in ast do
        if node.type == "FUNCTION" then
            output.add(generate_function(node))
        end
    end
    
    return output
end func
```

---

**© 2025 MLP Project**  
**Version:** 1.0  
**Status:** Strategic Decision Document  
**License:** MIT
