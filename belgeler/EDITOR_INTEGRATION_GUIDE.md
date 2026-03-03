# MLP Editor Integration Guide

> For IDE plugins, web editors, and language servers integrating MLP language support.
> **Source of truth:** `MLP_LANGUAGE_REFERENCE.md` — use `.mlp` syntax, never internal pmlp identifiers.

---

## Overview

MLP editor support consists of four layers:

| Layer | What it does | File |
|-------|-------------|------|
| **Syntax highlighting** | Color keywords, strings, numbers | `syntaxes/mlp.tmLanguage.json` |
| **Diagnostics** | Red underlines for errors/warnings | `mlpDiagnosticProvider.ts` |
| **Completion** | IntelliSense / autocomplete | `mlpCompletionProvider.ts` |
| **Hover** | Tooltip documentation | `mlpHoverProvider.ts` |

All function metadata is stored in `data/mlp_functions.json` — a single source for completion and hover across all editors (VSIX, CodeMirror, Monaco).

---

## 1. Compiler Error Format

The `melp_compiler` outputs errors in this format:

```
Error at line 5:12: Expected 'then' after 'if' condition
Error at line 10:1: Undefined variable 'foo'
```

Pattern: `Error at line <LINE>:<COLUMN>: <MESSAGE>`

Parse with:
```js
const pattern = /Error at line (\d+):(\d+): (.+)/;
const match = output.match(pattern);
if (match) {
  const line = parseInt(match[1]) - 1;   // 0-indexed for editors
  const col  = parseInt(match[2]) - 1;
  const msg  = match[3];
}
```

---

## 2. Static Diagnostics (No Compiler Required)

These checks can be done in-editor without running the compiler:

### E1 — Missing `then`

```
if x > 5             ← Error: missing 'then'
```

**Pattern:** Line starts with `if` or `else_if` but does not contain `then`

```js
if (/^\s*(if|else_if)\b/.test(line) && !/\bthen\b/.test(line)) {
  // E1: 'if' requires 'then'
}
```

### E2 — Two-token `else if`

```
else if x > 0 then   ← Error: must be 'else_if' (single token)
```

```js
if (/\belse\s+if\b/i.test(line)) {
  // E2: use 'else_if' not 'else if'
}
```

### E3 — Two-token block terminators

```
end if               ← Error: must be 'end_if'
end while            ← Error: must be 'end_while'
```

```js
const splitClosers = ['end if','end while','end for','end function',
                      'end struct','end enum','end match','end try'];
for (const s of splitClosers) {
  if (line.trim().toLowerCase().startsWith(s)) {
    // E3: use 'end_if' not 'end if'
  }
}
```

### E4 — Orphan block terminator

```
end_if               ← Error: no matching 'if' was opened
```

Implement with a block stack (push on openers, pop on terminators):

```js
const BLOCK_PAIRS = {
  'if': 'end_if', 'while': 'end_while', 'for': 'end_for',
  'function': 'end_function', 'struct': 'end_struct',
  'enum': 'end_enum', 'match': 'end_match', 'try': 'end_try',
  'lambda': 'end_lambda', 'debug': 'end_debug',
};
```

### E5 — Unclosed block (end of file)

All items remaining in the block stack after parsing → unclosed block errors.

### E6 — Unmatched parentheses

Count `(` and `)` on each line, ignoring string contents and `--` comments.

### E7 — Unclosed string literal

Track `"` characters; if a line ends while inside a string → error.

### W1 — `break` instead of `exit`

```
break               ← Warning: use 'exit' in MLP
```

### W6 — C-style logical operators

```
if a && b then      ← Warning: use 'and', not '&&'
if !flag then       ← Warning: use 'not', not '!'
```

---

## 3. Block Structure

Every block has a matching terminator. All terminators are **single tokens** with underscore:

```mlp
function name(type param) as type
    if condition then
        while condition
            for i = 0 to 10
                match x
                    case 1 then ...
                    else ...
                end_match
            end_for
        end_while
    else_if other then
        struct Name
            ...
        end_struct
    else
        try
            ...
        catch err
            ...
        end_try
    end_if
    return value
end_function
```

| Opener | Terminator |
|--------|-----------|
| `function` | `end_function` |
| `if` | `end_if` |
| `else_if` | (part of `if` block) |
| `else` | (part of `if` block) |
| `while` | `end_while` |
| `for` | `end_for` |
| `struct` | `end_struct` |
| `enum` | `end_enum` |
| `match` | `end_match` |
| `try` | `end_try` |
| `lambda` | `end_lambda` |
| `debug` | `end_debug` |
| `module` | `end_module` |
| `event` | `end_event` |
| `interface` | `end_interface` |

