# Manuel Bellek Yönetimi — Kullanım Rehberi

Bu kısa rehber Stage1 için güncel kullanıcı akışını özetler.

---

## Ne Zaman Kullanılır

Stage 0'da **çöp toplayıcı (GC) yoktur**, **RAII yoktur**, **ownership yoktur**.
Kendiniz ayırdığınız her belleği serbest bırakmak **sizin sorumluluğunuzdur**.

Kullanım durumları:
- Büyük veya dinamik boyutlu veri yapıları oluşturmak
- C kütüphanesine (`allocate`/`free`) bellek geçmek
- `Vector<T>` ile dinamik dizi kullanmak

---

## Temel Sözdizimi — allocate / free

```
function main()
  -- 100 bayt bellek ayır
  pointer ptr = allocate(100)

  if ptr == null then
    print("Bellek ayırma başarısız!")
    return 1
  end if

  -- Belleği kullan...

  -- MUTLAKA serbest bırak!
  free(ptr)
  return 0
end function
```

---

## Vector\<T\> — Dinamik Dizi

```
function demonstrate()
  -- Oluştur
  Vector<numeric> sayilar = Vector<numeric>.new()

  -- Ekle (otomatik büyüme: 4 → 8 → 16 → ...)
  sayilar.append(10)
  sayilar.append(20)
  sayilar.append(30)

  -- Oku
  numeric birinci = sayilar.get(0)
  numeric uzunluk = sayilar.length()
  print(uzunluk)

  -- MUTLAKA serbest bırak!
  sayilar.free()
end function
```

Desteklenen tipler: `Vector<numeric>`, `Vector<string>`, `Vector<pointer>`

---

## Sık Yapılan Hatalar

| Hata | Açıklama | Çözüm |
|---|---|---|
| **Bellek sızıntısı** | `free()` çağrılmadı | Her çıkış yolunda `free()` ekle |
| **Çift serbest bırakma** | `free(ptr)` iki kez çağrıldı | İlk `free()`'den sonra `ptr = null` yap |
| **Serbest bırakıldıktan sonra kullanım** | `free()` sonrası `vec.get()` çağrıldı | `free()` sonrası o işaretçiyi kullanma |
| **Eksik serbest bırakma (erken return)** | Hata dalında `free()` unutuldu | Tüm kod dallarına `free()` ekle |

```
-- ✅ DOĞRU: tüm çıkış yolları free() içeriyor
function isle()
  Vector<numeric> veri = Vector<numeric>.new()
  veri.append(10)

  if hata_durumu then
    veri.free()
    return 1
  end if

  veri.free()
  return 0
end function
```

---

## Bellek Büyüme Tablosu

| Eleman sayısı | Kapasite |
|---|---|
| 1–4 | 4 |
| 5–8 | 8 |
| 9–16 | 16 |
| 17–32 | 32 |

Append: O(1) amortize. Büyüme: O(n) (yalnızca kapasite dolduğunda).

---

## Daha Fazlası

- FFI ile birlikte bellek yönetimi: [ffi.md](ffi.md)
