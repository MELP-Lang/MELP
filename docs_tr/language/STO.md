# Akıllı Tip Optimizasyonu (STO)

**Versiyon:** 1.0  
**Son Güncelleme:** 12 Aralık 2025  
**Durum:** Tasarım Belgesi

---

## Genel Bakış

STO (Smart Type Optimization - Akıllı Tip Optimizasyonu), MELP'in yüksek performansı korurken basit bir kullanıcı deneyimi sağlama yaklaşımıdır. Kullanıcılar sadece iki temel tip görür (`numeric` ve `string`), ancak derleyici otomatik olarak optimal dahili temsilleri seçer.

---

## Felsefe

### Problem

Geleneksel diller kullanıcıları birçok sayısal tip arasında seçim yapmaya zorlar:
- `int8`, `int16`, `int32`, `int64`
- `float`, `double`
- `BigInteger`, `BigDecimal`

Bu durum bilişsel yük ve potansiyel hatalar (taşma, hassasiyet kaybı) yaratır.

### MELP'in Çözümü

**Kullanıcı perspektifi:** Sadece `numeric` ve `string` var.

**Derleyici perspektifi:** Otomatik olarak optimal depolamayı seçer:
- Küçük tamsayılar → `int64` (register)
- Ondalıklar → `double` (XMM register)
- Büyük sayılar → `BigDecimal` (heap)

---

## Tip Eşleştirme

### Numeric Dahili Temsiller

| Kullanıcı Yazar | Değer Aralığı | Dahili Tip | Depolama | Performans |
|-------------|-------------|---------------|---------|-------------|
| `numeric x = 42` | -2^63 ile 2^63-1 | `int64` | Register/Stack | ⚡ En Hızlı |
| `numeric y = 3.14` | ~15 basamak hassasiyet | `double` | XMM Register | ⚡ Hızlı |
| `numeric z = 10^100` | Sınırsız | `BigDecimal` | Heap | 🐢 Daha Yavaş |

### String Dahili Temsiller

| Kullanıcı Yazar | Uzunluk | Dahili Tip | Depolama | Performans |
|-------------|--------|---------------|---------|-------------|
| `string s = "Ali"` | ≤23 byte | SSO (satır içi) | Stack | ⚡ En Hızlı |
| `string t = "Uzun metin..."` | >23 byte | Heap pointer | Heap | 🔄 Normal |
| `string c = "Sabit"` | Herhangi (literal) | `.rodata` | Salt okunur | ⚡ Paylaşımlı |

**SSO = Small String Optimization (Küçük String Optimizasyonu):** Kısa stringler heap tahsisi olmadan doğrudan stack'te saklanır.

---

## Derleme Zamanı Analizi

### Numeric Tip Seçim Algoritması

```
function analyze_numeric(value):
    if value is_integer AND -2^63 ≤ value ≤ 2^63-1:
        return INT64          -- Register'da sakla
    else if value is_decimal AND digits ≤ 15:
        return DOUBLE         -- XMM register'da sakla
    else:
        return BIGDECIMAL     -- Heap'te sakla
```

### String Tip Seçim Algoritması

```
function analyze_string(value):
    if is_literal_constant(value):
        return RODATA_STRING  -- .rodata bölümü
    else if length(value) ≤ 23:
        return SSO_STRING     -- Stack satır içi
    else:
        return HEAP_STRING    -- Heap tahsisi
```

---

## Çalışma Zamanı Davranışı

### Otomatik Taşma İşleme

```mlp
numeric x = 9223372036854775807  -- Maksimum int64
x = x + 1                        -- TAŞMA!

-- Dahili olarak ne olur:
-- 1. Taşma tespit edilir
-- 2. x BigDecimal'e yükseltilir
-- 3. İşlem BigDecimal ile devam eder
-- 4. Kullanıcı doğru sonucu görür (çökme yok!)
```

### Tip Yükseltme Kuralları

| İşlem | Tipler | Sonuç |
|-----------|-------|--------|
| `int64 + int64` | Aynı | `int64` |
| `int64 + double` | Karışık | `double` |
| `int64 taşma` | - | `BigDecimal` |
| `double + BigDecimal` | Karışık | `BigDecimal` |

---

## Bellek Düzeni

