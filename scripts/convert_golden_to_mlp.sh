#!/bin/bash
# =============================================================================
# convert_golden_to_mlp.sh
# PMLP golden test dosyalarını English MLP formatına çevirerek
# examples/en/golden/ altına kopyalar.
# Kullanım: bash scripts/convert_golden_to_mlp.sh
# =============================================================================

set -e

GTS=/home/pardus/PROJELER/MELP/LLVM/STAGE0/GOLDEN_TEST_SUITE
STAGE1=/home/pardus/PROJELER/MELP/LLVM/STAGE1
DEST="$STAGE1/examples/en/golden"
COMP="$STAGE1/bin/melp_compiler_bin"

pmlp_to_mlp() {
  # PMLP underscore token'larını MLP space token'larına dönüştür
  # Sadece kelime sınırlarında değiştir, string literalleri etkiletme
  sed \
    -e 's/\bend_function\b/end function/g' \
    -e 's/\belse_if\b/else if/g' \
    -e 's/\bend_if\b/end if/g' \
    -e 's/\bend_while\b/end while/g' \
    -e 's/\bend_for\b/end for/g' \
    -e 's/\bend_match\b/end match/g' \
    -e 's/\bend_enum\b/end enum/g' \
    -e 's/\bend_struct\b/end struct/g' \
    -e 's/\bend_interface\b/end interface/g' \
    -e 's/\bend_module\b/end module/g' \
    -e 's/\bend_case\b/end case/g' \
    -e 's/\bend_loop\b/end loop/g' \
    "$1"
}

echo "=== PMLP → MLP Golden Test Dönüşümü ==="
echo "Kaynak : $GTS"
echo "Hedef  : $DEST"
echo

CONVERTED=0
SKIPPED=0

for src in "$GTS"/*/*.mlp; do
  [ -f "$src" ] || continue
  cat=$(basename "$(dirname "$src")")
  fname=$(basename "$src")
  destdir="$DEST/$cat"
  destfile="$destdir/$fname"

  mkdir -p "$destdir"
  pmlp_to_mlp "$src" > "$destfile"
  CONVERTED=$((CONVERTED+1))
done

echo "Dönüştürülen: $CONVERTED dosya"
echo "Hedef dizin : $DEST"
echo
echo "Tamamlandı."
