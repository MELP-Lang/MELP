# HashMap — MELP Başvuru

**Durum:** ✅ Temel — oku/yaz/sıfır çalışıyor; metod sözdizimi ❌ (EKS-014)  
**Kaynak feature kartı:** `belgeler/language/feature_cards/HASHMAP.md`  
**Test tarihi:** 8 Mart 2026

---

## Tanımlama

```mlp
hashmap<string; numeric> isim     = {}
hashmap<string; string>  kayit    = {}
hashmap<numeric; numeric> indeks  = {}
```

- Sözdizimi: `hashmap<AnahtarTipi; DeğerTipi>`
- Eleman ayırıcı **`;`** (virgül değil)
- Başlangıç değer: `{}`

---

## Değer Atama ve Okuma

```mlp
hashmap<string; numeric> puan = {}
puan["ali"]  = 95
puan["veli"] = 87
puan["ayse"] = 100

println(str(puan["ali"]))    -- 95
println(str(puan["veli"]))   -- 87

-- Olmayan anahtar: 0 döner (numeric), "" döner (string)
println(str(puan["yok"]))    -- 0
```

---

## has / size / delete — FONKSİYON ÇAĞRISI SÖZDİZİMİ

> ⚠️ **EKS-014:** `puan.has("ali")` metod sözdizimi **çalışmıyor** — `hashmap_str_has(puan; "ali")` fonksiyon sözdizimi kullan.

### String anahtarlı hashmap fonksiyonları

```mlp
hashmap<string; numeric> puan = {}
puan["ali"] = 95
puan["veli"] = 87

-- Varlık kontrolü
numeric varMi = hashmap_str_has(puan; "ali")     -- 1 (var)
numeric yokMu = hashmap_str_has(puan; "mehmet")  -- 0 (yok)
println(str(varMi))    -- 1
println(str(yokMu))    -- 0

-- Boyut
numeric boyut = hashmap_str_size(puan)
println(str(boyut))    -- 2

-- Silme
hashmap_str_delete(puan; "veli")
println(str(hashmap_str_size(puan)))   -- 1

-- Kalan
println(str(hashmap_str_has(puan; "veli")))   -- 0
```

### Numeric anahtarlı hashmap fonksiyonları

```mlp
hashmap<numeric; numeric> veri = {}
veri[1] = 100
veri[2] = 200

numeric varMi = hashmap_has(veri; 1)     -- 1
numeric boyut = hashmap_size(veri)       -- 2
hashmap_delete(veri; 1)
println(str(hashmap_size(veri)))         -- 1
```

---

## Tam Örnek

```mlp
function main()
    hashmap<string; numeric> notlar = {}
    notlar["matematik"] = 90
    notlar["fizik"]     = 75
    notlar["kimya"]     = 88

    println("matematik=" + str(notlar["matematik"]))   -- matematik=90
    println("fizik="     + str(notlar["fizik"]))       -- fizik=75

    if hashmap_str_has(notlar; "kimya") == 1 then
        println("kimya mevcut")
    end if

    println("toplam=" + str(hashmap_str_size(notlar)))   -- toplam=3

    hashmap_str_delete(notlar; "fizik")
    println("fizik silindi, toplam=" + str(hashmap_str_size(notlar)))   -- 2

    -- Olmayan anahtar
    println("biyoloji=" + str(notlar["biyoloji"]))   -- biyoloji=0

end function
```

---

## Fonksiyon Referansı

| İşlem | String anahtarlı | Numeric anahtarlı |
|-------|------------------|-------------------|
| Oluşturma | `hashmap<string; T> m = {}` | `hashmap<numeric; T> m = {}` |
| Atama | `m[key] = val` | `m[key] = val` |
| Okuma | `m[key]` | `m[key]` |
| Varlık | `hashmap_str_has(m; key)` | `hashmap_has(m; key)` |
| Boyut | `hashmap_str_size(m)` | `hashmap_size(m)` |
| Silme | `hashmap_str_delete(m; key)` | `hashmap_delete(m; key)` |

---

## Önemli Notlar

| Konu | Açıklama |
|------|----------|
| Olmayan anahtar | `0` (numeric) veya `""` (string) döner |
| Metod sözdizimi | `m.has(key)` çalışmıyor — EKS-014 |
| Eleman ayırıcı | `hashmap_str_has(m; key)` — `;` kullan |
| `for each` keys | Henüz desteklenmiyor |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_function` | `end function` |
| `end_if` | `end if` |
| `return 0` (main içinde) | kaldır |
| `m.has(key)` | `hashmap_str_has(m; key)` |

---

*Bağlantılı dosyalar: [ARRAY.md](ARRAY.md) · [TYPES.md](TYPES.md)*
