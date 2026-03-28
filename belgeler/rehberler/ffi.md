# FFI — Kullanım Rehberi

Bu kısa rehber Stage1 için güncel kullanıcı akışını özetler.

---

## Ne Zaman Kullanılır

Bir C kütüphanesini (örneğin `libc`, `libm`, `libpng`) doğrudan MELP kodundan çağırmak
istediğinizde FFI kullanılır. Özellikle:

- Mevcut bir C kütüphanesinin fonksiyonlarına erişmek
- Sistem çağrılarını (`getpid`, `malloc`) doğrudan sarmak
- MELP'in henüz desteklemediği platforma özgü bir özellik kullanmak

---

## Temel Sözdizimi

```
-- Kütüphaneyi aç
metin lib = DynamicLibrary.aç("libc.so.6")

-- Fonksiyon sembolünü çöz
fonksiyon strlen = lib.sembol<fonksiyon(işaretçi) -> sayısal>("strlen")

-- Fonksiyonu çağır
metin benim_metin = "Merhaba MELP"
sayısal uzunluk = strlen(benim_metin)
yaz(uzunluk)

-- Kütüphaneyi kapat
lib.kapat()
```

İngilizce eşdeğeri:

```
function main()
  string lib = DynamicLibrary.open("libc.so.6")
  function strlen = lib.symbol<function(pointer) -> numeric>("strlen")

  string my_text = "Hello MELP"
  numeric length = strlen(my_text)
  print(length)

  lib.close()
end function
```

---

## Tip Eşlemeleri

| MELP Tipi | C Tipi | Not |
|---|---|---|
| `sayısal` / `numeric` | `double` | 64-bit kayan nokta |
| `metin` / `string` | `char*` | Null-sonlu |
| `işaretçi` / `pointer` | `void*` | Genel işaretçi |
| `mantıksal` / `boolean` | `int` | 0 = yanlış, 1 = doğru |

---

## Sık Yapılan Hatalar

| Hata | Nedeni | Çözüm |
|---|---|---|
| `Library not found` | `.so` yolu yanlış veya yok | `ldconfig -p \| grep libname` ile kontrol et |
| `Symbol not resolved` | Fonksiyon adı yanlış | `nm -D /path/lib.so \| grep fn_name` ile doğrula |
| Yanlış dönüş tipi | MELP tipi ile C tipi uyumsuz | Tip tablosuna göre imzayı düzelt |
| Variadic fonksiyon çağrısı | `printf` gibi fonksiyonlar desteklenmez | Sabit parametreli sarmalayıcı (wrapper) yaz |

> **Stage 0 kısıtlaması:** Geri çağrım (callback) ve karmaşık struct aktarımı henüz desteklenmiyor.

---

## Daha Fazlası

- Elle bellek yönetimi ile birlikte kullanım: [bellek.md](bellek.md)
