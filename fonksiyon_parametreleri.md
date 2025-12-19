# 📋 Fonksiyon Parametreleri - MELP/PMPL

**Son Güncelleme:** 19 Aralık 2025 (YZ_30/YZ_31)

---

## 🎯 Modern Programlama Dillerinde Parametre Türleri

### 1. Temel Tipler
```
function test(
    numeric x,              // Tam sayı veya ondalık
    string s,               // Metin
    boolean b,              // true/false
    char c,                 // Tek karakter
    byte raw                // Ham byte
)
```
| MELP Durumu | Stage 1 Gerekli |
|-------------|-----------------|
| ✅ Var (numeric, string, boolean) | ✅ Evet |
| ❌ char, byte yok | ❌ Hayır |

---

### 2. Varsayılan Değerler (Default Values)
```
function test(
    numeric x := 10,        // := ile varsayılan
    string s := "hello",    
    boolean b := true,
    numeric precision := 5.14
)
```
| MELP Durumu | Stage 1 Gerekli |
|-------------|-----------------|
| ❌ YOK | 🟡 Opsiyonel |

**Öneri:** `:=` operatörü varsayılan değer için kullanılabilir.

---

### 3. Referans vs Değer (Reference vs Value)
```
function test(
    numeric x,              // By value (kopya)
    ref numeric y,          // By reference (orijinal değişir)
    const numeric z,        // Sabit (değiştirilemez)
    const ref numeric w     // Sabit referans
)
```
| MELP Durumu | Stage 1 Gerekli |
|-------------|-----------------|
| ✅ By value (varsayılan) | ✅ Evet |
| ❌ ref keyword yok | 🟡 Stage 1.5 |
| ❌ const keyword yok | 🟡 Stage 1.5 |

---

### 4. Array/List Parametreleri
```
function test(
    numeric[] arr,          // Dinamik boyutlu array
    numeric[10] fixed,      // Sabit boyutlu array
    string[] names,         // String listesi
    numeric[][] matrix      // 2D array
)
```
| MELP Durumu | Stage 1 Gerekli |
|-------------|-----------------|
| ✅ Array var (`array numbers`) | ✅ Evet |
| ✅ List var (`list items`) | ✅ Evet |
| ❌ 2D array kısmi | ❌ Hayır |

---

### 5. Nullable/Optional
```
function test(
    numeric? x,             // Nullable (none olabilir)
    string? name := none,   // Optional + nullable
    optional numeric y      // Açık optional keyword
)
```
| MELP Durumu | Stage 1 Gerekli |
|-------------|-----------------|
| ❌ YOK | ❌ Hayır |

**Öneri:** `?` suffix nullable için, `optional` keyword opsiyonel için.

---

### 6. Range/Precision Kısıtlamaları
```
function test(
    numeric x [0..100],           // Range constraint
    numeric y {5,14},             // Precision (5 digit, 14 decimal)
    string s [1..255],            // String length constraint
    numeric port [1..65535]       // Valid port range
)
```
| MELP Durumu | Stage 1 Gerekli |
|-------------|-----------------|
| ❌ YOK | ❌ Hayır (Stage 2+) |

**Kullanıcı Örneği:** `numeric 5,14` → 5 basamak, 14 ondalık precision

---

### 7. Variadic (Değişken Sayıda)
```
function test(
    numeric first,
    numeric... rest,        // Sıfır veya daha fazla
    string+ names           // Bir veya daha fazla
)
```
| MELP Durumu | Stage 1 Gerekli |
|-------------|-----------------|
| ❌ YOK | ❌ Hayır |

---

### 8. Named Parameters (İsimli Parametreler)
```
function test(
    named string title,     // Çağrıda isim zorunlu
    numeric x
)
// Çağrı: test(title: "Hello", 5)
```
| MELP Durumu | Stage 1 Gerekli |
|-------------|-----------------|
| ❌ YOK | ❌ Hayır |

---

### 9. Positional Optional (Sıralı Opsiyonel) - Dart Stili
```
function test(
    string a,                     // Zorunlu
    [numeric b := 10],            // Opsiyonel, varsayılan 10
    [numeric c := 20]             // Opsiyonel, varsayılan 20
)
// Çağrı: test("hi")          → b=10, c=20
// Çağrı: test("hi", 5)       → b=5, c=20
// Çağrı: test("hi", 5, 15)   → b=5, c=15
```
| MELP Durumu | Stage 1 Gerekli |
|-------------|-----------------|
| ❌ YOK | 🟡 Opsiyonel |

---

### 10. Generic/Template
```
function test<T>(
    T value,                // Generic tip
    T[] items,
    list<T> collection
)
```
| MELP Durumu | Stage 1 Gerekli |
|-------------|-----------------|
| ❌ YOK | ❌ Hayır (Stage 2+) |

---

### 11. Callback/Function Parametresi
```
function test(
    func(numeric) -> numeric transformer,   // Fonksiyon parametresi
    func(string, string) -> boolean comparator,
    action callback                          // void dönen fonksiyon
)
```
| MELP Durumu | Stage 1 Gerekli |
|-------------|-----------------|
| ❌ YOK | ❌ Hayır (Stage 2+) |

