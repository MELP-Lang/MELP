# CHANNELS - Özellik Kartı

**Özellik:** `Channel{T}` — Tip güvenli eşzamanlı mesajlaşma  
**Durum:** � TOKEN-ONLY (Phase 3: SEND/RECEIVE/END_SELECT token ✅, parser REJECT ✅, runtime: Phase 4+)  
**Hafta:** Stage 1.5  
**Sözdizimi Sürümü:** pmlp2

---

## 🎯 ÖZELLİK

Go tarzı kanallar. `Channel{T}` tipi; `spawn` ile goroutine benzeri iş parçacığı başlatır. `send` / `receive` ile veri gönderir/alır. `select` birden fazla kanalı dinler.

---

## 📖 SÖZDİZİMİ

```pmlp
-- Kanal oluşturma
Channel{numeric} ch = make_channel(10)   -- tampon boyutu 10

-- Gönderme
send(ch; 42)

-- Alma
numeric val = receive(ch)

-- spawn — yeni iş parçacığı
spawn function worker(Channel{numeric} c) as void
    send(c; compute())
end_function

-- select — ilk gelen kanaldan al
select
    case receive(ch1):
        handle_a()
    case receive(ch2):
        handle_b()
end_select

-- for each ile kanal tüketimi
for each msg in ch
    print(msg)
end_for
```

**Tip Kuralları:**
```pmlp
Channel{T}               -- T tipi mesajlar için kanal
Channel{string}          -- string mesajları
Channel{Result{numeric; string}}  -- hata yayılımlı veri
```

**Mevcut Durum — YZ DİKKAT:**
```pmlp
-- ⚠️ Stage 1.5'e kadar MEVCUT DEĞİL
-- Eş zamanlılık yoktur — seri çalışma
-- stdlib ffi üzerinden OS thread mümkün (ham, tip güvensiz)
```

---

## 🧪 GOLDEN TEST

```pmlp
-- Stage 1.5 sonrası hedef test:
function producer(Channel{numeric} ch) as void
    for each i in 1..=5
        send(ch; i)
    end_for
end_function

function main() as numeric
    Channel{numeric} ch = make_channel(5)
    spawn producer(ch)
    numeric total = 0
    for each n in ch
        total = total + n
    end_for
    -- 1+2+3+4+5 = 15
    if total == 15 then return 1 end_if
    return 0
end_function
```

---

**Bağımlılıklar:** spawn, select, for-each, Result{T;E}  
**Hafta:** Stage 1.5 (Aralık 2026)  
**Referans Diller:** Go channels, Rust `std::sync::mpsc`, Clojure core.async
