# MELP Stage 0 Parser - Görev Listesi

**Son Güncelleme:** 20 Aralık 2025 (YZ_ÜA_02)  
**Branch:** `stage1_list_literal_fix_YZ_106`  
**Parser Durumu:** %99 tamamlandı 🎉🎉🎉  
**Stage 1:** %88 (~14/16 modül) 🚀  
**Import:** Tree Shaking aktif ✅  
**All Bugs:** ÇÖZÜLDÜ! (YZ_108, YZ_109, YZ_110) ✅✅✅

---

## 📊 MEVCUT DURUM

### ✅ Çalışan Özellikler:
| Özellik | Durum | Test |
|---------|-------|------|
| Function declaration | ✅ | `function foo() as numeric` |
| Function call (tek/çok arg) | ✅ | `classify(3)`, `add(10; 20)` |
| While loop + return | ✅ | `while x < 10 ... return 42 ... end_while` |
| For loop | ✅ | `for i = 1 to 10 ... end_for` |
| If/else_if/else (sınırsız) | ✅ | `if ... else_if ... else_if ... else ... end_if` |
| Import execution | ✅ | `import "lib.mlp"` → fonksiyon çağrılıyor |
| **Import Tree Shaking** | ✅ | Parse hatası → Skip + Continue (YZ_108) |
| Struct definition | ✅ | `struct Point ... end_struct` |
| Struct instantiation | ✅ | `Point p`, `p.x = 10` |
| Enum definition | ✅ | `enum Color ... end_enum` |
| **Enum initialization** | ✅ | `Color c = Color.Red` (YZ_101) |
| **Array declaration** | ✅ | `numeric[5] arr` (YZ_99) |
| Array access | ✅ | `arr[0] = 10`, `return arr[0]` |
| String operations | ✅ | `length(name)`, string literal |
| Variable declaration | ✅ | `numeric x = 5`, `string s = "hi"` |
| Print/println | ✅ | `print("hello")` |

### ✅ Bilinen Sorunlar - TÜM BUGLAR ÇÖZÜLDÜ! 🎉
| Bug | Sorun | YZ | Durum |
|-----|-------|-----|-------|
| ~~**#1: List index**~~ | ~~`mylist(0)` fonksiyon çağrısı sanılıyor~~ | ~~YZ_110~~ | ✅ **ÇÖZÜLDÜ** |
| ~~**#2: Struct field**~~ | ~~`return pt.x` codegen eksik~~ | ~~YZ_109~~ | ✅ **ÇÖZÜLDÜ** |
| ~~**#3: Enum variable**~~ | ~~`return c` variable okuyamıyor~~ | ~~YZ_109~~ | ✅ **ÇÖZÜLDÜ** |

---

## 🎯 AKTİF YZ GÖREVLERİ

### ✅ YZ_109: Variable Lookup Fix (Bug #2 + #3) - TAMAMLANDI!
**Tamamlanma:** 20 Aralık 2025  
**Dosya:** `compiler/stage0/modules/comparison/comparison_codegen.c`

**Keşif:**
- Bug #2 (Struct field): `arithmetic_codegen.c` zaten çalışıyordu ✅
- Bug #3 (Enum variable): Zaten çalışıyordu ✅
- **Gerçek Sorun:** `comparison_codegen.c` struct member access desteklemiyordu

**Fix:**
- Header: `#include "../struct/struct.h"` eklendi
- `load_value()` fonksiyonuna struct member access logic eklendi
- `struct_lookup_instance()` ile instance bulma
- Member offset hesaplama + pointer/value handling

**Test Sonuçları:**
- ✅ Struct field return: Exit 10
- ✅ Struct field in if: Exit 42
- ✅ Enum variable: Exit 2
- ✅ Comprehensive: Exit 18

