# 🐛 BİLİNEN SORUNLAR - STAGE 0 & STAGE 1

**Son Güncelleme:** 18 Aralık 2025  
**Durum:** YZ_21 active investigation  
**Kaynak:** YZ_ÜA_01 Gap Analysis + YZ_21 discoveries

---

## 📊 STAGE 0 EKSİKLER

### ❌ #1: Complex Expressions in IF Conditions (KRİTİK)
**Durum:** 🔴 ACTIVE - YZ_21 investigating  
**Etki:** 16/22 Stage 1 modül (YZ_03-11)  
**Başarı Oranı:** %27 → Hedef: %90+

**Desteklenmiyor:**
```mlp
if pos >= tokens.length then          -- Property access
if arr[i] != 0 then                    -- Array indexing
if func() == value then                -- Function call
if (a and b) or c then                 -- Parenthesized boolean
```

**Mevcut Çalışan:**
```mlp
if pos >= 10 then                      -- Simple comparison
if a > b then                          -- Two variables
```

**Geçici Çözüm (Kullanılıyor ama kötü):**
```mlp
numeric len = tokens.length
if pos >= len then
```

**İlgili Dosyalar:**
- `compiler/stage0/modules/control_flow/control_flow_parser.c` (satır 29: arithmetic_parse_expression_stateless)
- `compiler/stage0/modules/arithmetic/arithmetic_parser.c` (array/property access logic)
- `compiler/stage0/modules/comparison/comparison_parser.c` (YZ_21 yanlış burayı değiştirdi)

**Son Keşif (YZ_21):**
- control_flow ZATEN arithmetic parser kullanıyor ✅
- arithmetic parser ZATEN array access destekliyor ✅
- **Sorun başka yerde!** (Codegen? Nested control flow? Array declaration?)

---

### ⚠️ #2: Parenthesized Boolean Expressions
**Durum:** 🟡 KNOWN  
**Etki:** 1 modül (char_utils.mlp)

**Desteklenmiyor:**
```mlp
if (c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z') then
```

**Workaround:**
```mlp
if c >= 'a' and c <= 'z' or c >= 'A' and c <= 'Z' then
-- ⚠️ Precedence hatalı olabilir!
```

**İlgili Dosyalar:**
- `compiler/stage0/modules/arithmetic/arithmetic_parser.c` (parenthesis handling)

---

### ⚠️ #3: Expression in Function Calls
**Durum:** 🟡 KNOWN  
**Etki:** 1 modül (parser_api.mlp)

**Desteklenmiyor:**
```mlp
append_to_array(arr, create_token(type, value))
```

**Workaround:**
```mlp
Token temp = create_token(type, value)
append_to_array(arr, temp)
```

**İlgili Dosyalar:**
- `compiler/stage0/modules/functions/function_call_parser.c`

---

### ⚠️ #4: Nested Control Flow Bug (YZ_21 Keşfi)
**Durum:** 🔴 NEW - Just discovered  
**Etki:** WHILE + IF kombinasyonları

**Hata:**
```mlp
while condition do
    if inner_condition then
        -- statements
    end
end
-- Error: "Expected 'function' keyword"
```

**İlgili Dosyalar:**
- `compiler/stage0/modules/control_flow/control_flow_parser.c` (statement parser)
- Loop body parsing logic

---

### ⚠️ #5: Array Declaration
**Durum:** 🟡 UNKNOWN - Not tested  
**Etki:** TBD

**Destekleniyor mu?**
```mlp
numeric arr[5]  -- ❌ Desteklenmiyor?
```

**İlgili Dosyalar:**
- `compiler/stage0/modules/variables/variable_declaration_parser.c`
- `compiler/stage0/modules/arrays/array_parser.c`

---

### 🟢 #6: Boolean Literal Assignment (FIXED)
**Durum:** 🟢 FIXED - YZ_21  
**Fix Date:** 18 Aralık 2025  
**Git Commit:** (pending)

**Sorun:**
```mlp
boolean flag = true  -- ❌ "Unexpected token in arithmetic expression"
```

**Sebep:**
- `arithmetic_parser.c` sadece TOKEN_NUMBER parse ediyordu
- TOKEN_TRUE/TOKEN_FALSE desteklenmiyordu

