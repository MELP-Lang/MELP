# 🤖 YZ_01 GÖREV PLANI

**Tarih:** 29 Aralık 2025  
**YZ:** SELFHOSTING_YZ_01  
**Durum:** ⏳ Atanmadı  
**Öncelik:** 🔴 KRİTİK

---

## 🎯 SENİN GÖREVİN

**PHASE 0 + PHASE 1: CODEGEN ASM→C DÖNÜŞÜMÜ**

Sen bu projenin en kritik kısmını yapacaksın! Codegen modüllerini ASM üretiminden C üretimine çevireceksin.

**Tahmini Süre:** 6-12 saat  
**Zorluk:** ⭐⭐⭐ Zor ama yapılabilir

---

## 📋 GÖREV LİSTESİ

### ✅ PHASE 0: HAZIRLIK (2-4 saat)

#### P0.1: Proje Yapısını Doğrula
```bash
ls MELP/C/stage0/modules/ | wc -l  # 69 olmalı
ls MELP/MLP/stage1/modules/ | wc -l  # 17 olmalı
```

- [ ] 69 C modülü mevcut
- [ ] 17 MLP modülü mevcut
- [ ] STO runtime mevcut
- [ ] Makefile'lar çalışıyor

#### P0.2: GCC Derleme Testi
```bash
cd MELP/C/stage0/modules/lexer
make clean && make
```

- [ ] Her modül bağımsız derlenebiliyor
- [ ] Hiçbir LLVM bağımlılığı yok
- [ ] Header'lar düzgün include ediliyor

#### P0.3: Codegen Analizi
```bash
cd MELP/C/stage0/modules
find . -name "*codegen*" -type d
grep -r "emit.*mov\|push\|pop" codegen*/
```

- [ ] Codegen dizinlerini listele
- [ ] ASM emit() çağrılarını bul
- [ ] Hangi dosyalar değişmeli? (liste oluştur)

---

### 🔴 PHASE 1: CODEGEN ASM→C (4-8 saat)

#### P1.1: emit_c() Fonksiyonu (30-60 dk)

**Dosyalar:** 
- `MELP/C/stage0/modules/codegen/codegen_emit.c`
- `MELP/C/stage0/modules/codegen/codegen_emit.h`

**Görev:**
```c
// codegen_emit.h'ye ekle:
void emit_c(const char* format, ...);

// codegen_emit.c'ye ekle:
void emit_c(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
}
```

- [ ] `emit_c()` fonksiyonu yazıldı
- [ ] Test edildi: `emit_c("int x = 5;")` → stdout'a yazıyor
- [ ] Header'da tanımlandı

#### P1.2: Print Statement Dönüşümü (1-2 saat)

**Dosya:** `MELP/C/stage0/modules/codegen/codegen.c` veya `codegen_print.c`

**ESKİ (ASM):**
```c
emit("    mov rdi, fmt_string");
emit("    mov rsi, %s", str);
emit("    call printf");
```

**YENİ (C):**
```c
emit_c("printf(\"%%s\\n\", %s);", str);
```

- [ ] Print statement ASM→C dönüştü
- [ ] String literal'ler escape ediliyor
- [ ] Test edildi: `print("hello")` çalışıyor

#### P1.3: Variable Declaration (2-3 saat)

**Dosya:** `MELP/C/stage0/modules/codegen/codegen_variables.c` veya benzeri

**Dönüşüm:**
```
MLP: x = 42
ASM: mov rax, 42; mov [rbp-8], rax
C: int64_t x = 42;
```

**STO Tip Mapping:**
- Küçük integer → `int64_t`
- Ondalık → `double`
- Büyük sayı → `mlp_bigdecimal_t`
- Kısa string → `char*`
- Uzun string → `mlp_string_t`

- [ ] numeric → int64_t/double dönüşümü
- [ ] String → char*/mlp_string_t dönüşümü
- [ ] Değişken tanımlama C olarak emit ediliyor

#### P1.4: Control Flow (2-3 saat)

**Dosya:** `MELP/C/stage0/modules/codegen/codegen_control.c` veya benzeri

**Dönüşümler:**

