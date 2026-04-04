# BASIC_TYPES - Özellik Kartı

**Özellik:** Temel tipler — `numeric`, `string`, `boolean`, `null`, `void`  
**Durum:** ✅ COMPLETE (Stage1)  
**Hafta:** Stage1  
**Sözdizimi Sürümü:** mlp0

---

## 🎯 ÖZELLİK

MELP'te tüm sayılar `numeric` tiptir — ayrı integer/float yok. Türkçe ondalık: virgül (`,`) kullanılır nokta (`.`) değil. `string` hem tek hem çift tırnak kabul eder.

---

## 📖 SÖZDİZİMİ

```mlp
-- numeric: tüm sayılar tek tip
numeric x = 42
numeric pi = 3,14        -- ⚠️ Türkçe format: virgül ondalık ayırıcı
numeric big = 1000000
numeric neg = -7

-- string: metin
string s1 = "merhaba"
string s2 = 'dünya'      -- tek tırnak da geçerli
string empty = ""

-- boolean: mantıksal
boolean flag = true
boolean done = false

-- null: boş değer
numeric? n = null        -- nullable ile kullanılır
string? name = null

-- void: fonksiyon dönüş tipi (değer tutmaz)
function log(string msg)
    print(msg)
end function

-- tip dönüşümü
string s = str(42)              -- numeric → string  ⚠️ Stage2: str() (eski: to_string())
numeric n = to_number("3,14")   -- string → numeric
boolean b = to_boolean(1)       -- numeric → boolean
```

**Türkçe Sayı Formatı (KRİTİK):**
```mlp
-- ✅ DOĞRU
numeric pi = 3,14
numeric rate = 0,05

-- ❌ YANLIŞ (İngilizce format)
numeric pi = 3.14    -- HATA veya integer parse
```

**Tip Çıkarımı:**
```mlp
-- MELP'te açık tip bildirimi zorunludur
-- ❌ Yok: var x = 42  veya  let x = 42
-- ✅ Her zaman: numeric x = 42
```

---

## 🧪 GOLDEN TEST

```mlp
function main()
    numeric i = 10
    numeric f = 2,5
    string s = "test"
    boolean b = true
    if i == 10 then
        if f == 2,5 then
            if s == "test" then
                if b == true then
                    print("all types ok")
                end if
            end if
        end if
    end if
end function
-- Output: all types ok
```

---

**Bağımlılıklar:** Yok (temel)  
**Hafta:** Stage1  
**Kanonik Kaynak:** `mlp0_sozdizimi.md` — Temel tipler bölümü