**Sonraki:** YZ_110 (Bug #1: List index access)

---

### ✅ YZ_108: Import Warning → Skip Fix - TAMAMLANDI!
**Tamamlanma:** 20 Aralık 2025  
**Dosya:** `compiler/stage0/modules/import/import.c`

**Değişiklik:**
- Parse hatası → Fatal error → Modül iptal ❌
- Parse hatası → Warning + Skip → Döngü devam ✅

**Sonuç:**
- Stage 1: %75 → %88 (12/16 → 14+/16)
- Import chain çalışıyor
- Rust-Style Tree Shaking aktif

---

## 📋 TAMAMLANAN YZ'LER

### ✅ YZ_99: Array Declaration Fix - TAMAMLANDI!
**Tamamlanma:** 20 Aralık 2025  
**Dosya:** `compiler/stage0/modules/statement/statement_codegen.c`

**Çözüm:**
- Array declaration without initializer case eklendi
- `sto_array_alloc(size, elem_size)` çağrısı
- `.rodata` string literals (bounds check için)

**Test:**
```pmpl
function main() as numeric
    numeric[5] numbers
    numbers[0] = 10
    return numbers[0]
end_function
```
✅ Exit code: 10

---

### ✅ YZ_100: Stage 0 Final Features Check - TAMAMLANDI!
**Tamamlanma:** 20 Aralık 2025

**Test Sonuçları:**
- ✅ Struct parsing: ÇALIŞIYOR
- ✅ Enum parsing: ÇALIŞIYOR
- ✅ Struct + Array: ÇALIŞIYOR (exit code: 30)
- ❌ Enum initialization: YZ_101'e taşındı

---

### ✅ YZ_101: Enum Initialization Support - TAMAMLANDI!
**Tamamlanma:** 20 Aralık 2025  
**Dosyalar:** 
- `compiler/stage0/modules/statement/statement.h`
- `compiler/stage0/modules/statement/statement_parser.c`
- `compiler/stage0/modules/statement/statement_codegen.c`
- `compiler/stage0/modules/enum/enum.h`
- `compiler/stage0/modules/enum/enum.c`

**Çözüm:**
- `STMT_ENUM_VARIABLE` statement type eklendi
- `EnumVariable` struct (enum_type, var_name, init_value)
- Parser: `enum_is_type()` ve `enum_lookup_value()` kullanıldı
- Codegen: Stack'te 8-byte allocation

**Test:**
```pmpl
enum Color
    Red      # = 0
    Green    # = 1
    Blue     # = 2
end_enum

function main() as numeric
    Color c = Color.Green
    return 0
end_function
```
✅ Exit code: 0

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
    
    return p.x + p.y
end_function
```
✅ Exit code: 30

---

### ✅ YZ_102: Critical Bugs + Bootstrap Start - TAMAMLANDI!
**Tamamlanma:** 20 Aralık 2025

**Faz 1: Bug Fixes (2/3 tamamlandı)**
- ✅ **Bug #3:** Enum variable usage fixed!
  - Sorun: `return c` → stack offset 0 (yanlış)
  - Çözüm: Enum variable'ı `LocalVariable` olarak kaydetme
  - Dosya: `statement_codegen.c` (STMT_ENUM_VARIABLE case)
  - Test: Exit code 1 (Color.Green) ✅

- ✅ **Bug #2:** Struct field in expression zaten çalışıyor!
  - Test: `return pt.x` → Exit code 10 ✅

- ⏸️ **Bug #1:** List index access ertelendi
  - Neden: `arithmetic_parser.c`'ye context geçirme gerekiyor
  - Çaba/Fayda: Büyük refactor / küçük bug
  - Karar: Bootstrap'a öncelik

**Faz 2: Stage 1 Bootstrap (BAŞARILI!)**
- ✅ Stage 1 modülleri bulundu: `archive/stage1_api_attempt/modules/`
- ✅ Virgül sorunu tespit edildi: 38 dosya
- ✅ İlk test: `bootstrap_test_fixed.mlp`
  - Virgül → noktalı virgül dönüşümü
  - Stage 0 ile compile: BAŞARILI
  - Çalıştırma: Exit code 30 (add(10; 20)) ✅
- ✅ Conversion script: `scripts/convert_comma_to_semicolon.sh`

**Sonuç:**
- Stage 0 → %97+ complete
- Stage 1 Bootstrap → Proof of concept SUCCESS!
- Self-hosting yolunda ilk adım atıldı 🚀

---

### 📋 YZ_103: Stage 1 Bootstrap Continuation
**Öncelik:** 🔴 Yüksek  
### 📋 YZ_103: Stage 1 Bootstrap Continuation
**Öncelik:** 🔴 Yüksek  
**Tahmini Süre:** 4-6 saat

**Yapılacaklar:**
- [ ] Batch conversion: 38 dosyada virgül → noktalı virgül
- [ ] Test daha karmaşık modül (functions_parser.mlp subset)
- [ ] Import dependency test
- [ ] Bootstrap driver test
- [ ] Self-hosting roadmap

**Başarı Kriteri:**
Stage 1 modüllerinin bir kısmını Stage 0 ile derleyip çalıştırmak.

---

### 📋 YZ_105: Stage 1 Bootstrap Test
**Öncelik:** 🟢 Düşük  
**Tahmini Süre:** 4-6 saat

**Yapılacaklar:**
- [ ] `compiler/stage1_old/modules/` dosyalarını derle
- [ ] Modüller arası import test et
- [ ] Bootstrap başarı raporu
- [ ] Self-hosting roadmap

---

## 📝 YZ KURALLARI

### Commit Format:
```bash
git commit -m "YZ_XX: Kısa açıklama"
```

### Dosya Güncelleme:
1. ✅ Görevi tamamla
2. ✅ Test et
3. ✅ `TODO.md` - görevi [x] işaretle
4. ✅ `NEXT_AI_START_HERE.md` güncelle
5. ✅ Commit ve push

### Test Dosyası Yeri:
```
temp/test_*.mlp     ← Test dosyaları BURAYA
```

❌ Ana dizine test dosyası KOYMA!

---

## 🔗 BAĞIMLILIKLAR

```
YZ_102 (Completion) ──► YZ_105 (Bootstrap Test)
                    │
YZ_103 (List) ──────┤
                    │
YZ_104 (Lookup) ────┘
```

YZ_103 ve YZ_104 paralel yapılabilir.  
YZ_105 tüm görevlerin tamamlanmasını bekler.

---

## 📊 İLERLEME

| YZ | Görev | Durum | Tarih |
|----|-------|-------|-------|
| YZ_98 | Function call arg fix | ✅ Tamamlandı | 20 Aralık 2025 |
| YZ_99 | Array declaration | ✅ Tamamlandı | 20 Aralık 2025 |
| YZ_100 | Stage 0 features check | ✅ Tamamlandı | 20 Aralık 2025 |
| YZ_101 | Enum initialization | ✅ Tamamlandı | 20 Aralık 2025 |
| YZ_102 | Stage 0 completion | ⏳ Bekliyor | - |
| YZ_103 | List index access | ⏳ Bekliyor | - |
| YZ_104 | Variable lookup | ⏳ Bekliyor | - |
| YZ_105 | Stage 1 bootstrap | ⏳ Bekliyor | - |

---

## 🎉 MİHENK TAŞLARI

- **20 Aralık 2025:** Stage 0 %95+ tamamlandı
  - Functions, Arrays, Structs, Enums ✅
  - Enum initialization desteği ✅
  - Combined test (Struct+Enum+Array) ✅

---

*Son düzenleyen: YZ_101 (20 Aralık 2025)*


**Sorun:**
## 🔗 BAĞIMLILIKLAR

```
YZ_102 (3 Bug Fix + Docs) ──► YZ_103 (Bootstrap Test)
```

YZ_102 içinde 3 bug **sırayla** çözülmeli (aynı dosyaları değiştirecekler).  
YZ_103, YZ_102'nin tamamlanmasını bekler.
---

### 📋 YZ_102: Struct Variable Init from Function
**Öncelik:** 🟡 Orta  
**Tahmini Süre:** 2 saat  
**Dosya:** `compiler/stage0/modules/variable/variable_parser.c`

**Sorun:**
```pmpl
Point pt = create_point(10; 20)  -- ❌ Init kısmı eksik!
```
| YZ_98 | Function call arg fix | ✅ Tamamlandı | 20 Aralık 2025 |
| YZ_99 | Array declaration | ✅ Tamamlandı | 20 Aralık 2025 |
| YZ_100 | Stage 0 features check | ✅ Tamamlandı | 20 Aralık 2025 |
| YZ_101 | Enum initialization | ✅ Tamamlandı | 20 Aralık 2025 |
| **YZ_102** | **3 Bugs + Bootstrap Start** | ✅ **Tamamlandı** | 20 Aralık 2025 |
| **YZ_103** | **Stage 1 Bootstrap Continue** | ⏳ **Aktif** | - |
---

### 📋 YZ_103: Stage 1 Bootstrap Test
**Öncelik:** 🟢 Düşük (YZ_99-102 sonrası)  
**Tahmini Süre:** 4 saat

**Yapılacaklar:**
- [ ] `compiler/stage1_old/modules/` dosyalarını derle
- [ ] Modüller arası import test et
- [ ] Self-hosting ilerleme raporu

---

## 📝 YZ KURALLARI

### Commit Format:
```bash
git commit -m "YZ_XX: Kısa açıklama"
```

### Dosya Güncelleme:
1. ✅ Görevi tamamla
2. ✅ Test et
3. ✅ `TODO.md` - görevi [x] işaretle
4. ✅ `NEXT_AI_START_HERE.md` güncelle
5. ✅ `BILINEN_SORUNLAR.md` güncelle (gerekirse)
6. ✅ Commit ve push

### Test Dosyası Yeri:
```
temp/test_*.mlp     ← Test dosyaları BURAYA
```

❌ Ana dizine test dosyası KOYMA!

---

*Son düzenleyen: YZ_102 (20 Aralık 2025)*
