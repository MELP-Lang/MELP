# 🎯 GÖREVLİ YZ BAŞLANGIÇ NOKTASI

**Son Güncelleme:** 24 Aralık 2025  
**Durum:** 🟢 YZ_09 Göreve Hazır!  
**Önceki YZ:** YZ_08 (Syntax Analizi - 77+ hata tespit edildi)  
**Sen:** selfhosting_YZ_09

---

## ✅ YZ_08 BAŞARISI!

```
┌─────────────────────────────────────────────────────────────┐
│  SYNTAX ANALİZİ TAMAMLANDI!                                 │
│                                                             │
│  Tespit edilen hatalar:                                    │
│  → Virgüllü parametre: 19 dosya                            │
│  → while...do: 7 dosya                                     │
│  → Array literal virgül: 51 dosya                          │
│  → Toplam: 77+ dosyada syntax hatası                       │
│                                                             │
│  Düzeltilen dosyalar:                                      │
│  ✅ functions_codegen.mlp                                   │
│  ✅ bootstrap_minimal.mlp (test: exit 230 ✅)              │
│  ✅ string_utils.mlp                                        │
│  ✅ math_utils.mlp                                          │
│                                                             │
│  İlk self-hosting test başarılı! 🎉                        │
└─────────────────────────────────────────────────────────────┘
```

---

## 📍 ŞİMDİKİ DURUM

**Branch:** `TODO_STAGE1_TO_SELFHOSTING_FINAL`  
**İlerleme:** 5/13 task (%38)

**Aktif Görev:** Phase 1 Task 1.2 - Toplu Syntax Düzeltme

---

## 🎯 YZ_09 GÖREVİ

**Phase 1, Task 1.2: Toplu Syntax Düzeltme**

### ⚠️ DOĞRU SYNTAX (pmlp_kesin_sozdizimi.md'den):

```
Parametre: func(a; b; c)                -- noktalı virgül ayırıcı
Array:     numeric[] arr = [1; 2; 3;]   -- köşeli parantez + trailing ;
List:      list data = (1; "x"; true;)  -- normal parantez + trailing ;
While:     while cond ... end_while     -- "do" YOK!
If:        if cond then ... end_if      -- "then" ZORUNLU!
```

### YZ_08'den Kalan İş:

**Henüz düzeltilmedi:**
- ⏳ **15 dosya** virgüllü parametre (19'dan 4'ü düzeltildi)
- ⏳ **6 dosya** while...do (7'den 1'i düzeltildi)
- ⏳ **51 dosya** array literal virgül

### Yapılacaklar:

**1. Öncelikli Modüller (Lexer, Parser, Codegen):**
   - lexer_mlp/*.mlp
   - parser_mlp/*.mlp
   - codegen_mlp/*.mlp

**2. Her Dosya İçin:**
   ```bash
   # Düzelt
   # Test et
   ./compiler/stage0/modules/functions/functions_compiler dosya.mlp output.ll
   # Çalıştır (mümkünse)
   lli output.ll
   ```

**3. Hedef:**
   - %50+ modül derleniyor olmalı (~54 dosya)
   - Kritik modüller (lexer, parser, codegen) %100 düzeltilmiş

### Başarı Kriteri:

- [ ] 15+ dosya daha düzeltilmiş
- [ ] Lexer, parser, codegen modülleri syntax açısından temiz
- [ ] En az 5 dosya derlenip test edilmiş
- [ ] YZ_09_TAMAMLANDI.md raporu

---

## 📚 OKUMAN GEREKENLER

1. **selfhosting_YZ/YZ_08_TAMAMLANDI.md** - **ÖNCE BUNU OKU!**
2. **pmlp_kesin_sozdizimi.md** - Syntax referansı
3. **/tmp/syntax_report.txt** - YZ_08'in oluşturduğu hata listesi

---

## ⚠️ KURALLAR

- TODO'da ne yazıyorsa onu yap
- "Detaylandırmamı ister misin?" YASAK
- Phase/Task icat etme
- Raporu `selfhosting_YZ/YZ_09_TAMAMLANDI.md` olarak yaz

---

**🚀 TOPLU DÜZELTMEYİ BAŞLAT!** 🚀
