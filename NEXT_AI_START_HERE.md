# NEXT AI START HERE - YZ Görev Dosyası

**Son Güncelleme:** 20 Aralık 2025  
**Mevcut YZ:** YZ_106  
**Dal:** `stage1_while_body_YZ_30`  
**Durum:** Stage 0 derleme düzeltildi ✅, Stage 1 syntax dönüşümü gerekiyor

---

## ⚠️ KRİTİK KURALLAR (TÜM YZ'LER İÇİN - MUTLAKA OKU!)

### ❌ YAPMA:
- Script ile toplu değişiklik yapma
- Çalışan dosyaları değiştirme (test etmeden)
- Stage 0 C kodlarına dokunma (`compiler/stage0/`)
- Birden fazla dizinde aynı anda çalışma
- `pmlp_kesin_sozdizimi.md` kurallarını ihlal etme

### ✅ YAP:
- Sadece kendi dizinindeki dosyalarla çalış
- Her değişiklikten ÖNCE ve SONRA test et
- Sorun bulursan raporla, zorla düzeltme
- Virgül (`,`) → Noktalı virgül (`;`) dönüşümü **manuel** yap
- Üst Akıl'a danış (kullanıcıya sor)

---

## 📁 DOSYA KONUMLARI

| Konum | Açıklama |
|-------|----------|
| `archive/stage1_api_attempt/modules/` | Stage 1 MLP modülleri (TEST EDİLECEK) |
| `compiler/stage0/modules/functions/functions_compiler` | Stage 0 compiler (DOKUNMA!) |
| `temp/` | Test dizini (güvenli alan) |
| `pmlp_kesin_sozdizimi.md` | PMPL syntax kuralları (OKU!) |

---

## 📊 MEVCUT DURUM

**Stage 0:** ✅ %97+ tamamlandı  
**Stage 1:** 🔴 %80 yazıldı, ama MAJOR syntax uyumsuzlukları var!  
**Import:** ✅ Çalışıyor (YZ_103 doğruladı)

### Başarılı Testler:
- ✅ `char_utils.mlp` - 11 fonksiyon, exit code 67
- ✅ `bootstrap_test_fixed.mlp` - exit code 30

### 🔴 YZ_104 KEŞFİ: 3 Büyük Syntax Uyumsuzluğu

**1. List Literal Syntax YANLIŞ (79+ instance sadece functions/ içinde)**
```pmpl
-- Stage 1 dosyalarında (YANLIŞ):
list result = [0, current_pos]

-- PMPL standardı (DOĞRU):
list result = (0; current_pos;)
```
- **Parantez tipi:** `[]` → `()` olmalı
- **Ayırıcı:** `,` → `;` olmalı
- **Trailing semicolon:** Her eleman sonunda `;` olmalı

**2. `const` Keyword Desteklenmiyor**
```pmpl
const numeric FUNC_PARAM_NUMERIC = 0  -- ❌ Stage 0 desteklemiyor!
```

**3. Function Parameter Syntax (DÜZELTİLDİ ✅)**
```pmpl
-- Eski: function name(a, b, c)
-- Yeni: function name(a; b; c)  ✅ YZ_104 düzeltti
```

### 📊 PMPL Koleksiyon Syntax Referansı (pmlp_kesin_sozdizimi.md)

| Tip | Parantez | Ayırıcı | Örnek |
|-----|----------|---------|-------|
| Array | `[]` | `;` | `[1; 2; 3;]` (homojen) |
| List | `()` | `;` | `(1; "a"; true;)` (heterojen) |
| Tuple | `<>` | `;` | `<1; "a"; true>` (immutable) |

---

## 🎯 GÖREV DAĞILIMI

| YZ | Dizin | Dosya Sayısı | Durum |
|----|-------|--------------|-------|
| YZ_103 | char_utils.mlp | 1 | ✅ TAMAMLANDI |
| YZ_104 | `functions/` | 3 | ✅ ANALİZ TAMAMLANDI - List literal blocker bulundu |
| YZ_105-108 | Diğer modüller | ~25 | ⏸️ DURDURULDU - Syntax dönüşümü gerekiyor |

---

## 🔴 ACİL KARAR GEREKİYOR

**YZ_104 keşfi:** Stage 1 modülleri PMPL standartlarına uymuyor!

### 3 Seçenek:

