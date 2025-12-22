# SELF-HOSTING YZ - BURADAN BAŞLA

**Son Güncelleme:** 23 Aralık 2025 (YZ_09)  
**Üst Akıl:** Opus  
**Ana TODO:** `/TODO_SELFHOSTING_FINAL.md`  
**Kurallar:** `/TODO_kurallari.md`

---

## 🚨 GÜNCEL DURUM (23 Aralık 2025 - YZ_09)

**🎉 YZ_09 TAMAMLANDI! Arithmetic + Comparison Operatörler + If-Statement Eklendi!**

**Phase 3.4 Tamamlandı:**
- ✅ Arithmetic operators: -, *, / (sub, mul, sdiv)
- ✅ Comparison operators: >, <, == (icmp sgt/slt/eq)
- ✅ If-statement: if-then-end_if (basic blocks + br)
- ✅ AST nodes: BINOP (11), COMPOP (13), IF (14)
- ✅ All tests passed: arithmetic, comparison, control flow ✓

**Test Sonuçları:**
- ✅ Subtraction: 50 - 8 = 42 ✓
- ✅ Multiplication: 6 * 7 = 42 ✓
- ✅ Division: 84 / 2 = 42 ✓
- ✅ Comparison: 50 > 8, 5 < 10, 42 == 42 ✓
- ✅ If-statement: control flow with basic blocks ✓

---

## 📋 ÖNCEDEN TAMAMLANANLAR

**YZ_08 TAMAMLANDI:**

---

## 📋 ÖNCEDEN TAMAMLANANLAR

**YZ_08 TAMAMLANDI:**
- ✅ tokenize_source(): Mock tokens → gerçek tokenize_next() döngüsü
- ✅ parse_tokens(): Variable declarations (numeric x = 42)
- ✅ parse_tokens(): Arithmetic expressions (x + y)
- ✅ codegen_ast(): Variable allocation (alloca, store)
- ✅ codegen_ast(): Load/store operations
- ✅ codegen_ast(): Arithmetic operations (add)
- ✅ End-to-end test: "numeric x=10, y=32, return x+y" → exit code 42 ✓

---

## 🔵 YZ_10 SENİN GÖREVIN:

**Görev:** Phase 3 Bootstrap + Phase 4 Convergence

**Durum:** YZ_09 operatörleri ekledi ama bootstrap yapamadı (araç eksikliği)

---

### 🛠️ ADIM 1: Ortam Hazırlığı (1-2 saat)

#### 1.1. LLVM Runtime Kurulumu
```bash
# LLVM araçlarını kur
sudo apt update
sudo apt install llvm-14 llvm-14-runtime llvm-14-dev

# veya en son sürüm
sudo apt install llvm llvm-runtime

# Kontrol et
which lli    # /usr/bin/lli olmalı
which llc    # /usr/bin/llc olmalı
which opt    # /usr/bin/opt olmalı

lli --version  # LLVM version bilgisi
```

#### 1.2. compiler.mlp Düzeltmeleri
**Sorun:** Stage 0 compiler main() fonksiyonunu derlemiyor

**Çözüm seçenekleri:**
1. **Basit yaklaşım**: compiler.mlp'yi Stage 0 için uyarla
2. **Alternatif**: C'de basit bir wrapper yaz (main.c)
3. **En iyi**: compiler_integration.mlp'ye main ekle

**Test için:**
```bash
# compiler.mlp'yi derle
./compiler/stage0/modules/functions/functions_compiler \
  modules/compiler.mlp build/compiler_gen0.ll

# Main fonksiyonu var mı kontrol et
grep "define.*@main" build/compiler_gen0.ll

# Yoksa: compiler.mlp'yi düzelt veya wrapper ekle
```

---

### 🚀 ADIM 2: Bootstrap Süreci (4-6 saat)

#### 2.1. Gen1 Oluştur (Stage 0 ile)
```bash
# compiler.mlp'yi Stage 0 ile derle
./compiler/stage0/modules/functions/functions_compiler \
  modules/compiler.mlp build/stage1_gen1.ll

# Gen1 oluştu mu kontrol et
ls -lh build/stage1_gen1.ll
grep "define.*@main" build/stage1_gen1.ll  # main olmalı!
```

#### 2.2. Gen1 Test Et
```bash
# Basit test programı
echo 'function main() returns numeric
    return 42
end_function' > /tmp/test.mlp

# Gen1 ile test programını derle
lli build/stage1_gen1.ll /tmp/test.mlp /tmp/test_output.ll

# Çıktıyı çalıştır
lli /tmp/test_output.ll
echo $?  # 42 olmalı!

# ✅ Gen1 çalışıyor → Devam et
```

