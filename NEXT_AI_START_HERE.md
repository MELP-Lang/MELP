# NEXT AI START HERE - YZ Görev Dosyası

**Son Güncelleme:** 19 Aralık 2025, 04:30  
**Önceki YZ:** YZ_ÜA_03 (YZ_30)  
**Dal:** `stage1_while_body_YZ_30`  
**Commit'ler:** `ad9b3a7`, `9dc9c9a`, `90cf3fd`

---

## ✅ YZ_30 TAMAMLANAN İŞLER

### 8 Kritik Bug Düzeltildi:
1. Arrow operator `->` (lexer.c)
2. Generic `end` keyword (statement_parser.c)
3. Two-word `end X` terminators (statement_parser.c)
4. Two-word `exit X` statements (statement_parser.c)
5. Function call in assignment (arithmetic_parser.c)
6. While boolean condition (comparison_parser.c)
7. Import execution (functions_standalone.c)
8. List return type (functions.h, functions_parser.c)

### Sonuçlar:
- **15/20 dosya hatasız** (önceki: 3/20)
- **82+ fonksiyon** parse edildi (önceki: 22)
- Başarı oranı: **%45 → %75**

---

## 🎯 SONRAKİ GÖREV: Function Call Heuristic İyileştirme

### Sorun:
`test4(1, 2, 3, 4)` gibi çağrılar hata veriyor:
```
error: Expected ')' after list index
```

### Neden:
`arithmetic_parser.c`'de function/list ayrımı heuristic'e dayalı.
`test4` ismi heuristic'te yok → list access olarak algılanıyor.

### Çözüm Önerileri:
1. **Virgül kontrolü:** Parantez içinde virgül varsa = function call
2. **Default function:** Unknown identifier + `(` = function call varsay
3. **Heuristic genişlet:** Daha fazla prefix/isim ekle

### İlgili Dosya:
`compiler/stage0/modules/arithmetic/arithmetic_parser.c` (satır 800-970)

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
