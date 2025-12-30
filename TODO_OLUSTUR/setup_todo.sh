#!/bin/bash
# setup_todo.sh - Yeni TODO Oluşturma Scripti
# Kullanım: ./setup_todo.sh TODO_KISA TODO_TAM TODO_BELGE

set -e  # Hata olursa dur

# Renkler
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Banner
echo -e "${BLUE}╔════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║   TODO OLUŞTURMA SİSTEMİ              ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════╝${NC}"
echo ""

# Parametreleri kontrol et
if [ "$#" -ne 3 ]; then
    echo -e "${RED}❌ HATA: Yanlış parametre sayısı${NC}"
    echo ""
    echo "Kullanım:"
    echo "  $0 TODO_KISA TODO_TAM TODO_BELGE"
    echo ""
    echo "Örnek:"
    echo "  $0 RUNTIME RUNTIME_INT 2-TODO_RUNTIME_INTEGRATION.md"
    echo ""
    echo "Parametreler:"
    echo "  TODO_KISA  : Kısa ad (STAGE0, RUNTIME, BACKEND)"
    echo "  TODO_TAM   : Tam ad (STAGE0_PMLP, RUNTIME_INT, BACKEND_LLVM)"
    echo "  TODO_BELGE : TODO belge adı (1-TODO_....md)"
    exit 1
fi

TODO_KISA=$1
TODO_TAM=$2
TODO_BELGE=$3

# Project config dosyasını oku (opsiyonel)
if [ -f "project_config.yaml" ]; then
    PROJECT_NAME=$(grep "name:" project_config.yaml | head -1 | awk '{print $2}' | tr -d '"')
else
    # Varsayılan proje adı - parent dizin adından al (TODO_OLUSTUR'un üstü)
    PROJECT_NAME=$(basename "$(cd .. && pwd)")
    echo -e "${YELLOW}ℹ️  project_config.yaml bulunamadı, proje adı: $PROJECT_NAME${NC}"
fi

echo -e "${GREEN}📋 Yapılandırma:${NC}"
echo "  Proje      : $PROJECT_NAME"
echo "  TODO Kısa  : $TODO_KISA"
echo "  TODO Tam   : $TODO_TAM"
echo "  TODO Belge : $TODO_BELGE"
echo ""

# Template kontrolü
if [ ! -d "TODO_TEMPLATE" ]; then
    echo -e "${RED}❌ HATA: TODO_TEMPLATE/ bulunamadı!${NC}"
    exit 1
fi

# Hedef klasör kontrolü (root dizinde oluştur)
TARGET_DIR="../TODO_${TODO_TAM}"
if [ -d "$TARGET_DIR" ]; then
    echo -e "${YELLOW}⚠️  UYARI: $TARGET_DIR zaten mevcut!${NC}"
    read -p "Üzerine yazılsın mı? (y/N) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        echo -e "${BLUE}ℹ️  İptal edildi.${NC}"
        exit 0
    fi
    rm -rf "$TARGET_DIR"
fi

echo -e "${BLUE}📁 TODO klasörü oluşturuluyor...${NC}"

# Absolute path'i hemen al
SCRIPT_DIR=$(pwd)
ABS_TARGET_DIR=$(cd .. && pwd)/$(basename "$TARGET_DIR")

# Template'i kopyala
cp -r TODO_TEMPLATE "$TARGET_DIR"
echo "  ✓ Template kopyalandı"

# Root'a YZ_KURALLAR.md ve NEXT_AI_START_HERE.md kopyala
echo -e "${BLUE}📋 Root dosyaları kopyalanıyor...${NC}"
cp TODO_TEMPLATE/YZ_KURALLAR.md.template "../YZ_KURALLAR.md"
cp TODO_TEMPLATE/NEXT_AI_START_HERE.md.template "../NEXT_AI_START_HERE.md"
echo "  ✓ YZ_KURALLAR.md kopyalandı (root)"
echo "  ✓ NEXT_AI_START_HERE.md kopyalandı (root)"

# Klasör isimlerini değiştir
cd "$TARGET_DIR"
mv TODO_KISA_MM "${TODO_KISA}_MM"
mv TODO_KISA_UA "${TODO_KISA}_UA"
mv TODO_KISA_YZ "${TODO_KISA}_YZ"
mv TODO_KISA_PD "${TODO_KISA}_PD"

