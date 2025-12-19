# NEXT AI START HERE - YZ Görev Dosyası

**Son Güncelleme:** 19 Aralık 2025, 23:30  
**Önceki YZ:** YZ_32  
**Dal:** `stage1_while_body_YZ_30`  
**Commit'ler:** Bekliyor

---

## ✅ YZ_32 TAMAMLANAN İŞLER

### Import Sistemi Düzeltildi:
- **Path resolution** - `.mlp` uzantısı otomatik ekleniyor
- **Import execution** - Modüller yükleniyor ve parse ediliyor
- **Fonksiyon çağrısı** - Import edilen fonksiyonlar çağrılabiliyor

### Struct/Enum Top-Level Parsing:
- **Struct** - ✅ Top-level struct parsing çalışıyor
- **Enum** - ✅ Top-level enum parsing çalışıyor
- **Enum bug fix** - Registry double-linking sorunu çözüldü

### Assembly Syntax Düzeltmesi:
- `.intel_syntax noprefix` → `.att_syntax` (register format uyumu)

### Test Sonuçları:
| Özellik | Durum |
|---------|-------|
| Fonksiyon | ✅ |
| While/For/If | ✅ |
| Struct (top-level) | ✅ |
| Enum (top-level) | ✅ |
| Import (path + exec) | ✅ |
| Import + function call | ✅ |
| STO runtime link | ✅ |

### Parser Tamamlanma: **~90%**

---

## 🎯 SONRAKİ GÖREVLER (YZ_33)

### 1. Fonksiyon Çağrısında `;` Ayırıcı Desteği ⚠️ ÖNCELİKLİ

**Sorun:** Fonksiyon çağrısında `;` ayırıcı parse edilmiyor, sadece `,` çalışıyor.

```mlp
-- ŞU AN ÇALIŞMIYOR:
numeric sum = add(10; 20)

-- ŞU AN ÇALIŞIYOR (ama YANLIŞ syntax):
numeric sum = add(10, 20)
```

**Neden Önemli:** PMPL'de parametre ayırıcı HER ZAMAN `;` çünkü `,` Türk sayı formatında ondalık ayırıcı (345,95).

**Düzeltilecek Dosya:** `compiler/stage0/modules/arithmetic/arithmetic_parser.c`
- `parse_function_call_args()` fonksiyonunda `;` desteği ekle

### 2. Array Desteği (~4%)

**Eksikler:**
- Array declaration: `numeric[] arr`
- Array indexing: `arr[i]`
- Array literal: `[1, 2, 3]` veya `[1; 2; 3]`

**Dosyalar:**
- `compiler/stage0/modules/array/array_parser.c`
- `compiler/stage0/modules/statement/statement_parser.c` (array declaration)

### 3. Struct/Enum Kullanımı (~4%)

**Eksikler:**
- Struct instance: `Person p`
- Field access: `p.name`
- Enum usage: `Status.ACTIVE`

**Dosyalar:**
- `compiler/stage0/modules/struct/struct_parser.c`
- `compiler/stage0/modules/enum/enum_parser.c`

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
