---
applyTo: "**/*.{md,mlp}"
description: "Use when writing or editing MELP docs, examples, or source files. Enforces canonical keyword selection from ORTAK/dil/*/keywords.json and blocks legacy or inferred syntax."
---

# MELP Canonical Language Instruction

For any public-facing MELP documentation, examples, comments, tests, or `.mlp` source files in this repository:

- Treat `/home/pardus/PROJELER/MELP/ORTAK/dil/<lang>/keywords.json` as the source of truth for keyword forms.
- Do not infer keyword spellings from existing README files, editor docs, examples, or legacy files.
- Use English MLP as the default canonical authoring form unless the task explicitly requests a localized example.
- For Turkish examples, prefer the canonical `keywords` values over `aliases` values.
- Treat aliases as accepted variants, not the default public documentation form.
- Never introduce legacy underscore forms such as `end_function`, `end_if`, `end_for`, `end_while`, `end_match`, `end_enum`, `end_struct`, or `else_if` in public-facing content.
- Treat `belgeler_old/` as archival material outside the active Stage1 documentation surface.
- Do not expand, modernize, or promote `belgeler_old/` as a living documentation tree.
- When consolidating Stage1 documentation, prefer a single active reference tree instead of parallel authoritative trees.
- In this repository, prefer `belgeler_v2/` as the target reference tree unless the user explicitly requests a different target.

Canonical reminder:

- English canonical block forms are space-separated, such as `end function`, `end if`, `end for`, `end while`, `end match`, `end enum`, and `end struct`.
- Turkish canonical forms must come from `/home/pardus/PROJELER/MELP/ORTAK/dil/tr/keywords.json`, for example `function -> fonksiyon`, `end function -> fonksiyon sonu`, `main -> giriş`, `print -> yaz`, `if -> koşul`, `else -> yoksa`, `end if -> koşul sonu`.

If repository content conflicts with these files, treat the repository content as stale.