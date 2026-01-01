# MELP Error Categories

**Tarih:** 1 Ocak 2026  
**Versiyon:** 1.0  
**Amaç:** Error kodlarının kategorileri ve açıklamaları

---

## 📋 KATEGORI YAPISI

### **E00XX: Syntax Errors (E0001-E0099)**

Sözdizimi hataları - parser seviyesinde yakalanan hatalar

| Kod | Açıklama |
|-----|----------|
| E0001 | Tanımlanmamış Değişken |
| E0002 | Tip Uyuşmazlığı |
| E0003 | Sözdizimi Hatası |
| E0004 | Fonksiyon Bulunamadı |
| E0005 | Yanlış Argüman Sayısı |
| E0006 | Null Pointer Erişimi |
| E0007 | Dizin Sınır Dışı |
| E0008 | Bölme Hatası (Sıfıra bölme) |
| E0009 | Return Eksik |
| E0010 | Çoklu Tanımlama |

---

### **E01XX: Type Errors (E0100-E0199)**

Tip sistemi hataları - type checker seviyesinde yakalanan hatalar

| Kod | Açıklama |
|-----|----------|
| E0100 | Geçersiz Tip Dönüşümü |
| E0101 | Tip Çıkarımı Başarısız |
| E0102 | Geçersiz İşlem |
| E0103 | Uyumsuz Tipler |
| E0104 | Struct Alan Bulunamadı |
| E0105 | Geçersiz Dönüş Tipi |

---

### **E02XX: Name Resolution (E0200-E0299)**

İsim çözümlemesi hataları - modül ve kapsam hataları

| Kod | Açıklama |
|-----|----------|
| E0200 | Modül Bulunamadı |
| E0201 | İsim Çakışması |
| E0202 | Private Erişim |
| E0203 | Export Hatası |
| E0204 | Döngüsel Import |

---

### **E03XX: Import/File Errors (E0300-E0399)**

Dosya ve import hataları

| Kod | Açıklama |
|-----|----------|
| E0300 | Dosya Bulunamadı |
| E0301 | Okuma Hatası |
| E0302 | Yazma Hatası |

---

### **E04XX: Control Flow Errors (E0400-E0499)**

Kontrol akışı hataları

| Kod | Açıklama |
|-----|----------|
| E0400 | Ulaşılamaz Kod |
| E0401 | Break Yanlış Kullanım |
| E0402 | Continue Yanlış Kullanım |
| E0403 | Sonsuz Döngü |
| E0404 | If Koşul Hatası |
| E0405 | While Koşul Hatası |

---

### **E05XX: Memory/Runtime Errors (E0500-E0599)**

Bellek ve runtime hataları

| Kod | Açıklama |
|-----|----------|
| E0500 | Bellek Hatası |
| E0501 | Stack Overflow |
| E0502 | Geçersiz Pointer |

---

## ⚠️ WARNING CATEGORIES

### **W00XX: Unused Code (W0001-W0099)**

Kullanılmayan kod uyarıları

| Kod | Açıklama |
|-----|----------|
| W0001 | Kullanılmayan Değişken |
| W0002 | Kullanılmayan Import |
| W0003 | Kullanılmayan Fonksiyon |
| W0004 | Shadowing (Değişken gölgeleme) |
| W0005 | Potansiyel Null |
| W0006 | Tip Annotation Önerilir |
| W0007 | Boş Catch Bloğu |
| W0008 | Magic Number |
| W0009 | Karmaşık İfade |
| W0010 | Uzun Fonksiyon |

---

### **W01XX: Style Warnings (W0100-W0199)**

Kod stili uyarıları

