# Atama (Assignment) — MELP Sozdizimi

**Durum:** ✅ COMPLETE (Stage1)  
**Sozdizimi Surumu:** mlp1 / Stage1

---

## 🎯 OZELLIK

Degisken bildirimi ve yeniden atama kurallari. Bildirimde tip zorunludur; yeniden atamada tip yazilmaz.

---

## 📖 SOZDIZIMI

```mlp
-- Ilk bildirim + atama
TipAdi degiskenAdi = ifade

-- Yeniden atama (tip artik bilinir)
degiskenAdi = ifade
```

---

## ✅ ORNEKLER

### Bildir ve Ata
```mlp
numeric x = 10
print(x)
-- Yazdirir: 10
```

### Yeniden Ata
```mlp
numeric sayac = 0
sayac = 5
sayac = sayac + 1
print(sayac)
-- Yazdirir: 6
```

### Dongu Icinde Atama
```mlp
numeric son = 0
loop i = 1 to 5
    son = i
end loop
print(son)
-- Yazdirir: 5
```

### String Atama
```mlp
string ad = "Ali"
ad = "Veli"
print(ad)
-- Yazdirir: Veli
```

### Fonksiyon Donus Degerinden Atama
```mlp
numeric function kare(numeric n)
    return n * n
end function

numeric sonuc = kare(7)
print(sonuc)
-- Yazdirir: 49
```

---

## ⚠️ KRITIK KURALLAR

- Bildirimde tip anahtar kelimesi zorunludur (`numeric`, `string` vb.).
- Yeniden atamada degisken adi kullanilir — **tip yazilmaz**.
- Bir degiskenin tipi bildirimden sonra degistirilemez.
- `+=`, `-=` gibi bilesik operatorler **henuz desteklenmez**; `x = x + n` kullan.

---

## 🔗 ILGILI KARTLAR

- [VARIABLE.md](VARIABLE.md) — degisken bildirimi
- [BASIC_TYPES.md](BASIC_TYPES.md) — tip sistemi
