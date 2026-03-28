# STRING_INTERPOLATION - Özellik Kartı

**Özellik:** String Interpolation — `"{name}"` sözdizimi, runtime string üretimi  
**Durum:** 📅 PLANNED (Week 6.2, Mart-Nisan 2026)  
**Hafta:** Week 6.2  
**Sözdizimi Sürümü:** mlp2

---

## 🎯 ÖZELLİK

String literal içindeki `{expr}` ifadeleri runtime'da değerlendirilerek string'e dönüştürülür. Normalizer katmanında `+` concatenation'a dönüştürülecek (zero compiler overhead). En çok kullanılan "YZ düşme tuzağı" — YZ'ler mevcut durumda `+` kullanmalı.

---

## 📖 SÖZDİZİMİ

```mlp
-- Hedef sözdizimi (Week 6.2)
string name = "Ali"
numeric age = 25
string msg = "Merhaba {name}, yaşın {age}!"
-- → "Merhaba Ali, yaşın 25!"

-- İfade interpolasyonu
numeric x = 10
string result = "x'in karesi: {x * x}"
-- → "x'in karesi: 100"

-- Fonksiyon çağrısı interpolasyonu
string info = "Kullanıcı: {get_username()}"
```

**Normalizer Dönüşümü (compile-time):**
```mlp
-- Kaynak:
string msg = "Merhaba {name}, yaşın {age}!"

-- Normalizer çıktısı:
string msg = "Merhaba " + name + ", yaşın " + age + "!"
```

**Mevcut Durum (Week 5.x) — YZ DİKKAT:**
```mlp
-- ⚠️ Henüz ÇALIŞMIYOR — şu an + kullan:
string msg = "Merhaba " + name + ", yaşın " + age + "!"
```

---

## 🧪 GOLDEN TEST

```mlp
function main()
    string name = "MELP"
    numeric version = 1
    -- Week 6.2 sonrası:
    -- string s = "Dil: {name} v{version}"
    -- print(s)
    -- Şu an:
    string s = "Dil: " + name + " v" + version
    print(s)
end function
-- Output: Dil: MELP v1
```

---

**Bağımlılıklar:** Normalizer katmanı  
**Hafta:** Week 6.2 (Mart-Nisan 2026)  
**Kanonik Kaynak:** `belgeler/mlp_sozdizimi.md` L1275 civarı  
**Referans Diller:** Python f-string, Kotlin `"$var"`, Swift `"\(expr)"`
