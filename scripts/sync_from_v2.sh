#!/usr/bin/env bash
# sync_from_v2.sh — stage0/v2 build çıktılarını stage1/bin/ dizinine kopyalar
#
# Kullanım:
#   bash scripts/sync_from_v2.sh              # sadece kopyala
#   bash scripts/sync_from_v2.sh --verify     # kopyala + smoke test yap
#
# stage1/ dizininden çalıştır:
#   cd /home/pardus/projeler/MLP/LLVM/stage1
#   bash scripts/sync_from_v2.sh

set -e

STAGE0_V2="/home/pardus/projeler/MLP/LLVM/stage0/v2"
STAGE1_DIR="$(cd "$(dirname "$0")/.." && pwd)"
V2_COMPILER="$STAGE0_V2/build/compiler/stage1/modules/orchestrator/melp_compiler"
V2_RUNTIME="$STAGE0_V2/build/mlp_runtime.bc"
BIN_DIR="$STAGE1_DIR/bin"

# ── Renk kodları ──────────────────────────────────────────────
RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'; NC='\033[0m'

info()    { echo -e "${GREEN}[SYNC]${NC} $*"; }
warn()    { echo -e "${YELLOW}[WARN]${NC} $*"; }
error()   { echo -e "${RED}[ERROR]${NC} $*"; exit 1; }

# ── 1. Kaynak kontrol ─────────────────────────────────────────
info "Kaynak kontrol ediliyor..."
[ -f "$V2_COMPILER" ] || error "melp_compiler bulunamadı: $V2_COMPILER"
[ -f "$V2_RUNTIME"  ] || error "mlp_runtime.bc bulunamadı: $V2_RUNTIME"

# ── 2. Versiyon bilgisi ───────────────────────────────────────
V2_DATE=$(date -r "$V2_COMPILER" "+%Y-%m-%d %H:%M:%S")
info "Kaynak: $V2_COMPILER ($V2_DATE)"

# ── 3. Eskiler yedekle ────────────────────────────────────────
if [ -f "$BIN_DIR/melp_compiler" ]; then
    OLD_DATE=$(date -r "$BIN_DIR/melp_compiler" "+%Y%m%d_%H%M%S")
    cp "$BIN_DIR/melp_compiler" "$BIN_DIR/melp_compiler.bak.$OLD_DATE"
    info "Eski binary yedeklendi: melp_compiler.bak.$OLD_DATE"
fi

# ── 4. Kopyala ────────────────────────────────────────────────
mkdir -p "$BIN_DIR"
cp "$V2_COMPILER" "$BIN_DIR/melp_compiler"
cp "$V2_RUNTIME"  "$BIN_DIR/mlp_runtime.bc"
chmod +x "$BIN_DIR/melp_compiler"

info "Kopyalandı:"
ls -lh "$BIN_DIR/melp_compiler" "$BIN_DIR/mlp_runtime.bc"

# ── 5. Eski yedekleri temizle (7 günden eski) ─────────────────
find "$BIN_DIR" -name "melp_compiler.bak.*" -mtime +7 -delete 2>/dev/null || true

# ── 6. İsteğe bağlı smoke test ────────────────────────────────
if [ "${1:-}" = "--verify" ]; then
    info "Smoke test yapılıyor..."
    
    # clang-14 var mı?
    CLANG="clang-14"
    command -v "$CLANG" &>/dev/null || {
        warn "clang-14 bulunamadı, clang deneniyor..."
        CLANG="clang"
    }
    command -v "$CLANG" &>/dev/null || error "clang bulunamadı — LLVM 14 kurun: sudo apt install clang-14"

    # Test programı yaz
    TMPDIR=$(mktemp -d)
    cat > "$TMPDIR/hello.mlp" << 'EOF'
function main() as void
    print("sync_verify: OK")
end_function
EOF

    "$BIN_DIR/melp_compiler" "$TMPDIR/hello.mlp" -o "$TMPDIR/hello.ll"
    # mlp_runtime.bc bazı fonksiyonları zaten IR'a gömer; çift tanım olunca -Wl,--allow-shlib-undefined yerine
    # önce runtime'sız dene, başarısız olursa runtime ile dene
    if ! "$CLANG" "$TMPDIR/hello.ll" -lm -o "$TMPDIR/hello" 2>/dev/null; then
        "$CLANG" "$TMPDIR/hello.ll" "$BIN_DIR/mlp_runtime.bc" -lm \
            -Wl,--allow-multiple-definition -o "$TMPDIR/hello"
    fi

    OUTPUT=$("$TMPDIR/hello")
    rm -rf "$TMPDIR"
    
    if [ "$OUTPUT" = "sync_verify: OK" ]; then
        echo -e "${GREEN}✓ Smoke test geçti: '$OUTPUT'${NC}"
    else
        error "Smoke test başarısız — çıktı: '$OUTPUT'"
    fi
fi

info "Senkronizasyon tamamlandı."
echo ""
echo "Sonraki adım: git commit ve push"
echo "  cd $STAGE1_DIR"
echo "  git add bin/ && git commit -m 'chore: derleyici v$(date +%Y%m%d) güncellendi'"
