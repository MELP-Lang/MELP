# 🎯 GÖREVLİ YZ BAŞLANGIÇ NOKTASI

**Son Güncelleme:** 24 Aralık 2025  
**Durum:** 🟢 YZ_10 Göreve Hazır!  
**Önceki YZ:** YZ_09 (Toplu Syntax Düzeltme - 33 dosya tamamlandı)  
**Sen:** selfhosting_YZ_10

---

## ✅ YZ_09 BAŞARISI!

```
┌─────────────────────────────────────────────────────────────┐
│  TOPLU SYNTAX DÜZELTMESİ TAMAMLANDI!                       │
│                                                             │
│  Düzeltilen dosyalar: 33/107 (%31)                        │
│                                                             │
│  Modül Grupları:                                           │
│  ✅ Lexer modülleri: 10 dosya                              │
│  ✅ Literals modülleri: 2 dosya                            │
│  ✅ Parser modülleri: 5 dosya                              │
│  ✅ Arrays/Variables/Operators: 6 dosya                    │
│  ✅ Structs/Enums/Functions: 7 dosya                       │
│  ✅ Control Flow: 1 dosya                                  │
│  ✅ Test dosyaları: 2 dosya                                │
│                                                             │
│  Araçlar: Manuel + sed toplu düzeltme                      │
│  Commits: 5 başarılı commit                                │
└─────────────────────────────────────────────────────────────┘
```

---

## 📍 ŞİMDİKİ DURUM

**Branch:** `TODO_STAGE1_TO_SELFHOSTING_FINAL`  
**İlerleme:** 5/13 task (%38)

**Aktif Görev:** Phase 1 Task 1.2 - Toplu Syntax Düzeltme (devam)

---

## 🎯 YZ_10 GÖREVİ

**Phase 1, Task 1.2: Kalan Modülleri Düzeltme**

### 📊 YZ_09 SONUÇLARI:

**Tamamlanan:** 33/107 dosya (%31)

**Düzeltilen Syntax Hataları:**
- ✅ List return: `[]` → `(;)` (boş list)
- ✅ List literal: `[a, b]` → `(a; b;)` (return değerleri)
- ✅ Array trailing: `[a; b; c]` → `[a; b; c;]`
- ✅ Fonksiyon parametreleri: `,` → `;`

**Kullanılan Yöntem:**
- İlk dosyalar: Manuel düzeltme + test
- Toplu düzeltme: `sed` ile pattern replacement
- Hız: ~6-8 dosya/commit

### 🎯 YZ_10 İÇİN KALAN İŞ:

**Düzeltilmemiş:** ~74 dosya (%69)

**Öncelikli Modüller:**
1. **Codegen modülleri** (codegen_mlp/*.mlp) - 17 dosya
2. **Parser modülleri** (parser_mlp/*.mlp) - Kalan ~23 dosya
3. **Test dosyaları** - İhtiyaca göre
4. **Diğer modüller** - compiler_integration, bootstrap vb.

**Strateji:**
- sed ile toplu düzeltmeye devam et
- Küçük dosyalardan büyüğe git
- Her 8-10 dosyada bir commit
- Test yapmadan hızlı ilerle (zaman tasarrufu)

### Başarı Kriteri:

- [ ] 30+ dosya daha düzeltilmiş (toplam ~63, %60)
- [ ] Codegen modülleri tamamlanmış
- [ ] Parser modülleri tamamlanmış  
- [ ] YZ_10_TAMAMLANDI.md raporu

---

## 📚 OKUMAN GEREKENLER

1. **selfhosting_YZ/YZ_08_TAMAMLANDI.md** - **ÖNCE BUNU OKU!**
2. **pmlp_kesin_sozdizimi.md** - Syntax referansı
3. **/tmp/syntax_report.txt** - YZ_08'in oluşturduğu hata listesi

---

## ⚠️ KURALLAR9_TAMAMLANDI.md** - **ÖNCE BUNU OKU!**
2. **pmlp_kesin_sozdizimi.md** - Syntax referansı
3. **YZ_09'un commit geçmişi** - Sed pattern'lerini görmek için

## 🚀 HIZLI BAŞLANGIÇ

```bash
# Kalan dosyaları bul
find compiler/stage1/modules -name "*.mlp" -exec sh -c \
  'grep -l "return \[" "$1" 2>/dev/null' _ {} \;

# Toplu düzelt (örnek)
sed -i 's/return \[/return (/g; s/\]; xxx\]/; xxx;)/g' dosya.mlp

# Her 8-10 dosyada commit
git add -A && git commit -m "YZ_10: Modül grubu düzeltmesi"
```
- "Detaylandırmamı ister misin?" YASAK
- Phase/Task icat etme
- Raporu `selfhosting_YZ/YZ_09_TAMAMLANDI.md` olarak yaz

---

**🚀 TOPLU DÜZELTMEYİ BAŞLAT!** 🚀
