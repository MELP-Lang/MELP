# MELP Dış Fonksiyon Arayüzü (FFI) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ `external function` ile çalışıyor — Stage 1

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Tip Eşlemeleri](#tip-eşlemeleri)
3. [Kullanım Örnekleri](#kullanım-örnekleri)
4. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
5. [İlgili Özellikler](#ilgili-özellikler)
6. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
-- C fonksiyonu declare et
external function fonksiyon_adi(tip parametre) as donus_tipi

-- Sonra normal gibi çağır
numeric sonuc = fonksiyon_adi(arguman)
```

`external function` bildirimi LLVM IR'da `declare` üretir; linker C sembolünü çözümler.

---

## Tip Eşlemeleri

| MELP Tipi | C Tipi |
|-----------|--------|
| `numeric` | `int64_t` / `long` |
| `string` | `const char*` |
| `boolean` | `int` (0/1) |
| `ptr` | `void*` / `i8*` |

---

## Kullanım Örnekleri

### Örnek 1: strlen
```mlp
external function strlen(string s) as numeric

function main()
    numeric uzunluk = strlen("merhaba")
    print(uzunluk)   -- 7
end function
```

### Örnek 2: puts
```mlp
external function puts(string s) as numeric

function main()
    puts("Merhaba Dunya")
end function
```

### Örnek 3: atoi (string → numeric)
```mlp
external function atoi(string s) as numeric

function main()
    numeric sayi = atoi("42")
    print(sayi)   -- 42
end function
```

### Örnek 4: Matematik fonksiyonları (libm)
```mlp
external function sqrt(numeric x) as numeric
external function abs(numeric x) as numeric

function main()
    numeric kok = sqrt(16)
    print(kok)   -- 4
end function
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- `extern "C"` ile dışa açılmış C fonksiyonları
- C standart kütüphanesi (`libc`, `libm`)
- Statik link edilmiş kütüphaneler

### ❌ Çalışmayan (Planlanmış)
- `DynamicLibrary.open()` syntax'ı — implement edilmemiş
- Variadic fonksiyonlar (`printf`, `scanf`) — sınırlı destek
- C++ isim karmaşası — sadece `extern "C"` fonksiyonlar çalışır

### Derleme Sırasında
```bash
# -lm ile math kütüphanesi linkle:
clang -O0 -x ir prog.ll build/mlp_runtime.bc -lm -o prog
```

---

## İlgili Özellikler

- **EXTERNAL** — Daha geniş FFI referansı
- **MODULES** — Modül import/export

**Ayrıca bakınız:**
- [EXTERNAL.md](EXTERNAL.md)
- [MODULES.md](MODULES.md)

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `external function f(s as string)` | `external function f(string s)` |
| `end_function` | `end function` |
| `return 0` (main içinde) | kaldır |

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `external function` bildirimi | ✅ | ✅ | ✅ |
| C stdlib fonksiyonları | ✅ | ✅ | ✅ |
| libm fonksiyonları | ✅ | ✅ | ✅ |
| `DynamicLibrary.open()` | ❌ | ❌ | ✅ |
| Variadic (`printf`) | ❌ | ❌ | ⚠️ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/FFI.md](../belgeler/language/feature_cards/FFI.md)
- **Implementation:** `compiler/stage1/modules/ffi/ffi_codegen.cpp`
- **User Guide:** [belgeler/FFI_USER_GUIDE.md](../belgeler/FFI_USER_GUIDE.md)
