# TREE_SITTER - Özellik Kartı

**Özellik:** Tree-sitter Grammar  
**Durum:** 🔴 TODO (Phase 4.4)  
**Sözdizimi Sürümü:** N/A (Parser generator)

---

## 🎯 GRAMER DOSYASI

### grammar.js (Tree-sitter)
```javascript
module.exports = grammar({
  name: 'mlp',
  
  rules: {
    source_file: $ => repeat($._statement),
    
    _statement: $ => choice(
      $.function_declaration,
      $.variable_declaration,
      $.expression_statement
    ),
    
    function_declaration: $ => seq(
      optional(field('return_type', $.type)),  // MELP kanonik: numeric function foo()
      'function',
      field('name', $.identifier),
      $.parameter_list,
      $.block,
      'end function'
    ),
    
    identifier: $ => /[a-zA-Z_][a-zA-Z0-9_]*/
  }
});
```

### Syntax Highlighting
- Keywords: `function`, `if`, `for`
- Types: `numeric`, `character[]`
- Operators: `+`, `-`, `*`, `/`
- Comments: `--`

---

**Phase:** 4.4 (Month 7)
