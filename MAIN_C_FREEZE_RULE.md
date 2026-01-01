# MAIN.C FREEZE KURALI - PD SEÇENEK C

**Tarih:** 1 Ocak 2026  
**Yetki:** PD_02 - SEÇENEK C (MODÜLER)  
**Durum:** 🔒 AKTİF - İHLAL = VETO!

---

## 🚨 KATI KURAL: main.c FREEZE ❄️

### **Kural:**

```
MELP/C/stage0/main.c = DONMUŞ (FREEZE)

Mevcut: 651 satır
Maksimum: 700 satır (+49 satır acil düzeltme için)
Değişiklik: YASAK!
```

**İhlal = PD VETO!** ⚠️

---

## 📋 NEDEN BU KURAL?

### **1. Monolitik Büyüme Riski**

**Geçmiş:**
```
main.c → Özellik ekle → 1000 satır
       → Özellik ekle → 2000 satır
       → Özellik ekle → 5000 satır
       → Özellik ekle → 10000 satır ❌ MONOLİTİK!
```

**Sonuç:** 6 Esas ihlali, bakım zorlaşır, test zorlaşır

---

### **2. Stage0 Stabildir**

**Bootstrap-Ready (30 Aralık 2025):**
- ✅ 7/7 test passing
- ✅ 6/6 Esas uyumlu
- ✅ Production-ready
- ✅ ÇALIŞIYOR!

**Neden bozalım?** main.c stabil, dokunma!

---

### **3. Modüler Alternatif Var**

```
Yeni Özellik?
├─ main.c'ye ekle ❌ YASAK!
└─ Ayrı binary yap ✅ DOĞRU!
   └─ tools/mlp-{özellik}/
```

**Avantaj:**
- main.c güvenli
- Bağımsız geliştirme
- Test kolay
- Rollback kolay

---

## ✅ İZİN VERİLEN DEĞİŞİKLİKLER

### **Kategori 1: Acil Bug Düzeltmeleri**

**Örnek:**
```c
// Segmentation fault düzeltmesi
if (ptr == NULL) {  // +3 satır
    return ERROR;
}
```

**Sınır:** Max +10 satır/düzeltme  
**Onay:** MM + UA

---

### **Kategori 2: Tool Integration**

**Örnek:**
```c
// Tool call eklemesi
void report_error(const char* code, ...) {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "./tools/mlp-errors/mlp-errors %s ...", code);
    system(cmd);  // +4 satır
}
```

**Sınır:** Max +10 satır/tool  
**Toplam:** Max +40 satır (4 tool)

---

### **Kategori 3: Minimal Refactoring**

**Örnek:**
```c
// Fonksiyon extract (satır sayısı değişmez)
// Before: 20 satır inline kod
// After: extract_function(); // 1 satır call + 20 satır fonksiyon = 21 satır ❌

// Sadece: aynı satır sayısı, daha okunabilir
```

**Sınır:** Net +0 satır  
**Onay:** MM + UA

---

## ❌ YASAK DEĞİŞİKLİKLER

### **1. Yeni Özellik Ekleme**

```c
// ❌ YASAK!
void add_error_formatting() {
    // 100 satır error formatting kodu
}
```

**Alternatif:** `tools/mlp-errors/` ayrı binary

---

### **2. Büyük Refactoring**

```c
// ❌ YASAK!
// main.c'yi 1000 satıra çıkar
```

**Alternatif:** Gerekiyorsa TODO #3'ten sonra değerlendir

---

### **3. Experimental Features**

```c
// ❌ YASAK!
#ifdef EXPERIMENTAL_ASYNC
    // 200 satır async kodu
#endif
```

**Alternatif:** `tools/mlp-async/` ayrı binary

---

## 📏 SATIR SAYISI TAKİBİ

### **Başlangıç (1 Ocak 2026):**
```
main.c: 651 satır
```

### **TODO #1 Sonrası (Hedef):**
```
main.c: 691 satır (max 700)
├─ +40 satır (tool calls)
└─ +0 satır (yeni özellik YOK!)
```

### **Kontrol:**
```bash
# Her commit öncesi
wc -l MELP/C/stage0/main.c
# 651 → 691 → 700'den fazla ise ❌ RED!
```

---

## 🚦 ONAY SÜRECİ

### **Değişiklik Önerisi:**

1. **YZ:** main.c'ye +X satır eklemek istiyorum  
2. **MM:** Neden? Alternatif var mı?  
3. **YZ:** [Gerekçe]  
4. **MM:** [Değerlendirme]  
   - ✅ Onay: +X satır OK  
   - ❌ Red: Ayrı binary yap  
   - ⏸️ PD'ye escalate

5. **PD (gerekirse):** [Final karar]

---

## 📊 MONITORING

### **Haftalık Rapor:**

```markdown
# main.c Durum Raporu - Hafta X

Başlangıç: 651 satır
Şu An: XXX satır
Değişim: +YY satır

Değişiklikler:
- [Commit ABC] +5 satır: Error call eklendi (tool integration)
- [Commit DEF] +3 satır: Null check (bug fix)

Durum: ✅ Sınır içinde (< 700)
```

---

## 🎯 HEDEF

**TODO #1-2-3 Sonunda:**
```
main.c: Max 750 satır (başlangıç 651 + 99 acil)
```

**Stage0 Retirement (TODO #3 sonrası):**
```
main.c: MÜZE MOD 🏛️
Stage2 (MLP): AKTİF GELİŞTİRME 🚀
```

---

## 📞 SORULAR VE CEVAPLAR

### **S: Bu düzeltmeyi main.c'ye ekleyebilir miyim?**

**C:** Önce sor:
1. Acil bug mı? → +10 satır OK
2. Tool integration mı? → +10 satır OK
3. Yeni özellik mi? → ❌ Ayrı binary yap!

---

### **S: main.c 700 satırı aştı, ne yapmalıyım?**

**C:** 
1. Commit'i geri al
2. Alternatif bul (ayrı binary)
3. MM'ye danış

---

### **S: Refactoring gerekli, main.c 1500 satır olacak**

**C:**
1. TODO #3'ten sonra değerlendir
2. Stage0 retirement yakın
3. Stage2'de temiz başla

---

## ✅ ÖZET

```
main.c = FREEZE ❄️

İzin:
├─ Bug fix: +10 satır
├─ Tool call: +40 satır (4 tool × 10)
└─ Refactor: +0 satır (net sıfır)

Yasak:
├─ Yeni özellik
├─ Büyük refactoring
└─ Experimental kod

Sınır: 651 → 700 satır (max +49)
İhlal: PD VETO! ⚠️
```

---

**Belge Tarihi:** 1 Ocak 2026  
**Hazırlayan:** MM_01 (Mastermind)  
**PD Onayı:** PD_02 - SEÇENEK C ✅  
**Durum:** 🔒 AKTİF  
**Geçerlilik:** TODO #3 bitişine kadar (~11 Mart 2026)

**FREEZE ❄️ = STAB İL = GÜVENLİ = PRODUCTION-READY!**
