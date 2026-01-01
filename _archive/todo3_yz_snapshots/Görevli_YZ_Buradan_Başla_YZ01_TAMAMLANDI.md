# 🤖 SEN: TOOLING_YZ_02

**İkinci görevlisin!** Debug Support yapacaksın.

---

## 🎯 BAĞLAM

**TODO:** #1 - TOOLING_BASICS  
**Task:** 2 - Debug Support (GDB Integration)  
**Süre:** 5-6 gün  
**Yaklaşım:** DWARF debug info + GDB integration

---

## 📋 ÖNCEKİ DURUM

✅ **TODO #0: SELFHOSTING tamamlandı** (30 Aralık 2025)
- Stage0 Bootstrap-Ready ✅
- 7/7 test geçti ✅
- main.c: 651 satır ✅

✅ **Task 1: Error Messages TAMAMLANDI** (YZ_01)
- 50+ error codes, 20+ warnings ✅
- JSON database + mlp-errors binary ✅
- Renkli, çok dilli mesajlar ✅
- main.c: 651 → 659 satır (+8) ✅

⏸️ **TODO #3: LANGUAGE_FEATURES pause'da**
- 10 hafta pause (1 Ocak - 18 Mart 2026)

🎯 **TODO #1 devam ediyor** (1 Ocak 2026)
- Task 1: Error Messages ✅ (YZ_01 - TAMAMLANDI)
- Task 2: Debug Support (SEN! - YZ_02)
- Task 3: LSP Server (YZ_03)
- Task 4: Code Formatter (YZ_04)

---

## 🎯 GÖREVİN: DEBUG SUPPORT

**Hedef:** GDB ile MLP kodunu debug edebilme (DWARF debug info)

