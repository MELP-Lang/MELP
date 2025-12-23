# 🎯 GÖREVLİ YZ BAŞLANGIÇ NOKTASI

**Son Güncelleme:** 23 Aralık 2025  
**Durum:** 🟡 YZ_02 Göreve Hazır!  
**Önceki YZ:** YZ_01 (File I/O entegre edildi, println bloker tespit edildi)  
**Sen:** selfhosting_YZ_02

---

## 🚨 MASTERMIND KARARI

```
┌─────────────────────────────────────────────────────────────────┐
│  SYMBOLIC BOOTSTRAP YETERLİ DEĞİL!                              │
│                                                                 │
│  ✅ Gerçek dosya I/O ŞART                                       │
│  ✅ mlp_read_file() ile .mlp dosyası okunacak                   │
│  ✅ mlp_write_file() ile .ll dosyası yazılacak                  │
│  ✅ Gen1 → Gen2 → Gen3 gerçek convergence testi                 │
│                                                                 │
│  ❌ Hardcoded string'ler kabul edilmez                          │
│  ❌ Simüle edilmiş I/O kabul edilmez                            │
└─────────────────────────────────────────────────────────────────┘
```

---

## 📍 GÜNCEL DURUM

**YZ_01 Tamamlandı! ✅ Ama...**

- ✅ `read_source_file()` → `mlp_read_file()` entegre edildi
- ✅ `write_output_file()` → `mlp_write_file()` entegre edildi
- ✅ `compile_source()` → Gerçek pipeline eklendi (lexer/parser/codegen)
- ✅ `modules/compiler_minimal.mlp` oluşturuldu (235 satır)
- 🔴 **BLOKER:** Stage 0 `println` desteklemiyor → Derlenemiyor!

**Kritik Sorun:** 
```
Error: Generic template 'println' not found
```

## 🎯 YZ_02 GÖREVİ

**Phase 2, Task 2.1: println Blokerını Çöz ve İlk Derleme**

### Ne yapacaksın:

1. **println Sorununu Çöz** (2 seçenek)
   
   **Seçenek A: Silent Compiler (ÖNERİLEN)**
   - `modules/compiler_minimal.mlp`'den tüm `println`'leri kaldır
   - Debug output yok, sadece file I/O ve return codes
   - Hızlı ve kesin çözüm
   
   **Seçenek B: Extern Printf**
   ```mlp
   extern "C" function printf(string format) returns numeric
   ```
   - Stage 0'da extern syntax test et
   - `test_extern_printf.mlp` dosyasını referans al

2. **Syntax Düzeltmeleri**
   - `!=` yerine `== ... else` kullan
   - `end_while` ve `exit_while` kontrol et (underscore var mı?)
   - Nested if'leri basitleştir

3. **İlk Derleme**
   ```bash
   # Silent compiler'ı derle
   cd compiler/stage0/modules/functions
   ./functions_compiler ../../../../modules/compiler_minimal.mlp -o ../../../../build/compiler_gen1.ll
   
   # Başarı kontrolü
   ls -lh ../../../../build/compiler_gen1.ll
   ```

4. **Test**
   ```bash
   # Basit test
## 📋 OKUMAN GEREKENLER

| Dosya | İçerik |
|-------|--------|
| `YZ_reports/YZ_01_CALISMA_RAPORU.md` | **ÖNCE BUNU OKU!** YZ_01'in yaptıkları ve bloker |
| `TODO_STAGE1_TO_SELFHOSTING_FINAL.md` | Ana TODO |
| `modules/compiler_minimal.mlp` | Düzeltilecek dosya (235 satır) |
| `compiler/stage0/test_extern_printf.mlp` | Extern örneği |
| `pmlp_kesin_sozdizimi.md` | PMPL syntax referansı |
- [ ] `compiler_gen1.ll` oluştu
- [ ] Test dosyası başarıyla derlendi
- [ ] Gen1 çıktısı çalıştırılabilir
- [ ] Lexer pipeline bağlandı
- [ ] Parser pipeline bağlandı  
- [ ] CodeGen pipeline bağlandı
- [ ] Basit bir .mlp dosyası derlenebiliyor
## 📁 ÖNEMLİ DOSYALAR

```
YZ_reports/
└── YZ_01_CALISMA_RAPORU.md  ← **ÖNCELİKLE OKU!**

modules/
├── compiler_minimal.mlp      ← DÜZELT (println kaldır)
├── compiler.mlp             ← File I/O hazır (ama println var)
└── compiler_integration.mlp  ← 1112 satır full pipeline

compiler/stage0/
├── test_extern_printf.mlp   ← Extern örneği
└── test_while_simple.mlp    ← Syntax örneği

build/
└── compiler_gen1.ll         ← Burası oluşacak!
```📁 ÖNEMLİ DOSYALAR

## 🚀 BAŞLA!

```bash
git checkout -b selfhosting_YZ_02

# 1. ÖNCE YZ_01 raporunu oku
cat YZ_reports/YZ_01_CALISMA_RAPORU.md

# 2. println'leri kaldır
# modules/compiler_minimal.mlp'yi düzelt

# 3. Derle
cd compiler/stage0/modules/functions
## ⚠️ YASAKLAR

| YASAK | NEDEN |
|-------|-------|
| Stage 0'a dokunma | Stage 0 KAPALI - println eklenemez! |
| Yeni TODO yazma | TEK TODO var |
| TODO dışı iş yapma | Scope creep yasak |
| Symbolic/mock çözüm | Mastermind kararı: Gerçek I/O şart |

## 🆘 YZ_01'DEN ÖĞRENILENLER

✅ **Doğru Yaklaşım:**
- `mlp_read_file()` / `mlp_write_file()` direkt kullan
- `end_while`, `exit_while` (underscore ile)
- Basit tutun, nested yapılar sorun çıkarıyor

❌ **Yanlış Yaklaşım:**
- `println` kullanma → Stage 0 desteklemiyor
- `!=` kullanma → `==` + `else` kullan
- `read_file()` wrapper yok → `mlp_read_file()` kullan

# 6. Bu dosyayı YZ_03 için güncelle
``` └── statements.mlp       ← statement parsing
└── codegen_mlp/
    ├── ir_generator.mlp     ← LLVM IR üretimi
    └── ir_types.mlp         ← Tip dönüşümleri
```

---

## 🚀 BAŞLA!

```bash
git checkout -b selfhosting_YZ_01
# Görevi yap
# Test et
# Rapor yaz: YZ_reports/YZ_01_TAMAMLANDI.md
# Bu dosyayı YZ_02 için güncelle
git push origin selfhosting_YZ_01
```

---

## ⚠️ YASAKLAR

| YASAK | NEDEN |
|-------|-------|
| Stage 0'a dokunma | Stage 0 KAPALI |
| Yeni TODO yazma | TEK TODO var |
| TODO dışı iş yapma | Scope creep yasak |
| Symbolic/mock çözüm | Mastermind kararı: Gerçek I/O şart |

---

## 🎯 HEDEF: GERÇEK SELF-HOSTING

```
┌─────────────────────────────────────────────────────────────┐
│                                                             │
│   lli gen1.ll compiler.mlp > gen2.ll                        │
│   diff gen1.ll gen2.ll  # BOŞ ÇIKTI = BAŞARI!              │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

**Takıldığın yer olursa sor, yoksa direkt başla!** 🚀
