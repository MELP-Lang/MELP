# YZ_212 GÖREVI: Standard Library - Collections

**Tarih:** 22 Aralık 2025  
**Öncelik:** 🔴 Yüksek (temel yapılar)  
**Süre:** 1 hafta  
**Kaynak:** TODO_MODERN_LANGUAGE.md Line 595-601

---

## 🎯 GÖREV TANIMI

MELP'in standard library'sine **Collections (Koleksiyon)** modülünü ekle. List, Map, Set, Queue, Stack için temel operasyonlar.

---

## 📋 YAPILACAKLAR

### 1. List Operations (Genişletme)
Mevcut list type'ına operasyonlar ekle:
- [x] append (var) ✅
- [x] get (var) ✅
- [x] length (var) ✅
- [ ] **prepend(list, item)** - Başa ekle
- [ ] **insert(list, index, item)** - Belirli index'e ekle
- [ ] **remove(list, index)** - Index'ten sil
- [ ] **find(list, item)** - İlk bulunduğu index
- [ ] **sort(list)** - Sırala (numeric için)

### 2. Map Operations (Genişletme)
Mevcut map type'ına operasyonlar ekle:
- [x] insert (var) ✅
- [x] get (var) ✅
- [x] has_key (var) ✅
- [ ] **remove(map, key)** - Anahtar sil
- [ ] **keys(map)** - Tüm anahtarları liste olarak döndür
- [ ] **values(map)** - Tüm değerleri liste olarak döndür
- [ ] **clear(map)** - Tüm elemanları sil

### 3. Set (Yeni Tip)
Benzersiz elemanlar koleksiyonu:
- [ ] **set type** - Yeni veri yapısı
- [ ] **add(set, item)** - Eleman ekle (duplicate check)
- [ ] **contains(set, item)** - Eleman var mı?
- [ ] **remove(set, item)** - Eleman sil
- [ ] **union(set1, set2)** - Birleşim
- [ ] **intersection(set1, set2)** - Kesişim
- [ ] **size(set)** - Eleman sayısı

### 4. Queue (Yeni Tip)
FIFO (First In First Out):
- [ ] **queue type** - Yeni veri yapısı
- [ ] **enqueue(queue, item)** - Sona ekle
- [ ] **dequeue(queue)** - Baştan çıkar
- [ ] **peek(queue)** - Başa bak (çıkarmadan)
- [ ] **is_empty(queue)** - Boş mu?

### 5. Stack (Yeni Tip)
LIFO (Last In First Out):
- [ ] **stack type** - Yeni veri yapısı
- [ ] **push(stack, item)** - Üste ekle
- [ ] **pop(stack)** - Üstten çıkar
- [ ] **peek(stack)** - Üste bak (çıkarmadan)
- [ ] **is_empty(stack)** - Boş mu?

---

## 📊 SYNTAX ÖRNEKLERİ

### List Operations
```pmpl
list numbers = (1; 2; 3; 4; 5;)

-- Prepend
prepend(numbers, 0)  -- (0; 1; 2; 3; 4; 5;)

-- Insert
insert(numbers, 2, 99)  -- (0; 1; 99; 2; 3; 4; 5;)

-- Remove
remove(numbers, 2)  -- (0; 1; 2; 3; 4; 5;)

-- Find
numeric index = find(numbers, 3)  -- Returns 3

-- Sort
sort(numbers)  -- (0; 1; 2; 3; 4; 5;)
```

### Map Operations
```pmpl
map person = {"name": "Alice"; "age": 30;}

-- Remove
remove(person, "age")  -- {"name": "Alice"}

-- Keys
list keys = keys(person)  -- ("name";)

-- Values
list vals = values(person)  -- ("Alice";)

-- Clear
clear(person)  -- {}
```

### Set Operations
```pmpl
set numbers = {1; 2; 3; 2; 1;}  -- Auto-deduplicate: {1; 2; 3;}

add(numbers, 4)  -- {1; 2; 3; 4;}
boolean has = contains(numbers, 3)  -- true
remove(numbers, 2)  -- {1; 3; 4;}

set evens = {2; 4; 6;}
set odds = {1; 3; 5;}
set all = union(evens, odds)  -- {1; 2; 3; 4; 5; 6;}
```

