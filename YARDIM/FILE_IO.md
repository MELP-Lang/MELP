# MELP Dosya G/Ç (File I/O) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Stage 1'de tam çalışıyor

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [Hata Yönetimi](#hata-yönetimi)
4. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
5. [İlgili Özellikler](#ilgili-özellikler)
6. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
-- Dosya okuma:
string icerik = read_file("dosya.txt")

-- Dosya yazma:
numeric basari = write_file("dosya.txt"; icerik)

-- Dosyaya ekleme:
numeric basari = append_file("dosya.txt"; satir)
```

---

## Kullanım Örnekleri

### Örnek 1: Dosya okuma
```mlp
function main()
    string icerik = read_file("data.txt")
    print(icerik)
end function
```

### Örnek 2: Dosya yazma
```mlp
function main()
    string veri = "Merhaba Dünya!"
    numeric basari = write_file("cikti.txt"; veri)
    
    if basari == 1 then
        print("Yazma başarılı")
    else
        print("Yazma hatası")
    end if
end function
```

### Örnek 3: Dosyaya satır ekleme
```mlp
function main()
    string log = "Yeni kayıt\n"
    numeric basari = append_file("log.txt"; log)
    println(str(basari))   -- 1 = başarılı
end function
```

### Örnek 4: Oku-İşle-Yaz
```mlp
function main()
    string icerik = read_file("girdi.txt")
    
    if icerik == "" then
        print("Hata: Dosya okunamadı")
    end if
    
    string islenmis = icerik + "\nİşlendi!"
    numeric sonuc = write_file("cikti.txt"; islenmis)
    println(str(sonuc))   -- 1 = başarılı
end function
```

---

## Hata Yönetimi

| Fonksiyon | Başarı | Hata |
|-----------|--------|------|
| `read_file()` | Dosya içeriği (string) | Boş string `""` |
| `write_file()` | `1` | `0` |
| `append_file()` | `1` | `0` |

```mlp
-- Hata kontrolü:
string icerik = read_file("yok.txt")
if icerik == "" then
    print("Dosya bulunamadı")
end if

numeric r = write_file("cikti.txt"; "veri")
if r == 0 then
    print("Yazma başarısız")
end if
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- `read_file(dosya_yolu)` → string döndürür
- `write_file(dosya_yolu; icerik)` → numeric döndürür
- `append_file(dosya_yolu; icerik)` → numeric döndürür
- Göreceli veya mutlak dosya yolları
- İleri eğik çizgi `/` kullan: `"veri/girdi.txt"`

### ❌ Yasaklar
- İstisna atmaz — her zaman dönüş değerini kontrol et
- Dosya tanıtıcı (file handle) API'si yoktur — doğrudan okuma/yazma

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_function` | `end function` |
| `end_if` | `end if` |
| `function main() as numeric` | `function main()` |
| `return 0` (main içinde) | kaldır |

---

## İlgili Özellikler

- **STRING_CONCAT** — Dosya içeriği işleme
- **IF_STATEMENT** — Hata kontrolü
- **VARIABLE** — Dosya içeriğini saklama

**Ayrıca bakınız:**
- [STRING_CONCAT.md](STRING_CONCAT.md)
- [ERROR_HANDLING.md](ERROR_HANDLING.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `read_file()` | ❌ | ✅ | ✅ |
| `write_file()` | ❌ | ✅ | ✅ |
| `append_file()` | ❌ | ✅ | ✅ |
| Binary dosya G/Ç | ❌ | ❌ | ⏳ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/FILE_IO.md](../belgeler/language/feature_cards/FILE_IO.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/fs_runtime/](../GOLDEN_TEST_SUITE/fs_runtime/)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)
