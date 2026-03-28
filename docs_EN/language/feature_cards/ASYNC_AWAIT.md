# ASYNC_AWAIT - Özellik Kartı

**Özellik:** `async` / `await` — Asenkron görev yönetimi  
**Durum:** 💡 DESIGN (Stage 1.5 — sözdizimi henüz belirlenmedi)  
**Hafta:** Stage 1.5+  
**Sözdizimi Sürümü:** mlp2

---

## 🎯 ÖZELLİK

Asenkron fonksiyon bildirimi ve bekleme syntax'ı. **Sözdizimi henüz belirlenmedi** — bu kart tasarım notlarını içerir. `Channel{T}` + `spawn` tasarımından bağımsız değil; önce o tamamlanacak.

---

## 📖 SÖZDİZİMİ

```mlp
-- ⚠️ TASLAK SÖZDİZİMİ — kesin değil, Stage 1.5 tasarımında değişebilir

-- Seçenek A: async anahtar kelimesi ile
async Promise{User} function fetch_user(string id)
    -- gövde
end function

string result = await fetch_user("42")

-- Seçenek B: Task{T} return tipi yeterli
Task{string} function fetch_data(string url)
    -- gövde
end function

Task{string} t = fetch_data("http://...")
string data = t.await()

-- Seçenek C: spawn + Channel tabanlı (tercih edilen)
void function fetch_async(string url; Channel{Result{string; string}} ch)
    Result{string; string} r = http_get(url)
    send(ch; r)
end function
```

**Kesin Olmayan Noktalar:**
- `async` anahtar kelimesi mi, `Task{T}` return tipi mi?
- `await` fonksiyon mu, operatör mü?
- Hata yayılımı: `Result{T;E}` + `?` ile veya catch ile?

**Mevcut Durum — YZ DİKKAT:**
```mlp
-- ⚠️ KESİNLİKLE EKLEMEYİN — sözdizimi belirlenmedi
-- Asenkron ihtiyaç için spawn + Channel kullanın (Stage 1.5)
-- Şu an: senkron FFI (C stdlib ffi)
```

---

## 🧪 GOLDEN TEST

```mlp
-- Henüz test yazılamaz — sözdizimi tasarım aşamasında
-- Stage 1.5 tasarım kararı sonrası bu karta test eklenir
```

---

**Bağımlılıklar:** Channel{T}, spawn, Result{T;E}  
**Hafta:** Stage 1.5+ (Aralık 2026 sonrası)  
**Referans Diller:** Rust async/await, JavaScript Promise, Python asyncio, Go goroutines  
**Tasarım Notu:** Channel+spawn yaklaşımı async/await'ten önce implement edilecek