#### 2.3. Gen2 Oluştur (Gen1 ile)
```bash
# Gen1 kullanarak compiler.mlp'yi tekrar derle
lli build/stage1_gen1.ll \
  modules/compiler.mlp \
  build/stage1_gen2.ll

# Gen2 oluştu mu kontrol et
ls -lh build/stage1_gen2.ll
```

#### 2.4. Gen3 Oluştur (Gen2 ile)
```bash
# Gen2 kullanarak compiler.mlp'yi tekrar derle
lli build/stage1_gen2.ll \
  modules/compiler.mlp \
  build/stage1_gen3.ll

# Gen3 oluştu mu kontrol et
ls -lh build/stage1_gen3.ll
```

---

### ✅ ADIM 3: Convergence Testi (1-2 saat)

#### 3.1. Gen2 ve Gen3 Karşılaştır
```bash
# Byte-level karşılaştırma
diff build/stage1_gen2.ll build/stage1_gen3.ll

# Boş çıktı = BAŞARI! 🎉
# Farklılık var = Analiz gerekli
```

#### 3.2. Convergence Analizi
```bash
if [ "$(diff build/stage1_gen2.ll build/stage1_gen3.ll)" == "" ]; then
    echo "🎉 CONVERGENCE SAĞLANDI!"
    echo "Stage 1 compiler stable - self-hosting TAMAMLANDI!"
else
    echo "⚠️ Gen2 ve Gen3 farklı"
    echo "Fark analizi:"
    diff -u build/stage1_gen2.ll build/stage1_gen3.ll | head -50
fi
```

---

### 🧪 ADIM 4: Final Validation (1 saat)

#### 4.1. Gen3 ile Test Programları Derle
```bash
# Arithmetic test
echo 'function main() returns numeric
    numeric x = 10
    numeric y = 32
    return x + y
end_function' > /tmp/test_arith.mlp

lli build/stage1_gen3.ll /tmp/test_arith.mlp /tmp/test_arith.ll
lli /tmp/test_arith.ll
echo $?  # 42 olmalı

# If-statement test (YZ_09'un eklediği feature)
echo 'function main() returns numeric
    numeric x = 50
    numeric y = 10
    if x > y then
        return 1
    end_if
    return 0
end_function' > /tmp/test_if.mlp

lli build/stage1_gen3.ll /tmp/test_if.mlp /tmp/test_if.ll
lli /tmp/test_if.ll
echo $?  # 1 olmalı
```

---

### 🎯 Başarı Kriterleri

```
Phase 3 Bootstrap:
[ ] LLVM runtime kurulu (lli çalışıyor)
[ ] compiler.mlp Stage 0 ile derlendi (Gen1 oluştu)
[ ] Gen1 test programlarını derleyebiliyor
[ ] Gen1 ile Gen2 oluşturuldu
[ ] Gen2 çalışıyor

Phase 4 Convergence:
[ ] Gen2 ile Gen3 oluşturuldu
[ ] diff Gen2 Gen3 → boş (convergence sağlandı)
[ ] Gen3 test programları derleyebiliyor
[ ] Tüm testler geçti

🎉 Self-hosting TAMAMLANDI!
```

---

### ⚠️ Önemli Notlar:

**YZ_09'dan Devralınan:**
- ✅ Arithmetic operators: +, -, *, / (LLVM: add, sub, mul, sdiv)
- ✅ Comparison operators: >, <, == (LLVM: icmp sgt/slt/eq)
- ✅ If-statement: if-then-end_if (basic blocks + br)
- ✅ AST nodes: BINOP (11), COMPOP (13), IF (14)
- ✅ Testler: Tüm operatörler validate edildi

**YZ_09'un Tamamlayamadığı:**
- ❌ Bootstrap (LLVM runtime eksik)
- ❌ Gen1/Gen2/Gen3 oluşturma (araç eksikliği)
- ❌ Convergence testi

**Sorunlar ve Çözümleri:**
1. **LLVM eksik**: `sudo apt install llvm-14 llvm-14-runtime` ile çöz
2. **compiler.mlp main()**: Stage 0 derleyemedi → düzelt veya wrapper ekle
3. **Stage 0 sınırlamaları**: Tam MELP syntax'ı desteklemiyor

