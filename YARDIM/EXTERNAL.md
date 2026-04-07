# EXTERNAL — MELP Hızlı Referans

**Özellik:** C/sistem kütüphane fonksiyonu bildirimi (`external function`)  
**Durum:** ✅ Tam çalışıyor  
**Bağlı Kart:** `belgeler/FFI_USER_GUIDE.md`

---

## Temel Sözdizimi

```mlp
external function c_fonksiyon_adi(tip param) as donus_tipi   -- as sözdizimi external için geçerli
```

`external` bildirimi derleyiciye "bu fonksiyon dış kütüphanede, bağlayıcı çözecek" der.  
Parametre sayısı istediğiniz kadar olabilir (`;` ile ayır).

---

## Örnekler

### C standard library

```mlp
-- libc fonksiyonları
external function puts(string s) as numeric
external function abs(numeric x) as numeric
external function strlen(string s) as numeric

function main()
    puts("Merhaba C stdlib")
    numeric n = abs(0 - 42)
    println("abs(-42)=" + str(n))     -- 42
    numeric len = strlen("merhaba")
    println("uzunluk=" + str(len))    -- 7
end function
```

### libm matematik fonksiyonları

```mlp
external function sqrt(numeric x) as numeric
external function pow(numeric base; numeric exp) as numeric
external function floor(numeric x) as numeric

function main()
    numeric kok = sqrt(16)
    println(str(kok))    -- 4
end function
```

**Derleme notu:** Matematik fonksiyonları için `-lm` bayrağı gereklidir:
```bash
clang -O0 output.ll -lm -o program
```

---

## Tip Eşleştirme

| MLP tipi | C tipi (LLVM ABI) |
|----------|-------------------|
| `numeric` | `int64_t` / `double` |
| `string` | `char*` (i8*) |
| Dönüş yok | `void` → Declare etme, sadece çağır |

---

## Test Çıktıları (Doğrulanmış)

| Test | Çağrı | Beklenen | Sonuç |
|------|-------|----------|-------|
| libc puts | `puts("Merhaba C stdlib")` | stdout çıktı | ✅ |
| libc abs | `abs(0 - 42)` | 42 | ✅ |

---

## Anti-Pattern'ler

```mlp
-- ❌ YANLIŞ: Dönüş tipi void için from yapma
external function printf(string fmt) as void  -- "void" tip yok

-- ✅ DOĞRU: void dönenler ignore edilir
external function printf(string fmt) as numeric  -- geçici çözüm

-- ❌ YANLIŞ: Variadic fonksiyonlar
external function printf(string fmt; ...) as numeric  -- desteklenmiyor

-- ✅ DOĞRU: Sabit parametreli fonksiyonlar
external function puts(string s) as numeric
```

---

## Uygulama Kılavuzu

1. `external function` satırı dosyanın en üstüne yaz (fonksiyon gövdelerinden önce)
2. Parametre isimleri opsiyonel ama tip zorunlu
3. Bağlantı zamanında kütüphane linke edilmelidir (`-lm`, `-lpthread` vb.)

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_function` | `end function` |
| `return 0` (main içinde) | kaldır |
| `f(a, b)` (virgül) | `f(a; b)` |

---

## İlgili YARDIM Dosyaları

- [FUNCTION.md](FUNCTION.md) — Normal MLP fonksiyonu tanımı
- [KEYWORDS.md](KEYWORDS.md) — Tüm keyword listesi
