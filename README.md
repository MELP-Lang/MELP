# MELP

> **Scope is the universal lifetime unit.**
> No GC · No borrow checker · Memory safe

---

## 🚧 Under construction

This repository is **not current**. The code and documents here are a snapshot
from April 2026 and describe an earlier design; several things they claim are no
longer part of the language.

Work right now is focused on **custody** — the scope-bound resource-lifetime
mechanism the whole model rests on. Until that lands and is proven, this
repository is not a reliable description of MELP.

### → For current information: **[melp.dev](https://melp.dev)**

| | |
|---|---|
| What MELP is | [melp.dev](https://melp.dev) |
| Features, each with an evidence stamp | [melp.dev/features.html](https://melp.dev/features.html) |
| Language reference | [melp.dev/docs.html](https://melp.dev/docs.html) |
| Try it in your browser — the real compiler | [melp.dev/web-ide/](https://melp.dev/web-ide/) |
| Benchmarks, with the full protocol | [melp.dev/benchmark.html](https://melp.dev/benchmark.html) |
| Türkçe | [melp.dev/tr/](https://melp.dev/tr/) |

---

## Status, honestly

| | |
|---|---|
| Self-hosting — the compiler compiles its own source | ✅ |
| Custody — scope-bound memory, no GC, no annotations | 🔬 in progress |
| Public release | ❌ none yet — there is no downloadable binary |

The way to run MELP today is the [Web IDE](https://melp.dev/web-ide/): the
self-hosted compiler built to WASM, running in the browser, nothing to install.

Claims about the language belong on melp.dev, where each one carries its evidence
stamp and its honesty line. This page deliberately makes none.

## Licence

© 2026 MELP. All rights reserved.

MELP is closed source. No licence to use, copy, modify or distribute the
contents of this repository is granted.
