# Karsilastirma Operatorleri — MELP Sozdizimi

**Durum:** ✅ COMPLETE (Stage1)  
**Sozdizimi Surumu:** mlp1 / Stage1

---

## 🎯 OZELLIK

Iliskisel karsilastirma operatorleri. Sonuc `if`, `loop` ve `else if` kosullarinda kullanilabilir.

---

## 📖 SOZDIZIMI

| Operator | Anlam |
|----------|-------|
| `==` | Esit |
| `!=` | Esit degil |
| `<` | Kucuk |
| `>` | Buyuk |
| `<=` | Kucuk veya esit |
| `>=` | Buyuk veya esit |

---

## ✅ ORNEKLER

### Esit
```mlp
numeric x = 5
if x == 5 then
    print(1)
end if
-- Yazdirir: 1
```

### Esit Degil
```mlp
numeric y = 3
if y != 5 then
    print(1)
end if
-- Yazdirir: 1
```

### Kucuk / Buyuk
```mlp
numeric a = 2
numeric b = 7
if a < b then
    print(1)
end if
-- Yazdirir: 1
```

### Kucuk veya Esit
```mlp
numeric sayac = 5
if sayac <= 5 then
    print(1)
end if
-- Yazdirir: 1
```

### Buyuk veya Esit
```mlp
numeric puan = 90
if puan >= 90 then
    print(4)
else
    if puan >= 80 then
        print(3)
    else
        print(2)
    end if
end if
-- Yazdirir: 4
```

### Dongu Kosulunda
```mlp
numeric i = 0
loop i != 3
    i = i + 1
end loop
print(i)
-- Yazdirir: 3
```

---

## ⚠️ KRITIK KURALLAR

- Karsilastirmalar simdilik yalnizca `numeric` tipinde calisir.
- String esitligi `==` ile **desteklenmez** (adres karsilastirir, icerik degil).
- `0 < x < 10` gibi zincir karsilastirma **gecersizdir**; `x > 0 and x < 10` kullan.
- `=` **atama**, `==` **esitlik testidir** — karistirma.

---

## 🔗 ILGILI KARTLAR

- [BOOLEAN_OPS.md](BOOLEAN_OPS.md) — and / or / not
- [IF_STATEMENT.md](IF_STATEMENT.md)
