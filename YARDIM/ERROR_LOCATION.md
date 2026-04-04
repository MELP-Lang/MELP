# MELP Hata Konumu (Error Location) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ COMPLETE — Phase 1 (P-03), 25 Şubat 2026 doğrulandı

---

## Genel Bilgi

MELP derleyicisi hata mesajlarında `dosya.mlp:satır:sütun: ❌ mesaj` formatını kullanır.  
Bu format geliştirici deneyimini iyileştirir — hatanın tam yerini gösterir.

---

## Hata Mesajı Formatı

```
<dosya_yolu>:<satır>:<sütun>: ❌ <mesaj>
```

### Örnekler
```
program.mlp:3:1: ❌ Undefined variable: sayi
hesap.mlp:10:5: ❌ Undefined function: topla
main.mlp:7:3: ❌ Type mismatch: expected numeric, got string
```

### Sütun bilinmiyorsa
```
program.mlp:3:0: ❌ Undefined variable: sayi
```
(sütun `0` → fallback kabul edilir)

---

## Yaygın Hata Mesajları

| Hata | Örnek Çıktı |
|------|-------------|
| Tanımsız değişken | `main.mlp:5:3: ❌ Undefined variable: x` |
| Tanımsız fonksiyon | `main.mlp:10:5: ❌ Undefined function: foo` |
| Tip uyuşmazlığı | `main.mlp:7:1: ❌ Type mismatch: expected numeric, got string` |
| Eksik return | `main.mlp:15:1: ❌ Missing return in function: hesapla` |

---

## Derleyici Exit Kodları

| Durum | Exit Kodu |
|-------|-----------|
| Başarılı derleme | `0` |
| Sözdizimi hatası | `1` |
| Tip hatası | `1` |
| Çalışma zamanı hatası | `1` |

```bash
melp_compiler program.mlp
echo $?   # 0 = başarılı, 1 = hata
```

---

## İlgili Özellikler

- **ERROR_HANDLING** — Çalışma zamanı hata yönetimi
- **ERROR_PROPAGATION** — Hata iletimi

**Ayrıca bakınız:**
- [ERROR_HANDLING.md](ERROR_HANDLING.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `dosya:satır:sütun:` formatı | ✅ | ✅ | ✅ |
| Satır numarası | ✅ | ✅ | ✅ |
| Sütun numarası | ⚠️ (0 fallback) | ✅ | ✅ |
| Exit kodu 1 hata'da | ✅ | ✅ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/ERROR_LOCATION.md](../belgeler/language/feature_cards/ERROR_LOCATION.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/error_location/](../GOLDEN_TEST_SUITE/error_location/)
