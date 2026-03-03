# MELP Belgeler Dizini

**Stage 1 Proje Dokümantasyonu**  
**Güncelleme:** 3 Şubat 2026

---

## 📁 Klasör Yapısı

### 🎯 [melp_felsefesi/](melp_felsefesi/) - Çığır Açan Konseptler ⭐

MELP'in özgün ve game-changing özelliklerinin detaylı dokümantasyonu:

- **[MELP_OWNERSHIP_MODEL.md](melp_felsefesi/MELP_OWNERSHIP_MODEL.md)** - Radical Statelessness ile memory safety
- **[FBO_FAST_BINARY_OBJECTS.md](melp_felsefesi/FBO_FAST_BINARY_OBJECTS.md)** - 4-byte compact encoding
- **[README.md](melp_felsefesi/README.md)** - Konseptlerin kataloğu ve akademik değer

**Neden ayrı klasör?** Filozofik konseptler vs implementation detayları. Akademik araştırma potansiyeli olan fikirler.

---

### 📚 Ana Belgeler (Root)

#### Mimari ve Strateji
- **ARCHITECTURE.md** - Compiler mimarisi
- **BOOTSTRAP_STRATEGY.md** - Bootstrap yaklaşımı
- **STAGE0_TO_STAGE1_GUIDE.md** - Stage 0 → Stage 1 transition
- **BUILD_SYSTEM.md** - CMake + LLVM build
- **LAYER_ARCHITECTURE_BRIEF.md** - Katmanlı mimari

#### Core Felsefe
- **MELP_PHILOSOPHY.md** - 7 temel esas ⭐
- **MLP MODÜL FELSEFESİ.md** - "Her modül ölüdür" ⭐
- **modüler_mimari.md** - Peer-to-peer network pattern
- **MELP_REFERENCE.md** - Dil referansı

#### Optimizasyon (→ melp_felsefesi'ne taşınacak)
- **STO_PRINCIPLES.md** - Smart Type Optimization
- **SAO_PRINCIPLES.md** - Semantic-Aware Optimization
- **STO_PERFORMANCE.md** - STO performance analizi
- **STO_SAO_IMPLEMENTATION_ROADMAP.md** - Implementation planı
- **FBO_COMPACT_ENCODING.md** - (→ melp_felsefesi/FBO_FAST_BINARY_OBJECTS.md)

#### Memory Management
- **HEAP_MANUAL_MEMORY_GUIDE.md** - Manual memory yönetimi
- **MODULAR_GC_NOTES.md** - GC ile ilgili notlar

#### Stage 0 Dökümanları
- **STAGE0_LIMITATIONS.md** - Stage 0 limitleri
- **LESSONS_LEARNED.md** - Geçmiş projelerden dersler
- **WEEK5_COMPLETION_REPORT.md** - İlerleme raporu

#### Özel Modüller
- **FFI_USER_GUIDE.md** - Foreign Function Interface
- **DEBUG_MODULE_BRIEFING.md** - Debug modülü
- **PARADIGM2_MODULAR_VISION.md** - Paradigma 2 vizyonu
- **MM_02_DEVIR_TESLIM.md** - Modül devir teslim
- **MM_TALIMAT_PARADIGM2_POC.md** - POC talimatları

#### Test ve Kalite
- **TEST_COVERAGE_STRATEGY.md** - Test stratejisi
- **CONCURRENCY_PREP_READING_LIST.md** - Concurrency hazırlık
- **OWNERSHIP_PREP_READING_LIST.md** - Ownership hazırlık
- **PARALEL_POC_PLAN.md** - Parallel POC planı

#### Syntax ve Dil
- **kurallar_kitabı_v1.md** - Kurallar
- **pmlp_sozdizimi.md** - PMLP sözdizimi
- **pmlp0_sozdizimi.md** - PMLP0 sözdizimi
- **pmlp1_sozdizimi.md** - PMLP1 sözdizimi
- **YZ_SYNTAX_TRAINING_STRATEGY.md** - Syntax öğrenme