---

## 4. Keyword List (Complete)

Use for syntax highlighting and completion:

```js
const KEYWORDS = [
  // Control flow
  'if', 'else_if', 'else', 'end_if', 'then',
  'while', 'end_while',
  'for', 'to', 'downto', 'step', 'end_for',
  'exit', 'continue',
  // Pattern matching
  'match', 'case', 'end_match',
  // Error handling
  'try', 'catch', 'finally', 'end_try',
  // Functions
  'function', 'end_function', 'return', 'as', 'none',
  'lambda', 'end_lambda',
  // Struct / Enum / Interface
  'struct', 'end_struct',
  'enum', 'end_enum',
  'interface', 'end_interface',
  'implement', 'end_implement', 'implements',
  'this',
  // Logical operators
  'and', 'or', 'not',
  // Literals
  'true', 'false', 'null',
  // Builtins
  'print', 'str', 'typeof', 'range', 'mod',
  // Collections
  'for', 'each', 'in',
  // Modules
  'module', 'end_module', 'import', 'export',
  // Debug (MELP-specific, zero-cost in release)
  'debug', 'end_debug', 'pause', 'goto',
  // GUI events (.mlpgui)
  'event', 'end_event',
];

const TYPES = ['numeric', 'string', 'boolean', 'none'];
```

---

## 5. TextMate Grammar Scopes

Used by `syntaxes/mlp.tmLanguage.json`:

| Scope | Applies to |
|-------|-----------|
| `keyword.control.mlp` | `if`, `else_if`, `else`, `end_if`, `while`, `end_while`, `for`, `end_for`, `match`, `end_match`, `try`, `catch`, `finally`, `end_try`, `return`, `then` |
| `keyword.other.mlp` | `function`, `end_function`, `struct`, `end_struct`, `enum`, `end_enum`, `import`, `module`, `end_module`, `as`, `lambda`, `end_lambda` |
| `storage.type.mlp` | `numeric`, `string`, `boolean`, `none` |
| `constant.language.mlp` | `true`, `false`, `null` |
| `keyword.operator.mlp` | `and`, `or`, `not`, `mod` |
| `string.quoted.double.mlp` | `"..."` |
| `string.quoted.single.mlp` | `'...'` |
| `comment.line.mlp` | `-- ...` |
| `entity.name.function.mlp` | Function name after `function` keyword |
| `entity.name.type.mlp` | Struct/enum name after `struct`/`enum` |
| `constant.numeric.mlp` | Number literals: `42`, `3,14` |

### VSCode Theme Note

`Default Light Modern` and `Default Dark Modern` themes use **semantic tokens** (LSP-based), not TextMate grammar. To force TextMate grammar colors in these themes, add to `settings.json`:

```json
"editor.tokenColorCustomizations": {
  "textMateRules": [
    { "scope": "keyword.control.mlp",
      "settings": { "foreground": "#7C3AED", "fontStyle": "bold" } },
    { "scope": "storage.type.mlp",
      "settings": { "foreground": "#2563EB", "fontStyle": "bold" } },
    { "scope": "entity.name.function.mlp",
      "settings": { "foreground": "#92400E" } },
    { "scope": "string.quoted.double.mlp",
      "settings": { "foreground": "#DC2626" } },
    { "scope": "constant.numeric.mlp",
      "settings": { "foreground": "#16A34A" } },
    { "scope": "comment.line.mlp",
      "settings": { "foreground": "#16A34A", "fontStyle": "italic" } }
  ]
}
```

---

## 6. Completion Items

### Snippet Templates

| Trigger | Expands to | Note |
|---------|-----------|------|
| `fn` | `function name(numeric param) as numeric ... end_function` | Main function template |
| `main` | `function main() as numeric ... return 0 end_function` | Entry point |
| `if` | `if condition then ... end_if` | Basic if |
| `ife` | `if condition then ... else ... end_if` | If-else |
| `ifel` | `if ... then ... else_if ... then ... else ... end_if` | Full if chain |
| `wh` | `while condition ... end_while` | While loop |
| `fr` | `for i = 0 to 10 ... end_for` | Ascending for |
| `frd` | `for i = 10 downto 0 ... end_for` | Descending for |
| `frs` | `for i = 0 to 100 step 10 ... end_for` | Stepped for |
| `st` | `struct Name ... end_struct` | Struct definition |
| `en` | `enum Name ... end_enum` | Enum definition |
| `dbg` | `debug ... end_debug` | Debug block (zero-cost in release) |
| `dbgl` | `debug: print(...)` | Single-line debug annotation |
| `event` | `event widget_onClick ... end_event` | GUI event handler |

