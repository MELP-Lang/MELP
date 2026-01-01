# 🔄 İki Aşamalı Git Workflow - Kullanım Kılavuzu

## 📋 Özet

İki git repository'niz var:
1. **MELP-GCC-WORKSHOP** (private) - Tüm geliştirme, TODO'lar, notlar
2. **MELP-GCC** (public) - Sadece profesyonel, temiz kod

## 🚀 Hızlı Başlangıç

### Seçenek 1: Quick Commit (Önerilen)
```bash
# Tek komutta her şeyi yap
./quick_commit.sh "feat: Add new feature"

# Script size soracak: Public repo'ya da sync yapılsın mı?
# y -> Her iki repo'ya da push
# n -> Sadece WORKSHOP'a push
```

### Seçenek 2: Manuel (Adım Adım)
```bash
# 1. WORKSHOP'a commit
git add .
git commit -m "feat: Add new feature"
git push origin main

# 2. PUBLIC'e sync (opsiyonel)
./sync_to_public.sh
cd /home/pardus/projeler/MLP/MELP-GCC
git add -A
git commit -m "sync: Add new feature"
git push origin main
```

## 📁 Ne Nereye Gidiyor?

### ✅ PUBLIC'e Giden İçerik
```
MELP-GCC/ (public)
├── C/                    ← MELP/C/
├── MLP/                  ← MELP/MLP/
├── runtime/              ← MELP/runtime/
├── bootstrap/            ← MELP/bootstrap/
├── examples/             ← examples/
├── tests/                ← tests/
├── docs/                 ← docs/
├── README.md             ← README_PUBLIC.md
├── LICENSE               ← Auto-generated (MIT)
├── CONTRIBUTING.md       ← Auto-generated
└── .gitignore            ← Auto-generated (public için)
```

### 🚫 WORKSHOP'ta Kalan İçerik (Private Only)
```
MELP-GCC-WORKSHOP/
├── *TODO*.md             🔒 Private
├── TODO_*/               🔒 Private
├── YZ_*.md               🔒 Private
├── UA_*.md               🔒 Private
├── MM_*.md               🔒 Private
├── çıktı.md              🔒 Private
├── kullanıcı_görüşü.md   🔒 Private
├── sync_to_public.sh     🔒 Private
├── quick_commit.sh       🔒 Private
├── create-current-todo.sh 🔒 Private
├── pre-commit-check.sh   🔒 Private
├── PUBLIC_REPO_SETUP.md  🔒 Private (bu dosya)
├── *.backup              🔒 Private
└── temp/                 🔒 Private
```

## 🎯 Tipik Workflow Örnekleri

### Senaryo 1: Sadece Not Aldınız (TODO güncelleme)
```bash
# TODO dosyası güncellediniz
git add .
git commit -m "docs: Update TODO list"
git push origin main

# Public'e sync YAPMAYA GEREK YOK
# Çünkü TODO dosyaları zaten filtreleniyor
```

### Senaryo 2: Kod Değişikliği (Özellik Ekleme)
```bash
# Kod yazdınız, test ettiniz
./quick_commit.sh "feat: Add string interpolation"

# Script soracak: Public'e sync?
# -> y (evet) → Her iki repo da güncellenir
```

### Senaryo 3: Dokümantasyon Güncelleme
```bash
# docs/ dizininde değişiklik yaptınız
./quick_commit.sh "docs: Improve getting started guide"

# Public'e sync yapın (dokümantasyon public'te görünmeli)
```

### Senaryo 4: Acil Bug Fix
```bash
# Hızlıca düzelttiniz
git add .
git commit -m "fix: Critical parser bug"
git push origin main

# Sonra public'e sync
./sync_to_public.sh
cd /home/pardus/projeler/MLP/MELP-GCC
git add -A
git commit -m "sync: Critical parser bug fix"
git push origin main
```

## 🛠️ Özel Durumlar

### Public README'yi Güncelleme
```bash
# README_PUBLIC.md'yi düzenleyin (WORKSHOP'ta)
nano README_PUBLIC.md

# Sync yaptığınızda otomatik olarak README.md olarak kopyalanır
./sync_to_public.sh
```