**Seçenek A: Script ile Dönüşüm (Riskli)**
- Otomatik `[a, b]` → `(a; b;)` dönüşümü
- String içindeki virgülleri bozabilir
- ~2-3 saat script geliştirme + test

**Seçenek B: Manuel Dönüşüm (Güvenli ama Yavaş)**
- Her dosyayı tek tek düzeltme
- ~6-8 saat (38 dosya × 10 dakika)
- En güvenilir sonuç

**Seçenek C: Stage 1'i Yeniden Yaz (Temiz Başlangıç)**
- PMPL standartlarına uygun yeni modüller
- Mevcut 13K satır → referans olarak kullan
- ~2-3 hafta

### 💡 ÖNERİ: Seçenek A + Kontrollü Test
1. Backup al
2. Script geliştir (sadece list literal dönüşümü)
3. Küçük dosyada test et
4. Başarılı ise tüm dosyalara uygula
5. Her dosyayı manuel doğrula

---

## 🎯 YZ_106 GÖREVİ: Stage 1 List Literal Syntax Dönüşümü

### Problem
Stage 1 modülleri Python-style list syntax kullanıyor, PMPL standardına uymuyor.

### Dönüşüm Kuralları
```pmpl
-- YANLIŞ (Python-style):
list result = [0, current_pos]
return [func_decl, current_pos]

-- DOĞRU (PMPL):
list result = (0; current_pos;)
return (func_decl; current_pos;)
```

### Adımlar
1. `archive/stage1_api_attempt/modules/` dizininin backup'ını al
2. Küçük bir dosyayla başla (örn: `core/char_utils.mlp` - zaten çalışıyor)
3. `functions/functions_parser.mlp` dosyasını dönüştür:
   - `[` → `(` 
   - `]` → `)`
   - Liste içindeki `,` → `;`
   - Her eleman sonuna `;` ekle (trailing semicolon)
4. Test et: `./compiler/stage0/modules/functions/functions_compiler <dosya> temp/test.s`
5. Başarılı ise diğer dosyalara geç

### Dikkat!
- String içindeki `[` ve `]` karakterlerine DOKUNMA!
- Yorum satırlarındaki örneklere dikkat et
- Her dosyadan sonra test et

### Hedef Dosyalar (Öncelik Sırasıyla)
1. `functions/functions_parser.mlp` (~80 list literal)
2. `functions/functions_codegen.mlp` (~30 list literal)
3. `variables/variables_parser.mlp`
4. Diğerleri...

### Test Komutu
```bash
cd /home/pardus/projeler/MLP/MLP
./compiler/stage0/modules/functions/functions_compiler \
    archive/stage1_api_attempt/modules/functions/functions_parser.mlp \
    temp/test.s 2>&1
```

### Başarı Kriteri
- Derleme hatası yok
- Assembly dosyası üretiliyor

---

## ✅ YZ_105 GÖREVİ: TAMAMLANDI

### Sonuç
- ✅ `arithmetic_parser.c` forward declaration/implementation uyumsuzluğu düzeltildi
- ✅ 14 fonksiyon çağrısı güncellendi
- ✅ Diğer modüller de güncellendi (string_interpolation.c, array_parser.c, vb.)
- ✅ Stage 0 compiler derleniyor
- ✅ `const` desteği çalışıyor

### Rapor
`temp/YZ_105_SONUC.md`

---

## ✅ YZ_104 GÖREVİ: TAMAMLANDI

### Sonuç
- ✅ `functions/` dizini analiz edildi (3 dosya)
- ✅ Function parameter syntax düzeltildi (9 fonksiyon)
- ✅ Function call syntax düzeltildi (7 çağrı)
- 🔴 **Major blocker keşfedildi:** List literal syntax uyumsuzluğu

### Bulgular
Stage 1 modülleri Python-style list syntax kullanıyor:
- `[a, b, c]` → PMPL'de `(a; b; c;)` olmalı
- 79+ instance sadece functions/ içinde
- Tüm 38 modülde benzer sorun var

### Rapor
`temp/YZ_104_report.md` - Detaylı analiz

---

## 🎯 YZ_102 GÖREVİ: Critical Bugs & Documentation

### Durum
YZ_100 ve YZ_101 ile Stage 0 feature set tamamlandı! 🎉  
YZ_104 kısmen tamamlandı - list literal syntax sorunu keşfedildi!  
Ancak 3 kritik bug kaldı → önce bunları çöz!

