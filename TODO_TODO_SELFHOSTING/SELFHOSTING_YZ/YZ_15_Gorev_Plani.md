# YZ_15 Görev Planı: TODO #5 - STDLIB_COMPLETE

**Tarih:** 30 Aralık 2025, 15:00  
**Görevli:** YZ_15  
**Yönetici:** UA_04 (Üst Akıl #04)  
**Durum:** 🔄 BAŞLIYOR

---

## 🎯 GÖREV ÖZETİ

**Hedef:** P2'den eksik stdlib modüllerini MLP-GCC'ye taşı  
**Süre:** 3-5 gün  
**Kritiklik:** 🟡 YÜKSEK

**Kaynak Proje:** `/home/pardus/projeler/MLP/MLP-LLVM/` (P2)  
**Hedef Proje:** `/home/pardus/projeler/MLP/MLP-GCC/` (Bu proje)

---

## 📋 TASK 5.1: P2 stdlib Modüllerini Al (1. Gün)

### Hedef Modüller (7 adet):

| # | Modül | Dosyalar | Satır | Açıklama |
|---|-------|----------|-------|----------|
| 1 | mlp_list | mlp_list.c/h | 328 | Dinamik koleksiyonlar |
| 2 | mlp_optional | mlp_optional.c/h | 191 | Null safety |
| 3 | mlp_state | mlp_state.c/h | 426 | Modül state management |
| 4 | mlp_panic | mlp_panic.c/h | 103 | Error handling |
| 5 | mlp_io | mlp_io.c/h | 268 | I/O operations |
| 6 | file_io | file_io.c/h | 384 | File I/O |
| 7 | mlp_math | mlp_math.c/h | 243 | Math operations |

**Toplam:** ~1943 satır (14 dosya)

### Komutlar:

```bash
# Kaynak: P2 stdlib
cd /home/pardus/projeler/MLP/MLP-LLVM/runtime/stdlib/

# Hedef: MLP-GCC runtime
TARGET="/home/pardus/projeler/MLP/MLP-GCC/MELP/runtime/"

# Kopyala (7 modül × 2 dosya = 14 dosya)
cp mlp_list.c mlp_list.h $TARGET
cp mlp_optional.c mlp_optional.h $TARGET
cp mlp_state.c mlp_state.h $TARGET
cp mlp_panic.c mlp_panic.h $TARGET
cp mlp_io.c mlp_io.h $TARGET
cp file_io.c file_io.h $TARGET
cp mlp_math.c mlp_math.h $TARGET

# Doğrula
ls -lh $TARGET/mlp_*.c $TARGET/mlp_*.h $TARGET/file_io.*
```

### Başarı Kriterleri:

- [ ] 14 dosya başarıyla kopyalandı
- [ ] Header dosyaları (`*.h`) mevcut
- [ ] Source dosyaları (`*.c`) mevcut
- [ ] Dosya boyutları makul (100-500 satır arası)

---

## 📋 TASK 5.2: GCC Uyumluluk Kontrolü (1. Gün)

### Hedef:
P2'den gelen LLVM-optimized kod GCC ile derlenebilmeli.

### Kontroller:

```bash
cd /home/pardus/projeler/MLP/MLP-GCC/MELP/runtime/

# Test compile (her modül)
gcc -c mlp_list.c -o mlp_list.o -I. -Wall -Wextra
gcc -c mlp_optional.c -o mlp_optional.o -I. -Wall -Wextra
gcc -c mlp_state.c -o mlp_state.o -I. -Wall -Wextra
gcc -c mlp_panic.c -o mlp_panic.o -I. -Wall -Wextra
gcc -c mlp_io.c -o mlp_io.o -I. -Wall -Wextra
gcc -c file_io.c -o file_io.o -I. -Wall -Wextra
gcc -c mlp_math.c -o mlp_math.o -I. -Wall -Wextra
```

### Potansiyel LLVM → GCC Uyumluluk Sorunları:

1. **LLVM Builtins:** `__builtin_*` fonksiyonları GCC'de mevcut mu?
2. **Attribute Syntax:** `__attribute__((always_inline))` vs GCC syntax
3. **Math Functions:** `-lm` (math library) gerekebilir
4. **Include Paths:** `#include <...>` yolları doğru mu?

### Başarı Kriterleri:

- [ ] 7 modül GCC ile derlenebiliyor (0 error)
- [ ] Warning sayısı minimize edildi
- [ ] Math library (-lm) gerekirse eklendi
- [ ] Tüm header bağımlılıkları çözüldü

---

## 📋 TASK 5.3: Makefile Güncelle (2. Gün)

### Hedef:
7 stdlib modülünü `libmlp_runtime.a` içine entegre et.

### Makefile Değişiklikleri:

```makefile
# Dosya: MELP/runtime/Makefile

# Mevcut runtime objeleri
RUNTIME_OBJS = runtime_error.o runtime_memory.o runtime_string.o \
               runtime_types.o runtime_array.o runtime_print.o \
               runtime_io.o runtime_system.o

# YENİ: stdlib objeleri (Task 5.3)
STDLIB_OBJS = mlp_list.o mlp_optional.o mlp_state.o \
              mlp_panic.o mlp_io.o file_io.o mlp_math.o

# Library oluştur (runtime + stdlib)
libmlp_runtime.a: $(RUNTIME_OBJS) $(STDLIB_OBJS)
	ar rcs $@ $^
	ranlib $@

# Stdlib compile rules
mlp_list.o: mlp_list.c mlp_list.h
	$(CC) $(CFLAGS) -c $< -o $@

mlp_optional.o: mlp_optional.c mlp_optional.h
	$(CC) $(CFLAGS) -c $< -o $@

mlp_state.o: mlp_state.c mlp_state.h
	$(CC) $(CFLAGS) -c $< -o $@

mlp_panic.o: mlp_panic.c mlp_panic.h
	$(CC) $(CFLAGS) -c $< -o $@

mlp_io.o: mlp_io.c mlp_io.h
	$(CC) $(CFLAGS) -c $< -o $@

file_io.o: file_io.c file_io.h
	$(CC) $(CFLAGS) -c $< -o $@

mlp_math.o: mlp_math.c mlp_math.h
	$(CC) $(CFLAGS) -c $< -o $@ -lm

# Clean rule güncellendi
clean:
	rm -f $(RUNTIME_OBJS) $(STDLIB_OBJS) libmlp_runtime.a
```

### Test Build:

```bash
cd MELP/runtime
make clean
make libmlp_runtime.a

# Library boyutu kontrolü
ls -lh libmlp_runtime.a
# Beklenen: ~60-80KB (önceki 50KB + ~1943 satır kod)

# Archive içeriği
ar t libmlp_runtime.a | grep mlp_
# Beklenen: mlp_list.o, mlp_optional.o, mlp_state.o, ...
```

### Başarı Kriterleri:

- [ ] Makefile güncellendi (STDLIB_OBJS eklendi)
- [ ] `libmlp_runtime.a` başarıyla build ediliyor
- [ ] Library size artmış (~60-80KB)
- [ ] Archive içinde 7 yeni stdlib modülü var

---

## 📋 TASK 5.4: Test Dosyalarını Al (2-3. Gün)

### Hedef:
P2'nin stdlib test dosyalarını adapte et.

### Komutlar:

```bash
# P2 test dizini
P2_TESTS="/home/pardus/projeler/MLP/MLP-LLVM/tests/stdlib/"

# MLP-GCC test dizini (yoksa oluştur)
TARGET_TESTS="/home/pardus/projeler/MLP/MLP-GCC/tests/stdlib/"
mkdir -p $TARGET_TESTS

# Test dosyalarını kopyala
cp $P2_TESTS/test_list.mlp $TARGET_TESTS/
cp $P2_TESTS/test_optional.mlp $TARGET_TESTS/
cp $P2_TESTS/test_state.mlp $TARGET_TESTS/
cp $P2_TESTS/test_panic.mlp $TARGET_TESTS/
cp $P2_TESTS/test_io.mlp $TARGET_TESTS/
cp $P2_TESTS/test_file_io.mlp $TARGET_TESTS/
cp $P2_TESTS/test_math.mlp $TARGET_TESTS/
```

### Test Adaptasyonu (LLVM → GCC):

1. **Import Path'ler:** P2'deki module paths MLP-GCC'ye uygun mu?
2. **Stdlib API:** Function signatures değişmiş mi?
3. **Type System:** P2'nin type system MLP-GCC ile uyumlu mu?

### Başarı Kriterleri:

- [ ] 7 test dosyası kopyalandı
- [ ] Test dosyaları MLP-GCC syntax'ına adapte edildi
- [ ] Test dosyaları compile ediliyor (syntax check)

---

## 📋 TASK 5.5: Entegrasyon Testi (3-4. Gün)

### Hedef:
7 stdlib modülünün MLP-GCC runtime'da çalıştığını doğrula.

### Test Scenarios:

#### Test 1: mlp_list (Dinamik koleksiyonlar)

```mlp
-- test_stdlib_list.mlp
list<numeric> numbers = [1, 2, 3, 4, 5]
print("List size:", numbers.size())
print("First:", numbers[0])
print("Last:", numbers[numbers.size() - 1])

numbers.append(6)
print("After append:", numbers)
```

**Beklenen:**
```
List size: 5
First: 1
Last: 5
After append: [1, 2, 3, 4, 5, 6]
```

#### Test 2: mlp_optional (Null safety)

```mlp
-- test_stdlib_optional.mlp
optional<string> find_user(string name) {
    if name == "ali" {
        return some("Ali Veli")
    }
    return none
}

optional<string> result = find_user("ali")
if result.has_value() {
    print("User:", result.value())
} else {
    print("User not found")
}
```

**Beklenen:**
```
User: Ali Veli
```

#### Test 3: mlp_state (Modül state management)

```mlp
-- test_stdlib_state.mlp
state AppState {
    numeric counter = 0
}

func increment() {
    AppState.counter = AppState.counter + 1
}

increment()
increment()
print("Counter:", AppState.counter)
```

**Beklenen:**
```
Counter: 2
```

#### Test 4: mlp_panic (Error handling)

```mlp
-- test_stdlib_panic.mlp
func risky_operation(numeric x) {
    if x < 0 {
        panic("Negative value not allowed!")
    }
    print("Value:", x)
}

risky_operation(5)
risky_operation(-1)  -- Should panic
```

**Beklenen:**
```
Value: 5
PANIC: Negative value not allowed!
  at risky_operation (test_stdlib_panic.mlp:3)
```

### Test Execution:

```bash
cd /home/pardus/projeler/MLP/MLP-GCC

# Compile and run each test
./melpc tests/stdlib/test_stdlib_list.mlp -o test_list
./test_list

./melpc tests/stdlib/test_stdlib_optional.mlp -o test_optional
./test_optional

./melpc tests/stdlib/test_stdlib_state.mlp -o test_state
./test_state

./melpc tests/stdlib/test_stdlib_panic.mlp -o test_panic
./test_panic
```

### Başarı Kriterleri:

- [ ] mlp_list test PASS (4/4 assertions)
- [ ] mlp_optional test PASS (3/3 assertions)
- [ ] mlp_state test PASS (2/2 assertions)
- [ ] mlp_panic test PASS (panic handler works)
- [ ] No memory leaks (valgrind clean)
- [ ] No segfaults

---

## 🎖️ 6 TEMEL ESAS KONTROLÜ

Her task sonunda kontrol et:

### 1. MODULAR (Max 500 satır/dosya)
- [ ] mlp_list.c: 328 satır ✅
- [ ] mlp_optional.c: 191 satır ✅
- [ ] mlp_state.c: 426 satır ✅
- [ ] mlp_panic.c: 103 satır ✅
- [ ] mlp_io.c: 268 satır ✅
- [ ] file_io.c: 384 satır ✅
- [ ] mlp_math.c: 243 satır ✅

### 2. GCC (C backend, no LLVM)
- [ ] Tüm modüller GCC ile derleniyor
- [ ] LLVM-specific code temizlendi

### 3. STO (Smart Type Optimization)
- [ ] Generic types (list<T>, optional<T>) çalışıyor
- [ ] Type inference doğru

### 4. STATELESS (No global state)
- [ ] mlp_state modülü stateful ama encapsulated
- [ ] Diğer modüller tamamen stateless

### 5. STRUCT+FUNC (No OOP)
- [ ] Sadece struct + function (no class, no method)

### 6. MODÜL=ŞABLON (Stateless modules)
- [ ] Her modül bağımsız (no circular deps)

---

## 📊 SÜRE TAHMİNİ

| Task | Açıklama | Süre |
|------|----------|------|
| 5.1 | P2 stdlib modüllerini kopyala | 2-4 saat |
| 5.2 | GCC uyumluluk kontrolü | 4-6 saat |
| 5.3 | Makefile güncelle | 2-3 saat |
| 5.4 | Test dosyalarını al | 3-4 saat |
| 5.5 | Entegrasyon testi | 6-8 saat |
| **Toplam** | **5 task** | **17-25 saat (3-5 gün)** |

---

## 🚨 POTANSİYEL RISKLER

### Risk 1: LLVM → GCC Uyumluluk Sorunları
- **Olasılık:** 🟡 ORTA
- **Etki:** 🔴 YÜKSEK (Task 5.2 uzayabilir)
- **Mitigation:** Önce bir modülle test et (mlp_math en basit)

### Risk 2: P2 Test Dosyaları Eksik
- **Olasılık:** 🟢 DÜŞÜK
- **Etki:** 🟡 ORTA (Task 5.4 uzayabilir)
- **Mitigation:** Manuel test yazma hazırlığı

### Risk 3: API Uyumsuzluğu (P2 vs GCC)
- **Olasılık:** 🟡 ORTA
- **Etki:** 🔴 YÜKSEK (Tüm tasklar etkilenir)
- **Mitigation:** API diff analizi yap (Task 5.1 sonunda)

---

## 📝 RAPORLAMA

### Günlük Rapor (Her gün):
- Task progress (5.1-5.5)
- Karşılaşılan sorunlar
- Çözümler
- Sonraki adım

### Final Rapor (Task 5.5 sonunda):
- Tüm 5 task tamamlandı mı?
- 6 Temel Esas uyumu (6/6)
- Test sonuçları (PASS/FAIL)
- Süre karşılaştırması (Tahmini: 3-5 gün, Gerçek: ?)
- Lessons learned

### Rapor Dosyası:
`TODO_TODO_SELFHOSTING/SELFHOSTING_YZ/YZ_15_RAPOR.md`

---

## 🎯 BAŞARI KRİTERLERİ (EXIT CRITERIA)

TODO #5 Task 5.1-5.5 tamamlandı sayılır eğer:

1. ✅ 7 stdlib modülü MLP-GCC'ye taşındı
2. ✅ Tüm modüller GCC ile derleniyor
3. ✅ Makefile güncel (STDLIB_OBJS)
4. ✅ 4 temel test PASS (list, optional, state, panic)
5. ✅ 6 Temel Esas: 6/6 (%100)
6. ✅ libmlp_runtime.a boyutu: ~60-80KB
7. ✅ Entegrasyon testi PASS (MLP compiler rebuild)

---

## 📞 İLETİŞİM

**UA_04'e Rapor Et:**
- Task tamamlandığında
- Kritik sorun çıktığında
- Tahmini süre aşıldığında (>5 gün)
- API uyumsuzluğu bulunduğunda

**Escalation Kriterleri:**
- >2 gün task gecikme
- GCC uyumluluk sorunu çözülemezse
- Test FAIL rate >30%

---

**Hazırlayan:** UA_04  
**Onaylayan:** Kullanıcı  
**Tarih:** 30 Aralık 2025, 15:00
