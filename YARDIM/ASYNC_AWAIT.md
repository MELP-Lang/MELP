# MELP Asenkron Programlama (async/await) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 💡 DESIGN — Stage 1.5+ (sözdizimi henüz belirlenmedi)

---

## İçindekiler

1. [Genel Bilgi](#genel-bilgi)
2. [Hedef Syntax (Taslak)](#hedef-syntax-taslak)
3. [Mevcut Alternatif](#mevcut-alternatif)
4. [Tasarım Notları](#tasarım-notları)
5. [İlgili Özellikler](#ilgili-özellikler)
6. [Durum Tablosu](#durum-tablosu)

---

## Genel Bilgi

MELP'te asenkron programlama için `async`/`await` sözdizimi **henüz kesinleşmemiştir**.  
Özellik Stage 1.5+ için planlanmıştır (Aralık 2026 sonrası).

> **⚠️ UYARI:** Bu özellikleri kullanmayın. `Channel{T}` + `spawn` tasarımı öncelikli implement edilecek; `async`/`await` o tasarıma bağımlıdır.

---

## Hedef Syntax (Taslak)

```mlp
-- ⚠️ TASLAK — kesin sözdizimi değil, Stage 1.5 tasarımında değişebilir

-- Seçenek A: async anahtar kelimesi
async function fetch_user(string id) as Promise{User}
    -- gövde
end function

string result = await fetch_user("42")

-- Seçenek B: Task{T} return tipi
function fetch_data(string url) as Task{string}
    -- gövde
end function

Task{string} t = fetch_data("http://...")
string data = t.await()

-- Seçenek C: spawn + Channel (tercih edilen tasarım)
function fetch_async(string url; Channel{Result{string; string}} ch) as void
    Result{string; string} r = http_get(url)
    send(ch; r)
end function
```

**Henüz karara bağlanmamış sorular:**
- `async` anahtar kelimesi mi, `Task{T}` return tipi mi?
- `await` fonksiyon mu, operatör mü?
- Hata yayılımı nasıl olacak?

---

## Mevcut Alternatif

Stage 1.5'e kadar senkron çalışma kullanın:

```mlp
-- Şu an: senkron + FFI (C stdlib)
external function sleep(numeric seconds) as numeric

function main()
    -- Senkron iş
    string result = fetch_data_sync("http://...")
    print(result)
end function
```

---

## Tasarım Notları

`Channel{T}` + `spawn` yaklaşımı `async/await`'ten önce implement edilecek.  
Detaylar için: [CHANNELS.md](CHANNELS.md)

**Referans diller:** Rust async/await, JavaScript Promise, Python asyncio, Go goroutines.

---

## İlgili Özellikler

- **CHANNELS** — Channel{T} ve spawn (Stage 1.5 çekirdek tasarımı)
- **RESULT_OPTION** — Hata yayılımı

**Ayrıca bakınız:**
- [CHANNELS.md](CHANNELS.md)
- [RESULT_OPTION.md](RESULT_OPTION.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage1.5 | Stage2 |
|---------|--------|--------|----------|--------|
| `async` fonksiyon | ❌ | ❌ | 💡 Tasarım | ✅ |
| `await` operatör | ❌ | ❌ | 💡 Tasarım | ✅ |
| `spawn` | ❌ | ❌ | ✅ Planlı | ✅ |
| Senkron FFI | ✅ | ✅ | ✅ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/ASYNC_AWAIT.md](../belgeler/language/feature_cards/ASYNC_AWAIT.md)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)
