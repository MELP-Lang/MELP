# MELP Keyword Referansı

**Son Güncelleme:** 8 Mart 2026  
**Kaynak:** `compiler/stage0/lexer/lexer.cpp` (doğrudan)

Her keyword için durum:  
✅ Tam çalışıyor | ⚠️ Kısmi | 🔴 Henüz yok | 📋 Planlandı

---

## 1. Tipler

| Keyword | Açıklama | Durum | YARDIM |
|---------|----------|-------|--------|
| `numeric` | 64-bit tam sayı / ondalık | ✅ | [TYPES.md](TYPES.md) |
| `string` | Metin | ✅ | [TYPES.md](TYPES.md) · [STRING.md](STRING.md) |
| `boolean` | true / false | ✅ | [TYPES.md](TYPES.md) |
| `true` / `false` | Boolean literaller | ✅ | [TYPES.md](TYPES.md) |
| `null` / `none` | Null referans (→ 0) | ✅ | — |
| `auto` | Tip çıkarımı (numeric ✅, string ❌ EKS-018) | ⚠️ Kısmi | — |

---

## 2. Değişken ve Atama

| Keyword | Açıklama | Durum | YARDIM |
|---------|----------|-------|--------|
| `as` | Tip belirteci (external func / eski format — yeni: `numeric function f()`) | ✅ | [VARIABLE.md](VARIABLE.md) |
| `const` | Sabit değişken | ⚠️ Kısmi | [VARIABLE.md](VARIABLE.md) |
| `ref` | Referans parametre | ✅ Tam (EKS-019, 8 Mart 2026) | [FUNCTION.md](FUNCTION.md) |
| `is` | Tip kontrolü | ❌ Parser hatası (if x is T then) | — |

---

## 3. Fonksiyon ve Metod

| Keyword | Açıklama | Durum | YARDIM |
|---------|----------|-------|--------|
| `function` | Fonksiyon tanımı | ✅ | [FUNCTION.md](FUNCTION.md) |
| `end function` | Fonksiyon sonu | ✅ | [FUNCTION.md](FUNCTION.md) |
| `return` | Değer döndür | ✅ | — |
| `this` | Struct method içinde instance | ✅ | [FUNCTION.md](FUNCTION.md) |
| `external` | Dışarıdan fonksiyon bildirimi | ✅ | [EXTERNAL.md](EXTERNAL.md) |
| `lambda` / `end lambda` | Anonim fonksiyon | ✅ (inline) / ❌ (blok) | [LAMBDA.md](LAMBDA.md) |
| `method` | Struct method tanımı | ✅ Tam (EKS-010, 8 Mart 2026) | [FUNCTION.md](FUNCTION.md) |

---

## 4. Struct, Enum, Interface

| Keyword | Açıklama | Durum | YARDIM |
|---------|----------|-------|--------|
| `struct` / `end struct` | Yapı tipi | ✅ | [STRUCT.md](STRUCT.md) |
| `enum` / `end enum` | Sayım tipi | ✅ | [ENUM.md](ENUM.md) |
| `interface` / `end interface` | Arayüz (static dispatch) | ✅ Tam (30/30 test) | [STRUCT.md](STRUCT.md) |
| `implements` | Interface uygulama bildirimi | ✅ Tam | [STRUCT.md](STRUCT.md) |
| `type` | Tip takma adı | ⚠️ Kısmi | — |
| `with` / `end with` | Struct field kısaltması | ✅ Tam (EKS-004, 8 Mart 2026) | [WITH.md](WITH.md) |

---

## 5. Kontrol Akışı

| Keyword | Açıklama | Durum | YARDIM |
|---------|----------|-------|--------|
| `if` / `then` / `end if` | Koşul bloğu | ✅ | [CONTROL_FLOW.md](CONTROL_FLOW.md) |
| `else` | Else dalı | ✅ | [CONTROL_FLOW.md](CONTROL_FLOW.md) |
| `else if` | Else-if dalı | ✅ | [CONTROL_FLOW.md](CONTROL_FLOW.md) |
| `match` / `end match` | Pattern matching | ⚠️ Kısmi | [CONTROL_FLOW.md](CONTROL_FLOW.md) |
| `case` | Match dalı | ⚠️ Kısmi | [CONTROL_FLOW.md](CONTROL_FLOW.md) |
| `switch` / `end switch` | Switch-case | ✅ | [CONTROL_FLOW.md](CONTROL_FLOW.md) |
| `when` | Guard koşulu | ⚠️ Kısmi | [CONTROL_FLOW.md](CONTROL_FLOW.md) |
| `let` | Bağlama (if let) | ⚠️ Kısmi | — |

---

## 6. Döngüler

