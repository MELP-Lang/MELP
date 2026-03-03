# MELP %100 Modular Architecture

## Durum
- Tarih: 15 Şubat 2026
- Geçerli durum: Phase 1-5 test/doğrulama akışı aktif
- Mimari sinyal: `./scripts/check_architecture.sh` PASS

## Temel Kurallar
- Modül CMake pattern: yalnız `add_executable()`
- Modüller arası doğrudan include/link coupling yok
- Orchestrator, modül binary çağrısını subprocess üzerinden yürütür

## Plug-and-Play Akışı
1. Yeni modül binary’sini `build/bin/` altına kopyala
2. Orchestrator çalıştır: `./build/compiler/stage1/modules/orchestrator/melp_compiler <input> -o <out.ll>`
3. Discovery çıktısında modül sayısı otomatik artar

## Doğrulama Komutları
```bash
# Mimari gate
./scripts/check_architecture.sh

# Core Golden (orchestrator + phase kapsamı)
bash TODO_100_PERCENT_MODULAR/YZ/golden_tests/core/run_core_checks.sh \
  compiler/stage1/modules/orchestrator \
  compiler/stage1/modules/variable_compiler \
  compiler/stage1/modules/function_compiler \
  compiler/stage1/modules/import \
  compiler/stage1/modules/vector \
  compiler/stage1/modules/heap \
  compiler/stage1/modules/binary_ops

# Phase 4 Golden
bash TODO_100_PERCENT_MODULAR/YZ/golden_tests/phase4/run_phase_checks.sh

# Phase 5 Golden
bash TODO_100_PERCENT_MODULAR/YZ/golden_tests/phase5/run_phase_checks.sh
```

## Mevcut Test Sonuç Özeti
- Standalone module tests: PASS (6/6)
- Core Golden (orchestrator dahil): PASS
- Phase 4 Golden: PASS (5/5)
- Phase 5 Golden: PASS

## Not
Bu doküman yalnızca mevcut doğrulanmış durumu özetler; ÜA bağımsız validasyon PASS olmadan nihai kapanış beyanı yapılmaz.