# On_Bilgilendirme.md root'taki yeterli, her TODO'da ayrı kopyaya gerek yok
echo "  ✓ Klasör isimleri güncellendi"

cd "$SCRIPT_DIR"

# Proje prensiplerini oku (project_config.yaml'dan - opsiyonel)
PRINCIPLES=""
if [ -f "project_config.yaml" ]; then
    while IFS= read -r line; do
        if [[ $line == *"name:"* ]] && [[ $PRINCIPLES != "" ]]; then
            PRINCIPLE_NAME=$(echo $line | awk '{print $2}' | tr -d '"')
            PRINCIPLES="${PRINCIPLES}${PRINCIPLE_NAME}, "
        fi
    done < <(sed -n '/principles:/,/^[^ ]/p' project_config.yaml 2>/dev/null)
    PRINCIPLES=${PRINCIPLES%, }  # Son virgülü kaldır
fi

# Dosya içeriklerini değiştir
echo -e "${BLUE}🔄 Placeholder'lar değiştiriliyor...${NC}"

# Tarih bilgisi
CURRENT_DATE=$(date +"%d %B %Y" | sed 's/January/Ocak/; s/February/Şubat/; s/March/Mart/; s/April/Nisan/; s/May/Mayıs/; s/June/Haziran/; s/July/Temmuz/; s/August/Ağustos/; s/September/Eylül/; s/October/Ekim/; s/November/Kasım/; s/December/Aralık/')

# Root dosyalarını değiştir
echo "  ✓ Root dosyaları güncelleniyor..."
cd ..
sed -i "s/{{PROJECT_NAME}}/${PROJECT_NAME}/g" YZ_KURALLAR.md
sed -i "s/{{TODO_FILE}}/${TODO_BELGE}/g" YZ_KURALLAR.md
sed -i "s/{{CREATION_DATE}}/${CURRENT_DATE}/g" YZ_KURALLAR.md

sed -i "s/{{PROJECT_NAME}}/${PROJECT_NAME}/g" NEXT_AI_START_HERE.md
sed -i "s/{{TODO_FILE}}/${TODO_BELGE}/g" NEXT_AI_START_HERE.md
sed -i "s/{{CREATION_DATE}}/${CURRENT_DATE}/g" NEXT_AI_START_HERE.md

cd "$SCRIPT_DIR"

# TODO klasörü içindeki dosyaları değiştir
find "$ABS_TARGET_DIR" -type f \( -name "*.md" -o -name "*.template" \) | while read -r file; do
    # Temel placeholder'lar
    sed -i "s/{PROJECT_NAME}/${PROJECT_NAME}/g" "$file"
    sed -i "s/{TODO_KISA}/${TODO_KISA}/g" "$file"
    sed -i "s/{TODO_TAM}/${TODO_TAM}/g" "$file"
    sed -i "s/{TODO_BELGE}/${TODO_BELGE}/g" "$file"
    sed -i "s/{CURRENT_DATE}/${CURRENT_DATE}/g" "$file"
    
    # Varsayılan değerler (kullanıcı sonradan düzenleyecek)
    sed -i "s/{MAIN_CODE_PATH}/compiler\/stage1-mlp\/mlp_compiler.c/g" "$file"
    sed -i "s/{MAIN_FILES}/compiler\/ tests\/ docs\//g" "$file"
    sed -i "s/{TEST_COMMAND}/\.\/build\/bin\/mlpc tests\/test.mlp \/tmp\/test.asm/g" "$file"
    sed -i "s/{PHASE1_NAME}/İlk Phase Adı/g" "$file"
    sed -i "s/{PHASE1_TIME}/1-2 saat/g" "$file"
    sed -i "s/{TASK1_NAME}/İlk Task/g" "$file"
    sed -i "s/{TASK1_TIME}/30 dakika/g" "$file"
    sed -i "s/{TASK1_DESC_1}/Task açıklaması 1/g" "$file"
    sed -i "s/{TASK1_DESC_2}/Task açıklaması 2/g" "$file"
    sed -i "s/{TASK1_DESC_3}/Task açıklaması 3/g" "$file"
    sed -i "s/{TASK2_NAME}/İkinci Task/g" "$file"
    sed -i "s/{TASK2_TIME}/45 dakika/g" "$file"
    sed -i "s/{TASK2_DESC_1}/Task açıklaması 1/g" "$file"
    sed -i "s/{TASK2_DESC_2}/Task açıklaması 2/g" "$file"
    sed -i "s/{TASK2_DESC_3}/Task açıklaması 3/g" "$file"
    sed -i "s/{TASK3_NAME}/Üçüncü Task/g" "$file"
    sed -i "s/{TASK3_TIME}/30 dakika/g" "$file"
    sed -i "s/{TASK3_DESC_1}/Task açıklaması 1/g" "$file"
    sed -i "s/{TASK3_DESC_2}/Task açıklaması 2/g" "$file"
    sed -i "s/{TASK3_DESC_3}/Task açıklaması 3/g" "$file"
    sed -i "s/{CRITERIA_1}/Başarı kriteri 1/g" "$file"
    sed -i "s/{CRITERIA_2}/Başarı kriteri 2/g" "$file"
    sed -i "s/{CRITERIA_3}/Başarı kriteri 3/g" "$file"
    📁 ROOT (../):"
