# 🎯 MLP MODÜL FELSEFESİ

**Tarih:** 29 Aralık 2025  
**Konu:** "Her modül ölüdür; onu çağıran diriltir ve öldürür"  
**Durum:** ✅ ONAYLANDI - 6. TEMEL ESAS Eklendi  
**Yetki:** Üst Akıl (Danışman + Mastermind Consensus)

---

## 📜 KARAR ÖZETİ

**6. TEMEL ESAS** MLP-GCC'ye eklendi:

```
6. MODÜL=ŞABLON → Her çağrı independent instantiate
                 → Modül state tutmaz
                 → Pure functional paradigm
```

**Bu esas:** TODO #2, #5, #7'de revizyon gerektirir.

---

## 🔍 MODÜL FELSEFESİ: "HER MODÜL ÖLÜDÜR"

### Temel Prensip

```mlp
-- Her modül çağrısı BAĞIMSIZ instantiate
result1 = math.topla(2, 3)     -- math: dirildi → topladı → öldü
result2 = math.topla(5, 7)     -- YENİ instantiate, öncekinden HABERSİZ

-- Modül hiçbir şey HATIRLAMIYOR
content = file.oku("test.txt")  -- file: dirildi → okudu → öldü
```

### Ne Demek?

**❌ Modül DEĞİLDİR:**
- Persistent API (C kütüphanesi gibi)
- State tutan object
- Resource manager
- Long-running process

**✅ Modül SADECE:**
- Function template collection
- Her çağrıda fresh instantiate
- Tamamen stateless transformer
- Functional programming paradigm

---

## 📋 7 TEMEL ESAS (Güncellenmiş)

```
1. MODULAR       → Sorumluluk bazlı bölme (satır sayısı SINIRSIZ!)
                   → 1 modül = 1 sorumluluk (size irrelevant)
                   → Peer-to-peer network yapısı
2. GCC/LLVM      → MLP → C/LLVM IR → GCC/LLVM → Binary
3. STO           → Smart Type Optimization
4. STATELESS     → Global state yasak
5. STRUCT+FUNC   → OOP yok
6. MODÜL=ŞABLON  → Her çağrı independent instantiate
                   → Modül state tutmaz
                   → Pure functional paradigm
7. SAO           → Semantic-Aware Optimization
                   → Frontend semantic → Backend optimization metadata
```

**⚠️ CRITICAL: Modülerlik ≠ Dosya Bölme**
- ✅ 1 modül = 1 sorumluluk (single responsibility)
- ✅ Satır sayısı sınırı YOK (800, 1500, 2000 satır OK!)
- ❌ "500 satır kuralı" KALDIRILDI!

**⚠️ CRITICAL: SYMBOL INDEX = Kural (ESAS değil!)**
- SYMBOL INDEX: Best practice/kural (500+ satır dosyalar için)
- Context loss önleme aracı, temel prensip değil

**Bu 7 esasa aykırı her tasarım REDDEDİLİR!**

---

## ✅ ONAYLANAN PATTERN'LER

### 1. File I/O → MÜKEMMEL

```mlp
content = file.oku("test.txt")   -- open → read → close → öldü
file.yaz("out.txt", content)     -- open → write → close → öldü
```

**Kural:** Her çağrı tamamen bağımsız, modül file handle tutmaz.

---

### 2. Database → HER QUERY FRESH

```mlp
-- ✅ DOĞRU API:
result = db.query("postgres://...", "SELECT ...")
-- Modül: dirildi → connect → query → disconnect → öldü
```

**Not:** Connection pooling runtime/compiler'da optimize edilebilir ama **API'de görünmez!** Çünkü connection pool = state = YASAK!

---

### 3. Threading → FUNCTIONAL PATTERN

