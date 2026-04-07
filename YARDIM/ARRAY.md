# Dizi (Array) — MELP Başvuru

**Durum:** ✅ Temel — okuma/for/for-each tam; atama ve `length` kısmi  
**Kaynak feature kartı:** `belgeler/language/feature_cards/ARRAY.md`  
**Test tarihi:** 8 Mart 2026

---

## Tanımlama

```mlp
numeric[] nums   = [10; 20; 30; 40; 50]
string[] isimler = ["Ali"; "Veli"; "Ayse"]
boolean[] flags  = [true; false; true]
```

**Kritik kurallar:**
- Tip sözdizimi: `numeric[]` — `array<numeric>` YASAK (segfault verir)
- Eleman ayırıcı: **`;`** (noktalı virgül) — virgül `,` değil
- İndeksler **0-tabanlıdır**

---

## Eleman Erişimi

```mlp
numeric[] nums = [10; 20; 30; 40; 50]
println(str(nums[0]))   -- 10
println(str(nums[2]))   -- 30
println(str(nums[4]))   -- 50

string[] isimler = ["Ali"; "Veli"; "Ayse"]
println(isimler[0])     -- Ali
println(isimler[2])     -- Ayse
```

---

## Döngüyle İterasyon

### `for` döngüsü
```mlp
string[] isimler = ["Ali"; "Veli"; "Ayse"]
loop i = 0 to 2
    println(isimler[i])
end loop
-- Ali
-- Veli
-- Ayse
```

### `for each` döngüsü
```mlp
numeric[] nums = [10; 20; 30]
loop each n in nums
    println(str(n))
end loop
-- 10
-- 20
-- 30
```

---

## Uzunluk (Kısmi ⚠️)

```mlp
-- length() derleme uyarısı veriyor ama 0 döndürüyor
println(str(length(nums)))   -- Uyarı + 0 döner
```

> ⚠️ `length()` henüz tam çalışmıyor — dizi boyutunu değişkende sakla.

**Geçici çözüm:** Dizin boyutunu manuel takip et.
```mlp
numeric boyut = 5   -- nums dizisinin boyutu
loop i = 0 to boyut - 1
    println(str(nums[i]))
end loop
```

---

## İndeks Ataması (Kısmi ⚠️)

```mlp
-- Sözdizimi derleniyor ama 0 yazıyor
nums[1] = 99
println(str(nums[1]))   -- 0 okur (hatalı)
```

> ⚠️ `arr[i] = val` ataması derleyicide bilinen bir bug'dur. Stage 0'da mevcut kısıtlama.

---

## Fonksiyon Parametresi Olarak Dizi

```mlp
numeric function topla(numeric[] dizi)
    numeric toplam = 0
    loop each n in dizi
        toplam = toplam + n
    end loop
    return toplam
end function

function main()
    numeric[] sayilar = [10; 20; 30]
    println(str(topla(sayilar)))   -- 60
end function
```

---

## 2D Dizi (Feature Kartta Belirtilmiş)

```mlp
-- Sözdizimi kabul edilmesi bekleniyor
numeric[][] matris = [[1; 2]; [3; 4]]
println(str(matris[0][0]))   -- 1
```

> ⚠️ 2D dizi test edilmemiş — dikkatli kullan.

---

## Durum Özeti

| Özellik | Durum |
|---------|-------|
| `T[]` tip sözdizimi | ✅ |
| Literal tanımlama `[a; b; c]` | ✅ |
| İndeks okuma `arr[i]` | ✅ |
| `for i = 0 to n` iterasyon | ✅ |
| `for each x in arr` | ✅ |
| `length(arr)` | ⚠️ Derlenir, 0 döner |
| `arr[i] = val` atama | ⚠️ Derlenir, 0 okur (bug) |
| `push` / `pop` | ❌ Yok |
| `array<T>` sözdizimi | ❌ YASAK — segfault |

---

*Bağlantılı dosyalar: [LOOPS.md](LOOPS.md) · [TYPES.md](TYPES.md) · [COLLECTIONS.md](HASHMAP.md)*
