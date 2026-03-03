# IF_LET - Özellik Kartı

**Özellik:** `if let` / `while let` — Pattern matching ile değişken bağlama kısa formu  
**Durum:** 📅 PLANNED (Week 9.2, Ağustos 2026)  
**Hafta:** Week 9.2  
**Sözdizimi Sürümü:** pmlp2

---

## 🎯 ÖZELLİK

`match` bloğunun tek `case` olan kısa formu. `Option{T}` veya `Result{T;E}` değerlerini tek satırda güvenli şekilde çözer. Mevcut durumda `match` kullanmak gerekiyor.

---

## 📖 SÖZDİZİMİ

```pmlp
-- if let (hedef sözdizimi, Week 9.2)
if let Some(user) = find_user("Ali") then
    print("Bulundu: " + user.name)
else
    print("Bulunamadı")
end_if

-- while let (iterasyon için)
while let Some(line) = read_next_line(reader)
    process(line)
end_while

-- Result ile
if let Ok(value) = parse_number("42") then
    print("Sayı: " + value)
else
    print("Geçersiz sayı")
end_if
```

**Normalizer Dönüşümü:**
```pmlp
-- if let Some(v) = expr then ... else ... end_if
-- →
match expr
    case Some(v):
        ...
    else
        ...
end_match
```

**Mevcut Durum (Week 9.2'ye kadar) — YZ DİKKAT:**
```pmlp
-- ⚠️ Henüz ÇALIŞMIYOR — match kullan:
Option{string} user = find_user("Ali")
match user
    case Some(u):
        print("Bulundu: " + u)
    case None:
        print("Bulunamadı")
end_match
```

---

## 🧪 GOLDEN TEST

```pmlp
function find_even(numeric[] arr) as Option{numeric}
    for each n in arr
        if n mod 2 == 0 then
            return Some(n)
        end_if
    end_for
    return None
end_function

function main() as numeric
    -- Week 9.2 sonrası:
    -- if let Some(n) = find_even([1;3;4;5]) then
    --     if n == 4 then return 1 end_if
    -- end_if
    Option{numeric} result = find_even([1; 3; 4; 5])
    match result
        case Some(n):
            if n == 4 then return 1 end_if
        case None:
            return 0
    end_match
    return 0
end_function
```

---

**Bağımlılıklar:** Option{T}, Result{T;E}, Pattern Matching  
**Hafta:** Week 9.2 (Ağustos 2026)  
**Referans Diller:** Rust `if let`, Swift `if let`, Kotlin `?.let { }`
