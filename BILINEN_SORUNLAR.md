# 🐛 BİLİNEN SORUNLAR - STAGE 0 & STAGE 1

**Son Güncelleme:** 19 Aralık 2025 (YZ_30)  
**Durum:** Major fixes applied - %75 başarı oranı  
**Branch:** `stage1_while_body_YZ_30`

---

## ✅ YZ_30 ÇÖZÜLEN SORUNLAR

### ✅ #1: Arrow Operator (->) Parse Edilmiyordu
**Çözüm:** `lexer.c` - `->` artık `TOKEN_RETURNS` olarak tokenize ediliyor  
**Dosya:** `compiler/stage0/modules/lexer/lexer.c`  
**Etki:** Multi-function dosyalar artık parse ediliyor

### ✅ #2: Generic 'end' Keyword
**Çözüm:** `statement_parser.c` - `TOKEN_END` case eklendi  
**Dosya:** `compiler/stage0/modules/statement/statement_parser.c`  
**Etki:** Python-style `end` block terminator çalışıyor

### ✅ #3: Two-Word Block Terminators (end function, end if, end while)
**Çözüm:** `statement_parser.c` - `end` sonrası keyword kontrolü eklendi  
**Dosya:** `compiler/stage0/modules/statement/statement_parser.c`  
**Etki:** `end function`, `end if`, `end while`, `end for` çalışıyor

### ✅ #4: Two-Word Exit Statements (exit while, exit for)
**Çözüm:** `statement_parser.c` - `exit` sonrası keyword kontrolü eklendi  
**Dosya:** `compiler/stage0/modules/statement/statement_parser.c`  
**Etki:** `exit while`, `exit for`, `exit if` çalışıyor

### ✅ #5: Function Call in Assignment
**Çözüm:** `arithmetic_parser.c` - Heuristic genişletildi  
**Dosya:** `compiler/stage0/modules/arithmetic/arithmetic_parser.c`  
**Etki:** `numeric x = add(3, 4)` artık çalışıyor

### ✅ #6: While Boolean Condition
**Çözüm:** `comparison_parser.c` - Lookahead token'lar eklendi  
**Dosya:** `compiler/stage0/modules/comparison/comparison_parser.c`  
**Etki:** `while flag do` artık parse ediliyor

### ✅ #7: Import Execution
**Çözüm:** `functions_standalone.c` - Import handling eklendi  
**Dosya:** `compiler/stage0/modules/functions/functions_standalone.c`  
**Etki:** Modüller yüklenip parse ediliyor

### ✅ #8: List Return Type
**Çözüm:** `functions.h` + `functions_parser.c` - `FUNC_RETURN_LIST` eklendi  
**Dosyalar:** `functions.h`, `functions_parser.c`  
**Etki:** `function foo() -> list` çalışıyor

---

## 📊 TEST SONUÇLARI (YZ_30 Sonrası)

| Dosya | Fonksiyon | Hata | Durum |
|-------|-----------|------|-------|
| char_utils.mlp | 11 | 0 | ✅ |
| lexer_api.mlp | 3 | 0 | ✅ |
| lexer.mlp | 12 | 0 | ✅ |
| token.mlp | 8 | 0 | ✅ |
| tokenize_identifiers.mlp | 9 | 0 | ✅ |
| tokenize_literals.mlp | 6 | 0 | ✅ |
| tokenize_operators.mlp | 11 | 0 | ✅ |
| test_minimal_token.mlp | 2 | 0 | ✅ |
| test_nested_4.mlp | 2 | 0 | ✅ |
| test_scan_*.mlp | 9 | 0 | ✅ |
| test_token_simple.mlp | 4 | 0 | ✅ |
| test_while_no_do.mlp | 2 | 0 | ✅ |

**Toplam: 15/20 dosya hatasız, 82+ fonksiyon parse edildi**

---

## ⚠️ KALAN SORUNLAR

### #1: Function Call Heuristic Eksikleri
**Durum:** �� KISMEN ÇÖZÜLDÜ  
**Sorun:** `test4(1,2,3,4)` gibi isimler heuristic'te yok  
**Çözüm önerisi:** Parantez içinde virgül varsa function call olarak algıla  
**Dosya:** `arithmetic_parser.c`

### #2: Complex Expressions in IF
**Durum:** 🔴 AÇIK  
**Sorun:** `if arr[i] != 0 then` çalışmıyor  
**Dosya:** `control_flow_parser.c` veya `arithmetic_parser.c`

### #3: Parenthesized Boolean
**Durum:** 🔴 AÇIK  
**Sorun:** `if (a and b) or c then` çalışmıyor  
**Dosya:** `logical_parser.c` veya `comparison_parser.c`

### #4: While Without 'do' Keyword
**Durum:** 🟡 DOSYALARA BAĞLI  
**Sorun:** Bazı eski dosyalar `while condition` kullanıyor (`do` olmadan)  
**Not:** Yeni syntax `while condition do` gerektiriyor

---

## 📁 DEĞİŞTİRİLEN DOSYALAR (YZ_30)

```
compiler/stage0/modules/
├── lexer/lexer.c                    ← Arrow operator (->)
├── statement/statement_parser.c     ← end/exit two-word syntax
├── comparison/comparison_parser.c   ← while boolean fix
├── arithmetic/arithmetic_parser.c   ← function call heuristic
├── functions/functions.h            ← FUNC_RETURN_LIST
├── functions/functions_parser.c     ← list return type parsing
└── functions/functions_standalone.c ← import handling
```

---

## 🎯 SONRAKİ YZ İÇİN TAVSİYELER

1. **Function call heuristic'i iyileştir:**
   - Virgül kontrolü ekle (parantez içinde virgül = function call)
   - Veya: Tüm unknown identifier(args) = function call varsay

2. **While 'do' keyword kontrolü:**
   - Eski syntax desteği ekle veya dosyaları güncelle

3. **Parser modüllerini test et:**
   - `archive/old_stage1_monolithic/parser_mlp/` dosyalarını test et

4. **Başarı oranını ölç:**
   - Tüm Stage 1 dosyalarını test et
   - Yeni başarı oranı hesapla
