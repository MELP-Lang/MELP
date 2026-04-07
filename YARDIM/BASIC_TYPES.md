# MELP Temel Tipler (Basic Types) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Stage 0'da tam olarak çalışıyor

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Tip Listesi](#tip-listesi)
3. [Kullanım Örnekleri](#kullanım-örnekleri)
4. [Tip Dönüşümü](#tip-dönüşümü)
5. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
6. [Yasaklar](#yasaklar)
7. [İlgili Özellikler](#ilgili-özellikler)
8. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
numeric x = 42
string s = "merhaba"
boolean flag = true
```

---

## Tip Listesi

| Tip | Açıklama | Örnek |
|-----|----------|-------|
| `numeric` | Tüm sayılar (tam + ondalık) | `numeric x = 42` |
| `string` | Metin değerleri | `string s = "merhaba"` |
| `boolean` | Mantıksal değer | `boolean b = true` |
| `void` | Fonksiyon dönüş tipi (değer yok) | `function log()` |

---

## Kullanım Örnekleri

### Örnek 1: numeric — tam sayı
```mlp
numeric x = 42
numeric neg = -7
numeric big = 1000000
print(x)
-- Çıktı: 42
```

### Örnek 2: numeric — ondalık (Türkçe format)
```mlp
numeric pi = 3,14
numeric rate = 0,05
print(pi)
-- Çıktı: 3.14
```

> ⚠️ **Türkçe Format:** Ondalık ayırıcı olarak nokta (`.`) değil, virgül (`,`) kullanılır!

### Örnek 3: string — tek ve çift tırnak
```mlp
string s1 = "merhaba"
string s2 = 'dünya'
string empty = ""
print(s1)
-- Çıktı: merhaba
```

### Örnek 4: boolean
```mlp
boolean flag = true
boolean done = false
if flag then
    print("aktif")
end if
-- Çıktı: aktif
```

### Örnek 5: void fonksiyon
```mlp
function log(string msg)
    print(msg)
end function

log("test")
-- Çıktı: test
```

---

## Tip Dönüşümü

```mlp
-- numeric → string
string s = str(42)          -- "42"

-- string → numeric
numeric n = to_number("42") -- 42

-- numeric → boolean
boolean b = to_boolean(1)   -- true
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- MELP'te tüm sayılar `numeric` tipindedir — ayrı `int`/`float` yoktur
- Türkçe ondalık format: virgül (`,`) ondalık ayırıcı olarak kullanılır
- `string` hem tek (`'`) hem çift (`"`) tırnak kabul eder
- Açık tip bildirimi zorunludur: `numeric x = 42`

### ❌ Yasaklar
- `var x = 42` veya `let x = 42` yoktur — her zaman tip belirt
- İngilizce ondalık `3.14` yerine `3,14` kullan
- `int`, `float`, `double`, `char` tipleri yoktur — yalnızca `numeric`, `string`, `boolean`

---

## İlgili Özellikler

- **ASSIGNMENT** — Değişken atama
- **VARIABLE** — Değişken bildirim kuralları
- **STR_BUILTIN** — `str()` ile tip dönüşümü
- **OPERATORS** — Aritmetik işlemler

**Ayrıca bakınız:**
- [VARIABLE.md](VARIABLE.md)
- [ASSIGNMENT.md](ASSIGNMENT.md)
- [STR_BUILTIN.md](STR_BUILTIN.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `numeric` | ✅ | ✅ | ✅ |
| `string` | ✅ | ✅ | ✅ |
| `boolean` | ✅ | ✅ | ✅ |
| `void` | ✅ | ✅ | ✅ |
| Türkçe ondalık (`,`) | ✅ | ✅ | ✅ |
| `null` / nullable | ⏳ | ✅ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/BASIC_TYPES.md](../belgeler/language/feature_cards/BASIC_TYPES.md)
- **pmlp Syntax:** [belgeler/pmlp0_sozdizimi.md](../belgeler/pmlp0_sozdizimi.md)
