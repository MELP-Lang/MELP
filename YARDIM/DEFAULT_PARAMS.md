# MELP Varsayılan Parametreler (Default Params) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 TODO — Phase 3 (EKS-009)

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
4. [Yasaklar](#yasaklar)
5. [İlgili Özellikler](#ilgili-özellikler)
6. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
donustipi function fonksiyon(tip param: varsayilan)
    -- gövde
end function
```

**Dikkat:** MELP'te default parametre için `=` değil `:` kullanılır!

---

## Kullanım Örnekleri

### Örnek 1: String varsayılan değer
```mlp
function log(string mesaj; string seviye: "INFO")
    print(seviye + ": " + mesaj)
end function

-- Default kullanım:
log("Başladı")                    -- → INFO: Başladı
-- Override:
log("Hata!"; seviye: "ERR")       -- → ERR: Hata!
```

### Örnek 2: Numeric varsayılan
```mlp
numeric function kuvvet(numeric taban; numeric us: 2)
    numeric sonuc = 1
    numeric i = 0
    loop i < us
        sonuc = sonuc * taban
        i = i + 1
    end loop
    return sonuc
end function

kuvvet(3)           -- us=2 → 9
kuvvet(3; us: 3)    -- us=3 → 27
```

### Örnek 3: Boolean varsayılan
```mlp
function kaydet(string dosya; boolean zorla: false)
    print(dosya)
end function

kaydet("test.txt")               -- zorla=false
kaydet("test.txt"; zorla: true)  -- zorla=true
```

---

## Kurallar ve Kısıtlamalar

### ✅ Planlanan (Phase 3)
- Default parametre sentaksı: `tip param: varsayılan`
- Default'lu parametreler sona gelir (zorunlulardan sonra)
- Named params ile birlikte çalışır
- `=` değil `:` kullanılır

### ❌ Yasaklar
- `function f(string s = "x")` — `=` yerine `:` kullan
- Default parametreden sonra zorunlu parametre — HATA
- `f(; parametre: değer)` — boş pozisyonel argüman geçersiz

---

## İlgili Özellikler

- **NAMED_PARAMS** — İsimli parametreler
- **FUNCTION_DEF** — Fonksiyon tanımları
- **VARIADIC** — Değişken sayıda parametre

**Ayrıca bakınız:**
- [NAMED_PARAMS.md](NAMED_PARAMS.md)
- [FUNCTION_DEF.md](FUNCTION_DEF.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| Default parametreler | ❌ | ❌ | ✅ |
| Named override | ❌ | ❌ | ✅ |
| Zorunlu + default karışık | ❌ | ❌ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/DEFAULT_PARAMS.md](../belgeler/language/feature_cards/DEFAULT_PARAMS.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/default_params/](../GOLDEN_TEST_SUITE/default_params/)
- **pmlp Syntax:** [belgeler/pmlp_sozdizimi.md](../belgeler/pmlp_sozdizimi.md)
