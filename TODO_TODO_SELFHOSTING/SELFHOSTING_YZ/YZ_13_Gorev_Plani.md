# YZ_13 GÖREV PLANI - Runtime Modularization (Implementation)

**YZ:** SELFHOSTING_YZ_13  
**Tarih:** 30 Aralık 2025, 14:15  
**TODO:** #4 - Runtime Modularization (Implementation Fazı)  
**Yönetici:** UA_04  
**Süre Hedefi:** 3 gün (24 saat çalışma)  
**Durum:** 🟢 AKTIF

---

## 🎯 SENİN GÖREVİN (YZ_13)

**YZ_12 Analizi:** ✅ TAMAMLANDI - [YZ_12_ANALIZ_RAPOR.md](../../YZ_12_ANALIZ_RAPOR.md)

**Görev:** YZ_12'nin analizine göre runtime.c'yi 8 modüle böl, implement et.

**Hedef:** MODULAR esasını restore et (3245 satır → 8 modül × ~400 satır)

---

## 📋 ZORUNLU OKUMALAR

**ÖNCE BUNLARI OKU:**

1. ✅ [YZ_12_ANALIZ_RAPOR.md](../../YZ_12_ANALIZ_RAPOR.md) - Detaylı plan burada!
2. ✅ [YZ_KURALLAR.md](../../YZ_KURALLAR.md) - YZ kuralları
3. ✅ [TODO_SELFHOSTING.txt](../../TODO_SELFHOSTING.txt) - İzin verilen dosyalar
4. ✅ [0-TODO_SELFHOSTING.md](../../0-TODO_SELFHOSTING.md) - TODO #4 detayları

**YZ_12'nin 808 satırlık raporunu OKU! Tüm detaylar orada!**

---

## 📋 TASK LİSTESİ (11 Task)

### Phase 1: Core Modules (Day 1) - 8 saat

#### Task 1.1: runtime_error.c + .h ⏳ NEXT
**Süre:** 1 saat  
**Hedef:** Error handling modülü (temel bağımlılık)

**Fonksiyonlar (5):**
- `panic(message)`
- `mlp_assert(condition, message)`
- `exit_with_code(code)`
- `get_error_code()` / `set_error_code(code)`

**⚠️ DİKKAT: STATELESS İhlali!**
- `static int mlp_last_error_code` → thread-safe yap!
- **Çözüm:** `__thread int mlp_last_error_code` (thread-local storage)

**Dosyalar:**
- `MELP/runtime/runtime_error.c` (yeni)
- `MELP/runtime/runtime_error.h` (yeni)

**Test:**
```bash
cd MELP/runtime
gcc -c runtime_error.c -o runtime_error.o
# No errors = OK
```

---

#### Task 1.2: runtime_memory.c + .h ⏳
**Süre:** 2 saat  
**Hedef:** Memory management modülü

**Fonksiyonlar (5 + helpers):**
- `mlp_malloc(size)`, `mlp_free(ptr)`
- `check_memory_leaks()`, `get_allocated_bytes()`
- `c_malloc()`, `c_free()`, `c_memcpy()`, `c_memset()`

**Bağımlılık:**
- Depends on: `runtime_error.h`

**Test:**
```bash
gcc -c runtime_memory.c -o runtime_memory.o
```

---

#### Task 1.3: runtime_string.c + .h ⏳
**Süre:** 3 saat  
**Hedef:** String operations (39 fonksiyon!)

**Önemli:**
- En büyük modüllerden biri (~480 satır)
- YZ_12 raporunda tüm fonksiyonlar listelendi
- Dikkat: < 500 satır limiti!

**Bağımlılık:**
- Depends on: `runtime_memory.h`, `runtime_error.h`

**Test:**
```bash
gcc -c runtime_string.c -o runtime_string.o
```

---

#### Task 1.4: runtime_types.c + .h ⏳
**Süre:** 2 saat  
**Hedef:** Type conversion + Dict

**🔴 KRİTİK: Dict STATELESS İhlali!**

**Mevcut Durum:**
```c
static SozlukGirdisi* sozluk_tablosu = NULL;  // ❌ Global state!
static int sozluk_boyut = 0;
```

**Çözüm:**
```c
typedef struct {
    SozlukGirdisi* tablo;
    int boyut;
} MLP_Dict;

MLP_Dict* sozluk_olustur(int boyut);
void sozluk_ekle(MLP_Dict* dict, const char* anahtar, int64_t deger);
int64_t sozluk_getir(MLP_Dict* dict, const char* anahtar);
void sozluk_yok_et(MLP_Dict* dict);
```

**DİKKAT:** Signature değişikliği → compiler güncellemesi gerekebilir!

**Bağımlılık:**
- Depends on: `runtime_memory.h`, `runtime_string.h`, `runtime_error.h`