#### Methodology
- **PROJECT_MANAGEMENT_METHODOLOGY.md** - Proje yönetimi
- **SAO_PRINCIPLES.md** - SAO prensipleri
- **IDEAL_STANDALONE_PLAN.md** - Standalone plan
- **PMLP_IMPROVEMENTS.md** - İyileştirmeler

---

## 🎓 Okuma Sırası (Yeni Başlayanlar)

### 1. Başlangıç (Felsefe)
1. **[MELP_PHILOSOPHY.md](MELP_PHILOSOPHY.md)** - 7 temel esas
2. **[melp_felsefesi/MELP_OWNERSHIP_MODEL.md](melp_felsefesi/MELP_OWNERSHIP_MODEL.md)** - Memory safety
3. **[MLP MODÜL FELSEFESİ.md](MLP%20MODÜL%20FELSEFESİ.md)** - Modül yaklaşımı

### 2. Syntax
4. **pmlp0_sozdizimi.md** - Temel syntax
5. **MELP_REFERENCE.md** - Referans

### 3. Mimari
6. **ARCHITECTURE.md** - Compiler mimarisi
7. **[melp_felsefesi/FBO_FAST_BINARY_OBJECTS.md](melp_felsefesi/FBO_FAST_BINARY_OBJECTS.md)** - Encoding
8. **BUILD_SYSTEM.md** - Build süreci

### 4. Optimizasyon
9. **STO_PRINCIPLES.md** - Type optimization
10. **SAO_PRINCIPLES.md** - Semantic optimization

---

## 🔧 Geliştirici Kaynakları

### Yeni Özellik Eklemek
- **STAGE0_TO_STAGE1_GUIDE.md** - Nasıl eklenecek
- **PROJECT_MANAGEMENT_METHODOLOGY.md** - Metodoloji

### Debug ve Test
- **DEBUG_MODULE_BRIEFING.md** - Debug araçları
- **TEST_COVERAGE_STRATEGY.md** - Test yaklaşımı

### FFI ve C++ Entegrasyonu
- **FFI_USER_GUIDE.md** - FFI kullanımı
- **HEAP_MANUAL_MEMORY_GUIDE.md** - Memory yönetimi

---

## 📊 Belge Durumu

### ✅ Production Ready (melp_felsefesi/)
- Ownership Model
- FBO
- (Daha fazlası eklenecek...)

### 🔄 Taşınması Planlanan
- STO_PRINCIPLES.md → melp_felsefesi/
- SAO_PRINCIPLES.md → melp_felsefesi/
- STO_SAO_IMPLEMENTATION_ROADMAP.md → melp_felsefesi/

### 📝 Güncellenecek
- ARCHITECTURE.md (FBO referansları)
- BUILD_SYSTEM.md (güncel yapı)

---

## 🎯 Klasör Reorganizasyon Planı

Belgeler klasörü çok dolu (50+ dosya). Temizlik planı:

### Phase 1: Felsefe Ayrımı ✅
- [x] melp_felsefesi/ klasörü oluştur
- [x] Ownership Model dokümante et
- [x] FBO dokümante et
- [ ] STO/SAO taşı

### Phase 2: Kategorilere Ayır
- [ ] language/ - Syntax ve dil belgeleri
- [ ] architecture/ - Mimari belgeler
- [ ] guides/ - Kullanıcı kılavuzları
- [ ] research/ - Araştırma notları
- [ ] stage0/ - Stage 0 spesifik

### Phase 3: Arşivleme
- [ ] Deprecated dosyaları archived/ klasörüne taşı
- [ ] Eski POC planlarını arşivle
- [ ] Tarihsel belgeleri ayır

---

## 📖 Notlar

**Belge Standardı:**
- Markdown formatı
- Front matter (tarih, durum, kategori)
- Başlık yapısı (##, ###)
- Code örnekleri
- Karşılaştırma tabloları

**Güncelleme:**
- Her değişiklikte "Son Güncelleme" tarihini güncelle
- Durum etiketlerini koru (✅, 🔄, 📝, ❌)
- Breaking change'lerde VERSION numarası artır

---

**Son Güncelleme:** 3 Şubat 2026  
**Toplam Belge:** 50+ dosya  
**Reorganizasyon:** Devam ediyor

