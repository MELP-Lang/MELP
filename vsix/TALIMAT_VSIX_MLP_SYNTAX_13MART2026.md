# VSIX MM TALİMATI — English MLP Sözdizimi Geçişi
**Tarih:** 13 Mart 2026  
**Öncelik:** Kritik  
**Kapsam:** `/home/pardus/projeler/MLP/editors/VSIX/src/`

---

## Arka Plan

Stage1 projesinin kanonik dili **English MLP**'dir.  
English MLP ile PMLP arasındaki fark şudur:

| Katman | Format | Örnek |
|--------|--------|-------|
| **MLP** (kullanıcı gören) | Boşluklu token | `end function`, `end if`, `else if` |
| **PMLP** (iç format) | Alt çizgili token | `end_function`, `end_if`, `else_if` |

Pipeline:
```
.mlp (English MLP)  →  melp_normalizer_bin  →  PMLP  →  melp_compiler_bin  →  LLVM IR
```

**Mevcut sorun:** VSIX denetleyicisi PMLP'yi kanonik dil olarak kabul ediyor.  
`end function` gördüğünde E3 hatası, `else if` gördüğünde E2 hatası üretiyor.  
Bu Stage1 politikasının tam tersidir.

**Yapılması gereken:** VSIX artık **MLP sözdizimini** doğru, **PMLP sözdizimini** geçersiz olarak işaretleyecek.

---

## Değişiklik 1: `mlpDiagnosticProvider.ts`

### 1a. `BLOCK_PAIRS` — kapayıcıları boşluklu forma çevir

**Dosya:** `src/mlpDiagnosticProvider.ts`

```typescript
// ÖNCE (PMLP — yanlış):
const BLOCK_PAIRS: Record<string, string> = {
  if: 'end_if',
  while: 'end_while',
  for: 'end_for',
  function: 'end_function',
  struct: 'end_struct',
  enum: 'end_enum',
  interface: 'end_interface',
  implement: 'end_implement',
  try: 'end_try',
  match: 'end_match',
  module: 'end_module',
  lambda: 'end_lambda',
  event: 'end_event',
  debug: 'end_debug',
  scope: 'end_scope',
  spawn: 'end_spawn',
  test: 'end_test',
};

// SONRA (English MLP — doğru):
const BLOCK_PAIRS: Record<string, string> = {
  if: 'end if',
  while: 'end while',
  for: 'end for',
  function: 'end function',
  struct: 'end struct',
  enum: 'end enum',
  interface: 'end interface',
  implement: 'end implement',
  try: 'end try',
  match: 'end match',
  module: 'end module',
  lambda: 'end lambda',
  event: 'end event',
  debug: 'end debug',
  scope: 'end scope',
  spawn: 'end spawn',
  test: 'end test',
};
```

### 1b. `SPLIT_CLOSERS` — tamamen kaldır (artık geçerli syntax)

`SPLIT_CLOSERS` dizisi ve bunu kullanan **E3 bloğu** tamamen silinecek.  
`end function`, `end if` vb. artık hata değil, doğru sözdizimi.

Silinecek kod bloğu:
```typescript
const SPLIT_CLOSERS = [
  'end if', 'end while', 'end for', 'end function', ...
];
```
Ve şu bloğun tamamı:
```typescript
// ── E3: İki-token kapanış ─────────────────────────────────────────
for (const split of SPLIT_CLOSERS) {
  if (trimmed.toLowerCase().startsWith(split)) {
    ...E3 diagnostic...
  }
}
```

### 1c. `E2: else if` kuralını tersine çevir

**Önce:** `else if` → hata (E2), `else_if` kullan  
**Sonra:** `else_if` → uyarı (W5 kapsamında), `else if` kullan

Mevcut E2 bloğunu şöyle değiştir:
```typescript
// ── E2: else_if alt-çizgili — artık eski PMLP syntax ─────────────
{
  const m = line.match(/\belse_if\b/i);
  if (m && m.index !== undefined) {
    const d = new vscode.Diagnostic(
      new vscode.Range(lineIndex, m.index, lineIndex, m.index + m[0].length),
      `'else_if' eski PMLP sözdizimi — 'else if' kullanın.`,
      vscode.DiagnosticSeverity.Warning
    );
    d.code = 'W5';
    d.source = 'MLP';
    diagnostics.push(d);
  }
}
```

