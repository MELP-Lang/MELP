# Görevli YZ_05 — Buradan Başla

**Rol:** Görevli YZ
**Numara:** YZ_05
**Proje:** MELP Stage 1 — examples/base Seviye 4
**Tarih:** 2026-03-03

---

## 🔴 ÖNCE BU KURALLARI OKU — HAYATI ÖNEM TAŞIYOR

### YASAK #1: STUB KOD YAZMA
```
-- ❌ YANLIŞ — stub, sahte, boş implementation:
function main()
    numeric result = 100   -- sabit değer döndürmek
    print(result)
end_function

-- ✅ DOĞRU — gerçek özelliği kullanan kod:
function main()
    numeric[] arr = [5; 3; 8; 1; 9; 2]
    -- gerçek sıralama algoritması yaz
    ...
end_function
```

### YASAK #2: WORKAROUND YAPMA (B1-B4 dışında)
Bir özellik derlenemiyorsa:
1. Gerçek kodu yaz
2. Derle
3. Hata alıyorsan → **LOG'a kaydet + ÜA'ya bildir, bekle**
4. Farklı bir şey yazarak "sanki çalışıyormuş gibi" gösterme

### YASAK #3: COMPILER'IN EKSİKLİKLERİNİ ÖNCEDEN VARSAYMA
Feature card "TODO" veya "PARTIAL" yazsa bile **gerçek kodu yaz ve dene**.
Belki çalışır, belki çalışmaz — test et, rapor et.

---

## ✅ DOĞRU İŞ AKIŞI

Her görev için:
```
1. Feature card'ı oku
2. Gerçek syntax ile .mlp yaz
3. Derle:
   melp_compiler input.mlp -o /tmp/out.ll
4a. Derleme başarılı → çalıştır:
    clang -O2 -x ir /tmp/out.ll -o /tmp/out.elf && /tmp/out.elf
4b. Derleme başarısız → LOG'a hata mesajını yaz, ÜA'ya bildir
5. Çıktıyı doğrula
6. LOG'a kaydet
```

---

## 🔧 ARAÇLAR

```bash
# Compiler:
/home/pardus/projeler/MLP/LLVM/stage0/v2/build/compiler/stage1/modules/orchestrator/melp_compiler

# Derle + çalıştır:
/home/pardus/projeler/MLP/LLVM/stage0/v2/build/compiler/stage1/modules/orchestrator/melp_compiler input.mlp -o /tmp/out.ll && clang -O2 -x ir /tmp/out.ll -o /tmp/out.elf && /tmp/out.elf
```

**Feature card dizini:** `/home/pardus/projeler/MLP/LLVM/stage0/v2/belgeler/language/feature_cards/`

---

## ⚠️ BİLİNEN KISITLAMALAR (sadece bunlar için workaround)

| ID | Bug | Workaround |
|----|-----|------------|
| B1 | struct string field | kullanma |
| B2 | `Token[] = [t1;t2]` | numeric array |
| B3 | struct return | etme |
| B4 | `this.a * this.b` / `str(p.x)` / nested member assign | local var; flat struct |
| B5 | `read_file()` olmayan dosya → segfault | test etme |

**B1-B5 dışında hiçbir şey için workaround YAPMA.**

---

## 📋 GÖREV LİSTESİ — Seviye 4: Gerçek Uygulamalar

---

### GÖREV 31 — fibonacci

**Dizin:** `examples/base/31_fibonacci/`
**Beklenen:** İteratif + rekürsif fibonacci, her ikisi de doğru sonuç vermeli
```
fibonacci(10) → 55
fibonacci(20) → 6765
```

---

### GÖREV 32 — fizzbuzz

**Dizin:** `examples/base/32_fizzbuzz/`
**Beklenen:** 1-20 arası FizzBuzz
```
1, 2, Fizz, 4, Buzz, Fizz, 7, ...
```

---

### GÖREV 33 — calculator

**Dizin:** `examples/base/33_calculator/`
**Beklenen:** +, -, *, / işlemleri yapan fonksiyonlar, çeşitli hesaplamalar

---

### GÖREV 34 — wordcount

**Dizin:** `examples/base/34_wordcount/`
**Feature Cards:** `FILE_IO.md`
**Beklenen:** String içindeki kelime sayısını say

---

### GÖREV 35 — todo_cli

**Dizin:** `examples/base/35_todo_cli/`
**Beklenen:** Struct tabanlı todo listesi, ekleme/listeleme

---

### GÖREV 36 — json_parser

**Dizin:** `examples/base/36_json_parser/`
**Beklenen:** Minimal JSON string parse (sayı, string değer çıkar)

---

### GÖREV 37 — http_client

**Dizin:** `examples/base/37_http_client/`
**Feature Cards:** `FFI.md`
**Beklenen:** `external function` ile C socket veya libcurl çağrısı

---

### GÖREV 38 — linked_list

**Dizin:** `examples/base/38_linked_list/`
**Beklenen:** Struct tabanlı linked list, push/traverse

---

### GÖREV 39 — binary_search

**Dizin:** `examples/base/39_binary_search/`
**Beklenen:** Sıralı dizide binary search, doğru index döndür

---

### GÖREV 40 — matrix_ops

**Dizin:** `examples/base/40_matrix_ops/`
**Beklenen:** 2x2 / 3x3 matris toplama veya çarpma

---

## 📊 LOG DOSYASI

`/home/pardus/projeler/MLP/LLVM/stage1/examples/base/YZ_05/YZ_05_LOG.md`

Her satır:
```
| 31 | fibonacci | ✅ | 55, 6765 |
| 32 | fizzbuzz  | ❌ | Parser error: ... |
```

---

## ✅ TAMAMLAMA KRİTERLERİ

- Her görev için gerçek kod yazıldı (stub yok)
- Her görev derlenip çalıştırıldı
- Çalışmayanlar LOG'a hata mesajıyla kaydedildi ve ÜA'ya bildirildi
- YZ_05_LOG.md tamamlandı

Tamamlandığında ÜA'ya teslim raporu gönder.