| MLP | ASM (ESKİ) | C (YENİ) |
|-----|------------|----------|
| `if x > 0` | `cmp rax, 0; jg label` | `if (x > 0) {` |
| `while x < 10` | `cmp rax, 10; jl loop` | `while (x < 10) {` |
| `for i in 1..10` | `mov rcx, 1; ...` | `for (int64_t i = 1; i <= 10; i++) {` |

- [ ] if statement dönüştü
- [ ] while loop dönüştü
- [ ] for loop dönüştü
- [ ] else/elif dönüştü

#### P1.5: Functions (2-3 saat)

**Dosya:** `MELP/C/stage0/modules/codegen/codegen_functions.c`

**Dönüşüm:**
```
MLP: function topla(a, b) { return a + b }

C: int64_t topla(int64_t a, int64_t b) {
    return a + b;
}
```

- [ ] Function signature dönüştü
- [ ] Parametreler tipleşti
- [ ] Return statement dönüştü
- [ ] Function call'lar dönüştü

#### P1.6: Expressions (1-2 saat)

**Dosya:** `MELP/C/stage0/modules/codegen/codegen_expression.c`

**Dönüşümler:**

| MLP | C |
|-----|---|
| `x + y` | `x + y` |
| `x * y` | `x * y` |
| `x > y` | `x > y` |
| `x == y` | `x == y` |
| `x and y` | `x && y` |
| `x or y` | `x \|\| y` |

- [ ] Aritmetik operatörler dönüştü
- [ ] Karşılaştırma operatörleri dönüştü
- [ ] Mantıksal operatörler dönüştü

---

## ✅ TEST & COMMIT

### Test

```bash
# P1 bittiğinde test:
cd MELP/C/stage0
make clean && make

# Basit test:
echo 'print("Hello from C codegen!")' > test.mlp
./mlpc test.mlp -o test.c
gcc test.c -o test
./test  # "Hello from C codegen!" yazmalı
```

- [ ] Derleme başarılı
- [ ] Test çalışıyor
- [ ] C kodu okunabilir

### Commit

```bash
git add MELP/C/stage0/modules/codegen/
git commit -m "SELFHOSTING: Phase 1 - Codegen ASM→C dönüşümü tamamlandı

- emit_c() fonksiyonu eklendi
- Print statement C olarak emit ediliyor
- Variable declaration C syntax kullanıyor
- Control flow (if/while/for) C olarak üretiliyor
- Functions C fonksiyonları olarak emit ediliyor
- Expressions C operatörleri kullanıyor

Test: Basic MLP→C compilation başarılı"
```

- [ ] Commit yapıldı
- [ ] Commit mesajı açıklayıcı

---

## 📊 İLERLEME RAPORU

**Her gün sonunda YZ_RAPOR_TEMPLATE.md'yi doldur ve Üst Akıl'a bildir!**

```markdown
## YZ_01 Günlük Rapor - [Tarih]

### Tamamlanan:
- [ ] P0.1: Proje yapısı doğrulandı
- [ ] P0.2: GCC derleme testi başarılı
- [ ] ...

### Devam Eden:
- [ ] P1.3: Variable declaration (60% tamamlandı)

### Engeller:
- Yok / [Açıklama]

### Sonraki Adım:
- P1.3'ü bitir, P1.4'e geç
```

---

## 🚨 ÖNEMLİ NOTLAR

1. **Modüler Kal:** Her dosya max 500 satır
2. **Test Et:** Her değişiklikten sonra derle ve test et
3. **Commit Sık:** Her major değişiklik = 1 commit
4. **Dokümante Et:** Ne yaptığını yaz
5. **Takıldığında:** Üst Akıl'a sor, 2 saatten fazla takılma!

---

## 📚 REFERANSLAR

- `TODO_SELFHOSTING.md` - Ana TODO (hepsini oku!)
- `Danışman_Yol_Haritası.md` - 5 Temel Esas (YASAK: monolitik, ASM, vb.)
- `MELP/C/stage0/modules/codegen/` - Codegen modülleri
- `YZ_HIZLI_REFERANS.md` - PMLP syntax

---

**İYİ ÇALIŞMALAR! SEN BU İŞİN KRİTİK KISMINI YAPIYORSUN! 🚀**

*Üst Akıl seni destekliyor!* 🧠