### 1d. `LEGACY_KEYWORDS` — PMLP alt çizgili formları ekle

Şu anki `LEGACY_KEYWORDS` tablosuna aşağıdaki girişleri ekle:

```typescript
// PMLP (alt çizgili) formlar artık eski/legacy sayılır
'end_function':  'end function',
'end_if':        'end if',
'end_while':     'end while',
'end_for':       'end for',
'end_match':     'end match',
'end_struct':    'end struct',
'end_enum':      'end enum',
'end_interface': 'end interface',
'end_module':    'end module',
'end_try':       'end try',
'end_loop':      'end loop',
'else_if':       'else if',
```

W5 mesajındaki "PMPL karşılığı" ifadesini "MLP karşılığı" olarak güncelle.

### 1e. Blok kapayıcı tespiti — token çiftini boşluk-aware yap

Mevcut kod `firstToken` ile çalışıyor (tek token). `end function` gibi iki-token
kapayıcıları tespit etmek için satırın ilk **iki** tokenine bakması gerekiyor.

```typescript
// Mevcut (sadece tek token):
const firstToken = tokens[0].text.toLowerCase();
const closerEntry = Object.entries(BLOCK_PAIRS).find(([, closer]) => closer === firstToken);

// Yeni (iki-token kapayıcı desteği):
const firstToken  = tokens[0].text.toLowerCase();
const secondToken = tokens.length > 1 ? tokens[1].text.toLowerCase() : '';
const twoTokenKey = `${firstToken} ${secondToken}`.trim();

// Önce iki-token closer dene, sonra tek-token
const closerEntry =
  Object.entries(BLOCK_PAIRS).find(([, closer]) => closer === twoTokenKey) ??
  Object.entries(BLOCK_PAIRS).find(([, closer]) => closer === firstToken);

// closerEntry varsa, closer token'ın uzunluğu:
const closerLen = closerEntry ? closerEntry[1].length : 0;
```

E5 mesajındaki `'${BLOCK_PAIRS[unclosed.token]}' eksik` ifadesi de otomatik
doğru formu gösterecek çünkü `BLOCK_PAIRS` değerleri artık boşluklu.

### 1f. E1 kuralı — `else if` için `then` zorunluluğu

`else_if` kontrolünü `else if` olarak güncelle:
```typescript
// ÖNCE:
if (firstToken === 'if' || firstToken === 'else_if') {

// SONRA:
if (firstToken === 'if' || (firstToken === 'else' && secondToken === 'if')) {
```

---

## Değişiklik 2: `mlpCodeActionProvider.ts`

### 2a. E3 quick-fix kaldır

E3 artık üretilmeyeceği için ilgili `case 'E3'` bloğu kaldırılabilir (varsa).

### 2b. E2 quick-fix tersine çevir

```typescript
// ÖNCE:
case 'E2': {
  const action = new vscode.CodeAction(`'else if' -> 'else_if'`, ...);
  action.edit.replace(document.uri, diagnostic.range, 'else_if');
  ...
}

// SONRA (W5 olarak işaretlendiği için W5 quick-fix'i güncelle):
case 'W5': {
  // diagnostic.message 'else_if' içeriyorsa else if öner
  if (diagnostic.message.includes('else_if')) {
    const action = new vscode.CodeAction(`'else_if' → 'else if'`, vscode.CodeActionKind.QuickFix);
    action.diagnostics = [diagnostic];
    action.edit = new vscode.WorkspaceEdit();
    action.edit.replace(document.uri, diagnostic.range, 'else if');
    action.isPreferred = true;
    actions.push(action);
  }
  break;
}
```

### 2c. E5 quick-fix — boşluklu form ekle

```typescript
case 'E5': {
  const blockMatch = diagnostic.message.match(/'([^']+)' bloğu/);
  if (blockMatch) {
    // BLOCK_PAIRS artık 'end function' gibi değer döndürüyor
    const closer = `end ${blockMatch[1]}`;  // end function, end if vb.
    const line = document.lineAt(document.lineCount - 1);
    const action = new vscode.CodeAction(`Dosya sonuna '${closer}' ekle`, vscode.CodeActionKind.QuickFix);
    action.diagnostics = [diagnostic];
    action.edit = new vscode.WorkspaceEdit();
    action.edit.insert(document.uri, new vscode.Position(document.lineCount - 1, line.text.length), `\n${closer}\n`);
    actions.push(action);
  }
  actions.push(this.createHelpAction('Kapanmayan blok yardımını göster', 'E5_UNCLOSED_BLOCK'));
  break;
}
```