### Queue Operations
```pmpl
queue tasks = ()

enqueue(tasks, "task1")
enqueue(tasks, "task2")
enqueue(tasks, "task3")

string first = dequeue(tasks)  -- "task1"
string next = peek(tasks)      -- "task2" (not removed)
```

### Stack Operations
```pmpl
stack history = ()

push(history, "page1")
push(history, "page2")
push(history, "page3")

string current = pop(history)  -- "page3"
string prev = peek(history)    -- "page2" (not removed)
```

---

## 🏗️ DOSYALAR

**Runtime (C Implementation):**
- `runtime/stdlib/mlp_list_ext.{h,c}` - List operations
- `runtime/stdlib/mlp_map_ext.{h,c}` - Map operations
- `runtime/stdlib/mlp_set.{h,c}` - Set implementation
- `runtime/stdlib/mlp_queue.{h,c}` - Queue implementation
- `runtime/stdlib/mlp_stack.{h,c}` - Stack implementation

**Parser (if needed):**
- `compiler/stage0/modules/list/list_operations.{h,c}` - List ops parser
- `compiler/stage0/modules/map/map_operations.{h,c}` - Map ops parser

**Tests:**
- `tests/stdlib/test_list_ops.mlp`
- `tests/stdlib/test_map_ops.mlp`
- `tests/stdlib/test_set.mlp`
- `tests/stdlib/test_queue.mlp`
- `tests/stdlib/test_stack.mlp`

---

## ⚠️ MELP MİMARİ KURALLARI

1. **Mevcut kodu kontrol et:**
   - `runtime/stdlib/` klasöründe neler var?
   - List/Map runtime zaten var mı? (Muhtemelen var)
   - Varsa genişlet, yoksa oluştur

2. **Modüler yapı:**
   - Her koleksiyon tipi kendi dosyasında
   - Merkezi bir `collections.{h,c}` YAPMA!

3. **STO prensibi:**
   - `numeric`, `string` kullan (int/float değil)
   - Generic koleksiyonlar (type-agnostic)

4. **Keyword > Symbol:**
   - `prepend`, `enqueue`, `dequeue` gibi kelimeler
   - Sembolik operatör kullanma

5. **Kritik kararlar:**
   - Yeni type eklerken (set, queue, stack) → KULLANICIYA SOR!
   - Parser değişikliği gerekirse → KULLANICIYA SOR!

---

## 📚 ZORUNLU OKUMA

Başlamadan önce oku:
1. `MELP_VISION.md` - MELP felsefesi
2. `ARCHITECTURE.md` - Modüler yapı
3. `YZ_PROTOKOL.md` - Mimari kurallar
4. `melp_modulleri.md` - Mevcut modüller
5. `runtime/stdlib/README.md` - Runtime yapısı (varsa)

---

## ✅ BAŞARI KRİTERİ

```bash
# Test 1: List operations
./melp_compiler tests/stdlib/test_list_ops.mlp -o test_list
./test_list
# Expected: prepend, insert, remove, find, sort working

# Test 2: Map operations
./melp_compiler tests/stdlib/test_map_ops.mlp -o test_map
./test_map
# Expected: remove, keys, values, clear working

# Test 3: Set
./melp_compiler tests/stdlib/test_set.mlp -o test_set
./test_set
# Expected: add, contains, union, intersection working

# Test 4: Queue
./melp_compiler tests/stdlib/test_queue.mlp -o test_queue
./test_queue
# Expected: enqueue, dequeue, peek working

# Test 5: Stack
./melp_compiler tests/stdlib/test_stack.mlp -o test_stack
./test_stack
# Expected: push, pop, peek working
```

---

## 🎯 TAMAMLAMA

1. Tüm testler ✅
2. `LLVM_YZ/YZ_212_TAMAMLANDI.md` oluştur
3. `NEXT_AI_START_HERE.md` güncelle
4. `TODO_MODERN_LANGUAGE.md` YZ_212'yi işaretle
5. Git commit + push

---

**Hazır mısın? Başlayalım!** 🚀
