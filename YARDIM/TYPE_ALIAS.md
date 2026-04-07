# MELP Tip Alias (Type Alias) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 TODO — Phase 3

---

## Genel Bilgi

Tip alias, mevcut bir tipin alternatif bir isimle kullanılmasını sağlar.  
Yeni bir tip **oluşturmaz** — sadece yeni bir isim verir.

> **⚠️ Henüz Implemente Edilmedi:** `type` anahtar kelimesi Phase 3'te eklenecektir.  
> Şu an type alias sözdizimi **derleme hatası** verir.

---

## Planlanan Sözdizimi

```mlp
-- Temel alias (planlanan)
type Puan = numeric
type Ad = string
type AktifMi = boolean
```

```mlp
-- Alias ile değişken tanımlama (planlanan)
type Puan = numeric

Puan function hesapla(Puan taban; Puan bonus)
    return taban + bonus
end function

function main()
    Puan sonuc = hesapla(80; 20)
    print(sonuc)
end function
-- Çıktı: 100
```

---

## Semantik Kurallar

| Kural | Açıklama |
|-------|---------|
| `type A = B` | A, B ile tamamen aynı tiptir |
| Tip uyumu | `Puan` ve `numeric` birbirinin yerine kullanılabilir |
| İç içe alias | `type X = Y` ve `type Z = X` → Z de Y'dir |
| Yeni tip değil | Discriminated union için `UNION_TYPE` kullanın |

---

## Şu Anki Durum

`type` anahtar kelimesi henüz lexer'da tanınmıyor. Şu an için:

```mlp
-- ÇALIŞIYOR (alias yok, doğrudan numeric kullan)
numeric function hesapla(numeric taban; numeric bonus)
    return taban + bonus
end function
```

---

## Planlanan Hata Durumları

```mlp
-- HATA: Henüz desteklenmiyor (Phase 3 sonrası)
type Puan = numeric
type Unvan = numeric

-- Gelecekte bu UYARI verecek:
-- Puan ve Unvan aynı underlying tip, karıştırılabilir
```

---

## İlgili Özellikler

- **UNION_TYPE** — Farklı tipleri birleştirme
- **GENERICS** — Tip parametreleri
- **TYPE_SAFETY** — Tip güvenliği

**Ayrıca bakınız:**
- [UNION_TYPE.md](UNION_TYPE.md)
- [GENERICS.md](GENERICS.md)
- [belgeler/language/feature_cards/TYPE_ALIAS.md](../belgeler/language/feature_cards/TYPE_ALIAS.md)

---

## Durum Tablosu

| Özellik | Stage 0 | Stage 1 | Phase 3 |
|---------|---------|---------|---------|
| `type X = Y` sözdizimi | ❌ | ❌ | ✅ Planlı |
| Lexer tanıma | ❌ | ❌ | ✅ Planlı |
| Codegen | ❌ | ❌ | ✅ Planlı |
| Birleşik tip | ❌ | ❌ | ❌ (UNION_TYPE ayrı) |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `type Puan = numeric` (Stage1'de çalışmaz) | doğrudan `numeric` kullan |
| `end_function` | `end function` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/TYPE_ALIAS.md](../belgeler/language/feature_cards/TYPE_ALIAS.md)
