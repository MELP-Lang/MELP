# MELP Ayrılmış Anahtar Kelimeler (Reserved Keywords) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ⚠️ Kısmi — Bazı kelimeler gereksiz ayrılmış (G-18)

---

## İçindekiler

1. [Kontrol Akışı](#kontrol-akışı)
2. [Tip Tanımlamaları](#tip-tanımlamaları)
3. [Yapısal Tanımlar](#yapısal-tanımlar)
4. [Lambda ve HOF](#lambda-ve-hof)
5. [Sabitler](#sabitler)
6. [Blok Sonlandırıcılar](#blok-sonlandırıcılar)
7. [Durum Tablosu](#durum-tablosu)

---

## Kontrol Akışı

| Anahtar Kelime | Açıklama |
|----------------|----------|
| `if` / `then` / `end if` | Koşul bloğu |
| `else` / `else if` | Koşul dalları |
| `loop` / `end loop` | Koşullu döngü |
| `for` / `to` / `end for` | For sayaç döngüsü |
| `for each` / `end for` | Foreach döngüsü |
| `match` / `case` / `end match` | Pattern matching |
| `return` | Fonksiyondan dön |
| `exit` | Döngüden çık (break) |
| `continue` / `continue_while` / `continue_for` | Döngü devam et |

---

## Tip Tanımlamaları

| Anahtar Kelime | Açıklama |
|----------------|----------|
| `numeric` | 64-bit işaretli tamsayı |
| `string` | String referans tipi |
| `boolean` | Mantıksal tip (true/false) |
| `character` | Karakter tipi |
| `auto` | Otomatik tip çıkarımı (kısmi destek) |
| `void` | Dönüş tipi yok (iç kullanım) |

---

## Yapısal Tanımlar

| Anahtar Kelime | Açıklama |
|----------------|----------|
| `function` / `end function` | Fonksiyon bloğu |
| `struct` / `end struct` | Yapı tipi |
| `enum` / `end enum` | Sayım tipi |
| `import` | Modül yükleme |
| `module` | Modül tanımı |
| `as` | Dönüş tipi belirteci (yalnızca `external function` tanımında) |

---

## Lambda ve HOF

| Anahtar Kelime | Açıklama |
|----------------|----------|
| `lambda` / `end lambda` | Lambda ifadesi / bloğu |
| `->` | Lambda dönüş belirteci (inline form) |

---

## Sabitler

| Anahtar Kelime | Açıklama |
|----------------|----------|
| `true` | Boolean doğru |
| `false` | Boolean yanlış |
| `null` | Null değer (Nullable için) |

---

## Blok Sonlandırıcılar

MELP tüm blokları `end XXX` keyword'üyle kapatır — küme parantezi kullanmaz:

| Blok | Sonlandırıcı |
|------|-------------|
| `if` | `end if` |
| `loop` | `end loop` |
| `for` | `end for` |
| `function` | `end function` |
| `struct` | `end struct` |
| `enum` | `end enum` |
| `match` | `end match` |
| `lambda` | `end lambda` |
| `debug` | `end debug` |

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| Temel keyword'ler | ✅ | ✅ | ✅ |
| `auto` | ⚠️ Kısmi | ⚠️ | ✅ |
| `where` clause | ❌ | ❌ | ✅ |
| `defer` | ❌ | ❌ | ✅ |
| `async`/`await` | ❌ | ❌ | ✅ Stage 1.5 |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `while koşul ... end while` | `loop koşul ... end loop` |
| `end_if` | `end if` |
| `end_function` | `end function` |
| `function f() as numeric` | `numeric function f()` |
| `end_XXX` (alt çizgi ile) | `end XXX` (boşluk ile) |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/RESERVED_KEYWORDS.md](../belgeler/language/feature_cards/RESERVED_KEYWORDS.md)
- **pmlp Syntax:** [belgeler/pmlp0_sozdizimi.md](../belgeler/pmlp0_sozdizimi.md)
- **İlgili Bug:** `TODO_BUG_FIXES/TODO_BUG_FIXES.md#G-18`
