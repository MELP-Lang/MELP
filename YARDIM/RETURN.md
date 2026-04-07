# MELP Return (Geri Dönüş) Referansı

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
return ifade    -- değer döndür
return          -- void fonksiyondan çık
```

`return` mevcut fonksiyonu sonlandırır ve isteğe bağlı olarak çağırana bir değer gönderir.

---

## Kullanım Örnekleri

### Örnek 1: Numeric değer döndürme
```mlp
numeric function topla(numeric a; numeric b)
    return a + b
end function

numeric sonuc = topla(3; 4)
print(sonuc)
-- Çıktı: 7
```

### Örnek 2: Koşullu erken çıkış
```mlp
numeric function mutlak(numeric x)
    if x < 0 then
        return 0 - x
    end if
    return x
end function

print(mutlak(-5))
-- Çıktı: 5
```

### Örnek 3: Void fonksiyondan çıkış
```mlp
function kontrol(numeric x)
    if x == 0 then
        return
    end if
    print(x)
end function
```

### Örnek 4: Döngü içinde return
```mlp
numeric function ilk_cift(numeric limit)
    loop numeric i = 2; i <= limit; i = i + 2
        return i
    end loop
    return -1
end function

print(ilk_cift(10))
-- Çıktı: 2
```

### Örnek 5: String döndürme
```mlp
string function selamlama(string isim)
    return "Merhaba " + isim
end function

print(selamlama("Ali"))
-- Çıktı: Merhaba Ali
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- Her non-`void` fonksiyon tüm kod yollarında değer döndürmelidir
- Dönüş tipi fonksiyon imzasının başına konur: `Tip function f()`
- `void` fonksiyonlarda değersiz `return` kullanılabilir
- Birden fazla `return` deyimi (erken çıkış) desteklenir
- `return` yalnızca en yakın fonksiyondan çıkar (döngüden veya bloktan değil)

### ❌ Yasaklar
- Non-void fonksiyonda `return` olmadan çıkmak — derleyici hatası
- `return` ile yanlış tip döndürmek — tip hatası
- Void fonksiyonda değer döndürmek: `return 5` — HATA

---

## İlgili Özellikler

- **FUNCTION_DEF** — Fonksiyon tanımları
- **IF_STATEMENT** — Erken çıkış için koşullar
- **VOID_RETURN** — void dönüş tipi
- **ERROR_HANDLING** — Hata yayılımında return

**Ayrıca bakınız:**
- [FUNCTION_DEF.md](FUNCTION_DEF.md)
- [VOID_RETURN.md](VOID_RETURN.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| Değer döndürme | ✅ | ✅ | ✅ |
| Void return | ✅ | ✅ | ✅ |
| Erken çıkış | ✅ | ✅ | ✅ |
| Çoklu return | ✅ | ✅ | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `function f() as numeric` | `numeric function f()` |
| `return 0` (main içinde) | kaldır — `function main()` void |
| `end_function` | `end function` |
| `for i = 2 to N step 2` (Stage0) | `for numeric i = 2; i <= N; i = i + 2` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/RETURN.md](../belgeler/language/feature_cards/RETURN.md)
- **pmlp Syntax:** [belgeler/pmlp0_sozdizimi.md](../belgeler/pmlp0_sozdizimi.md)