```mlp
-- ❌ YANLIŞ (persistent state):
mutex m = mutex.create()
mutex.lock(m)
mutex.unlock(m)

-- ✅ DOĞRU (closure pattern):
result = mutex.with_lock("my_key", lambda() =>
    -- Critical section
    counter = counter + 1
    return counter
)
-- mutex: dirildi → lock → closure çalıştı → unlock → öldü
```

**Kural:** Thread = pure function çağrısı, mutex = closure pattern.

---

### 4. Event-Driven → CALLER'IN MAİN LOOP'UNDA

```mlp
-- ✅ DOĞRU: Caller kontrol ediyor
function main()
    while true do
        event = gui.wait_event()    -- Dirildi → event aldı → öldü
        action = on_click(event)     -- Pure function
        gui.apply(action)            -- Dirildi → apply etti → öldü
    end_while
end_function
```

**Kural:** Long-running loop = caller'ın sorumluluğu, modül sadece event handler (pure).

---

## 🔧 TOOL vs MODÜL Ayrımı

### 💎 ALTIN KURAL

```
EĞER BLOCKING / LONG-RUNNING / PERSISTENT STATE
  → TOOL (Binary)
  
EĞER PURE FUNCTION / STATELESS / INDEPENDENT CALLS
  → MLP MODÜLÜ
```

### TOOL Örnekleri (Binary):

```bash
# Web server
./mlp-web-server --port 3000 app.mlp

# GUI runtime
./mlp-gui-runtime app.mlp

# LSP server
./mlp-lsp --stdio

# Package registry server
./mlp-registry-server --port 8080
```

**Bu tool'lar:** C ile yazılır, MLP felsefesine tabi değil, long-running process olabilir.

### MLP MODÜLÜ Örnekleri:

```mlp
-- Handler functions (pure)
function handle_request(req) as res
    return res.create(200, "Hello")  -- Pure function!
end_function

-- Event handlers (pure)
function on_click(event) as action
    return action.update_label("Clicked!")
end_function

-- Database query (fresh connection)
result = db.query("postgres://...", "SELECT ...")
```

**Bu modüller:** Her çağrı independent, state tutmaz, pure functional.

---

## 🚨 TODO REVİZYONU GEREKLİ

### TODO #2: STDLIB_EXPAND → REVİZE ET

**Değiştirilecekler:**

#### ❌ SİL:
```mlp
mutex m = mutex.create()      -- Persistent state YASAK!
channel ch = channel.create() -- Persistent state YASAK!
conn = db.connect(...)        -- Persistent connection YASAK!
```

#### ✅ EKLE:
```mlp
-- Functional threading
result = thread.run(worker_fn, args)
result = mutex.with_lock("key", lambda() => critical_section())

-- Database (her query fresh)
result = db.query("postgres://...", "SELECT ...")

-- File I/O (zaten doğru)
content = file.oku("test.txt")
```

---

### TODO #5: PACKAGE_MANAGER → ONAY

- ✅ `mlp-pkg` CLI → Her komut independent (OK)
- 🔧 Registry server → TOOL (binary: `mlp-registry-server`)

**Not:** Registry server MLP modülü değil, C ile yazılacak binary tool.

---

### TODO #7: ECOSYSTEM → REVİZE ET

#### ❌ SİL: Web/GUI framework MLP modülü olarak

```mlp
-- ❌ YANLIŞ:
web.listen(app, 3000)  -- Blocking, long-running
gui.main_loop()        -- Blocking, long-running
```

#### ✅ DEĞİŞTİR:

**1. Web Framework:**
- **Handler functions:** MLP modülü (pure)
  ```mlp
  function handle_request(req) as res
      return res.create(200, "OK")
  end_function
  ```
- **Server:** TOOL (binary: `mlp-web-server`)
  ```bash
  ./mlp-web-server --port 3000 app.mlp
  ```

**2. GUI Framework:**
- **Event handlers:** MLP modülü (pure)
  ```mlp
  function on_click(event) as action
      return action.update_label("Clicked!")
  end_function
  ```