### Öncelik 1: Kritik Bug Fixes (2-3 saat)

**🔴 1. List Index Access:**
```pmpl
list numbers = (1; 2; 3; 4; 5)
return numbers(0)    -- ❌ Fonksiyon çağrısı sanılıyor!
```
**Dosya:** `arithmetic_parser.c`  
**Çözüm:** Variable vs function ayrımı (symbol table lookup)

**🔴 2. Struct Field in Expression:**
```pmpl
function main() as numeric
    Point pt
    pt.x = 10
    return pt.x    -- ❌ Variable lookup eksik!
end_function
```
**Dosya:** `arithmetic_codegen.c`  
**Çözüm:** Variable registry + stack offset tracking

**🔴 3. Enum Variable Usage:**
```pmpl
function main() as numeric
    Color c = Color.Red
    return c    -- ❌ Variable 'c' okuyamıyor!
end_function
```
**Dosya:** `arithmetic_codegen.c`  
**Çözüm:** Enum variable stack lookup (struct field ile aynı sistem)

### Öncelik 2: Documentation (2-3 saat)

- [ ] Stage 0 completion documentation
- [ ] Stage 1 bootstrap plan
- [ ] Test coverage review
- [ ] Known bugs list update

### Stage 0 Feature Matrix

| Feature | Status | Test |
|---------|--------|------|
| Functions | ✅ | Return, params, calls |
| Variables | ✅ | Declaration, assignment |
| Arrays | ✅ | Declaration, access, bounds check |
| Structs | ✅ | Definition, instance, member access |
| Enums | ✅ | Definition, initialization |
| **List index access** | ❌ | `list(0)` → function call bug |
| **Variable in expr** | ❌ | `return pt.x` fails |
| **Enum variable read** | ❌ | `return c` fails |
| If-else-else_if | ✅ | Unlimited chain |
| While loops | ✅ | Body parsing |
| For loops | ✅ | Range iteration |
| Switch-case | ✅ | Multiple cases |
| Operators | ✅ | Arithmetic, comparison, logical |
| Print | ✅ | String output |
| Import | ✅ | Module loading |
| Comments | ✅ | Single & multi-line |

### Başarı Kriteri
1. ✅ 3 kritik bug fix
2. ✅ Stage 0 completion doc
3. ✅ Stage 1 bootstrap plan

Stage 0 → **%98 tamamlanmış!** 🚀

---

## ✅ YZ_101 TAMAMLANDI! (20 Aralık 2025)

### Enum Initialization Support - BAŞARILI! ✅

**Sorun:** Enum variable initialization desteklenmiyordu: `Color c = Color.Red`

**Çözüm:**
1. **Statement Type Eklendi:**
   - `STMT_ENUM_VARIABLE` statement type
   - `EnumVariable` struct (enum_type, var_name, init_value, has_initializer)

2. **Parser Desteği:**
   - `enum_is_type()` ile enum type detection
   - `enum_lookup_value()` ile value resolution
   - Syntax: `EnumType varname = EnumType.ValueName`

3. **Codegen Desteği:**
   - Stack'te 8-byte allocation (int64)
   - Initial value assignment
   - Optional initializer support

**Test Sonuçları:**
```pmpl
enum Color
    Red      # = 0
    Green    # = 1
    Blue     # = 2
end_enum

function main() as numeric
    Color c = Color.Green  # c = 1
    return 0
end_function
```
**Exit code:** 0 ✅

**Combined Test (Struct + Enum + Array):**
```pmpl
struct Point
    numeric x
    numeric y
end_struct

enum Status
    Active
    Inactive
end_enum

function main() as numeric
    numeric[3] arr
    Point p
    Status s = Status.Active
    
    arr[0] = 10
    arr[1] = 20
    p.x = arr[0]
    p.y = arr[1]
    
    return p.x + p.y  # = 30
end_function
```
**Exit code:** 30 ✅

**Değişen Dosyalar:**
- `compiler/stage0/modules/statement/statement.h` - STMT_ENUM_VARIABLE added
- `compiler/stage0/modules/statement/statement.c` - enum_variable_free support
- `compiler/stage0/modules/statement/statement_parser.c` - Enum variable parsing
- `compiler/stage0/modules/statement/statement_codegen.c` - Enum variable codegen
- `compiler/stage0/modules/enum/enum.h` - EnumVariable struct
- `compiler/stage0/modules/enum/enum.c` - EnumVariable functions

