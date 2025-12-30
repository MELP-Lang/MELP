# 📋 TODO: MLP-GCC %100 SELF-HOSTING

**Proje:** MLP-GCC  
**Başlangıç:** 29 Aralık 2025  
**Durum:** 🔴 BAŞLA VE BİTİR - Monolitik sabotaj sonrası kurtarma  
**Nihai Hedef:** %100 SELF-HOSTING (MLP kendini derliyor!)  
**Yetki:** Danışman_Yol_Haritası.md + 5 Core Principles

---

## 🚨 CRITICAL: BU TODO BİTTİĞİNDE

**SELF-HOSTING İLAN EDİLECEK! BAŞKA ADIM YOK!**

```bash
# Bu TODO bittiğinde şu komut çalışacak:
cd MELP/MLP/stage1
./melpc_stage1 melpc.mlp -o melpc_stage2  # MLP kendini derliyor!
./melpc_stage2 --version                  # Self-hosting başarılı!
```

**"Hayır henüz self-hosting ilan edemeyiz çünkü..." DİYEMEZSİN!**

Bu TODO'daki her görev self-hosting için GEREKLİ ve YETERLİ!

---

## 🎯 NİHAİ HEDEF: SELF-HOSTING

**Self-hosting tanımı:**
```
MLP derleyicisi (Stage1-MLP) kendi kaynak kodunu derleyebiliyor.
```

**Başarı kriteri:**
```bash
# Stage1-MLP derleyicisi Stage1-MLP kaynak kodunu derliyor:
./melpc_stage1 compiler/lexer.mlp -o lexer.o     ✅
./melpc_stage1 compiler/parser.mlp -o parser.o   ✅
./melpc_stage1 compiler/codegen.mlp -o codegen.o ✅
# ... tüm modüller

# Link edilip yeni derleyici oluşturuluyor:
gcc *.o -o melpc_stage2

# Yeni derleyici çalışıyor:
./melpc_stage2 --version
# MLP-GCC Stage2 Self-Hosted Compiler v1.0

# 🎉 SELF-HOSTING BAŞARILI!
```

**GERÇEK:** Bu proje %90 HAZIR, son dokunuşlar gerekiyor!
- ✅ Stage0-C derleyici çalışıyor (~70 modül)
- ✅ Stage1-MLP modülleri hazır (~17 modül)
- ✅ STO runtime entegre
- ❌ Codegen ASM üretiyor → C üretmeli (KALAN TEK ŞEY!)

---

## 🎯 BÜYÜK RESİM

