# MELP Kullanıcı Tanımlı String Dönüşü (User String Return) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 BUG G-05

---

## Genel Bilgi

`string` tipinde değer döndüren fonksiyonlar tanımlanabilir.  
Ancak **G-05 hatası** nedeniyle, dönen değer doğrudan `print()` içinde kullanılırsa  
adres/pointer bilgisi yazdırılır — string içeriği değil.

---

## Hata Gösterimi

```mlp
-- G-05 HATASI: Pointer adresi yazdırılıyor
string function selamlama(string isim)
    return "Merhaba " + isim
end function

function main()
    print(selamlama("Ahmet"))
    -- BEKLENİYOR: Merhaba Ahmet
    -- GERÇEK: 0x7fff5d3a2b10  (pointer adresi)
end function
```

---

## Geçici Çözüm (Şu An Çalışır)

Fonksiyon dönen değerini önce bir değişkene atayın, sonra yazdırın:

```mlp
string function selamlama(string isim)
    return "Merhaba " + isim
end function

function main()
    -- ✅ ÇALIŞIYOR: Önce değişkene ata
    string sonuc = selamlama("Ahmet")
    print(sonuc)
    -- Çıktı: Merhaba Ahmet
end function
```

---

## Daha Fazla Örnek

```mlp
-- ⚠️ HATALI (G-05)
string function tam_ad(string ad; string soyad)
    return ad + " " + soyad
end function

function main()
    print(tam_ad("Ali"; "Yılmaz"))    -- BUG: adres yazdırır
end function
```

```mlp
-- ✅ DOĞRU (geçici çözüm)
string function tam_ad(string ad; string soyad)
    return ad + " " + soyad
end function

function main()
    string isim = tam_ad("Ali"; "Yılmaz")
    print(isim)    -- Çıktı: Ali Yılmaz
end function
```

---

## Etkilenmeyen Durumlar

```mlp
-- String literal doğrudan print: ✅ ÇALIŞIYOR
function main()
    print("Merhaba Dünya")
end function

-- String değişken print: ✅ ÇALIŞIYOR
function main()
    string mesaj = "Merhaba"
    print(mesaj)
end function

-- Numeric döndüren fonksiyon print: ✅ ÇALIŞIYOR (G-05 sadece string)
numeric function topla(numeric a; numeric b)
    return a + b
end function

function main()
    print(topla(3; 5))    -- 8
end function
```

---

## G-05 Düzeltme Planı

- **Kök Neden:** `codegen_call.cpp` — string döndüren fonksiyon çağrısında  
  değer doğrudan `print()`'e gelindiğinde pointer kopyalanmıyor
- **Etkilenen Modül:** `source/codegen/codegen_call.cpp`
- **Faz:** Bug fix Phase 2

---

## İlgili Özellikler

- **FUNCTION** — Fonksiyon tanımlama
- **STRING** — String tipi ve operasyonları

**Ayrıca bakınız:**
- [FUNCTION.md](FUNCTION.md)
- [STRING.md](STRING.md)
- [belgeler/language/feature_cards/USER_STRING_RETURN.md](../belgeler/language/feature_cards/USER_STRING_RETURN.md)

---

## Durum Tablosu

| Durum | Çalışıyor mu? |
|-------|--------------|
| `print("literal")` | ✅ |
| `string x = ...; print(x)` | ✅ |
| `print(string_fonk())` | 🔴 BUG G-05 |
| `string x = string_fonk(); print(x)` | ✅ (geçici çözüm) |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `print(string_fonk())` (G-05 bug) | `string s = string_fonk(); print(s)` |
| `function f() as string` | `string function f()` |
| `end_function` | `end function` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/USER_STRING_RETURN.md](../belgeler/language/feature_cards/USER_STRING_RETURN.md)
- **Bug ID:** G-05
