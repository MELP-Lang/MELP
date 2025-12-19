# PMPL SÖZDİZİMİ TUTARSIZLIKLARI - ÇÖZÜLDÜ

**Kaynak:** 4 belge karşılaştırması  
**Tarih:** 19 Aralık 2025  
**Durum:** ✅ TÜM TUTARSIZLIKLAR ÇÖZÜLDÜ

---

## ✅ ÇÖZÜLEN TUTARSIZLIKLAR

### 1. While/For döngüsünde `do` keyword'ü
**KARAR:** `do` keyword'ü **YOK**
```pmpl
while condition
    statement
end_while
```

### 2. Parametre Ayırıcı
**KARAR:** Her yerde `;` (noktalı virgül)
- Neden: MELP'te `345,95` ondalık sayıdır
- Örnek: `func(345,95; true; "OK")`

### 3. Standalone exit/continue
**KARAR:** TOKEN_EXIT ve TOKEN_CONTINUE **VAR**
- Stage 2'de çok dilli syntax için gerekli
- Diğer dillerdeki `break`/`continue` karşılığı

### 4. Enum örneği
**KARAR:** `pmlp_kesin_sozdizimi.md`'ye eklendi
```pmpl
enum Color
    RED
    GREEN
    BLUE
end_enum
```

### 5. kurallar_kitabı_v1.md durumu
**KARAR:** Artık kullanım dışı
- MLP ve PMPL karışık anlatıyordu
- YZ'lerin kafasını karıştırıyordu
- `pmlp_kesin_sozdizimi.md` tek referans olacak

### 6. Return type syntax
**KARAR:** Hem `returns` hem `as` kabul edilir
```pmpl
function test() returns numeric
function test() as numeric
```

---

## 📋 KESİN REFERANS

Artık tek referans belgesi: **`pmlp_kesin_sozdizimi.md`**

---

**Tarih:** 19 Aralık 2025  
**Durum:** ✅ TAMAMLANDI