echo "     ├── YZ_KURALLAR.md          ← YZ için TEK KAYNAK!"
echo "     └── NEXT_AI_START_HERE.md   ← YZ başlangıç noktası"
echo ""
echo "  📁 $TARGET_DIR/:"
echo "     ├── ${TODO_KISA}_PD/"
echo "     │   └── Danişman_buradan_basla.md"
echo "     ├── ${TODO_KISA}_MM/"
echo "     │   └── Mastermind_buradan_basla.md"
echo "     ├── ${TODO_KISA}_UA/"
echo "     │   └── Ust_Akil_buradan_basla.md"
echo "     ├── ${TODO_KISA}_YZ/"
echo "     │   ├── YZ_RAPOR_TEMPLATE.md"
echo "     │   └── completed/"
echo "     └── docs/"
echo "         └── On_Bilgilendirme.md"
echo ""
echo -e "${YELLOW}⚠️  SONRAKİ ADIMLAR:${NC}"
echo "  1. ${TODO_BELGE} dosyasını oluştur (ana görev listesi)"
echo "  2. YZ_KURALLAR.md'de 'PROJE SPESİFİK BÖLÜM'ü doldur:"
echo "     - Proje prensipleri"
echo "     - Mimari açıklaması"
echo "     - Test komutları"
echo "     - Başarı kriteri"
echo "  3. NEXT_AI_START_HERE.md'yi kontrol et (Phase 1 doğru mu?)"
echo "  4. YZ'yi başlat: 'NEXT_AI_START_HERE.md dosyasını oku ve başla.'"
echo ""
echo -e "${BLUE}💡 YZ İÇİN:${NC}"
echo "  YZ sadece 2 belge okuyacak:"
echo "  ✅ YZ_KURALLAR.md (10 dakika - HER ŞEY burada!)"
echo "  ✅ ${TODO_BELGE} (5 dakika - görevler)
echo "  └── docs/"
echo "      └── On_Bilgilendirme.md"
echo ""
echo -e "${YELLOW}⚠️  SONRAKİ ADIMLAR:${NC}"
echo "  1. ${TODO_BELGE} dosyasını oluştur (ana görev listesi)"
echo "  2. ${TARGET_DIR}/${TODO_KISA}_YZ/NEXT_AI_START_HERE.md'yi DÜZENLE:"
echo "     - {PHASE1_NAME}, {TASK1_NAME} gibi placeholder'ları doldur"
echo "     - {TEST_COMMAND} projeye göre ayarla"
echo "  3. ${TARGET_DIR}/docs/ klasörüne proje spesifik belgeler ekle"
echo "  4. YZ'yi başlat: TODO_KURALLARI.md → NEXT_AI_START_HERE.md"
echo ""
echo -e "${BLUE}💡 İPUCU:${NC}"
echo "  NEXT_AI_START_HERE.md'de '🔍 İLK ÖNCE: MEVCUT DURUMU KONTROL ET!'"
echo "  bölümü çok önemli - YZ'nin gereksiz iş yapmasını önler!"
echo ""
echo -e "${GREEN}🚀 Hazır! İyi çalışmalar!${NC}"
