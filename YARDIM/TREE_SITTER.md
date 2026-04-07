# MELP Tree-sitter Grammar Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 TODO — Phase 4.4

---

## Genel Bilgi

Tree-sitter grammar, MELP için hızlı ve hatasız toleranslı syntax parsing sağlar.  
VS Code, Neovim, Emacs gibi editörlerde syntax highlighting için kullanılır.

> **⚠️ UYARI:** Tree-sitter grammar Phase 4.4'e kadar implement edilmeyecektir.  
> Şu an VS Code uzantısında TextMate grammar kullanılmaktadır.

---

## Planlanan grammar.js

```javascript
// grammar.js (Tree-sitter)
module.exports = grammar({
  name: 'mlp',
  
  rules: {
    source_file: $ => repeat($._statement),
    
    _statement: $ => choice(
      $.function_declaration,
      $.variable_declaration,
      $.if_statement,
      $.for_loop,
      $.while_loop,
      $.expression_statement
    ),
    
    function_declaration: $ => seq(
      'function',
      field('name', $.identifier),
      $.parameter_list,
      'as',
      field('return_type', $.type),
      $.block,
      'end function'
    ),
    
    parameter_list: $ => seq(
      '(',
      optional(seq(
        $.parameter,
        repeat(seq(';', $.parameter))
      )),
      ')'
    ),
    
    identifier: $ => /[a-zA-Z_][a-zA-Z0-9_]*/
  }
});
```

---

## Syntax Highlighting Hedefleri

| Kategori | Highlight Rengi |
|----------|----------------|
| Keywords | Mavi: `function`, `if`, `for`, `while` |
| Types | Yeşil: `numeric`, `string`, `boolean` |
| Operators | Sarı: `+`, `-`, `*`, `/`, `mod` |
| Comments | Gri: `--` ile başlayan |
| Strings | Turuncu: `"..."` |
| Numbers | Açık Yeşil: `42`, `0` |

---

## Şu Anki Durum

VS Code uzantısı TextMate grammar kullanıyor:
```bash
editors/vsix/syntaxes/mlp.tmLanguage.json
```

---

## İlgili Özellikler

- **LSP** — Language Server (tree-sitter ve birlikte kullanılır)
- **CODE_INTELLIGENCE** — Refactoring özellikleri

**Ayrıca bakınız:**
- [LSP.md](LSP.md)
- [belgeler/EDITOR_INTEGRATION_GUIDE.md](../belgeler/EDITOR_INTEGRATION_GUIDE.md)

---

## Durum Tablosu

| Özellik | Phase 4.4 | Şu An |
|---------|-----------|-------|
| Tree-sitter grammar | ✅ Planlı | ❌ |
| `grammar.js` | ✅ | ❌ |
| TextMate grammar | N/A | ✅ (VSIX) |
| Node packages | ✅ | ❌ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `while_loop` (grammar rule adı) | `loop_statement` |
| `'function' ... 'as' ...` | return tip `'as'` yerine önde: `tip function` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/TREE_SITTER.md](../belgeler/language/feature_cards/TREE_SITTER.md)