**Yaklaşım:** 
1. DWARF debug info generation
2. Source line mapping
3. GDB integration
4. Minimal binary (main.c'ye minimal dokunuş)

**Kritik Kural:** 🔒 **main.c FREEZE** (659 → max 700 satır)
- main.c'ye max +10 satır eklenebilir (debug flag handling)
- Debug info generation ayrı modülde

---

## 📋 GÖREV DETAYLARI

### **Phase 1: DWARF Debug Info** (2 gün)

#### **Adım 1.1: Debug Info Module** (1 gün)

**Değiştirilecek:** `MELP/C/stage0/modules/codegen/debug_info.c` (mevcut dosya genişletilecek)

**Mevcut:** ~100 satır (basit debug info)  
**Hedef:** ~300 satır (tam DWARF desteği)

**Eklenecekler:**
```c
void emit_dwarf_line_info(FILE* out, const char* mlp_file, int line) {
    fprintf(out, "#line %d \"%s\"\n", line, mlp_file);
}

void emit_dwarf_variable_info(FILE* out, const char* var_name, const char* type) {
    // DWARF DIE (Debug Information Entry)
    fprintf(out, "/* DWARF: var %s : %s */\n", var_name, type);
}
```

**Başarı Kriterleri:**
- [ ] debug_info.c genişletildi (~300 satır)
- [ ] DWARF line info emit ediliyor
- [ ] Variable debug info eklendi
- [ ] main.c değişmedi!

---

#### **Adım 1.2: Source Mapping** (1 gün)

**Oluştur:** `tools/mlp-debug/source_mapper.c` (ayrı binary)

**Dosya Yapısı:**
```
tools/mlp-debug/
├── main.c           (~150 satır)
├── source_mapper.c  (~250 satır)
├── gdb_wrapper.c    (~200 satır)
└── Makefile
```

**source_mapper.c:**
```c
// MLP line → C line → Binary address mapping
typedef struct {
    char* mlp_file;
    int mlp_line;
    char* c_file;
    int c_line;
} SourceMap;

SourceMap* create_source_map(const char* mlp_file);
int mlp_to_c_line(SourceMap* map, int mlp_line);
```

**Başarı Kriterleri:**
- [ ] tools/mlp-debug/ oluşturuldu
- [ ] Source mapping çalışıyor
- [ ] MLP line → C line dönüşümü OK

---

### **Phase 2: GDB Integration** (2 gün)

#### **Adım 2.1: GDB Wrapper** (1 gün)

**tools/mlp-debug/gdb_wrapper.c:**
```c
// GDB'yi MLP context ile çalıştır
int run_gdb_with_mlp_context(const char* binary, const char* mlp_file) {
    // 1. Source map oku
    // 2. GDB başlat
    // 3. Breakpoint'leri translate et (MLP line → C line)
    // 4. GDB output'u MLP context'e çevir
}
```

**Kullanım:**
```bash
# Debug mode ile derle
./melpc -g test.mlp test.c
gcc -g test.c -o test

# GDB ile debug (MLP line numbers ile)
./tools/mlp-debug/mlp-debug test test.mlp
```

**Başarı Kriterleri:**
- [ ] gdb_wrapper.c çalışıyor
- [ ] Breakpoint translation OK
- [ ] MLP line numbers görünüyor

---

#### **Adım 2.2: Debug Commands** (1 gün)

**mlp-debug binary komutları:**
```
(mlp-debug) break test.mlp:10      # MLP line'da breakpoint
(mlp-debug) run                     # Program çalıştır
(mlp-debug) print x                 # Variable değeri
(mlp-debug) step                    # Bir satır ilerle
(mlp-debug) continue                # Devam et
```

**Başarı Kriterleri:**
- [ ] break command çalışıyor
- [ ] print command çalışıyor
- [ ] step/continue OK

---

### **Phase 3: Integration & Test** (1 gün)

#### **Adım 3.1: main.c Integration** (0.5 gün)

**MELP/C/stage0/main.c'ye +10 satır MAX:**

```c
// -g flag handling
if (debug_mode) {
    enable_debug_info = 1;
}
```

**Başarı Kriterleri:**
- [ ] main.c: 659 → 669 satır (+10)
- [ ] -g flag çalışıyor
- [ ] Debug info generation tetikleniyor

---

#### **Adım 3.2: Test Suite** (0.5 gün)

`tests/debug/` dizini:
```
tests/debug/
├── test_breakpoint.mlp
├── test_step.mlp
├── test_variables.mlp
└── run_debug_tests.sh
```

**Başarı Kriterleri:**
- [ ] 10+ test dosyası
- [ ] Tüm testler geçiyor
- [ ] GDB integration test edildi

---

## 🎯 BAŞARI KRİTERLERİ

**Deliverables:**
- [ ] `debug_info.c` genişletildi (~300 satır)
- [ ] `tools/mlp-debug/` binary (~600 satır)
- [ ] `main.c` (+10 satır, 659 → 669)
- [ ] `tests/debug/` (10+ test)
- [ ] GDB ile debug çalışıyor

**Kalite:**
- [ ] main.c FREEZE kuralı OK (< 700 satır)
- [ ] DWARF info doğru
- [ ] Source mapping doğru
- [ ] MLP line numbers GDB'de görünüyor

---

## ⏱️ ZAMAN ÇİZELGESİ

| Gün | Task |
|-----|------|
| 1 | debug_info.c genişlet |
| 2 | Source mapper |
| 3 | GDB wrapper |
| 4 | Debug commands |
| 5 | Integration + Test |

**Deadline:** 12 Ocak 2026 (YZ_01'den 5 gün sonra)

---

## 📚 ZORUNLU OKUMALAR

1. **TODO_KURALLARI.md** → Bölüm 1
2. **1-TODO_TOOLING_BASICS_MODULAR.md** → Task 2
3. **MAIN_C_FREEZE_RULE.md** → main.c kuralları
4. **YZ_01_RAPOR** → Error system nasıl çalışıyor

---

**BAŞARILAR YZ_02!** 🎯

**Tarih:** 1 Ocak 2026  
**MM:** MM_01  
**Önceki:** YZ_01 (Error Messages) ✅  
**Durum:** 🟢 AKTİF

