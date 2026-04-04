# Stage1 Canonical Language Policy

This repository's canonical source language is English MLP.

## ⚠️ COMPILATION PIPELINE — READ BEFORE TESTING

Stage1 and Stage0 are **strictly isolated**:

| Layer | Scope | Status |
|---|---|---|
| **Stage1** | `.mlp` source files, normalizer input, examples, docs | Active |
| **Stage0** | `melp_normalizer_bin`, `melp_compiler`, lexer/parser/codegen | **FROZEN** |

Pipeline:
```
.mlp (English/Turkish/Russian MLP)
  ↓  melp_normalizer_bin   ← Stage1/Stage0 boundary
PMLP (internal: end_function, end_if, else_if, ...)
  ↓  melp_compiler         ← Stage0 (frozen, never modify)
LLVM IR
```

**NEVER call `melp_compiler` directly on `.mlp` files.**  
English MLP uses space-separated tokens (`end function`, `end if`). The normalizer converts these to the PMLP underscore form the compiler expects. Skipping the normalizer causes "Expected function name" / "Expected expression" errors.

To test compilation of any `.mlp` file:
```bash
# Always use the wrapper:
bin/run_melp path/to/file.mlp

# Or manual pipeline (english/turkish/russian):
cp file.mlp /tmp/.melp_norm_src && printf 'english' > /tmp/.melp_norm_lang
/home/pardus/PROJELER/MELP/ORTAK/normalizer/melp_normalizer_bin > /tmp/norm.pmlp
melp_compiler /tmp/norm.pmlp -o out.ll

# WRONG:
melp_compiler file.mlp -o out.ll  # ← bypasses normalizer, DO NOT DO THIS
```

Mandatory rules for all future work:

- Treat English MLP as the default language of the project.
- The source of truth for public keyword forms is `/home/pardus/PROJELER/MELP/ORTAK/dil/<lang>/keywords.json`.
- Before writing or rewriting any public-facing MELP example, check the relevant `keywords.json` file instead of inferring syntax from existing docs, examples, editor READMEs, or legacy files.
- Do not create new public-facing `.mlp` code, docs, tests, examples, comments, or explanations in legacy Stage0 syntax.
- Do not use or mention `MLP` or `MLP` in new user-facing content.
- Consider legacy underscore tokens such as `end_function`, `end_if`, `end_for`, `end_while`, `end_match`, `end_enum`, `end_struct`, and `else_if` to be internal historical syntax only.
- Do not copy legacy Stage0 examples as the template for new `.mlp` files.
- If old examples conflict with this policy, this policy wins.

Canonical localized examples must also come from `keywords.json`:

- English canonical forms come from `/home/pardus/PROJELER/MELP/ORTAK/dil/en/keywords.json`.
- Turkish canonical forms come from `/home/pardus/PROJELER/MELP/ORTAK/dil/tr/keywords.json`.
- Treat `aliases` entries as accepted alternate spellings, not the default public documentation form, unless the user explicitly asks for aliases.

Repository hygiene expectations:

- If a public-facing document conflicts with `keywords.json`, treat the document as stale and update it toward `keywords.json`.
- Do not infer Turkish syntax from prior README files; prefer the canonical mappings such as `print -> yaz`, `main -> giriş`, `end function -> fonksiyon sonu`, `if -> koşul`, `else -> yoksa`, `end if -> koşul sonu`.
- When uncertainty remains, state that `ORTAK/dil/.../keywords.json` is authoritative and avoid inventing forms.

Canonical English MLP syntax for new code:

- `function main()` ... `end function`
- `if ... then` / `else if ... then` / `else` / `end if`
- `while ...` / `end while`
- `for ...` / `end for`
- `for each item in items` / `end for`
- `loop` / `end loop`
- `struct ...` / `end struct`
- `enum ...` / `end enum`
- `match ...` / `end match`
- `case ...` / `end case`
- `interface ...` / `end interface`
- `module ...` / `end module`

Language hierarchy:

- English MLP is the canonical authoring language.
- Turkish and other localized forms are translations into English MLP.
- Internal compiler normalization details are not part of the public language contract.

Behavior expectations for agents:

- When asked to add or rewrite examples, produce English MLP first.
- When migrating legacy files, rewrite them into English MLP instead of preserving legacy Stage0 syntax.
- When explaining the language to users, describe only MLP. Do not surface MLP terminology unless the user explicitly asks about legacy internals.
- If a task touches compiler internals, keep any unavoidable legacy discussion confined to internal implementation notes, not public docs or examples.
