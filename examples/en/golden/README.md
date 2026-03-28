# MLP Golden Test Suite — English

Bu dizin, Stage0 PMLP golden test suite'inin English MLP karşılığıdır.  
`scripts/convert_golden_to_mlp.sh` tarafından otomatik üretilmiştir.

## Sonuçlar (Stage1 pipeline)

```
PASS : 681 / 790  (86%)
```

### Tam Geçen Kategoriler (81) ✅

| Kategori | Test Sayısı | Açıklama |
|---|---|---|
| array_native | 5 | Dizi literalleri ve erişim |
| array_tests | 5 | Dizi operasyonları |
| auto_type_tests | 5 | Otomatik tip çıkarımı |
| cli_runtime | 5 | CLI argümanları |
| collections_runtime | 5 | Koleksiyon runtime |
| combinators | 5 | Fonksiyon kombinatörleri |
| control_flow | 10 | if/else, while, for |
| debug_block | 3 | debug bloğu |
| debugger | 7 | Debugger desteği |
| debugger_tests | 7 | Debugger testleri |
| defer | 3 | defer ifadesi |
| electron_ide | 5 | Electron IDE entegrasyonu |
| enum_advanced | 5 | Gelişmiş enum |
| enum_tests | 3 | Enum testleri |
| error_handling_tests | 5 | Hata yakalama |
| event_block | 5 | Event blokları |
| event_param_tests | 5 | Event parametreleri |
| ffi_tests | 3 | FFI (Foreign Function Interface) |
| float_numeric | 5 | Kayan nokta sayılar |
| for_downto | 5 | `for i = 10 downto 0` |
| for_each | 5 | `for each item in list` |
| fs_runtime | 5 | Dosya sistemi runtime |
| generics_basic | 3 | Temel generics |
| hashmap_basic | 3 | HashMap temel |
| hashmap_method_tests | 5 | HashMap metotları |
| hashmap_tests | 2 | HashMap testleri |
| higher_order | 10 | Yüksek dereceli fonksiyonlar |
| hof | 1 | HOF temel |
| hof_array | 3 | Dizi üzerinde HOF |
| ide_advanced | 5 | IDE gelişmiş özellikler |
| if_let | 5 | `if let` pattern |
| if_tests | 5 | if/else testleri |
| interfaces_tests | 30 | Arayüz (interface) |
| io | 6 | Giriş/Çıkış |
| lambda_hof | 3 | Lambda + yüksek dereceli |
| lambda_if | 2 | Lambda + koşullu |
| lambda_let | 2 | Lambda + let |
| lsp | 28 | LSP (Language Server Protocol) |
| lsp_intelligence | 15 | LSP zeka özellikleri |
| method_keyword | 5 | `method` anahtar kelimesi |
| module_block | 2 | Modül blokları |
| module_namespace | 5 | Modül namespace |
| native_binary | 4 | Native binary çıktı |
| network_runtime | 6 | Ağ runtime |
| none_type | 2 | None tipi |
| nullable | 3 | Nullable tipler |
| option_type | 16 | Option/Maybe tipi |
| pattern_matching_advanced | 20 | Gelişmiş pattern matching |
| phase7_async | 4 | Async/await |
| phase8_testing | 1 | Test framework |
| power_op | 2 | Üs operatörü (`**`) |
| product_polish | 10 | Ürün kalitesi örnekleri |
| program_sugar | 5 | Sözdizimi şekeri |
| range_for | 3 | Range tabanlı for |
| repl | 10 | REPL desteği |
| result_type | 18 | Result tipi |
| spawn_handle_tests | 5 | Spawn/goroutine benzeri |
| stdin_tests | 2 | Standart giriş |
| stdlib | 11 | Standart kitaplık |
| str_builtin | 13 | Yerleşik string fonksiyonları |
| str_heap | 7 | Heap string |
| string_array | 4 | String dizi |
| string_eq | 5 | String eşitlik |
| string_interp | 4 | String interpolasyon |
| string_literal_bool | 5 | String literal boolean |
| string_ops | 4 | String operasyonları |
| string_tests | 5 | String testleri |
| struct_advanced | 3 | Gelişmiş struct |
| switch_case | 3 | Match/switch |
| ternary | 4 | Üçlü operatör |
| test_utils | 5 | Test yardımcıları |
| to_number_tests | 5 | Sayıya dönüştürme |
| type_aliases | 16 | Tip takma adları |
| user_string_return | 5 | String döndüren fonksiyonlar |
| validation_testing | 10 | Doğrulama testleri |
| variable_tests | 5 | Değişken testleri |
| void_return | 5 | void dönüş tipi |
| week_5_2_hof_filesystem | 10 | HOF + dosya sistemi |
| week_5_4_lsp_network | 5 | LSP + ağ |
| while_tests | 5 | while döngüsü |
| with_block | 5 | `with` bloğu |
| xor_op | 2 | XOR operatörü |

