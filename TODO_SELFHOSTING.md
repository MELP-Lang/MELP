# 📋 MLP-GCC: SIFIRDAN SELF-HOSTING'E

**Proje:** MLP-GCC  
**Başlangıç:** 29 Aralık 2025  
**Hedef:** MLP → C → GCC → Binary (Self-hosting)  
**Yetki:** Danışman_Yol_Haritası.md kuralları geçerli

---

## 🎯 BÜYÜK RESİM

```
┌─────────────────────────────────────────────────────────────┐
│                    MLP-GCC SELF-HOSTING                     │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│   [P0] HAZIRLIK           → Modülleri hazırla, GCC uyumu    │
│         ↓                                                   │
│   [P1] CODEGEN ASM→C      → En kritik değişiklik!          │
│         ↓                                                   │
│   [P2] BOOTSTRAP TEST     → Stage0-C çalışıyor mu?          │
│         ↓                                                   │
│   [P3] STAGE1 DERLEME     → MLP modüllerini derle           │
│         ↓                                                   │
│   [P4] SELF-HOSTING       → MLP kendini derliyor! 🎉        │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

**Formül:** `MELP = Modüler + GCC + STO + Stateless + (struct + functions)`

---

## 📊 MEVCUT DURUM

| Bileşen | Konum | Durum |
|---------|-------|-------|
| Stage0-C Modülleri | `MELP/C/stage0/modules/` | ✅ 69 modül (P1'den) |
| Stage1-MLP Modülleri | `MELP/MLP/stage1/modules/` | ✅ 17 modül |
| STO Runtime | `MELP/runtime/sto/` | ✅ Import edildi |
| P2 Modülleri | `MELP/C/stage0/modules_p2_import/` | ⏳ Entegre edilmedi |
| Codegen (ASM→C) | - | ❌ YAPILMADI |

---

## 🔴 P0: HAZIRLIK (2-4 saat)

### P0.1: Proje Yapısını Doğrula
```bash
# Modüllerin doğru konumda olduğunu kontrol et
ls MELP/C/stage0/modules/ | wc -l  # 69 olmalı
ls MELP/MLP/stage1/modules/ | wc -l  # 17 olmalı
```

- [ ] 69 C modülü mevcut
- [ ] 17 MLP modülü mevcut
- [ ] STO runtime mevcut
- [ ] Makefile'lar çalışıyor

### P0.2: GCC Derleme Testi
```bash
cd MELP/C/stage0/modules/lexer
make clean && make
```

- [ ] Her modül bağımsız derlenebiliyor
- [ ] Hiçbir LLVM bağımlılığı yok
- [ ] Header'lar düzgün include ediliyor

### P0.3: Bağımlılık Grafiği
```
lexer → token_types
parser → lexer, ast_nodes
codegen → parser, ast_nodes  ← BU ASM→C OLACAK!
```

- [ ] Bağımlılık döngüsü yok
- [ ] Include path'ler doğru

---

## 🔴 P1: CODEGEN ASM→C DÖNÜŞÜMÜ (4-8 saat) ⚠️ KRİTİK

**Bu en önemli adım!** Mevcut codegen modülleri x86-64 ASM üretiyor. Bunları C üretecek şekilde değiştirmeliyiz.

### P1.1: Mevcut Codegen Analizi

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

### P1.2: C Codegen Tasarımı

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

### P1.3: Temel C Üretimi

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

### P1.4: STO Tip Mapping

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

### P1.5: Codegen Test

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

## 🟡 P2: BOOTSTRAP TEST (2-3 saat)

### P2.1: Stage0-C Compiler Build

```bash
cd MELP/C/stage0
make clean && make
# veya
./build.sh
```

- [ ] Tüm modüller derlendi
- [ ] `mlpc` binary oluştu
- [ ] Hata/warning yok

### P2.2: Basit MLP Derleme

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

### P2.3: Kapsamlı Test

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

## 🟡 P3: STAGE1-MLP DERLEME (3-4 saat)

### P3.1: MLP Modüllerini Derle

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

### P3.2: Stage1 Linker

```bash
# Tüm object dosyalarını birleştir
gcc *.o -o mlpc_stage1 -L../../runtime -lmlp_runtime
```

- [ ] Linking başarılı
- [ ] `mlpc_stage1` binary oluştu

### P3.3: Stage1 Doğrulama

```bash
# Stage1 compiler'ı test et
./mlpc_stage1 test.mlp -o test.c
gcc test.c -o test
./test
```

- [ ] Stage1 compiler çalışıyor
- [ ] Aynı çıktıyı üretiyor

---

## 🟢 P4: SELF-HOSTING (2-3 saat)

### P4.1: Bootstrap Zinciri

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

### P4.2: Eşitlik Kontrolü

```bash
# Stage1 ve Stage2 aynı C çıktısı üretmeli
diff stage1_output.c stage2_output.c
# Fark olmamalı!
```

- [ ] Stage1 == Stage2
- [ ] Binary'ler aynı çıktı üretiyor

### P4.3: Final Doğrulama

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
