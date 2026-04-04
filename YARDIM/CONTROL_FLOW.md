# MELP — Kontrol Akışı

**Durum:** ✅ if/else tam çalışıyor | ⚠️ match kısmi
**Versiyon:** Stage 1 (mlp1)

---

## if / else if / else

### Temel Sözdizimi

```mlp
if koşul then
    -- gövde
end if
```

### else ile

```mlp
if koşul then
    -- doğru dalı
else
    -- yanlış dalı
end if
```

### else if ile

```mlp
if not == 100 then
    println("mükemmel")
else
    if not >= 85 then
        println("çok iyi")
    else
        if not >= 70 then
            println("iyi")
        else
            println("başarısız")
        end if
    end if
end if
```

### Kritik Kurallar

| Kural | Doğru | Yanlış |
|-------|-------|--------|
| `then` zorunlu | `if x > 5 then` | `if x > 5` |
| Kapatıcı token | `end if` | blok kapatıcıyı atlamak |
| Else-if iki token | `else if` | `elif` |
| Parantez gerekmez | `if x > 5 then` | `if (x > 5) then` |

---

## switch / when / default

Sabit değerlere göre dallanma:

```mlp
switch gun
    when 1 then println("Pazartesi")
    when 2 then println("Salı")
    when 3 then println("Çarşamba")
    when 6; 7 then println("Hafta sonu")
    default println("Bilinmeyen")
end switch
```

> `when` içinde `;` ile birden fazla değer verilebilir.

---

## match / case / end match

Pattern matching — daha güçlü bir dallanma yapısı.

> ⚠️ **Stage 0'da kısmi destek.** Tam implement Stage 1'de.

### Basit match

```mlp
numeric x = 5
match x
    case 1 then print("bir")
    case 2 then print("iki")
    else print("diğer")
end match
```

### Guard clause (koşullu case)

```mlp
match n
    case x if x < 10 then println("küçük")
    case x if x < 20 then println("orta")
    else println("büyük")
end match
```

### match ifade olarak (değer döndürür)

```mlp
string sinif = match puan
    case x if x >= 90 then "A"
    case x if x >= 75 then "B"
    case x if x >= 60 then "C"
    else "F"
end match
```

---

## Karşılaştırma Operatörleri

```mlp
if a == b then ...    -- eşit
if a != b then ...    -- eşit değil
if a > b  then ...    -- büyük
if a < b  then ...    -- küçük
if a >= b then ...    -- büyük eşit
if a <= b then ...    -- küçük eşit
```

---

## Mantıksal Operatörler

```mlp
if x > 0 and y > 0 then ...   -- VE
if x > 0 or y > 0 then ...    -- VEYA
if not aktif then ...          -- DEĞİL
```

---

## İç İçe if

```mlp
if x > 0 then
    if y > 0 then
        println("birinci çeyrek")
    else
        println("dördüncü çeyrek")
    end if
else
    println("soldaki yarım")
end if
```

---

## Anti-Pattern'lar

```mlp
-- ❌ then unutuldu
if x > 5
    print("hata")

-- ❌ Python stili
if x > 5:
    print("hata")

-- ❌ C stili
if (x > 5) {
    printf("hata");
}

-- ❌ elif (MELP'te yok)
elif x > 2 then

-- ✅ Doğru
if x > 5 then
    println("tamam")
else
    if x > 2 then
        println("orta")
    else
        println("küçük")
    end if
end if
```

## if let

✅ **Durum: TAM** — `if let` ile `Option<T>` enum'unu güvenli açma

`if let`, bir `Option<T>` değerinin `Some` varyantı olup olmadığını kontrol eder ve içindeki değeri doğrudan bağlar. `None` ise blok atlanır.

### Temel Sözdizimi

```mlp
enum Option<T>
  Some(T)
  None
end enum

Option x = Option.Some(42)

if let Some(v) = x then
    print(v)    -- 42
end if
```

### İçindeki Değerle İşlem

```mlp
Option x = Option.Some(10)

if let Some(v) = x then
    numeric sonuc = v * 2
    print(sonuc)   -- 20
end if
```

### `None` Durumunu Ele Alma

`if let` doğrudan `else` desteklemez. `None` dalı için standart `if/else` kullanın:

```mlp
-- ✅ Önerilen yöntem: if let içinde değeri kontrol et,
--    None için ayrı dal kullan
if let Some(v) = secim then
    print("Değer: " + str(v))
end if
```

### Kural ve Kısıtlar

| Kural | Açıklama |
|-------|----------|
| Yalnızca `Some(T)` destekli | `if let None = x` yazılamaz |
| Bağlama adı serbest | `Some(v)`, `Some(sayi)` vb. |
| `Option<T>` ile çalışır | Diğer enum varyantlarında henüz desteklenmiyor |
| `then` / `end if` zorunlu | Standart if bloğu sözdizimi geçerli |

---

## SSS

**S: `if` içinde `return` kullanabilir miyim?**
C: Evet. `if koşul then return değer end if` geçerlidir.

**S: Tek satır if mümkün mü?**
C: Evet: `if koşul then ifade end if` tek satırda yazılabilir.

**S: `switch` ile `match` farkı nedir?**
C: `switch`/`when` sabit değerler için; `match`/`case` pattern ve guard clause destekler.

**S: `else if` sonunda `end if` mi yazacağım?**
C: En sondaki dalın kapattığı tüm `if` bloğu için tek `end if` yeterlidir.