### Function Completion from `mlp_functions.json`

Load the functions database and offer each function as a completion item with:
- `kind`: `Function`
- `detail`: signature string (e.g., `print(value: numeric) as none`)
- `documentation`: markdown with description, params, returns, example
- `insertText`: snippet with tab stops for each parameter

---

## 7. Hover Documentation

### Keywords

For each keyword hovered, show a markdown tooltip. Example for `if`:

```markdown
**if** `condition` **then** ... `end_if`

⚠️ `then` is **required!** `end_if` is a single token (with underscore).

```mlp
if score >= 90 then
    print("A")
else_if score >= 80 then
    print("B")
else
    print("F")
end_if
```
```

### User-Defined Functions

When hovering a function call, find the definition in the current document and show:

```markdown
```mlp
function add(numeric a; numeric b) as numeric
```

*Line 5 — defined in this file*
```

### Built-in Functions from `mlp_functions.json`

```markdown
```mlp
print(value) as none
```

**Prints value to stdout followed by newline**

**Parameters:**
- `value` *(any)*: Value to print

**Example:**
```mlp
print("Hello, World!")
print(42)
print("Count: " + str(x))
```
```

---

## 8. `mlp_functions.json` Schema

Location: `data/mlp_functions.json` (relative to extension root)

```json
{
  "categories": [
    {
      "id": "io",
      "name": "Input / Output",
      "functions": [
        {
          "name": "print",
          "signature": "print(value) as none",
          "description": "Prints value to stdout followed by a newline.",
          "params": [
            {
              "name": "value",
              "type": "any",
              "description": "The value to print (numeric, string, or boolean)"
            }
          ],
          "returns": {
            "type": "none",
            "description": "No return value"
          },
          "example": "print(42)\nprint(\"Hello\")\nprint(\"Value: \" + str(x))"
        }
      ]
    }
  ]
}
```

Full field reference:

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| `name` | string | ✅ | Function name (used as completion trigger) |
| `signature` | string | ✅ | Full signature shown in completion detail |
| `description` | string | ✅ | Short one-line description |
| `params` | array | ✅ | Parameter list (can be empty `[]`) |
| `params[].name` | string | ✅ | Parameter name |
| `params[].type` | string | ✅ | Parameter type |
| `params[].description` | string | ✅ | Parameter description |
| `returns` | object | optional | Return type and description |
| `returns.type` | string | ✅ if present | Return type |
| `returns.description` | string | ✅ if present | Return description |
| `example` | string | optional | Code example (newlines as `\n`) |

---

## 9. CodeMirror 6 Integration

For web editors using CodeMirror 6:

### Language Definition

```js
import { LanguageSupport, StreamLanguage } from '@codemirror/language';

const mlpLanguage = StreamLanguage.define({
  token(stream) {
    if (stream.match(/--.*$/)) return 'comment';
    if (stream.match(/"[^"]*"/)) return 'string';
    if (stream.match(/'[^']*'/)) return 'string';
    if (stream.match(/\b(if|else_if|else|end_if|then|while|end_while|for|end_for|to|downto|step|match|end_match|case|try|catch|finally|end_try|function|end_function|return|as|none|struct|end_struct|enum|end_enum|lambda|end_lambda|import|module|end_module|debug|end_debug)\b/))
      return 'keyword';
    if (stream.match(/\b(numeric|string|boolean)\b/))
      return 'typeName';
    if (stream.match(/\b(true|false|null)\b/))
      return 'bool';
    if (stream.match(/\b(and|or|not|mod)\b/))
      return 'operatorKeyword';
    if (stream.match(/\b\d+([,\.]\d+)*\b/))
      return 'number';
    stream.next();
    return null;
  }
});

export function mlp() {
  return new LanguageSupport(mlpLanguage);
}
```

### Autocompletion

```js
import { autocompletion, CompletionContext } from '@codemirror/autocomplete';
import mlpFunctions from './data/mlp_functions.json';

function mlpCompletions(context) {
  const word = context.matchBefore(/\w*/);
  if (!word || word.from === word.to) return null;

  const options = [
    // Keywords
    ...KEYWORDS.map(kw => ({ label: kw, type: 'keyword' })),
    // Functions from JSON
    ...mlpFunctions.categories.flatMap(cat =>
      cat.functions.map(fn => ({
        label: fn.name,
        type: 'function',
        detail: fn.signature,
        info: fn.description,
        apply: buildSnippet(fn),
      }))
    ),
  ];

  return { from: word.from, options };
}
```

