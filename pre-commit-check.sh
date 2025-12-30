#!/bin/bash
# pre-commit-check.sh - YZ Commit Öncesi Zorunlu Kontrol
# Her YZ commit öncesi BU SCRIPT'İ ÇALIŞTIRMALI!

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "⏳ YZ COMMIT ÖNCESİ KONTROL BAŞLIYOR..."
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""

# TODO dosyasından izin verilen dosyaları oku
TODO_FILE="CURRENT_TODO.txt"
if [ ! -f "$TODO_FILE" ]; then
    echo -e "${RED}❌ HATA: CURRENT_TODO.txt bulunamadı!${NC}"
    echo "   TODO başlamadan önce bu dosyayı oluştur:"
    echo "   echo 'codegen_emit.c' > CURRENT_TODO.txt"
    exit 1
fi

ALLOWED_FILES=$(cat "$TODO_FILE")
echo -e "${YELLOW}📁 İzin verilen dosyalar:${NC}"
echo "$ALLOWED_FILES" | sed 's/^/   - /'
echo ""

# 0. READ-ONLY DOSYALAR (SABOTAJ ÖNLEMİ!)
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "0️⃣  READ-ONLY Dosya Kontrolü (Sabotaj Önlemi)"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

# Read-only dosyalar listesi (YZ ASLA değiştiremez!)
READ_ONLY_FILES=(
    "YZ_KURALLAR.md"
    "Danışman_Yol_Haritası.md"
    "docs/ARCHITECTURE_AND_MEMORY_STRATEGY.md"
    "docs/COMPILER_BACKENDS_COMPARISON.md"
    "docs/LANGUAGE_EVOLUTION_GUIDE.md"
    "docs/kurallar_kitabı_v1.md"
    "pre-commit-check.sh"
    "create-current-todo.sh"
    "TODO_OLUSTUR/setup_todo.sh"
)

# Progress update dosyaları (YZ güncelleyebilir ama SADECE işaretleme için!)
PROGRESS_FILES=(
    "TODO_SELFHOSTING.md"
    "NEXT_AI_START_HERE.md"
)

CHANGED_FILES=$(git diff --name-only HEAD)

if [ -z "$CHANGED_FILES" ]; then
    echo -e "${RED}❌ HATA: Hiçbir dosya değişmemiş!${NC}"
    exit 1
fi

# Read-only dosya kontrolü
SABOTAGE=0
for file in $CHANGED_FILES; do
    # Progress dosyaları için özel kontrol
    is_progress_file=0
    for progress_file in "${PROGRESS_FILES[@]}"; do
        if [[ "$file" == "$progress_file" ]]; then
            is_progress_file=1
            echo -e "${YELLOW}⚠️  Progress dosyası: $file${NC}"
            echo "   Sadece [x] işaretleme ve NEXT_AI güncelleme izni var!"
            echo "   Görev ekleme/değiştirme YASAK!"
            # Progress dosyası için izin ver (detaylı kontrol yapılabilir ama şimdilik izin)
            break
        fi
    done
    
    if [ $is_progress_file -eq 1 ]; then
        continue
    fi
    
    # Read-only dosya kontrolü
    for readonly_file in "${READ_ONLY_FILES[@]}"; do
        if [[ "$file" == "$readonly_file" ]] || [[ "$file" == *"TODO_TEMPLATE"* ]]; then
            echo -e "${RED}❌ SABOTAJ TESPİT EDİLDİ: $file${NC}"
            echo "   Bu dosya READ-ONLY! YZ değiştiremez!"
            echo ""
            echo "   🚨 BU BİR SABOTAJ GİRİŞİMİ!"
            echo "   Önceki 10+ deneme bu yüzden başarısız oldu."
            echo "   YZ'ler dökümantasyonu/TODO'yu değiştirerek projeyi sabote etti!"
            echo ""
            SABOTAGE=1
            break
        fi
    done
done

if [ $SABOTAGE -eq 1 ]; then
    echo ""
    echo -e "${RED}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${RED}❌ COMMIT REDDEDİLDİ - SABOTAJ!${NC}"
    echo -e "${RED}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo ""
    echo "Sen YZ'sin, SADECE KOD yazabilirsin!"
    echo "Dökümantasyon, TODO, kurallar DEĞİŞTİRİLEMEZ!"
    echo ""
    echo "İzin verilen dosyalar:"
    cat "$TODO_FILE" | sed 's/^/   - /'
    echo "   - YZ_RAPOR.md (sadece kendin için)"
    echo ""
    echo "git reset --hard HEAD  # Tüm değişiklikleri geri al"
    exit 1
fi

echo -e "${GREEN}✅ Read-only dosyalara dokunulmamış${NC}"
echo ""

# 1. Değişen dosyaları kontrol et
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "1️⃣  Dosya Değişikliği Kontrolü (Scope)"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

echo -e "${YELLOW}📝 Değişen dosyalar:${NC}"
echo "$CHANGED_FILES" | sed 's/^/   - /'
echo ""

