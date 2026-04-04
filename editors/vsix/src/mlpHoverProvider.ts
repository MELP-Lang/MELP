import * as vscode from 'vscode';
import * as fs from 'fs';
import * as path from 'path';

interface MlpFunction {
  name: string;
  signature: string;
  description: string;
  params: Array<{ name: string; type: string; description: string }>;
  returns?: { type: string; description: string };
  example?: string;
}

interface MlpFunctionsData {
  categories: Array<{ functions: MlpFunction[] }>;
}

const KEYWORD_DOCS: Record<string, string> = {
  if: '**if** `koşul` **then** ... `end if`\n\n`then` her zaman zorunludur.',
  'else if': '**else if** `koşul` **then** ...\n\nİki token, boşlukla yazılır.',
  'end if': '**end if** if bloğunu kapatır.',
  while: '⚠️ `while` eski sözdizimi — **`loop`** kullanın.\n\n```mlp\nloop koşul\n    ...\nend loop\n```',
  loop: '**loop** `koşul` ... `end loop`\n\nMLP kanonik döngü formu.\n\n```mlp\nloop x > 0\n    x = x - 1\nend loop\n```',
  'end loop': '**end loop** — loop bloğunu kapatır.',
  for: '**for** `i = başlangıç to bitiş` veya `downto` ile yazılır.',
  each: '**each** `eleman` **in** `koleksiyon` — `loop` ile kullanılır.\n\n```mlp\nloop each item in liste\n    print(item)\nend loop\n```',
  where: '**where** `koşul` — filtre ifadesi. Döngü veya `for` ile kullanılır.\n\n```mlp\nloop each x in liste where x > 0\n    print(x)\nend loop\n```',
  function: '**`tip` function** `ad(params)` — dönüş tipi fonksiyon adından önce yazılır.\n\n```mlp\nnumeric function topla(numeric a; numeric b)\n    return a + b\nend function\n```',
  'end function': '**end function** fonksiyon bloğunu kapatır.',
  own: '**own** — değer sahipliği transferi (move semantics).',
  mut: '**mut** — değiştirilebilir parametre veya değişken.',
  pure: '**pure** — yan etkisiz fonksiyon bildirimi.',
  inline: '**inline** — derleyiciye fonksiyonu satır içi genişletmesini öner.',
  comptime: '**comptime** — derleme zamanı hesaplama.',
  struct: '**struct** veri taşıyıcı tip tanımlar.',
  'end struct': '**end struct** struct bloğunu kapatır.',
  enum: '**enum** numaralandırma tanımlar.',
  'end enum': '**end enum** enum bloğunu kapatır.',
  debug: '**debug** bloğu debug çalıştırmada etkindir.',
  pause: '**pause** yalnızca `debug ... end debug` içinde anlamlıdır.',
  'end debug': '**end debug** debug bloğunu kapatır.',
  numeric: '**numeric** tüm sayısal değerler için kullanılan tiptir.',
  string: '**string** metin tipidir.',
  boolean: '**boolean** mantıksal tiptir.',
  list: '**list** heterojen koleksiyon tipidir.',
  print: '**print(value)** çıktıyı konsola yazar.',
  println: '**println(value)** çıktıyı konsola yazar. `print` ile özdeştir.',
  range: '**range(start; end)** sayı aralığı üretir.',
  scope: '**scope** `<isim>` ... `end scope`\n\nİsimli kontrol akışı bloğu. `exit_<isim>` ile erken çıkış sağlanır.\n\n```mlp\nscope dogrulama\n    if x < 0 then\n        exit_dogrulama\n    end if\nend scope\n```',
  'end scope': '**end scope** — scope bloğunu kapatır.',
  test: '**test** `"açıklama"` **do** ... `end test`\n\nBirim test bloğu. `assert()` ile doğrulama yapılır.\n\n```mlp\ntest "iki kere iki dört" do\n    assert(2 * 2 == 4)\nend test\n```',
  'end test': '**end test** — test bloğunu kapatır.',
  spawn: '**spawn** ... `end spawn`\n\nYeni thread başlatır. Handle değişkenine atanabilir.\n\n```mlp\nnumeric h = spawn\n    print("thread çalışıyor")\nend spawn\nmlp_spawn_join(h)\n```',
  'end spawn': '**end spawn** — spawn bloğunu kapatır.',
  external: '**external function** `ad(tip param)`\n\nC/sistem kütüphane fonksiyonu bildirimi. Bağlayıcı çözer.\n\n```mlp\nexternal function strlen(string s)\n```',
  async: '**async function** — async fonksiyon tanımı (pass-through). Derleyici `await` ile birlikte işler.',
  await: '**await** `ifade` — async fonksiyon çağrısının tamamlanmasını bekler.',
  method: '**method** `StructAdı.ad(params)`\n\n`function` ile tamamen eşdeğer; struct method tanımı için alternatif keyword.\n\n```mlp\nnumeric method Sayac.artir()\n    this.deger = this.deger + 1\n    return this.deger\nend_artir\n```',
};

