#!/usr/bin/env bash
# sync-binary.sh — STAGE0 compiler binary'sini MELP IDE bin/ klasörüne kopyalar
# Kullanım: bash scripts/sync-binary.sh [STAGE0_BUILD_DIR]
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
MELP_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
PROJ_ROOT="$(cd "$MELP_ROOT/../.." && pwd)"

STAGE0_BINARY="${1:-$PROJ_ROOT/LLVM/STAGE0/build/compiler/stage1/modules/orchestrator/melp_compiler}"

if [[ ! -f "$STAGE0_BINARY" ]]; then
  echo "HATA: STAGE0 binary bulunamadı: $STAGE0_BINARY" >&2
  echo "  Önce STAGE0 build adımını çalıştırın." >&2
  exit 1
fi

BIN_DIR="$MELP_ROOT/bin"
mkdir -p "$BIN_DIR"
cp "$STAGE0_BINARY" "$BIN_DIR/melp_compiler"
chmod +x "$BIN_DIR/melp_compiler"

echo "✅ Binary kopyalandı: $BIN_DIR/melp_compiler"
