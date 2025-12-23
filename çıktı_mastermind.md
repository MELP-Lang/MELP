# 🎯 MASTERMIND DÜZELTMESİ

**Tarih:** 23 Aralık 2025

---

## ⚠️ ÖNCEKİ KARAR HATALI

**Stage 0 %100 = YANLIŞ hedef**

Stage 0'ı "tam" yapmaya çalışmak = yeni dipsiz kuyu

---

## ✅ DOĞRU HEDEF

```
Stage 0 = Self-hosting için YETEN kadar
        = Fırlatma rampası
        = Minimum Viable Bootstrap
```

---

## 📋 SELF-HOSTING İÇİN GERÇEKTEN NE LAZIM?

Bir compiler yazmak için:

| Özellik | Gerekli mi? | Neden? |
|---------|-------------|--------|
| **numeric** | ✅ EVET | Token ID'leri, pozisyonlar |
| **string** | ✅ EVET | Kaynak kod, IR output |
| **if-then-else** | ✅ EVET | Koşullu derleme |
| **while-do** | ✅ EVET | Kaynak kodu iterate etmek |
| **function** | ✅ EVET | Modülerlik |
| **function call** | ✅ EVET | Lexer→Parser→Codegen |
| **read_file** | ✅ EVET | Kaynak oku |
| **write_file** | ✅ EVET | IR yaz |
| **string[i]** | ✅ EVET | Karakter erişimi |
| **string concat** | ✅ EVET | IR string birleştirme |
| --- | --- | --- |
| struct | ⚠️ BELKİ | Token struct olabilir, ama numeric array de olur |
| enum | ❌ HAYIR | Numeric sabitler yeterli |
| global var | ❌ HAYIR | Fonksiyon parametresi olarak geç |
| import | ❌ HAYIR | Tek dosya compiler yeterli |
| array return | ❌ HAYIR | Çoklu return yerine out parametresi |

---

## 🎯 YENİ PLAN

```
Stage 0 için SADECE:
1. While/For codegen düzelt (KRİTİK - döngü lazım)
2. String karakter erişimi: s[i] (KRİTİK - lexer için)
3. String concat (KRİTİK - IR için)

GEREKMİYOR:
- Struct (numeric array ile çöz)
- Enum (numeric sabitler yeterli)
- Global değişken (parametre geç)
- Import (tek dosya compiler)
```

---

## 📊 GÜNCEL YZ PLANI

| YZ | Görev | Gerekli mi? |
|----|-------|-------------|
| YZ_04 | Stage 0 Analiz | ✅ AMA sadece gerekli olanları test et |
| YZ_05 | While/For Fix | ✅ KRİTİK |
| YZ_06 | String ops (s[i], concat) | ✅ KRİTİK |
| ~~YZ_07~~ | ~~Import/Module~~ | ❌ GEREKSIZ - İPTAL |
| ~~YZ_08~~ | ~~Global değişken~~ | ❌ GEREKSIZ - İPTAL |

**Toplam: 3 YZ, 4-6 saat**

---

## ✅ BAŞARI KRİTERİ (Basitleştirilmiş)

```mlp
-- Bu compiler derlenebilmeli ve çalışmalı:
function main() returns numeric
    string source = read_file("test.mlp")
    numeric i = 0
    while i < len(source) do
        string ch = char_at(source; i)  -- veya source[i]
        -- tokenize logic
        i = i + 1
    end_while
    string ir = "; LLVM IR\ndefine i64 @main() { ret i64 0 }"
    write_file("test.ll"; ir)
    return 0
end_function
```

---

## 📋 ÜA İÇİN TALİMAT

TODO'yu şöyle güncelle:

**ESKİ:**
```
Phase 0: Stage 0 Tamamlama (11 kategori)
```

**YENİ:**
```
Phase 0: Stage 0 Minimum Viable Bootstrap
- Task 0.1: While/For codegen fix
- Task 0.2: String karakter erişimi (s[i])
- Task 0.3: String concat

SADECE BU KADAR. DAHA FAZLASI YOK.
```

---

## 🚫 YASAKLAR (YENİLENDİ)

| YASAK | NEDEN |
|-------|-------|
| Struct ekleme | Numeric array yeterli |
| Enum ekleme | Numeric sabitler yeterli |
| Import ekleme | Tek dosya compiler yeterli |
| Global değişken | Parametre olarak geç |
| "Stage 0'ı %100 yap" | Dipsiz kuyu! |

---

## 🎯 ÖZET

```
Stage 0 = Fırlatma rampası
        = Minimum Viable Bootstrap
        = Self-hosting için YETEN kadar
        
DAHA FAZLASI DEĞİL!
```

---

**MASTERMIND**  
23 Aralık 2025
