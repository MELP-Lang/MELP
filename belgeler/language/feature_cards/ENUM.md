# ENUM - Özellik Kartı

**Özellik:** Enumeration Types — Type-safe named constant grupları  
**Durum:** ✅ COMPLETE (Stage 1 Phase 1)  
**Hafta:** Week 5.x  
**Sözdizimi Sürümü:** pmlp1

---

## 🎯 ÖZELLİK

Enum, compile-time'da numeric sabitlere eşlenen isimsiz sabitler grubudur. Değerler 0'dan başlayarak otomatik numaralandırılır, erişim `EnumName.VALUE` notasyonuyla yapılır.

---

## 📖 SÖZDİZİMİ

### Basit Enum
```pmlp
enum Color
    RED
    GREEN
    BLUE
end_enum

-- RED=0, GREEN=1, BLUE=2
numeric c = Color.RED

if c == Color.GREEN then
    print("yeşil")
end_if
```

### Enum ile Match
```pmlp
enum Status
    PENDING
    ACTIVE
    COMPLETED
end_enum

function describe(numeric s) as string
    match s
        case Status.PENDING then return "Bekliyor"
        case Status.ACTIVE then return "Aktif"
        case Status.COMPLETED then return "Tamamlandı"
        else return "Bilinmiyor"
    end_match
end_function

print(describe(Status.ACTIVE))  -- "Aktif"
```

### Stdlib Enum'lar (Option, Result)
```pmlp
-- Option{T} — stdlib'de tanımlı
Option{string} user = find_user("Ali")
match user
    case Some(u):
        print(u)
    case None:
        print("bulunamadı")
end_match

-- Result{T;E} — stdlib'de tanımlı
Result{numeric; string} r = divide(10; 0)
match r
    case Ok(v): print(v)
    case Err(e): print(e)
end_match
```

---

## 🧪 GOLDEN TEST

```pmlp
enum Day
    MON
    TUE
    WED
end_enum

function main() as numeric
    numeric d = Day.WED
    if d == 2 then  -- WED=2
        return 1    -- PASS
    end_if
    return 0  -- FAIL
end_function
```

---

**Bağımlılıklar:** Yok  
**Hafta:** Stage 1 Phase 1  
**Kanonik Kaynak:** `belgeler/pmlp1_sozdizimi.md` (Enum bölümü)  
**Impl:** `compiler/stage1/`
