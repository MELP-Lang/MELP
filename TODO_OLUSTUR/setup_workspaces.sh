#!/bin/bash
# setup_workspaces.sh - VS Code Multi-Workspace Oluşturma Scripti
# Farklı YZ'lerin aynı proje üzerinde çalışabilmesi için
# Kullanım: ./setup_workspaces.sh [proje_adı]

set -e

# Renkler
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m'

# Banner
echo -e "${PURPLE}╔════════════════════════════════════════════════╗${NC}"
echo -e "${PURPLE}║   🖥️  VS CODE MULTI-WORKSPACE OLUŞTURUCU       ║${NC}"
echo -e "${PURPLE}║   Farklı YZ'ler için ayrı pencereler           ║${NC}"
echo -e "${PURPLE}╚════════════════════════════════════════════════╝${NC}"
echo ""

# Proje adını al
if [ -n "$1" ]; then
    PROJECT_NAME="$1"
elif [ -f "project_config.yaml" ]; then
    PROJECT_NAME=$(grep "name:" project_config.yaml | head -1 | awk '{print $2}' | tr -d '"')
else
    PROJECT_NAME=$(basename "$(pwd)")
fi

PROJECT_NAME_UPPER=$(echo "$PROJECT_NAME" | tr '[:lower:]' '[:upper:]')
PROJECT_NAME_LOWER=$(echo "$PROJECT_NAME" | tr '[:upper:]' '[:lower:]')

echo -e "${GREEN}📋 Proje: ${CYAN}$PROJECT_NAME${NC}"
echo ""

# Workspace tanımları
# Format: "dosya_adı|başlık|renk|emoji"
WORKSPACES=(
    "${PROJECT_NAME_LOWER}-mastermind.code-workspace|🧠 ${PROJECT_NAME_UPPER} - MASTERMIND|#4B0082"
    "${PROJECT_NAME_LOWER}-ua.code-workspace|🎯 ${PROJECT_NAME_UPPER} - ÜST AKIL|#006400"
    "${PROJECT_NAME_LOWER}-yz.code-workspace|⚙️ ${PROJECT_NAME_UPPER} - GÖREVLİ YZ|#8B4513"
    "${PROJECT_NAME_LOWER}-danisman.code-workspace|📚 ${PROJECT_NAME_UPPER} - DANIŞMAN|#1E90FF"
)

echo -e "${BLUE}📁 Oluşturulacak workspace'ler:${NC}"
echo ""

# Her workspace için dosya oluştur
for ws in "${WORKSPACES[@]}"; do
    IFS='|' read -r filename title color <<< "$ws"
    
    # Dosya zaten varsa atla
    if [ -f "$filename" ]; then
        echo -e "  ${YELLOW}⚠️  $filename zaten var, atlanıyor${NC}"
        continue
    fi
    
    # Workspace dosyası oluştur
    cat > "$filename" << EOF
{
    "folders": [{ "path": "." }],
    "settings": {
        "window.title": "$title",
        "workbench.colorCustomizations": {
            "titleBar.activeBackground": "$color",
            "titleBar.activeForeground": "#FFFFFF"
        }
    }
}
EOF
    
    echo -e "  ${GREEN}✅ $filename${NC} - $title"
done

echo ""
echo -e "${GREEN}════════════════════════════════════════════════${NC}"
echo ""
echo -e "${CYAN}🚀 Kullanım:${NC}"
echo ""
echo -e "  Her YZ için ayrı VS Code penceresi açın:"
echo ""
echo -e "  ${YELLOW}code ${PROJECT_NAME_LOWER}-mastermind.code-workspace${NC}  → Mastermind için"
echo -e "  ${YELLOW}code ${PROJECT_NAME_LOWER}-ua.code-workspace${NC}          → Üst Akıl için"
echo -e "  ${YELLOW}code ${PROJECT_NAME_LOWER}-yz.code-workspace${NC}          → Görevli YZ için"
echo -e "  ${YELLOW}code ${PROJECT_NAME_LOWER}-danisman.code-workspace${NC}    → Danışman için"
echo ""
echo -e "${CYAN}📝 Renk Kodları:${NC}"
echo -e "  🧠 Mastermind : ${PURPLE}Mor (#4B0082)${NC}"
echo -e "  🎯 Üst Akıl   : ${GREEN}Yeşil (#006400)${NC}"
echo -e "  ⚙️  Görevli YZ : ${YELLOW}Kahverengi (#8B4513)${NC}"
echo -e "  📚 Danışman   : ${BLUE}Mavi (#1E90FF)${NC}"
echo ""
echo -e "${GREEN}✨ Tamamlandı!${NC}"
