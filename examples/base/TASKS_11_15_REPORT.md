# MELP Stage 1 - Seviye 2 Görevleri (11-15) — Rapor

**Görevli:** YZ_02
**Tarih:** 2026-03-03
**Durum:** Tamamlandı

---

## ÖZET

5 görevin tamamı ilgili feature card'larına göre yazılmıştır. Tüm örnekler MELP sözdizimi kurallarına uygundur:
- `;` parametre ayracı kullanılmıştır
- `end_if`, `end_while`, `end_for`, `end_function` blok sonlandırıcıları kullanılmıştır
- `function main()` parametresiz, dönüş tipi yok

---

## GÖREV 11 — String Interpolation ✅

**Dosya:** `/home/pardus/projeler/MLP/LLVM/stage1/examples/base/11_string_interpolation/string_interpolation.mlp`

**Feature Cards:** STRING_INTERPOLATION.md, STR_BUILTIN.md, PRINT.md

**Özellikler:**
- `str()` builtin ile numeric → string dönüşümü
- `str()` builtin ile boolean → string dönüşümü
- String birleştirme `+` operatörü
- `print()` fonksiyonu testi
- Multiple `str()` concatenation

**Test Kodu Özeti:**
```mlp
function main()
    -- str() with numeric literal
    string msg1 = "Değer: " + str(42)
    print(msg1)

    -- str() with variable
    numeric age = 25
    string msg2 = "Yaş: " + str(age)
    print(msg2)

    -- str() with boolean
    boolean durum = true
    string msg3 = "Durum: " + str(durum)
    print(msg3)

    -- Multiple concatenation
    numeric x = 10
    numeric y = 20
    string msg5 = "x=" + str(x) + " y=" + str(y)
    print(msg5)
end_function
```

**Beklenen Çıktı:**
```
Değer: 42
Yaş: 25
Durum: true
Bayrak: false
x=10 y=20
Sonuç: 10
```

**Notlar:**
- STR_BUILTIN feature card'ında ✅ COMPLETE olarak işaretli (Phase 1 — P-02)
- Dil: Turkish variable names kullanılmıştır
- 6 test case ile comprehensive coverage sağlanmıştır

---

## GÖREV 12 — Nested Functions ✅

**Dosya:** `/home/pardus/projeler/MLP/LLVM/stage1/examples/base/12_nested_functions/nested_functions.mlp`

**Feature Card:** FUNCTION_DEF.md

**Özellikler:**
- Fonksiyon tanımı (function definition)
- Fonksiyon içinden başka fonksiyon çağrısı
- Helper function pattern (add, multiply, calculate, print_with_label)
- Nested function calls (calculate uses add ve multiply)

**Fonksiyonlar:**
1. `add(x; y)` — iki sayı topla
2. `multiply(x; y)` — iki sayı çarp
3. `calculate(a; b)` — add ve multiply'ı kullan
4. `print_with_label(label; value)` — yazdır

**Test Kodu Özeti:**
```mlp
function add(numeric x; numeric y) as numeric
    return x + y
end_function

function multiply(numeric x; numeric y) as numeric
    return x * y
end_function

function calculate(numeric a; numeric b) as numeric
    numeric sum = add(a; b)
    numeric product = multiply(a; b)
    return add(sum; product)
end_function

function main()
    numeric r1 = add(10; 5)
    print_with_label("add(10;5)"; r1)

    numeric r3 = calculate(4; 5)
    print_with_label("calculate(4;5)"; r3)
end_function
```

**Beklenen Çıktı:**
```
add(10;5): 15
multiply(3;7): 21
calculate(4;5): 45
chained: 52
```

**Notlar:**
- FUNCTION_DEF.md ✅ COMPLETE (Stage 0)
- Kurallar doğru uygulanmıştır: `function name(type p1; type p2) as return_type`
- Chain of calls: `step1 = add(2;3)` → `step2 = multiply(step1; 4)` → `step3 = add(step2; 10)`