**Alternatif Yaklaşım (Stage 0 sorunlu ise):**
- C'de basit main.c wrapper yaz
- compiler_integration.mlp'yi direkt kullan
- Manuel LLVM IR ile test et

---

## 📝 ÖNCEKİ YZ'LERDEN NOTLAR

**YZ_09 TAMAMLANDI (Kısmi):** ✅ (23 Aralık 2025)

**Yapılanlar:**
- ✅ Arithmetic operators: -, *, / eklendi
- ✅ Comparison operators: >, <, == eklendi  
- ✅ If-statement: if-then-end_if eklendi
- ✅ Parser: Tüm yeni operatörleri parse ediyor
- ✅ CodeGen: sub, mul, sdiv, icmp, br instructions
- ✅ Testler: Tüm operatör ve control flow testleri geçti (exit code 42)

**Tamamlanamadılar:**
- ❌ Bootstrap süreci (LLVM runtime kurulu değil)
- ❌ Gen1/Gen2/Gen3 oluşturma
- ❌ Convergence testi

**YZ_10'a Devredilen:**
- 🔧 LLVM runtime kurulumu (`lli`, `llc`, `opt`)
- 🔧 compiler.mlp düzeltmeleri (main fonksiyonu)
- 🔧 Bootstrap tam süreci (Gen1 → Gen2 → Gen3)
- 🔧 Convergence validation (diff Gen2 Gen3)
- 🎯 Self-hosting kanıtı

**Dosyalar:**
- ✅ `modules/compiler_integration.mlp`: 1104 satır (updated)
- ✅ `build/stage1_gen1.ll`: 671 satır (partial, no main)
- ✅ Test scripts: `temp/test_yz09_*.sh`

---

**YZ_08 TAMAMLANDI:**

Stage 1 compiler'ın kendini derleyebilmesi (self-hosting %100).

```
Stage 0 (C) ──compile──> Stage 1 (MELP) ──compile──> Stage 1' (MELP)
                                │                         │
                                └─────── AYNI ────────────┘
```

---

## 📋 GÖREV DAĞILIMI

| YZ | Phase | Görev | Durum | Branch |
|----|-------|-------|-------|--------|
| YZ_00 | Phase 0 | Sistem Tutarlılığı | ✅ TAMAMLANDI | `selfhosting_YZ_00` |
| YZ_01 | Phase 1.1-1.2 | Core + Parser Syntax | ✅ TAMAMLANDI | `selfhosting_YZ_01` |
| YZ_02 | Phase 1.3-1.5 | Kalan Modüller + While | ✅ TAMAMLANDI | `selfhosting_YZ_02` |
| YZ_03 + ÜA_00 | Phase 2 | Integration + Stage 0 Fix | ✅ TAMAMLANDI | `selfhosting_YZ_03` |
| YZ_04 | Phase 1.0 | 133 `then` Eksikliğini Düzelt | ✅ TAMAMLANDI | `selfhosting_YZ_04` |
| YZ_05 | Phase 2 | Pipeline Yapısı + Testler | ✅ TAMAMLANDI | `selfhosting_YZ_05` |
| YZ_06 | Phase 3.1 | Lexer/Parser/CodeGen Entegrasyonu | ✅ TAMAMLANDI | `selfhosting_YZ_06` |
| YZ_07 | Phase 3.2 | Parser/CodeGen Modül Entegrasyonu | ✅ TAMAMLANDI | `selfhosting_YZ_07` |
| YZ_08 | Phase 3.3 | Gerçek Lexer + Parser Genişletme | ✅ TAMAMLANDI | `selfhosting_YZ_08` |
| **YZ_09** | **Phase 3.4** | **Operatörler + Control Flow** | 🔵 **AKTİF** | `selfhosting_YZ_09` |
| YZ_10 | Phase 4 | Bootstrap ve Convergence | ⏳ BEKLEMEDE | `selfhosting_YZ_10` |

---
## 🔵 ŞU AN AKTİF GÖREV

### YZ_09: Phase 3.4 - Operatörler ve Control Flow

**Durum:** 🔵 AKTİF  
**Bağımlılık:** YZ_08 ✅ (tamamlandı)  
**Tahmini Süre:** 8-11 saat

**🎯 GÖREV:**

1. **Daha Fazla Operatör** (2-3 saat)
   - Subtraction: `-`
   - Multiplication: `*`
   - Division: `/`
   - Parser ve CodeGen güncellemeleri

