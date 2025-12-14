# PMPL (Pragmatik MLP) Sözdizimi Referansı

**Versiyon:** 1.0  
**Tarih:** 14 Aralık 2025  
**Durum:** Resmi Spesifikasyon

---

## 📖 PMPL Nedir?

**PMPL (Pragmatic MLP)**, MELP derleyicilerinin çalıştığı dahili temsil dilidir. Kullanıcıya sunulan kod (herhangi bir desteklenen dil/stilde olabilir) ile derleyicinin lexer/parser'ı arasında **normalize edilmiş ara sözdizimi** görevi görür.

### Tasarım Felsefesi

> "MELP derleyicisi sadece PMPL görür. Bu, dil tasarımında tutarlılığı sağlar."  
> — MELP Mimari Prensipleri

**Temel Prensipler:**
1. **Tek Doğruluk Kaynağı:** Tüm sözdizimi varyasyonları PMPL'ye normalize edilir
2. **Lexer Basitliği:** PMPL tek-token anahtar kelimeler kullanır (alt çizgi ile ayrılmış)
3. **Parser Netliği:** Pattern matching gerekmez; basit switch-case ifadeleri
4. **Self-Hosting Hazırlığı:** PMPL derleyicisi PMPL kodunu derleyebilir

### Mimari Akış

```
[Kullanıcı Kodu] → [Normalizer] → [PMPL] → [Lexer] → [Parser] → [Codegen]
      (any)        (syntax.json)   (bu)    (tokenlar)  (AST)      (assembly)
```

---

## 🎯 PMPL Sözdizimi Genel Bakış

### Blok Sonlandırıcılar

Tüm blok-bitirme anahtar kelimeleri **alt çizgi** notasyonu kullanır:

```pmpl
-- If ifadesi
if koşul then
    ifade
end_if          -- Tek token: TOKEN_END_IF

-- While döngüsü
while koşul do
    ifade
end_while       -- Tek token: TOKEN_END_WHILE

-- For döngüsü
for i = 1 to 10 do
    ifade
end_for         -- Tek token: TOKEN_END_FOR

-- Fonksiyon
function isim() returns numeric
    ifade
end_function    -- Tek token: TOKEN_END_FUNCTION

-- Yapı (Struct)
struct Nokta
    numeric x
    numeric y
end_struct      -- Tek token: TOKEN_END_STRUCT
```

### Kontrol Akışı

```pmpl
-- Else if (DEĞİL "else if" - alt çizgi!)
if x > 5 then
    print("Büyük")
else_if x > 2 then    -- TOKEN_ELSE_IF
    print("Orta")
else
    print("Küçük")
end_if
```

### Döngü Kontrolü

```pmpl
-- Exit (break karşılığı)
while koşul do
    if çıkış_koşulu then
        exit        -- Mevcut döngüden çık (standalone)
    end_if
end_while

-- Belirli döngü çıkışı
exit_for        -- TOKEN_EXIT_FOR
exit_while      -- TOKEN_EXIT_WHILE
exit_if         -- TOKEN_EXIT_IF (erken if çıkışı)
exit_function   -- TOKEN_EXIT_FUNCTION (erken return)
exit_switch     -- TOKEN_EXIT_SWITCH (switch'ten çık)

-- Continue (devam)
while koşul do
    if atla_koşulu then
        continue    -- Mevcut döngüye devam (standalone)
    end_if
end_while

-- Belirli döngü devamı
continue_for    -- TOKEN_CONTINUE_FOR
continue_while  -- TOKEN_CONTINUE_WHILE
```

### Debug Anahtar Kelimeleri

```pmpl
-- Debug özellikleri (--debug flag ile aktif)
debug_print(değişken)    -- TOKEN_DEBUG_PRINT - Debug bilgisi yazdır
debug_pause              -- TOKEN_DEBUG_PAUSE - Çalışmayı duraklat
debug_label @isim        -- TOKEN_DEBUG_LABEL - Debug etiketi belirle
debug_goto @etiket       -- TOKEN_DEBUG_GOTO - Debug etiketine atla

-- Örnek
function hesapla(numeric x) returns numeric
    debug_print(x)         -- --debug aktifken x değerini yazdır
    debug_label @baslangic -- Debug noktası işaretle
    
    if x < 0 then
        debug_pause        -- İnceleme için duraklat
        return 0
    end_if
    
    return x * 2
end_function
```

---

## 📋 Eksiksiz Token Referansı

### Anahtar Kelimeler (Alfabetik)

