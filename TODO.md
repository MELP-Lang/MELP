# MELP Stage 0 Parser - Görev Listesi

**Son Güncelleme:** 20 Aralık 2025 (YZ_98)  
**Branch:** `stage1_while_body_YZ_30`  
**Parser Durumu:** %90+ tamamlandı

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
| Struct definition | ✅ | `struct Point ... end_struct` |
| Enum definition | ✅ | `enum Color ... end_enum`, `Color.RED` |
| String operations | ✅ | `length(name)`, string literal |
| Variable declaration | ✅ | `numeric x = 5`, `string s = "hi"` |
| Print/println | ✅ | `print("hello")` |

### ❌ Eksik Özellikler (Parser Bug'ları):
| Özellik | Sorun | Öncelik |
|---------|-------|---------|
| **Array declaration** | `array[5] numeric x` parse edilmiyor | 🔴 Yüksek |
| **List index access** | `mylist(0)` fonksiyon çağrısı sanılıyor | 🔴 Yüksek |
| **Struct field in expr** | `return pt.x` çalışmıyor | 🟡 Orta |
| **Struct init from func** | `Point p = create_point(...)` eksik | 🟡 Orta |

---

## 🎯 YZ GÖREVLERİ

### 📋 YZ_99: Array Declaration Fix
**Öncelik:** 🔴 Yüksek  
**Tahmini Süre:** 2-3 saat  
**Dosya:** `compiler/stage0/modules/array/array_parser.c`

**Sorun:**
```pmpl
function main() as numeric
    array[5] numeric numbers    -- ❌ Parse edilmiyor!
    numbers[0] = 10
    return numbers[0]
end_function
```

Assembly çıktısı tamamen boş - fonksiyon body'si yok.

**Yapılacaklar:**
- [ ] `array_parser.c` incele - neden statement olarak algılanmıyor?
- [ ] `statement_parser.c` - array keyword için case ekle
- [ ] Test: `array[5] numeric x`, `x[0] = 10`, `return x[0]`
- [ ] Commit ve rapor

**Başarı Kriteri:**
```bash
./functions_compiler test_array.mlp test_array.s
gcc test_array.s -o test && ./test  # Exit code: 10
```

---

### 📋 YZ_100: List Index Access Fix
**Öncelik:** 🔴 Yüksek  
**Tahmini Süre:** 2-3 saat  
**Dosya:** `compiler/stage0/modules/arithmetic/arithmetic_parser.c`

**Sorun:**
```pmpl
list numbers = (1; 2; 3; 4; 5)
return numbers(0)    -- ❌ Fonksiyon çağrısı olarak yorumlanıyor!
```

MELP'te list index syntax: `mylist(i)` ama parser bunu `call numbers` yapıyor.

**Yapılacaklar:**
- [ ] `arithmetic_parser.c` - identifier'ın variable mı function mı olduğunu kontrol et
- [ ] Symbol table'dan değişken lookup yap
- [ ] Variable ise `(i)` → list index access
- [ ] Function ise `(i)` → function call
- [ ] Test: `list x = (1;2;3)`, `return x(1)` → 2

**Başarı Kriteri:**
```bash
./functions_compiler test_list_index.mlp test_list_index.s
gcc test_list_index.s ... -o test && ./test  # Exit code: 2
```

---

### 📋 YZ_101: Struct Field Access in Return
**Öncelik:** 🟡 Orta  
**Tahmini Süre:** 2 saat  
**Dosya:** `compiler/stage0/modules/arithmetic/arithmetic_parser.c`

**Sorun:**
```pmpl
function main() as numeric
    Point pt = create_point(10; 20)
    return pt.x    -- ❌ Parse edilmiyor!
end_function
```

**Yapılacaklar:**
- [ ] `arithmetic_parser.c` - `identifier.field` pattern'i ekle
- [ ] Struct field offset hesaplama
- [ ] Test: `return pt.x`, `return pt.y`

---

### 📋 YZ_102: Struct Variable Init from Function
**Öncelik:** 🟡 Orta  
**Tahmini Süre:** 2 saat  
**Dosya:** `compiler/stage0/modules/variable/variable_parser.c`

**Sorun:**
```pmpl
Point pt = create_point(10; 20)  -- ❌ Init kısmı eksik!
```

Struct tanımı oluyor ama `= create_point(...)` çalışmıyor.

**Yapılacaklar:**
- [ ] `variable_parser.c` - struct init için function call desteği
- [ ] Return value'yu struct'a kopyala (STO aware)
- [ ] Test: struct init from function

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

## 🔗 BAĞIMLILIKLAR

```
YZ_99 (Array) ─────┐
                   ├─► YZ_103 (Bootstrap Test)
YZ_100 (List) ─────┤
                   │
YZ_101 (Field) ────┤
                   │
YZ_102 (Struct) ───┘
```

YZ_99 ve YZ_100 paralel yapılabilir.  
YZ_101 ve YZ_102 de paralel yapılabilir.  
YZ_103 hepsinin tamamlanmasını bekler.

---

## 📊 İLERLEME

| YZ | Görev | Durum | Tarih |
|----|-------|-------|-------|
| YZ_98 | Function call arg fix | ✅ Tamamlandı | 20 Aralık 2025 |
| YZ_99 | Array declaration | ⏳ Bekliyor | - |
| YZ_100 | List index access | ⏳ Bekliyor | - |
| YZ_101 | Struct field access | ⏳ Bekliyor | - |
| YZ_102 | Struct init from func | ⏳ Bekliyor | - |
| YZ_103 | Stage 1 bootstrap | ⏳ Bekliyor | - |

---

*Son düzenleyen: YZ_98 (20 Aralık 2025)*