---

### 12. Struct/Record Parametresi
```
function test(
    Point p,               // Struct tipi
    {name: string, age: numeric} inline,  // Inline struct
    Point& point           // Struct referansı
)
```
| MELP Durumu | Stage 1 Gerekli |
|-------------|-----------------|
| ✅ Struct param var (YZ_84) | ✅ Evet |
| ❌ Inline struct yok | ❌ Hayır |

---

### 13. Enum Parametresi
```
function test(
    Color c,                // Enum tipi
    Direction dir := Direction.North
)
```
| MELP Durumu | Stage 1 Gerekli |
|-------------|-----------------|
| 🟡 Kısmi (enum tanımı var) | 🟡 Opsiyonel |

---

### 14. Union/Either Tipi
```
function test(
    numeric | string value,     // Ya numeric ya string
    Result<numeric, Error> res  // Success veya Error
)
```
| MELP Durumu | Stage 1 Gerekli |
|-------------|-----------------|
| ❌ YOK | ❌ Hayır (Stage 2+) |

---

### 15. Output Parametresi
```
function test(
    numeric input,
    out numeric result,          // Çıkış parametresi
    out numeric error_code       // Birden fazla değer döndürme
)
```
| MELP Durumu | Stage 1 Gerekli |
|-------------|-----------------|
| ❌ YOK | ❌ Hayır |

---

## 📊 ÖZET TABLOSU

| # | Özellik | MELP'te Var mı? | Stage 1 Gerekli mi? |
|---|---------|-----------------|---------------------|
| 1 | Temel Tipler | ✅ Evet | ✅ Evet |
| 2 | Varsayılan Değerler (`:=`) | ❌ Hayır | 🟡 Opsiyonel |
| 3 | ref/const | ❌ Hayır | 🟡 Stage 1.5 |
| 4 | Array/List | ✅ Evet | ✅ Evet |
| 5 | Nullable (`?`) | ❌ Hayır | ❌ Hayır |
| 6 | Range/Precision | ❌ Hayır | ❌ Hayır |
| 7 | Variadic (`...`) | ❌ Hayır | ❌ Hayır |
| 8 | Named Parameters | ❌ Hayır | ❌ Hayır |
| 9 | Positional Optional (`[]`) | ❌ Hayır | 🟡 Opsiyonel |
| 10 | Generic (`<T>`) | ❌ Hayır | ❌ Hayır |
| 11 | Callback/Function | ❌ Hayır | ❌ Hayır |
| 12 | Struct Param | ✅ Evet (YZ_84) | ✅ Evet |
| 13 | Enum Param | 🟡 Kısmi | 🟡 Opsiyonel |
| 14 | Union/Either | ❌ Hayır | ❌ Hayır |
| 15 | Out Param | ❌ Hayır | ❌ Hayır |

---

## 🎯 STAGE 1 İÇİN GEREKLİ OLANLAR

### Zorunlu (Zaten Var):
1. ✅ Temel tipler: `numeric`, `string`, `boolean`
2. ✅ Array/List: `array numbers`, `list items`
3. ✅ Struct parametresi: `Point p` (YZ_84)

### Stage 1 İçin Öncelikli Eklenecekler:
1. 🟡 **Varsayılan değerler** - Bootstrap için faydalı
2. 🟡 **Positional optional** - Dart stili `[param := default]`

### Stage 1.5+ İçin Bırakılacaklar:
- ref/const
- Generic
- Callback
- Union

---

## 📝 MEVCUT MELP PARAMETRE SYNTAX'I

```pmpl
-- Eski syntax (type param)
function test(numeric x, string y) -> numeric
    return x
end function

-- Yeni syntax (param: type) - YZ_15
function test(x: numeric, y: string) -> numeric
    return x
end function

-- Struct parametresi - YZ_84
function distance(Point p1, Point p2) -> numeric
    return 0
end function
```

### Desteklenen Tipler:
- `numeric` → FUNC_PARAM_NUMERIC
- `string` → FUNC_PARAM_TEXT
- `boolean` → FUNC_PARAM_BOOLEAN
- `list` → FUNC_PARAM_LIST
- `StructName` → FUNC_PARAM_STRUCT (YZ_84)

---

## 🚀 ÖNERİLEN SYNTAX GENİŞLETMELERİ

### 1. Varsayılan Değerler
```pmpl
function greet(string name := "World") -> string
    return "Hello, " + name
end function

greet()           -- "Hello, World"
greet("MELP")     -- "Hello, MELP"
```

### 2. Precision Syntax (Kullanıcı İsteği)
```pmpl
function calculate(numeric {5,14} value) -> numeric
    -- 5 basamak, 14 ondalık precision
    return value * 2
end function
```

### 3. Output Parametresi (Gelecek)
```pmpl
function divide(numeric a, numeric b, out numeric result, out boolean success) -> none
    if b == 0 then
        success := false
        result := 0
    else
        success := true
        result := a / b
    end if
end function

-- Çağrı
numeric sonuc
boolean basarili
divide(10, 3, sonuc, basarili)
```
