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
**Durum:** 🔴 NEW - YZ_21 testing  
**Etki:** TBD

**Destekleniyor mu?**
```mlp
numeric arr[5]  -- ❌ Desteklenmiyor?
```

**İlgili Dosyalar:**
- `compiler/stage0/modules/variables/variable_declaration_parser.c`
- `compiler/stage0/modules/arrays/array_parser.c`

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

### Stage 1 Modül Başarı (YZ_21 Hedefi):
- **Hedef:** 32+/36 (%90+)
- **YZ_03-11 Core:** 20+/22 (%90+)

---

## 🔄 DEĞİŞİKLİK GEÇMİŞİ

**18 Ara 2025 20:30 - YZ_ÜA_01:**
- İlk analiz (STAGE_0_GAP_ANALYSIS.md)
- #1-#3 tespit edildi
- #F1-#F2 false alarms düzeltildi

**18 Ara 2025 21:00 - YZ_21:**
- #1 için comparison_parser.c değiştirdi (YANLIŞ DOSYA!)
- Sonuç: %27 → %30.55 (sadece +%3.55)
- Root cause: control_flow ZATEN doğru kodu kullanıyor
- #4 (nested control flow) keşfedildi
- #5 (array declaration) test ediliyor

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

1. 🔴 #1: Complex expressions in IF (16 modül etki)
2. 🔴 #4: Nested control flow bug (kritik pattern)
3. 🔴 #5: Array declaration (validation needed)
4. 🟡 #2: Parenthesized boolean (1 modül)
5. 🟡 #3: Expression in function calls (1 modül)

---

*Bu dosya tüm YZ'ler tarafından güncel tutulmalı!*
