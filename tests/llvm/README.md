# MELP LLVM Backend Test Suite

LLVM backend için kapsamlı test altyapısı.

## 📊 Test Sonuçları (Son Güncelleme: 20 Aralık 2025)

### YZ_02: Unit Tests ✅
- **12 test** oluşturuldu
- **9/12 geçti** (%75)
- 3 test skipped (dead code issues)

### YZ_03: Regression Tests ✅
- **15 test** çalıştırıldı
- **15/15 geçti** (%100)
- **0 regression** detected 🎉

## Test Kategorileri

### 01_basic/ - Temel Özellikler
| Test | Açıklama | Beklenen Çıktı |
|------|----------|----------------|
| test_return.mlp | Basit return ifadesi | 42 |
| test_arithmetic.mlp | Temel aritmetik işlemler | 30 |
| test_variables.mlp | Değişken tanımlama ve kullanım | 88 |
| test_arithmetic_complex.mlp | Karmaşık aritmetik ifadeler | 17 |
| test_variable_arithmetic.mlp | Değişkenlerle aritmetik | 100 |

### 02_control/ - Kontrol Akışı
| Test | Açıklama | Beklenen Çıktı |
|------|----------|----------------|
| test_if.mlp | Basit if ifadesi | 99 |
| test_if_else.mlp | If-else yapısı | 55 |
| test_while.mlp | While döngüsü | 10 |
| test_for.mlp | For döngüsü | 15 |

### 03_functions/ - Fonksiyonlar
| Test | Açıklama | Beklenen Çıktı |
|------|----------|----------------|
| test_call.mlp | Basit fonksiyon çağrısı | 15 |
| test_params.mlp | Parametreli fonksiyon | 35 |
| test_multiple_calls.mlp | Çoklu fonksiyon çağrıları | 25 |

## Test Nasıl Çalıştırılır

### Unit Tests (YZ_02)
```bash
cd tests/llvm
./run_llvm_tests.sh
```

### Regression Tests (YZ_03)
```bash
cd tests/llvm
./run_quick_regression.sh        # Quick smoke test (2 saniye)
./run_regression_tests.sh        # Comprehensive (gelecekte)
```

## Test Sonuçları

- `test_results.log` - YZ_02 unit test results
- `regression_results.log` - YZ_03 regression results  
- `stage0_sample_results.csv` - Stage0 sample comparison

## Gereksinimler

- MELP Compiler (stage0/modules/functions/functions_compiler)
- LLVM toolchain (clang veya llc + gcc)

## Not

Testler MELP'in orijinal syntax'ını kullanır:
- `function...end function` blokları
- `returns numeric` return type
- `numeric` değişken tipi
- Noktalı virgül yok
