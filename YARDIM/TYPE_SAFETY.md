# MELP Tip Güvenliği (Type Safety) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 KRİTİK BUG — EKS-002

---

## Genel Bilgi

MELP statik tipli bir dildir; değişkenler tanımlandıktan sonra tip değiştirilmemelidir.  
Ancak **EKS-002** hatası nedeniyle bazı tip ihlalleri sessizce geçiyor.

---

## Bilinen Hatalar

### Bug 1: Tip Uyumsuzluğu Sessizce Geçiyor

```mlp
-- BEKLENEN: Derleme hatası
-- GERÇEK: Sessizce derleniyor (EKS-002)
function main()
    numeric x = 10
    x = "merhaba"    -- BUG: numeric değişkene string atanıyor!
    print(x)
end function
```

**Sorun:** `x = "merhaba"` atanıyor, `x` tipi `numeric` olmasına rağmen hata verilmiyor.

### Bug 2: Bilinmeyen Anahtar Kelime Blokları Sessizce Yoksayılıyor

```mlp
-- BEKLENEN: Derleme hatası
-- GERÇEK: Blok yoksayılıyor, program çalışıyor
function main()
    gecersiz_anahtar_kelime x = 10
    print("Merhaba")
end function
-- Çıktı: Merhaba (hata yok!)
```

**Sorun:** Bilinmeyen token ile başlayan ifade sessizce atlanıyor.

---

## Doğru Kullanım (Şu An Çalışan)

```mlp
-- DOĞRU: Tip tutarlılığı
function main()
    numeric sayi = 42
    string metin = "Merhaba"
    boolean durum = true
    
    -- Tip uyumlu atamalar
    sayi = 100          -- numeric = numeric ✅
    metin = "Selam"     -- string = string ✅
    durum = false       -- boolean = boolean ✅
    
    print(sayi)
    print(metin)
end function
```

---

## Güvenli Programlama Pratiği (EKS-002 Geçici Çözüm)

EKS-002 düzeltilinceye kadar şu pratiği uygulayın:

```mlp
-- 1. Her değişkeni açıkça tiplendir
-- 2. Aynı değişkene farklı tipte atama YAPMA
-- 3. String/numeric dönüşümü için str() kullan

function main()
    numeric yas = 25
    string mesaj = "Yaş: "
    
    -- Tip karıştırma yapma:
    -- yas = "yirmibeş"   -- YANLIŞ: EKS-002 şu an buna izin veriyor ama tehlikeli!
    
    -- Doğru dönüşüm:
    print(mesaj)
    print(yas)
end function
```

---

## Tür Uyumluluk Tablosu (Planlanan)

| Kaynak Tip | Hedef Tip | Durum |
|-----------|-----------|-------|
| `numeric` | `numeric` | ✅ Her zaman uyumlu |
| `string` | `string` | ✅ Her zaman uyumlu |
| `boolean` | `boolean` | ✅ Her zaman uyumlu |
| `numeric` | `string` | ❌ Hata vermeli (EKS-002: vermiyor) |
| `string` | `numeric` | ❌ Hata vermeli (EKS-002: vermiyor) |
| `boolean` | `numeric` | ❌ Hata vermeli (EKS-002: vermiyor) |

---

## EKS-002 Düzeltme Planı

- **Faz:** Phase 1 bug fix
- **Etkilenen Modül:** `codegen_vars.cpp`, `semantic_analyzer.cpp`
- **Çözüm:** Atama ifadelerinde sağ taraf tipini kontrol et

---

## İlgili Özellikler

- **TYPES** — Temel tip sistemi
- **TYPE_ALIAS** — Tip tanımlama
- **UNION_TYPE** — Çoklu tip

**Ayrıca bakınız:**
- [TYPES.md](TYPES.md)
- [TYPE_ALIAS.md](TYPE_ALIAS.md)
- [belgeler/language/feature_cards/TYPE_SAFETY.md](../belgeler/language/feature_cards/TYPE_SAFETY.md)

---

## Durum Tablosu

| Kontrol | Stage 0 | Stage 1 | Fix Sonrası |
|---------|---------|---------|-------------|
| `numeric = string` hatası | ❌ BUG | ❌ BUG | ✅ Planlı |
| Bilinmeyen token hatası | ❌ BUG | ❌ BUG | ✅ Planlı |
| Tip uyumu fonksiyon çağrısında | ⚠️ Kısmi | ⚠️ Kısmi | ✅ Planlı |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `numeric x = "metin"` (tip ihlali — EKS-002) | tip uyumlu ata |
| `print("x=" + n)` (sayıyı direkt birleştirme) | `print("x=" + str(n))` |
| `end_function` | `end function` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/TYPE_SAFETY.md](../belgeler/language/feature_cards/TYPE_SAFETY.md)
- **Bug Tracker:** EKS-002
