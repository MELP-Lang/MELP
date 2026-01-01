#!/bin/bash
# create-current-todo.sh - UA'nın YZ'ye TODO atarken kullanacağı script
# Bu script CURRENT_TODO.txt oluşturur (pre-commit-check.sh için gerekli)
# 
# Güncel TODO: 3-TODO_LANGUAGE_FEATURES.md
# Proje: MLP-GCC Language Features (Generics, Pattern Matching, etc.)

set -e

# Kullanım kontrolü
if [ $# -lt 1 ]; then
    echo "Kullanım: $0 <dosya1> [dosya2] [dosya3] ..."
    echo ""
    echo "Örnek (3-TODO için):"
    echo "  $0 generics_parser.c generics_codegen.c"
    echo "  $0 pattern_matching_parser.c lambda_codegen.c"
    echo ""
    echo "Bu script CURRENT_TODO.txt oluşturur ve pre-commit-check.sh"
    echo "bu dosyadaki izinleri kontrol eder."
    exit 1
fi

PROJECT_ROOT="/home/pardus/projeler/MLP/MLP-GCC"
TODO_FILE="${PROJECT_ROOT}/CURRENT_TODO.txt"

# Mevcut TODO varsa sil
if [ -f "$TODO_FILE" ]; then
    echo "⚠️  Mevcut CURRENT_TODO.txt siliniyor..."
    rm -f "$TODO_FILE"
fi

# Yeni TODO oluştur
echo "📝 CURRENT_TODO.txt oluşturuluyor..."
echo ""

for file in "$@"; do
    # Sadece dosya adını al (path'siz)
    filename=$(basename "$file")
    echo "$filename" >> "$TODO_FILE"
    echo "  ✅ $filename eklendi"
done

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "✅ CURRENT_TODO.txt oluşturuldu!"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""
echo "İçerik:"
cat "$TODO_FILE" | sed 's/^/  - /'
echo ""
echo "💡 YZ artık SADECE bu dosyalara dokunabilir!"
echo "💡 pre-commit-check.sh otomatik kontrol edecek."
echo ""
