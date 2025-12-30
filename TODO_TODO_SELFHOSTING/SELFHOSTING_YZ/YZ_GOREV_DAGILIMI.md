# 📊 TODO_SELFHOSTING - YZ GÖREV DAĞILIMI

**Tarih:** 29 Aralık 2025  
**Proje:** MLP-GCC Self-Hosting  
**Durum:** 🟢 Göreve Hazır  

---

## 🎯 BÜYÜK RESİM

```
┌─────────────────────────────────────────────────────────┐
│              MLP-GCC SELF-HOSTING PIPELINE              │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  YZ_01: CODEGEN ASM→C      → [P0 + P1] 6-12 saat       │
│           ↓                                             │
│  YZ_02: BOOTSTRAP TEST     → [P2] 2-3 saat             │
│           ↓                                             │
│  YZ_03: STAGE1 + SELF-HOST → [P3 + P4] 3-5 saat        │
│           ↓                                             │
│      🎉 SELF-HOSTING BAŞARILI!                          │
│                                                         │
└─────────────────────────────────────────────────────────┘

TOPLAM: 11-20 saat (2-3 iş günü yoğun çalışma)
```

---

## 🤖 YZ GÖREV DAĞILIMI

### 🔴 YZ_01: CODEGEN ASM→C (KRİTİK!)

**Dosya:** [YZ_01_Gorev_Plani.md](YZ_01_Gorev_Plani.md)

**Görevler:**
- ✅ **P0:** Proje yapısını doğrula (2-4 saat)
  - Modülleri say (69 C, 17 MLP)
  - GCC derleme testi
  - Codegen analizi (hangi dosyalar değişecek?)

- 🔴 **P1:** Codegen ASM→C dönüşümü (4-8 saat)
  - `emit_c()` fonksiyonu yaz
  - Print → C printf
  - Variables → C declarations (STO mapping)
  - Control flow → C if/while/for
  - Functions → C functions
  - Expressions → C operators

**Süre:** 6-12 saat  
**Zorluk:** ⭐⭐⭐ Zor  
**Kritiklik:** 🔴 En Yüksek (Proje bunun üzerine kurulu!)

**Teslim Edilebilirlik:**
```bash
./mlpc test.mlp -o test.c  # C kodu üretiyor
gcc test.c -o test          # GCC başarıyla derliyor
./test                      # Program çalışıyor
```

---

### 🟡 YZ_02: BOOTSTRAP TEST

**Dosya:** [YZ_02_Gorev_Plani.md](YZ_02_Gorev_Plani.md)

**Bağımlılık:** YZ_01 bitmiş olmalı

**Görevler:**
- 🟡 **P2:** Bootstrap Test (2-3 saat)
  - Stage0-C derleyiciyi derle (`make`)
  - Hello World testi
  - Değişken testi
  - Control flow testi
  - Function testi
  - Test suite çalıştır

**Süre:** 2-3 saat  
**Zorluk:** ⭐⭐ Orta  
**Kritiklik:** 🟡 Orta (Test ve doğrulama)

**Teslim Edilebilirlik:**
```bash
cd MELP/C/stage0
make                       # Derleme başarılı
./mlpc examples/hello.mlp  # MLP derliyor
./tests/run_all.sh         # Tüm testler geçiyor
```

---

### 🟢 YZ_03: STAGE1 + SELF-HOSTING

**Dosya:** [YZ_03_Gorev_Plani.md](YZ_03_Gorev_Plani.md)

**Bağımlılık:** YZ_02 bitmiş olmalı

**Görevler:**
- 🟢 **P3:** Stage1 Derleme (3-4 saat)
  - 17 MLP modülünü Stage0 ile derle
  - Stage1 binary oluştur (mlpc_stage1)
  - Stage1 test et

- 🎉 **P4:** Self-Hosting (2-3 saat)
  - Stage1 kendini derlesin → Stage2
  - Stage2 doğrula (Stage1 ile aynı çıktı mı?)
  - Bootstrap zinciri test (convergence)
  - **🎉 SELF-HOSTING İLAN ET!**

**Süre:** 3-5 saat  
**Zorluk:** ⭐⭐ Orta  
**Kritiklik:** 🟢 Yüksek (Final adım!)

**Teslim Edilebilirlik:**
```bash
./mlpc_stage1 modules/lexer.mlp -o lexer.c  # Stage1 kendi kodunu derliyor
gcc lexer.c ... -o mlpc_stage2              # Stage2 oluşuyor
./mlpc_stage2 --version                     # Çalışıyor!
# 🎉 SELF-HOSTING BAŞARILI!
```

---

## 📊 ZAMAN ÇİZELGESİ

