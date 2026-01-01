# MELP-GCC Public Repository Setup Guide

Bu rehber, MELP-GCC public repository'sinin profesyonel bir açık kaynak projesi olarak yapılandırılması için gerekli adımları içerir.

## 📋 Mevcut Durum vs Hedef Yapı

### Mevcut Yapı (WORKSHOP)
```
MELP-GCC-WORKSHOP/
├── MELP/                    # Ana kaynak dizin
│   ├── C/
│   ├── MLP/
│   ├── compiler/           # Sadece WORKSHOP'ta kalacak
│   ├── normalizer/         # Sadece WORKSHOP'ta kalacak
│   ├── runtime/
│   └── bootstrap/
├── *TODO*.md               # Private only
├── çıktı.md                # Private only
├── sync_to_public.sh       # Private only
└── ...
```

### Hedef Yapı (PUBLIC)
```
MELP-GCC/
├── README.md               ✅ Profesyonel açıklama + badges
├── LICENSE                 ✅ MIT License
├── CONTRIBUTING.md         ✅ Katkı rehberi
├── CHANGELOG.md            📦 Sürüm geçmişi (opsiyonel)
├── .gitignore              ✅ Temiz ignore listesi
│
├── C/                      ✅ Stage0 compiler (C codebase)
├── MLP/                    ✅ Stage1 compiler (MLP codebase)
├── runtime/                ✅ Runtime library (C)
├── bootstrap/              ✅ Bootstrap components
│
├── examples/               ✅ Örnek programlar
├── tests/                  ✅ Test suite
├── docs/                   ✅ Dokümantasyon
│
└── scripts/                📦 Build/test scripts (opsiyonel)
```

## 🚀 Kullanım: İki Aşamalı Git Workflow

### Adım 1: Private Repo'ya Commit & Push
```bash
# WORKSHOP dizininde çalış
cd /home/pardus/projeler/MLP/MELP-GCC-WORKSHOP

# Normal git workflow
git add .
git commit -m "feat: Add new feature"
git push origin main
```

### Adım 2: Public Repo'ya Sync
```bash
# Sync script'ini çalıştır
./sync_to_public.sh

# Public repo'ya git
cd /home/pardus/projeler/MLP/MELP-GCC

# Değişiklikleri kontrol et
git status
git diff

# Commit ve push
git add -A
git commit -m "sync: Update from workshop $(date +%Y-%m-%d)"
git push origin main
```

## 📝 Script Ne Yapar?

`sync_to_public.sh` şunları yapar:

1. **MELP/ içeriğini root'a kopyalar:**
   - `MELP/C/` → `C/`
   - `MELP/MLP/` → `MLP/`
   - `MELP/runtime/` → `runtime/`
   - `MELP/bootstrap/` → `bootstrap/`

2. **Public dizinleri kopyalar:**
   - `examples/` → temiz örnekler
   - `tests/` → test suite
   - `docs/` → dokümantasyon

3. **Gereksizleri filtreler:**
   - TODO dosyaları (private kalır)
   - Test artifacts (*.log, *.tmp)
   - Development scripts (*.sh)
   - Türkçe notlar (çıktı.md vb.)

4. **Profesyonel dosyalar oluşturur:**
   - LICENSE (MIT)
   - CONTRIBUTING.md
   - .gitignore (public için optimize)

## 🎯 Public Repo Profesyonelleştirme Önerileri

### 1. README.md Geliştir
```markdown
# Mevcut hali iyi ama eklenebilir:
- Badge'ler (build status, license, version)
- Daha fazla görsel (screenshot, GIF)
- Quick start daha önde
- Installation bölümü daha detaylı
```

### 2. LICENSE Ekle
Script otomatik MIT License ekliyor. Değiştirmek isterseniz:
- MIT: En liberal, önerilen
- Apache 2.0: Patent koruması ile
- GPL: Copyleft, fork'lar da açık kaynak olmalı

### 3. CONTRIBUTING.md
Script otomatik ekliyor. Özelleştirmek için:
- Code of Conduct ekle
- Commit message formatı belirt
- Development setup detaylandır

### 4. CHANGELOG.md (Opsiyonel)
Sürüm geçmişini takip için:
```markdown
# Changelog

## [0.9.0] - 2025-12-30
### Added
- Bootstrap-ready status achieved
- Self-hosting compiler

## [0.8.0] - 2025-12-20
### Added
- Runtime library improvements
...
```

### 5. Documentation Geliştir
`docs/` dizininde:
- Getting Started Guide
- Language Reference
- API Documentation
- Architecture Overview
- Examples Tutorial

## 🔒 Private'da Kalacak İçerik

`.gitignore` (WORKSHOP) bu dosyaları zaten engelliyor:
- `*TODO*.md`, `*TODO*.txt`
- `TODO_*/`, `TODO_OLUSTUR/`
- `YZ_*.md`, `UA_*.md`, `MM_*.md`, `PD_*.md`
- `çıktı.md`, `kullanıcı_görüşü.md`
- `create-current-todo.sh`
- `pre-commit-check.sh`
- `sync_to_public*.sh`
- `*.backup`

## 🎨 Profesyonel Görünüm İçin Ekstra Fikirler

### README.md Badge Örnekleri
```markdown
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![Version](https://img.shields.io/badge/version-0.9.0-blue.svg)]()
[![Language](https://img.shields.io/badge/language-MLP-orange.svg)]()
```

### Directory Tree (README.md için)
```markdown
## Project Structure
```
MELP-GCC/
├── C/          # Stage0 compiler (C implementation)
├── MLP/        # Stage1 compiler (MLP implementation)
├── runtime/    # Runtime library (GC, stdlib)
├── bootstrap/  # Bootstrap components
├── examples/   # Example MLP programs
├── tests/      # Test suite
└── docs/       # Documentation
```
```

### Social Links
```markdown
## Community
- 📫 Issues: https://github.com/MELP-Lang/MELP-GCC/issues
- 💬 Discussions: https://github.com/MELP-Lang/MELP-GCC/discussions
- 🌟 Star us on GitHub!
```

## 🔄 Workflow Özeti

1. **Geliştirme (WORKSHOP)**
   - Kod yaz, test et
   - TODO'lar, notlar, deneysel kod
   - Internal documentation
   - `git push origin main` (private)

2. **Public Release (Public Repo)**
   - `./sync_to_public.sh` çalıştır
   - Değişiklikleri gözden geçir
   - `git push origin main` (public)

## 📞 Yardım

Sorular için:
- GitHub Issues: Teknik sorular, bug reports
- GitHub Discussions: Genel tartışmalar
- Pull Requests: Kod katkıları

---

**Not:** Bu rehber WORKSHOP repo'sunda kalır, public repo'ya kopyalanmaz.
