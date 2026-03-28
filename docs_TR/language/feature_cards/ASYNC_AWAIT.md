# ASYNC_AWAIT — Ozellik Karti

**Ozellik:** `async` / `await` — Asenkron gorev yonetimi  
**Durum:** 💡 TASARIM (Stage 1.5 — sozdizimi henuz belirlenmedi)  
**Sozdizimi Surumu:** mlp2

---

## 🎯 OZELLIK

Asenkron fonksiyon bildirimi ve bekleme sozdizimi. **Sozdizimi henuz kesinlesmedi** — bu kart tasarim notlarini icerir. `Channel{T}` + `spawn` tasarimindan bagimsiz degil; once o tamamlanacak.

---

## 📖 SOZDIZIMI (TASLAK)

```mlp
-- ⚠️ TASLAK SOZDIZIMI — kesin degil, Stage 1.5'te degisebilir

-- Secenek A: async anahtar kelimesi ile
async Promise{string} function kullanici_getir(string id)
    -- govde
end function

string sonuc = await kullanici_getir("42")

-- Secenek B: Task{T} donus tipi yeterli
Task{string} function veri_getir(string url)
    -- govde
end function

Task{string} t = veri_getir("http://...")
string veri = t.await()

-- Secenek C: spawn + Channel tabanli (tercih edilen yaklasim)
void function async_getir(string url; Channel{Result{string; string}} ch)
    Result{string; string} r = http_get(url)
    send(ch; r)
end function
```

**Kesinlesmemis Noktalar:**
- `async` anahtar kelimesi mi, `Task{T}` donus tipi mi?
- `await` fonksiyon mu, operator mu?
- Hata yayilimi: `Result{T;E}` + `?` ile mi, catch ile mi?

**Mevcut Durum — YZ DİKKAT:**
```mlp
-- ⚠️ KESİNLİKLE EKLEMEYİN — sozdizimi belirlenmedi
-- Asenkron ihtiyac icin spawn + Channel kullanin (Stage 1.5)
-- Su an: senkron FFI (C stdlib ffi)
```

**Stage0 Parser Notu (teknik):**  
`async function` → parser tarafindan normal `FUNCTION` nodu olarak islenir (`ASYNC_FUNCTION` marker eklenir, codegen kullanmaz).  
`await expr` → **senkron pass-through** — ifadeyi dogrudan hesaplar, beklemez, dondurmez.  
Sonuc: crash vermez; ama gercek asenkron davranis sifirdir. Event loop, thread scheduler ve timer olmadan async anlamsizdir.  
Bu eksikler Stage 1.5 kapsamindadir.

---

## 🧪 GOLDEN TEST

```mlp
-- Henuz test yazilamaz — sozdizimi tasarim asamasinda
-- Stage 1.5 tasarim karari sonrasi bu karta test eklenir
```

---

**Bagimliliklar:** Channel{T}, spawn, Result{T;E}  
**Surec:** Stage 1.5+ (Aralik 2026 sonrasi)  
**Referans Diller:** Rust async/await, JavaScript Promise, Python asyncio, Go goroutines  
**Tasarim Notu:** Channel+spawn yaklasimi async/await'ten once implement edilecek