---

## GÖREV 13 — Higher Order Functions ⚠️

**Dosya:** `/home/pardus/projeler/MLP/LLVM/stage1/examples/base/13_higher_order/higher_order.mlp`

**Feature Cards:** HIGHER_ORDER_FUNCTIONS.md, FUNCREF.md

**Durum:** ⚠️ KISMÎ — FUNCREF TODO

**Özellikler:**
- FUNCREF `🔴 TODO (Phase 3 — funcref token lexer'da yok)`
- HIGHER_ORDER_FUNCTIONS ✅ COMPLETE ama map/filter/reduce array methods kullanıyor
- Fallback: basit fonksiyon çağrıları + lambda approximation

**Test Kodu Özeti:**
```mlp
function double(numeric x) as numeric
    return x * 2
end_function

function main()
    numeric n = 5
    print("Original: " + str(n))

    numeric d = double(n)
    print("Doubled: " + str(d))

    numeric t = triple(n)
    print("Tripled: " + str(t))

    -- Lambda approx (if supported)
    numeric multiplier = 2
    numeric lambda_result = n * multiplier
    print("Lambda approx: " + str(lambda_result))
end_function
```

**Notlar:**
- FUNCREF henüz implement edilmemiş — `funcref<(T) -> R>` sözdizimi lexer'da yok
- HIGHER_ORDER_FUNCTIONS (map/filter/reduce) array methods gerekli
- Görev yazılmıştır ama tam çalışma garantisi yok
- Lambda inline expressions (`lambda(x) -> x*2`) kullanılabilir

---

## GÖREV 14 — Lambda ✅

**Dosya:** `/home/pardus/projeler/MLP/LLVM/stage1/examples/base/14_lambda/lambda.mlp`

**Feature Cards:** LAMBDA.md, CLOSURE.md

**Durum:** ✅ Inline lambda çalışıyor

**Özellikler:**
- Lambda inline expressions: `numeric double = lambda(x) -> x * 2`
- Closure: dış scope değişkenlerini capture etme
- Parameter ayracı: `;`
- Multiple lambda tanımları

**Test Kodu Özeti:**
```mlp
function main()
    -- Simple lambda
    numeric double = lambda(x) -> x * 2
    numeric r1 = double(5)
    print("double(5) = " + str(r1))

    -- Lambda with multiple params
    numeric add = lambda(x; y) -> x + y
    numeric r2 = add(10; 20)
    print("add(10;20) = " + str(r2))

    -- Closure: capturing outer variable
    numeric base = 100
    numeric add_base = lambda(x) -> x + base
    numeric r4 = add_base(23)
    print("add_base(23) = " + str(r4))

    -- Closure: multiple captures
    numeric factor = 3
    numeric offset = 5
    numeric complex_fn = lambda(x) -> x * factor + offset
    numeric r5 = complex_fn(4)
    print("complex(4) = " + str(r5))
end_function
```

**Beklenen Çıktı:**
```
double(5) = 10
add(10;20) = 30
multiply(6;7) = 42
add_base(23) = 123
complex(4) = 17
nested(15) = 25
```

**Notlar:**
- LAMBDA.md: ✅ inline lambda çalışıyor
- LAMBDA.md: ❌ `end_lambda` block syntax implement edilmemiş
- CLOSURE.md: 🔴 TODO (Phase 1.2) ama lambda closure'lar supportlanıyor
- Feature card'da kanıt: `test_01_basic_lambda.mlp` ve `test_06_closure.mlp` çalışıyor

---

## GÖREV 15 — Struct ✅

**Dosya:** `/home/pardus/projeler/MLP/LLVM/stage1/examples/base/15_struct/struct.mlp`

**Feature Card:** STRUCT.md

**Durum:** ✅ COMPLETE (Stage 0 — minimal / Stage 1 — struct methods)

**Özellikler:**
- Struct tanımı
- Alan erişimi (field access)
- İç içe struct (nested struct)
- Struct methods (external definition)
- Numeric alanlar kullanıldı (string field issues kaçınıldı)