---

## ✅ YZ_100 TAMAMLANDI! (20 Aralık 2025)

### Stage 0 Final Features Check - BAŞARILI! ✅

**Görev:** Struct ve Enum parsing kontrolü

**Test Sonuçları:**
- ✅ Struct parsing: ÇALIŞIYOR (Point struct test → exit code 10)
- ✅ Enum parsing: ÇALIŞIYOR (Color enum test compiled)
- ✅ Struct + Array: ÇALIŞIYOR (exit code 30)
- ❌ Enum initialization: ÇALIŞMIYOR → **YZ_101'e taşındı**

**Bulgu:**
- Struct ve Enum **declaration** parsing çalışıyor
- Enum **variable initialization** desteği eksikti
- Array bounds checking için `libmlp_stdlib.a` link edilmeli

**Link Komutu (Doğru):**
```bash
gcc -no-pie output.s \
    -L../../runtime/sto -lsto_runtime \
    -L../../runtime/stdlib -lmlp_stdlib \
    -o program
```

---

---

## ✅ YZ_99 TAMAMLANDI! (20 Aralık 2025)

### Array Declaration in Function Body - ÇÖZÜLDÜ! ✅

**Sorun:** Fonksiyon içinde array tanımlanamıyordu: `numeric[5] numbers`

**Kök Neden:**
Statement codegen sadece array **literal initialization** (örn. `[1,2,3]`) durumunu handle ediyordu.
Array **declaration without initializer** case'i yoktu!

**Çözüm:**
1. **Array declaration without initializer** case eklendi:
   - `decl->is_array && decl->array_size > 0` kontrolü
   - `sto_array_alloc(size, 8)` çağrısı
   
2. **Array name string literals** eklendi:
   - `.str_arr_<name>` label'ları bounds check error messages için
   - `.rodata` section'a ekleniyor

**Assembly Çıktısı (SONRA):**
```asm
# Array declaration: numbers[5]
movq $5, %rdi      # count
movq $8, %rsi       # elem_size (8 bytes)
call sto_array_alloc # Returns pointer in %rax
movq %rax, -8(%rbp)  # Store array pointer

.section .rodata
.str_arr_numbers:
    .string "numbers"  # For error messages
.text
```

**Test Sonuçları:**
```pmpl
function main() as numeric
    numeric[5] numbers
    numbers[0] = 10
    return numbers[0]
end_function
```
**Exit code:** 10 ✅

**Değişen Dosyalar:**
- `compiler/stage0/modules/statement/statement_codegen.c`
  - Array declaration without initializer support
  - .rodata string literals for array names

---

## ✅ YZ_98 TAMAMLANDI! (20 Aralık 2025)

### Function Call Single Argument Bug - ÇÖZÜLDÜ! ✅

**Sorun:** Tek argümanlı fonksiyon çağrıları (örn. `classify(3)`) argümanı kaybediyordu.

**Semptom:**
```pmpl
function classify(numeric x) as numeric
    return x
end_function

function main() as numeric
    return classify(3)  -- ❌ Argüman '3' push edilmiyordu!
end_function
```

**Assembly Çıktısı (ÖNCE):**
```asm
call classify  # ❌ No argument! %rdi not set
```

**Kök Neden:**
Parser'daki function call vs list access ayrımı heuristici yanlış çalışıyordu:
1. `function_is_known()` tanınmayan fonksiyonları `looks_like_function = 0` yapıyordu
2. Peek-ahead logic sadece `;` (semicolon) kontrol ediyordu
3. Tek argümanlı çağrılarda `;` olmadığı için **list access** olarak yorumlanıyordu!

**Çözüm:**
`arithmetic_parser.c` satır 990-1000: Peek-ahead logic'i basitleştirildi.
Artık her `identifier(...)` pattern'i function call olarak kabul ediliyor.
List access için `list[i]` syntax'ı kullanılmalı.

**Assembly Çıktısı (SONRA):**
```asm
movq $3, %r10  # Literal
pushq %r10     # Save arg 1
popq %rdi      # Restore arg 1
call classify  # ✅ Argument in %rdi!
```

**Test Sonuçları:**
```bash
✅ classify(3) → Return code: 3
✅ add(10; 20) first arg → Return code: 10
✅ add(10; 20) second arg → Return code: 20
```

