# MELP — Değişken Tanımlama

**Durum:** ✅ Tam Çalışıyor  
**Versiyon:** Stage 0 (pmlp0)

---

## Temel Sözdizimi

```mlp
tip isim = değer
```

Üç zorunlu kural:
1. **Tip önce** gelir
2. **Noktalı virgül YOK** satır sonunda
3. **Tip her zaman açık** belirtilir (`var`, `let`, `auto` yok)

---

## Değişken Tipleri

### numeric
```mlp
numeric x = 42
numeric pi = 3,14159
numeric nüfus = 84.000.000
numeric negatif = -7
```

### string
```mlp
string isim = "Ali"
string selamlama = "Merhaba"   -- çift tırnak kullanılmalı
string bos = ""
```

### boolean
```mlp
boolean aktif = true
boolean bitti = false
```

---

## Türkçe Sayı Formatı ⚠️

MELP Türkçe sayı formatı kullanır:

| Amaç | MELP | Klasik (İngilizce) |
|------|------|--------------------|
| Ondalık | `3,14` | `3.14` |
| Binler | `1.000` | `1,000` |
| Karma | `126.555.852,36` | `126,555,852.36` |

```mlp
numeric pi = 3,14159          -- Doğru ✅
numeric pi = 3.14             -- Yanlış ❌ (3 olarak okunur)
numeric milyon = 1.000.000    -- Doğru ✅
```

---

## Atama (Tanımdan Sonra)

Tip tekrar yazılmaz, sadece değer atanır:

```mlp
numeric sayac = 0
sayac = sayac + 1         -- tip yok, sadece isim = ifade
sayac = 100
```

---

## İfade ile Başlatma

```mlp
numeric en = 100
numeric boy = 50
numeric alan = en * boy         -- 5000

string ad = "Ali"
string soyad = "Veli"
string tam_ad = ad + " " + soyad   -- "Ali Veli"
```

---

## Sabitler

`const` ile değiştirilemez değişken tanımlanır:

```mlp
const numeric MAX_BOYUT = 1000
const string PROJE_ADI = "MELP"
```

> ⚠️ `const` kısmi destek — Stage 0'da atama kontrolü tam uygulanmamış.

---

## Kapsam (Scope) Kuralları

- Değişkenler tanımlandıkları blokta geçerlidir.
- İç blok dış bloğu okuyabilir; dış blok iç bloğa erişemez.
- Şu an global değişken tanımı desteklenmez (fonksiyon dışı).

```mlp
function ornk()
    numeric a = 10
    if a > 5 then
        numeric b = 20     -- sadece if bloğunda geçerli
        print(a)           -- ✅ dış değişkene erişim
    end if
    -- print(b)            -- ❌ b burada geçersiz
end function
```

---

## Tip Dönüşümü

```mlp
string s = str(42)              -- numeric → string
numeric n = to_number("42")     -- string → numeric
boolean b = to_boolean(1)       -- numeric → boolean
```

> Gelecekte: `str()` fonksiyon adı Stage 2'de `to_string()` ile birleşecek.

---

## Anti-Pattern'lar

```mlp
-- ❌ Python stili (YANLIŞ)
x = 42
isim = "Ali"

-- ❌ JavaScript stili (YANLIŞ)
let x = 42;
const isim = "Ali";

-- ❌ Tip sonra (YANLIŞ)
x: numeric = 42
isim: string = "Ali"

-- ✅ MELP stili (DOĞRU)
numeric x = 42
string isim = "Ali"
```

---

## SSS

**S: Virgül mü, nokta mı kullanacağım ondalık için?**  
C: Virgül (`,`) ondalık ayırıcı, nokta (`.`) binler ayırıcıdır. `3,14` doğru, `3.14` yanlış.

**S: Değişkeni tekrar tanımlayabilir miyim aynı isimle?**  
C: Aynı kapsamda hayır. Farklı kapsamda (iç blok) ise yeni tanım dışarıdakini gizler.

**S: `null` atayabilir miyim?**  
C: `numeric? n = null` sözdizimi planlandı (`?` nullable). Stage 0'da kısmi destek var.

**S: `auto` veya `var` yok mu?**
C: Yok. MELP'te tip her zaman açıkça yazılır — bu bir tasarım kararıdır.

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `string s = 'merhaba'` (tek tırnak) | `string s = "merhaba"` |
| `let x = 42` | `numeric x = 42` |
| `var x = 42` | `numeric x = 42` |
| `x: numeric = 42` | `numeric x = 42` |
| `end_function` | `end function` |
| `end_if` | `end if` |
