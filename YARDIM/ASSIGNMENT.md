# MELP Atama (Assignment) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Stage 0'da tam olarak çalışıyor

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
-- İlk bildiri + atama
TipAdı değişkenAdı = ifade

-- Yeniden atama (tip zaten biliniyor)
değişkenAdı = ifade
```

**Açıklama:** Bir değişkeni ilk kez tanımlarken tip belirtilir. Sonraki atamalarda tip yazılmaz.

---

## Kullanım Örnekleri

### Örnek 1: Temel atama ve yeniden atama
```mlp
numeric x = 10
print(x)
-- Çıktı: 10

x = 20
print(x)
-- Çıktı: 20
```

### Örnek 2: Sayaç ile döngüde atama
```mlp
numeric count = 0
count = 5
count = count + 1
print(count)
-- Çıktı: 6
```

### Örnek 3: Döngü içinde atama
```mlp
numeric last = 0
loop i = 1 to 5
    last = i
end loop
print(last)
-- Çıktı: 5
```

### Örnek 4: String ataması
```mlp
string name = "Alice"
name = "Bob"
print(name)
-- Çıktı: Bob
```

### Örnek 5: Fonksiyon dönüş değerinden atama
```mlp
numeric function square(numeric n)
    return n * n
end function

numeric result = square(7)
print(result)
-- Çıktı: 49
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- Bildirimde tip keyword'ü zorunludur: `numeric x = 10`
- Yeniden atamada sadece değişken adı kullanılır: `x = 20`
- Döngü içinde dışarıda tanımlanan değişken güncellenebilir
- Fonksiyon dönüş değeri doğrudan değişkene atanabilir

### ❌ Yasaklar
- Bildirimsiz atama yapılamaz: `x = 10` (önce `numeric x = ...` olmalı)
- Değişkenin tipi değiştirilemez: `string x = ""; x = 42` — HATA
- `+=`, `-=`, `*=`, `/=` bileşik operatörler desteklenmez; bunun yerine `x = x + n` kullan
- `var`, `let`, `const` anahtar kelimeleri yoktur; her zaman `numeric`, `string` vb. kullan

---

## İlgili Özellikler

- **VARIABLE** — Değişken bildiriminin tam kuralları
- **BASIC_TYPES** — Mevcut tipler
- **FOR_LOOP** — Döngüde atama
- **OPERATORS** — Aritmetik operatörler

**Ayrıca bakınız:**
- [VARIABLE.md](VARIABLE.md)
- [BASIC_TYPES.md](BASIC_TYPES.md)
- [OPERATORS.md](OPERATORS.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| Temel atama | ✅ | ✅ | ✅ |
| Yeniden atama | ✅ | ✅ | ✅ |
| Döngüde atama | ✅ | ✅ | ✅ |
| Bileşik operatörler (`+=`) | ❌ | ⏳ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/ASSIGNMENT.md](../belgeler/language/feature_cards/ASSIGNMENT.md)
- **pmlp Syntax:** [belgeler/pmlp0_sozdizimi.md](../belgeler/pmlp0_sozdizimi.md)