function loadFunctions(extensionPath: string): Map<string, MlpFunction> {
  const catalogPath = path.join(extensionPath, 'data', 'mlp_functions.json');
  const functions = new Map<string, MlpFunction>();
  if (!fs.existsSync(catalogPath)) {
    return functions;
  }

  try {
    const raw = JSON.parse(fs.readFileSync(catalogPath, 'utf-8')) as MlpFunctionsData;
    for (const category of raw.categories) {
      for (const fn of category.functions) {
        functions.set(fn.name, fn);
      }
    }
  } catch {
    return functions;
  }

  return functions;
}

export class MlpHoverProvider implements vscode.HoverProvider {
  private readonly functions: Map<string, MlpFunction>;

  constructor(context: vscode.ExtensionContext) {
    this.functions = loadFunctions(context.extensionPath);
  }

  provideHover(
    document: vscode.TextDocument,
    position: vscode.Position,
    _token: vscode.CancellationToken
  ): vscode.Hover | undefined {
    const config = vscode.workspace.getConfiguration('mlp');
    if (!config.get<boolean>('showFunctionSignatures', true)) {
      return undefined;
    }

    const wordRange = document.getWordRangeAtPosition(
      position,
      /[a-zA-ZçÇğĞıİöÖşŞüÜ_][a-zA-ZçÇğĞıİöÖşŞüÜ0-9_]*/
    );
    if (!wordRange) {
      return undefined;
    }

    const word = document.getText(wordRange);
    // Hem 'function name(...)' hem 'type function name(...)' sözdizimini destekle
    const fnRegex = /^\s*(?:(numeric|string|boolean|none|list)\s+)?function\s+([a-zA-ZçğıöşüÇĞİÖŞÜ_][a-zA-Z0-9çğıöşüÇĞİÖŞÜ_.]*)\s*\(([^)]*)\)/i;
    const lines = document.getText().split('\n');
    for (let index = 0; index < lines.length; index++) {
      const match = lines[index].match(fnRegex);
      if (!match || match[2] !== word) {
        continue;
      }

      const returnType = match[1] ?? 'numeric';
      const signature = `${returnType} function ${word}(${match[3].trim()})`;
      const comment = lines[index + 1]?.trim().startsWith('--')
        ? lines[index + 1]!.trim().replace(/^--\s*/, '')
        : '';
      const md = new vscode.MarkdownString();
      md.appendCodeblock(signature, 'mlp');
      if (comment) {
        md.appendMarkdown(`\n\n${comment}`);
      }
      md.appendMarkdown(`\n\n*Bu dosyada, satır ${index + 1}*`);
      return new vscode.Hover(md, wordRange);
    }

    const fn = this.functions.get(word);
    if (fn) {
      const md = new vscode.MarkdownString();
      md.appendCodeblock(fn.signature, 'mlp');
      md.appendMarkdown(`\n\n${fn.description}`);
      if (fn.params.length > 0) {
        md.appendMarkdown('\n\n**Parametreler**\n');
        for (const param of fn.params) {
          md.appendMarkdown(`- \`${param.name}\` (${param.type}): ${param.description}\n`);
        }
      }
      if (fn.returns) {
        md.appendMarkdown(`\n**Döndürür:** \`${fn.returns.type}\` - ${fn.returns.description}`);
      }
      if (fn.example) {
        md.appendMarkdown('\n\n**Örnek**\n');
        md.appendCodeblock(fn.example, 'mlp');
      }
      return new vscode.Hover(md, wordRange);
    }

    const lineText = document.lineAt(position.line).text;
    const tokenMatches = [...lineText.matchAll(/[a-zA-ZçÇğĞıİöÖşŞüÜ_][a-zA-ZçÇğĞıİöÖşŞüÜ0-9_]*/g)];
    const tokenIndex = tokenMatches.findIndex(match => {
      const start = match.index ?? 0;
      const end = start + match[0].length;
      return position.character >= start && position.character <= end;
    });

    const lowerWord = word.toLowerCase();
    const nextToken = tokenIndex >= 0 && tokenIndex + 1 < tokenMatches.length
      ? tokenMatches[tokenIndex + 1][0].toLowerCase()
      : '';
    const prevToken = tokenIndex > 0
      ? tokenMatches[tokenIndex - 1][0].toLowerCase()
      : '';

    const keywordCandidates = [
      lowerWord,
      `${lowerWord} ${nextToken}`.trim(),
      `${prevToken} ${lowerWord}`.trim(),
    ];
    const keyword = keywordCandidates.find(candidate => KEYWORD_DOCS[candidate]);

    if (keyword) {
      return new vscode.Hover(new vscode.MarkdownString(KEYWORD_DOCS[keyword]), wordRange);
    }

    return undefined;
  }
}