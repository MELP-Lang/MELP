# 🎯 GÖREVLİ YZ BAŞLANGIÇ NOKTASI

**Son Güncelleme:** 24 Aralık 2025  
**Durum:** 🟢 YZ_08 Göreve Hazır!  
**Önceki YZ:** YZ_07 (String return bug fix)  
**Sen:** selfhosting_YZ_08

---

## ✅ PHASE 0 TAMAMLANDI!

```
┌─────────────────────────────────────────────────────────────┐
│  Stage 0 artık self-hosting için %100 HAZIR!               │
│                                                             │
│  Tamamlanan özellikler:                                    │
│  ✅ YZ_05: While/For Codegen Fix                           │
│  ✅ YZ_06: char_at() string karakter erişimi               │
│  ✅ YZ_06: String concat (+) operatörü                     │
│  ✅ YZ_07: String return bug fix (i8* return type)         │
│                                                             │
│  Test sonuçları:                                           │
│  → While: exit code 10 ✅                                  │
│  → char_at("MELP"; 0) → "M" ✅                             │
│  → "Hello " + "World" → "Hello World" ✅                   │
│  → String döndüren fonksiyon çağrısı ✅                    │
└─────────────────────────────────────────────────────────────┘
```

---

## 📍 ŞİMDİKİ DURUM

**Branch:** `TODO_STAGE1_TO_SELFHOSTING_FINAL`  
**İlerleme:** 4/13 task (%30)

**Aktif Görev:** Phase 1 Task 1.1 - Syntax Analizi

---

## 🎯 YZ_08 GÖREVİ

**Phase 1, Task 1.1: Syntax Analizi**

### Ön Analiz (hazır veri):

```
Syntax Hata Özeti:
- Virgüllü parametre (, yerine ;): 19 dosya
- while...do (do OLMAMALI): 5 dosya  
- Array literal virgül ([a,b] yerine [a;b]): 51 dosya
- then eksik olabilecek if'ler: 20+ dosya

Toplam Stage 1 modül sayısı: 107 dosya
```

### Yapılacaklar:

1. **Tüm Stage 1 modüllerini tara**
   ```bash
   find compiler/stage1/modules -name "*.mlp"
   ```

2. **pmlp_kesin_sozdizimi.md'ye göre kontrol et:**
   - Parametre ayırıcı: `,` → `;`
   - Array literal: `[a, b]` → `[a; b]`
   - while: `while cond do` → `while cond` (do YOK!)
   - Çok satırlı if: `then` gerekli mi?

3. **Düzeltilecek dosya listesi çıkar**

4. **Derleme testi yap:**
   ```bash
   ./scripts/run_mlp.sh dosya.mlp
   ```

### Başarı Kriteri:

- [ ] Tüm syntax hataları listelenmiş
- [ ] En az 3 örnek dosya düzeltilmiş ve derlenmiş
- [ ] Task 1.2 için hazırlık raporu

---

## 📚 ZORUNLU OKUMALAR

1. **TODO_STAGE1_TO_SELFHOSTING_FINAL.md** - Ana görev listesi
2. **TODO_kurallari.md** - YZ kuralları
3. **pmlp_kesin_sozdizimi.md** - MELP syntax referansı (KRİTİK!)

---

## ⚠️ KURALLAR

- TODO'da ne yazıyorsa onu yap
- "Detaylandırmamı ister misin?" YASAK
- Phase/Task icat etme
- Yeni TODO yazma
- Raporu `selfhosting_YZ/YZ_08_TAMAMLANDI.md` olarak yaz

---

## 🛠️ FAYDALI KOMUTLAR

```bash
# MELP programı derle ve çalıştır
./scripts/run_mlp.sh dosya.mlp

# Virgüllü parametre bul
find compiler/stage1/modules -name "*.mlp" -exec grep -l "function.*(.*, " {} \;

# while...do bul
find compiler/stage1/modules -name "*.mlp" -exec grep -l "while.*do" {} \;

# Stage 0 compiler
./compiler/stage0/melp dosya.mlp -o output.ll
```

---

**🚀 PHASE 1 BAŞLIYOR!** 🚀