### 2d. HELP_TOPICS — örnekleri güncelle

```typescript
// ÖNCE:
E5_UNCLOSED_BLOCK: {
  body: [
    '```mlp',
    'function topla(numeric a; numeric b) as numeric',
    '    return a + b',
    'end_function',   // ← PMLP
    '```',
  ].join('\n'),
},

// SONRA:
E5_UNCLOSED_BLOCK: {
  body: [
    '```mlp',
    'function topla(numeric a; numeric b) as numeric',
    '    return a + b',
    'end function',   // ← English MLP
    '```',
  ].join('\n'),
},
```

---

## Değişiklik 3: `mlpCompletionProvider.ts`

### 3a. Anahtar kelime listesini güncelle

```typescript
// ÖNCE:
'if', 'else_if', 'else', 'end_if', 'then',
'while', 'end_while',
'for', 'to', 'downto', 'step', 'end_for',
'match', 'case', 'end_match',
'function', 'end_function', 'return', 'as', 'none',
'struct', 'end_struct',
'enum', 'end_enum',

// SONRA:
'if', 'else if', 'else', 'end if', 'then',
'while', 'end while',
'for', 'to', 'downto', 'step', 'end for',
'match', 'case', 'end match',
'function', 'end function', 'return', 'as', 'none',
'struct', 'end struct',
'enum', 'end enum',
```

### 3b. Snippet şablonlarını güncelle

```typescript
// ÖNCE:
['fn',   'function ${1:ad}(...)\n\t$0\n\treturn ${4:0}\nend_function', ...],
['if',   'if ${1:koşul} then\n\t$0\nend_if', ...],
['ife',  'if ${1:koşul} then\n\t${2}\nelse\n\t$0\nend_if', ...],
['wh',   'while ${1:koşul}\n\t$0\nend_while', ...],
['fr',   'for ${1:i} = ${2:0} to ${3:10}\n\t$0\nend_for', ...],
['main', 'function main()\n\t$0\nend_function', ...],

// SONRA:
['fn',   'function ${1:ad}(...)\n\t$0\n\treturn ${4:0}\nend function', ...],
['if',   'if ${1:koşul} then\n\t$0\nend if', ...],
['ife',  'if ${1:koşul} then\n\t${2}\nelse\n\t$0\nend if', ...],
['wh',   'while ${1:koşul}\n\t$0\nend while', ...],
['fr',   'for ${1:i} = ${2:0} to ${3:10}\n\t$0\nend for', ...],
['main', 'function main()\n\t$0\nend function', ...],
```

### 3c. `end_struct` regex'ini güncelle

```typescript
// ÖNCE:
const endStructRe = /^\s*end_struct\b/i;

// SONRA:
const endStructRe = /^\s*end\s+struct\b/i;
```

---

## Değişiklik 4: `mlpHoverProvider.ts`

Tüm hover açıklamalarındaki `end_*` formlarını `end *` olarak güncelle:

```typescript
// ÖNCE:
if: '**if** `koşul` **then** ... `end_if`\n\n`then` zorunludur.',
else_if: '**else_if** `koşul` **then** ...\n\nTek token kullanın.',
end_if: '**end_if** if bloğunu kapatır.',
while: '**while** `koşul` ... `end_while`\n\n`do` kullanılmaz.',
end_function: '**end_function** fonksiyon bloğunu kapatır.',

// SONRA:
if: '**if** `koşul` **then** ... `end if`\n\n`then` her zaman zorunludur.',
'else if': '**else if** `koşul` **then** ...\n\nİki token, boşlukla yazılır.',
'end if': '**end if** if bloğunu kapatır.',
while: '**while** `koşul` ... `end while`\n\n`do` kullanılmaz.',
'end function': '**end function** fonksiyon bloğunu kapatır.',
```

Hover provider'daki anahtar kelime eşleşme mantığı `firstToken` ile çalışıyorsa,  
iki-token hover'lar için `firstToken + ' ' + secondToken` kombinasyonuna da  
bakacak şekilde güncellenmeli.

---

## Değişiklik 5: `mlpCompiler.ts` — Normalizer pipeline ekle

**Bu değişiklik en kritik olanıdır.**  
Şu an extension .mlp dosyasını doğrudan `melp_compiler_bin`'e gönderiyor.  
Bu Stage1 mimarisini ihlal ediyor.

`compileMlpProgram` fonksiyonu şu iki adımla güncellenecek:

```typescript
// 1. Normalize: .mlp → PMLP temp dosyası
// 2. Compile: PMLP → LLVM IR

