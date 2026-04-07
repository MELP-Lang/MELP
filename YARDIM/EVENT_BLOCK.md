# MELP Event Bloğu Referansı

**Son Güncelleme:** 15 Mart 2026
**Durum:** ✅ TAMAMLANDI — Phase 2 (L-01)

---

## Genel Bilgi

Event bloğu, olay tabanlı (event-driven) programlama için bir hook mekanizmasıdır.
`event` anahtar kelimesi ile olay dinleyicisi tanımlanır.

---

## Temel Sözdizimi

```mlp
-- Olay dinleyicisi tanımlama
event on_click()
    print("Butona tıklandı!")
end event

-- Parametre alan olay
event on_message(string mesaj)
    print("Mesaj: ")
    print(mesaj)
end event
```

---

## Uygulama Giriş Noktası

```mlp
-- app_start / end app bloğu
app_start
    print("Uygulama başladı")
    on_click()
end app
```

---

## Program Bloğu ile Birlikte

```mlp
-- program / end program (main() için syntactic sugar - planlanan)
program
    print("Merhaba")
end program

-- Eşdeğeri:
function main()
    print("Merhaba")
end function
```

---

## Tam Örnek

```mlp
-- Olay tabanlı uygulama
event on_start()
    print("Uygulama başlatıldı")
end event

event on_click(string dugme_id)
    print("Tıklanan: ")
    print(dugme_id)
end event

event on_stop()
    print("Uygulama durduruluyor")
end event

app_start
    on_start()
    on_click("btn_onayla")
    on_stop()
end app
-- Çıktı:
-- Uygulama başlatıldı
-- Tıklanan: btn_onayla
-- Uygulama durduruluyor
```

---

## Kurallar

| Kural | Açıklama |
|-------|---------|
| `event ... end event` | Blok zorunlu terminatörü |
| Parametreler | `;` ile ayrılır (her parametre için) |
| Dönüş tipi | event bloğu dönüş tipi almaz |
| Çağırma | Normal fonksiyon gibi çağrılır |
| Tekrar tetikleme | Aynı event birden fazla çağrılabilir |

---

## Normal Fonksiyon ile Fark

| | `function` | `event` |
|-|-----------|---------|
| Tanım | `function ad()` | `event ad()` |
| Bitiş | `end function` | `end event` |
| Amaç | Genel amaçlı | Olay dinleyicisi |
| Çağırma | Her yerden | Her yerden (şu an) |

---

## Hata Tablosu

| Yanlış | Doğru |
|--------|-------|
| `end_event` | `end event` |
| `end_function` | `end function` |
| `end_program` | `end program` |
| `event on_click() as void` | `event on_click()` |

---

## İlgili Özellikler

- **FUNCTION** — Normal fonksiyon tanımlama
- **ASYNC_AWAIT** — Asenkron programlama

**Ayrıca bakınız:**
- [FUNCTION.md](FUNCTION.md)
- [ASYNC_AWAIT.md](ASYNC_AWAIT.md)
- [belgeler/language/feature_cards/EVENT_BLOCK.md](../belgeler/language/feature_cards/EVENT_BLOCK.md)

---

## Durum Tablosu

| Özellik | Stage 0 | Stage 1 | Stage 2 |
|---------|---------|---------|---------|
| `event ... end event` | ✅ | ✅ | ✅ |
| `app_start / end app` | ✅ | ✅ | ✅ |
| Parametre desteği | ✅ | ✅ | ✅ |
| Event propagation | ❌ | ❌ | ✅ Planlı |
| Olay kuyruğu | ❌ | ❌ | ✅ Planlı |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/EVENT_BLOCK.md](../belgeler/language/feature_cards/EVENT_BLOCK.md)
- **Tamamlanma:** L-01 (8 Mart 2026)
