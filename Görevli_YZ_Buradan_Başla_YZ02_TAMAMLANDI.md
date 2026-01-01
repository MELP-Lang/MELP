# 🤖 SEN: TOOLING_YZ_02

**İkinci görevlisin!** Debug Support yapacaksın.

---

## 🎯 BAĞLAM

**TODO:** #1 - TOOLING_BASICS  
**Task:** 2 - Debug Support  
**Süre:** 5-6 gün  
**Yaklaşım:** GDB-style Debugger + Line-by-line execution

---

## 📋 ÖNCEKİ DURUM

✅ **TODO #0: SELFHOSTING tamamlandı** (30 Aralık 2025)
- Stage0 Bootstrap-Ready ✅
- 7/7 test geçti ✅
- main.c: 651 satır ✅

✅ **Task 1: Error Messages TAMAMLANDI** (1 Ocak 2026)
- YZ_01 tarafından tamamlandı ✅
- 54 error/warning code (TR+EN) ✅
- Modüler binary (tools/mlp-errors/) ✅
- main.c: 659 satır (+8, limit içinde) ✅
- 22/22 test passing ✅
- Rapor: [TODO_TOOLING_BASICS/TOOLING_YZ/YZ_01_RAPOR_ERROR_MESSAGES.md](TODO_TOOLING_BASICS/TOOLING_YZ/YZ_01_RAPOR_ERROR_MESSAGES.md)

✅ **Task 2: Debug Support TAMAMLANDI** (1 Ocak 2026)
- YZ_02 tarafından tamamlandı ✅
- DWARF debug info + GDB integration ✅
- Modüler binary (tools/mlp-debug/) ~600 satır ✅
- debug_info modülü ~220 satır ✅
- main.c: 687 satır (+28, limit içinde) ✅
- 6/6 test passing ✅

⏸️ **TODO #3: LANGUAGE_FEATURES pause'da**
- PD Kararı: TODO #1-2 önce yapılacak
- Sebep: Stage0 import desteği yok
- 10 hafta pause (1 Ocak - 18 Mart 2026)

🎯 **TODO #1 devam ediyor** (1 Ocak 2026)
- Task 1: Error Messages ✅ (YZ_01 - TAMAMLANDI)
- Task 2: Debug Support ✅ (YZ_02 - TAMAMLANDI)
- Task 3: LSP Server (YZ_03)
- Task 4: Code Formatter (YZ_04)

---

## 🎯 GÖREVİN: DEBUG SUPPORT

**Hedef:** GDB-style debugger ile line-by-line execution, breakpoints, variable inspection

