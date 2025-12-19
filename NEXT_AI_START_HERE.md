# NEXT AI START HERE - YZ Görev Dosyası

**Son Güncelleme:** 19 Aralık 2025, 15:00  
**Önceki YZ:** YZ_31  
**Dal:** `stage1_while_body_YZ_30`  
**Commit'ler:** `596b768` (PMPL sync), `84d4b37`, `8db2720`

---

## ✅ YZ_31 TAMAMLANAN İŞLER

### PMPL vs MLP Karışıklığı Çözüldü:
- **`do` keyword kaldırıldı** - Parser artık `while cond` bekliyor (do yok!)
- **For loop `=` desteği** - Hem `for i = 1` hem `for i from 1` kabul
- **`as` return type aliası** - `returns` ile aynı işlev

### Test Sonuçları:
| Özellik | Durum |
|---------|-------|
| Fonksiyon | ✅ |
| While loop | ✅ |
| Nested while | ✅ |
| For loop | ✅ |
| If/else_if/else | ✅ |
| Switch/case | ✅ |
| String değişken | ✅ |
| Boolean değişken | ✅ |
| Print/println | ✅ |
| exit_while | ✅ |
| continue_while | ✅ |
| Return | ✅ |

### Parser Tamamlanma: **~85%**

---

## 🎯 SONRAKİ GÖREV: Struct/Enum/Array Desteği

### Eksik Kalanlar:

| Özellik | Durum | Sorun |
|---------|-------|-------|
| **Array** | ❌ | Fonksiyon içinde parse edilmiyor |
| **Struct** | ❌ | Top-level struct fonksiyonları engelliyor |
| **Enum** | ❌ | Top-level enum fonksiyonları engelliyor |
| **Import exec** | ⚠️ | Parse ediyor, execution test edilmeli |

### Öncelik Sırası:
1. **Struct** - Top-level struct parsing
2. **Enum** - Top-level enum parsing  
3. **Array** - Function body içinde array declaration
4. **Import execution** - Modüller arası çağrı

### İlgili Dosyalar:
- `compiler/stage0/modules/struct/struct.c`
- `compiler/stage0/modules/enum/enum.c`
- `compiler/stage0/modules/array/array.c`
- `compiler/stage0/modules/functions/functions_standalone.c`

---

## 📊 TEST KOMUTLARI

```bash
# Derle
cd compiler/stage0/modules/functions && make

# Tek dosya test
./functions_compiler input.mlp output.s

# Batch test
for f in /path/*.mlp; do
  ./functions_compiler "$f" /tmp/out.s 2>&1
done
```

---

## 📁 ÖNEMLİ DOSYALAR

### Stage 0 Compiler (C):
```
compiler/stage0/modules/
├── arithmetic/arithmetic_parser.c  ← Function call heuristic
├── comparison/comparison_parser.c  ← Boolean conditions
├── statement/statement_parser.c    ← Block terminators
├── lexer/lexer.c                   ← Tokenization
└── functions/                      ← Function parsing
```

### Stage 1 Test Dosyaları:
```
archive/old_stage1_monolithic/
├── lexer_mlp/     ← 15/20 başarılı
└── parser_mlp/    ← Test edilmedi
```

---

## ⚠️ KURALLAR

1. **Template Pattern:** Tüm state parametre olarak geçmeli
2. **Global state yasak:** `static` değişken kullanma
3. **Modüler yapı:** Her modül bağımsız çalışmalı
4. **STO entegrasyonu:** Overflow koruması aktif

---

## 📚 REFERANSLAR

- `BILINEN_SORUNLAR.md` - Detaylı bug listesi
- `ARCHITECTURE.md` - Mimari kurallar
- `stage_0_YZ/` - Önceki AI raporları