2. **Control Flow (Opsiyonel)** (3-4 saat)
   - If statements
   - Comparison operators
   - LLVM IR basic blocks

3. **Function Calls (Opsiyonel)** (2-3 saat)
   - Simple function calls
   - Call instruction

4. **End-to-End Test** (1 saat)
   - Complex arithmetic test
   - Exit code validationtik)
- Import sistemi yok, fonksiyonları doğrudan çağır

---

## 📝 ÖNCEKİ YZ'DEN NOTLAR (YZ_06)

**YZ_06 Tamamlandı:** ✅ (22 Aralık 2025)

**Yapılanlar:**
- ✅ compiler.mlp: stub → gerçek implementasyon (tokenize_next döngüsü eklendi)
- ✅ compiler_integration.mlp: 3 faz gerçek API çağrıları yapıyor
- ✅ Lexer entegrasyonu: `tokenize_next()` döngüde çağrılıyor
- ✅ Parser entegrasyonu: `parse_tokens()` çağrısı eklendi
- ✅ CodeGen entegrasyonu: `codegen_ast()` çağrısı eklendi

**Test Sonuçları:**
- ✅ compiler.mlp: 12 functions → 555 lines LLVM IR
---

## 📝 ÖNCEKİ YZ'DEN NOTLAR (YZ_08)

**YZ_08 Tamamlandı:** ✅ (22 Aralık 2025)

**Yapılanlar:**
- ✅ tokenize_source(): Mock tokens → gerçek tokenize_next() döngüsü
- ✅ tokenize_next(): Basitleştirilmiş lexer (numbers, ids, keywords, operators)
- ✅ parse_tokens(): Variable declarations (numeric x = 42)
- ✅ parse_tokens(): Arithmetic expressions (x + y)
- ✅ codegen_ast(): Variable allocation (alloca, store, load)
- ✅ codegen_ast(): Arithmetic operations (add)
- ✅ 15 functions compiled successfully

**Test Sonuçları:**
- ✅ Test 1: Simple return (return 42) → exit code 42
- ✅ Test 2: Arithmetic (x=10, y=32, return x+y) → exit code 42
- ✅ LLVM IR geçerli (clang validation passed)
- ✅ Pipeline: Source → Tokenize → Parse → CodeGen → LLVM IR ✓

**AST Yapısı:**
```mlp
-- Function: [2; func_name; return_type; statements]
-- VARDECL: [10; var_name; var_value]
-- BINOP:   [11; operator; left; right]
-- RETURN:  [12; value_or_expr]
```

**Token Types:**
- Numbers: 30, Identifiers: 10, Keywords: 1-7, 20-21
- Operators: `(` 40, `)` 41, `=` 50, `+` 51

**Önemli Bulgu:**
- Gerçek lexer çalışıyor (tokenize_next loop)
- Variable declarations ve arithmetic expressions parse ediliyor
- LLVM IR generation çalışıyor (alloca, load, store, add)
- **Sonraki adım:** Daha fazla operatör (-, *, /) ve control flow

**Araçlar:**
- `temp/test_yz08_e2e.sh`: End-to-end test script
- `temp/test_arithmetic_manual_yz08.ll`: Manuel LLVM IR test

---

## 📝 ÖNCEKİ YZ'DEN NOTLAR (YZ_07)9 lines LLVM IR
- ✅ codegen_integration.mlp: compiled → 1085 lines LLVM IR
- ✅ Toplam: 3138 satır LLVM IR
- ✅ Tüm modüller başarıyla derlendi (5/5)

**Önemli Bulgu:**
- Pipeline yapısı hazır, stub'lar kaldırıldı
- `tokenize_next()` döngüde çağrılıyor, tokenization çalışıyor
- Parser ve CodeGen minimal AST/IR üretiyor
- **Sonraki adım:** Parser ve CodeGen detaylarını entegre et

---

## 📝 ÖNCEKİ YZ'DEN NOTLAR (YZ_05)

**YZ_05 Tamamlandı:** ✅ (22 Aralık 2025)

**Yapılanlar:**
- ✅ compiler.mlp modernize edildi (stub → pipeline yapısı)
- ✅ `compile_source()` fonksiyonu 3 fazlı pipeline haline getirildi
- ✅ Pipeline testleri: basit (return 42), fonksiyon çağrısı, control flow
- ✅ 102/107 modül derleniyor (%95 başarı)
- ✅ Production modülleri %100 çalışıyor

