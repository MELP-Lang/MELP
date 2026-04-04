# MELP str() Heap Implementasyon Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ COMPLETE — Phase 2 (STR-HEAP), 25 Şubat 2026

---

## Genel Bilgi

`str()` fonksiyonunun heap tabanlı implementasyonu. Phase 1'de `alloca` (stack) kullanılıyordu; bu fonksiyon sınırları geçildiğinde **dangling pointer** oluşturuyordu. Phase 2'de `malloc` (heap) ile düzeltildi.

---

## Sorun (Phase 1 — Stack Impl.)

```mlp
-- Phase 1'de BU CRASH'E NEDEN OLUYORDU:
string function sayi_al()
    return str(999)      -- stack buffer → fonksiyon döndüğünde geçersiz
end function

function main()
    print(sayi_al())     -- dangling pointer → undefined behavior / crash
end function
```

**LLVM IR problemi (Phase 1):**
```llvm
%buf = alloca [32 x i8]    ; ← stack allocation
%ptr = getelementptr ...
call i32 @snprintf(...)
ret i8* %ptr               ; ← fonksiyon döndüğünde geçersiz!
```

---

## Düzeltme (Phase 2 — Heap Impl.)

Phase 2'den itibaren `str()` heap'te (`malloc`) alan ayırır:

```mlp
-- Artık GÜVENLİ çalışır:
string function sayi_al()
    return str(999)      -- heap'te, scope bağımsız
end function

function main()
    print(sayi_al())     -- ✅ güvenli
end function
```

---

## str() Kullanım Kuralları

```mlp
-- numeric → string:
numeric n = 42
string s = str(n)         -- "42"
print(s)                  -- 42

-- Birleştirmede:
string mesaj = "Sayı: " + str(42)    -- "Sayı: 42"

-- Zincirleme:
numeric sonuc = 100
print("Sonuç: " + str(sonuc))
```

---

## Golden Tests

Phase 2 STR-HEAP golden testleri: `GOLDEN_TEST_SUITE/str_heap/` (7/7 geçiyor)

```mlp
-- test_01: Fonksiyon sınırı aşımı
string function get()
    return str(42)
end function

function main()
    string s = get()
    if s == "42" then print("ok") end if
end function
```

---

## İlgili Özellikler

- **STR_BUILTIN** — `str()` sözdizimsel tanım (kullanım kuralları)

**Ayrıca bakınız:**
- [STR_BUILTIN.md](STR_BUILTIN.md)

---

## Durum Tablosu

| Özellik | Phase 1 (Eski) | Phase 2+ (Şimdi) |
|---------|---------------|-----------------|
| `str(n)` tek scope | ✅ | ✅ |
| Fonksiyon sınırı geçme | 🔴 Crash | ✅ Güvenli |
| `return str(n)` | 🔴 Dangling | ✅ Güvenli |
| Golden test 7/7 | ❌ | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `function f() as string` | `string function f()` |
| `end_function` | `end function` |
| `return 0` (main içinde) | kaldır |
| `return 1` (main içinde) | kaldır veya `print()` kullan |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/STR_HEAP.md](../belgeler/language/feature_cards/STR_HEAP.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/str_heap/](../GOLDEN_TEST_SUITE/str_heap/)
- **Bağlantılı:** [STR_BUILTIN.md](STR_BUILTIN.md)
