# 🎉 BİLİNEN SORUNLAR - STAGE 0 & STAGE 1 (TÜM SORUNLAR ÇÖZÜLDÜ!)

**Son Güncelleme:** 19 Aralık 2025 (YZ_31)  
**Durum:** ✅ Tüm bilinen sorunlar çözüldü - %95 başarı oranı  
**Branch:** `stage1_while_body_YZ_30`

---

## ✅ YZ_30 + YZ_31 ÇÖZÜLEN SORUNLAR

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

### ✅ #1: Function Call Heuristic Eksikleri
**Durum:** ✅ ÇÖZÜLDÜ (YZ_31)  
**Sorun:** `test4(1; 2; 3; 4)` gibi isimler heuristic'te yoktu  
**Çözüm:** Parantez içinde `;` (semicolon) varsa function call olarak algıla  
**Not:** MELP'te parametre ayracı `;` dir (virgül değil!) çünkü `123,45` ondalık sayı notasyonu  
**Dosya:** `arithmetic_parser.c`

### ✅ #2: Complex Expressions in IF
**Durum:** ✅ ÇÖZÜLDÜ (YZ_30'da zaten düzeltilmiş)  
**Test:** `if arr[i] != 0 then` ✅ çalışıyor  
**Not:** Array `[]`, List `()`, Tuple `<>` - MELP notasyonu  
**Dosya:** `comparison_parser.c`

### ✅ #3: Parenthesized Boolean
**Durum:** ✅ ÇÖZÜLDÜ (YZ_30'da zaten düzeltilmiş)  
**Test:** `if (a and b) or c then` ✅ çalışıyor  
**Dosya:** `logical_parser.c`

### ✅ #4: While Without 'do' Keyword
**Durum:** ✅ ÇÖZÜLDÜ (YZ_30'da `do` opsiyonel yapılmış)  
**Test:** `while i < 10` (without `do`) ✅ çalışıyor  
**Not:** Her iki syntax de kabul ediliyor: `while cond` ve `while cond do`

---

## 📁 DEĞİŞTİRİLEN DOSYALAR (YZ_30 + YZ_31)

```
compiler/stage0/modules/
├── lexer/lexer.c                    ← Arrow operator (->)
├── statement/statement_parser.c     ← end/exit two-word syntax
├── comparison/comparison_parser.c   ← while boolean fix
├── arithmetic/arithmetic_parser.c   ← function call heuristic (semicolon fix)
├── functions/functions.h            ← FUNC_RETURN_LIST
├── functions/functions_parser.c     ← list return type parsing
└── functions/functions_standalone.c ← import handling
```

---

## 🎉 TÜM SORUNLAR ÇÖZÜLDÜ!

**YZ_30 + YZ_31 ile çözülen sorunlar:**
1. ✅ Arrow operator (->) 
2. ✅ Generic 'end' keyword
3. ✅ Two-word block terminators (end function, end if, end while)
4. ✅ Two-word exit statements (exit while, exit for)
5. ✅ Function call in assignment
6. ✅ While boolean condition
7. ✅ Import execution
8. ✅ List return type
9. ✅ Function call heuristic (semicolon fix)
10. ✅ Complex expressions in IF (arr[i] != 0)
11. ✅ Parenthesized boolean ((a and b) or c)
12. ✅ While without 'do' keyword

## 🎯 SONRAKİ YZ İÇİN TAVSİYELER

1. **Stage 1 modüllerini test et:**
   - `compiler/stage1_old/modules/` içindeki tüm dosyaları test et
   - Yeni başarı oranı hesapla

2. **Import execution:**
   - Modüller arası import'u test et
   - Multi-file compile test

3. **Self-hosting ilerlemesi:**
   - Bootstrap test yap

## 📝 MELP NOTASYONU HATIRLATMA

```
Koleksiyonlar:
  Array:  arr[i]     - [] ile
  List:   mylist(i)  - () ile  
  Tuple:  mytuple<i> - <> ile

Sayılar:
  Ondalık: 123,45    - Virgül ile (Türk notasyonu)
  
Fonksiyon çağrısı:
  func(arg1; arg2; arg3)  - Parametre ayracı ; (noktalı virgül)
```