---

### Phase 2: Data & I/O Modules (Day 2) - 8 saat

#### Task 2.1: runtime_array.c + .h ⏳
**Süre:** 2 saat  
**Hedef:** Array/List operations (14 fonksiyon)

**Bağımlılık:**
- Depends on: `runtime_memory.h`, `runtime_error.h`

---

#### Task 2.2: runtime_print.c + .h ⏳
**Süre:** 1.5 saat  
**Hedef:** Print/Debug utilities (5 fonksiyon)

**Fonksiyonlar:**
- `yazdir_sayi()`, `yazdir_metin()`
- `read_input()`, `read_line()`, `read_int()`

**Bağımlılık:**
- Depends on: `runtime_memory.h`, `runtime_string.h`, `runtime_types.h`

---

#### Task 2.3: runtime_io.c + .h ⏳
**Süre:** 3 saat  
**Hedef:** File I/O operations (13 fonksiyon)

**Fonksiyonlar:**
- `dosya_ac()`, `dosya_oku()`, `dosya_yaz()`, `dosya_kapat()`
- `read_file()`, `write_file()`, `append_file()`, etc.

**Bağımlılık:**
- Depends on: `runtime_memory.h`, `runtime_string.h`, `runtime_error.h`

---

#### Task 2.4: runtime_system.c + .h ⏳
**Süre:** 1.5 saat  
**Hedef:** System/Process/Time/Math utilities (31 fonksiyon)

**Kategoriler:**
- Math (12 fonksiyon): `abs`, `sqrt`, `pow`, `sin`, `cos`, etc.
- Time (5 fonksiyon): `time_now()`, `time_format()`, etc.
- System (6 fonksiyon): `get_env()`, `set_env()`, etc.
- Process (3 fonksiyon): `execute()`, `get_pid()`, etc.
- Path (1 fonksiyon): `path_join()`

**Bağımlılık:**
- Depends on: `runtime_memory.h`, `runtime_string.h`, `runtime_error.h`, `runtime_array.h`

---

### Phase 3: Integration & Testing (Day 3) - 6-8 saat

#### Task 3.1: Makefile Güncelleme ⏳
**Süre:** 1 saat  
**Hedef:** 8 modülü derleyen Makefile oluştur

**Referans:** YZ_12 raporunda Makefile template var!

**Dosya:** `MELP/runtime/Makefile`

**Compilation Order:**
```makefile
# Dependency order (bottom to top)
runtime_error.o      # No dependencies
runtime_memory.o     # Depends on: error
runtime_string.o     # Depends on: memory, error
runtime_types.o      # Depends on: memory, string, error
runtime_array.o      # Depends on: memory, error
runtime_print.o      # Depends on: memory, string, types, error
runtime_io.o         # Depends on: memory, string, error
runtime_system.o     # Depends on: memory, string, error, array
```

**Test:**
```bash
cd MELP/runtime
make clean && make
# Should create: libruntime.a
```

---

#### Task 3.2: Header Dependencies Fix ⏳
**Süre:** 2 saat  
**Hedef:** Header guards, forward declarations, circular dependency prevention

**Her header'da:**
```c
#ifndef RUNTIME_MODULE_H
#define RUNTIME_MODULE_H

// Forward declarations (if needed)
typedef struct MLP_List MLP_List;

// Includes
#include "runtime_error.h"

// Function declarations

#endif
```

**Test:**
```bash
# Test all headers compile independently
gcc -c -x c runtime_error.h
gcc -c -x c runtime_memory.h
# etc...
```

---

#### Task 3.3: Dict Struct Refactor Integration ⏳
**Süre:** 2 saat  
**Hedef:** Dict'i struct'a çevirdiğinde compiler'da gereken değişiklikleri yap

**Olası etkilenen dosyalar:**
- Stage0 compiler'da dict kullanımı var mı?
- Test dosyalarında dict kullanımı?

**Strateji:**
1. Eski `sozluk_*` fonksiyonlarını wrapper'la:
   ```c
   // Backward compatibility (deprecated)
   static MLP_Dict* global_dict = NULL;
   
   void sozluk_olustur(int boyut) {
       if (global_dict) sozluk_yok_et(global_dict);
       global_dict = dict_create(boyut);
   }
   ```

2. Yeni kod'da `dict_create()` kullan
3. Eskiyi deprecate et

**Test:**
```bash
cd MELP/MLP/stage0
make clean && make
# Tüm testler pass olmalı!
```

---

#### Task 3.4: Test & Debug ⏳
**Süre:** 1-2 saat  
**Hedef:** Tüm testleri çalıştır, regression check

