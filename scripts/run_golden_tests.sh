#!/usr/bin/env bash
# run_golden_tests.sh — Stage1 Golden Test Runner
# Pipeline: .mlp → melp_normalizer_bin → .pmlp → melp_compiler_bin → lli-19
# Çıktıyı "-- Expected:" etiketi ile karşılaştırır.
#
# Kullanım:
#   ./run_golden_tests.sh                    # tüm suite'ler
#   ./run_golden_tests.sh control_flow       # tek suite
#   ./run_golden_tests.sh control_flow enum  # birden fazla suite

set -uo pipefail

STAGE1_ROOT="$(cd "$(dirname "$0")" && pwd)"
GOLDEN_DIR="$STAGE1_ROOT/examples/en/golden"
NORMALIZER="/home/pardus/PROJELER/MELP/ORTAK/normalizer/melp_normalizer_bin"
COMPILER="$STAGE1_ROOT/bin/melp_compiler_bin"
LLI="${LLI:-lli-19}"

RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[0;33m'; RESET='\033[0m'

# --- Ön kontrol ---
for bin in "$NORMALIZER" "$COMPILER"; do
    if [[ ! -x "$bin" ]]; then
        echo -e "${RED}HATA: Binary bulunamadı: $bin${RESET}"
        exit 1
    fi
done
if ! command -v "$LLI" &>/dev/null; then
    echo -e "${RED}HATA: $LLI bulunamadı.${RESET}"
    exit 1
fi

# --- Suite listesi ---
if [[ $# -gt 0 ]]; then
    SUITES=("$@")
else
    mapfile -t SUITES < <(find "$GOLDEN_DIR" -mindepth 1 -maxdepth 1 -type d | sort | xargs -n1 basename)
fi

total_pass=0
total_fail=0
total_skip=0

for suite in "${SUITES[@]}"; do
    suite_dir="$GOLDEN_DIR/$suite"
    if [[ ! -d "$suite_dir" ]]; then
        echo -e "${YELLOW}⊘ SKIP: $suite (dizin yok)${RESET}"
        continue
    fi

    s_pass=0; s_fail=0; s_skip=0
    fail_details=""

    for f in "$suite_dir"/*.mlp; do
        [[ -f "$f" ]] || continue

        # Expected etiketi oku
        exp_raw=$(grep -i "^-- expected:" "$f" | head -1 | sed 's/^-- [Ee][Xx][Pp][Ee][Cc][Tt][Ee][Dd]: //')
        if [[ -z "$exp_raw" ]]; then
            ((s_skip++)); continue
        fi
        exp=$(printf '%b' "$exp_raw")

        fname="${f##*/}"
        fdir="$(dirname "$f")"
        tmp_pmlp="$fdir/_golden_norm_$$.pmlp"
        tmp_ll="/tmp/golden_comp_$$.ll"

        # 1. Normalize: .mlp → .pmlp
        cp "$f" /tmp/.melp_norm_src
        printf 'english' > /tmp/.melp_norm_lang
        norm_err=$("$NORMALIZER" > "$tmp_pmlp" 2>&1)
        if [[ $? -ne 0 ]]; then
            ((s_fail++))
            fail_details+="  FAIL $fname (normalizer hata: $norm_err)\n"
            rm -f "$tmp_pmlp" "$tmp_ll"
            continue
        fi

        # 2. Derle: .pmlp → LLVM IR
        comp_err=$("$COMPILER" "$tmp_pmlp" -o "$tmp_ll" 2>&1)
        if [[ $? -ne 0 ]]; then
            # COMPILE_ERROR: beklentisi var mı?
            if [[ "$exp_raw" == COMPILE_ERROR:* ]]; then
                needle="${exp_raw#COMPILE_ERROR: }"
                if [[ "$comp_err" == *"$needle"* ]]; then
                    ((s_pass++))
                else
                    ((s_fail++))
                    fail_details+="  FAIL $fname (COMPILE_ERROR beklendi: '$needle', alınan: '$comp_err')\n"
                fi
            else
                ((s_fail++))
                fail_details+="  FAIL $fname (derleme hatası: $comp_err)\n"
            fi
            rm -f "$tmp_pmlp" "$tmp_ll"
            continue
        fi

        # 3. Çalıştır
        got=$("$LLI" "$tmp_ll" 2>/dev/null | grep -v '^\[EnumCodegen\]')
        rm -f "$tmp_pmlp" "$tmp_ll"

        if [[ "$got" == "$exp" ]]; then
            ((s_pass++))
        else
            ((s_fail++))
            fail_details+="  FAIL $fname\n    Beklenen : '$exp'\n    Alınan   : '$got'\n"
        fi
    done

    total_pass=$((total_pass + s_pass))
    total_fail=$((total_fail + s_fail))
    total_skip=$((total_skip + s_skip))

    suite_total=$((s_pass + s_fail))
    if [[ $s_fail -eq 0 && $suite_total -gt 0 ]]; then
        echo -e "${GREEN}✓ $suite: $s_pass/$suite_total${RESET}"
    elif [[ $suite_total -eq 0 ]]; then
        echo -e "${YELLOW}⊘ $suite: 0 test (Expected etiketi yok)${RESET}"
    else
        echo -e "${RED}✗ $suite: $s_pass/$suite_total${RESET}"
        printf "$fail_details"
    fi
done

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
grand_total=$((total_pass + total_fail))
if [[ $grand_total -gt 0 ]]; then
    rate=$(awk "BEGIN {printf \"%.1f\", ($total_pass/$grand_total)*100}")
    echo "TOPLAM: $total_pass/$grand_total ($rate%)"
else
    echo "TOPLAM: test bulunamadı"
fi
[[ $total_skip -gt 0 ]] && echo "Atlanan (Expected yok): $total_skip"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
