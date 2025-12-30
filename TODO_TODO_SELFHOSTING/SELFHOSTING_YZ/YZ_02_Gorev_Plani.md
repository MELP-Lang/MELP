# 🤖 YZ_02 GÖREV PLANI

**Tarih:** 29 Aralık 2025  
**YZ:** SELFHOSTING_YZ_02  
**Durum:** ⏸️ YZ_01 Bitmesini Bekliyor  
**Öncelik:** 🟡 Orta (YZ_01'den sonra)

---

## 🎯 SENİN GÖREVİN

**PHASE 2: BOOTSTRAP TEST**

YZ_01 codegen'i C üretecek hale getirdi. Sen şimdi Stage0-C derleyiciyi derleyip test edeceksin.

**Tahmini Süre:** 2-3 saat  
**Zorluk:** ⭐⭐ Orta

---

## 📋 GÖREV LİSTESİ

### 🟡 PHASE 2: BOOTSTRAP TEST

#### P2.1: Stage0 Derleme (30 dk)

**Konum:** `MELP/C/stage0/`

```bash
cd MELP/C/stage0
make clean && make
# veya
./build.sh
```

**Kontroller:**
- [ ] Make clean yapıldı
- [ ] Make başarılı (hata yok)
- [ ] `mlpc` binary oluştu
- [ ] Binary çalışıyor: `./mlpc --version`

**Sorun Çıkarsa:**
- Derleme hatası → Üst Akıl'a bildir
- Linking hatası → Library path'leri kontrol et
- Binary yok → Make output'u incele

#### P2.2: Hello World Testi (30 dk)

**Basit MLP Test:**

```bash
# hello.mlp oluştur
cat > hello.mlp << 'EOF'
print("Merhaba Dünya!")
print("MLP-GCC Stage0-C çalışıyor!")
EOF

# Derle
./mlpc hello.mlp -o hello.c

# İncele
cat hello.c  # C kodu doğru mu?

# GCC ile derle
gcc hello.c -o hello -L../runtime -lmlp_runtime

# Çalıştır
./hello
```

**Beklenen Çıktı:**
```
Merhaba Dünya!
MLP-GCC Stage0-C çalışıyor!
```

**Kontroller:**
- [ ] MLP → C dönüşümü başarılı
- [ ] C kodu okunabilir (printf kullanıyor)
- [ ] GCC derleme başarılı
- [ ] Program doğru çıktı veriyor

#### P2.3: Değişken Testi (30 dk)

```bash
cat > test_variables.mlp << 'EOF'
x = 42
y = 3.14
name = "Ali"

print("x =", x)
print("y =", y)
print("name =", name)
EOF

./mlpc test_variables.mlp -o test_variables.c
gcc test_variables.c -o test_variables -L../runtime -lmlp_runtime
./test_variables
```

**Beklenen:**
```
x = 42
y = 3.14
name = Ali
```

**Kontroller:**
- [ ] Değişken tanımları C'de doğru (int64_t, double, char*)
- [ ] Print doğru çalışıyor
- [ ] STO type mapping çalışıyor

#### P2.4: Control Flow Testi (30 dk)

```bash
cat > test_control.mlp << 'EOF'
x = 10

if x > 5 {
    print("x büyük")
}

i = 0
while i < 3 {
    print("i =", i)
    i = i + 1
}
EOF

./mlpc test_control.mlp -o test_control.c
gcc test_control.c -o test_control -L../runtime -lmlp_runtime
./test_control
```

**Beklenen:**
```
x büyük
i = 0
i = 1
i = 2
```

**Kontroller:**
- [ ] if statement çalışıyor
- [ ] while loop çalışıyor
- [ ] Karşılaştırma operatörleri doğru

#### P2.5: Function Testi (30 dk)

```bash
cat > test_functions.mlp << 'EOF'
function topla(a, b) {
    return a + b
}

function selamla(isim) {
    print("Merhaba", isim)
}

sonuc = topla(5, 3)
print("5 + 3 =", sonuc)

selamla("Dünya")
EOF

./mlpc test_functions.mlp -o test_functions.c
gcc test_functions.c -o test_functions -L../runtime -lmlp_runtime
./test_functions
```

**Beklenen:**
```
5 + 3 = 8
Merhaba Dünya
```

**Kontroller:**
- [ ] Function tanımı doğru
- [ ] Return çalışıyor
- [ ] Function call çalışıyor
- [ ] Parametreler doğru geçiyor

---

## ✅ TEST SUITE (Opsiyonel)

**Eğer `tests/` dizini varsa:**

```bash
cd MELP/C/stage0/tests
./run_all.sh
```

**Kontroller:**
- [ ] Tüm testler geçti
- [ ] Başarısız test varsa → detaylı inceleme

---

## 📊 İLERLEME RAPORU

### Başarı Kriterleri

- [ ] ✅ Stage0-C derleyici derlendi
- [ ] ✅ Hello World çalıştı
- [ ] ✅ Değişkenler çalışıyor
- [ ] ✅ Control flow çalışıyor
- [ ] ✅ Functions çalışıyor
- [ ] ✅ Tüm temel testler geçti

### Test Sonuçları Tablosu

| Test | Durum | Not |
|------|-------|-----|
| Stage0 Build | ⏳ | - |
| Hello World | ⏳ | - |
| Variables | ⏳ | - |
| Control Flow | ⏳ | - |
| Functions | ⏳ | - |
| Test Suite | ⏳ | - |

---

## 🚨 SORUN ÇÖZME

### Derleme Hataları

**Hata:** `undefined reference to emit_c`
**Çözüm:** YZ_01 emit_c() fonksiyonunu eklememiş → YZ_01'e bildir

**Hata:** `printf: undefined reference`
**Çözüm:** Runtime library link edilmemiş → `-lmlp_runtime` ekle

### Runtime Hataları

**Hata:** Segmentation fault
**Çözüm:** C kodunda pointer hatası → C çıktısını incele

**Hata:** Yanlış çıktı
**Çözüm:** Codegen hatası → YZ_01'e bildir

---

## 🔄 YZ_03'E GEÇİŞ

**P2 bittiğinde:**

1. YZ_RAPOR doldur
2. Üst Akıl'a bildir: "P2 tamamlandı, YZ_03 başlayabilir"
3. Test sonuçlarını belge

**YZ_03'ün görevi:** Stage1-MLP modüllerini derlemek

---

## 📚 REFERANSLAR

- `TODO_SELFHOSTING.md` - P2 bölümü
- `MELP/C/stage0/` - Stage0 kaynak kodu
- `MELP/runtime/` - Runtime library
- `YZ_01_Gorev_Plani.md` - Önceki phase

---

**YZ_01'İN İŞİNİ TEST EDİYORSUN! DİKKATLİ OL! 🧪**

*Üst Akıl seni destekliyor!* 🧠