| Gün | YZ | Phase | Görev | Süre | Durum |
|-----|-----|-------|-------|------|-------|
| **Gün 1** | YZ_01 | P0 | Proje analizi | 2-4h | ⏳ |
| **Gün 1-2** | YZ_01 | P1 | Codegen ASM→C | 4-8h | ⏳ |
| **Gün 2** | YZ_02 | P2 | Bootstrap test | 2-3h | ⏸️ |
| **Gün 2-3** | YZ_03 | P3 | Stage1 derleme | 3-4h | ⏸️ |
| **Gün 3** | YZ_03 | P4 | Self-hosting! | 2-3h | ⏸️ |

**TOPLAM:** 13-22 saat (2-3 iş günü)

---

## 🔄 İŞ AKIŞI

### YZ_01 İş Akışı

1. TODO_KURALLARI.md + TODO_SELFHOSTING.md oku
2. Kendini tanıt
3. P0: Proje analizi (ne var, ne değişecek?)
4. P1.1: emit_c() fonksiyonu yaz
5. P1.2-P1.6: Codegen dönüşümleri (print, var, control, func, expr)
6. Test: MLP→C→binary çalışıyor mu?
7. Commit + YZ_RAPOR doldur
8. Üst Akıl'a bildir: "P1 tamamlandı"

### YZ_02 İş Akışı

1. YZ_01'in bittiğini doğrula
2. Stage0-C derle
3. Test suite çalıştır (hello, var, control, func)
4. Sorun varsa → YZ_01'e geri bildir
5. Tüm testler geçerse → YZ_RAPOR doldur
6. Üst Akıl'a bildir: "P2 tamamlandı"

### YZ_03 İş Akışı

1. YZ_02'nin bittiğini doğrula
2. 17 MLP modülünü Stage0 ile derle
3. Stage1 binary oluştur
4. Stage1 kendi kodunu derle → Stage2
5. Stage2 doğrula (convergence test)
6. 🎉 SELF-HOSTING İLAN ET!
7. Mastermind'a bildir

---

## 📋 KRİTİK NOKTALAR

### YZ_01 için:

- **Modüler kal:** Her dosya max 500 satır
- **Test et:** Her değişiklikten sonra derle
- **STO mantığı:** Type mapping doğru olmalı
- **Takıldığında:** 2 saat → Üst Akıl'a sor

### YZ_02 için:

- **Kapsamlı test:** Sadece hello world yetmez
- **Hata ayıklama:** Test başarısız → detaylı analiz
- **Dokümante et:** Hangi testler geçti/geçmedi

### YZ_03 için:

- **Sabırlı ol:** 17 modül derlemesi zaman alır
- **Convergence:** Stage2 == Stage3 olmalı
- **Kutla!** Self-hosting büyük başarı!

---

## 🚨 ÜST AKIL MÜDAHALE NOKTALARI

| Durum | YZ | Aksiyon |
|-------|-----|---------|
| YZ_01 2 saat takıldı | YZ_01 | Üst Akıl teknik rehberlik |
| P1 test başarısız | YZ_01 | Üst Akıl analiz + yeni YZ ata |
| P2 testler geçmiyor | YZ_02 | Üst Akıl YZ_01'e geri bildir |
| P3 modül derleme hatası | YZ_03 | Üst Akıl → YZ_01 codegen bug |
| Convergence sorunu | YZ_03 | Mastermind'a escalate (stateless ihlali?) |

---

## 🎯 BAŞARI KRİTERİ (TEKRAR)

**Proje başarılı sayılır:**

```bash
# Stage1 MLP derleyicisi kendi kaynak kodunu derliyor:
./mlpc_stage1 MELP/MLP/stage1/modules/lexer.mlp -o lexer.c

# Stage2 oluşuyor:
gcc *.c -o mlpc_stage2

# Stage2 çalışıyor:
./mlpc_stage2 --version
# MLP-GCC Stage2 Self-Hosted Compiler v1.0

# 🎉 SELF-HOSTING BAŞARILI!
```

---

## 📚 REFERANSLAR

- [TODO_SELFHOSTING.md](../../TODO_SELFHOSTING.md) - Ana TODO
- [Danışman_Yol_Haritası.md](../../Danışman_Yol_Haritası.md) - 5 Core Principles
- [YZ_01_Gorev_Plani.md](YZ_01_Gorev_Plani.md) - YZ_01 detayları
- [YZ_02_Gorev_Plani.md](YZ_02_Gorev_Plani.md) - YZ_02 detayları
- [YZ_03_Gorev_Plani.md](YZ_03_Gorev_Plani.md) - YZ_03 detayları

---

**HER YZ KENDİ GÖREV DOSYASINI OKUSUN VE BAŞLASIN!**

**İYİ ÇALIŞMALAR! SELF-HOSTING GELİYOR! 🚀🎉**

---

*Üst Akıl: SELFHOSTING_UA_01*  
*Tarih: 29 Aralık 2025*
