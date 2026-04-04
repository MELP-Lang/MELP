# Enum - MeLP Sozdizimi

**Durum:** ✅ COMPLETE (Stage1)
**Sozdizimi Surumu:** mlp1 / Stage1

---

## 📖 TEMEL SOZDIZIMI

Enum, isimlendirilmis sabitler grubu tanimlar. Degerler 0'dan baslar ve otomatik artar. Erisim `EnumAdi.DEGER` formundadir.

```mlp
enum Color
    RED
    GREEN
    BLUE
end enum
```

---

## ✅ TAM ORNEKLER

### Ornek 1 - Temel Enum
```mlp
enum Color
    RED
    GREEN
    BLUE
end enum

numeric c = Color.RED
if c == Color.GREEN then
    print("yesil")
else
    print("yesil degil")
end if
```

### Ornek 2 - Match ile Enum
```mlp
enum Status
    PENDING
    ACTIVE
    COMPLETED
end enum

string function acikla(numeric s)
    match s
        case Status.PENDING then return "Bekliyor"
        case Status.ACTIVE then return "Aktif"
        case Status.COMPLETED then return "Tamamlandi"
        else return "Bilinmiyor"
    end match
end function
```

---

## ⚠️ KRITIK KURALLAR

1. Enum degerleri 0'dan baslar ve +1 artar.
2. Erisim daima `EnumAdi.DEGER` bicimindedir.
3. Blok kapatici: `end enum`.
4. Enum degerleri `numeric` gibi kullanilir.

---

## 🚫 YANLIS KULLANIM

```mlp
-- ❌ YANLIS: enum on eki yok
numeric c = RED
```

```mlp
-- ✅ DOGRU
numeric c = Color.RED
```

---

## 🔗 ILGILI KARTLAR

- [PATTERN_MATCHING.md](PATTERN_MATCHING.md) - match ile enum kullanimi
- [SWITCH_CASE.md](SWITCH_CASE.md) - switch/match kontrol akisi
