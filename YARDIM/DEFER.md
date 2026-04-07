# MELP Defer Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 📅 PLANNED — Week 6.1 (Mart 2026)

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [LIFO Sıralaması](#lifo-sıralaması)
4. [Hata Durumu](#hata-durumu)
5. [Geçici Alternatif](#geçici-alternatif)
6. [İlgili Özellikler](#ilgili-özellikler)
7. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
-- Hedef syntax (Week 6.1 sonrası):
defer ifade

-- Yaygın kullanım:
defer f.close()
defer lock.release()
```

`defer` ile işaretlenen ifade, mevcut fonksiyon döndüğünde (başarı veya hata) otomatik çalışır.

> **⚠️ UYARI:** `defer` henüz implement edilmemiştir (Week 6.1 planlanmıştır).  
> Mevcut durumda manuel cleanup yapın — aşağıdaki [Geçici Alternatif](#geçici-alternatif) bölümüne bakınız.

---

## Kullanım Örnekleri

> Aşağıdaki örnekler hedef syntax'ı göstermektedir.

### Örnek 1: Dosya kapatma
```mlp
-- Hedef (Week 6.1 sonrası):
numeric function dosya_isle(string yol)
    string f = open_file(yol)
    defer f.close()         -- Fonksiyon bitince otomatik çağrılır
    
    string icerik = f.read()
    return icerik.length
end function              -- defer burada çalışır
```

### Örnek 2: Mutex / kilit serbest bırakma
```mlp
-- Hedef (Week 6.1 sonrası):
numeric function kritik_bolge()
    string kilit = acquire_lock()
    defer kilit.release()
    -- kritik işlemler...
    return 0
end function
```

---

## LIFO Sıralaması

Birden fazla `defer` **LIFO (Last In, First Out)** sırasıyla çalışır:

```mlp
-- Hedef (Week 6.1 sonrası):
numeric function cok_kaynak()
    string f1 = open("a.txt")
    defer f1.close()       -- 3. çalışır (ilk defer)
    
    string f2 = open("b.txt")
    defer f2.close()       -- 2. çalışır
    
    string kilit = acquire_lock()
    defer kilit.release()  -- 1. çalışır (son defer)
    
    return 0
end function
```

---

## Hata Durumu

`defer` erken `return` ve hata durumlarında da tetiklenir:

```mlp
-- Hedef (Week 6.1 sonrası):
numeric function guvenlı_yaz(string yol; string veri)
    string f = open_file(yol)
    defer f.close()    -- return -1 olsa da çalışır
    
    if veri.length == 0 then
        return -1      -- defer burada da tetiklenir
    end if
    
    f.write(veri)
    return 0
end function
```

---

## Geçici Alternatif

Week 6.1'e kadar manuel cleanup yapın:

```mlp
-- Şu an çalışan form:
numeric function dosya_isle(string yol)
    string f = open_file(yol)
    string icerik = f.read()
    numeric sonuc = icerik.length
    f.close()              -- manuel cleanup
    return sonuc
end function
```

---

## İlgili Özellikler

- **FILE_IO** — Dosya işlemleri
- **ERROR_HANDLING** — Hata yönetimi

**Ayrıca bakınız:**
- [FILE_IO.md](FILE_IO.md)
- [ERROR_HANDLING.md](ERROR_HANDLING.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `defer ifade` | ❌ | ❌ | ✅ Week 6.1 |
| LIFO sıralama | ❌ | ❌ | ✅ |
| Erken return ile | ❌ | ❌ | ✅ |
| Manuel cleanup | ✅ | ✅ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/DEFER.md](../belgeler/language/feature_cards/DEFER.md)
- **pmlp Syntax:** [belgeler/pmlp_sozdizimi.md](../belgeler/pmlp_sozdizimi.md)
- **Referans:** Go `defer`, Swift `defer`, Zig `defer`