**Test Sonuçları:**
- ✅ hello_simple.mlp: 1 function compiled
- ✅ func_call.mlp: 2 functions compiled
- ✅ control.mlp: 1 function compiled (while syntax düzeltildi - `do` yok)
- ✅ Toplu derleme: 102/107 başarılı

**Önemli Bulgu: While Syntax**
```pmpl
-- YANLIŞ:
while i < 10 do
    ...
end_while

-- DOĞRU:
while i < 10
    ...
end_while
```
- PMPL'de `while` sonra `do` yok!
- `pmlp_kesin_sozdizimi.md` doğrulandı

**Hazır API'ler:**
- `lexer.mlp`: `tokenize_next(source; pos; line; col)` → [token; new_pos; new_col; new_line]
- `compiler_integration.mlp`: `tokenize_source(source)`, `parse_tokens(tokens)`, `codegen_ast(ast)`
- `codegen_integration.mlp`: `codegen_expression()`, `codegen_statement()`

**Araçlar:**
- `temp/test_stage1_yz05.sh`: Toplu derleme script (107 modül)
- Test dosyaları: hello_simple.mlp, func_call.mlp, control.mlp

---

## 📝 ÖNCEKİ YZ'DEN NOTLAR (YZ_04)

**YZ_04 Tamamlandı:** ✅ (22 Aralık 2025)

**Yapılanlar:**
- ✅ 133 çok satırlı `then` eksikliği düzeltildi
- ✅ 6 dosya tamamen düzeltildi:
  - control_flow_parser.mlp (42), operators_codegen.mlp (41)
  - test_control_flow.mlp (19), test_operators.mlp (17)
  - control_flow_codegen.mlp (12), type_mapper.mlp (2)
- ✅ Python script ile otomatik düzeltme (45 dakika)
- ✅ Tüm düzeltmeler test edildi ve derlendi

**Test Sonuçları:**
- ✅ operators_codegen.mlp: 25 functions compiled
- ✅ type_mapper.mlp: 3 functions compiled
- ✅ Final doğrulama: 0 kalan `then` eksikliği

**Önemli Bulgu:**
- ⚠️ operators_parser.mlp'de parse hataları (274, 279, 284, 390)
- Bu `then` eksikliği değil, fonksiyon tanımı sorunu
- YZ_05 bu dosyayı öncelikli düzeltmeli

**Araçlar:**
- Python regex script (çok satırlı if tespiti ve düzeltme)
- Stage 0 compiler ile test (timeout 30s)

---

## 📝 ÖNCEKİ YZ'DEN NOTLAR (YZ_02)

**YZ_02 Tamamlandı:** ✅ (22 Aralık 2025)

**Yapılanlar:**
- ✅ Task 1.3: 12 modül syntax fix (compiler ana modüller, arrays, control_flow, enums)
- ✅ Task 1.4: 52 while syntax fix (while X → while X do), 20 dosya
- ✅ Task 1.5: Test ve doğrulama (7/9 başarılı)
- ✅ **Toplam 77+ modül düzeltildi** (YZ_01: 65+ | YZ_02: 12)

**Syntax Düzeltmeleri:**
- Virgül → Semicolon: ~400+ değişiklik
- While do ekleme: 52 değişiklik
- Blok sonları: ~100+ değişiklik
- Boolean → numeric: (devam)
- exit/break düzeltmeleri

**Test Sonuçları:**
- ✅ compiler.mlp, compiler_integration.mlp, compiler_full.mlp: Derlenmiş (78KB toplam)
- ✅ arrays_codegen.mlp, arrays_parser.mlp: Derlenmiş (19KB)
- ✅ control_flow_parser.mlp, enums_parser.mlp: Derlenmiş (26KB)
- ⚠️ control_flow_codegen.mlp, enums_codegen.mlp: Import errors

**Araçlar:**
- `temp/fix_syntax_advanced.py` - Kapsamlı syntax fixer
- `temp/fix_while.py` - While do fixer
- `temp/test_stage1_modules.sh` - Test scripti
- `temp/compilation_results_yz02.txt` - Detaylı rapor

**Bilinen Sorunlar:**
- 2 modül import edilen dosyalardaki syntax sorunları nedeniyle derlenemiyor
- ~30 modül henüz kontrol edilmedi (test dosyaları, yardımcı modüller)

---

## 📝 ÖNCEKİ YZ'DEN NOTLAR (YZ_01)

**YZ_01 Tamamlandı:** ✅ (22 Aralık 2025)

