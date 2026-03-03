# NAMED_PARAMS - Özellik Kartı

**Özellik:** `greet(name: "Ali"; age: 25)` — İsimli parametreler  
**Durum:** ⚠️ PARTIAL (Stage 1 — parse edilir, sıra bağımsızlığı eksik)  
**Hafta:** Stage 1  
**Sözdizimi Sürümü:** pmlp1

---

## 🎯 ÖZELLİK

Çağrı sırasında parametre ismini belirterek argüman geçme. Okunabilirliği artırır ve uzun parametre listelerinde sıra karışıklığını önler. Swift / Python keyword argument tarzı.

---

## 📖 SÖZDİZİMİ

```pmlp
-- Fonksiyon tanımı (normal)
function create_user(string name; numeric age; boolean active) as User
    -- gövde
end_function

-- İsimli parametre ile çağrı
User u = create_user(name: "Ali"; age: 25; active: true)

-- Kısmi isimli (ilk positional, sonrası named)
User u = create_user("Ali"; age: 25; active: true)

-- Varsayılan değer ile isimli (planlı)
-- function connect(string host; numeric port: 8080) as Connection
-- Connection c = connect(host: "localhost")  -- port = 8080

-- Hedef: sıra bağımsız (HENÜZ TAM ÇALIŞMIYOR)
User u = create_user(active: true; name: "Ali"; age: 25)
```

**Token:**
| Token | Açıklama |
|-------|----------|
| `name:` çağrı bağlamında | `TOKEN_NAMED` — parametre ismi belirteci |

**Mevcut Durum — YZ DİKKAT:**
```pmlp
-- ⚠️ Sıra bağımsızlığı henüz tam çalışmıyor
-- Güvenli kullanım: tanım sırasıyla isimli parametre
-- Alternatif: positional (sırasıyla)
User u = create_user("Ali"; 25; true)
```

**Varsayılan Değerler (Planlı):**
```pmlp
-- Hedef sözdizimi (henüz yok):
-- function log(string msg; string level: "INFO") as void
-- log("Başladı")              -- level = "INFO"
-- log("Hata!"; level: "ERR")  -- level = "ERR"
```

---

## 🧪 GOLDEN TEST

```pmlp
function rect_area(numeric width; numeric height) as numeric
    return width * height
end_function

function main() as numeric
    numeric area = rect_area(width: 5; height: 3)
    if area == 15 then return 1 end_if
    return 0
end_function
```

---

**Bağımlılıklar:** Fonksiyon tanımları  
**Eksik:** Sıra bağımsız çağrı, varsayılan değerler  
**Hafta:** Stage 1  
**Referans Diller:** Swift named labels, Python kwargs, Kotlin named args  
**Kanonik Kaynak:** `pmlp1_sozdizimi.md` — Fonksiyon parametreleri
