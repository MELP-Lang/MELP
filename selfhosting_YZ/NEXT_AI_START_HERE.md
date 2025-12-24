# 🎯 GÖREVLİ YZ BAŞLANGIÇ NOKTASI

**Son Güncelleme:** 24 Aralık 2025  
**Durum:** 🟢 YZ_14 Göreve Hazır!  
**Önceki YZ:** YZ_13 (Hata Analizi - 101 dosya test edildi)  
**Sen:** selfhosting_YZ_14

---

## ✅ YZ_13 BAŞARISI!

```
┌─────────────────────────────────────────────────────────────┐
│  SİSTEMATİK HATA ANALİZİ TAMAMLANDI!                      │
│                                                             │
│  Test Edilen: 101 dosya                                   │
│  Başarılı: 63 dosya (%62)                                 │
│                                                             │
│  Hata Kategorileri:                                        │
│  🟡 Println eksikliği: 27 dosya (non-blocking)            │
│  🔴 Syntax hatası: 26 dosya (blocking)                    │
│                                                             │
│  Kritik Dosyalar Tespit Edildi:                           │
│  1. control_flow_codegen.mlp (4 hata)                     │
│  2. enums_codegen.mlp (4 hata)                            │
│  3. structs_codegen.mlp (5 hata)                          │
│  4. operators_codegen.mlp (4 hata)                        │
│                                                             │
│  Rapor: YZ_13_TAMAMLANDI.md                               │
│  Süre: 5 dakika                                            │
│                                                             │
│  🎯 SONUÇ: 4 kritik dosya manuel düzeltme bekliyor!       │
└─────────────────────────────────────────────────────────────┘
```

---

## 📍 ŞİMDİKİ DURUM

**Branch:** `TODO_STAGE1_TO_SELFHOSTING_FINAL`  
**İlerleme:** 9/13 task (%69)

**Aktif Görev:** Phase 1 Task 1.6 - Kritik Dosyaları Manuel Düzelt

---

## 🎯 YZ_14 GÖREVİ

**Phase 1, Task 1.6: Kritik Dosyaları Manuel Düzelt**

### 📊 YZ_13 SONUÇLARI:

**Sistematik Analiz:** ✅ BAŞARILI

**Test Edilen:** 101 dosya  
**Başarılı:** 63 dosya (%62)  
**Kalan Hatalar:** 26 dosya (gerçek syntax) + 27 dosya (println eksikliği)

**Kritik Tespit:** 4 codegen dosyasında `Expected 'function' keyword` hatası

### 🎯 YZ_14 İÇİN GÖREV:

**Hedef:** 4 kritik codegen dosyasını manuel incele ve düzelt

**Neden ÖNEMLİ?**
- Bu dosyalar compiler'ın temel code generation işlevselliği
- YZ_12'nin toplu düzeltmesinden kaçan edge case'ler
- Control flow, enum, struct, operator desteği için kritik
- Düzeltilmeden compiler tamamlanamaz

**Strateji: Manuel İnceleme ve Pattern Tespiti**

### Yapılacaklar:

**Adım 1:** İlk kritik dosyayı incele (control_flow_codegen.mlp)
```bash
cd /home/pardus/projeler/MLP/MLP

# Hatayı bul
grep -n "Expected 'function' keyword" <(compiler/stage0/modules/functions/functions_compiler \
    compiler/stage1/modules/control_flow/control_flow_codegen.mlp /tmp/test.s 2>&1)

# Hata satırlarını incele (274, 279, 284, 390)
sed -n '270,280p' compiler/stage1/modules/control_flow/control_flow_codegen.mlp
sed -n '385,395p' compiler/stage1/modules/control_flow/control_flow_codegen.mlp
```

**Adım 2:** Pattern tespit et
```bash
# Hatalı satırlardaki ortak yapıyı bul
for line in 274 279 284 390; do
    echo "=== Satır $line ==="
    sed -n "${line}p" compiler/stage1/modules/control_flow/control_flow_codegen.mlp
done
```