```
┌─────────────────────────────────────────────────────────────┐
│                    MLP-GCC SELF-HOSTING                     │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│   [P0] HAZIRLIK (YZ_01)   → Analiz, emit_c(), print/var    │
│         ↓                                                   │
│   [P1] CODEGEN (YZ_02)    → Control/func/expr ASM→C        │
│         ↓                                                   │
│   [P2] BOOTSTRAP (YZ_03)  → Stage0-C test                   │
│         ↓                                                   │
│   [P3] STAGE1 (YZ_04)     → MLP modüllerini derle           │
│         ↓                                                   │
│   [P4] SELF-HOST (YZ_05)  → MLP kendini derliyor! 🎉        │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

**YZ Dağılımı:**
- **YZ_01:** P0 - Hazırlık + emit_c() + print/variable (3-5 saat)
- **YZ_02:** P1 - Control flow + functions + expressions (3-5 saat)  
- **YZ_03:** P2 - Bootstrap test (2-3 saat)
- **YZ_04:** P3 - Stage1 derleme (3-4 saat)
- **YZ_05:** P4 - Self-hosting! (2-3 saat)

**Formül:** `MELP = Modüler + GCC + STO + Stateless + (struct + functions)`

---

## 📊 MEVCUT DURUM (GERÇEK)

| Bileşen | Konum | Durum | Detay |
|---------|-------|-------|-------|
| Stage0-C Modülleri | `MELP/C/stage0/modules/` | ✅ ~70 modül | STAGE3_COMPLETE.md |
| Stage1-MLP Modülleri | `MELP/MLP/stage1/modules/` | ✅ 17 modül | Hazır |
| STO Runtime | `MELP/runtime/` | ✅ Aktif | C runtime |
| Modülerlik | - | ✅ %100 | 30/30 modül entegre |
| **Codegen (ASM→C)** | - | ❌ KRİTİK | **SADECE BU EKSİK!** |

**ANALİZ:**
- ✅ Lexer: Çalışıyor
- ✅ Parser: Çalışıyor  
- ✅ AST: Çalışıyor
- ❌ Codegen: x86-64 ASM üretiyor → **C üretmeli!**
- ✅ Runtime: Hazır
- ✅ Test suite: Mevcut

**SORUN:** Sadece codegen ASM üretiyor, C üretmeli. Bu değişirse → SELF-HOSTING!

---

## � GÖREVLER (BAŞLA ve BİTİR!)

**Her görevi tamamladıkça `[ ] → [x]` işaretle!**

---

## 🔴 PHASE 0: HAZIRLIK VE TEMEL CODEGEN (YZ_01)

**Hedef:** Proje analizi + emit_c() + print/variable dönüşümü  
**Süre:** 3-5 saat  
**YZ:** YZ_01

### P0.1: Proje Yapısını Doğrula (YZ_01)
- [ ] `MELP/C/stage0/modules/codegen/` dizinini incele
- [ ] Hangi dosyalar ASM emit() çağrısı yapıyor? (grep ile bul)
- [ ] emit() fonksiyonlarını listele
- [ ] Test case'leri incele

**Beklenen Süre:** 1-2 saat

#### 1.2 emit_c() Fonksiyonu (YZ_01)
- [ ] `codegen_emit.c` dosyasına `emit_c()` fonksiyonu ekle
- [ ] Parametre: `const char* c_code`
- [ ] Çıktı: stdout'a C kodu yaz
- [ ] Test: `emit_c("int x = 5;")` → stdout'a yaz

**Dosyalar:** `codegen_emit.c`, `codegen_emit.h`  
**Beklenen Süre:** 30-60 dakika

#### 1.3 Print Statement Dönüşümü (YZ_01)
- [ ] `codegen_print.c` → ASM emit yerine C emit
- [ ] `print("hello")` → `emit_c("printf(\"hello\\n\");")`
- [ ] Test: Örnek print case'i çalıştır

**Dosyalar:** `codegen_print.c`  
**Beklenen Süre:** 1-2 saat

#### 1.4 Variable Declaration Dönüşümü (YZ_01)
- [ ] `codegen_variable.c` → ASM emit yerine C emit
- [ ] `numeric x = 5` → `emit_c("int64_t x = 5;")`
- [ ] STO type mapping: numeric → int64_t/double
- [ ] Test: Değişken tanımlama case'leri

**Dosyalar:** `codegen_variable.c`  
**Beklenen Süre:** 2-3 saat

**🧪 YZ_01 TEST (ZORUNLU!):**
```bash
cd MELP/C/stage0
make clean && make
./melpc test_print.mlp -o test.c
gcc test.c -o test && ./test  # print çalışmalı
./melpc test_var.mlp -o test.c
gcc test.c -o test && ./test  # değişkenler çalışmalı
```

---

## 🟡 PHASE 1: CONTROL FLOW VE FUNCTIONS (YZ_02)

**Hedef:** Control flow + functions + expressions ASM→C  
**Süre:** 3-5 saat  
**YZ:** YZ_02  
**Bağımlılık:** YZ_01 bitmiş olmalı

#### 1.5 Control Flow Dönüşümü (if/while/for) (YZ_02)
- [ ] `codegen_control.c` → if statement ASM→C
- [ ] `codegen_control.c` → while loop ASM→C
- [ ] `codegen_control.c` → for loop ASM→C
- [ ] Test: Control flow case'leri

**Dosyalar:** `codegen_control.c`  
**Beklenen Süre:** 2-3 saat

#### 1.6 Function Definition Dönüşümü (YZ_02)
- [ ] `codegen_functions.c` → fonksiyon tanımı ASM→C
- [ ] Parametre tipleri, dönüş tipleri
- [ ] Test: Fonksiyon case'leri

**Dosyalar:** `codegen_functions.c`  
**Beklenen Süre:** 2-3 saat

#### 1.7 Expression Dönüşümü (YZ_02)
- [ ] `codegen_expression.c` → aritmetik işlemler ASM→C
- [ ] `codegen_expression.c` → karşılaştırma işlemleri ASM→C
- [ ] Test: Expression case'leri

**Dosyalar:** `codegen_expression.c`  
**Beklenen Süre:** 1-2 saat

**🧪 YZ_02 TEST (ZORUNLU!):**
```bash
cd MELP/C/stage0
make clean && make
./melpc test_control.mlp -o test.c
gcc test.c -o test && ./test  # if/while çalışmalı
./melpc test_function.mlp -o test.c
gcc test.c -o test && ./test  # fonksiyonlar çalışmalı
```

---

## 🟢 PHASE 2: BOOTSTRAP TEST (YZ_03)

**Hedef:** Stage0-C derleyici çalışıyor mu kontrol et.  
**Süre:** 2-3 saat  
**YZ:** YZ_03  
**Bağımlılık:** YZ_02 bitmiş olmalı

#### 2.1 Stage0 Derleme (YZ_02)
- [ ] `cd MELP/C/stage0 && make clean && make`
- [ ] Binary oluştu mu: `./melpc`
- [ ] Test suite çalıştır: `./run_tests.sh`

**Beklenen Süre:** 30 dakika

#### 2.2 Hello World Testi (YZ_02)
- [ ] `examples/hello_english.mlp` → C kod üret
- [ ] `./melpc examples/hello_english.mlp -o hello.c`
- [ ] `gcc hello.c -o hello`
- [ ] `./hello` → "Hello, World!" yazdırıyor mu?

**Beklenen Süre:** 30 dakika

#### 2.3 Complex Test (YZ_02)
- [ ] Fonksiyonlar, döngüler, koşullar içeren test
- [ ] Stage0 başarıyla derliyor mu?

**Beklenen Süre:** 1 saat

**🧪 YZ_03 TEST (ZORUNLU!):**
```bash
cd MELP/C/stage0
make clean && make
./run_tests.sh  # Tüm testler geçmeli
./melpc examples/hello_english.mlp -o hello.c
gcc hello.c -o hello && ./hello
```

---

## 🟡 PHASE 3: STAGE1 DERLEME (YZ_04)

**Hedef:** Stage0 ile Stage1 modüllerini derle.  
**Süre:** 3-4 saat  
**YZ:** YZ_04  
**Bağımlılık:** YZ_03 bitmiş olmalı

#### 3.1 Stage1 Modüllerini Derle (YZ_04)
- [ ] `./melpc MELP/MLP/stage1/modules/lexer.mlp -o lexer.c`
- [ ] `./melpc MELP/MLP/stage1/modules/parser.mlp -o parser.c`
- [ ] `./melpc MELP/MLP/stage1/modules/codegen.mlp -o codegen.c`
- [ ] ... (17 modül)

**Beklenen Süre:** 2-3 saat

#### 3.2 Stage1 Link (YZ_03)
- [ ] Tüm .c dosyalarını derle: `gcc *.c -o melpc_stage1`
- [ ] Stage1 derleyici oluştu mu?

**Beklenen Süre:** 30 dakika

#### 3.3 Stage1 Test (YZ_03)
- [ ] `./melpc_stage1 --version`
- [ ] `./melpc_stage1 examples/hello_english.mlp -o hello2.c`
- [ ] Çalışıyor mu?

**Beklenen Süre:** 30 dakika

**🧪 YZ_04 TEST (ZORUNLU!):**
```bash
cd MELP/MLP/stage1/bin
./mlpc_stage1 --version
./mlpc_stage1 test.mlp -o test.c
gcc test.c -o test && ./test
```

---

## 🎉 PHASE 4: SELF-HOSTING! (YZ_05)

**Hedef:** Stage1 kendini derleyebiliyor mu?  
**Süre:** 2-3 saat  
**YZ:** YZ_05  
**Bağımlılık:** YZ_04 bitmiş olmalı

#### 4.1 Self-Host Test (YZ_05)
```bash
# Stage1 kendi kaynak kodunu derliyor:
./melpc_stage1 MELP/MLP/stage1/modules/lexer.mlp -o lexer2.c
./melpc_stage1 MELP/MLP/stage1/modules/parser.mlp -o parser2.c
./melpc_stage1 MELP/MLP/stage1/modules/codegen.mlp -o codegen2.c
# ... tüm modüller

