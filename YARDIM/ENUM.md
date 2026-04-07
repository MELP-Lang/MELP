# Enum — MELP Başvuru

**Durum:** ✅ TAM — tanımlama, erişim, karşılaştırma, match tümü çalışıyor  
**Kaynak feature kartı:** `belgeler/language/feature_cards/ENUM.md`  
**Test tarihi:** 8 Mart 2026

---

## Temel Sözdizimi

```mlp
enum EnumAdı
    DEĞER_1
    DEĞER_2
    DEĞER_3
end enum
```

- Değerler otomatik olarak **0'dan** numaralandırılır
- Erişim `EnumAdı.DEĞER` notasyonuyla yapılır
- Saklanan tip `numeric` — karşılaştırma sayısal

---

## Örnek: Renk Enum

```mlp
enum Renk
    KIRMIZI    -- 0
    YESIL      -- 1
    MAVI       -- 2
end enum

function main()
    numeric r = Renk.KIRMIZI   -- 0
    numeric g = Renk.YESIL     -- 1
    numeric b = Renk.MAVI      -- 2

    println(str(r))   -- 0
    println(str(g))   -- 1
    println(str(b))   -- 2

    if r == 0 then
        println("KIRMIZI==0: doğru")
    end if
end function
```

---

## Büyük Enum

```mlp
enum Gun
    PAZ    -- 0
    PZT    -- 1
    SAL    -- 2
    CAR    -- 3
    PER    -- 4
    CUM    -- 5
    CMT    -- 6
end enum

numeric bugun = Gun.CUM   -- 5
if bugun == 5 then
    println("Cuma: doğru")
end if
```

---

## Enum ile `match`

```mlp
enum Durum
    BEKLIYOR    -- 0
    AKTIF       -- 1
    TAMAMLANDI  -- 2
end enum

string function acikla(numeric s)
    match s
        case Durum.BEKLIYOR    then return "Bekliyor"
        case Durum.AKTIF       then return "Aktif"
        case Durum.TAMAMLANDI  then return "Tamamlandı"
        else                        return "Bilinmiyor"
    end match
end function

function main()
    println(acikla(Durum.AKTIF))      -- Aktif
    println(acikla(Durum.TAMAMLANDI)) -- Tamamlandı
    println(acikla(99))               -- Bilinmiyor
end function
```

---

## Enum ile `switch`

```mlp
enum Yön
    KUZEY
    GÜNEY
    DOGU
    BATI
end enum

numeric yon = Yön.DOGU

switch yon
    when Yön.KUZEY then println("Kuzey")
    when Yön.GÜNEY then println("Güney")
    when Yön.DOGU  then println("Doğu")
    when Yön.BATI  then println("Batı")
end switch
```

---

## Önemli Notlar

| Konu | Açıklama |
|------|----------|
| Değerler | 0'dan başlar, +1 artış; özel değer atama yok |
| Erişim | `EnumAdı.DEĞER` — noktayla |
| Karşılaştırma | `==`, `!=` ile sayısal karşılaştır |
| Depolama | `numeric` değişkende saklanır |
| `match` ile | `case EnumAdı.DEĞER then` sözdizimi |
| `switch` ile | `when EnumAdı.DEĞER then` sözdizimi |
| Derleme mesajı | `[EnumCodegen] Registered enum: X with N variants` normal — hata değil |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_enum` | `end enum` |
| `end_function` | `end function` |
| `end_if` | `end if` |
| `end_match` | `end match` |
| `function acikla() as string` | `string function acikla()` |
| `return 0` (main içinde) | kaldır |

---

*Bağlantılı dosyalar: [TYPES.md](TYPES.md) · [CONTROL_FLOW.md](CONTROL_FLOW.md)*
