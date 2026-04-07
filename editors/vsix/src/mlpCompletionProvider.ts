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
  categories: Array<{
    id: string;
    name: string;
    functions: MlpFunction[];
  }>;
}

const KEYWORDS_EN = [
  'if', 'else if', 'else', 'end if', 'then',
  'loop', 'end loop',
  'for', 'to', 'downto', 'step', 'end for',
  'each', 'in', 'where',
  'exit', 'continue',
  'match', 'case', 'end match',
  'try', 'catch', 'finally', 'end try',
  'function', 'end function', 'return', 'none',
  'lambda', 'end lambda',
  'own', 'mut', 'pure', 'inline', 'comptime',
  'struct', 'end struct',
  'enum', 'end enum',
  'interface', 'end interface',
  'implement', 'end implement', 'implements',
  'this',
  'and', 'or', 'not',
  'true', 'false', 'null',
  'print', 'println', 'typeof', 'range', 'mod',
  'module', 'end module', 'import', 'export',
  'debug', 'end debug', 'pause', 'goto',
  'event', 'end event',
  'scope', 'end scope',
  'test', 'end test',
  'spawn', 'end spawn',
  'external', 'async', 'await', 'method',
];

const TYPES_EN = ['numeric', 'string', 'boolean', 'list', 'none', 'Result', 'Option'];

function loadFunctions(extensionPath: string): MlpFunction[] {
  const catalogPath = path.join(extensionPath, 'data', 'mlp_functions.json');
  if (!fs.existsSync(catalogPath)) return [];
  try {
    const raw = JSON.parse(fs.readFileSync(catalogPath, 'utf-8')) as MlpFunctionsData;
    return raw.categories.flatMap(category => category.functions);
  } catch {
    return [];
  }
}

// lang: 'turkish'|'russian'|'arabic'|'chinese'|'english' (veya 'tr'|'ru'|'ar'|'zh'|'en')
function loadDillerKeywords(extensionPath: string, lang: string): { [en: string]: string } {
  const codeMap: { [k: string]: string } = {
    turkish: 'tr', russian: 'ru', arabic: 'ar', chinese: 'zh', english: 'en',
  };
  const code = codeMap[lang] ?? lang; // 'arabic' → 'ar', 'ar' → 'ar'

  // Önce per-lang dosyasını dene: language/<code>/keywords.json
  const perLangPath = path.join(extensionPath, 'language', code, 'keywords.json');
  if (fs.existsSync(perLangPath)) {
    try {
      const raw = JSON.parse(fs.readFileSync(perLangPath, 'utf-8'));
      if (raw.meta?.enabled !== false && raw.keywords) {
        return raw.keywords as { [en: string]: string };
      }
    } catch { /* fall through */ }
  }

  return {};
}

// lang: 'turkish'|'russian'|'arabic'|'chinese'|'english' (veya 'tr'|'ru'|'ar'|'zh'|'en')
function getActiveLang(document?: vscode.TextDocument): string {
  // Dosya adından dil algıla (örn. kitchen_sink.ar.mlp → 'ar')
  if (document) {
    const basename = path.basename(document.uri.fsPath);
    const m = basename.match(/\.([a-z]{2})\.mlp$/i);
    if (m) {
      const code = m[1].toLowerCase();
      if (['ar', 'tr', 'ru', 'zh'].includes(code)) { return code; }
      if (code === 'en') { return 'en'; }
    }
  }
  return vscode.workspace.getConfiguration('mlp').get<string>('language', 'en');
}

export class MlpCompletionProvider implements vscode.CompletionItemProvider {
  private readonly functions: MlpFunction[];
  private readonly extensionPath: string;
  private readonly lspActive: boolean;

  constructor(context: vscode.ExtensionContext, lspActive = false) {
    this.functions = loadFunctions(context.extensionPath);
    this.extensionPath = context.extensionPath;
    this.lspActive = lspActive;
  }