**Yapılanlar:**
- ✅ lexer_mlp: lexer.mlp düzeltildi (6 fonksiyon derlenmiş)
- ✅ parser_mlp: 28 modül syntax fix (toplu düzeltme)
- ✅ codegen_mlp: 17 modül syntax fix (toplu düzeltme)
- ✅ **Toplam 65+ modül düzeltildi**

**Syntax Düzeltmeleri:**
- Virgül → Semicolon: ~300+ değişiklik
- Blok sonları: ~200+ değişiklik (end_if, end_while, vb.)
- Boolean → numeric: ~50 değişiklik (STO prensibi)
- exit while → exit: ~20 değişiklik

**Önemli Bulgular:**
- ✅ Stage 0 semicolon'u TAM destekliyor
- ✅ Array literal'lerde semicolon zorunlu: `[a; b; c]`
- ✅ Fonksiyon parametreleri/çağrıları: semicolon
- ⚠️ Bazı modüller kısmen derlenmiş (hata var ama output üretiyor)

**Araçlar:**
- `temp/fix_syntax_complete.py` oluşturuldu
- Python script ile toplu düzeltme çok hızlı

**Bilinen Sorunlar:**
- Bazı modüller "println not found" hatası veriyor (runtime dependency)
- While syntax (32 adet `while X` do eksik) henüz düzeltilmedi

---

## 📝 ÖNCEKİ YZ'DEN NOTLAR (YZ_00)

**YZ_00 Tamamlandı:** ✅ (22 Aralık 2025)

**Bulguları:**
- ✅ Stage 0 build ve test başarılı
- ✅ Import sistemi çalışıyor
- ⚠️ 89/107 Stage 1 modülü syntax fix gerekiyor (%83)
- 1,104 virgül → semicolon değişikliği
- 32 `while` → `while do` değişikliği
- 10 `break` → `exit` değişikliği

**En Çok Sorun Olan Modüller:**
1. codegen_mlp/codegen_arrays.mlp - 52 virgül
2. codegen_mlp/codegen_functions.mlp - 45 virgül
3. codegen_mlp/codegen_structs.mlp - 38 virgül
4. parser_mlp/parser_expressions.mlp - 34 virgül

**Önemli:** `temp/syntax_inventory.txt` detaylı rapor (982 satır)

**Bilinen durumlar:**
- Stage 1 modüllerinde eski syntax var (virgül kullanımı)
- 98/107 modül derleniyor (%92)
- `lexer.mlp`'de substring() çağrıları düzeltilmeli

---

## ⚠️ KRİTİK KURALLAR

### Git Workflow

```bash
# 1. Branch oluştur
git checkout -b selfhosting_YZ_XX

# 2. Çalış ve commit et
git add .
git commit -m "YZ_XX: [açıklama]"

# 3. Push et
git push origin selfhosting_YZ_XX

# ⚠️ MERGE YAPMA! Pull request AÇMA!
```

### Zorunlu Okumalar

Göreve başlamadan önce oku:
1. `TODO_kurallari.md` - Tüm kurallar
2. `TODO_SELFHOSTING_FINAL.md` - Detaylı görev listesi
3. `pmlp_kesin_sozdizimi.md` - Syntax referans
4. `MELP_VISION.md` - Vizyon
5. `MELP_REFERENCE.md` - Referans
6. `ARCHITECTURE.md` - Mimari

## 📊 GENEL İLERLEME

```
Phase 0: [✅] [✅] [✅] [✅]         4/4  (YZ_00 ✅)
Phase 1: [✅] [✅] [✅] [✅] [✅]    5/5  (YZ_01 ✅ | YZ_02 ✅ | YZ_04 ✅)
Phase 2: [ ] [ ] [ ] [ ] [ ]       0/5  (YZ_05 🔵)
Phase 3: [ ] [ ] [ ]               0/3 
Phase 4: [ ] [ ] [ ]               0/3 
Phase 5: [ ] [ ] [ ] [ ]           0/4 

TOPLAM: 9/24 task (38%)
```

**Prensip ihlali tespit edersen: DURDUR ve Üst Akıl'a danış!**

---

## 🚀 BAŞLA!

1. `TODO_kurallari.md` oku ✓
2. Bu dosyayı oku ✓
3. `TODO_SELFHOSTING_FINAL.md` oku
4. Zorunlu belgeleri oku
5. Kullanıcıya kendini tanıt ve onay al
6. Branch oluştur
7. Çalış
8. Rapor yaz
9. Push et
10. Bu dosyayı güncelle

**Başarılar!**