| PMPL Anahtar Kelime | Token | Açıklama |
|---------------------|-------|----------|
| `and` | TOKEN_AND | Mantıksal VE |
| `array` | TOKEN_ARRAY | Dizi tipi |
| `boolean` | TOKEN_BOOLEAN | Boolean tipi |
| `continue` | TOKEN_CONTINUE | Döngüye devam (standalone) |
| `continue_for` | TOKEN_CONTINUE_FOR | For döngüsüne devam |
| `continue_while` | TOKEN_CONTINUE_WHILE | While döngüsüne devam |
| `debug_goto` | TOKEN_DEBUG_GOTO | Debug goto etiketi |
| `debug_label` | TOKEN_DEBUG_LABEL | Debug etiket işaretçisi |
| `debug_pause` | TOKEN_DEBUG_PAUSE | Debug çalışmayı duraklat |
| `debug_print` | TOKEN_DEBUG_PRINT | Debug değer yazdır |
| `do` | TOKEN_DO | Döngü gövdesi işaretçisi |
| `downto` | TOKEN_DOWNTO | For döngüsü azalan |
| `each` | TOKEN_EACH | For-each döngüsü |
| `else` | TOKEN_ELSE | Else cümlesi |
| `else_if` | TOKEN_ELSE_IF | Else-if cümlesi |
| `enum` | TOKEN_ENUM | Enum bildirimi |
| `end_enum` | TOKEN_END_ENUM | Enum bitişi |
| `end_for` | TOKEN_END_FOR | For döngüsü bitişi |
| `end_function` | TOKEN_END_FUNCTION | Fonksiyon bitişi |
| `end_if` | TOKEN_END_IF | If ifadesi bitişi |
| `end_match` | TOKEN_END_MATCH | Match ifadesi bitişi |
| `end_operator` | TOKEN_END_OPERATOR | Operatör tanımı bitişi |
| `end_struct` | TOKEN_END_STRUCT | Struct bitişi |
| `end_switch` | TOKEN_END_SWITCH | Switch ifadesi bitişi |
| `end_try` | TOKEN_END_TRY | Try-catch bloğu bitişi |
| `end_while` | TOKEN_END_WHILE | While döngüsü bitişi |
| `exit` | TOKEN_EXIT | Bloktan çık |
| `exit_for` | TOKEN_EXIT_FOR | For döngüsünden çık |
| `exit_function` | TOKEN_EXIT_FUNCTION | Fonksiyondan çık |
| `exit_if` | TOKEN_EXIT_IF | If ifadesinden çık |
| `exit_switch` | TOKEN_EXIT_SWITCH | Switch'ten çık |
| `exit_while` | TOKEN_EXIT_WHILE | While döngüsünden çık |
| `false` | TOKEN_FALSE | Boolean yanlış |
| `for` | TOKEN_FOR | For döngüsü |
| `function` | TOKEN_FUNCTION | Fonksiyon bildirimi |
| `if` | TOKEN_IF | If ifadesi |
| `import` | TOKEN_IMPORT | Modül içe aktarma |
| `in` | TOKEN_IN | For-each iteratörü |
| `list` | TOKEN_LIST | Liste tipi |
| `mod` | TOKEN_MOD | Modulo operatörü |
| `module` | TOKEN_MODULE | Modül bildirimi |
| `not` | TOKEN_NOT | Mantıksal DEĞİL |
| `numeric` | TOKEN_NUMERIC | Sayısal tip |
| `or` | TOKEN_OR | Mantıksal VEYA |
| `print` | TOKEN_PRINT | Yazdırma ifadesi |
| `println` | TOKEN_PRINTLN | Satır sonu ile yazdırma |
| `return` | TOKEN_RETURN | Return ifadesi |
| `returns` | TOKEN_RETURNS | Dönüş tipi bildirimi |
| `string` | TOKEN_STRING_TYPE | String tipi |
| `struct` | TOKEN_STRUCT | Struct bildirimi |
| `then` | TOKEN_THEN | If/While koşul sonu |
| `to` | TOKEN_TO | For döngüsü artan |
| `true` | TOKEN_TRUE | Boolean doğru |
| `tuple` | TOKEN_TUPLE | Tuple tipi |
| `while` | TOKEN_WHILE | While döngüsü |
| `xor` | TOKEN_XOR | Bitwise XOR |

### Operatörler

| Sembol | Token | Açıklama |
|--------|-------|----------|
| `+` | TOKEN_PLUS | Toplama |
| `-` | TOKEN_MINUS | Çıkarma |
| `*` | TOKEN_MULTIPLY | Çarpma |
| `/` | TOKEN_DIVIDE | Bölme |
| `%` | TOKEN_MOD | Modulo |
| `**` | TOKEN_POWER | Üs alma |
| `==` | TOKEN_EQUAL | Eşitlik |
| `!=` | TOKEN_NOT_EQUAL | Eşitsizlik |
| `<` | TOKEN_LESS | Küçüktür |
| `>` | TOKEN_GREATER | Büyüktür |
| `<=` | TOKEN_LESS_EQUAL | Küçük eşit |
| `>=` | TOKEN_GREATER_EQUAL | Büyük eşit |
| `=` | TOKEN_ASSIGN | Atama |