  provideCompletionItems(
    document: vscode.TextDocument,
    position: vscode.Position,
    _token: vscode.CancellationToken,
    _context: vscode.CompletionContext
  ): vscode.CompletionItem[] {
    const items: vscode.CompletionItem[] = [];
    const lineText = document.lineAt(position).text.substring(0, position.character);
    const lang = getActiveLang(document);

    // LSP aktifse snippet'lar + keyword item'ları döndür
    if (this.lspActive) {
      const trMap = lang === 'tr' ? loadDillerKeywords(this.extensionPath, 'turkish')
                  : lang === 'ru' ? loadDillerKeywords(this.extensionPath, 'russian')
                  : lang === 'ar' ? loadDillerKeywords(this.extensionPath, 'arabic')
                  : lang === 'zh' ? loadDillerKeywords(this.extensionPath, 'chinese')
                  : {};
      const snippets = lang === 'tr' ? this.turkishSnippets(trMap)
                     : this.englishSnippets();

      for (const [label, snippet, detail, filterText] of snippets) {
        const item = new vscode.CompletionItem(label, vscode.CompletionItemKind.Snippet);
        item.insertText = new vscode.SnippetString(snippet);
        item.detail = detail;
        if (filterText) item.filterText = filterText;
        item.sortText = `0-${label}`;
        items.push(item);
      }

      // Türkçe modda keyword'lerin kendisini de ekle — boşluk basıp devam etmek için
      // Örnek: "fonk" yazınca listede "fonksiyon" keyword'ü de görünsün
      if (lang !== 'en') {
        for (const [, tr] of Object.entries(trMap)) {
          const kw = new vscode.CompletionItem(tr, vscode.CompletionItemKind.Keyword);
          kw.detail = 'MLP anahtar kelimesi';
          kw.sortText = `1-${tr}`;
          items.push(kw);
        }
      }

      return items;
    }

    // Dile göre keyword haritası (en → yerel)
    const _langNameMap: { [k: string]: string } = { tr: 'turkish', ru: 'russian', ar: 'arabic', zh: 'chinese' };
    const trMap = lang !== 'en' ? loadDillerKeywords(this.extensionPath, _langNameMap[lang] ?? lang) : {};
    // Ters harita: tr keyword → en keyword (completion için)
    const reverseMap: { [tr: string]: string } = {};
    for (const [en, tr] of Object.entries(trMap)) {
      reverseMap[tr] = en;
    }

    // Aktif dile göre keyword listesi
    const activeKeywords = lang !== 'en'
      ? KEYWORDS_EN.map(k => trMap[k] ?? k)
      : [...KEYWORDS_EN];

    // Aktif dile göre tip listesi
    const activeTypes = lang !== 'en'
      ? TYPES_EN.map(t => trMap[t] ?? t)
      : [...TYPES_EN];

    // Satır başında tip önerisi
    if (/^\s*\w*$/.test(lineText) && lineText.trim().length > 0) {
      const partial = lineText.trim();
      if (activeTypes.some(t => t.startsWith(partial) && t !== partial)) {
        return activeTypes.map((typeName, i) => {
          const item = new vscode.CompletionItem(typeName, vscode.CompletionItemKind.TypeParameter);
          item.detail = lang === 'tr' ? 'MLP veri tipi' : 'MLP type';
          item.sortText = `0-${TYPES_EN[i]}`;
          return item;
        });
      }
    }

    // Dosya-lokal fonksiyonlar
    const langKeyword = lang !== 'en' ? (trMap['function'] ?? 'function') : 'function';
    const fnRegex = new RegExp(
      `^\\s*(?:(${activeTypes.join('|')})\\s+)?(?:${langKeyword}|function)\\s+([\\p{L}_][\\p{L}\\p{N}_.]*)\\s*\\(([^)]*)\\)`,
      'iu'
    );
    const structKeyword = lang !== 'en' ? (trMap['struct'] ?? 'struct') : 'struct';
    const endStructKeyword = lang !== 'en' ? (trMap['end struct'] ?? 'end struct') : 'end struct';
    const structBlockRe = new RegExp(`^\\s*(?:${structKeyword}|struct)(?:\\s|$)`, 'iu');
    const endStructRe = new RegExp(`^\\s*(?:${endStructKeyword}|end struct)(?:\\s|$)`, 'iu');

    let insideStruct = false;
    for (const line of document.getText().split('\n')) {
      if (structBlockRe.test(line)) { insideStruct = true; }
      if (endStructRe.test(line)) { insideStruct = false; continue; }
      if (insideStruct) continue;
      const match = line.match(fnRegex);
      if (!match) continue;
      const returnType = match[1] ?? (lang === 'tr' ? 'sayısal' : 'numeric');
      const name = match[2];
      const params = match[3].trim();
      const item = new vscode.CompletionItem(name, vscode.CompletionItemKind.Function);
      item.detail = `(bu dosya) ${returnType} ${langKeyword} ${name}(${params})`;
      item.sortText = `1-${name}`;
      const paramNames = params
        .split(';')
        .filter(Boolean)
        .map((param, index) => `\${${index + 1}:${param.trim().split(' ').pop() ?? 'p'}}`)
        .join(', ');
      item.insertText = new vscode.SnippetString(`${name}(${paramNames})`);
      items.push(item);
    }

    // Keyword'ler
    for (const keyword of activeKeywords) {
      const item = new vscode.CompletionItem(keyword, vscode.CompletionItemKind.Keyword);
      item.detail = lang === 'tr' ? 'MLP anahtar kelimesi' : 'MLP keyword';
      items.push(item);
    }

    // Tipler
    for (const typeName of activeTypes) {
      const item = new vscode.CompletionItem(typeName, vscode.CompletionItemKind.TypeParameter);
      item.detail = lang === 'tr' ? 'MLP veri tipi' : 'MLP type';
      items.push(item);
    }

    // Builtin fonksiyonlar
    for (const fn of this.functions) {
      const item = new vscode.CompletionItem(fn.name, vscode.CompletionItemKind.Function);
      item.detail = fn.signature;
      item.documentation = new vscode.MarkdownString(
        `**${fn.description}**\n\n${fn.example ? `\`\`\`mlp\n${fn.example}\n\`\`\`` : ''}`
      );
      const paramNames = fn.params.map((param, index) => `\${${index + 1}:${param.name}}`).join(', ');
      item.insertText = new vscode.SnippetString(`${fn.name}(${paramNames})`);
      items.push(item);
    }

    // Snippet'lar — dile göre
    const snippets = lang === 'tr'
      ? this.turkishSnippets(trMap)
      : this.englishSnippets();

    for (const [label, snippet, detail, filterText] of snippets) {
      const item = new vscode.CompletionItem(label, vscode.CompletionItemKind.Snippet);
      item.insertText = new vscode.SnippetString(snippet);
      item.detail = detail;
      if (filterText) item.filterText = filterText;
      items.push(item);
    }

    return items;
  }

  private englishSnippets(): Array<[string, string, string, string?]> {
    return [
      // fn: imleç önce ada gider, Tab ile parametreye, Tab ile gövdeye
      ['fn',    '${1:numeric} function ${2:name}(${3:numeric param})\n\t$0\nend function', 'numeric function name(...) ... end function', 'fn function'],
      ['main',  'function main()\n\t$0\nend function', 'function main() — entry point'],
      ['if',    'if ${1:condition} then\n\t$0\nend if', 'if then end if'],
      ['ife',   'if ${1:condition} then\n\t${2}\nelse\n\t$0\nend if', 'if else end if', 'ife if else'],
      ['lp',    'loop ${1:condition}\n\t$0\nend loop', 'loop end loop', 'lp loop'],
      ['lpi',   'loop ${1:i} = ${2:0} to ${3:10}\n\t$0\nend loop', 'loop ascending range', 'lpi loop'],
      ['lpd',   'loop ${1:i} = ${2:10} downto ${3:0}\n\t$0\nend loop', 'loop descending range', 'lpd loop'],
      ['lpe',   'loop each ${1:item} in ${2:collection}\n\t$0\nend loop', 'loop each in', 'lpe loop each'],
      ['fr',    'for ${1:i} = ${2:0} to ${3:10}\n\t$0\nend for', 'for to end for', 'fr for'],
      ['dbg',   'debug\n\t${0:print("debug")}\nend debug', 'Debug block', 'dbg debug'],
      ['scope', 'scope ${1:name}\n\t$0\nend scope', 'scope ... end scope'],
      ['tst',   'test "${1:description}" do\n\t${0:assert(true)}\nend test', 'test block', 'tst test'],
      ['spn',   'numeric ${1:h} = spawn\n\t$0\nend spawn', 'spawn thread', 'spn spawn'],
      ['ext',   'external function ${1:c_func}(${2:numeric param})', 'external C declaration', 'ext external'],
    ];
  }

  private turkishSnippets(t: { [en: string]: string }): Array<[string, string, string, string?]> {
    const fn   = t['function']     ?? 'fonksiyon';
    const endfn= t['end function'] ?? 'fonksiyon sonu';
    const ifw  = t['if']           ?? 'koşul';
    const then = t['then']         ?? 'ise';
    const endif= t['end if']       ?? 'koşul sonu';
    const els  = t['else']         ?? 'yoksa';
    const lp   = t['loop']         ?? 'döngü';
    const endlp= t['end loop']     ?? 'döngü sonu';
    const fr   = t['for']          ?? 'için';
    const endfr= t['end for']      ?? 'için sonu';
    const to   = t['to']           ?? 'kadar';
    const down = t['downto']       ?? 'geriye';
    const each = t['each']         ?? 'her';
    const ins  = t['in']           ?? 'içinde';
    const num  = t['numeric']      ?? 'sayısal';
    const ret  = t['return']       ?? 'döndür';
    const main = t['main']         ?? 'giriş';

    return [
      // giriş/main — en sık kullanılan, listenin başında
      ['giriş',     `${fn} giriş()\n\t$0\n${endfn}`, `${fn} giriş() — ana giriş noktası`, `giriş ${fn} main`],
      ['main',      `${fn} giriş()\n\t$0\n${endfn}`, `${fn} giriş() — ana giriş noktası`, `main giriş ${fn}`],
      // fonk/fonksiyon — tam şablon (Enter), ya da keyword seçip boşluk basıp devam
      ['fonk',      `\${1:${num}} ${fn} \${2:ad}(\${3:${num} param})\n\t$0\n${endfn}`, `${num} ${fn} ad(...) — tam şablon`, `fonk ${fn}`],
      ['fonksiyon', `\${1:${num}} ${fn} \${2:ad}(\${3:${num} param})\n\t$0\n${endfn}`, `${num} ${fn} ad(...) — tam şablon`, `fonksiyon ${fn}`],
      ['koşul',     `${ifw} \${1:koşul} ${then}\n\t$0\n${endif}`, `${ifw} ... ${then} ... ${endif}`],
      ['koşulse',   `${ifw} \${1:koşul} ${then}\n\t\${2}\n${els}\n\t$0\n${endif}`, `${ifw} ... ${then} ... ${els} ... ${endif}`, `koşulse ${ifw} ${els}`],
      ['döngü',     `${lp} \${1:koşul}\n\t$0\n${endlp}`, `${lp} ... ${endlp}`],
      ['döngüi',    `${lp} \${1:i} = \${2:0} ${to} \${3:10}\n\t$0\n${endlp}`, 'Artan döngü', `döngüi ${lp}`],
      ['döngüd',    `${lp} \${1:i} = \${2:10} ${down} \${3:0}\n\t$0\n${endlp}`, 'Azalan döngü', `döngüd ${lp}`],
      ['döngüh',    `${lp} ${each} \${1:eleman} ${ins} \${2:koleksiyon}\n\t$0\n${endlp}`, 'Her eleman için döngü', `döngüh ${lp} ${each}`],
      ['için',      `${fr} \${1:i} = \${2:0} ${to} \${3:10}\n\t$0\n${endfr}`, `${fr} ... ${to} ... ${endfr}`, `için ${fr}`],
      // İngilizce kısa trigger'lar Türkçe snippet üretir
      ['fn',        `\${1:${num}} ${fn} \${2:ad}(\${3:${num} param})\n\t$0\n${endfn}`, `${num} ${fn} ad(...) — fonksiyon tanımı`, `fn ${fn}`],
      ['if',        `${ifw} \${1:koşul} ${then}\n\t$0\n${endif}`, `${ifw} ... ${then} ... ${endif}`],
      ['lp',        `${lp} \${1:koşul}\n\t$0\n${endlp}`, `${lp} ... ${endlp}`, `lp ${lp}`],
      ['lpi',       `${lp} \${1:i} = \${2:0} ${to} \${3:10}\n\t$0\n${endlp}`, 'Artan döngü', `lpi ${lp}`],
      ['lpd',       `${lp} \${1:i} = \${2:10} ${down} \${3:0}\n\t$0\n${endlp}`, 'Azalan döngü', `lpd ${lp}`],
      ['lpe',       `${lp} ${each} \${1:eleman} ${ins} \${2:koleksiyon}\n\t$0\n${endlp}`, 'Her eleman için döngü', `lpe ${lp} ${each}`],
      ['dbg',       `debug\n\t\${0:yaz("debug")}\nend debug`, 'Debug bloğu', 'dbg debug'],
      ['ext',       `external ${fn} \${1:c_fonk}(\${2:${num} param})`, 'external C bildirimi', 'ext external'],
    ];
  }
}