### Sadece Belirli Dosyaları Public'e Göndermek
sync_to_public.sh scripti zaten filtreleme yapıyor, ama ek filtre eklemek isterseniz:
```bash
# sync_to_public.sh'ı düzenleyin
nano sync_to_public.sh

# rsync --exclude parametresi ekleyin
rsync -av --delete \
    --exclude='.git' \
    --exclude='EKLEMEK_ISTEDIGINIZ_FILTRE' \
    ...
```

### Public Repo'yu Sıfırdan Klonlamak
```bash
cd /home/pardus/projeler/MLP
git clone https://github.com/MELP-Lang/MELP-GCC.git

# İlk sync
cd MELP-GCC-WORKSHOP
./sync_to_public.sh
```

## 🔍 Kontrol ve Doğrulama

### Public'e Ne Gideceğini Kontrol Etme
```bash
# Dry-run (test modu)
rsync -avn --exclude='.git' \
    /home/pardus/projeler/MLP/MELP-GCC-WORKSHOP/MELP/C/ \
    /home/pardus/projeler/MLP/MELP-GCC/C/

# n = dry-run (sadece göster, kopyalama)
```

### Public Repo'da Ne Var Görmek
```bash
cd /home/pardus/projeler/MLP/MELP-GCC
ls -la

# TODO dosyaları YOK olmalı
ls *TODO* 2>/dev/null || echo "✅ No TODO files (correct!)"
```

### WORKSHOP'ta Ne Var Görmek
```bash
cd /home/pardus/projeler/MLP/MELP-GCC-WORKSHOP
ls *TODO*

# TODO dosyaları VAR olmalı
```

## 📊 Commit Message Formatı (Önerilen)

Tutarlılık için semantic commit kullanın:

```
feat:     Yeni özellik
fix:      Bug düzeltme
docs:     Dokümantasyon
style:    Kod formatı (logic değişmedi)
refactor: Kod refactoring
test:     Test ekleme/düzeltme
chore:    Build, config vb.
```

Örnekler:
```bash
./quick_commit.sh "feat: Add HashMap implementation"
./quick_commit.sh "fix: Memory leak in string module"
./quick_commit.sh "docs: Add API reference"
./quick_commit.sh "refactor: Simplify parser logic"
```

## 🎨 Public Repo Profesyonelleştirme Checklist

- [x] LICENSE dosyası (MIT) - Auto-generated
- [x] CONTRIBUTING.md - Auto-generated
- [x] Professional README with badges
- [x] Clean .gitignore
- [ ] CHANGELOG.md (opsiyonel)
- [ ] CODE_OF_CONDUCT.md (opsiyonel)
- [ ] GitHub Actions CI/CD (opsiyonel)
- [ ] GitHub Issues templates (opsiyonel)
- [ ] GitHub PR templates (opsiyonel)

## 🚨 Önemli Hatırlatmalar

1. **TODO dosyaları asla public'e gitMEZ** - .gitignore bunu engelliyor
2. **Sync yapmadan önce WORKSHOP'a commit yapın** - Değişiklikler kaybolmasın
3. **Public'e push yapmadan önce değişiklikleri kontrol edin** - `git diff` ile
4. **İki repo'yu karıştırmayın** - Hangi dizinde olduğunuzu kontrol edin

## 💡 İpuçları

- Her gün sonunda quick_commit kullanın
- Büyük özellikler için branch kullanın (her iki repo'da da)
- Public'e sync'i haftalık yapabilirsiniz (acil değilse)
- README_PUBLIC.md'yi public için özel tutun

## 🆘 Sorun Giderme

### "git push failed"
```bash
# Remote branch'i pull edin
git pull origin main
git push origin main
```

### "rsync: file not found"
```bash
# Dosya yolu doğru mu kontrol edin
ls -la /home/pardus/projeler/MLP/MELP-GCC-WORKSHOP/MELP/
```

### "Public repo bulunamadı"
```bash
# Klonlayın
cd /home/pardus/projeler/MLP
git clone https://github.com/MELP-Lang/MELP-GCC.git
```

## 📞 Yardım

Sorularınız için:
- Bu dosyayı okuyun: PUBLIC_REPO_SETUP.md
- Scriptleri inceleyin: sync_to_public.sh, quick_commit.sh
- Git durumunu kontrol edin: git status, git log

---

**Hazırlayan:** GitHub Copilot  
**Tarih:** 31 Aralık 2025  
**Versiyon:** 1.0
