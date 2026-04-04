# MELP — Hata Yönetimi

**Durum:**  
- `defer` ✅ Tam çalışıyor  
- `try/catch/throw/finally` ✅ Tam (EKS-011, 8 Mart 2026)  
**Versiyon:** Stage 0 (pmlp0)  
**Test Tarihi:** 8 Mart 2026

---

## Özet

| Keyword | Açıklama | Durum |
|---------|----------|-------|
| `defer` | Fonksiyon bitişinde otomatik çalışır | ✅ Tam |
| `try` / `end try` | Hata yakalama bloğu | ✅ Tam |
| `catch` | Hata dalı | ✅ Tam |
| `throw` | Hata fırlat | ✅ Tam |
| `finally` | Her durumda çalışır | ✅ Tam |

---

## defer ✅

`defer`, bir ifadeyi fonksiyon çıkışına erteler. `return` nerede olursa olsun tetiklenir. Birden fazla `defer` varsa **LIFO** (son giren, ilk çıkar) sırasıyla çalışır.

### Temel Kullanım

```mlp
function main()
    defer println("temizlik")   -- en sonda çalışır
    println("iş yapılıyor")
end function
-- Çıktı:
-- iş yapılıyor
-- temizlik
```

### LIFO Sırası (Doğrulandı ✅)

```mlp
function main()
    defer println("A — ilk eklenen")
    defer println("B — ikinci")
    defer println("C — son eklenen")
    println("başlıyor")
end function
-- Çıktı:
-- başlıyor
-- C — son eklenen
-- B — ikinci
-- A — ilk eklenen
```

### Erken Return'de defer

`defer`, `return` öncesinde de tetiklenir:

```mlp
numeric function kontrol(numeric x)
    defer println("kontrol bitti")
    if x < 0 then
        println("negatif, erken çıkış")
        return -1                    -- defer burada tetiklenir
    end if
    println("değer: " + str(x))
    return x
end function
-- kontrol(-5) çıktısı:
-- negatif, erken çıkış
-- kontrol bitti
```

### Kaynak Temizleme Örüntüsü

`defer`, `try/finally` yerine kullanılabilir:

```mlp
numeric function islemi_yap(string dosya)
    -- Gerçek dosya API'si geldiğinde:
    -- string f = ac(dosya)
    -- defer f.kapat()   ← artık her durumda kapanır

    defer println("dosya kapatıldı: " + dosya)

    -- İş yap
    println("işleniyor: " + dosya)
    return 0
end function
```

### Dikkat: defer İfadeleri

`defer` tek bir ifade alır:

```mlp
-- ✅ Doğru — tek ifade
defer println("bitti")
defer sayac = sayac + 1

-- ✅ Doğru — fonksiyon çağrısı
defer temizle()

-- ❌ Yanlış — blok değil
defer
    println("a")
    println("b")
end defer   -- bu sözdizimi yok
```

> Birden fazla temizlik için birden fazla `defer` satırı yazın.

---

## try / catch / throw ✅

**EKS-011** tamamlandı. `try/catch/throw/finally` tam olarak çalışıyor.

### Temel Kullanım

```mlp
string function dosya_oku(string yol)
    if yol == "" then
        throw "dosya bulunamadi"
    end if
    return "icerik"
end function

function main()
    try
        string icerik = dosya_oku("")
        print(icerik)
    catch hata_mesaj
        print("Hata: " + hata_mesaj)   -- Hata: dosya bulunamadi
    end try
end function
```

### finally ile

`finally` bloğu, `try` başarılı olsa da `catch` tetiklense de **her durumda** çalışır:

```mlp
function riskli()
    throw "test hatasi"
end function

function main()
    try
        riskli()
    catch e
        print("hata yakalandi")
    finally
        print("her zaman calisir")
    end try
end function
```

### throw

Herhangi bir fonksiyondan `string` hata mesajı fırlatılabilir:

```mlp
numeric function yas_dogrula(numeric yas)
    if yas < 0 then
        throw "Yas negatif olamaz"
    end if
    return yas
end function
```

| Sözdizimi | Açıklama |
|-----------|----------|
| `try ... end try` | Hata yakalama bloğu |
| `catch degisken` | Hata mesajını yakalar |
| `finally` | Her durumda çalışır (isteğe bağlı) |
| `throw "mesaj"` | String hata mesajı fırlat |

---

## defer + if ile Alternatif Yaklaşım

`throw` kullanmak zorunda değilsiniz — hataları `if` ile yönetmek ve temizlik için `defer` kullanmak da geçerlidir:

```mlp
numeric function bolme(numeric a; numeric b)
    if b == 0 then
        print("Hata: sifira bolme")
        return -1
    end if
    return a / b
end function

function main()
    defer print("islem tamamlandi")

    numeric sonuc = bolme(10; 0)
    if sonuc == -1 then
        print("Islem basarisiz")
    else
        print("Sonuc: " + str(sonuc))
    end if
end function
```

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_function` | `end function` |
| `end_if` | `end if` |
| `end_try` | `end try` |
| `function f() as numeric` | `numeric function f()` |
| `function f() as void` | `function f()` |
| `f(a, b)` (virgül) | `f(a; b)` |
| `return 0` (main içinde) | kaldır |

---

## SSS

**S: `defer` ile `finally` ne zaman kullanılacak?**  
C: İkisi de çalışıyor. `finally` bir `try` bloğunun parçasıyken `defer` bağımsız kullanılabilir. Kaynak temizleme için `defer` genellikle daha temizdir.

**S: `defer` içinde `return` kullanabilir miyim?**  
C: Hayır. `defer` içinde `return` yazılamaz — defer bir ifade (expression) alır, blok değil.

**S: `catch` değişkeninde ne var?**  
C: `throw "mesaj"` ile atılan string değer. `catch e` yazarsanız `e` o string olur.

**S: `throw` ile sadece string fırlatılabilir mi?**  
C: Şu an evet — yalnızca `string` hata mesajı fırlatılabilir.
