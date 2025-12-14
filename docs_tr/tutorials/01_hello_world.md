# Eğitim 01: Merhaba Dünya - İlk MELP Programınız

MELP'e hoş geldiniz! Bu eğitimde, ilk MELP programınızı nasıl yazacağınızı, derleyeceğinizi ve çalıştıracağınızı öğreneceksiniz.

## İçindekiler
- [Neler Öğreneceksiniz](#neler-öğreneceksiniz)
- [Ön Koşullar](#ön-koşullar)
- [İlk Programınızı Yazma](#i̇lk-programınızı-yazma)
- [Kodu Anlamak](#kodu-anlamak)
- [LLVM Backend ile Derleme](#llvm-backend-ile-derleme)
- [x86-64 Backend ile Derleme](#x86-64-backend-ile-derleme)
- [Çıktıyı Analiz Etme](#çıktıyı-analiz-etme)
- [Yaygın Sorunlar](#yaygın-sorunlar)
- [Sonraki Adımlar](#sonraki-adımlar)

---

## Neler Öğreneceksiniz

Bu eğitimin sonunda:
- Basit bir MELP programı yazabileceksiniz
- MELP programının temel yapısını anlayacaksınız
- MELP kodunu hem LLVM hem de x86-64 backend'leri ile derleyebileceksiniz
- Derlenmiş programınızı çalıştırabileceksiniz
- Derleme sürecini anlayacaksınız

---

## Ön Koşullar

Başlamadan önce şunlara sahip olduğunuzdan emin olun:
- MELP derleyici kurulu (Stage 0)
- LLVM 19 kurulu (LLVM backend için)
- GCC veya Clang (linkleme için)
- Temel komut satırı bilgisi

**Kurulumu kontrol edin:**
```bash
# MELP derleyicinin varlığını doğrulayın
ls compiler/stage0/melp

# LLVM'in kurulu olduğunu doğrulayın
llvm-config --version  # 19.x göstermeli
```

---

## İlk Programınızı Yazma

Hadi MELP'te klasik "Merhaba Dünya" programını oluşturalım.

**Bir dosya oluşturun:** `merhaba.mlp`

```mlp
function main() returns numeric
    print("Merhaba, MELP!")
    return 0
end
```

İşte bu kadar! Sadece 4 satır kod. Hadi inceleyelim.

---

## Kodu Anlamak

### 1. Satır: Fonksiyon Bildirimi
```mlp
function main() returns numeric
```

- **`function`** - Bir fonksiyon bildiren anahtar kelime
- **`main()`** - Her MELP programının giriş noktası (C, Rust veya Go gibi)
- **`returns numeric`** - Bu fonksiyonun sayısal bir değer döndürdüğünü belirtir (C'deki `int` gibi)

> **Not:** Her MELP programının bir `main()` fonksiyonu olmalıdır. Çalıştırma buradan başlar.

### 2. Satır: Print İfadesi
```mlp
    print("Merhaba, MELP!")
```

- **`print()`** - Konsola metin yazdıran yerleşik fonksiyon
- **`"Merhaba, MELP!"`** - Bir string literal (çift tırnak içine alınmış metin)

> **Not:** MELP'teki string literal'ler UTF-8 kodlamasını destekler, yani özel karakterler kullanabilirsiniz!

### 3. Satır: Return İfadesi
```mlp
    return 0
```

- **`return`** - Fonksiyondan çıkan ve bir değer döndüren anahtar kelime
- **`0`** - Gelenek: 0 "başarı" anlamına gelir, sıfırdan farklı değerler "hata" anlamına gelir

### 4. Satır: End İfadesi
```mlp
end
```

- **`end`** - Fonksiyon gövdesinin sonunu işaretler

> **MELP Felsefesi:** Daha iyi okunabilirlik için süslü parantezler `{}` yerine `end` kullanıyoruz!

---

## LLVM Backend ile Derleme

**LLVM backend**, herhangi bir platformda çalışabilen taşınabilir LLVM IR (Intermediate Representation - Ara Gösterim) üretir.

### Adım 1: LLVM IR'ye Derleme

```bash
./compiler/stage0/melp merhaba.mlp merhaba.ll
```

**Ne olur:**
1. MELP `merhaba.mlp` dosyasını okur
2. Lexer kodu token'lara ayırır
3. Parser bir Soyut Sözdizim Ağacı (AST) oluşturur
4. LLVM backend `merhaba.ll` (LLVM IR dosyası) üretir

### Adım 2: Üretilen LLVM IR'yi Görüntüleme

```bash
cat merhaba.ll
```

**Çıktı (basitleştirilmiş):**
```llvm
; ModuleID = 'merhaba.mlp'
source_filename = "merhaba.mlp"

@.str.0 = private unnamed_addr constant [15 x i8] c"Merhaba, MELP!\00", align 1

declare i32 @puts(i8*)

define i64 @main() {
entry:
    %tmp1 = getelementptr inbounds [15 x i8], [15 x i8]* @.str.0, i64 0, i64 0
    %tmp2 = call i32 @puts(i8* %tmp1)
    ret i64 0
}
```

**Önemli Parçalar:**
- `@.str.0` - String'iniz salt okunur bellekte saklanıyor
- `@main()` - main fonksiyonunuz
- `call i32 @puts` - String'i yazdırıyor
- `ret i64 0` - 0 döndürüyor

### Adım 3: LLVM IR'yi Çalıştırılabilir Dosyaya Derleme

```bash
clang merhaba.ll -o merhaba
```

**Optimizasyon ile alternatif:**
```bash
clang -O2 merhaba.ll -o merhaba
```

### Adım 4: Programınızı Çalıştırma

```bash
./merhaba
```

**Çıktı:**
```
Merhaba, MELP!
```

🎉 **Tebrikler!** İlk MELP programınızı çalıştırdınız!

---

## x86-64 Backend ile Derleme

**x86-64 backend**, Linux x86-64 sistemleri için doğrudan assembly kodu üretir.

### Adım 1: Assembly'ye Derleme

```bash
./compiler/stage0/melp merhaba.mlp merhaba.s
```

**Ne olur:**
1. MELP `merhaba.mlp` dosyasını okur
2. Lexer ve Parser kodu işler
3. x86-64 backend `merhaba.s` (assembly dosyası) üretir

### Adım 2: Üretilen Assembly'yi Görüntüleme

```bash
cat merhaba.s
```

**Çıktı (basitleştirilmiş):**
```asm
.section .rodata
.LC0:
    .string "Merhaba, MELP!"

.text
.globl main
.type main, @function
main:
    pushq   %rbp
    movq    %rsp, %rbp
    
    leaq    .LC0(%rip), %rdi
    call    puts@PLT
    
    movl    $0, %eax
    popq    %rbp
    ret
```

**Önemli Parçalar:**
- `.rodata` - Salt okunur veri bölümü (string'lerin yaşadığı yer)
- `.LC0` - "Merhaba, MELP!" string'iniz
- `main:` - main fonksiyonunuz assembly'de
- `call puts@PLT` - C kütüphanesinin `puts()` fonksiyonunu çağırıyor
- `movl $0, %eax` - 0 döndürüyor

### Adım 3: Assembleme ve Linkleme

```bash
gcc merhaba.s -o merhaba
```

**Runtime kütüphanesi ile alternatif:**
```bash
gcc merhaba.s runtime/stdlib/libsto.a -o merhaba
```

### Adım 4: Programınızı Çalıştırma

```bash
./merhaba
```

**Çıktı:**
```
Merhaba, MELP!
```

---

## Çıktıyı Analiz Etme

### Backend'leri Karşılaştırma

| Özellik | LLVM Backend | x86-64 Backend |
|---------|--------------|----------------|
| **Taşınabilirlik** | ✅ Çapraz platform | ❌ Sadece Linux x86-64 |
| **Optimizasyon** | ✅ LLVM optimizasyonları | ❌ Optimizasyon yok |
| **Öğrenme Değeri** | Orta | ✅ Yüksek (ham assembly görünür) |
| **Hız** | ✅ Hızlı (optimize edilmiş) | Orta (optimizasyon yok) |
| **Dosya Boyutu** | Küçük (`-O2` ile) | Küçük |

### Hangi Backend Ne Zaman Kullanılmalı?

**LLVM Backend kullanın:**
- Çapraz platform uyumluluğu istediğinizde (Windows, macOS, Linux)
- Optimize edilmiş kod gerektiğinde (`-O2`, `-O3`)
- Üretim yazılımı geliştirirken

**x86-64 Backend kullanın:**
- Assembly'nin nasıl çalıştığını öğrenmek istediğinizde
- Düşük seviye sorunları ayıklarken
- MELP'in makine kodunu nasıl ürettiğini merak ediyorsanız
- Linux x86-64 üzerindeyseniz (masaüstü/dizüstü)

---

## Yaygın Sorunlar

### Sorun 1: "Command not found: ./compiler/stage0/melp"

**Problem:** Derleyici henüz derlenmemiş.

**Çözüm:**
```bash
cd compiler/stage0
make clean
make
```

### Sorun 2: "LLVM not found"

**Problem:** LLVM 19 kurulu değil.

**Çözüm (Ubuntu/Debian):**
```bash
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 19
```

### Sorun 3: "Undefined reference to `puts`"

**Problem:** Standart kütüphane linklenmemiş.

**Çözüm:**
```bash
# LLVM backend
clang merhaba.ll -o merhaba

# x86-64 backend (runtime fonksiyonları kullanıyorsanız)
gcc merhaba.s runtime/stdlib/libsto.a -o merhaba
```

### Sorun 4: Print Çıktısı Görünmüyor

**Problem:** Çıktı tamponlaması.

**Çözüm:** `print()` ile çıktınız hemen görünmeli. Görünmüyorsa:
```bash
./merhaba | cat
```

---

## Sonraki Adımlar

Artık ilk MELP programınızı oluşturduğunuza göre, bu zorlukları deneyin:

### Zorluk 1: Çoklu Yazdırma
Programınızı birden fazla satır yazdıracak şekilde değiştirin:
```mlp
function main() returns numeric
    print("Merhaba, MELP!")
    print("Programlamaya hoş geldiniz!")
    print("Hadi harika bir şey yapalım!")
    return 0
end
```

### Zorluk 2: UTF-8 Desteği
Özel karakterler yazdırmayı deneyin:
```mlp
function main() returns numeric
    print("Merhaba Dünya! 🚀")
    print("你好世界")
    print("Привет мир")
    return 0
end
```

### Zorluk 3: Backend'leri Karşılaştırın
Aynı programı her iki backend ile derleyin ve karşılaştırın:
```bash
# LLVM
./compiler/stage0/melp merhaba.mlp merhaba_llvm.ll
cat merhaba_llvm.ll > llvm_cikti.txt

# x86-64
./compiler/stage0/melp merhaba.mlp merhaba_x86.s
cat merhaba_x86.s > x86_cikti.txt

# Karşılaştır
diff llvm_cikti.txt x86_cikti.txt
```

---

## Ne Öğrendiniz

✅ MELP program yapısı (`function main()`, `return`, `end`)  
✅ Metin göstermek için `print()` kullanımı  
✅ LLVM backend ile derleme (`.mlp` → `.ll` → çalıştırılabilir)  
✅ x86-64 backend ile derleme (`.mlp` → `.s` → çalıştırılabilir)  
✅ Üretilen LLVM IR ve assembly kodunu anlama  
✅ İki backend arasındaki farklar  

---

## Öğrenmeye Devam Edin

**Sonraki Eğitim:** [02 - Değişkenler ve Tipler](02_variables.md)

Şunları öğrenin:
- Değişken bildirimi
- Farklı veri tiplerini kullanma (numeric, string, boolean)
- Değişken kapsamını anlama
- Temel işlemler yapma

---

## Ek Kaynaklar

- **Dil Referansı:** [docs_tr/language/README.md](../language/README.md)
- **LLVM IR Kılavuzu:** [docs/LLVM_IR_GUIDE.md](../../docs/LLVM_IR_GUIDE.md)
- **Mimari Genel Bakış:** [ARCHITECTURE.md](../../ARCHITECTURE.md)
- **Örnekler:** [examples/basics/](../../examples/basics/)

---

**Mutlu kodlamalar! 🚀**
