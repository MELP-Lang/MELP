# DEFER - Özellik Kartı

**Özellik:** Defer — Fonksiyon sonunda otomatik çalışan cleanup ifadesi  
**Durum:** 📅 PLANNED (Week 6.1, Mart 2026)  
**Hafta:** Week 6.1  
**Sözdizimi Sürümü:** pmlp2

---

## 🎯 ÖZELLİK

`defer` ile işaretlenen ifade, mevcut fonksiyon döndüğünde (başarı veya hata) otomatik çalışır. Kaynak serbest bırakma (file close, mutex unlock) için kullanılır. Go ve Swift'ten ilham alır.

---

## 📖 SÖZDİZİMİ

```pmlp
-- Temel kullanım
function process_file(string path) as numeric
    string f = open_file(path)
    defer f.close()        -- Fonksiyon bitince otomatik çağrılır
    
    string content = f.read()
    return content.length
end_function  -- defer burada çalışır

-- Birden fazla defer (LIFO sırasıyla)
function multi_resource() as numeric
    string f1 = open("a.txt")
    defer f1.close()   -- 3. çalışır
    
    string f2 = open("b.txt")
    defer f2.close()   -- 2. çalışır
    
    string lock = acquire_lock()
    defer lock.release()  -- 1. çalışır (en son defer)
    
    return 0
end_function

-- Error durumunda da çalışır
function safe_write(string path; string data) as numeric
    string f = open_file(path)
    defer f.close()  -- Hata olsa da çalışır
    
    if data.length == 0 then
        return -1  -- defer burada da tetiklenir
    end_if
    
    f.write(data)
    return 0
end_function
```

**try-finally ile karşılaştırma:**
```pmlp
-- defer (temiz):
defer f.close()

-- try-finally (verbose):
try
    -- kod
finally
    f.close()
end_try
```

---

## 🧪 GOLDEN TEST

```pmlp
numeric cleanup_count = 0

function test_defer() as numeric
    defer cleanup_count = cleanup_count + 1
    defer cleanup_count = cleanup_count + 1
    return 0
end_function

function main() as numeric
    test_defer()
    if cleanup_count == 2 then
        return 1  -- PASS (LIFO: ikisi de çalıştı)
    end_if
    return 0
end_function
```

---

**Bağımlılıklar:** Yok (isteğe bağlı try-finally alternatifi)  
**Hafta:** Week 6.1 (Mart 2026)  
**Kanonik Kaynak:** `belgeler/pmlp_sozdizimi.md` L1460 civarı  
**Referans Diller:** Go `defer`, Swift `defer`, Zig `defer`