| Kod | Açıklama |
|-----|----------|
| W0100 | İsimlendirme Uyarısı (Naming convention) |
| W0101 | TODO Yorumu |
| W0102 | Console Log (Production'da) |
| W0103 | Deprecated Kullanımı |
| W0104 | Performans Uyarısı |
| W0105 | Güvenlik Uyarısı |
| W0106 | Büyük Dosya |
| W0107 | Çok Fazla Parametre |
| W0108 | Deep Nesting (İç içe bloklar) |
| W0109 | Duplicate Code |
| W0110 | Implicit Type Coercion |

---

## 📊 İSTATİSTİKLER

**Errors:** 33 kod (E0001-E0502)  
**Warnings:** 21 kod (W0001-W0110)  
**Toplam:** 54 kod  
**Diller:** Türkçe + İngilizce

---

## 🎨 SEVERITY SEVİYELERİ

| Severity | Açıklama | Örnek |
|----------|----------|-------|
| **error** | Derleme durur, düzeltilmesi zorunlu | E0001-E0502 |
| **warning** | Derleme devam eder, düzeltilmesi önerilir | W0001-W0110 |
| **info** | Bilgilendirme mesajı (gelecekte eklenecek) | - |

---

## 🔧 PLACEHOLDER SİSTEMİ

Error mesajlarında kullanılan placeholderlar:

| Placeholder | Açıklama | Örnek |
|-------------|----------|-------|
| `{var}` | Değişken adı | x, userName, count |
| `{func}` | Fonksiyon adı | add, calculate, getData |
| `{type}` | Tip adı | int, string, bool |
| `{expected}` | Beklenen değer | int, 42, true |
| `{found}` | Bulunan değer | string, "hello", false |
| `{op}` | Operatör | +, -, *, / |
| `{module}` | Modül adı | std/math, utils |
| `{file}` | Dosya adı | test.mlp, main.mlp |
| `{import}` | Import edilen isim | math, log |
| `{left}` | Sol taraf tipi | int, string |
| `{right}` | Sağ taraf tipi | int, string |
| `{struct}` | Struct adı | Point, User |
| `{field}` | Struct alanı | x, y, name |
| `{index}` | Dizin değeri | 0, 5, 10 |
| `{max}` | Maksimum değer | 9, 99, 999 |
| `{reason}` | Sebep | Permission denied, ... |
| `{token}` | Token | ;, ), } |
| `{cycle}` | Döngü | a.mlp → b.mlp → a.mlp |
| `{value}` | Değer | 404, 500, 42 |
| `{comment}` | Yorum | TODO: Fix this |
| `{issue}` | Güvenlik sorunu | SQL injection risk |
| `{operation}` | İşlem adı | string concat in loop |
| `{suggestion}` | Öneri | Use StringBuilder |
| `{count}` | Sayı | 7, 10 |
| `{depth}` | Derinlik seviyesi | 5, 8 |
| `{lines}` | Satır sayısı | 100, 500 |
| `{alternative}` | Alternatif | useNewAPI() |
| `{from}` | Kaynak tip | string |
| `{to}` | Hedef tip | int |
| `{name}` | Genel isim | MyClass, myVar |

---

## 📖 KULLANIM ÖRNEKLERİ

### **Örnek 1: Basit Error**

```bash
mlp-errors E0001 test.mlp 5 10 x
```

**Çıktı:**
```
Error[E0001]: Tanımlanmamış Değişken
  --> test.mlp:5:10
   |
 5 | print(x)
   |       ^ 'x' değişkeni bu kapsamda bulunamadı
   |
   = help: Değişkeni kullanmadan önce tanımlamayı deneyin:
     let x = değer;
```

---

### **Örnek 2: Tip Hatası**

```bash
mlp-errors E0002 main.mlp 12 5 int string
```

**Çıktı:**
```
Error[E0002]: Tip Uyuşmazlığı
  --> main.mlp:12:5
   |
12 | let y: int = "hello"
   |     ^ 'int' tipi beklenirken 'string' tipi bulundu
   |
   = help: Değişkenin tipini kontrol edin veya tip dönüşümü yapın
```

---

### **Örnek 3: Warning**

```bash
mlp-errors W0001 utils.mlp 8 5 unused
```

**Çıktı:**
```
Warning[W0001]: Kullanılmayan Değişken
  --> utils.mlp:8:5
   |
 8 | let unused = 5
   |     ^^^^^^ 'unused' değişkeni tanımlandı ama kullanılmadı
   |
   = help: Kullanılmayan değişkeni kaldırın veya '_' ile başlatın (private)
```

---

## 🌍 ÇOK DİLLİ DESTEK

Error mesajları **MLP_LANG** environment variable ile kontrol edilir:

```bash
# Türkçe (varsayılan)
export MLP_LANG=tr
mlp-errors E0001 test.mlp 5 10 x

# İngilizce
export MLP_LANG=en
mlp-errors E0001 test.mlp 5 10 x
```

---

## 🎯 GELECEK PLANLAR

### **Versiyon 1.1** (Şubat 2026)
- [ ] Info severity seviyesi
- [ ] Daha fazla error code (E0600-E0699)
- [ ] Hint sistemi (birden fazla çözüm önerisi)

### **Versiyon 1.2** (Mart 2026)
- [ ] Fix suggestion'lar (otomatik düzeltme önerileri)
- [ ] Related error'lar (benzer hatalar)
- [ ] Error documentation links

### **Versiyon 2.0** (Nisan 2026)
- [ ] Üçüncü dil desteği (örn: Almanca, Fransızca)
- [ ] Custom error templates
- [ ] IDE integration (LSP ile)

---

**Tarih:** 1 Ocak 2026  
**Yazar:** TOOLING_YZ_01  
**Durum:** ✅ Tamamlandı