# Link:
gcc lexer2.c parser2.c codegen2.c ... -o melpc_stage2

# Test:
./melpc_stage2 --version
```

- [ ] Stage1 kendi kaynak kodunu derledi
- [ ] Stage2 binary oluştu
- [ ] Stage2 çalışıyor

**Beklenen Süre:** 1-2 saat

#### 4.2 Self-Host Doğrulama (YZ_03)
- [ ] Stage2 ile Stage1 kodunu tekrar derle → Stage3
- [ ] Stage3 ile Stage1 kodunu tekrar derle → Stage4
- [ ] Stage3 == Stage4 (binary aynı mı?)

**Beklenen Süre:** 1 saat

#### 4.3 🎉 SELF-HOSTING İLANI! (YZ_03)
```bash
echo "🎉 MLP-GCC SELF-HOSTING BAŞARILI!"
echo "MLP derleyicisi kendi kaynak kodunu derleyebiliyor!"
echo "Tarih: $(date)"
```

- [ ] ✅ **SELF-HOSTING TAMAMLANDI!**

**Beklenen Süre:** 5 dakika (kutlama! 🎉)

---

## 📊 PROGRESS TRACKING

**Toplam Görev:** ~25 görev  
**Tamamlanan:** 0  
**Kalan:** 25  

**Estimated Time:**
- Phase 1: 12-18 saat (kritik!)
- Phase 2: 2-3 saat
- Phase 3: 3-4 saat
- Phase 4: 2-3 saat

**TOPLAM:** 19-28 saat (3-5 gün yoğun çalışma)

---

## 🎯 BAŞARI KRİTERİ

```bash
./melpc_stage1 MELP/MLP/stage1/modules/lexer.mlp -o lexer.c
gcc lexer.c ... -o melpc_stage2
./melpc_stage2 --version
# MLP-GCC Stage2 Self-Hosted Compiler v1.0
```

**BU KOMUT ÇALIŞIYORSA → SELF-HOSTING!**

**"Hayır henüz ilan edemeyiz çünkü..." DİYEMEZSİN!**

---

## 🔗 İLGİLİ DOSYALAR

- [YZ_HIZLI_REFERANS.md](YZ_HIZLI_REFERANS.md) - PMLP syntax ve kurallar
- [TODO_KURALLARI.md](TODO_KURALLARI.md) - YZ için genel kurallar
- [NEXT_AI_START_HERE.md](NEXT_AI_START_HERE.md) - Sonraki YZ için başlangıç
- [YZ_WORKFLOW.md](YZ_WORKFLOW.md) - Detaylı iş akışı

---

**BAŞARILI OLACAĞIZ! 🚀 SELF-HOSTING GELİYOR!**

### P0.1: Proje Yapısını Doğrula (YZ_01)
```bash
# Modüllerin doğru konumda olduğunu kontrol et
ls MELP/C/stage0/modules/ | wc -l  # 69 olmalı
ls MELP/MLP/stage1/modules/ | wc -l  # 17 olmalı
```

- [ ] 69 C modülü mevcut
- [ ] 17 MLP modülü mevcut
- [ ] STO runtime mevcut
- [ ] Makefile'lar çalışıyor

### P0.2: GCC Derleme Testi (YZ_01)
```bash
cd MELP/C/stage0/modules/lexer
make clean && make
```

- [ ] Her modül bağımsız derlenebiliyor
- [ ] Hiçbir LLVM bağımlılığı yok
- [ ] Header'lar düzgün include ediliyor

### P0.3: Bağımlılık Grafiği (YZ_01)
```
lexer → token_types
parser → lexer, ast_nodes
codegen → parser, ast_nodes  ← BU ASM→C OLACAK!
```

- [ ] Bağımlılık döngüsü yok
- [ ] Include path'ler doğru

---

## 🔴 P0: HAZIRLIK VE TEMEL CODEGEN (3-5 saat) (YZ_01)

**Bu adım:** Proje analizi + emit_c() + print/variable dönüşümü

### P0.1: Mevcut Codegen Analizi (YZ_01)

**Dosyalar:**
```
MELP/C/stage0/modules/
├── codegen/
│   ├── codegen.c           ← Ana codegen
│   ├── codegen_x86.c       ← x86-64 ASM üretimi
│   └── codegen.h
├── codegen_functions/
├── codegen_control/
├── codegen_lambda/
├── codegen_async/
└── codegen_structures/
```

- [ ] Mevcut ASM üretim kodunu incele
- [ ] C eşdeğerlerini planla
- [ ] Header yapısını koru

### P1.2: C Codegen Tasarımı (YZ_01)

**ESKİ (ASM):**
```c
void codegen_print_string(const char* str) {
    emit("    mov rdi, fmt_string");
    emit("    mov rsi, %s", str);
    emit("    call printf");
}
```

**YENİ (C):**
```c
void codegen_print_string(const char* str) {
    emit("printf(\"%%s\", %s);", str);
}
```

- [ ] `codegen_c.c` oluştur (yeni)
- [ ] `emit_c()` fonksiyonu yaz
- [ ] ASM yerine C statement'ları üret

### P0.3: Print ve Variable Dönüşümü (YZ_01)

| MLP Kodu | Üretilecek C Kodu |
|----------|-------------------|
| `print("hello")` | `printf("hello\n");` |
| `x = 5` | `int64_t x = 5;` |
| `if x > 0` | `if (x > 0) {` |
| `while x < 10` | `while (x < 10) {` |
| `function foo(a)` | `void foo(int64_t a) {` |
| `return x` | `return x;` |

- [ ] Değişken tanımı → C variable
- [ ] Atama → C assignment
- [ ] Print → printf()
- [ ] Koşul → if/while
- [ ] Fonksiyon → C function
- [ ] Return → return statement

### P0.4: STO Tip Mapping (YZ_01)

**MLP'de:**
```mlp
x = 42        // numeric
y = "hello"   // string
```

**C'de:**
```c
int64_t x = 42;           // Küçük sayı → int64
double x = 3.14;          // Ondalık → double
mlp_bigdecimal_t x = ...; // Büyük sayı → bigdecimal
char* y = "hello";        // Kısa string → literal
mlp_string_t y = ...;     // Uzun string → heap
```

- [ ] numeric → int64_t / double / mlp_bigdecimal_t
- [ ] string → char* / mlp_string_t
- [ ] Tip kararı derleme zamanında

### P0.5: Temel Codegen Test (YZ_01)

```bash
# Test: Basit MLP → C
echo 'print("Hello, MLP-GCC!")' > test.mlp
./mlpc test.mlp -o test.c
gcc test.c -o test
./test  # "Hello, MLP-GCC!" yazmalı
```

- [ ] print çalışıyor
- [ ] değişkenler çalışıyor
- [ ] if/while çalışıyor
- [ ] fonksiyonlar çalışıyor

---

## 🟡 P1: CONTROL FLOW VE FUNCTIONS (3-5 saat) (YZ_02)

**Bu adım:** Control flow + functions + expressions ASM→C dönüşümü

### P1.1: Control Flow Dönüşümü (YZ_02)

### P2.1: Stage0-C Compiler Build (YZ_02)

```bash
cd MELP/C/stage0
make clean && make
# veya
./build.sh
```

- [ ] Tüm modüller derlendi
- [ ] `mlpc` binary oluştu
- [ ] Hata/warning yok

### P2.2: Basit MLP Derleme (YZ_02)

```bash
# merhaba.mlp
print("Merhaba Dünya!")

# Derle
./mlpc merhaba.mlp -o merhaba.c
gcc merhaba.c -o merhaba -L../runtime -lmlp_runtime
./merhaba
```

- [ ] MLP → C dönüşümü başarılı
- [ ] GCC derleme başarılı
- [ ] Çıktı doğru

### P2.3: Kapsamlı Test (YZ_02)

```bash
# Test dizinindeki tüm testleri çalıştır
cd tests
./run_all.sh
```

- [ ] Değişken testleri geçti
- [ ] Kontrol akışı testleri geçti
- [ ] Fonksiyon testleri geçti
- [ ] String testleri geçti

---

## 🟡 P3: STAGE1-MLP DERLEME (3-4 saat) (YZ_04)

### P3.1: MLP Modüllerini Derle (YZ_03)

```bash
cd MELP/MLP/stage1/modules

# Her modülü ayrı derle
for f in *.mlp; do
    echo "Derleniyor: $f"
    ../../bin/mlpc $f -o ${f%.mlp}.c
    gcc -c ${f%.mlp}.c -o ${f%.mlp}.o
done
```

**Modül Listesi (17):**
1. token_types.mlp
2. lexer.mlp
3. ast_nodes.mlp
4. parser.mlp
5. parser_functions.mlp
6. parser_structures.mlp
7. parser_advanced.mlp
8. codegen.mlp
9. codegen_functions.mlp
10. codegen_control.mlp
11. codegen_lambda.mlp
12. codegen_async.mlp
13. codegen_structures.mlp
14. function_registry.mlp
15. scope_manager.mlp
16. import_handler.mlp
17. mlp_compiler_main.mlp

- [ ] 17/17 modül derlendi
- [ ] C dosyaları üretildi
- [ ] Object dosyaları oluştu

### P3.2: Stage1 Linker (YZ_03)

```bash
# Tüm object dosyalarını birleştir
gcc *.o -o mlpc_stage1 -L../../runtime -lmlp_runtime
```

- [ ] Linking başarılı
- [ ] `mlpc_stage1` binary oluştu

### P3.3: Stage1 Doğrulama (YZ_03)

```bash
# Stage1 compiler'ı test et
./mlpc_stage1 test.mlp -o test.c
gcc test.c -o test
./test
```

- [ ] Stage1 compiler çalışıyor
- [ ] Aynı çıktıyı üretiyor

---

## 🟢 P4: SELF-HOSTING (2-3 saat) (YZ_05)

### P4.1: Bootstrap Zinciri (YZ_03)

```
Stage0-C Compiler (el yazısı C)
        ↓
Stage1-MLP Modülleri derle → Stage1-C çıktı
        ↓
GCC ile derle → mlpc_stage1 binary
        ↓
mlpc_stage1 ile Stage1-MLP derle → Stage2-C çıktı
        ↓
GCC ile derle → mlpc_stage2 binary
        ↓
Stage2 çıktısı == Stage1 çıktısı? → SELF-HOSTING BAŞARILI! 🎉
```

### P4.2: Eşitlik Kontrolü (YZ_03)

```bash
# Stage1 ve Stage2 aynı C çıktısı üretmeli
diff stage1_output.c stage2_output.c
# Fark olmamalı!
```

- [ ] Stage1 == Stage2
- [ ] Binary'ler aynı çıktı üretiyor

### P4.3: Final Doğrulama (YZ_03)

```bash
# Kendini derleyebiliyor mu?
./mlpc_stage2 mlp_compiler_main.mlp -o self.c
gcc self.c -o mlpc_self
./mlpc_self test.mlp -o test.c
```

- [ ] Self-compile başarılı
- [ ] Aynı sonuç

---

## 📋 P2 MODÜL ENTEGRASYONU (OPSIYONEL)

P1'de olmayıp P2'de olan yararlı modüller:

| Modül | Açıklama | Öncelik |
|-------|----------|---------|
| import | Modül import sistemi | 🔴 Yüksek |
| for_loop | For döngüsü | 🔴 Yüksek |
| switch | Switch-case | 🟡 Orta |
| error | Hata yönetimi | 🟡 Orta |
| async | Asenkron | 🟢 Düşük |

**Entegrasyon Rehberi:** `P2_ENTEGRASYON_YONERGESI.md`

---

## ⏱️ ZAMAN TAHMİNİ

| Phase | Süre | Zorluk |
|-------|------|--------|
| P0: Hazırlık | 2-4 saat | ⭐ Kolay |
| P1: Codegen ASM→C | 4-8 saat | ⭐⭐⭐ Zor |
| P2: Bootstrap Test | 2-3 saat | ⭐⭐ Orta |
| P3: Stage1 Derleme | 3-4 saat | ⭐⭐ Orta |
| P4: Self-hosting | 2-3 saat | ⭐⭐ Orta |

**TOPLAM:** 13-22 saat (2-3 iş günü)

---

## 🚨 YASAKLAR (Danışman_Yol_Haritası.md'den)

- ❌ Monolitik kod yazma (max 500 satır/dosya)
- ❌ Doğrudan ASM üretme (C → GCC)
- ❌ Global mutable state
- ❌ OOP/class kullanma
- ❌ "Sonra düzeltiriz" yaklaşımı

---

## 📝 NOTLAR

1. **P1 en kritik** - Codegen değişikliği tüm sistemi etkiler
2. **Her phase'de test** - İlerleme olmadan geçme
3. **Modüler kal** - Büyük dosya = YASAK
4. **GCC'ye güven** - Optimizasyonu GCC yapar
5. **P2 modülleri bekleyebilir** - Önce temel self-hosting

---

## 🔗 İLGİLİ DOSYALAR

- `Danışman_Yol_Haritası.md` - 5 Temel Esas (DEĞİŞTİRİLEMEZ)
- `P2_ENTEGRASYON_YONERGESI.md` - P2 modül rehberi
- `MELP/C/stage0/modules/` - C bootstrap modülleri
- `MELP/MLP/stage1/modules/` - MLP self-host modülleri
