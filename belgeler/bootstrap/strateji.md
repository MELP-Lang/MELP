# MELP Bootstrap Stratejisi — Özet

**Tam kaynak:** [`belgeler/BOOTSTRAP_STRATEGY.md`](../../belgeler/BOOTSTRAP_STRATEGY.md)
**Durum:** ✅ ONAYLANDI (Üst Akıl, Ocak 2026)

---

## Temel Fikir: Source Project

MELP bir **kaynak kütüphane projesini** derlemeyi hedefler, kendini değil.

```
compiler/TC/      → Minimal bootstrap derleyicisi (bağımsız)
compiler/base/    → Temel özellikler (bağımsız)
compiler/ideal/   → İleri özellikler (bağımsız)
compiler/modern/  → En güncel özellikler (bağımsız)

Kural: Hiçbiri diğerine bağımlı değil.
Strateji: TC → BASE (TC kopyala + genişlet) → IDEAL → MODERN
```

Neden code duplication tolere edilir?
- Her modül standalone → başka projelerde copy-paste çalışır
- Binary değil kaynak dağıtılır
- Zero dependency → her modül kendi lexer/parser/codegen içerir

---

## Üç Aşama

### Stage 0 — C++ ile C++ Derle ✅

```bash
# Mevcut durum: Stage 0 çalışıyor
cd LLVM/STAGE0/
mkdir build && cd build
cmake ..
make
./melp_compiler input.mlp -o output
```

**Ne var:**
- Lexer (PMLP tokenları)
- Parser (AST)
- STO (tip çıkarımı: numeric → i64/f64/BigDecimal)
- SAO (saflık analizi, temel)
- FBO (4-byte encoding, Phase 5.1)
- LLVM IR codegen
- Debug module
- Scope/exit if
- Temel struct, if/loop/function

**Kısıtlamalar:** `belgeler/STAGE0_LIMITATIONS.md`

---

### Stage 1 — .mlp Modülleri C++ Stage 0 ile Derle 🔄

**Hedef:** `.melp` dosyaları Stage 0 binary ile derlenebilir olmalı.

**Eksik özellikler (Stage 1 tamamlanması için):**
- Enum + match (exhaustive)
- Generics / monomorphization
- Result<T;E> / Option<T> stdlib
- defer
- FFI geliştirilmiş API
- Normalizer Türkçe ek kelimeler

Rehber: `belgeler/STAGE0_TO_STAGE1_GUIDE.md`

---

### Stage 2 — Self-Hosting 🗓️

**Hedef:** MELP derleyicisinin kendisi `.mlp` ile yazılacak ve Stage 1 derleyiciyle derlenecek.

**Önkoşullar:**
- Stage 1 tamamlanmış ve stabil
- WASM backend (tarayıcıdan derleme için)
- Ownership modeli uygulanmış

---

## Öğrenilen Dersler

1. **Monolitik `main.c` yasaklanmıştır** — önceki AI ajanlar bu hatayı yaptı
2. **Modüler mimari zorunlu** — her dosya tek sorumluluk
3. **Stage atlamamak** — Stage 2 öncesi Stage 1 tamamlanmalı
4. **WASM Stage 2 öncesi** — WASM backend C++'ta yapılmazsa Stage 2'de çifte iş

Detay: `belgeler/LESSONS_LEARNED.md`