### Kısmen Geçen Kategoriler (25) ⚠️

| Kategori | Geçen | Kalan | Neden |
|---|---|---|---|
| build_process | 4 | 1 | Derleme süreci edge case |
| code_intelligence | 4 | 1 | LSP zeka edge case |
| enum | 23 | 3 | Enum payload bazı formlar |
| error_propagation | 16 | 5 | `?` operatörü ve yayılma |
| funcref_tests | 5 | 5 | Fonksiyon referansları bazıları |
| hof_tests | 13 | 4 | HOF + tip çıkarımı |
| import_tests | 1 | 6 | `import "file"` — eşlik dosyası gerekli |
| lambda | 3 | 4 | Lambda tip çıkarımı bazı formlar |
| lambda_tests | 6 | 5 | Lambda karmaşık formlar |
| module_import_export | 1 | 4 | Modül yolu — eşlik dosyası gerekli |
| module_resolution | 1 | 8 | Göreceli modül yolları — eşlik dosyası gerekli |
| modules | 3 | 2 | Modül sistemi bazı formlar |
| month_09_generics | 23 | 2 | Generics kısıtlamalar |
| named_params_order | 4 | 1 | İsimli parametre sırası |
| package_manager | 3 | 2 | Paket yöneticisi |
| pattern_matching | 8 | 2 | Pattern matching bazı formlar |
| reserved_keywords | 4 | 1 | Ayrılmış kelime edge case |
| result_option | 1 | 4 | Result/Option karmaşık pattern |
| selfhost_prep | 6 | 3 | Self-host hazırlığı |
| struct | 25 | 5 | Struct metot dispatch bazıları |
| struct_constructor | 4 | 1 | Struct constructor edge case |
| testing_framework | 4 | 1 | Test framework edge case |
| test_utils_runtime | 4 | 1 | Test yardımcısı runtime |
| tree_sitter | 4 | 1 | Tree-sitter entegrasyon |
| type_safety | 3 | 2 | Tip güvenliği katı kontroller |

### Desteklenmeyen Özellikler (8 kategori sıfır geçti) ❌

| Kategori | Hata | Durum | Açıklama |
|---|---|---|---|
| `closure` | `Unknown identifier: x` | Stage0 codegen bug | Lambda dış kapsam değişkenlerini yakalayamıyor |
| `default_params` | `Expected ')' after parameters` | Uygulanmadı | `fun(x; y: "default")` sözdizimi yok |
| `ref_param_tests` | `Expected parameter name or type` | Uygulanmadı | `ref` anahtar kelimesi parametrelerde kabul edilmiyor |
| `variadic_spread` | `Expected parameter name after type` | Uygulanmadı | `numeric...` değişken uzunluklu parametre yok |
| `higher_order_functions` | `Expected ']'` | Uygulanmadı | `function` tipi parametre olarak kullanılamıyor |
| `channels_token` | Sözdizimi hataları | Beklenen | Bu testler kasıtlı olarak yanlış sözdizimi içeriyor |
| `error_location` | `Unknown identifier` | Beklenen | Bu testler tanımsız değişken hatalarını test ediyor |
| `stdlib_import` | `Module not found` | Runtime | `import stdlib/io` çalışma zamanı modül çözümlemesi gerektirir |

## Pipeline

Bu dizindeki tüm `.mlp` dosyaları English MLP formatındadır:

```bash
# Tek dosya test:
bin/run_melp examples/en/golden/control_flow/test_01_if_else.mlp

# Tüm golden testleri çalıştır:
bash scripts/test_golden_mlp.sh
```

## Dönüşüm Süreci

```
STAGE0/GOLDEN_TEST_SUITE/*.mlp  (legacy Stage0 fixtures)
  ↓  scripts/convert_golden_to_mlp.sh
examples/en/golden/*.mlp        (canonical English MLP)
  ↓  bin/run_melp
LLVM IR → binary
```

Dönüşüm, eski Stage0 kapanış biçimlerini kanonik English MLP yazımına normalize eder.
