#!/usr/bin/env bash
# build.sh — MELP LSP Server derleme scripti
#
# Pattern:
#   1. Tüm .mlp dosyaları cat ile birleştiriliyor (import desteklemiyor)
#   2. Mevcut normalizer binary ile MLP→pmlp dönüşümü yapılıyor (bootstrap)
#   3. Stage1 derleyici ile LLVM IR üretiliyor (is_null, split vb. Stage1 özellik)
#   4. clang ile native binary üretiliyor
#
# Kullanım:  bash src/lsp/build.sh
# Çıktı:     src/lsp/melp_lsp_bin

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
STAGE0="$SCRIPT_DIR/../../bin/melp_compiler"  # Stage1 derleyici (Stage1 projesi)
CLANG="${CLANG:-clang-19}"
OUT_BIN="$SCRIPT_DIR/melp_lsp_bin"
FLAT_SRC="/tmp/lsp_flat.mlp"
FLAT_PMLP="/tmp/lsp_flat_pmlp.mlp"
FLAT_IR="/tmp/lsp_flat.ll"

# Normalizer binary (bootstrap için)
NORMALIZER_BIN="$SCRIPT_DIR/../normalizer/melp_normalizer_bin"

echo "🔧 MELP LSP Server build başlıyor..."
echo "   Kaynak dizin: $SCRIPT_DIR"

# ── Adım 1: Flat kaynak oluştur ────────────────────────────────────────────
# Bağımlılık sırası (forward reference yok):
#   1. lsp_json.mlp      — JSON yardımcıları (temel)
#   2. lsp_keywords.mlp  — keyword tabloları (lsp_json kullanır)
#   3. lsp_protocol.mlp  — JSON-RPC framing (lsp_json kullanır)
#   4. lsp_handlers.mlp  — LSP handler'ları (hepsini kullanır)
#   5. lsp.mlp           — main() + event loop (hepsini kullanır)

echo "📦 Flat kaynak oluşturuluyor: $FLAT_SRC"

# import satırlarını ve yorum başlık bloklarını filtrele, dosyaları sıraya göre ekle
{
    grep -v "^-- import\|^import" "$SCRIPT_DIR/lsp_json.mlp"
    echo ""
    grep -v "^-- import\|^import" "$SCRIPT_DIR/lsp_keywords.mlp"
    echo ""
    grep -v "^-- import\|^import" "$SCRIPT_DIR/lsp_protocol.mlp"
    echo ""
    grep -v "^-- import\|^import" "$SCRIPT_DIR/lsp_handlers.mlp"
    echo ""
    grep -v "^-- import\|^import" "$SCRIPT_DIR/lsp.mlp"
} > "$FLAT_SRC"

echo "   $(wc -l < "$FLAT_SRC") satır, $(wc -c < "$FLAT_SRC") byte"

# ── Adım 1b: Bootstrap — MLP→pmlp çevir ───────────────────────────────────
if [[ -x "$NORMALIZER_BIN" ]]; then
    echo "🔄 Bootstrap: MLP→pmlp dönüştürülüyor..."
    cat "$FLAT_SRC" > /tmp/.melp_norm_src
    printf 'english' > /tmp/.melp_norm_lang
    printf '%s' "$SCRIPT_DIR/../normalizer/configs/diller.json" > /tmp/.melp_norm_diller
    "$NORMALIZER_BIN" > "$FLAT_PMLP"
    echo "   $(wc -l < "$FLAT_PMLP") satır pmlp üretildi"
else
    echo "⚠️  Normalizer binary bulunamadı. Önce normalizer'ı derleyin:"
    echo "    bash /home/pardus/PROJELER/MELP/ORTAK/normalizer/build.sh"
    echo "   Flat kaynak doğrudan kullanılıyor (Stage0 pmlp syntax gerekebilir)"
    cp "$FLAT_SRC" "$FLAT_PMLP"
fi

# ── Adım 2: Stage 0 ile LLVM IR ───────────────────────────────────────────
echo "🔨 Stage 0 derleniyor: $FLAT_IR"
"$STAGE0" "$FLAT_PMLP" -o "$FLAT_IR" 2>&1

# ── Adım 3: clang ile native binary ───────────────────────────────────────
echo "🔗 clang ile linkleniyor: $OUT_BIN"
$CLANG -O0 "$FLAT_IR" "$SCRIPT_DIR/lsp_io.c" -lm -o "$OUT_BIN" 2>&1

chmod +x "$OUT_BIN"

echo ""
echo "✅ LSP Server derleme başarılı!"
echo "   Binary: $OUT_BIN"
echo ""
echo "Editör entegrasyonu için:"
echo "   VS Code settings.json:"
echo "   \"melp.lsp.serverPath\": \"$OUT_BIN\""
