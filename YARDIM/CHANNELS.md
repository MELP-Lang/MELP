# MELP Kanal (Channel) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 📅 TOKEN-ONLY — Stage 1.5'te implement edilecek

---

## İçindekiler

1. [Genel Bilgi](#genel-bilgi)
2. [Hedef Syntax](#hedef-syntax)
3. [Mevcut Durum](#mevcut-durum)
4. [Kurallar](#kurallar)
5. [İlgili Özellikler](#ilgili-özellikler)
6. [Durum Tablosu](#durum-tablosu)

---

## Genel Bilgi

`Channel{T}` Go tarzı tip güvenli eşzamanlı mesajlaşma kanalı.  
`spawn` ile goroutine benzeri iş parçacığı başlatılır.  
`send`/`receive` ile veri gönderilir/alınır.

> **⚠️ UYARI:** `Channel{T}` Stage 1.5'e kadar mevcut değildir.  
> Token'lar parse edilir ama runtime yok. Şu an seri (senkron) çalışma kullanın.

---

## Hedef Syntax

```mlp
-- Kanal oluştur (tampon boyutu 10)
Channel{numeric} ch = make_channel(10)

-- Gönder
send(ch; 42)

-- Al
numeric val = receive(ch)

-- spawn ile yeni iş parçacığı
spawn function worker(Channel{numeric} c)
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

**Tip Örnekleri:**
```mlp
Channel{string}                          -- string mesajları
Channel{numeric}                         -- sayı mesajları
Channel{Result{numeric; string}}         -- hata yayılımlı veri
```

---

## Mevcut Durum

Stage 1.5'e kadar eşzamanlılık yoktur. Mevcut alternatifler:

```mlp
-- Seri çalışma (şu an)
function process_items(numeric[] items)
    loop each item in items
        process(item)
    end loop
end function

-- FFI ile düşük seviye pthread (ham, tip güvensiz)
external function pthread_create(...) as numeric
```

---

## Kurallar

### ✅ İzin Verilenler (Stage 1.5 sonrası)
- Tamponlu ve tamponsuz kanal
- Çoklu üretici/tüketici
- `select` ile çoklu kanal dinleme

### ❌ Yasaklar
- Kapalı kanala gönderme
- Nil kanaldan okuma

---

## İlgili Özellikler

- **ASYNC_AWAIT** — Asenkron fonksiyon tasarımı (Stage 1.5+)
- **RESULT_OPTION** — Hata yayılımlı kanal tipi

**Ayrıca bakınız:**
- [ASYNC_AWAIT.md](ASYNC_AWAIT.md)
- [RESULT_OPTION.md](RESULT_OPTION.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage1.5 | Stage2 |
|---------|--------|--------|----------|--------|
| `Channel{T}` tipi | ❌ | Token | ✅ | ✅ |
| `spawn` | ❌ | ❌ | ✅ | ✅ |
| `send`/`receive` | ❌ | ❌ | ✅ | ✅ |
| `select` | ❌ | ❌ | ✅ | ✅ |
| Buffered channel | ❌ | ❌ | ✅ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/CHANNELS.md](../belgeler/language/feature_cards/CHANNELS.md)
- **Referans:** Go channels, Rust mpsc
