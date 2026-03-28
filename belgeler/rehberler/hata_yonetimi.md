# Hata Yönetimi — Kullanım Rehberi

Bu kısa rehber Stage1 için güncel kullanıcı akışını özetler.

---

## Ne Zaman Kullanılır

`Result<T; E>` ve `Option<T>` aşağıdaki durumlarda kullanılır:

- Bir işlemin başarılı ya da başarısız olabileceğini açıkça ifade etmek
- İstisna (exception) yerine değer tabanlı hata yönetimi yapmak
- `null` ya da sihirli sayı (-1 gibi) döndürmek yerine anlamlı hata bilgisi iletmek

---

## Temel Sözdizimi — Result\<T; E\>

```
enum Result<T; E>
  Ok(T)
  Err(E)
end enum

-- Başarı durumu
Result basari = Result.Ok(42)
match basari
  case Ok(deger) -> print(deger)    -- Çıktı: 42
  case Err(hata) -> print(0)
end match

-- Hata durumu
Result hata = Result.Err(404)
match hata
  case Ok(deger) -> print(0)
  case Err(kod) -> print(kod)       -- Çıktı: 404
end match
```

---

## Option\<T\> Kalıbı

```
enum Option<T>
  Some(T)
  None
end enum

Option belki = Option.Some(100)
match belki
  case Some(v) -> print(v)          -- Çıktı: 100
  case None -> print(0)
end match

Option hicbir = Option.None
match hicbir
  case Some(v) -> print(999)
  case None -> print(0)             -- Çıktı: 0
end match
```

---

## Sık Yapılan Hatalar

| Hata | Nedeni | Çözüm |
|---|---|---|
| İfade argümanı çalışmıyor | `Result.Ok(x + y)` doğru değer vermiyor | Önce değeri bir değişkene ata, sonra geç |
| Yeniden atama sonrası eski değer | `r = Result.Err(2)` sonrası eski değer kalıyor | Her durum için ayrı değişken kullan |
| Yalnızca `Ok` dalı yazıldı | Eksik `Err` dalı → derleme uyarısı | Her iki dalı her zaman yaz |
| String payload desteklenmiyor | `Result.Err("mesaj")` henüz yok | Sayısal hata kodu kullan |

```
-- ❌ YANLIŞ: ifade argümanı
numeric x = 10
numeric y = 5
Result r = Result.Ok(x + y)       -- 1 yazdırabilir (hata)

-- ✅ DOĞRU: önce hesapla
numeric toplam = x + y
Result r = Result.Ok(toplam)      -- 15 yazdırır
```

---

## Önerilen Kalıplar

**1. Önceden hesapla, sonra geç:**
```
numeric sonuc = hesapla()
Result r = Result.Ok(sonuc)
```

**2. Her dal için ayrı değişken:**
```
Result r1 = Result.Ok(1)
Result r2 = Result.Err(2)
```

**3. Her zaman her iki dalı yaz:**
```
match sonuc
  case Ok(v) -> print(v)
  case Err(e) -> print(0)
end match
```

---

## Daha Fazlası

- Stage 1 enum desteği: [../dil/referans.md](../dil/referans.md) — Enum bölümü
