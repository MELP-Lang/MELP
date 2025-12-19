# NEXT AI START HERE - YZ Görev Dosyası

**Son Güncelleme:** 20 Aralık 2025  
**Önceki YZ:** YZ_99  
**Mevcut YZ:** YZ_100  
**Dal:** `stage1_while_body_YZ_30`  
**Commit'ler:** 4ebcd2f (YZ_99 tamamlandı)

---

## 🎯 YZ_100 GÖREVİ: Stage 0 Final Features Check

### Durum
YZ_99 ile array declaration tamamlandı! Stage 0 artık çok olgun durumda. Final eksiklikleri kontrol edelim:

### Yapılacaklar
- [ ] **Struct parsing**: Top-level struct declaration parse edilemiyor
- [ ] **Enum parsing**: Top-level enum declaration parse edilemiyor
- [ ] Stage 0 feature completion assessment
- [ ] Stage 1 bootstrap hazırlığı

### Test Komutu
```bash
cd temp
cat > test_struct.mlp << 'EOF'
struct Point
    numeric x
    numeric y
end_struct

function main() as numeric
    Point p
    p.x = 10
    return p.x
end_function
EOF

../compiler/stage0/modules/functions/functions_compiler test_struct.mlp test_struct.s
# Struct parse ediliyor mu?
```

### Başarı Kriteri
Struct ve enum tanımlamaları parse edilmeli. Eğer parse ediliyorsa, Stage 0 → %95+ tamamlanmış demektir!

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