```
┌─────────────────────────────────────────────────────────────┐
│                         STACK                                │
├─────────────────────────────────────────────────────────────┤
│  int64 değerleri (8 byte)                                   │
│  double değerleri (8 byte, hizalı)                          │
│  SSO stringler (≤24 byte, satır içi)                        │
│  Heap pointer'ları (8 byte, heap verisine işaret eder)      │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                          HEAP                                │
├─────────────────────────────────────────────────────────────┤
│  BigDecimal yapıları                                        │
│  Uzun string verisi (>23 byte)                              │
│  Dinamik diziler                                            │
│  Struct örnekleri                                           │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                     .RODATA (Salt okunur)                    │
├─────────────────────────────────────────────────────────────┤
│  String literalleri                                         │
│  Sabit numeric değerler                                     │
└─────────────────────────────────────────────────────────────┘
```

---

## Kod Üretim Örnekleri

### Kullanıcı Kodu

```mlp
numeric kucuk = 42
numeric ondalik = 3.14159
numeric devasa = 10 ^ 1000

string kisa = "Ali"
string uzun = read_file("kitap.txt")
```

### Üretilen Assembly (Kavramsal)

```asm
; kucuk = 42 → int64, register'da saklanır
mov rax, 42
mov [rbp-8], rax

; ondalik = 3.14159 → double, XMM'de saklanır
movsd xmm0, [.LC0]
movsd [rbp-16], xmm0

; devasa = 10^1000 → BigDecimal, heap'te tahsis edilir
call bigdec_pow
mov [rbp-24], rax      ; heap pointer

; kisa = "Ali" → SSO, stack'te satır içi
mov qword [rbp-48], "Ali\0"  ; 24-byte alan, satır içi

; uzun = read_file(...) → heap string
call read_file
mov [rbp-56], rax      ; heap pointer
```

---

## Avantajlar

1. **Kullanıcı Basitliği:** Sadece `numeric` ve `string` - tip karışıklığı yok
2. **Otomatik Performans:** Küçük değerler = hızlı, büyük değerler = güvenli
3. **Bellek Verimliliği:** Gereksiz heap tahsisi yok
4. **Geriye Uyumlu:** Mevcut MELP kodu değişmeden çalışır
5. **Şeffaf:** Kullanıcıların optimizasyon hakkında bilgi sahibi olması gerekmez

---

## Uygulama Durumu

### Faz 1: Temel STO (Devam Ediyor)

**Numeric:**
- [ ] int64 desteği (küçük tamsayılar)
- [ ] double desteği (ondalıklar)
- [ ] BigDecimal yedekleme (büyük/hassas)
- [ ] Taşma tespiti ve otomatik yükseltme

**String:**
- [ ] SSO uygulaması (≤23 byte satır içi)
- [ ] Heap string (>23 byte)
- [ ] Sabit string → .rodata

**Kod Üretimi:**
- [ ] Tip farkında register tahsisi
- [ ] Optimize edilmiş assembly kalıpları

### Faz 2: Gelişmiş STO (Gelecek)

- [ ] Copy-on-write stringler
- [ ] String interning
- [ ] Daha küçük değerler için int32 kullanımı
- [ ] SIMD optimizasyonları

---

## Dikkat Edilmesi Gerekenler

### Aritmetik İşlemler

| İşlem | Not |
|-----------|------|
| `int64 + double` | Sonuç `double` |
| Taşma | Otomatik olarak `BigDecimal`'e yükselt |
| Bölme | `int64`'ten `double` üretebilir |

### String İşlemleri

| İşlem | Not |
|-----------|------|
| Birleştirme | SSO + SSO heap'e dönüşebilir |
| Alt string | Orijinale referans verebilir |
| Karşılaştırma | Tüm dahili tiplerle çalışır |

---

## AI Agent'lar İçin

- STO, self-hosting'den ÖNCE uygulanmalıdır
- C runtime temel fonksiyonları sağlar
- Kod üretimi tip farkında assembly üretir
- Test: Her tip kombinasyonu test edilmelidir
- Kullanıcı API'si ASLA DEĞİŞMEZ - sadece backend optimize eder

---

*Bu belge MELP'in dahili optimizasyon stratejisini açıklar. Kullanıcıların bu detayları bilmesi beklenmez.*
