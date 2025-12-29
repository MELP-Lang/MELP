# P2 (MLP-LLVM) Modül Entegrasyon Yönergesi

**Tarih:** 29 Aralık 2025  
**Güncelleme:** 29 Aralık 2025  
**Hazırlayan:** Proje Danışmanı (YZ)  
**Yetkili:** Sonraki YZ veya Kullanıcı onayı ile

---

## 📦 Bu Dizindeki Modüller

P2 projesinden (MLP-LLVM) alınan **36 modül/dizin**, henüz entegre EDİLMEMİŞ:

### Compiler Modülleri (32)
| Modül | Açıklama | P1'de Var mı? |
|-------|----------|---------------|
| `arithmetic/` | Aritmetik işlemler | ✅ Var |
| `array/` | Dizi işlemleri | ✅ Var |
| `async/` | Async/await | ✅ Var |
| `bitwise_operations/` | Bit işlemleri | ✅ Var |
| `codegen_context/` | Codegen context pattern | ✅ Var |
| `comments/` | Yorum işleme | ✅ Var |
| `comparison/` | Karşılaştırma | ✅ Var |
| `control_flow/` | Kontrol akışı | ✅ Var |
| `debug/` | Debug desteği | ✅ Var (debug_features) |
| `enum/` | Enum yapısı | ✅ Var |
| `error/` | Hata yönetimi | ❌ Yok |
| `expression/` | İfade işleme | ✅ Var |
| `file_io/` | Dosya I/O | ✅ Var |
| `for_loop/` | For döngüsü | ❌ Yok (ayrı modül) |
| `functions/` | Fonksiyon işleme | ✅ Var |
| `import/` | Modül import sistemi | ❌ Yok |
| `lambda/` | Lambda fonksiyonlar | ✅ Var |
| `lexer/` | Lexer modülü | ✅ Var (karşılaştır) |
| `logical/` | Mantıksal işlemler | ✅ Var |
| `memory/` | Bellek yönetimi | ✅ Var |
| `null_safety/` | Null güvenliği | ✅ Var |
| `optimization_pass/` | Optimizasyon | ✅ Var |
| `parser_core/` | Parser çekirdeği | ✅ Var |
| `print/` | Print işlemi | ✅ Var |
| `runtime_sto/` | Runtime STO | ✅ Var |
| `statement/` | Statement işleme | ✅ Var |
| `sto_runtime/` | STO runtime | ✅ Var |
| `string_operations/` | String işlemleri | ✅ Var |
| `struct/` | Struct yapısı | ✅ Var |
| `switch/` | Switch-case | ❌ Yok (ayrı modül) |
| `type_system/` | Tip sistemi | ✅ Var |
| `variable/` | Değişken işleme | ✅ Var |

### Runtime Modülleri (3)
| Modül | Açıklama |
|-------|----------|
| `runtime_stdlib/` | Standard kütüphane |
| `runtime_memory/` | Bellek yönetimi |
| `runtime_wrapper/` | Wrapper fonksiyonlar |

### MLP Modülleri (1 dizin, 9 dosya)
| Modül | Açıklama |
|-------|----------|
| `mlp_modules/` | MLP ile yazılmış compiler modülleri |

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
