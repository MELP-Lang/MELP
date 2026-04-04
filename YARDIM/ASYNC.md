# ASYNC / SPAWN / CHANNEL — MELP Hızlı Referans

**Özellik:** Eşzamanlılık — thread spawn, channel mesajlaşma, async/await  
**Durum:**  
✅ `spawn/end spawn` — thread başlatma
✅ `numeric h = spawn ... end spawn` — handle atama (EKS-017 çözüldü)  
✅ `mlp_spawn_join(h)` — thread bitmesini bekleme  
✅ `channel_create/send/receive` — güvenli kanal iletişimi  
✅ `async function` + `await` — pass-through  

**Runtime:** `-lpthread` gereklidir

---

## Temel: spawn / end spawn

```mlp
function main()
    spawn
        println("thread calisiyor")
    end spawn
    println("main devam")
end function
```

**Uyarı:** `return 0` thread bitmeden process'i sonlandırabilir.  
Güvenli çalışma için `channel` kullanın (receive bloklayıcıdır).

---

## Spawn Handle Atama ve Join

✅ **Durum: TAM** (EKS-017, 8 Mart 2026)

`spawn` bloğu bir `numeric` değer (thread handle) döndürür. Bu handle `mlp_spawn_join()` ile thread bitmesini beklemek için kullanılabilir.

```mlp
function main()
    numeric h = spawn
        print("thread bitti")
    end spawn
    mlp_spawn_join(h)   -- thread bitene kadar bekle
end function
```

### Birden fazla thread join:
```mlp
function main()
    numeric h1 = spawn
        print("thread1")
    end spawn
    numeric h2 = spawn
        print("thread2")
    end spawn
    mlp_spawn_join(h1)
    mlp_spawn_join(h2)
end function
```

**Önemli:** Sonuç sırası garantisiz (thread scheduling'e bağlı). Belirli bir sıra gerekseyse channel kullan.

---

## Channel ile Thread İletişimi (Tavsiye Edilen)

```mlp
function main()
    numeric ch = channel_create()

    spawn
        send(ch; 42)
        send(ch; 99)
    end spawn

    numeric a = receive(ch)
    numeric b = receive(ch)
    println("a=" + str(a) + " b=" + str(b))   -- a=42 b=99
end function
```

- `channel_create()` — thread-safe bounded FIFO kuyruk (kapasite: 64)
- `send(ch; deger)` — channel'a yaz (dolu ise bekler)
- `receive(ch)` — channel'dan oku (**bloklayıcı** — veri gelene kadar bekler)
- Thread senkronizasyonu için en güvenli yöntem budur

---

## async function + await

```mlp
async numeric function hesapla(numeric x)
    return x * x
end function

function main()
    numeric sonuc = await hesapla(7)
    println("sonuc: " + str(sonuc))   -- 49
end function
```

**Önemli:** `await` şu an **synchronous pass-through** — bloklamaz, anında sonucu döndürür.  
Gerçek async yürütme henüz implement edilmemiş.

---

## Closure Yakalama (spawn içinde)

```mlp
function main()
    numeric ch = channel_create()
    numeric katsayi = 3

    spawn
        -- katsayi dış scope'tan yakalanır (by-value)
        send(ch; katsayi * 10)
    end spawn

    numeric sonuc = receive(ch)
    println(str(sonuc))   -- 30
end function
```

---

## Derleme Notu

```bash
# -lpthread zorunlu!
clang -O0 output.ll -lm -lpthread -o program
```

---

## Test Çıktıları (Doğrulanmış)

| Test | Açıklama | Sonuç |
|------|----------|-------|
| `spawn ... end spawn` | Thread başlatma | ✅ |
| `numeric h = spawn...end spawn` | Handle atama | ✅ |
| `mlp_spawn_join(h)` | Thread bekleme | ✅ |
| `channel_create()` + `send/receive` | İki thread arası veri | ✅ a=42 b=99 |
| `async function` + `await` | Synchronous çalışma | ✅ 7²=49 |
| `receive(ch)` bloklama | Main waits for thread | ✅ |

---

## Sınırlamalar

| Özellik | Durum |
|---------|-------|
| `spawn ... end spawn` | ✅ Thread başlatıyor |
| `numeric h = spawn...end spawn` | ✅ Handle atama (EKS-017) |
| `mlp_spawn_join(handle)` | ✅ Thread bekleme |
| `channel_create/send/receive` | ✅ Thread-safe, bloklayıcı |
| `async function` + `await` | ✅ Synchronous pass-through (async IO değil) |
| Kanal kapasitesi | 64 eleman (sabit) |
| Channel boyutu dinamik değiştirme | ❌ Yok |
| `select` (çoklu kanal bekleme) | ❌ Planlandı |

---

## EKS Kaydı

**EKS-017** — ✅ Çözüldü (8 Mart 2026)  
`numeric h = spawn ... end spawn` artık çalışıyor.  
Parser'da `spawn` ifade olarak tanınıyor; codegen thread handle'i i64 olarak döndürüyor.  
Handle ile `mlp_spawn_join(h)` çağrılabilir. 5/5 test geçiyor.

---

## Anti-Pattern'ler

```mlp
-- ❌ YANLIŞ: join olmadan println bekleme
spawn
    println("thread")
end spawn
return 0   -- process bitiyor, thread yazdıramıyor

-- ✅ DOĞRU: channel ile senkronize ol
numeric ch = channel_create()
spawn
    println("thread")
    send(ch; 1)
end spawn
receive(ch)  -- thread bitene kadar bekle
return 0
```

---

## İlgili YARDIM Dosyaları

- [FUNCTION.md](FUNCTION.md) — Normal fonksiyon tanımı
- [KEYWORDS.md](KEYWORDS.md) — Tüm keyword listesi
