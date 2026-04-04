# Boolean Operatorleri — MELP Sozdizimi

**Durum:** ✅ COMPLETE (Stage1)  
**Sozdizimi Surumu:** mlp1 / Stage1

---

## 🎯 OZELLIK

Mantiksal operatorler: `and`, `or`, `not`. `if`, `loop` ve `else if` kosullarinda kullanilir.

---

## 📖 SOZDIZIMI

```mlp
kosul1 and kosul2   -- mantiksal VE
kosul1 or kosul2    -- mantiksal VEYA
not kosul           -- mantiksal DEĞİL
```

---

## ✅ ORNEKLER

### Mantiksal AND (VE)
```mlp
numeric x = 5
numeric y = 10

if x > 0 and y > 0 then
    print(1)
else
    print(0)
end if
-- Yazdirir: 1
```

### Mantiksal OR (VEYA)
```mlp
numeric a = 0
numeric b = 5

if a > 0 or b > 0 then
    print(1)
else
    print(0)
end if
-- Yazdirir: 1
```

### Mantiksal NOT (DEĞİL)
```mlp
numeric bayrak = 0

if not bayrak == 1 then
    print(2)
else
    print(3)
end if
-- Yazdirir: 2
```

### Birlesirilmis Ifade
```mlp
numeric yas = 25
numeric puan = 80

if yas >= 18 and puan >= 60 then
    print(1)
end if
-- Yazdirir: 1
```

### Dongude Kisa Devre
```mlp
numeric i = 0
numeric sinir = 5

loop i < sinir and i < 3
    i = i + 1
end loop
print(i)
-- Yazdirir: 3
```

---

## ⚠️ KRITIK KURALLAR

- Operatorler kucuk harfle yazilir: `and`, `or`, `not` (buyuk harf YANLIS).
- `&&`, `||`, `!` sembolleri gecersizdir.
- `not` tek operandlidir; `not` ile gruplamada parantez kullanilabilir: `not (a and b)`.

---

## 🔗 ILGILI KARTLAR

- [COMPARISON.md](COMPARISON.md) — karsilastirma operatorleri
- [IF_STATEMENT.md](IF_STATEMENT.md)