async function compileMlpProgram(options: CompileMlpProgramOptions): Promise<string> {
  const normalizerBin = options.normalizerPath;  // yeni alan ekle
  const tmpPmlp = path.join(os.tmpdir(), `melp_vsix_${Date.now()}.pmlp`);

  // Dil tespiti (dosya uzantısından)
  const lang = options.sourcePath.endsWith('.tr.mlp') ? 'turkish'
             : options.sourcePath.endsWith('.ru.mlp') ? 'russian'
             : 'english';

  // Stage1/Stage0 boundary: normalizer çalıştır
  fs.writeFileSync('/tmp/.melp_norm_src', fs.readFileSync(options.sourcePath));
  fs.writeFileSync('/tmp/.melp_norm_lang', lang);
  const normOutput = spawnSync(normalizerBin, [], { encoding: 'utf-8' });
  fs.writeFileSync(tmpPmlp, normOutput.stdout);

  try {
    // PMLP'yi compiler'a gönder (asla .mlp değil)
    const compileArgs = [tmpPmlp, '-o', options.outputPath, ...compilerArgs];
    return await runProcess(options.compilerPath, compileArgs, workDir, true);
  } finally {
    fs.unlinkSync(tmpPmlp);
  }
}
```

`CompileMlpProgramOptions` interface'ine `normalizerPath: string` alanı eklenmeli.  
`extension.ts`'de normalizer binary yolu `bin/melp_normalizer_bin` olarak  
konfigüre edilmeli (compiler binary'nin yanında).

---

## Değişiklik 6: `mlpCompletionProvider.ts` — `snippets/mlp.json`

`snippets/mlp.json` dosyasındaki tüm snippet'lerde de `end_*` → `end *`  
dönüşümü yapılmalı. Örnek:

```json
// ÖNCE:
"body": ["function main()", "\t$0", "end_function"]

// SONRA:
"body": ["function main()", "\t$0", "end function"]
```

---

## Test Kriterleri

Değişiklikler sonrası şu dosya **hata/uyarı vermemeli:**

```mlp
function main()
    for i = 1 to 10
        if i mod 2 == 0 then
            print(i)
        else if i mod 3 == 0 then
            print("fizz")
        else
            print("other")
        end if
    end for
end function
```

Ve şu dosya **W5 uyarısı vermeli:**

```mlp
function main()
    if true then
        print("ok")
    end_if        -- uyarı: 'end_if' → 'end if' kullanın
end_function      -- uyarı: 'end_function' → 'end function' kullanın
```

---

## Öncelik Sırası

1. `mlpDiagnosticProvider.ts` — E3 kaldır, BLOCK_PAIRS güncelle, PMLP legacy ekle (**en acil**)
2. `mlpCompletionProvider.ts` — snippet'ler
3. `mlpCodeActionProvider.ts` — quick-fix'ler
4. `mlpHoverProvider.ts` — açıklamalar
5. `mlpCompiler.ts` — normalizer pipeline (**mimari zorunluluk**)
6. `snippets/mlp.json` — snippet dosyası

---

## Notlar

- `end_program`, `end_func` gibi legacy alias'lar LEGACY_KEYWORDS'de kalabilir  
  ama öneri `end function` olmalı, `end_function` değil.
- W5 mesajlarındaki "PMPL karşılığı" ifadesi → "MLP karşılığı" olarak güncellenmeli.
- VSIX `package.json` sürümü bu değişiklikten sonra minor bump yapılmalı (0.4.13 → 0.5.0).
- Normalizer binary `bin/melp_normalizer_bin` olarak extension'ın `bin/` dizinine  
  kopyalanmalı; kaynağı: `/home/pardus/PROJELER/MELP/LLVM/STAGE1/src/normalizer/melp_normalizer_bin`
