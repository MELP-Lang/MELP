# NAMED_PARAMS - Özellik Kartı

**Özellik:** `greet(name: "Ali"; age: 25)` — İsimli parametreler  
**Durum:** ⚠️ PARTIAL (Stage 1 — parse edilir, sıra bağımsızlığı eksik)  
**Hafta:** Stage 1  
**Sözdizimi Sürümü:** mlp1

---

## 🎯 ÖZELLİK

Çağrı sırasında parametre ismini belirterek argüman geçme. Okunabilirliği artırır ve uzun parametre listelerinde sıra karışıklığını önler. Swift / Python keyword argument tarzı.

---

## 📖 SÖZDİZİMİ

```mlp
-- Fonksiyon tanımı (normal)
User function create_user(string name; numeric age; boolean active)
    -- gövde
end function

-- İsimli parametre ile çağrı
User u = create_user(name: "Ali"; age: 25; active: true)

-- Kısmi isimli (ilk positional, sonrası named)
User u = create_user("Ali"; age: 25; active: true)

-- Varsayılan değer ile isimli (planlı)
-- Connection function connect(string host; numeric port: 8080)
-- Connection c = connect(host: "localhost")  -- port = 8080

-- Hedef: sıra bağımsız (HENÜZ TAM ÇALIŞMIYOR)
User u = create_user(active: true; name: "Ali"; age: 25)
```

**Token:**
| Token | Açıklama |
|-------|----------|
| `name:` çağrı bağlamında | `TOKEN_NAMED` — parametre ismi belirteci |

**Mevcut Durum — YZ DİKKAT:**
```mlp
-- ⚠️ Sıra bağımsızlığı henüz tam çalışmıyor
-- Güvenli kullanım: tanım sırasıyla isimli parametre
-- Alternatif: positional (sırasıyla)
User u = create_user("Ali"; 25; true)
```

**Varsayılan Değerler (Planlı):**
```mlp
-- Hedef sözdizimi (henüz yok):
-- void function log(string msg; string level: "INFO")
-- log("Başladı")              -- level = "INFO"
-- log("Hata!"; level: "ERR")  -- level = "ERR"
```

---

## 🧪 GOLDEN TEST

```mlp
numeric function rect_area(numeric width; numeric height)
    return width * height
end function

function main()
    numeric area = rect_area(width: 5; height: 3)
    if area == 15 then return 1 end if
end function
```

---

**Bağımlılıklar:** Fonksiyon tanımları  
**Eksik:** Sıra bağımsız çağrı, varsayılan değerler  
**Hafta:** Stage 1  
**Referans Diller:** Swift named labels, Python kwargs, Kotlin named args  
**Kanonik Kaynak:** `mlp1_sozdizimi.md` — Fonksiyon parametreleri