**Test Suite:**
```bash
# Runtime tests
cd MELP/runtime
make test

# Stage0 compile test
cd ../MLP/stage0
make clean && make

# Example tests
cd ../../examples
./test_all.sh

# Specific tests
./melpc hello_english.mlp -o hello.c
gcc hello.c -L../MELP/runtime -lruntime -o hello
./hello
```

**Beklenen:**
- ✅ Tüm testler PASS
- ✅ No segfaults
- ✅ No memory leaks
- ✅ No regressions

---

## 🚫 YAPAMAZSIN (KRİTİK!)

**❌ Fonksiyon signature değiştirme (Dict hariç)**
- Dict struct refactor iznin var (STATELESS için gerekli)
- Diğer fonksiyonları aynen kopyala!

**❌ TODO_SELFHOSTING.txt dışı dosya**
- Sadece `MELP/runtime/` altında çalış
- Compiler değişikliği gerekirse UA'ya sor!

**❌ Stub/Hack kullanma**
- Her fonksiyon gerçek implementasyon
- `return 0;` gibi geçici kodlar yasak!

**❌ Modül > 500 satır**
- MODULAR esasını korumak ZORUNLU!
- Eğer bir modül 500'ü geçerse UA'ya sor!

---

## ✅ BAŞARI KRİTERLERİ

TODO #4 (Implementation) bittiğinde:

1. ✅ **8 modül oluşturuldu:**
   - [ ] runtime_error.c + .h (~150 satır)
   - [ ] runtime_memory.c + .h (~450 satır)
   - [ ] runtime_string.c + .h (~480 satır)
   - [ ] runtime_types.c + .h (~250 satır)
   - [ ] runtime_array.c + .h (~350 satır)
   - [ ] runtime_print.c + .h (~250 satır)
   - [ ] runtime_io.c + .h (~450 satır)
   - [ ] runtime_system.c + .h (~480 satır)

2. ✅ **MODULAR esası restore edildi:**
   - [ ] Her modül < 500 satır
   - [ ] runtime.c silindi veya deprecated

3. ✅ **STATELESS ihlali düzeltildi:**
   - [ ] Dict struct-based (global state yok)
   - [ ] Error code thread-safe

4. ✅ **Compilation başarılı:**
   - [ ] Makefile güncellendi
   - [ ] `make clean && make` çalışıyor
   - [ ] libruntime.a oluşturuluyor

5. ✅ **Testler PASS:**
   - [ ] Runtime modül testleri
   - [ ] Stage0 compilation
   - [ ] Example programs çalışıyor
   - [ ] No regressions

6. ✅ **Git commit:**
   - [ ] Her modül commit edildi
   - [ ] YZ_13_RAPOR.md yazıldı
   - [ ] NEXT_AI güncellendi

---

## 📚 REFERANSLAR

**ANA REFERANS: YZ_12_ANALIZ_RAPOR.md**
- 808 satır detaylı analiz
- Her modül için fonksiyon listesi
- Header tasarımları
- Bağımlılık analizi
- Makefile template
- Test stratejisi

**Diğer Referanslar:**
- `/home/pardus/projeler/MLP/MLP-GCC/MELP/runtime/runtime.c` (kaynak)
- `YZ_KURALLAR.md` - YZ kuralları
- `0-TODO_SELFHOSTING.md` - TODO #4 açıklaması
- `docs/ARCHITECTURE_AND_MEMORY_STRATEGY.md` - Mimari

---

## 📌 İLK ADIM

**ŞİMDİ YAP:**

1. **YZ_12_ANALIZ_RAPOR.md'yi OKU** (808 satır - atla!)
   ```bash
   cat /home/pardus/projeler/MLP/MLP-GCC/YZ_12_ANALIZ_RAPOR.md | less
   ```

2. **Kendini tanıt** (YZ_KURALLAR.md protokolü)

3. **Task 1.1'e başla:** runtime_error.c + .h oluştur

4. **Her task sonunda test et:**
   ```bash
   gcc -c runtime_error.c && echo "OK"
   ```

---

## ⚠️ SORUN OLURSA

**3 deneme kuralı:**
1. İlk deneme → Kendi çöz
2. İkinci deneme → Farklı yaklaşım
3. Üçüncü deneme → Başarısız → UA_04'e sor!

**Context dolduğunda:**
- Yarım kalan task'ı commit et
- YZ_13_PARTIAL_RAPOR.md yaz
- UA_04'e bildir

**Dict refactor sorun çıkarırsa:**
- Backward compatibility wrapper kullan
- Eski fonksiyonları deprecate et
- Yeni kod'da struct kullan

---

**İYİ ÇALIŞMALAR YZ_13! UA_04 seni takip ediyor.** 🚀

---

*Oluşturuldu: 30 Aralık 2025, 14:15*  
*Yönetici: UA_04*  
*Önceki: YZ_12 (Analiz - TAMAMLANDI)*
