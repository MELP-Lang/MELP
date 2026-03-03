# BASIC_TYPES - Özellik Kartı

**Özellik:** Temel tipler — `numeric`, `string`, `boolean`, `null`, `void`  
**Durum:** ✅ COMPLETE (Stage 0)  
**Hafta:** Stage 0  
**Sözdizimi Sürümü:** pmlp0

---

## 🎯 ÖZELLİK

MELP'te tüm sayılar `numeric` tiptir — ayrı integer/float yok. Türkçe ondalık: virgül (`,`) kullanılır nokta (`.`) değil. `string` hem tek hem çift tırnak kabul eder.

---

## 📖 SÖZDİZİMİ

```pmlp
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

-- void: fonksiyon dönüř tipi (değer tutmaz)
function log(string msg) as void
    print(msg)
end_function

-- tip dönüşümü
string s = str(42)              -- numeric → string  ⚠️ Stage2: str() (eski: to_string())
numeric n = to_number("3,14")   -- string → numeric
boolean b = to_boolean(1)       -- numeric → boolean
```

**Türkçe Sayı Formatı (KRİTİK):**
```pmlp
-- ✅ DOĞRU
numeric pi = 3,14
numeric rate = 0,05

-- ❌ YANLIŞ (İngilizce format)
numeric pi = 3.14    -- HATA veya integer parse
```

**Tip Çıkarımı:**
```pmlp
-- MELP'te açık tip bildirimi zorunludur
-- ❌ Yok: var x = 42  veya  let x = 42
-- ✅ Her zaman: numeric x = 42
```

---

## 🧪 GOLDEN TEST

```pmlp
function main() as numeric
    numeric i = 10
    numeric f = 2,5
    string s = "test"
    boolean b = true
    if i == 10 then
        if f == 2,5 then
            if s == "test" then
                if b == true then return 1 end_if
            end_if
        end_if
    end_if
    return 0
end_function
```

---

**Bağımlılıklar:** Yok (temel)  
**Hafta:** Stage 0  
**Kanonik Kaynak:** `pmlp0_sozdizimi.md` — Temel tipler bölümü