**Fix:**
- `compiler/stage0/modules/arithmetic/arithmetic_parser.c`
- TOKEN_TRUE/TOKEN_FALSE support eklendi (2 yerde)
- STOTypeInfo: INTERNAL_TYPE_BOOLEAN

**Test:**
```mlp
boolean flag
flag = true   -- ✅ ARTIK ÇALIŞIYOR
flag = false  -- ✅ ÇALIŞIYOR
```

**Etki:**
- ✅ Boolean literals artık parse ediliyor
- ⚠️ Stage 1 başarı oranı artmadı (%30.55 → %30.55)
- Not: Başka sorunlar var, systematic analysis gerekli

---

## ✅ YANLIŞ ALARM - SORUN DEĞİL!

### ✅ #F1: String Parameters (FALSE)
**Durum:** ✅ ZATEN DESTEKLENİYOR  
**YZ_14, YZ_16 yanlış tespit:** "string params desteklenmiyor" ❌ YANLIŞ!

**Gerçek:**
```mlp
function greet(string name) return void
    -- ✅ ÇALIŞIYOR!
end
```

---

### ✅ #F2: Boolean Keyword (FALSE)
**Durum:** ✅ ZATEN DESTEKLENİYOR  
**YZ_15 yanlış tespit:** "boolean keyword yok" ❌ YANLIŞ!

**Gerçek:**
```mlp
boolean flag = true
-- ✅ ÇALIŞIYOR!
```

---

## 📈 BAŞARI ORANLARI

### Stage 1 Modül Başarı (YZ_21 Öncesi):
- **Toplam:** 11/36 modül (%30.55)
- **YZ_03-11 Core:** 6/22 (%27) ← Kritik!
- **Parser Modülleri:** 24/27 (%89)
- **Codegen Modülleri:** 16/17 (%94)

### Stage 1 Modül Başarı (YZ_21 Sonrası):
- **Toplam:** 11/36 modül (%30.55) ← DEĞİŞMEDİ
- **YZ_03-11 Core:** 6/22 (%27) ← Hala kritik
- **Parser Modülleri:** 24/27 (%89)
- **Codegen Modülleri:** 16/17 (%94)
- **Boolean fix:** ✅ Çözüldü ama başarı oranına etki yok

### Stage 1 Modül Başarı (Hedef):
- **Hedef:** 32+/36 (%90+)
- **YZ_03-11 Core:** 20+/22 (%90+)

---

## 🔄 DEĞİŞİKLİK GEÇMİŞİ

**18 Ara 2025 20:30 - YZ_ÜA_01:**
- İlk analiz (STAGE_0_GAP_ANALYSIS.md)
- #1-#3 tespit edildi
- #F1-#F2 false alarms düzeltildi

**18 Ara 2025 21:00 - YZ_21 (BAŞLANGIÇ):**
- #1 için comparison_parser.c değiştirdi (YANLIŞ DOSYA!)
- Geri alındı

**18 Ara 2025 22:00 - YZ_21 (ROOT CAUSE):**
- Root cause discovery: Boolean literal assignment
- #6 tespit edildi ve FİX EDİLDİ
- arithmetic_parser.c: TOKEN_TRUE/TOKEN_FALSE support
- Sonuç: %30.55 → %30.55 (başarı artmadı)
- Öğrenim: Tek fix yeterli değil, systematic analysis gerekli
- #4 (nested control flow) keşfedildi
- #5 (array declaration) test edilmedi

