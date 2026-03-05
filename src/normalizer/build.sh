#!/usr/bin/env bash
# build.sh — MELP Normalizer derleme scripti
#
# Stage 0 sınırlamaları nedeniyle iki workaround:
#   1. Bug I1: 'import' desteklenmiyor → dosyalar cat ile birleştiriliyor
#   2. Bug B2: read_arg() crash yapıyor → main() /tmp/.melp_norm_* kullanıyor
#
# Kullanım:  bash build.sh
# Çıktı:     src/normalizer/melp_normalizer_bin  (native ELF binary)

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
STAGE0="/home/pardus/projeler/MLP/LLVM/stage0/v2/build/compiler/stage1/modules/orchestrator/melp_compiler"
CLANG="${CLANG:-clang}"
OUT_BIN="$SCRIPT_DIR/melp_normalizer_bin"
FLAT_SRC="/tmp/normalizer_flat.mlp"
FLAT_IR="/tmp/normalizer_flat.ll"

echo "🔧 MELP Normalizer build başlıyor..."
echo "   Kaynak dizin: $SCRIPT_DIR"

# ── Adım 1: Flat kaynak oluştur ────────────────────────────────────────────
# normalizer.mlp içindeki import satırlarını sıyır,
# sonuna lang dosyalarını ekle (forward-ref sorunsuz çalışır)
echo "📦 Flat kaynak oluşturuluyor: $FLAT_SRC"

# Doğru bağımlılık sırası (Stage 0 forward reference desteklemiyor):
#   Part 1: is_word_char, replace_keyword  (lang dosyalarının bağımlılığı)
#   Part 2: lang_turkish, lang_russian
#   Part 3: translate_chunk, normalize_source, main
#
# normalizer.mlp içindeki "-- ###LANG_INJECT###" marker bölüm sınırıdır.

MARKER="-- ###LANG_INJECT###"

# Part 1: marker'a kadar olan satırlar (import satırları hariç)
grep -v "^import" "$SCRIPT_DIR/normalizer.mlp" | \
    awk "/^$MARKER/{exit} {print}" > "$FLAT_SRC"
echo "" >> "$FLAT_SRC"

# Part 2: lang dosyaları
cat "$SCRIPT_DIR/lang_turkish.mlp" >> "$FLAT_SRC"
echo "" >> "$FLAT_SRC"
cat "$SCRIPT_DIR/lang_russian.mlp" >> "$FLAT_SRC"
echo "" >> "$FLAT_SRC"

# Part 3: marker sonrası satırlar (import satırları hariç)
grep -v "^import" "$SCRIPT_DIR/normalizer.mlp" | \
    awk "found{print} /^$MARKER/{found=1}" >> "$FLAT_SRC"

echo "   $(wc -l < "$FLAT_SRC") satır, $(wc -c < "$FLAT_SRC") byte"

# ── Adım 2: Stage 0 ile LLVM IR'a derle ───────────────────────────────────
echo "🔨 Stage 0 derleniyor: $FLAT_IR"
"$STAGE0" "$FLAT_SRC" -o "$FLAT_IR" 2>&1

# ── Adım 3: clang ile native binary ───────────────────────────────────────
RUNTIME="/home/pardus/projeler/MLP/LLVM/stage0/v2/build/mlp_runtime.bc"
echo "🔗 clang ile linkleniyor: $OUT_BIN"
$CLANG -O0 -x ir "$FLAT_IR" "$RUNTIME" -lm -o "$OUT_BIN" 2>&1

chmod +x "$OUT_BIN"

echo ""
echo "✅ Derleme başarılı!"
echo "   Binary: $OUT_BIN"
echo "   Kullanmak için: melp_normalizer.sh <dosya.tr.mlp>"
