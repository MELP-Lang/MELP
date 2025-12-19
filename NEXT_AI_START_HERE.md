# NEXT AI START HERE - YZ Görev Dosyası

**Son Güncelleme:** 19 Aralık 2025  
**Önceki YZ:** YZ_36  
**Mevcut YZ:** YZ_37  
**Dal:** `stage1_while_body_YZ_30`  
**Commit'ler:** Bekliyor

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

## 🎯 SONRAKİ GÖREVLER (YZ_37+)

### 1. Function Call Argument Parsing Bug (YÜKSEK ÖNCELİK!)

**Sorun:** Function call'larda argument parse edilmiyor.
```pmpl
function classify(numeric x) as numeric
    return x
end_function

function main() as numeric
    return classify(3)  -- ❌ Argument '3' push edilmiyor!
end_function
```

**Assembly Çıktısı:**
```asm
main:
    call classify  # ❌ No argument!
    movq %rax, %r8
```

**Beklenen:**
```asm
main:
    movq $3, %rdi  # ✅ Argument 3
    call classify
    movq %rax, %r8
```

**Olası Neden:**
- `arithmetic_parser.c` - function call parsing
- Argument list parse ediliyor mu?
- Codegen'de argument push eksik mi?

**Aksiyonlar:**
- [ ] `arithmetic_parser.c` → function call parsing kontrol et
- [ ] `functions_codegen.c` → call codegen'de argument handling
- [ ] Test: single arg, multiple args, no args

---

### 2. Stage 0 Completion Checkpoint

YZ_36 ile birlikte **else_if blocker kaldırıldı!** Stage 0 artık:

**✅ Desteklediklerimiz:**
- Functions (declaration, call, return)
- Variables (declaration, assignment)
- If-else-else_if (sınırsız chain!) ✅ YENİ!
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

**❌ Kalan Blocker'lar:**
1. **Function call arguments** (yukarıda) - YÜKSEK ÖNCELİK
2. **Import execution** - Module load/execute eksik
3. **While loop return** - Return inside while çalışmıyor

**Tahmin:** 2-3 YZ ile Stage 0 → %60-65 tamamlanmış olur!

---

## ⚠️ PÜF NOKTALARI

### PMPL Syntax Kuralları
- `else_if` = TEK TOKEN (TOKEN_ELSE_IF)
- Tüm chain için TEK `end_if`
- `else_if` sayısı sınırsız olmalı

### Mimari Kurallar
- ❌ Merkezi dosya YOK
- ✅ `main()` = entry point fonksiyonu (merkezi değil!)
- ✅ Her dosya bağımsız, modüler

---

*YZ_36 tarafından güncellendi - 19 Aralık 2025*

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
