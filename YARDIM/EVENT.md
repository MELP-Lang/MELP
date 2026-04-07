# EVENT / APP_START / PROGRAM — MELP Hızlı Referans

**Özellik:** GUI program yapısı — `event`, `app_start`, `program` blokları  
**Durum:** ✅ Parametresiz event çalışıyor | ❌ Parametreli event broken (EKS-016)  
**Feature Kartı:** `belgeler/language/feature_cards/EVENT_BLOCK.md`

---

## Temel Yapı

```mlp
program UygulamaAdi

  event olay_adi()
    -- olay kodu
  end event

  app_start()
    -- giriş noktası kodu
  end app

end program
```

`program` / `end program` — namespace wrapper (IR üretmez)  
`event` bloğu — `void` dönen LLVM fonksiyonuna derlenir  
`app_start` bloğu — `@main` entry point olarak derlenir

---

## Çalışan Örnek

```mlp
program TestApp
  event on_click()
    println("Tiklanma eventi")
  end event

  app_start()
    println("Uygulama basladi")
    on_click()
    println("Bitti")
  end app
end program
```

**Çıktı:**
```
Uygulama basladi
Tiklanma eventi
Bitti
```

---

## Birden Fazla Event

```mlp
program FormApp

  event on_open()
    println("Form acildi")
  end event

  event on_close()
    println("Form kapatildi")
  end event

  app_start()
    on_open()
    -- ... uygulama mantığı ...
    on_close()
  end app

end program
```

---

## Codegen Karşılığı

| MLP | LLVM IR |
|-----|---------|
| `event on_click()` | `define void @on_click() { ... }` |
| `app_start()` | `define i32 @main() { ... ret i32 0 }` |
| `program` / `end program` | IR yok (sadece parser scope) |

---

## Test Çıktıları (Doğrulanmış)

| Test | Açıklama | Sonuç |
|------|----------|-------|
| Parametresiz event tanımla + çağır | `event on_click()` içinde println | ✅ |
| `app_start` giriş noktası | `@main` olarak derleniyor | ✅ |
| program / end program wrapper | Derleme başarılı | ✅ |
| Parametreli event `event on_update(numeric val)` | **EKS-016** | ❌ |

---

## Sınırlamalar

| Özellik | Durum |
|---------|-------|
| `event isim()` (parametresiz) | ✅ Tam çalışıyor |
| `event isim(numeric val)` (parametreli) | ❌ EKS-016: `Expected ')' after event parameters` |
| `event` içinde `return 0` | ❌ Yanlış — event void döner, `return` veya boş bırak |
| `event` standalone (program dışı) | ❌ Sadece `program` içinde geçerli |

---

## Anti-Pattern'ler

```mlp
-- ❌ YANLIŞ: event dönüş tipi yok
event on_click() as numeric
  ...
end event

-- ✅ DOĞRU: dönüş tipi belirtilmez
event on_click()
  ...
end event

-- ❌ YANLIŞ: return değeri
event on_update()
  return 0      -- hata!
end event

-- ✅ DOĞRU: boş return veya yok
event on_update()
  println("guncellendi")
end event
```

---

## EKS Kaydı

**EKS-016** — Parametreli event syntax broken  
`event on_update(numeric val)` → `error: Expected ')' after event parameters`  
Etkilenen: `compiler/stage1/modules/parser/` event argüman parse aşaması  
Geçici çözüm: Parametreler yerine global değişken kullan

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_event` | `end event` |
| `end_program` | `end program` |
| `end_app` | `end app` |
| `event on_click() as numeric` | `event on_click()` |
| `return 0` (event içinde) | kaldır |

---

## İlgili YARDIM Dosyaları

- [FUNCTION.md](FUNCTION.md) — Normal fonksiyon tanımı (event alternatifi)
- [SCOPE.md](SCOPE.md) — İsimli scope blokları
- [KEYWORDS.md](KEYWORDS.md) — Tüm keyword listesi