| Keyword | Açıklama | Durum | YARDIM |
|---------|----------|-------|--------|
| `loop` / `end loop` | Koşullu döngü (while yerine) | ✅ | [LOOPS.md](LOOPS.md) |
| `for` / `to` / `end for` | For döngüsü | ✅ | [LOOPS.md](LOOPS.md) |
| `downto` | Azalan for | ✅ | [LOOPS.md](LOOPS.md) |
| `step` | Adım boyutu | ✅ | [LOOPS.md](LOOPS.md) |
| `each` / `in` | For-each | ✅ | [LOOPS.md](LOOPS.md) |
| `exit` | Döngüden çık (break) | ✅ | [LOOPS.md](LOOPS.md) |
| `exit_<isim>` | İsimli scope'dan çık | ✅ | [SCOPE.md](SCOPE.md) |
| `continue` | Döngü devam | ✅ | [LOOPS.md](LOOPS.md) |
| `continue_for` | For döngüsünde devam | ✅ | [LOOPS.md](LOOPS.md) |
| `continue_while` | While döngüsünde devam | ✅ | [LOOPS.md](LOOPS.md) |

---

## 7. Scope

| Keyword | Açıklama | Durum | YARDIM |
|---------|----------|-------|--------|
| `scope` | İsimli blok başlat | ✅ | [SCOPE.md](SCOPE.md) |
| `end scope` | Scope sonu (evrensel) | ✅ | [SCOPE.md](SCOPE.md) |
| `end <isim>` | Scope sonu (isim spesifik) | ✅ | [SCOPE.md](SCOPE.md) |

---

## 8. Mantıksal / Aritmetik Operatörler

| Keyword | Açıklama | Durum | |
|---------|----------|-------|--|
| `and` | Mantıksal VE | ✅ | [OPERATORS.md](OPERATORS.md) |
| `or` | Mantıksal VEYA | ✅ | [OPERATORS.md](OPERATORS.md) |
| `not` | Mantıksal DEĞİL | ✅ | [OPERATORS.md](OPERATORS.md) |
| `xor` | Mantıksal XOR | ✅ | [OPERATORS.md](OPERATORS.md) |
| `mod` | Modulo | ✅ | [OPERATORS.md](OPERATORS.md) |
| `**` | Üs alma | ✅ | [OPERATORS.md](OPERATORS.md) |

---

## 9. Girdi / Çıktı

| Keyword | Açıklama | Durum |
|---------|----------|-------|
| `print` | Ekrana yaz | ✅ | [IO.md](IO.md) |
| `println` | Satır atlayarak yaz | ✅ | [IO.md](IO.md) |
| `input("prompt")` | Prompt ile stdin oku | ❌ Parser hatası | [INPUT.md](INPUT.md) |
| `read_line()` | stdin'den satır oku | ✅ | [INPUT.md](INPUT.md) |
| `read_arg(n)` | n. komut satırı argümanı | ✅ | [INPUT.md](INPUT.md) |
| `read_file` | Dosya oku | ✅ | [IO.md](IO.md) |
| `write_file` | Dosya yaz | ✅ | [IO.md](IO.md) |
| `append_file` | Dosyaya ekle | ✅ | [IO.md](IO.md) |

---

## 10. Modül Sistemi

| Keyword | Açıklama | Durum |
|---------|----------|-------|
| `import` | Modül yükle (`import utils`) | ✅ | [MODULES.md](MODULES.md) |
| `from` | Seçici import (`from m import f`) | ✅ Tam (EKS-012, 8 Mart 2026) | [MODULES.md](MODULES.md) |
| `export` | Dışa aç (`export function/struct`) | ✅ | [MODULES.md](MODULES.md) |
| `module` / `end module` | Modül bloğu (namespace) | ✅ Tam (EKS-012, 8 Mart 2026) | [MODULES.md](MODULES.md) |

---

## 11. Hata Yönetimi

| Keyword | Açıklama | Durum |
|---------|----------|-------|
| `try` / `end try` | Dene bloğu | ✅ Tam (EKS-011, 8 Mart 2026) | [ERROR_HANDLING.md](ERROR_HANDLING.md) |
| `catch` | Hata yakala | ✅ Tam | [ERROR_HANDLING.md](ERROR_HANDLING.md) |
| `finally` | Her durumda çalış | ✅ Tam | [ERROR_HANDLING.md](ERROR_HANDLING.md) |
| `throw` | Hata fırlat | ✅ Tam | [ERROR_HANDLING.md](ERROR_HANDLING.md) |
| `defer` | Blok sonunda çalıştır (LIFO) | ✅ | [ERROR_HANDLING.md](ERROR_HANDLING.md) |

---

## 12. Program Yapısı

| Keyword | Açıklama | Durum |
|---------|----------|-------|
| `program` / `end program` | Ana program bloğu | ✅ | [EVENT.md](EVENT.md) |
| `app_start` / `end app` | Uygulama giriş noktası (@main) | ✅ | [EVENT.md](EVENT.md) |
| `event` / `end event` | Olay bloğu (parametreli) | ✅ Tam (EKS-016, 8 Mart 2026) | [EVENT.md](EVENT.md) |

---

## 13. Koleksiyon Tipleri

