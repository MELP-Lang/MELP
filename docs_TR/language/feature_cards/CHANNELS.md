# CHANNELS (Kanallar) — Ozellik Karti

**Ozellik:** `Channel{T}` — Tip guvenli eslerin mesajlasmasi  
**Durum:** 🟡 TOKEN-ONLY (SEND/RECEIVE token ✅, parser REJECT ✅, runtime: Stage 1.5+)  
**Sozdizimi Surumu:** mlp2

---

## 🎯 OZELLIK

Go tarzı kanallar. `Channel{T}` tipi; `spawn` ile goroutine benzeri is parcacigi baslatir. `send` / `receive` ile veri gonderir/alir. `select` birden fazla kanali dinler.

---

## 📖 SOZDIZIMI

```mlp
-- Kanal olusturma
Channel{numeric} ch = make_channel(10)   -- tampon boyutu 10

-- Gonderme
send(ch; 42)

-- Alma
numeric deger = receive(ch)

-- spawn — yeni is parcacigi
spawn void function isci(Channel{numeric} c)
    send(c; hesapla())
end function

-- select — ilk gelen kanaldan al
select
    case receive(ch1):
        isle_a()
    case receive(ch2):
        isle_b()
end select

-- for each ile kanal tuketimi
loop each mesaj in ch
    print(mesaj)
end loop
```

---

## 📌 Tip Kurallari

```mlp
Channel{T}                            -- T tipinde mesajlar icin kanal
Channel{string}                       -- string mesajlari
Channel{Result{numeric; string}}      -- hata yayilimli veri
```

---

## ⚠️ MEVCUT DURUM — YZ DİKKAT

```mlp
-- ⚠️ Stage 1.5'e kadar MEVCUT DEĞİL
-- Esleme yoktur — seri calisma
-- stdlib ffi uzerinden OS thread mumkun (ham, tip guvensiz)
```

---

## 🧪 GOLDEN TEST (Stage 1.5 Hedefi)

```mlp
void function uretici(Channel{numeric} ch)
    loop each i in 1..=5
        send(ch; i)
    end loop
end function

function main()
    Channel{numeric} ch = make_channel(5)
    spawn uretici(ch)
    numeric toplam = 0
    loop each n in ch
        toplam = toplam + n
    end loop
    -- 1+2+3+4+5 = 15
    if toplam == 15 then return 1 end if
end function
```

---

**Bagimliliklar:** spawn, select, for-each, Result{T;E}  
**Surec:** Stage 1.5 (Aralik 2026)  
**Referans Diller:** Go channels, Rust `std::sync::mpsc`, Clojure core.async