### Sınırlayıcılar

| Sembol | Token | Açıklama |
|--------|-------|----------|
| `(` | TOKEN_LPAREN | Sol parantez |
| `)` | TOKEN_RPAREN | Sağ parantez |
| `[` | TOKEN_LBRACKET | Sol köşeli parantez (dizi) |
| `]` | TOKEN_RBRACKET | Sağ köşeli parantez |
| `<` | TOKEN_LANGLE | Sol açılı parantez (tuple) |
| `>` | TOKEN_RANGLE | Sağ açılı parantez |
| `,` | TOKEN_COMMA | Virgül ayırıcı |
| `:` | TOKEN_COLON | Tip ayırıcı |
| `;` | TOKEN_SEMICOLON | İfade sonlandırıcı |
| `.` | TOKEN_DOT | Üye erişimi |

---

## 🔧 Eksiksiz Örnekler

### 1. If-Else ile Fonksiyon

```pmpl
function hesapla(numeric x) returns numeric
    if x > 100 then
        return x * 2
    else_if x > 50 then
        return x + 10
    else
        return x
    end_if
end_function
```

**Oluşturulan Token'lar:**
```
TOKEN_FUNCTION, TOKEN_IDENTIFIER("hesapla"), TOKEN_LPAREN,
TOKEN_NUMERIC, TOKEN_IDENTIFIER("x"), TOKEN_RPAREN,
TOKEN_RETURNS, TOKEN_NUMERIC,
TOKEN_IF, TOKEN_IDENTIFIER("x"), TOKEN_GREATER, TOKEN_NUMBER(100), TOKEN_THEN,
TOKEN_RETURN, TOKEN_IDENTIFIER("x"), TOKEN_MULTIPLY, TOKEN_NUMBER(2),
TOKEN_ELSE_IF, TOKEN_IDENTIFIER("x"), TOKEN_GREATER, TOKEN_NUMBER(50), TOKEN_THEN,
TOKEN_RETURN, TOKEN_IDENTIFIER("x"), TOKEN_PLUS, TOKEN_NUMBER(10),
TOKEN_ELSE,
TOKEN_RETURN, TOKEN_IDENTIFIER("x"),
TOKEN_END_IF,
TOKEN_END_FUNCTION
```

### 2. Break/Continue ile While Döngüsü

```pmpl
function ogeleri_isle() returns numeric
    numeric sayac = 0
    numeric i = 0
    
    while i < 100 do
        i = i + 1
        
        if i mod 2 == 0 then
            continue        -- Çift sayıları atla
        end_if
        
        if i > 50 then
            exit            -- 50'de dur
        end_if
        
        sayac = sayac + 1
    end_while
    
    return sayac
end_function
```

### 3. İç İçe Kontrol ile For Döngüsü

```pmpl
function ilkini_bul(array sayilar) returns numeric
    for i = 0 to 99 do
        if sayilar[i] < 0 then
            exit_for        -- Açıkça for döngüsünden çık
        end_if
        
        if sayilar[i] == 0 then
            continue_for    -- Açıkça for döngüsüne devam
        end_if
        
        return sayilar[i]
    end_for
    
    return -1
end_function
```

### 4. Struct Tanımı

```pmpl
struct Nokta
    numeric x
    numeric y
end_struct

function mesafe(Nokta p1, Nokta p2) returns numeric
    numeric dx = p2.x - p1.x
    numeric dy = p2.y - p1.y
    return (dx * dx + dy * dy) ** 0.5
end_function
```

### 5. Enum Tanımı

```pmpl
enum Renk
    KIRMIZI
    YESIL
    MAVI
end_enum

enum Durum
    BEKLEMEDE
    AKTIF
    TAMAMLANDI
    BASARISIZ
end_enum

function durumu_kontrol_et(Durum d) returns string
    if d == Durum.AKTIF then
        return "Çalışıyor"
    else_if d == Durum.TAMAMLANDI then
        return "Bitti"
    else
        return "Diğer"
    end_if
end_function
```

---

## 🎨 Kullanıcı Sözdizimi ile Karşılaştırma

### Kullanıcı Kodu (C-stili)

```c
if (x > 5) {
    printf("Evet");
} else if (x > 2) {
    printf("Belki");
} else {
    printf("Hayır");
}
```

### PMPL'ye Normalize Edilmiş

```pmpl
if x > 5 then
    print("Evet")
else_if x > 2 then
    print("Belki")
else
    print("Hayır")
end_if
```

### Kullanıcı Kodu (Python-stili)

```python
if x > 5:
    print("Evet")
elif x > 2:
    print("Belki")
else:
    print("Hayır")
```

