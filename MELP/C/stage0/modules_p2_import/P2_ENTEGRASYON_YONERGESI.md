# P2 (MLP-LLVM) Modül Entegrasyon Yönergesi

**Tarih:** 29 Aralık 2025  
**Hazırlayan:** Proje Danışmanı (YZ)  
**Yetkili:** Sonraki YZ veya Kullanıcı onayı ile

---

## 📦 Bu Dizindeki Modüller

P2 projesinden (MLP-LLVM) alınan, henüz entegre EDİLMEMİŞ modüller:

| Modül | Kaynak | Açıklama | P1'de Var mı? |
|-------|--------|----------|---------------|
| `import/` | P2 | Modül import sistemi | ❌ Yok |
| `for_loop/` | P2 | For döngüsü (ayrı modül) | ❌ Yok |
| `switch/` | P2 | Switch-case yapısı | ❌ Yok |
| `error/` | P2 | Hata yönetimi | ❌ Yok |
| `lexer/` | P2 | Lexer modülü | ✅ Var (karşılaştır) |
| `debug/` | P2 | Debug desteği | ✅ Var (debug_features) |

---

## ⚠️ ENTEGRASYON KURALLARI

### 1. Backend Farkı
```
P2: LLVM backend → llvm_backend/ modülü kullanır
P1/MLP-GCC: GCC backend → C kodu üretir

❌ llvm_backend/ ALINMADI - uyumsuz
✅ Diğer modüller backend-agnostic
```

### 2. Codegen Pattern Farkı
```c
// P2 Pattern (CodegenContext):
void codegen_import(CodegenContext* ctx, ASTNode* node);

// P1 Pattern (fprintf):
void codegen_import(FILE* out, ASTNode* node);
```
**Aksiyon:** P2 modüllerini P1 pattern'ına adapte et veya CodegenContext'i MLP-GCC'ye taşı.

### 3. Entegrasyon Sırası
```
1. import/     → En kritik, modül sistemi için gerekli
2. for_loop/   → Döngü desteği genişletme
3. switch/     → Pattern matching temeli
4. error/      → Hata yönetimi
5. lexer/      → P1 ile karşılaştır, daha iyisini seç
6. debug/      → P1'deki debug_features ile birleştir
```

---

## 📋 ENTEGRASYON ADIMLARI

### Adım 1: Modülü İncele
```bash
# Modül yapısını gör
ls -la modules_p2_import/import/

# Header dosyasını oku
cat modules_p2_import/import/import.h
```

### Adım 2: P1 Karşılığını Bul
```bash
# P1'de benzer işlev var mı?
grep -r "import" ../modules/

# Varsa karşılaştır
diff modules_p2_import/import/import.c ../modules/XXX/XXX.c
```

### Adım 3: Backend Adaptasyonu
```c
// P2'de LLVM çağrısı varsa:
LLVMBuildCall(...);  // ❌ Kaldır

// GCC uyumlu C kodu üret:
fprintf(out, "// import: %s\n", module_name);  // ✅
```

### Adım 4: Test Et
```bash
# Modülü ana modules/ dizinine taşı
mv modules_p2_import/import ../modules/

# Derleme testi
make -C ../modules/import/
```

### Adım 5: Entegrasyonu Kaydet
```bash
# Bu dosyayı güncelle - hangi modül entegre edildi
# Git commit at
git add ../modules/import/
git commit -m "Entegrasyon: P2'den import modülü"
```

---

## ✅ ENTEGRASYON DURUMU

| Modül | Durum | Tarih | Notlar |
|-------|-------|-------|--------|
| import/ | ⏳ Bekliyor | - | - |
| for_loop/ | ⏳ Bekliyor | - | - |
| switch/ | ⏳ Bekliyor | - | - |
| error/ | ⏳ Bekliyor | - | - |
| lexer/ | ⏳ Bekliyor | - | P1 ile karşılaştır |
| debug/ | ⏳ Bekliyor | - | debug_features ile birleştir |

---

## 🔴 ÖNEMLİ UYARILAR

1. **LLVM bağımlılığı kontrol et** - `#include <llvm-c/...>` varsa adapte et
2. **CodegenContext pattern** - P1 pattern'ına çevir veya tüm projeye taşı
3. **Test** - Her entegrasyondan sonra bootstrap test çalıştır
4. **5 Temel Esas** - `Danışman_Yol_Haritası.md` kurallarına uy

---

## 📞 İLETİŞİM

Sorular için:
- `kullanıcı_talimatı.md` → Kullanıcıya sor
- `danışman_talimatı.md` → Danışman notları
- `Danışman_Yol_Haritası.md` → Değiştirilemez kurallar

---

*Bu yönerge P2→MLP-GCC modül transferi için hazırlanmıştır.*