### Linting

```js
import { linter, Diagnostic } from '@codemirror/lint';

const mlpLinter = linter(view => {
  const diagnostics = [];
  const lines = view.state.doc.toString().split('\n');
  const blockStack = [];

  lines.forEach((line, lineIdx) => {
    const trimmed = line.replace(/--.*$/, '').trim();

    // E1: if without then
    if (/^(if|else_if)\b/.test(trimmed) && !/\bthen\b/.test(trimmed)) {
      diagnostics.push({
        from: view.state.doc.line(lineIdx + 1).from,
        to: view.state.doc.line(lineIdx + 1).to,
        severity: 'error',
        message: "if condition requires 'then'",
      });
    }

    // E2: else if two tokens
    if (/\belse\s+if\b/.test(trimmed)) {
      diagnostics.push({ /* ... */ severity: 'error',
        message: "Use 'else_if' (single token), not 'else if'" });
    }
  });

  return diagnostics;
});
```

---

## 10. Compiler Integration (Run & Diagnostics)

To get real compiler diagnostics, pipe source through `melp_compiler`:

```js
// Node.js / Electron
const { execSync } = require('child_process');

function compile(sourceCode, compilerPath = 'melp_compiler') {
  const tmpFile = '/tmp/mlp_editor_tmp.mlp';
  fs.writeFileSync(tmpFile, sourceCode);

  try {
    const result = execSync(`${compilerPath} ${tmpFile} 2>&1`);
    return { success: true, output: result.toString() };
  } catch (err) {
    const stderr = err.stderr?.toString() ?? err.stdout?.toString() ?? '';
    const errors = parseCompilerErrors(stderr);
    return { success: false, errors };
  }
}

function parseCompilerErrors(output) {
  const errors = [];
  const pattern = /Error at line (\d+):(\d+): (.+)/g;
  let match;
  while ((match = pattern.exec(output)) !== null) {
    errors.push({
      line: parseInt(match[1]) - 1,    // 0-indexed
      column: parseInt(match[2]) - 1,
      message: match[3],
    });
  }
  return errors;
}
```

**Web API endpoint (for browser-based editors):**

```
POST /api/compile
Content-Type: application/json

{ "source": "function main() as numeric\n  print(42)\n  return 0\nend_function" }

Response:
{ "success": true, "output": "42\n" }

Error response:
{ "success": false, "errors": [
  { "line": 0, "column": 0, "message": "Expected 'then' after 'if' condition" }
]}
```

---

## 11. File Association

Register `.mlp` and `.mlpgui` extensions:

### VSCode (`package.json`)
```json
"languages": [{
  "id": "mlp",
  "extensions": [".mlp"],
  "configuration": "./language-configuration/mlp.json"
}]
```

### Language Configuration (`language-configuration/mlp.json`)
```json
{
  "comments": {
    "lineComment": "--"
  },
  "brackets": [
    ["(", ")"],
    ["[", "]"]
  ],
  "autoClosingPairs": [
    { "open": "(", "close": ")" },
    { "open": "[", "close": "]" },
    { "open": "\"", "close": "\"" }
  ],
  "indentationRules": {
    "increaseIndentPattern": "^\\s*(function|if|else|else_if|while|for|struct|enum|match|try|catch|debug|lambda|event|module|interface)\\b",
    "decreaseIndentPattern": "^\\s*(end_function|end_if|else|else_if|end_while|end_for|end_struct|end_enum|end_match|end_try|end_debug|end_lambda|end_event|end_module|end_interface)\\b"
  }
}
```

### CodeMirror / Monaco

Map `.mlp` file extension → `mlp` language mode.

---

## 12. Symbol Provider (Functions View)

Parse the document for function definitions to populate a function outline:

```js
const fnRegex = /^\s*function\s+([a-zA-Z_]\w*(?:\.\w+)?)\s*\(([^)]*)\)\s*(?:as\s+(\S+))?/gm;

function extractFunctions(source) {
  const functions = [];
  let match;
  while ((match = fnRegex.exec(source)) !== null) {
    functions.push({
      name: match[1],
      params: match[2].trim(),
      returnType: match[3] ?? 'numeric',
      line: source.substring(0, match.index).split('\n').length - 1,
    });
  }
  return functions;
}
```

---

*MLP Editor Integration Guide — Stage 0 — 2 March 2026*