**Adım 3:** Düzeltme uygula
- Manuel düzeltme (eğer az sayıda)
- VEYA sed düzeltme (eğer pattern bulunursa)

**Adım 4:** Test et
```bash
compiler/stage0/modules/functions/functions_compiler \
    compiler/stage1/modules/control_flow/control_flow_codegen.mlp /tmp/test.s
```

**Adım 5:** Diğer 3 dosyayı aynı şekilde düzelt
- enums_codegen.mlp
- structs_codegen.mlp
- operators_codegen.mlp

### Başarı Kriteri:

- [ ] control_flow_codegen.mlp düzeltildi ve compile edildi
- [ ] enums_codegen.mlp düzeltildi ve compile edildi
- [ ] structs_codegen.mlp düzeltildi ve compile edildi
- [ ] operators_codegen.mlp düzeltildi ve compile edildi
- [ ] Pattern tespit edildi (varsa)
- [ ] YZ_14_TAMAMLANDI.md raporu

---

## 📚 OKUMAN GEREKENLER

1. **selfhosting_YZ/YZ_13_TAMAMLANDI.md** - **ÖNCE BUNU OKU!**
2. Kritik dosyaların listesi ve hata satırları
3. **pmlp_kesin_sozdizimi.md** - Syntax referansı
```bash
echo "=== BAŞARILI ===" 
grep "✅ Compiled" /tmp/yz13_compile_report.txt | wc -l

echo "=== BAŞARISIZ ==="
grep "Error:" /tmp/yz13_compile_report.txt | wc -l
```

**Adım 4:** Manuel inceleme gereken dosyaları listele

### Başarı Kriteri:

- [ ] Tüm .mlp dosyaları test edildi
- [ ] Hata kategorileri belirlendi
- [ ] Başarı/başarısızlık oranı hesaplandı
- [ ] Manuel düzeltme listesi hazırlandı
- [ ] YZ_13_TAMAMLANDI.md raporu

---

## 📚 OKUMAN GEREKENLER

1. **selfhosting_YZ/YZ_12_TAMAMLANDI.md** - **ÖNCE BUNU OKU!**
2. YZ_12'nin düzelttikleri ve kalan sorunlar
3. **pmlp_kesin_sozdizimi.md** - Syntax referansı

---

## ⚠️ KURALLAR

- 4 dosyayı birer birer incele (atlama!)
- Hataların satır numaralarını kontrol et
- Pattern tespit et (varsa toplu düzelt)
- Her düzeltmeden sonra test et
- "Detaylandırmamı ister misin?" YASAK
- Phase/Task icat etme
- Raporu `selfhosting_YZ/YZ_14_TAMAMLANDI.md` olarak yaz

---

## 🚀 HIZLI BAŞLANGIÇ

```bash
# İlk dosyayı incele
cd /home/pardus/projeler/MLP/MLP

echo "=== CONTROL_FLOW_CODEGEN HATALARI ==="
compiler/stage0/modules/functions/functions_compiler \
    compiler/stage1/modules/control_flow/control_flow_codegen.mlp /tmp/test.s 2>&1 | \
    grep "error \[Parser\]" | head -10

echo ""
echo "=== HATA SATIRLARI ==="
for line in 274 279 284 390; do
    echo "--- Satır $line ---"
    sed -n "${line}p" compiler/stage1/modules/control_flow/control_flow_codegen.mlp
done

echo ""
echo "=== CONTEXT (satır 270-280) ==="
sed -n '270,280p' compiler/stage1/modules/control_flow/control_flow_codegen.mlp
```

---

**🎉 YZ_13 BAŞARIYLA TAMAMLANDI!** 🎉  
**📋 Sonraki görev: 4 KRİTİK DOSYAYI DÜZELT!** 📋

**Hazır mısın YZ_14?** 🚀
