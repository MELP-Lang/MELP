# YZ_06: Stage 0 List Syntax Support

**Tarih Başlangıç:** 21 Aralık 2025  
**Öncelik:** 🔴🔴🔴 EN YÜKSEK - BLOKER  
**Süre Tahmini:** 1 hafta (21-28 Aralık 2025)  
**Görevli YZ:** Atandı  
**Branch:** `stage0-list-syntax_YZ_06`

---

## 🚨 KRİTİK DURUM

**Problem:** 66 Stage 1 modül derlenemiyor (0 functions)  
**Neden:** Stage 0 parser `()` list literal syntax desteklemiyor  
**Etki:** 580+ satır, 25 dosya, **TÜM Stage 1 bootstrap engellenmiş!**

```bash
# Mevcut Durum:
./compiler/stage0/modules/functions/functions_compiler \
  modules/lexer_mlp/token.mlp temp/test.ll --backend=llvm
# Çıktı: 0 functions, 0 structs, 0 enums ❌

# Hedef:
# Çıktı: 6 functions, 1 struct, 1 enum ✅
```

---

## 📋 GÖREV DETAYLARI

### Detaylı Brief
**Dosya:** `temp/YZ_06_GOREV_OZET.md` (158 satır, detaylı analiz)

### Syntax Referansları
- `pmlp_kesin_sozdizimi.md` line 170, 640-642 (list syntax)
- `kurallar_kitabı.md` line 857-898 (collection types)

### PMPL List Syntax (Zorunlu)
```pmpl
-- Doğru Syntax:
list tokens = (1; 2; 3;)           -- Trailing ; zorunlu
list empty = ()                     -- Boş list
return (t; v; l; c;)               -- Return list literal
function f(numeric a; string b)    -- Parametre ayırıcı ;

-- Yanlış Syntax (Stage 1'de kullanılmış):
list tokens = [1, 2, 3]            -- ❌ Array syntax
return [t, v, l, c]                -- ❌ Virgül kullanımı
```

---

## 🔧 YAPILACAKLAR

### 1. Stage 0 Lexer (compiler/stage0/modules/lexer/lexer.c)
- [ ] `(` tokenization: context-aware list literal detection
- [ ] `;` tokenization: parametre ayırıcı olarak tanıma
- [ ] List literal başlangıç/bitiş marker

### 2. Stage 0 Parser (compiler/stage0/modules/functions/functions_parser.c)
- [ ] List literal parsing: `(elem1; elem2; elem3;)`
- [ ] Trailing `;` desteği
- [ ] Boş list: `()`
- [ ] Function parameters: `,` yerine `;` desteği
- [ ] Return statement: list literal döndürme

### 3. Stage 0 Codegen
**Dosya:** `compiler/stage0/modules/array/array_codegen.c` VEYA yeni `modules/list/`

- [ ] List allocation (dynamic)
- [ ] List element initialization
- [ ] LLVM IR generation
- [ ] Runtime entegrasyon: `sto_list_alloc()`, `sto_list_set()`

### 4. Test
- [ ] Test file: `tests/llvm/08_list/test_list_syntax.mlp`
- [ ] Test 66 Stage 1 modülün hepsini derle
- [ ] Başarı kriteri: `modules/lexer_mlp/token.mlp` → 6 functions ✅

---

## 📊 ETKİLENEN DOSYALAR

**Modül Analizi:** `temp/MODÜL_ANALIZ_KARŞILAŞTIRMA.md`

- 66 Stage 1 modül (lexer_mlp, parser_mlp, codegen_mlp)
- 541 satır: `list ... = [...]` → `list ... = (...)`
- 40 satır: `return [...]` → `return (...)`
- 25 dosya etkilenmiş

---

## ✅ BAŞARI KRİTERLERİ

1. **Compilation Test:**
   ```bash
   ./functions_compiler modules/lexer_mlp/token.mlp temp/test.ll
   # Hedef: 6 functions ✅ (şu an 0 ❌)
   ```

2. **All Stage 1 Modules:**
   ```bash
   for file in modules/**/*.mlp; do
       ./functions_compiler "$file" temp/test.ll || echo "FAIL: $file"
   done
   # Hedef: 0 başarısız ✅
   ```

3. **Syntax Tests:**
   ```bash
   ./functions_compiler tests/llvm/08_list/test_list_syntax.mlp temp/test.ll
   # Hedef: List literal çalışıyor ✅
   ```

---

## 📚 REFERANSLAR

### Kritik Belgeler
1. `NEXT_AI_START_HERE.md` - Proje genel bakış
2. `temp/YZ_06_GOREV_OZET.md` - Detaylı görev brifingi
3. `TODO_MODERN_LANGUAGE.md` - Faz 0 bölümü
4. `ust_akil_YZ/YZ_ÜA_05.md` - Önceki üst akıl raporu

### Stage 0 Kod
1. `compiler/stage0/modules/lexer/lexer.c` - Tokenization
2. `compiler/stage0/modules/functions/functions_parser.c` - Parsing
3. `compiler/stage0/modules/array/array_codegen.c` - Array codegen (referans)

### Runtime
1. `runtime/sto/sto_list.c` - List runtime functions
2. `runtime/sto/sto_list.h` - List API

---

## 🎯 SONRAKI ADIM

**Görevli YZ:**
1. Bu dosyayı oku ✅
2. `temp/YZ_06_GOREV_OZET.md` oku (detaylı brief)
3. Branch'te çalış: `stage0-list-syntax_YZ_06`
4. Mevcut test yap:
   ```bash
   ./compiler/stage0/modules/functions/functions_compiler \
     modules/lexer_mlp/token.mlp temp/test.ll --backend=llvm
   # Şu an: 0 functions ❌
   ```
5. Lexer → Parser → Codegen sıralamasıyla implement et
6. Her aşamada test et ve commit yap
7. Tamamlandığında: `LLVM_YZ/YZ_06_TAMAMLANDI.md` oluştur

---

## ⚠️ ÖNEMLİ NOTLAR

- **Syntax kesinleştirmesi:** Sadece `()` list literal, `;` parametre ayırıcı
- **Geriye uyumluluk:** Function call'larda `,` desteklenmeye devam edilebilir
- **Runtime:** STO list functions varsa kullan, yoksa minimal impl
- **Test önceliği:** Stage 1 modüller derlenebilir olmalı

---

**Hazırlayan:** YZ_ÜA_06 (Coordinator)  
**Tarih:** 21 Aralık 2025  
**Durum:** ✅ HAZIR - Görevli YZ başlayabilir
