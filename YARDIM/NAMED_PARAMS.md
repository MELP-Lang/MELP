# MELP İsimli Parametreler (Named Params) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Stage 1'de çalışıyor

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
-- Çağrı sırasında isimli argüman:
fonksiyon(parametre1; isim: deger)
```

İsimli parametreler, çağrı sırasında argümanları ismiyle belirtme olanağı verir. Bu, parametre sırası değiştirilebilir kılar.

---

## Kullanım Örnekleri

### Örnek 1: Temel isimli parametre
```mlp
numeric function kutu(numeric en; numeric boy; numeric yukseklik)
    return en * boy * yukseklik
end function

-- Pozisyonel:
print(kutu(2; 3; 4))
-- Çıktı: 24

-- İsimli:
print(kutu(en: 2; boy: 3; yukseklik: 4))
-- Çıktı: 24

-- Sıra değişik (isimli):
print(kutu(yukseklik: 4; en: 2; boy: 3))
-- Çıktı: 24
```

### Örnek 2: Kısmi isimli argüman
```mlp
function log(string mesaj; string seviye)
    print(seviye + ": " + mesaj)
end function

log("Başladı"; seviye: "INFO")
-- Çıktı: INFO: Başladı
```

### Örnek 3: Default params ile birlikte
```mlp
function baglan(string host; numeric port: 8080; boolean ssl: false)
    print(host + ":" + str(port))
end function

baglan("example.com")                       -- port=8080, ssl=false
baglan("example.com"; ssl: true)            -- port=8080, ssl=true
baglan("example.com"; port: 443; ssl: true) -- override
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- İsimli argümanlar `:` ile belirtilir: `isim: değer`
- İsimli argümanlar herhangi bir sırada kullanılabilir
- Pozisyonel ve isimli argümanları karıştırabilirsin
- Default params ile birlikte kullanılabilir

### ❌ Yasaklar
- `=` yerine `:` kullanılır: `port = 443` değil `port: 443`
- Tanımlanan parametre adından farklı isim kullanılamaz
- Aynı parametre hem pozisyonel hem isimli olarak iki kere verilemez

---

## İlgili Özellikler

- **DEFAULT_PARAMS** — Varsayılan parametre değerleri
- **FUNCTION_DEF** — Fonksiyon tanımları
- **VARIADIC** — Değişken sayıda parametre

**Ayrıca bakınız:**
- [DEFAULT_PARAMS.md](DEFAULT_PARAMS.md)
- [FUNCTION_DEF.md](FUNCTION_DEF.md)

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_function` | `end function` |
| `port = 443` (isimli argüman) | `port: 443` |
| `function f() as numeric` | `numeric function f()` |

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| İsimli parametreler | ❌ | ✅ | ✅ |
| Sıra bağımsız çağrı | ❌ | ✅ | ✅ |
| Default ile birlikte | ❌ | ✅ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/NAMED_PARAMS.md](../belgeler/language/feature_cards/NAMED_PARAMS.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/named_params_order/](../GOLDEN_TEST_SUITE/named_params_order/)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)
