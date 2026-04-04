# MELP switch/case Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 TODO — Phase 2'de planlandı (`switch`/`end switch` token'ları henüz yok)

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
4. [switch vs match Farkı](#switch-vs-match-farkı)
5. [Geçici Alternatif](#geçici-alternatif)
6. [İlgili Özellikler](#ilgili-özellikler)
7. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
-- Hedef syntax (Phase 2 sonrası):
switch ifade
    case değer1: eylem()
    case değer2: eylem()
    default: varsayılan_eylem()
end switch
```

> **⚠️ UYARI:** `switch`/`end switch`/`default` token'ları henüz lexer'da tanımlı değil.  
> Geçici çözüm için [Geçici Alternatif](#geçici-alternatif) bölümüne bakınız.

---

## Kullanım Örnekleri

> Aşağıdaki örnekler hedef syntax'ı göstermektedir. Phase 2 sonrası çalışacaktır.

### Örnek 1: Sayı dallanması
```mlp
-- Hedef (Phase 2 sonrası):
switch x
    case 1: print("bir")
    case 2: print("iki")
    case 3: print("üç")
    default: print("diğer")
end switch
```

### Örnek 2: String dallanması
```mlp
-- Hedef (Phase 2 sonrası):
switch day
    case "Cumartesi": rest()
    case "Pazar": rest()
    default: work()
end switch
```

### Örnek 3: Çok satırlı case
```mlp
-- Hedef (Phase 2 sonrası):
switch status
    case "ok":
        log("başarılı")
        count = count + 1
    case "err":
        log("hata")
        count = count - 1
    default:
        log("bilinmiyor")
end switch
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler (Phase 2 sonrası)
- `numeric` ve `string` değerlerle çalışır
- `default` zorunlu değil ama önerilir
- Çok satırlı case gövdesi desteklenir
- Her case otomatik durar (fall-through yok)

### ❌ Yasaklar
- **Fall-through yok** — C tarzı `case 1: case 2:` zincirleme yok
- Float değer ile switch yapılamaz
- Aralık (`case 1..5:`) — `match` kullan
- `break` anahtar kelimesi gerekmez (ve kullanılamaz)

---

## switch vs match Farkı

| Özellik | `switch` | `match` |
|---------|----------|---------|
| Kullanım | Sabit değer karşılaştırması | Pattern (enum, struct) |
| Tip | numeric, string | Option{T}, Result{T;E}, enum |
| Guard clause | ❌ | ✅ (`when` koşulu) |
| Destructuring | ❌ | ✅ |
| Fall-through | ❌ | ❌ |

```mlp
-- Basit değer → switch tercih edilir (Phase 2 sonrası)
switch color
    case "red": handle_red()
    default: handle_other()
end switch

-- Pattern / enum → match şu an tek seçenek
match opt_val
    case Some(v): use(v)
    case None: skip()
end match
```

---

## Geçici Alternatif

Phase 2 tamamlanana kadar `if/else if/else/end if` kullanın:

```mlp
-- Şu an çalışan form:
if x == 1 then
    print("bir")
else
    if x == 2 then
        print("iki")
    else
        if x == 3 then
            print("üç")
        else
            print("diğer")
        end if
    end if
end if
```

---

## İlgili Özellikler

- **PATTERN_MATCHING** — `match/case/end match` (şu an kullanılabilir)
- **IF_STATEMENT** — if/else if/else geçici alternatif
- **ELSE_IF** — else if zinciri

**Ayrıca bakınız:**
- [PATTERN_MATCHING.md](PATTERN_MATCHING.md)
- [IF_STATEMENT.md](IF_STATEMENT.md)
- [ELSE_IF.md](ELSE_IF.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `switch/end switch` | 🔴 TODO | ⏳ Phase 2 | ✅ |
| `default` kolu | 🔴 TODO | ⏳ Phase 2 | ✅ |
| String switch | 🔴 TODO | ⏳ Phase 2 | ✅ |
| `match` (alternatif) | ✅ | ✅ | ✅ |
| if/else if (alternatif) | ✅ | ✅ | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_switch` | `end switch` |
| `switch x ... break` (fall-through) | `break` gerekmez, her case otomatik durur |
| `end_if` | `end if` |
| Şu an `switch` kullanmak | `match` veya `if/else if/else` kullan |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/SWITCH_CASE.md](../belgeler/language/feature_cards/SWITCH_CASE.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/switch_case/](../GOLDEN_TEST_SUITE/switch_case/)
- **pmlp Syntax:** [belgeler/pmlp0_sozdizimi.md](../belgeler/pmlp0_sozdizimi.md)