**Test Kodu Özeti:**
```mlp
struct Point
    numeric x
    numeric y
end_struct

struct Color
    numeric r
    numeric g
    numeric b
end_struct

struct Rectangle
    Point top_left
    numeric width
    numeric height
end_struct

function Rectangle.area() as numeric
    return this.width * this.height
end_function

function main()
    -- Simple struct
    Point p1
    p1.x = 10
    p1.y = 20
    print("Point: x=" + str(p1.x) + " y=" + str(p1.y))

    -- Nested struct
    Rectangle rect
    rect.top_left.x = 0
    rect.top_left.y = 0
    rect.width = 100
    rect.height = 50
    print("Rectangle area: " + str(rect.area()))
end_function
```

**Beklenen Çıktı:**
```
Point: x=10 y=20
Point: x=5 y=15
Color: r=255 g=128 b=64
Rectangle area: 5000
Rectangle perimeter: 300
Pixel pos: x=100 y=200
Pixel color: r=255 g=0 b=0
```

**Notlar:**
- STRUCT.md ✅ COMPLETE (Stage 0)
- Method tanımı dışarıda (STRUCT+FUNC prensibi)
- `struct { string field }` hata verebilir — sadece numeric alanlar kullanıldı
- Test senaryoları: basit struct, nested struct, struct methods

---

## ÖZET TABLOSU

| Görev | Başlık | Dosya | Durum | Feature Status |
|-------|--------|-------|-------|----------------|
| 11 | String Interpolation | `string_interpolation.mlp` | ✅ | ✅ STR_BUILTIN complete |
| 12 | Nested Functions | `nested_functions.mlp` | ✅ | ✅ FUNCTION_DEF complete |
| 13 | Higher Order | `higher_order.mlp` | ⚠️ | 🔴 FUNCREF TODO |
| 14 | Lambda | `lambda.mlp` | ✅ | ✅ LAMBDA inline works |
| 15 | Struct | `struct.mlp` | ✅ | ✅ STRUCT complete |

**Genel Durum:** 4/5 tam ✅, 1/5 kismî ⚠️

---

## SÖZDIZIM KURAL KONTROL

- ✅ Parametre ayracı: `;` kullanılıyor (virgül yok)
- ✅ Blok sonlandırıcılar: `end_if`, `end_while`, `end_for`, `end_function`
- ✅ `function name() as return_type` sözdizimi
- ✅ `main()` parametresiz, dönüş tipi yok

---

## COMPILATION SCRIPT

`RUN_TASKS_11_15.sh` script'i tüm görevleri derleyip çalıştırır:

```bash
cd /home/pardus/projeler/MLP/LLVM/stage1/examples/base
bash RUN_TASKS_11_15.sh
```

---

## BUG RAPORLARI / NOTLAR

### Görev 13 (Higher Order) — FUNCREF Eksik
- FUNCREF feature'ı Phase 3'te planlanmış, henüz implement edilmemiş
- Fallback olarak basit fonksiyon çağrıları ve lambda approximation kullanıldı
- Tam çalışma için: `funcref<(T) -> R>` token'ı lexer'a eklenmeli

### Görev 15 (Struct) — String Alanlar
- Feature card'da: "string field çalışmayabilir (B1 bug)"
- Güvenlik için sadece numeric alanlar kullanıldı
- Alternatif: Color struct string yerine numeric RGB değerleri kullanıyor

---

## İlgili Dosyalar

- Feature cards: `/home/pardus/projeler/MLP/LLVM/stage0/v2/belgeler/language/feature_cards/`
- Task files: `/home/pardus/projeler/MLP/LLVM/stage1/examples/base/1[1-5]_*/`
- Script: `/home/pardus/projeler/MLP/LLVM/stage1/examples/base/RUN_TASKS_11_15.sh`

---

**Rapor hazırlandı:** 2026-03-03 YZ_02