**18 Ara 2025 23:00 - YZ_22 (SYSTEMATIC ANALYSIS ✅):**
- ✅ DATA-DRIVEN APPROACH BAŞARILI!
- 42 modül test edildi (17 başarı / 25 başarısız = %40.48)
- 6 kritik pattern tespit edildi:
  1. "Expected 'function'" - 90x (#7 olarak eklenecek)
  2. "Expected '(' after print" - 22x (#8)
  3. "Expected ')' after index" - 20x (#9)
  4. "Expected 'then'" - 17x (#10)
  5. "Module not found" - 15x (#11)
  6. "Unexpected token arithmetic" - 7x (YZ_21 partial fix)
- Tools: test_all_stage1_modules.sh, analyze_error_patterns.sh
- Strategy: 3-phase (Quick wins → Core → Polish)
- See: stage_1_YZ/YZ_22_SYSTEMATIC_ANALYSIS.md

**19 Ara 2025 00:30 - YZ_23 (PRINT SYNTAX ✅ - DEFERRED IMPACT):**
- ✅ Print syntax flexibility implemented
- ✅ Both print(...) and print ... supported
- ✅ Lookahead-based, backward compatible
- ⚠️ Success rate: %40.48 → %40.48 (UNCHANGED - deferred impact)
- ✅ ~75+ print statements ready (will activate after YZ_26)
- 📊 Pattern #2: 22 occurrences → FIXED (blocked by dependencies)
- 🔍 Discovery: Deferred impact pattern (fix ready, dependencies block)
- See: stage_1_YZ/YZ_23_RAPOR.md

**19 Ara 2025 01:00 - YZ_24 (THEN OPTIONAL ✅ - DEFERRED IMPACT #2):**
- ✅ Then keyword optional implemented (30 min - on target!)
- ✅ Both if x > 0 and if x > 0 then valid
- ⚠️ Success rate: %40.48 → %40.48 (UNCHANGED - expected!)
- ✅ ~774 'then' statements ready (will activate after YZ_26)
- 📊 Pattern #4: 17 occurrences → 0 (ELIMINATED)
- 🔍 Validation: Deferred impact pattern CONFIRMED (2nd occurrence)
- 🎯 Phase 1: 2/3 complete (building blocks strategy working)
- See: stage_1_YZ/YZ_24_RAPOR.md

**19 Ara 2025 02:00 - YZ_25 (TYPE CONVERSION ✅ - HYBRID IMPACT!):**
- ✅ Type keywords as function names (1h - on target!)
- ✅ string(), numeric(), boolean() in expressions
- 📈 Success rate: %40.48 → %42.86 (+2.38% - FIRST REAL GAIN!)
- ✅ 1 new module: variables_parser.mlp PASS!
- 📊 Pattern #6: 7 occurrences → 0 (ELIMINATED)
- 🔍 Discovery: HYBRID IMPACT - partial immediate, partial deferred
- ✅ ~100+ type conversions ready (remainder after YZ_26)
- 🎉 **PHASE 1 COMPLETE (3/3)** - Foundation ready!
- See: stage_1_YZ/YZ_25_RAPOR.md

---

## 📊 PHASE 1 SUMMARY (YZ_23-25)

**Errors Eliminated:** 46 total
- Pattern #2 (print): 22 → 0
- Pattern #4 (then): 17 → 0
- Pattern #6 (type): 7 → 0

**Impact:**
- Immediate: +2.38% (hybrid from YZ_25)
- Deferred: ~850+ statements ready for YZ_26 activation

**Time:** 2.5 hours (exactly on target!)

**Strategy Validated:**
- ✅ Building blocks approach working
- ✅ Deferred impact pattern understood
- ✅ Hybrid impact shows unblocking started
- ✅ Foundation complete for CASCADE ACTIVATION

**Next: YZ_26 (Pattern #1 - 90x) → CASCADE TRIGGER!**

---

## 📝 NOTLAR

**Yeni sorun eklerken:**
1. Durum emoji ekle: 🔴 ACTIVE, 🟡 KNOWN, 🟢 FIXED
2. Etki belirt (kaç modül)
3. Code example ver (çalışmayan + workaround)
4. İlgili dosyaları listele
5. Değişiklik geçmişine ekle

**Sorun çözüldüğünde:**
1. Durum → 🟢 FIXED
2. Fix date + YZ numarası ekle
3. Git commit hash ekle
4. Arşiv bölümüne taşı (bottom of file)

---

## 🎯 ÖNCELIK SIRALAMASI

1. 🔴 **SYSTEMATIC ANALYSIS NEEDED** (YZ_22 önerisi)
   - Her modülü tek tek test et
   - Hata pattern'lerini grupla
   - Data-driven approach
   
2. 🔴 #1: Complex expressions in IF (16 modül etki - ama belki başka sebep?)
3. 🔴 #4: Nested control flow bug (kritik pattern)
4. 🟡 #5: Array declaration (validation needed)
5. 🟡 #2: Parenthesized boolean (1 modül)
6. 🟡 #3: Expression in function calls (1 modül)

---

*Bu dosya tüm YZ'ler tarafından güncel tutulmalı!*
