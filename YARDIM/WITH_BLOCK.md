# MELP With Bloğu (With Block) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 TODO — Phase 3 (EKS-004)

---

## Genel Bilgi

`with` bloğu, struct alanlarına kısa yoldan erişim sağlar.  
`ali.isim`, `ali.yas` gibi qualifier tekrarını ortadan kaldırır.

> **⚠️ Henüz Implemente Edilmedi:** EKS-004 kapsamında Phase 3'te eklenecektir.  
> Şu an `with` anahtar kelimesi derleme hatası verir.

---

## Planlanan Sözdizimi

```mlp
-- with bloğu (planlanan)
struct Kisi
    string isim
    numeric yas
    string sehir
end struct

function main()
    Kisi ali
    
    -- Şu anki yol (uzun, tekrarlı)
    ali.isim = "Ali"
    ali.yas = 30
    ali.sehir = "İstanbul"
    
    -- Planlanan with bloğu (kısa yol)
    with ali
        isim = "Ali"
        yas = 30
        sehir = "İstanbul"
    end with
    
    -- Okuma da aynı şekilde
    with ali
        print(isim)
        print(yas)
    end with
end function
```

---

## Şu Anki Çözüm

```mlp
-- ÇALIŞIYOR: Doğrudan alan erişimi
struct Nokta
    numeric x
    numeric y
end struct

function main()
    Nokta p
    p.x = 10
    p.y = 20
    
    print(p.x)
    print(p.y)
end function
-- Çıktı:
-- 10
-- 20
```

---

## İç İçe Struct ile Kullanım (Planlanan)

```mlp
-- Planlanan: iç içe with
struct Adres
    string sokak
    string sehir
end struct

struct Kisi
    string isim
    Adres adres
end struct

function main()
    Kisi ali
    
    with ali
        isim = "Ali"
        with adres
            sokak = "Atatürk Cad."
            sehir = "Ankara"
        end with
    end with
end function
```

---

## EKS-004 Kapsam

- **Bağlam:** Phase 3 syntactic sugar
- **Etkisi:** Struct alan atamalarında kod hakimiyeti
- **Öncelik:** Düşük (çalışma yolu mevcut)

---

## İlgili Özellikler

- **STRUCT** — Struct tanımlama
- **SCOPE** — Kapsam kuralları

**Ayrıca bakınız:**
- [STRUCT.md](STRUCT.md)
- [SCOPE.md](SCOPE.md)
- [belgeler/language/feature_cards/WITH_BLOCK.md](../belgeler/language/feature_cards/WITH_BLOCK.md)

---

## Durum Tablosu

| Özellik | Stage 0 | Stage 1 | Phase 3 |
|---------|---------|---------|---------|
| `with ... end with` | ❌ | ❌ | ✅ Planlı |
| Alan kısa erişimi | ❌ | ❌ | ✅ Planlı |
| İç içe with | ❌ | ❌ | ✅ Planlı |
| `.` ile erişim | ✅ | ✅ | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_with` | `end with` |
| `end_struct` | `end struct` |
| `end_function` | `end function` |
| `with x` (struct olmayan) | sadece struct instance ile kullan |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/WITH_BLOCK.md](../belgeler/language/feature_cards/WITH_BLOCK.md)
- **Hata Kodu:** EKS-004
