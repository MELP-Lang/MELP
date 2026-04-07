# MELP Birleşik Tip (Union Type) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ⚠️ KISMİ — Stage 1 (parse var, tip kontrolü eksik)

---

## Genel Bilgi

Union type, bir değişkenin birden fazla tipte değer tutabilmesini sağlar.  
`numeric|string` gibi bir birleşik tip tanımlanabilir.

> **⚠️ Kısmi Destek:** Parser `numeric|string` sözdizimini kabul ediyor,  
> ancak tip doğrulama ve tür darlama (type narrowing) henüz implemente edilmedi.

---

## Sözdizimi

```mlp
-- Union tip değişken (parse edilir ama tip kontrolü eksik)
numeric|string deger = 42

-- String değer atama
numeric|string mesaj = "Merhaba"
```

---

## Tip Darlama (Type Narrowing) — Planlanan

Union type ile `match typeof()` kombinasyonu kullanılacak:

```mlp
-- PLANLANAN sözdizimi (henüz calışmıyor)
function isle(numeric|string giris)
    match typeof(giris)
        case numeric then
            print("Sayı: ")
            print(giris)
        case string then
            print("Metin: ")
            print(giris)
    end match
end function

function main()
    isle(42)
    isle("Merhaba")
end function
```

---

## Şu Anki Alternatif

Şu an union type yerine ayrı parametreler veya fonksiyon aşırı yüklemesi kullanın:

```mlp
-- Geçici çözüm: Ayrı fonksiyonlar
function isle_sayi(numeric giris)
    print("Sayı: ")
    print(giris)
end function

function isle_metin(string giris)
    print("Metin: ")
    print(giris)
end function

function main()
    isle_sayi(42)
    isle_metin("Merhaba")
end function
```

---

## Bilinen Sorunlar

| Sorun | Açıklama |
|-------|---------|
| Tip kontrolü yok | `numeric|string x = true` derleniyor (yanış!) |
| typeof() çalışmıyor | `match typeof(x)` for union henüz yok |
| Codegen eksik | Union runtime temsili tanımsız |

---

## Planlanan Sözdizimi Kuralları

| Kural | Örnek |
|-------|-------|
| İki tip birleşimi | `numeric\|string` |
| Üç tip birleşimi | `numeric\|string\|boolean` |
| Nullable | `numeric\|void` (null yoktur, void kullanın) |

---

## İlgili Özellikler

- **TYPES** — Temel tipler
- **TYPE_SAFETY** — Tip güvenliği
- **TYPE_ALIAS** — Tip tanımlama

**Ayrıca bakınız:**
- [TYPES.md](TYPES.md)
- [TYPE_SAFETY.md](TYPE_SAFETY.md)
- [belgeler/language/feature_cards/UNION_TYPE.md](../belgeler/language/feature_cards/UNION_TYPE.md)

---

## Durum Tablosu

| Özellik | Stage 0 | Stage 1 | Stage 2 |
|---------|---------|---------|---------|
| `numeric\|string` parse | ❌ | ⚠️ Kısmi | ✅ Planlı |
| Tip kontrolü | ❌ | ❌ | ✅ Planlı |
| `typeof()` operatörü | ❌ | ❌ | ✅ Planlı |
| Runtime union tag | ❌ | ❌ | ✅ Planlı |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_function` | `end function` |
| `end_match` | `end match` |
| Union tip kullanmak (tip kontrolü eksik) | ayrı fonksiyonlar kullan |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/UNION_TYPE.md](../belgeler/language/feature_cards/UNION_TYPE.md)