| Keyword | Açıklama | Durum | YARDIM |
|---------|----------|-------|--------|
| `T[]` | Sabit boyutlu dizi | ✅ (atama bug) | [ARRAY.md](ARRAY.md) |
| `hashmap<K; V>` | Anahtar-değer koleksiyonu | ✅ (metod OOP ❌) | [HASHMAP.md](HASHMAP.md) |
| `list` | Liste | 🔴 Phase 3'te gelecek | — |
| `tuple` | Demet | 🔴 Phase 3'te gelecek | — |

---

## 14. Eşzamanlılık

| Keyword | Açıklama | Durum | YARDIM |
|---------|----------|-------|--------|
| `async function` | Async fonksiyon tanımı | ✅ (sync pass-through) | [ASYNC.md](ASYNC.md) |
| `await` | Async sonuç bekleme | ✅ (sync pass-through) | [ASYNC.md](ASYNC.md) |
| `spawn` / `end spawn` | Thread başlatma + handle atama | ✅ Tam (EKS-017, 8 Mart 2026) | [ASYNC.md](ASYNC.md) |
| `channel_create` | Thread-safe kanal oluştur | ✅ | [ASYNC.md](ASYNC.md) |
| `send(ch; val)` | Kanala yaz | ✅ | [ASYNC.md](ASYNC.md) |
| `receive(ch)` | Kanaldan oku (bloklayıcı) | ✅ | [ASYNC.md](ASYNC.md) |
| `select` / `end select` | Çoklu kanal seçici | 📋 Planlandı | — |

---

## 15. Gelişmiş / Compile-time

| Keyword | Açıklama | Durum |
|---------|----------|-------|
| `comptime` | Derleme zamanı | 📋 Planlandı |
| `macro` / `end macro` | Makro | 📋 Planlandı |
| `where` | Generic kısıt | 📋 Planlandı |
| `operator` / `end operator` | Operatör aşırı yükleme | ⚠️ Kısmi |
| `test` / `end test` | Test bloğu | ✅ | [TEST_BLOCK.md](TEST_BLOCK.md) |
| `debug` / `end debug` | Zero-overhead debug bloğu (`--debug` bayrağı) | ✅ | [DEBUG.md](DEBUG.md) |
| `pause` | Debugger kesim noktası (`int3` / SIGTRAP) | ✅ | [DEBUG.md](DEBUG.md) |
| `goto` | Etiket atla (parser impl, geliştirilecek) | ⚠️ Kısmi |
| `abort` | Programı sonlandır | 📋 Planlandı |

---

## Türkçe Alias'lar

Lexer'da aktif (her zaman geçerli, `--lang` bayrağına gerek yok):

| Alias | Karşılığı |
|-------|-----------|
| `son_fonksiyon` | `end function` |
| `fonksiyon_sonu` | `end function` |
| `islev_sonu` | `end function` |

---

## EKS Listesiyle İlişki

| EKS | Keyword | Açıklama |
|-----|---------|----------|
| EKS-001 | `program` / `end program` | Sugar syntax |
| EKS-003 | struct `(params)` | Constructor parametresi | ✅ GİDERİLDİ 8 Mart 2026 |
| EKS-004 | `with` / `end_with` | Struct field kısaltması | ✅ GİDERİLDİ 8 Mart 2026 |
| EKS-010 | `method` / `end_<isim>` | Method keyword | ✅ GİDERİLDİ 8 Mart 2026 |
| EKS-011 | `try` / `catch` / `throw` / `finally` | Parser kaydı eksik | ✅ GİDERİLDİ 8 Mart 2026 |
| EKS-012 | `module` namespace + `from...import` | Codegen/parser eksik | ✅ GİDERİLDİ 8 Mart 2026 |
| EKS-013 | `read_file` NULL dönüşü | | ✅ GİDERİLDİ 8 Mar 2026 |
| EKS-014 | `hashmap.has()` / `.size()` / `.delete()` | Metod OOP sözdizimi yok — `hashmap_str_*()` kullan | ✅ GİDERİLDİ 8 Mart 2026 |
| EKS-015 | `to_number()` | Uygulanmamış | ✅ GİDERİLDİ 8 Mart 2026 |
| EKS-016 | `event` parametreli | param desteği eksik | ✅ GİDERİLDİ 8 Mart 2026 |
| EKS-017 | `spawn` handle atama | parser hatası | ✅ GİDERİLDİ 8 Mart 2026 |
| EKS-019 | `ref` parametre | parser hatası | ✅ GİDERİLDİ 8 Mart 2026 |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `while koşul` | `loop koşul` |
| `end_while` | `end loop` |
| `end_function` | `end function` |
| `end_if` / `else_if` | `end if` / `else if` |
| `end_for` | `end for` |
| `end_match` / `end_enum` | `end match` / `end enum` |
| `function f() as tip` | `tip function f()` |
| `return 0` (main içinde) | kaldır |

---

**© 2026 MELP Project**