### PMPL'ye Normalize Edilmiş

```pmpl
if x > 5 then
    print("Evet")
else_if x > 2 then      -- "elif" → "else_if"
    print("Belki")
else
    print("Hayır")
end_if                  -- girinti sonu → "end_if"
```

**Sonuç:** Her ikisi de **aynı PMPL**'ye normalize edilir! ✨

---

## 📐 Normalizasyon Kuralları

### Blok Sonlandırıcılar

```
Kullanıcı: }              → PMPL: end_if / end_while / end_for (bağlam tabanlı)
Kullanıcı: end if         → PMPL: end_if (boşluk → alt çizgi)
Kullanıcı: fi             → PMPL: end_if (shell-stili)
Kullanıcı: endif          → PMPL: end_if (basic-stili)
Kullanıcı: (girinti)      → PMPL: end_if (python-stili)
```

### Anahtar Kelimeler

```
Kullanıcı: else if        → PMPL: else_if
Kullanıcı: elif           → PMPL: else_if
Kullanıcı: elsif          → PMPL: else_if
Kullanıcı: break          → PMPL: exit
Kullanıcı: next (Ruby)    → PMPL: continue
Kullanıcı: && / and       → PMPL: and
Kullanıcı: || / or        → PMPL: or
Kullanıcı: ! / not        → PMPL: not
```

### Tip Anahtar Kelimeleri

```
Kullanıcı: int / i32      → PMPL: numeric
Kullanıcı: float / f64    → PMPL: numeric
Kullanıcı: str / char*    → PMPL: string
Kullanıcı: bool           → PMPL: boolean
```

---

## ⚙️ Implementasyon Notları

### Lexer Davranışı

**Girdi:** `end_if`  
**Çıktı:** `TOKEN_END_IF` (tek token)

**DEĞİL:**  
~~Girdi: `end if`~~  
~~Çıktı: `TOKEN_END`, `TOKEN_IF` (iki token)~~

Normalizer, `end if` → `end_if` dönüşümünü lexing'den **önce** yapmalıdır!

### Parser Basitleştirme

**PMPL Yaklaşımı (Basit):**
```c
switch (tok->type) {
    case TOKEN_END_IF:
        // If bloğu sonu
        return;
    
    case TOKEN_ELSE_IF:
        // Else-if parse et
        break;
}
```

**Eski Yaklaşım (Karmaşık - KAÇININ):**
```c
if (tok->type == TOKEN_END) {
    Token* next = lexer_next_token(lexer);
    if (next->type == TOKEN_IF) {
        // "end if" pattern
        return;
    }
    lexer_unget_token(lexer, next);
}
```

### Self-Hosting Gereksinimi

PMPL sözdizimi **PMPL derleyicisi tarafından parse edilebilir** olmalıdır:

```pmpl
-- Bu PMPL kodu PMPL derleyicisi ile derlenmeli
function parse_if(Lexer lexer) returns Statement
    if current_token == TOKEN_IF then
        -- Koşul parse et
        -- Then gövdesi parse et
        
        if current_token == TOKEN_ELSE_IF then
            -- Else-if'i recursive parse et
        end_if
        
        return if_statement
    end_if
    
    return null
end_function
```

---

## 🔍 Doğrulama Kontrol Listesi

Yeni PMPL özelliği için:

- [ ] Anahtar kelime alt çizgi kullanır (boşluk yok): `end_X`, `exit_X`, `continue_X`
- [ ] Lexer'da karşılık gelen `TOKEN_X` tanımı var
- [ ] Parser basit switch-case kullanır (pattern matching yok)
- [ ] Normalizer tüm varyantları → PMPL standardına çevirir
- [ ] Self-hosting testi: PMPL derleyicisi sözdizimini parse edebilir
- [ ] Dokümantasyon güncellendi (bu dosya + migration.md)

---

## 📚 İlgili Belgeler

- `docs_tr/migration.md` - Çoklu sözdizimi normalizasyon rehberi (Türkçe)
- `kurallar_kitabı.md` - MELP mimari prensipleri (Türkçe)
- `compiler/stage0/modules/lexer/lexer.h` - Token tanımları
- `compiler/stage0/normalize/syntax_rules.h` - Normalizasyon kuralları

---

## 📝 Versiyon Geçmişi

- **1.0** (14 Aralık 2025) - İlk spesifikasyon
  - Blok sonlandırıcılar: `end_X`
  - Kontrol akışı: `else_if`
  - Döngü kontrolü: `exit_X`, `continue_X`
  - Eksiksiz token referansı
  - Self-hosting gereksinimleri

---

**Bakımcı:** MELP Dil Ekibi  
**Lisans:** MELP Projesinin Parçası  
**Durum:** ✅ STABİL - Production Hazır
