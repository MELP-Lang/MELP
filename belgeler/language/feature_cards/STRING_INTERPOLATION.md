# STRING_INTERPOLATION - Özellik Kartı

**Özellik:** String Interpolation — `"{name}"` sözdizimi, runtime string üretimi  
**Durum:** 📅 PLANNED (Week 6.2, Mart-Nisan 2026)  
**Hafta:** Week 6.2  
**Sözdizimi Sürümü:** pmlp2

---

## 🎯 ÖZELLİK

String literal içindeki `{expr}` ifadeleri runtime'da değerlendirilerek string'e dönüştürülür. Normalizer katmanında `+` concatenation'a dönüştürülecek (zero compiler overhead). En çok kullanılan "YZ düşme tuzağı" — YZ'ler mevcut durumda `+` kullanmalı.

---

## 📖 SÖZDİZİMİ

```pmlp
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
```pmlp
-- Kaynak:
string msg = "Merhaba {name}, yaşın {age}!"

-- Normalizer çıktısı:
string msg = "Merhaba " + name + ", yaşın " + age + "!"
```

**Mevcut Durum (Week 5.x) — YZ DİKKAT:**
```pmlp
-- ⚠️ Henüz ÇALIŞMIYOR — şu an + kullan:
string msg = "Merhaba " + name + ", yaşın " + age + "!"
```

---

## 🧪 GOLDEN TEST

```pmlp
function main() as numeric
    string name = "MELP"
    numeric version = 1
    -- Week 6.2 sonrası:
    -- string s = "Dil: {name} v{version}"
    -- if s == "Dil: MELP v1" then return 1 end_if
    -- Şu an:
    string s = "Dil: " + name + " v" + version
    if s == "Dil: MELP v1" then
        return 1  -- PASS
    end_if
    return 0
end_function
```

---

**Bağımlılıklar:** Normalizer katmanı  
**Hafta:** Week 6.2 (Mart-Nisan 2026)  
**Kanonik Kaynak:** `belgeler/pmlp_sozdizimi.md` L1275 civarı  
**Referans Diller:** Python f-string, Kotlin `"$var"`, Swift `"\(expr)"`
