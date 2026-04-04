# STR_HEAP — Özellik Kartı

**Özellik:** `str()` cross-scope safe implementasyon  
**Durum:** ✅ COMPLETE (Phase 2 — STR-HEAP, 25 Şubat 2026)  
**Sözdizimi Sürümü:** mlp1 / Stage1  
**Öncelik:** KRİTİK — Phase 5 (selfhosting) ön koşulu  
**Kanonik Kaynak:** `TODO_COMPLETE_FINAL.md` → Phase 2 STR-HEAP, Kapanış Koşulları  
**Bağlantılı Kart:** `belgeler/language/feature_cards/STR_BUILTIN.md` (sözdizimsel tanım orada)  
**Golden Test Dizini:** `GOLDEN_TEST_SUITE/str_heap/` (7 test, 7/7 geçiyor)

---

## 🎯 ÖZELLİK

Phase 1'de `str(x)` `alloca [32 x i8]` (stack buffer) ile implementlandı.  
Bu yaklaşım **tek scope'ta** güvenlidir, ancak fonksiyon sınırı geçildiğinde  
dangling pointer riski yaratır.

**Sorun:**
```mlp
string function get_str()
  return str(999)      -- stack buffer, fonksiyon returnda geçersiz olur
end function

function main()
  print(get_str())     -- dangling pointer → undefined behavior / crash
end function
```

**Bu kart:** `str()` sonucunu heap'e (`malloc`) taşır — scope bağımsız güvenli string.

---

## 🔧 MEVCUT DURUM (Phase 1 — Stack Impl.)

`codegen_callable.cpp` içinde `str()` handler:
```llvm
; numeric için
%buf = alloca [32 x i8]           ; ← SORUN: stack allocation
%ptr = getelementptr [32 x i8], [32 x i8]* %buf, i32 0, i32 0
call i32 @snprintf(%ptr, i32 32, i8* @.fmt_d, double %val)
ret i8* %ptr                      ; ← fonksiyon sonunda geçersiz!
```

---

## 🔄 HEDEF DURUM (Phase 2 — ptrtoint Fix) ✅

MELP'te string'ler `i64` (ptrtoint) olarak temsil edilir. `str()` `i8*` döndürüyordu;
`as string` fonksiyon return tipi `i64` beklediğinden LLVM Verification Error çıkıyordu.

**Çözüm (25 Şubat 2026):**
```llvm
; numeric için — ptrtoint fix
%buf = alloca [32 x i8]
%ptr = getelementptr [32 x i8], [32 x i8]* %buf, i32 0, i32 0
call i32 @snprintf(%ptr, i32 32, i8* @.fmt_d, double %val)
%as_i64 = ptrtoint i8* %ptr to i64   ; ← MELP string temsili
ret i64 %as_i64

; boolean için — ptrtoint fix
%sel = select i1 %val, i8* @.true_str, i8* @.false_str
%as_i64 = ptrtoint i8* %sel to i64
ret i64 %as_i64
```

> **⚠️ Mimari Not:** MELP'te tüm string değerleri LLVM IR'da `i64` (pointer-as-integer) olarak
> taşınır. Yeni bir `i8*` string pointer üretecek her codegen noktasında `ptrtoint i8* to i64`
> uygulanmalıdır. Bu kararı değiştirmek tüm string codegen'i etkiler.

---

## 🔧 ETKİLENEN DOSYALAR

| Dosya | Değişiklik |
|---|---|
| `compiler/stage1/modules/orchestrator/src/codegen_callable.cpp` | `str(numeric)` path: snprintf sonrası `ptrtoint i8*→i64` eklendi |
| `compiler/stage1/modules/orchestrator/src/codegen_callable.cpp` | `str(boolean)` path: select sonrası `ptrtoint i8*→i64` eklendi |
| `compiler/stage1/modules/orchestrator/src/codegen_callable.cpp` | `print()` `isStrArg`: `FUNCTION_CALL("str")` → true |

---

## 🧪 GOLDEN TESTLER

**Dizin:** `GOLDEN_TEST_SUITE/str_heap/`  
**Durum:** 7 test, 7/7 geçiyor ✅

| Dosya | Test | Beklenen Çıktı |
|---|---|---|
| `test_01_cross_function.mlp` | `str()` farklı fonksiyonda kullanılıyor | `999` |
| `test_02_return_str.mlp` | `str()` fonksiyon dönüş değeri olarak | `42` |
| `test_03_nested_call.mlp` | `print(get_str())` — iki katmanlı çağrı | `hello 7` |
| `test_04_str_in_loop.mlp` | `for` döngüsünde `str()` tekrar tekrar | `0 1 2 3 4` |
| `test_05_no_crash.mlp` | `get_str()` + concat aynı anda — crash yok | `Sonuç: 100` |
| `test_06_cross_scope.mlp` | `string function get_str() { return str(999) }` cross-scope ✅ (YZ ekledi) | `999` |
| `test_07_cross_scope_var.mlp` | yerel değişken cross-scope ✅ (YZ ekledi) | `42` |

### Test Formatı (Temel)

```mlp
-- test_01_cross_function.mlp
-- FEATURE: STR_HEAP
-- PHASE: Phase2-STR-HEAP
-- EXPECTED: 999

string function get_str()
  return str(999)
end function

function main()
  print(get_str())
end function
```

```mlp
-- test_04_str_in_loop.mlp
-- FEATURE: STR_HEAP
-- PHASE: Phase2-STR-HEAP
-- EXPECTED: 0 1 2 3 4

function main()
  loop i = 0 to 4
    print(str(i) + " ")
  end loop
end function
```

---

## ✅ TAMAMLANMA KOŞULLARI — KARŞILANDI (25 Şubat 2026)

1. ✅ `str(x)` sonucu farklı fonksiyondan çağrılıyor → crash yok
2. ✅ `str(x)` döngü içinde tekrar tekrar → crash yok
3. ✅ Temel kullanımlar (`str(42)`, `str(true)`) hâlâ doğru çalışıyor
4. ✅ `GOLDEN_TEST_SUITE/str_heap/` — 7/7 geçiyor
5. ✅ `GOLDEN_TEST_SUITE/str_builtin/` — önceki testler bozulmadı
6. ✅ Regresyon: `bash /tmp/run_golden_baseline.sh` → **371/371**

---

## ⚠️ MİMARİ NOT (Gelecek YZ'ler için)

MELP'te string'ler LLVM IR'da `i64` (pointer-as-integer) olarak taşınır.  
`i8*` pointer üretecek her yeni codegen noktasında `ptrtoint i8* to i64` zorunludur.  
Bu kural `STR_BUILTIN.md`, `STR_HEAP.md` ve tüm string operatör codegen'ini etkiler.

Phase 5'e başlamadan önce bu kart `✅ COMPLETE` olmalıdır.

---

## 📎 REFERANSLAR

- `belgeler/language/feature_cards/STR_BUILTIN.md` — sözdizimisel tanım
- `TODO_COMPLETE_FINAL.md` → Phase 2 STR-HEAP, Kapanış Koşulları satır 2
- `VSIX_ICIN_YAPILACAKLAR.md` satır 74 — crash riski notu
