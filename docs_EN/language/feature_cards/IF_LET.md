# IF_LET - Özellik Kartı

**Özellik:** `if let` / `loop let` — Pattern matching ile değişken bağlama kısa formu  
**Durum:** 📅 PLANNED (Week 9.2, Ağustos 2026)  
**Hafta:** Week 9.2  
**Sözdizimi Sürümü:** mlp2

---

## 🎯 ÖZELLİK

`match` bloğunun tek `case` olan kısa formu. `Option{T}` veya `Result{T;E}` değerlerini tek satırda güvenli şekilde çözer. Mevcut durumda `match` kullanmak gerekiyor.

---

## 📖 SÖZDİZİMİ

```mlp
-- if let (hedef sözdizimi, Week 9.2)
if let Some(user) = find_user("Ali") then
    print("Bulundu: " + user.name)
else
    print("Bulunamadı")
end if

-- loop let (iterasyon için)
loop let Some(line) = read_next_line(reader)
    process(line)
end loop

-- Result ile
if let Ok(value) = parse_number("42") then
    print("Sayı: " + value)
else
    print("Geçersiz sayı")
end if
```

**Normalizer Dönüşümü:**
```mlp
-- if let Some(v) = expr then ... else ... end if
-- →
match expr
    case Some(v):
        ...
    else
        ...
end match
```

**Mevcut Durum (Week 9.2'ye kadar) — YZ DİKKAT:**
```mlp
-- ⚠️ Henüz ÇALIŞMIYOR — match kullan:
Option{string} user = find_user("Ali")
match user
    case Some(u):
        print("Bulundu: " + u)
    case None:
        print("Bulunamadı")
end match
```

---

## 🧪 GOLDEN TEST

```mlp
Option{numeric} function find_even(numeric[] arr)
    loop each n in arr
        if n mod 2 == 0 then
            return Some(n)
        end if
    end loop
    return None
end function

function main()
    -- Week 9.2 sonrası:
    -- if let Some(n) = find_even([1;3;4;5]) then
    --     if n == 4 then return 1 end if
    -- end if
    Option{numeric} result = find_even([1; 3; 4; 5])
    match result
        case Some(n):
            if n == 4 then return 1 end if
        case None:
    end match
end function
```

---

**Bağımlılıklar:** Option{T}, Result{T;E}, Pattern Matching  
**Hafta:** Week 9.2 (Ağustos 2026)  
**Referans Diller:** Rust `if let`, Swift `if let`, Kotlin `?.let { }`