**Yaklaşım:** Source Map + Debugger Binary (main.c'ye DOKUNMA!)

**Kritik Kural:** 🔒 **main.c FREEZE** (659 → max 700 satır)
- main.c'ye max +10 satır eklenebilir (debugger integration)
- Tüm debug logic ayrı binary'de olacak

---

## 📋 GÖREV DETAYLARI

### **Phase 1: Source Map Generation** (2 gün)

#### **Adım 1.1: Source Map Format** (1 gün)

**Oluştur:** `data/sourcemap_format.json`

Source map format spesifikasyonu:

```json
{
  "version": "1.0",
  "sourceFile": "test.mlp",
  "generatedFile": "test.c",
  "mappings": [
    {
      "mlpLine": 1,
      "mlpColumn": 1,
      "cLine": 10,
      "cColumn": 5,
      "scope": "global",
      "variables": ["x", "y"]
    }
  ]
}
```

**Gereksinimler:**
- MLP satır → C satır mapping
- Variable scope tracking
- Function boundaries
- JSON format

**Başarı Kriterleri:**
- [ ] sourcemap_format.json spec'i yazıldı
- [ ] Documentation complete
- [ ] Example mappings

---

#### **Adım 1.2: Source Map Generator** (1 gün)

**Lokasyon:** `tools/mlp-sourcemap/`

main.c'ye entegre source map generation:

```c
void generate_source_map(const char* mlp_file, const char* c_file) {
    // Track MLP line → C line mappings during codegen
    // Output: <output>.c.map
}
```

**Başarı Kriterleri:**
- [ ] Source map generation çalışıyor
- [ ] .map dosyası oluşturuluyor
- [ ] Mapping accuracy doğrulandı

---

### **Phase 2: Debugger Binary** (3 gün)

#### **Adım 2.1: Debugger Core** (1 gün)

**Lokasyon:** `tools/mlp-debug/`

**Dosya Yapısı:**
```
tools/mlp-debug/
├── main.c               (~200 satır) - CLI interface
├── debugger.c           (~300 satır) - Core debugger
├── breakpoint.c         (~150 satır) - Breakpoint management
├── sourcemap_reader.c   (~100 satır) - Read .map files
├── debugger.h           (~50 satır)
└── Makefile
```

**Temel Özellikler:**
- Breakpoint set/remove
- Step (next line)
- Continue
- Print variables
- Stack trace

**Başarı Kriterleri:**
- [ ] tools/mlp-debug/ oluşturuldu
- [ ] Basic debugger skeleton
- [ ] Makefile çalışıyor

---

#### **Adım 2.2: Breakpoint System** (1 gün)

Breakpoint yönetimi:

```c
// Set breakpoint
mlp-debug -b test.mlp:10

// Remove breakpoint
mlp-debug -d 1

// List breakpoints
mlp-debug -l
```

**Implementation:**
- Breakpoint database (in-memory)
- MLP line → C line translation
- Hit detection

**Başarı Kriterleri:**
- [ ] Breakpoint set/remove çalışıyor
- [ ] List breakpoints
- [ ] Persistent between sessions

---

#### **Adım 2.3: Interactive Debugger** (1 gün)

GDB-style interactive REPL:

```
(mlp-debug) break test.mlp:10
Breakpoint 1 set at test.mlp:10 (test.c:45)

(mlp-debug) run
Starting program: test

Breakpoint 1, test.mlp:10
10:   let x = calculate(42)

(mlp-debug) print x
x = <undefined>

(mlp-debug) step
11:   print(x)

(mlp-debug) print x
x = 84

(mlp-debug) continue
Program exited normally.
```

**Komutlar:**
- `break <file>:<line>` - Set breakpoint
- `run` - Start program
- `step` - Execute next line
- `next` - Step over function calls
- `continue` - Continue execution
- `print <var>` - Print variable value
- `backtrace` - Show stack trace
- `quit` - Exit debugger

**Başarı Kriterleri:**
- [ ] Interactive REPL çalışıyor
- [ ] Tüm komutlar implement edildi
- [ ] Variable inspection
- [ ] Line-by-line execution

---

### **Phase 3: Integration & Test** (1 gün)

#### **Adım 3.1: main.c Integration** (0.5 gün)

**MELP/C/stage0/main.c'ye +10 satır MAX:**

```c
void enable_debug_mode(const char* output_file) {
    // Generate source map: output.c.map
    generate_source_map(input_mlp, output_file);
}
```

**Başarı Kriterleri:**
- [ ] main.c: 659 → 669 satır (max)
- [ ] Source map generation
- [ ] --debug flag support

---

#### **Adım 3.2: Test Suite** (0.5 gün)

`tests/debug/` dizini + 10+ test dosyası

**Test Scenarios:**
1. Set/remove breakpoints
2. Step through code
3. Variable inspection
4. Function calls
5. Stack trace
6. Error conditions

**Başarı Kriterleri:**
- [ ] Tüm testler geçiyor
- [ ] Debugger stable

---

## 🎯 BAŞARI KRİTERLERİ

- [ ] `data/sourcemap_format.json` spec
- [ ] Source map generation (main.c'de)
- [ ] `tools/mlp-debug/` binary (< 1000 satır)
- [ ] Interactive debugger REPL
- [ ] `main.c` (+10 satır max)
- [ ] `tests/debug/` (10+ test)
- [ ] Breakpoint system
- [ ] Variable inspection
- [ ] Line-by-line execution

---

## ⏱️ ZAMAN ÇİZELGESİ

| Gün | Task |
|-----|------|
| 1-2 | Source map format + generator |
| 3 | Debugger core |
| 4 | Breakpoint system |
| 5 | Interactive debugger |
| 6 | Integration + Test |

**Deadline:** 8 Ocak 2026

---

## 📚 ZORUNLU OKUMALAR

1. **TODO_KURALLARI.md** → Bölüm 1
2. **1-TODO_TOOLING_BASICS_MODULAR.md** → Task 2
3. **MAIN_C_FREEZE_RULE.md** → main.c kuralları
4. **TODO_TOOLING_BASICS/TOOLING_YZ/YZ_01_RAPOR_ERROR_MESSAGES.md** → Önceki görevliden öğren

---

## 💡 İPUÇLARI

**YZ_01'den Öğrenmeler:**
- ✅ Modüler mimari çalıştı
- ✅ cJSON gibi mature libraries kullan
- ✅ Data-driven design (JSON)
- ✅ Kapsamlı test suite
- ✅ main.c'ye minimal dokunuş

**Debug-Specific:**
- GDB source koduna bakabilirsin (referans için)
- DWARF format yerine JSON-based source map kullan (basitlik için)
- Interactive REPL için readline kütüphanesi düşün
- Variable inspection için C debuginfo API kullanabilirsin

---

**BAŞARILAR YZ_02!** 🎯

**Tarih:** 1 Ocak 2026  
**MM:** MM_01  
**PD Onayı:** PD_02 ✅  
**Durum:** 🟢 AKTİF

---

**NOT:** YZ_01'in çalışmalarını inceleyerek başla. Onun modüler yaklaşımı ve test stratejisi çok başarılıydı!

