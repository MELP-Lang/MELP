#!/usr/bin/env bash
# melp_normalizer.sh — MELP Normalizer kullanıcı arayüzü
#
# Stage 0 bug workarounds:
#   B2: read_arg() crash → CLI args temp dosyaya yazılır
#   B3: read_file(variable) type error → kaynak içeriği temp dosyaya yazılır
#
# Temp file protokolü:
#   /tmp/.melp_norm_src    <- kaynak dosya içeriği
#   /tmp/.melp_norm_lang   <- "turkish" veya "russian" veya "english"
#   /tmp/.melp_norm_result <- binary'nin yazdığı normalize sonucu
#
# Kullanım:
#   melp_normalizer.sh <girdi.tr.mlp> [çıktı.mlp]
#   melp_normalizer.sh <girdi.ru.mlp> [çıktı.mlp]

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BINARY="$SCRIPT_DIR/melp_normalizer_bin"

if [[ ! -x "$BINARY" ]]; then
    echo "Hata: normalizer binary bulunamadı."
    echo "Önce build edin:  bash $SCRIPT_DIR/build.sh"
    exit 1
fi

INPUT="$1"
if [[ -z "$INPUT" ]]; then
    echo "Kullanım: melp_normalizer.sh <girdi.tr.mlp> [çıktı.mlp]"
    echo "           melp_normalizer.sh <girdi.ru.mlp> [çıktı.mlp]"
    echo ""
    echo "Dil, dosya uzantısından otomatik algılanır:"
    echo "  .tr.mlp → Türkçe anahtar kelimeler"
    echo "  .ru.mlp → Rusça anahtar kelimeler"
    exit 1
fi

if [[ ! -f "$INPUT" ]]; then
    echo "Hata: dosya bulunamadı: $INPUT"
    exit 1
fi

OUTPUT="$2"
if [[ -z "$OUTPUT" ]]; then
    case "$INPUT" in
        *.tr.mlp) OUTPUT="${INPUT%.tr.mlp}.mlp" ;;
        *.ru.mlp) OUTPUT="${INPUT%.ru.mlp}.mlp" ;;
        *)        OUTPUT="${INPUT}.out.mlp" ;;
    esac
fi

# İngilizce .mlp → normalizer gerekmez
if [[ "$INPUT" == *.mlp && "$INPUT" != *.tr.mlp && "$INPUT" != *.ru.mlp ]]; then
    echo "⏩ İngilizce .mlp — normalizer atlanıyor."
    [[ "$INPUT" != "$OUTPUT" ]] && cp "$INPUT" "$OUTPUT"
    exit 0
fi

# Dil algıla
LANG_TAG="english"
case "$INPUT" in
    *.tr.mlp) LANG_TAG="turkish" ;;
    *.ru.mlp) LANG_TAG="russian" ;;
esac

# Stage 0 bug B3 workaround: kaynak içeriğini temp dosyaya yaz
cat "$INPUT"                     > /tmp/.melp_norm_src
printf '%s' "$LANG_TAG"          > /tmp/.melp_norm_lang

# Bug B4 workaround: write_file(literal;var) codegen bug → binary stdout'a yazar
# Shell stdout'u yakalar ve OUTPUT dosyasına kopyalar
"$BINARY" > "$OUTPUT"

# Hata kontrolü: binary "__ERROR__:" ile başlayan satır bastırdıysa
FIRST_LINE=$(head -1 "$OUTPUT")
if [[ "$FIRST_LINE" == __ERROR__* ]]; then
    echo "❌ Normalizer hatası: $FIRST_LINE"
    rm -f "$OUTPUT"
    exit 1
fi

echo "✅ Çıktı: $OUTPUT ($(wc -c < "$OUTPUT") byte)"
