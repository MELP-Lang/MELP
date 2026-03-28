#!/bin/bash
# =============================================================================
# test_golden_mlp.sh
# examples/en/golden/ altındaki MLP dosyalarını Stage1 tam pipeline ile test et.
# Kullanım: bash scripts/test_golden_mlp.sh [--verbose]
# =============================================================================

STAGE1=/home/pardus/PROJELER/MELP/LLVM/STAGE1
COMP=/home/pardus/PROJELER/MELP/LLVM/STAGE0/build/compiler/stage1/modules/orchestrator/melp_compiler
GOLDEN="$STAGE1/examples/en/golden"
VERBOSE="${1:-}"

PASS=0; FAIL=0; TOTAL=0
declare -A CAT_PASS
declare -A CAT_FAIL
declare -a FAIL_LIST

for f in "$GOLDEN"/*/*.mlp; do
  [ -f "$f" ] || continue
  cat=$(basename "$(dirname "$f")")
  fname=$(basename "$f")
  TOTAL=$((TOTAL+1))

  if "$COMP" "$f" --normalize --lang=english --syntax=mlp -o /tmp/.melp_golden.ll 2>/tmp/comp_err.txt; then
    PASS=$((PASS+1))
    CAT_PASS[$cat]=$((${CAT_PASS[$cat]:-0}+1))
  else
    FAIL=$((FAIL+1))
    CAT_FAIL[$cat]=$((${CAT_FAIL[$cat]:-0}+1))
    FAIL_LIST+=("$cat/$fname")
    if [ "$VERBOSE" = "--verbose" ]; then
      echo "FAIL: $cat/$fname"
      cat /tmp/comp_err.txt 2>/dev/null | head -2
    fi
  fi
done

echo ""
echo "============================================================"
echo " Stage1 Pipeline Test: examples/en/golden/"
echo "============================================================"
printf " PASS  : %d\n" "$PASS"
printf " FAIL  : %d\n" "$FAIL"
printf " TOTAL : %d\n" "$TOTAL"
printf " ORAN  : %d%%\n" "$((PASS * 100 / TOTAL))"
echo "============================================================"

echo ""
echo "=== TAMAMEN GEÇEN KATEGORİLER ==="
for cat in $(echo "${!CAT_PASS[@]}" | tr ' ' '\n' | sort); do
  if [ "${CAT_FAIL[$cat]:-0}" -eq 0 ]; then
    echo "  ✅ $cat (${CAT_PASS[$cat]} test)"
  fi
done

echo ""
echo "=== KISMÎ BAŞARILI KATEGORİLER ==="
for cat in $(echo "${!CAT_PASS[@]}" | tr ' ' '\n' | sort); do
  if [ "${CAT_FAIL[$cat]:-0}" -gt 0 ]; then
    p=${CAT_PASS[$cat]:-0}; f=${CAT_FAIL[$cat]:-0}
    echo "  ⚠️  $cat ($p geçti, $f kaldı)"
  fi
done

echo ""
echo "=== TAMAMEN BAŞARISIZ KATEGORİLER ==="
for cat in $(echo "${!CAT_FAIL[@]}" | tr ' ' '\n' | sort); do
  if [ "${CAT_PASS[$cat]:-0}" -eq 0 ]; then
    echo "  ❌ $cat (${CAT_FAIL[$cat]} test başarısız)"
  fi
done