# Her değişen dosyayı kontrol et (YZ_RAPOR.md hariç)
UNAUTHORIZED=0
for file in $CHANGED_FILES; do
    # Dosya adını normalize et (path olmadan)
    filename=$(basename "$file")
    
    # YZ_RAPOR.md her zaman izinli
    if [[ "$filename" == "YZ_RAPOR.md" ]]; then
        echo -e "${GREEN}✅ İzinli (Rapor): $file${NC}"
        continue
    fi
    
    # İzin listesinde var mı?
    if ! echo "$ALLOWED_FILES" | grep -q "$filename"; then
        echo -e "${RED}❌ İZİNSİZ DEĞİŞİKLİK: $file${NC}"
        echo "   TODO'da izin verilen dosyalar:"
        echo "$ALLOWED_FILES" | sed 's/^/      - /'
        UNAUTHORIZED=1
    else
        echo -e "${GREEN}✅ İzinli: $file${NC}"
    fi
done

if [ $UNAUTHORIZED -eq 1 ]; then
    echo ""
    echo -e "${RED}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo -e "${RED}❌ COMMIT REDDEDİLDİ!${NC}"
    echo -e "${RED}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
    echo ""
    echo "Sadece TODO'da belirtilen dosyalara dokunabilirsin!"
    exit 1
fi

echo ""

# 2. Test kontrolü
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "2️⃣  Test Kontrolü"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

# Eğer MELP/C/stage0 içinde değişiklik varsa test et
if echo "$CHANGED_FILES" | grep -q "MELP/C/stage0"; then
    echo "MELP/C/stage0'da değişiklik var, test ediliyor..."
    
    cd MELP/C/stage0 || exit 1
    
    # Derleme
    echo "🔨 Derleniyor..."
    if ! make clean > /dev/null 2>&1 || ! make > /dev/null 2>&1; then
        echo -e "${RED}❌ DERLEME HATASI!${NC}"
        echo "make clean && make çalıştır ve hataları düzelt!"
        exit 1
    fi
    echo -e "${GREEN}✅ Derleme başarılı${NC}"
    
    # Test
    echo "🧪 Testler çalıştırılıyor..."
    if [ -f "run_tests.sh" ]; then
        if ! ./run_tests.sh > /dev/null 2>&1; then
            echo -e "${RED}❌ TESTLER FAIL!${NC}"
            echo "./run_tests.sh çalıştır ve hataları gör!"
            exit 1
        fi
    elif [ -f "Makefile" ] && grep -q "^test:" Makefile; then
        if ! make test > /dev/null 2>&1; then
            echo -e "${RED}❌ TESTLER FAIL!${NC}"
            echo "make test çalıştır ve hataları gör!"
            exit 1
        fi
    else
        echo -e "${YELLOW}⚠️  Test script bulunamadı, atlıyor...${NC}"
    fi
    echo -e "${GREEN}✅ Testler başarılı${NC}"
    
    cd - > /dev/null || exit 1
else
    echo -e "${YELLOW}ℹ️  MELP/C/stage0 dışında değişiklik, test atlanıyor${NC}"
fi

echo ""

# 3. Rapor kontrolü
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "3️⃣  Rapor Kontrolü"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

# YZ_RAPOR.md var mı?
if [ ! -f "YZ_RAPOR.md" ]; then
    echo -e "${RED}❌ RAPOR BULUNAMADI: YZ_RAPOR.md${NC}"
    echo "   Commit öncesi YZ_RAPOR.md oluşturmalısın!"
    echo "   Template: YZ_HIZLI_REFERANS.md'de var"
    exit 1
fi

# Rapor boş mu?
if [ ! -s "YZ_RAPOR.md" ]; then
    echo -e "${RED}❌ RAPOR BOŞ: YZ_RAPOR.md${NC}"
    exit 1
fi

# Minimum içerik kontrolü
if ! grep -q "YAPILAN" "YZ_RAPOR.md" || ! grep -q "TEST" "YZ_RAPOR.md"; then
    echo -e "${RED}❌ RAPOR EKSİK: YZ_RAPOR.md${NC}"
    echo "   Rapor şu bölümleri içermeli:"
    echo "   - ## ✅ YAPILAN"
    echo "   - ## 🧪 TEST"
    exit 1
fi

echo -e "${GREEN}✅ Rapor mevcut ve dolu${NC}"
echo ""

# 4. Commit mesajı önerisi
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "4️⃣  Commit Mesajı Önerisi"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

echo "Şu formatta commit yap:"
echo ""
echo -e "${GREEN}git commit -m \"feat: [ne yaptın kısa açıklama]\"${NC}"
echo ""
echo "Örnek:"
echo "  git commit -m \"feat: emit_c() fonksiyonu eklendi\""
echo "  git commit -m \"fix: parser null pointer hatası düzeltildi\""
echo ""

# TÜM KONTROLLER BAŞARILI
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo -e "${GREEN}✅ TÜM KONTROLLER BAŞARILI!${NC}"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""
echo "Şimdi commit yapabilirsin:"
echo "  git add ."
echo "  git commit -m \"feat: [açıklama]\""
echo ""
echo -e "${YELLOW}💡 İPUCU: YZ_RAPOR.md'yi de commit'e ekle!${NC}"
echo "  git add YZ_RAPOR.md"
echo ""
