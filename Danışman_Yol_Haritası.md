# 🔒 BU İÇERİK ASLA DEĞİŞTİRİLEMEZ

---

# MLP-GCC Danışman Yol Haritası

**Oluşturma:** 29 Aralık 2025  
**Yetki:** Üst Akıl (ÜA) onayı olmadan değiştirilemez

---

## 🎯 MELP'in 5 Temel Esası

```
┌─────────────────────────────────────────────────────────┐
│                                                         │
│   1. MODÜLER      Hiçbir dosya 500 satırı geçmez       │
│   2. GCC          MLP → C → GCC → Binary               │
│   3. STO          Smart Type Optimization              │
│   4. STATELESS    Global state yasak, pure functions   │
│   5. STRUCT+FUNC  OOP yok, sadece struct + functions   │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

**Formül:**
```
MELP = Modüler + GCC + STO + Stateless + (struct + functions)
```

---

## 🛡️ DEĞİŞTİRİLEMEZ KURALLAR

### Kural 1: MODÜLER MİMARİ
- Hiçbir kaynak dosya **500 satırı geçmeyecek**
- Her modül tek bir sorumluluğa sahip olacak
- "Hızlı çözüm" için monolitik yapı **YASAK**
- Modül birleştirme = **ÜA onayı gerektirir**

### Kural 2: GCC BACKEND
- MLP **asla doğrudan ASM üretmeyecek**
- Derleme zinciri: `MLP → C → GCC → Binary`
- GCC'nin 50 yıllık optimizasyon birikimi kullanılacak
- Platform bağımsızlığı GCC üzerinden sağlanacak

### Kural 3: STO (Smart Type Optimization)
- Kullanıcı sadece `numeric` ve `string` görür
- Compiler arka planda optimal tipi seçer:
  - `numeric` → INT64 / DOUBLE / BIGDECIMAL
  - `string` → SSO / HEAP / RODATA
- Tip kararları derleme zamanında yapılır

### Kural 4: STATELESS TASARIM
- Global mutable state **YASAK**
- Her fonksiyon pure olmalı (mümkün olduğunca)
- `state` keyword yalnızca açıkça işaretlenmiş modül state'i için
- Side effect'ler açıkça belirtilmeli

### Kural 5: STRUCT + FUNCTIONS
- OOP (class, inheritance, polymorphism) **YOK**
- Sadece `struct` ile veri yapıları
- Sadece `function` ile davranış
- Composition over inheritance

---

## 📋 MLP-GCC Proje Yapısı

```
MLP-GCC/
├── Danışman_Yol_Haritası.md    ← BU DOSYA (DEĞİŞTİRİLEMEZ)
├── MELP/
│   ├── C/
│   │   └── stage0/
│   │       └── modules/        ← Bootstrap compiler (C) - 69 modül
│   ├── MLP/
│   │   └── stage1/
│   │       └── modules/        ← Self-host compiler (MLP)
│   ├── runtime/
│   │   ├── runtime.c
│   │   └── runtime.h
│   ├── bootstrap/
│   ├── compiler/
│   └── normalizer/
├── docs/
└── TODO/
```

---

## 🚫 YASAKLAR

| Yasak | Neden |
|-------|-------|
| Monolitik kod | Bakım kabusu, P1'de kanıtlandı |
| Doğrudan ASM | Optimizasyon yok, platform bağımlı |
| Global state | Debugging zorlaşır, race condition |
| Class/OOP | Karmaşıklık, MELP felsefesine aykırı |
| "Sonra düzeltiriz" | Technical debt birikir |

---

## ✅ ZORUNLULUKLAR

| Zorunluluk | Uygulama |
|------------|----------|
| Her commit'te test | Pre-commit hook |
| Bootstrap doğrulama | CI/CD pipeline |
| Modül sınırı kontrolü | Lint kuralı (max 500 satır) |
| Değişiklik onayı | ÜA approval gerekli |
| Dokümantasyon | Her modül için README |

---

## 📊 Başarı Kriterleri

1. **Self-hosting:** Stage1 kendi kendini derleyebilmeli
2. **Performans:** GCC -O3 ile Rust/Go seviyesi
3. **Modülerlik:** Hiçbir dosya 500 satırı geçmemeli
4. **Test coverage:** %80+ coverage
5. **Regresyon:** Sıfır tolerans

---

## 🔄 Değişiklik Prosedürü

```
1. Değişiklik talebi → kullanıcı_talimatı.md
2. Danışman analizi → danışman_talimatı.md
3. MM değerlendirme → mastermind_talimatı.md
4. ÜA onayı → üstakıl_talimatı.md
5. Uygulama → Git commit
```

**5 Temel Esas değişikliği:** ÜA onayı + Kullanıcı onayı gerektirir

---

## 📈 P1'den Alınan Dersler

| Hata | Sonuç | MLP-GCC'de Önlem |
|------|-------|------------------|
| Monolitik geçiş | 10.7K satır bakım kabusu | Max 500 satır kuralı |
| ASM üretimi | Optimizasyon yok | GCC backend zorunlu |
| MM kararları değişti | Proje sapması | Bu dosya değiştirilemez |
| Regresyon | Self-hosting kaybı | Pre-commit hook |

---

*Bu dosya MLP-GCC projesinin anayasasıdır. İhlal eden YZ hesap verir.*
