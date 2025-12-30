#!/bin/bash
# Test Script: TODO #2 - Import System (Stage1 Readiness Check)
# Stage1 henüz import system'i desteklemiyor (sadece type infrastructure)
# Bu script import_handler.mlp'nin varlığını ve yapısını test eder

echo "=========================================="
echo "TODO #2: IMPORT SYSTEM READINESS CHECK"
echo "=========================================="
echo ""
echo "⚠️  NOT: Stage1 sadece type infrastructure içerir"
echo "    Import system Stage2'de aktif olacak"
echo ""

# Test 1: import_handler.mlp Var mı?
echo "🧪 Test 1: import_handler.mlp Existence"
echo "--------------------------------------"
if [ -f "./MELP/MLP/stage1/modules/import_handler.mlp" ]; then
    echo "✅ PASS: import_handler.mlp found"
    wc -l ./MELP/MLP/stage1/modules/import_handler.mlp
else
    echo "❌ FAIL: import_handler.mlp not found"
    exit 1
fi
echo ""

# Test 2: ImportContext Struct Tanımlı mı?
echo "🧪 Test 2: ImportContext Struct Definition"
echo "--------------------------------------"
if grep -q "struct ImportContext" ./MELP/MLP/stage1/modules/import_handler.mlp; then
    echo "✅ PASS: ImportContext struct defined"
    grep -A 10 "struct ImportContext" ./MELP/MLP/stage1/modules/import_handler.mlp | head -12
else
    echo "❌ FAIL: ImportContext struct not found"
    exit 1
fi
echo ""

# Test 3: Circular Import Detection Function
echo "🧪 Test 3: Circular Import Detection Function"
echo "--------------------------------------"
if grep -q "function is_file_imported" ./MELP/MLP/stage1/modules/import_handler.mlp; then
    echo "✅ PASS: is_file_imported() function defined"
else
    echo "❌ FAIL: is_file_imported() function not found"
    exit 1
fi
echo ""

# Test 4: MLP_SOURCE_DIR Resolution
echo "🧪 Test 4: MLP_SOURCE_DIR Resolution Function"
echo "--------------------------------------"
if grep -q "MLP_SOURCE_DIR" ./MELP/MLP/stage1/modules/import_handler.mlp; then
    echo "✅ PASS: MLP_SOURCE_DIR resolution code found"
else
    echo "❌ FAIL: MLP_SOURCE_DIR resolution not found"
    exit 1
fi
echo ""

# Test 5: handle_import Function
echo "🧪 Test 5: handle_import() Function"
echo "--------------------------------------"
if grep -q "function handle_import" ./MELP/MLP/stage1/modules/import_handler.mlp; then
    echo "✅ PASS: handle_import() function defined"
else
    echo "❌ FAIL: handle_import() function not found"
    exit 1
fi
echo ""

echo "=========================================="
echo "✅ ALL READINESS CHECKS PASSED!"
echo "=========================================="
echo ""
echo "TODO #2 (Import System) readiness: 5/5 ✅"
echo "- import_handler.mlp exists (259 lines)"
echo "- ImportContext struct: ✅"
echo "- Circular detection: ✅"
echo "- MLP_SOURCE_DIR: ✅"
echo "- handle_import(): ✅"
echo ""
echo "📋 NEXT: Stage2'de import system'i aktive et"
echo "    → mlp_compiler_main.mlp'de import_handler kullan"
echo "    → Parser'da import statement handle et"
echo "    → Stage0 C binding ekle"

