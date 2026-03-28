# EVENT_BLOCK — Özellik Kartı

**Özellik:** `event` / `end event` blok — GUI olay işleyici tanımı
**Durum:** ✅ COMPLETE (Phase 2 — L-01, 25 Şubat 2026)  
**Sözdizimi Sürümü:** mlp1 / Stage1  
**Kanonik Kaynak:** `BEKLEYEN_SORUNLAR.md` → L-01, `TODO_COMPLETE_FINAL.md` → Phase 2 L-01  
**Golden Test Dizini:** `GOLDEN_TEST_SUITE/event_block/`

---

## 🎯 ÖZELLİK

`event` bloğu GUI programlarında olay işleyici fonksiyon tanımlar.  
Derleyici bunu `void` dönüşlü LLVM IR fonksiyonuna çevirir.  
`app_start` bloğu ise programın giriş noktasıdır — `@main` olarak emit edilir.

---

## 📖 SÖZDİZİMİ

```mlp
-- GUI program yapısı
program FormUygulamasi

  event on_btn_click()
    print("Butona tıklandı")
  end event

  event on_close()
    -- temizleme kodu
  end event

  app_start()
    -- başlangıç kodu (→ @main)
    print("Uygulama başladı")
  end app

end program
```

---

## 🔑 ANAHTAR KELİMELER

| Keyword | Token Adı | Açıklama |
|---|---|---|
| `event` | `TOKEN_EVENT` | Olay bloku başlangıcı |
| `end event` | `TOKEN_END_EVENT` | Olay bloku sonu |
| `app_start` | `TOKEN_APP_START` | Giriş noktası başlangıcı → `@main` |
| `end app` | `TOKEN_END_APP` | Giriş bloğu sonu |
| `program` | `TOKEN_PROGRAM` | Program bildirimi başlangıcı |
| `end program` | `TOKEN_END_PROGRAM` | Program bildirimi sonu |

---

## 🔧 ETKİLENEN DOSYALAR

| Dosya | Değişiklik |
|---|---|
| `compiler/stage0/lexer/token.hpp` | `TOKEN_EVENT`, `TOKEN_END_EVENT`, `TOKEN_APP_START`, `TOKEN_END_APP`, `TOKEN_PROGRAM`, `TOKEN_END_PROGRAM` enum değerleri ekle |
| `compiler/stage0/lexer/lexer.cpp` | `identifyKeyword()` içine yukarıdaki keyword'ler için case'ler ekle |
| `compiler/stage1/modules/parser/` | `program` bildirimi + `event` blok kuralı ekle |
| `compiler/stage1/modules/orchestrator/src/codegen_callable.cpp` | `event` bloku → `void` dönüşlü fonksiyon IR; `app_start` → `@main` entry point |

---

## 🔄 CODEGEN KARŞILIĞI

```
-- MLP kaynak          → LLVM IR
event on_btn_click()   → define void @on_btn_click() { ... }
end event

app_start()            → define i32 @main() { ... ret i32 0 }
end app
```

- `event` bloğu dönüş tipi her zaman `void`; `ret void` ile biter.
- `app_start` bloğu `@main` olarak emit edilir, dönüş `i32 0`.
- `program` / `end program` wrapper — sadece namespace gibi davranır, IR oluşturmaz.

---

## 🚫 ANTI-PATTERN'LER

| Yanlış ❌ | Doğru ✅ | Açıklama |
|---|---|---|
| `event on_click() -> numeric` | `event on_click()` | `event` bloklarının dönüş tipi yok — void |
| `return 0` inside event | `return` veya yok | `event` içinde `return 0` yazılamaz |
| `event` standalone (program dışı) | `program ... event ... end program` | `event` ancak `program` içinde geçerlidir |

---

## 🧪 GOLDEN TESTLER

**Dizin:** `GOLDEN_TEST_SUITE/event_block/`  
**Minimum:** 5 test  
**Not:** Bu testlerin çalışması için `event` + `program` + `app_start` üçü birlikte implement edilmeli.

| Dosya | Test | Beklenen Çıktı |
|---|---|---|
| `test_01_simple_event.mlp` | Boş `event` bloğu derlenebiliyor | (derleme başarısı, çıktı yok) |
| `test_02_event_with_print.mlp` | `event` içinde `print()` → doğru void IR | `Olay tetiklendi` |
| `test_03_app_start_runs.mlp` | `app_start` → `@main` entry point olarak çalışıyor | `Merhaba GUI` |
| `test_04_multiple_events.mlp` | Birden fazla `event` bloğu aynı `program` içinde | Hata yok |
| `test_05_event_no_return_type.mlp` | `event` dönüş tipi belirtilmez → void | Derleme başarılı |

### Test Formatı

```mlp
-- test_03_app_start_runs.mlp
-- FEATURE: EVENT_BLOCK
-- PHASE: Phase2-L01
-- EXPECTED: Merhaba GUI

program TestApp

  app_start()
    print("Merhaba GUI")
  end app

end program
```

---

## ✅ TAMAMLANMA KOŞULLARI

1. `token.hpp`'de tüm 6 token enum değeri tanımlı
2. `identifyKeyword()` tüm keyword'leri doğru token'a çeviriyor
3. `event <ad>() ... end event` → void fonksiyon IR — `lli` ile çalışıyor
4. `app_start() ... end app` → `@main` — ELF binary olarak çalışıyor
5. `GOLDEN_TEST_SUITE/event_block/test_01..05.mlp` — 5/5 geçiyor
6. Regresyon: `bash /tmp/run_golden_baseline.sh` → en az 359 (artmış olabilir)
7. `VSIX_ICIN_YAPILACAKLAR.md` → L-01 ✅

---

## 📎 REFERANSLAR

- `BEKLEYEN_SORUNLAR.md` satır 65, 85–115
- `VSIX_ICIN_YAPILACAKLAR.md` → L-01
- `TODO_COMPLETE_FINAL.md` → Phase 2 L-01, S-10, S-10b, S-10c