- **Event loop:** TOOL (binary: `mlp-gui-runtime`)
  ```bash
  ./mlp-gui-runtime app.mlp
  ```

**3. Test Framework:**
- ✅ Test = pure function (OK, değişiklik yok)

---

## 📊 TODO UYGUNLUK ANALİZİ

| TODO | Durum | Aksiyon |
|------|-------|---------|
| #0: SELFHOSTING | ✅ UYGUN | Değişiklik yok |
| #1: TOOLING_BASICS | ✅ UYGUN | Değişiklik yok |
| #2: STDLIB_EXPAND | ⚠️ PROBLEMATİK | Threading/DB API revize et |
| #3: LANGUAGE_FEATURES | ✅ UYGUN | Değişiklik yok |
| #4: IDE_INTEGRATION | ✅ UYGUN | Değişiklik yok |
| #5: PACKAGE_MANAGER | ⚠️ KISMEN | Registry = tool olarak işaretle |
| #6: OPTIMIZATION | ✅ UYGUN | Değişiklik yok |
| #7: ECOSYSTEM | ⚠️ PROBLEMATİK | Web/GUI framework revize et |

---

## 📝 MASTERMIND TALİMATI

**Öncelik:** 🔴 YÜKSEK

### Adım 1: TODO Revizyonları
1. TODO #2 → Threading/Database API'lerini functional pattern'e çevir
2. TODO #5 → Registry server'ı TOOL olarak işaretle
3. TODO #7 → Web/GUI framework'ü TOOL + MODÜL olarak ayır

### Adım 2: Dokümantasyon
1. Danışman_Yol_Haritası.md → 6. TEMEL ESAS ekle
2. YZ_KURALLAR.md → Modül felsefesi ekle
3. ARCHITECTURE_AND_MEMORY_STRATEGY.md → MODÜL=ŞABLON bölümü ekle

### Adım 3: Örnekler
1. `examples/threading_functional.mlp` oluştur
2. `examples/database_fresh_query.mlp` oluştur
3. `examples/event_handler_pattern.mlp` oluştur

---

## 🎯 BAŞARI KRİTERLERİ

- [ ] TODO #2, #5, #7 revize edildi
- [ ] 6. TEMEL ESAS dokümantasyona eklendi
- [ ] Functional pattern örnekleri oluşturuldu
- [ ] TOOL vs MODÜL ayrımı net tanımlandı
- [ ] Tüm persistent state pattern'leri kaldırıldı

---

## 💬 DANIŞMAN ONAYI

**Danışman'a:**

Bu karar **5 Temel Esas'a 6. Esas ekliyor**. Onay durumunuz:

- [ ] ✅ ONAYLADIM - 6 TEMEL ESAS yürürlükte
- [ ] ❌ REDDEDİYORUM - Revizyon gerekli (neden: _______)

**İmza:** _____________  
**Tarih:** _____________

---

## 🔗 İLGİLİ DOSYALAR

- [Danışman_Yol_Haritası.md](Danışman_Yol_Haritası.md) - 5 Temel Esas (6. eklenecek)
- [UST_AKIL_KARAR_01_Unified_Vision.md](UST_AKIL_KARAR_01_Unified_Vision.md) - Önceki karar
- [0-TODO_SELFHOSTING.md](0-TODO_SELFHOSTING.md) - Aktif TODO

---

## 🎉 ÖZET

**6. TEMEL ESAS:** MODÜL=ŞABLON

**Kural:** Her modül çağrısı independent, state tutmaz, pure functional.

**Sonuç:** TODO #2, #5, #7 revize edilecek.

**Durum:** ✅ ONAYLANDI

---

*Bu belge Üst Akıl tarafından onaylanmıştır ve 6 Temel Esas'a dahil edilmiştir.*

**Versiyon:** 1.0  
**Son Güncelleme:** 29 Aralık 2025  
**Onay:** Üst Akıl KARAR_02
