#!/usr/bin/env bash
# =============================================================================
# sync-binary.sh — STAGE0 binary'sini VSIX bin/ klasörüne kopyala
# =============================================================================
# Kullanım:
#   ./scripts/sync-binary.sh          # kuru çalışma (ne yapacağını gösterir)
#   ./scripts/sync-binary.sh --apply  # gerçekten kopyalar
#
# Çalıştırma yeri: /home/pardus/projeler/MLP/editors/VSIX/
# =============================================================================

set -euo pipefail

VSIX_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DEP_FILE="$VSIX_DIR/.stage0-dep"

RED='\033[0;31m'
GRN='\033[0;32m'
YEL='\033[1;33m'
NC='\033[0m'

APPLY=false
[[ "${1:-}" == "--apply" ]] && APPLY=true

# ── .stage0-dep oku ───────────────────────────────────────────────────────
if [[ ! -f "$DEP_FILE" ]]; then
  echo -e "${RED}[HATA]${NC} .stage0-dep bulunamadı: $DEP_FILE"
  exit 1
fi

STAGE0_ROOT=$(python3 -c "import json; d=json.load(open('$DEP_FILE')); print(d['stage0_root'])")
STAGE0_VERSION=$(python3 -c "import json; d=json.load(open('$DEP_FILE')); print(d['stage0_version'])")
STAGE0_BINARY="$STAGE0_ROOT/build/compiler/stage1/modules/orchestrator/melp_compiler"
STAGE0_RUNTIME="$STAGE0_ROOT/build/mlp_runtime.bc"
STAGE0_MANIFEST="$STAGE0_ROOT/VERSION"
NORMALIZER_SRC="/home/pardus/PROJELER/MELP/ORTAK/normalizer/melp_normalizer_bin"

# STAGE0'ın gerçek mevcut versiyonu
STAGE0_CURRENT=$(cat "$STAGE0_MANIFEST" | tr -d '[:space:]')

echo ""
echo -e "${YEL}VSIX → STAGE0 Binary Senkronizasyonu${NC}"
echo "  .stage0-dep versiyonu : $STAGE0_VERSION"
echo "  STAGE0 mevcut         : $STAGE0_CURRENT"

# ── Binary kontrolleri ────────────────────────────────────────────────────
NEED_UPDATE=false

for ITEM in "melp_compiler_linux:$STAGE0_BINARY" "mlp_runtime.bc:$STAGE0_RUNTIME" "melp_normalizer_bin:$NORMALIZER_SRC"; do
  DEST_NAME="${ITEM%%:*}"
  SRC_PATH="${ITEM##*:}"
  DEST_PATH="$VSIX_DIR/bin/$DEST_NAME"

  if [[ ! -f "$SRC_PATH" ]]; then
    echo -e "  ${RED}✗ Kaynak bulunamadı:${NC} $SRC_PATH"
    continue
  fi

  if [[ ! -f "$DEST_PATH" ]]; then
    echo -e "  ${YEL}⚠  bin/$DEST_NAME yok — kopyalanacak${NC}"
    NEED_UPDATE=true
  else
    SRC_SHA=$(sha256sum "$SRC_PATH" | cut -d' ' -f1)
    DST_SHA=$(sha256sum "$DEST_PATH" | cut -d' ' -f1)
    if [[ "$SRC_SHA" != "$DST_SHA" ]]; then
      echo -e "  ${YEL}⚠  bin/$DEST_NAME FARKLI (hash uyuşmuyor)${NC}"
      echo -e "       STAGE0 : $SRC_SHA"
      echo -e "       VSIX   : $DST_SHA"
      NEED_UPDATE=true
    else
      echo -e "  ${GRN}✓  bin/$DEST_NAME güncel${NC}"
    fi
  fi
done

# ── Güncelleme gerekiyorsa ────────────────────────────────────────────────
if ! $NEED_UPDATE; then
  echo ""
  echo -e "${GRN}Tüm binary'ler STAGE0 v${STAGE0_CURRENT} ile senkron. ✓${NC}"
  echo ""
  exit 0
fi

if ! $APPLY; then
  echo ""
  echo -e "${YEL}Güncellemek için:${NC} ./scripts/sync-binary.sh --apply"
  echo ""
  exit 1
fi

# ── Uygula ────────────────────────────────────────────────────────────────
echo ""
echo -e "  Binary'ler kopyalanıyor..."
cp "$STAGE0_BINARY" "$VSIX_DIR/bin/melp_compiler_linux"
chmod +x "$VSIX_DIR/bin/melp_compiler_linux"
echo -e "  ${GRN}✓ bin/melp_compiler_linux güncellendi${NC}"

if [[ -f "$STAGE0_RUNTIME" ]]; then
  cp "$STAGE0_RUNTIME" "$VSIX_DIR/bin/mlp_runtime.bc"
  echo -e "  ${GRN}✓ bin/mlp_runtime.bc güncellendi${NC}"
fi

if [[ -f "$NORMALIZER_SRC" ]]; then
  cp "$NORMALIZER_SRC" "$VSIX_DIR/bin/melp_normalizer_bin"
  chmod +x "$VSIX_DIR/bin/melp_normalizer_bin"
  echo -e "  ${GRN}✓ bin/melp_normalizer_bin güncellendi${NC}"
fi

# .stage0-dep dosyasını da güncelle
TODAY=$(date +%Y-%m-%d)
python3 - <<PYEOF
import json
with open('$DEP_FILE', 'r') as f:
    data = json.load(f)
data['stage0_version'] = '$STAGE0_CURRENT'
data['last_synced'] = '$TODAY'
with open('$DEP_FILE', 'w') as f:
    json.dump(data, f, indent=2, ensure_ascii=False)
    f.write('\n')
PYEOF
echo -e "  ${GRN}✓ .stage0-dep güncellendi → v${STAGE0_CURRENT}${NC}"
echo ""
echo -e "${GRN}Senkronizasyon tamamlandı. Şimdi VSIX paketini yeniden oluştur:${NC}"
echo -e "  npm run package-vsix"
echo ""
