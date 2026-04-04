# CHANNELS - Özellik Kartı

**Özellik:** `Channel{T}` — Tip güvenli eşzamanlı mesajlaşma  
**Durum:** � TOKEN-ONLY (Phase 3: SEND/RECEIVE/END_SELECT token ✅, parser REJECT ✅, runtime: Phase 4+)  
**Hafta:** Stage 1.5  
**Sözdizimi Sürümü:** mlp2

---

## 🎯 ÖZELLİK

Go tarzı kanallar. `Channel{T}` tipi; `spawn` ile goroutine benzeri iş parçacığı başlatır. `send` / `receive` ile veri gönderir/alır. `select` birden fazla kanalı dinler.

---

## 📖 SÖZDİZİMİ

```mlp
-- Kanal oluşturma
Channel{numeric} ch = make_channel(10)   -- tampon boyutu 10

-- Gönderme
send(ch; 42)

-- Alma
numeric val = receive(ch)

-- spawn — yeni iş parçacığı
spawn void function worker(Channel{numeric} c)
    send(c; compute())
end function

-- select — ilk gelen kanaldan al
select
    case receive(ch1):
        handle_a()
    case receive(ch2):
        handle_b()
end select

-- for each ile kanal tüketimi
loop each msg in ch
    print(msg)
end loop
```

**Tip Kuralları:**
```mlp
Channel{T}               -- T tipi mesajlar için kanal
Channel{string}          -- string mesajları
Channel{Result{numeric; string}}  -- hata yayılımlı veri
```

**Mevcut Durum — YZ DİKKAT:**
```mlp
-- ⚠️ Stage 1.5'e kadar MEVCUT DEĞİL
-- Eş zamanlılık yoktur — seri çalışma
-- stdlib ffi üzerinden OS thread mümkün (ham, tip güvensiz)
```

---

## 🧪 GOLDEN TEST

```mlp
-- Stage 1.5 sonrası hedef test:
void function producer(Channel{numeric} ch)
    loop each i in 1..=5
        send(ch; i)
    end loop
end function

function main()
    Channel{numeric} ch = make_channel(5)
    spawn producer(ch)
    numeric total = 0
    loop each n in ch
        total = total + n
    end loop
    -- 1+2+3+4+5 = 15
    if total == 15 then return 1 end if
end function
```

---

**Bağımlılıklar:** spawn, select, for-each, Result{T;E}  
**Hafta:** Stage 1.5 (Aralık 2026)  
**Referans Diller:** Go channels, Rust `std::sync::mpsc`, Clojure core.async