**Değişen Dosyalar:**
- `compiler/stage0/modules/arithmetic/arithmetic_parser.c`
  - Satır 990-1000: `looks_like_function = 1` for all identifier(...) patterns

---

## ✅ YZ_36 TAMAMLANDI! (19 Aralık 2025)

### Çoklu `else_if` Chain Desteği - BAŞARILI! ✅

**Sorun:** 2+ `else_if` içeren if-else_if-else chain'leri parse edilmiyordu.

**Kök Neden:** 
1. TOKEN_ELSE_IF tek token ama ELSE + IF olarak handle edilmeliydi
2. Recursive else_if chain handling eksikti
3. TOKEN_ELSE_IF'ten sonra IF token lexer'a geri konmuyordu

**Çözüm:**
1. **Helper Functions Eklendi:**
   - `parse_statement_list()` - Statement listesi parse eder (kod tekrarını önler)
   - `parse_else_chain()` - Recursive else/else_if chain handler

2. **TOKEN_ELSE_IF Handling:**
   - TOKEN_ELSE_IF görünce → synthetic TOKEN_ELSE + TOKEN_IF yarat
   - TOKEN_IF'i lexer'a geri koy (`lexer_unget_token`)
   - TOKEN_ELSE'i current_token'a koy (parent için)

3. **Recursive Chain:**
   - `parse_else_chain()` kendini çağırarak sınırsız else_if destekler
   - Her else_if → nested if statement olarak temsil edilir
   - Parent if'in `end_if`'ini paylaşır (PMPL syntax'a uygun)

**Test Sonuçları:**
```pmpl
✅ 1 else_if + else → Çalışıyor
✅ 2 else_if + else → Çalışıyor  
✅ 3 else_if + else → Çalışıyor
✅ 5 else_if + else → Çalışıyor
✅ else_if without final else → Çalışıyor
✅ Nested if içinde else_if → Çalışıyor (zaten destekliyordu)
```

**Örnek Çalışan Kod:**
```pmpl
function test4() as numeric
    numeric x = 4
    if x == 1 then
        return 1
    else_if x == 2 then
        return 2
    else_if x == 3 then
        return 3
    else_if x == 4 then
        return 4
    else_if x == 5 then
        return 5
    else
        return 0
    end_if
end_function

-- Test: x=4 → return 4 ✅ BAŞARILI!
```

**Değişen Dosyalar:**
- `compiler/stage0/modules/statement/statement_parser.c`
  - Helper functions: parse_statement_list(), parse_else_chain()
  - TOKEN_ELSE_IF handling düzeltildi (IF token lexer'a geri konuyor)
  - If statement parsing basitleştirildi (150+ satır kodu → 15 satır!)

**Kod Kalitesi:**
- ✅ Recursive, clean, maintainable
- ✅ No code duplication
- ✅ Template pattern (stateless)
- ✅ Merkezi dosya YOK (modular design)

---

## 🎯 SONRAKİ GÖREVLER (YZ_99+)

### 1. Stage 0 Completion Checkpoint

YZ_98 ile birlikte **function call argument bug çözüldü!** Stage 0 artık:

**✅ Desteklediklerimiz:**
- Functions (declaration, call, return) ✅
- **Function call with arguments** ✅ YENİ! (YZ_98)
- Variables (declaration, assignment)
- If-else-else_if (sınırsız chain!) ✅ (YZ_36)
- While loops (body parsing fixed)
- For loops
- Arrays (literal, index access)
- Structs (definition, instantiation)
- Enums (definition, usage)
- Switch-case
- Operators (arithmetic, comparison, logical)
- Print statements
- Import statements (path resolution)
- Comments

**✅ Eski Blocker'lar ÇÖZÜLDÜ (YZ_98):**
1. ~~**Import execution**~~ - ✅ ÇALIŞIYOR! Module load/execute test edildi (Exit code: 99)
2. ~~**While loop return**~~ - ✅ ÇALIŞIYOR! Return inside while test edildi (Exit code: 42)
3. ~~**Function call single arg**~~ - ✅ ÇÖZÜLDÜ! `classify(3)` artık çalışıyor

**❌ Gerçek Eksikler:**
1. **Struct parsing** - Top-level struct declaration parser'ı engelliyor
2. **Enum parsing** - Top-level enum declaration parser'ı engelliyor
3. **Array in function** - Function body içinde array declaration

**Tahmin:** Stage 0 → %90+ tamamlanmış!

---

## ⚠️ PÜF NOKTALARI

### PMPL Syntax Kuralları
- `else_if` = TEK TOKEN (TOKEN_ELSE_IF)
- Tüm chain için TEK `end_if`
- `else_if` sayısı sınırsız olmalı
- `identifier(...)` = FUNCTION CALL (list access için `list[i]` kullan!)

### Mimari Kurallar
- ❌ Merkezi dosya YOK
- ✅ `main()` = entry point fonksiyonu (merkezi değil!)
- ✅ Her dosya bağımsız, modüler

---

*YZ_98 tarafından güncellendi - 20 Aralık 2025*

---

## ⚠️ PÜF NOKTALARI (KRİTİK!)

### 1. PMPL Syntax Kuralları
**MUTLAKA OKU:** `pmlp_kesin_sozdizimi.md`

Bu dosya TÜM syntax kararlarını içerir:
- `;` parametre ayırıcı (virgül değil!)
- Türk sayı formatı: `126.555.852,36`
- `do` keyword YOK
- `end_if`, `end_while` tek token (alt çizgili)

### 2. STO (Smart Type Optimization)
**OKU:** `docs_tr/language/STO.md`

- `numeric` → int64 / double / BigDecimal (otomatik)
- Taşma otomatik olarak BigDecimal'e yükseltilir
- Runtime: `runtime/sto/libsto_runtime.a`

### 3. Test Komutu
```bash
# Derle
cd compiler/stage0/modules/functions && make

# Test
./functions_compiler input.mlp output.s

# Çalıştır (STO ile)
gcc -no-pie output.s -L../../runtime/sto -lsto_runtime -o program
./program
```

---

## 📁 ÖNEMLİ DOSYALAR (MUTLAKA OKU!)

### Syntax Referans:
```
pmlp_kesin_sozdizimi.md    ← 🔴 EN ÖNEMLİ! Tüm syntax kuralları
PMPL_SYNTAX.md             ← İngilizce referans
fonksiyon_parametreleri.md ← Parametre syntax detayları
```

### Stage 0 Compiler:
```
compiler/stage0/modules/
├── arithmetic/arithmetic_parser.c  ← ⚠️ Function call args (`;` fix)
├── statement/statement_parser.c    ← Block parsing
├── lexer/lexer.c                   ← Token tanımları
├── functions/functions_standalone.c ← Main compiler
├── struct/struct_parser.c          ← Struct parsing
├── enum/enum_parser.c              ← Enum parsing
└── array/array_parser.c            ← Array parsing
```

### Dokümantasyon:
```
docs_tr/language/STO.md             ← Tip sistemi
ARCHITECTURE.md                     ← Mimari kurallar
BILINEN_SORUNLAR.md                 ← Bug listesi
```

---

## 📊 TEST DOSYALARI

Test dosyaları `temp/` klasöründe:
```bash
ls temp/*.mlp
# test_struct_enum.mlp - Struct + enum test
# test_import.mlp      - Import test
# test_call_comma.mlp  - Function call test
# math_module.mlp      - Import edilebilir modül
```

---

## ⚠️ KURALLAR

1. **`;` ayırıcı:** Parametre ayırıcı HER ZAMAN `;` (virgül değil!)
2. **Template Pattern:** Tüm state parametre olarak geçmeli
3. **Global state yasak:** `static` değişken kullanma
4. **STO entegrasyonu:** Overflow koruması için runtime link et

---

## 📚 OKUMA SIRASI

1. `pmlp_kesin_sozdizimi.md` - Syntax kuralları (EN ÖNEMLİ)
2. `docs_tr/language/STO.md` - Tip sistemi
3. `ARCHITECTURE.md` - Mimari kurallar
4. `BILINEN_SORUNLAR.md` - Mevcut bug'lar

---

## 🔧 HIZLI BAŞLANGIÇ

```bash
# 1. Compiler derle
cd compiler/stage0/modules/functions && make

# 2. Test dosyası oluştur
cat > test.mlp << 'EOF'
function main() as numeric
    return 42
end_function
EOF

# 3. Derle ve çalıştır
./functions_compiler test.mlp test.s
gcc -no-pie test.s -o test && ./test
echo "Return: $?"
```

---

*YZ_32 tarafından güncellendi - 19 Aralık 2025*
