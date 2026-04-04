import * as vscode from 'vscode';
import * as fs from 'fs';
import * as path from 'path';

// ─── Yardımcı fonksiyonlar ────────────────────────────────────────────────────

interface Token {
  text: string;
  col: number;
}

function tokenizeLine(line: string): Token[] {
  const tokens: Token[] = [];
  let i = 0;
  const len = line.length;

  while (i < len) {
    const ch = line[i];
    if (/\s/.test(ch)) { i++; continue; }
    if (ch === '-' && line[i + 1] === '-') { break; }
    if (ch === '"') {
      i++;
      while (i < len && line[i] !== '"') {
        if (line[i] === '\\') { i++; }
        i++;
      }
      i++;
      continue;
    }
    if (/[\p{L}_]/u.test(ch)) {
      const start = i;
      while (i < len && /[\p{L}\p{N}_]/u.test(line[i])) { i++; }
      tokens.push({ text: line.substring(start, i), col: start });
      continue;
    }
    tokens.push({ text: ch, col: i });
    i++;
  }

  return tokens;
}

function escapeRegex(s: string): string {
  return s.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
}

/** String literallerini boşlukla maskeleyerek parantez/operatör kontrolü için temizler */
function stripStrings(line: string): string {
  let result = '';
  let inStr = false;
  for (let i = 0; i < line.length; i++) {
    const ch = line[i];
    if (ch === '\\' && inStr) { result += '  '; i++; continue; }
    if (ch === '"') { inStr = !inStr; result += ' '; continue; }
    if (!inStr && ch === '-' && line[i + 1] === '-') {
      result += ' '.repeat(line.length - i);
      break;
    }
    result += inStr ? ' ' : ch;
  }
  return result;
}

// ─── Sabit tablolar ───────────────────────────────────────────────────────────

/** `numeric function greet()` gibi tip-önce sözdiziminde kabul edilen tipler */
const TYPE_PREFIXES = new Set(['numeric', 'text', 'boolean', 'none', 'list', 'any']);

const BLOCK_PAIRS: Record<string, string> = {
  if: 'end if',
  loop: 'end loop',
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

/** Eski/Türkçe/PMLP anahtar kelimeler → önerilen MLP karşılığı */
const LEGACY_KEYWORDS: Record<string, string> = {
  'YAZDIR':      'print()',
  'DOĞRU':       'true',
  'YANLIŞ':      'false',
  'VE':          'and',
  'VEYA':        'or',
  'DEĞİL':       'not',
  'GERİ_DÖN':    'return',
  'PROGRAM':     'function main()',
  'program':     'function main()',
  'end_program': 'end function',
  'SON':         'end_<blok>',
  'FONK':        'function',
  'end_func':    'end function',
  'func':        'function',
  'logical':     'boolean',
  'void':        'none',
  'end_function':'end function',
  'end_if':      'end if',
  'while':       'loop',
  'end_while':   'end loop',
  'end_for':     'end for',
  'end_match':   'end match',
  'end_struct':  'end struct',
  'end_enum':    'end enum',
  'end_interface':'end interface',
  'end_module':  'end module',
  'end_try':     'end try',
  'else_if':     'else if',
};

const LEGACY_CLOSERS: Record<string, string> = {
  end_if: 'end if',
  end_loop: 'end loop',
  end_while: 'end loop',
  end_for: 'end for',
  end_function: 'end function',
  end_struct: 'end struct',
  end_enum: 'end enum',
  end_interface: 'end interface',
  end_implement: 'end implement',
  end_try: 'end try',
  end_match: 'end match',
  end_module: 'end module',
  end_lambda: 'end lambda',
  end_event: 'end event',
  end_debug: 'end debug',
  end_scope: 'end scope',
  end_spawn: 'end spawn',
  end_test: 'end test',
};

/** Tüm bilinen MLP kanonik İngilizce anahtar kelimeleri (W8 kontrolü için). */
const KNOWN_EN_KEYWORDS = new Set<string>([
  // Blok açıcılar
  'if', 'loop', 'for', 'function', 'struct', 'enum', 'interface',
  'implement', 'try', 'match', 'module', 'lambda', 'event',
  'debug', 'scope', 'spawn', 'test',
  // Blok kapayıcıların ilk token'ı ('end if' → 'end')
  'end',
  // Tip sistemi
  'numeric', 'string', 'text', 'boolean', 'none', 'list', 'any',
  'result', 'option', 'future',
  // Kontrol akışı
  'else', 'then', 'return', 'exit', 'continue',
  'case', 'catch', 'finally', 'throw',
  // Değer sabitleri
  'true', 'false', 'null',
  // Operatörler
  'and', 'or', 'not',
  // Yerleşik fonksiyonlar
  'print', 'println', 'assert',
  // Diğer keyword'ler
  'typeof', 'range', 'mod',
  'goto', 'pause',
  'const', 'comptime', 'pure', 'inline',
  'own', 'mut', 'external', 'async', 'await',
  'implements', 'this', 'self',
  'import', 'export',
  'from', 'in', 'each', 'where',
  'to', 'downto', 'step',
  'method', 'operator',
  'main', 'package', 'switch',
]);

// ─── Dil algılama yardımcıları ──────────────────────────────────────────────

/** Dosya adındaki dil kodunu algılar (örn. kitchen_sink.ar.mlp → 'ar'). */
function detectLangFromUri(uri: vscode.Uri): string {
  const basename = path.basename(uri.fsPath);
  const m = basename.match(/\.([a-z]{2})\.mlp$/i);
  if (m) {
    const code = m[1].toLowerCase();
    if (['ar', 'tr', 'ru', 'zh', 'en'].includes(code)) { return code; }
  }
  return vscode.workspace.getConfiguration('mlp').get<string>('language', 'en');
}

/** language/<lang>/keywords.json'dan yerel→İngilizce ters harita oluşturur. */
function loadReverseKeywordMap(lang: string, extensionPath: string): Record<string, string> {
  if (lang === 'en' || !extensionPath) { return {}; }
  const perLangPath = path.join(extensionPath, 'language', lang, 'keywords.json');
  if (!fs.existsSync(perLangPath)) { return {}; }
  try {
    const raw = JSON.parse(fs.readFileSync(perLangPath, 'utf-8'));
    if (!raw.keywords) { return {}; }
    const map: Record<string, string> = {};
    for (const [en, local] of Object.entries(raw.keywords as Record<string, string>)) {
      map[(local as string).toLowerCase()] = en.toLowerCase();
    }
    return map;
  } catch {
    return {};
  }
}

// ─── Provider ─────────────────────────────────────────────────────────────────

export class MlpDiagnosticProvider {
  readonly collection = vscode.languages.createDiagnosticCollection('mlp');
  private readonly extensionPath: string;

  constructor(extensionPath = '') {
    this.extensionPath = extensionPath;
  }

  analyze(document: vscode.TextDocument): void {
    if (document.languageId !== 'mlp') {
      return;
    }

    const diagnostics: vscode.Diagnostic[] = [];
    const blockStack: Array<{ token: string; origLen: number; line: number; col: number }> = [];
    let debugDepth = 0;
    const definedLabels = new Set<string>();
    const gotoRefs: Array<{ label: string; range: vscode.Range }> = [];

    // Dil algıla ve yerel→İngilizce anahtar kelime ters haritasını yükle
    const docLang = detectLangFromUri(document.uri);
    const reverseMap = loadReverseKeywordMap(docLang, this.extensionPath);
    /** Yerel keyword'ü kanonik İngilizce karşılığına çevirir. Bilinmeyeni aynen döndürür. */
    const toEn = (tok: string): string => reverseMap[tok] ?? tok;

    // ── W8 ön taraması: belgede tanımlı fonksiyon/struct/enum isimleri ────
    // Belge-yerel tanımlı isimler W8'den muaf tutulur (kullanıcı tanımlı çağrılar).
    const definedFnNames = new Set<string>();
    for (let li = 0; li < document.lineCount; li++) {
      const rawL = document.lineAt(li).text;
      const ci = rawL.indexOf('--');
      const l = ci >= 0 ? rawL.substring(0, ci) : rawL;
      const toks = tokenizeLine(l);
      if (toks.length < 2) { continue; }
      const cf0 = toEn(toks[0].text.toLowerCase());
      const cf1 = toEn(toks[1].text.toLowerCase());
      if (cf0 === 'function' || cf0 === 'method') {
        definedFnNames.add(toks[1].text.toLowerCase());
      } else if (TYPE_PREFIXES.has(cf0) && cf1 === 'function' && toks.length >= 3) {
        definedFnNames.add(toks[2].text.toLowerCase());
      } else if (['struct', 'enum', 'interface', 'module', 'scope', 'spawn', 'test'].includes(cf0)) {
        definedFnNames.add(toks[1].text.toLowerCase());
      }
    }

    for (let lineIndex = 0; lineIndex < document.lineCount; lineIndex++) {
      const rawLine = document.lineAt(lineIndex).text;
      const commentIdx = rawLine.indexOf('--');
      const line = commentIdx >= 0 ? rawLine.substring(0, commentIdx) : rawLine;
      const trimmed = line.trim();
      if (!trimmed) { continue; }

      // ── W5: Eski/Türkçe keyword ──────────────────────────────────────
      for (const [legacy, modern] of Object.entries(LEGACY_KEYWORDS)) {
        if (legacy.toLowerCase() === 'else_if') {
          continue;
        }
        const re = new RegExp(`\\b${escapeRegex(legacy)}\\b`);
        const m = line.match(re);
        if (m && m.index !== undefined) {
          const d = new vscode.Diagnostic(
            new vscode.Range(lineIndex, m.index, lineIndex, m.index + legacy.length),
            `Eski sözdizimi: '${legacy}' — MLP karşılığı: '${modern}'`,
            vscode.DiagnosticSeverity.Warning
          );
          d.code = 'W5';
          d.source = 'MLP';
          diagnostics.push(d);
        }
      }

      // ── W5: else_if alt-çizgili (legacy) ───────────────────────────────
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

      // ── W1: break → exit ──────────────────────────────────────────────
      {
        const m = line.match(/\bbreak\b/i);
        if (m && m.index !== undefined) {
          const d = new vscode.Diagnostic(
            new vscode.Range(lineIndex, m.index, lineIndex, m.index + 5),
            `'break' yerine 'exit' kullanın.`,
            vscode.DiagnosticSeverity.Warning
          );
          d.code = 'W1';
          d.source = 'MLP';
          diagnostics.push(d);
        }
      }

      // ── W6: C-stili operatörler ───────────────────────────────────────
      {
        const stripped = stripStrings(line);
        const cOps: Array<[RegExp, string, string]> = [
          [/&&/g,   '&&', 'and'],
          [/\|\|/g, '||', 'or'],
          [/!/g,    '!',  'not'],
        ];
        for (const [re, op, replacement] of cOps) {
          re.lastIndex = 0;
          let m2: RegExpExecArray | null;
          while ((m2 = re.exec(stripped)) !== null) {
            const d = new vscode.Diagnostic(
              new vscode.Range(lineIndex, m2.index, lineIndex, m2.index + op.length),
              `C-stili '${op}' geçersiz — '${replacement}' kullanın.`,
              vscode.DiagnosticSeverity.Warning
            );
            d.code = 'W6';
            d.source = 'MLP';
            diagnostics.push(d);
          }
        }
      }

      // ── E6: Eşleşmemiş parantez ───────────────────────────────────────
      {
        const stripped = stripStrings(line);
        let depth = 0;
        let firstOpenCol = -1;
        for (let ci = 0; ci < stripped.length; ci++) {
          if (stripped[ci] === '(') {
            if (depth === 0) { firstOpenCol = ci; }
            depth++;
          } else if (stripped[ci] === ')') {
            depth--;
            if (depth < 0) {
              const d = new vscode.Diagnostic(
                new vscode.Range(lineIndex, ci, lineIndex, ci + 1),
                `Eşleşmemiş ')' — açılan '(' bulunamadı.`,
                vscode.DiagnosticSeverity.Error
              );
              d.code = 'E6';
              d.source = 'MLP';
              diagnostics.push(d);
              depth = 0;
            }
          }
        }
        if (depth > 0 && firstOpenCol >= 0) {
          const endCol = line.trimEnd().length;
          const d = new vscode.Diagnostic(
            new vscode.Range(lineIndex, endCol - 1, lineIndex, endCol),
            `Kapatılmamış '(' — ')' eksik.`,
            vscode.DiagnosticSeverity.Error
          );
          d.code = 'E6';
          d.source = 'MLP';
          diagnostics.push(d);
        }
      }

      // ── E7: Kapatılmamış string ───────────────────────────────────────
      {
        let inStr = false;
        let strStart = -1;
        for (let ci = 0; ci < line.length; ci++) {
          const ch = line[ci];
          if (ch === '\\' && inStr) { ci++; continue; }
          if (ch === '"') {
            if (!inStr) { inStr = true; strStart = ci; }
            else { inStr = false; strStart = -1; }
          }
        }
        if (inStr && strStart >= 0) {
          const d = new vscode.Diagnostic(
            new vscode.Range(lineIndex, strStart, lineIndex, line.length),
            `Kapatılmamış string — satır sonunda '"' bulunamadı.`,
            vscode.DiagnosticSeverity.Error
          );
          d.code = 'E7';
          d.source = 'MLP';
          diagnostics.push(d);
        }
      }

      // ── Token analizi ─────────────────────────────────────────────────
      const tokens = tokenizeLine(line);
      if (tokens.length === 0) { continue; }

      const firstToken = tokens[0].text.toLowerCase();
      const secondToken = tokens.length > 1 ? tokens[1].text.toLowerCase() : '';
      // Kanonik (İngilizce) formlar — dil-bağımsız mantık için
      const canonFirst = toEn(firstToken);
      const canonSecond = toEn(secondToken);
      const twoTokenKey = `${canonFirst} ${canonSecond}`.trim();
      const normalizedSingleCloser = LEGACY_CLOSERS[canonFirst] ?? canonFirst;
      const firstCol = tokens[0].col;

      // label tanımı — Unicode harflerini destekler
      const labelM = trimmed.match(/^([\p{L}_][\p{L}\p{N}_]*):/u);
      if (labelM) { definedLabels.add(labelM[1].toLowerCase()); }

      // goto referansı
      if (canonFirst === 'goto' && tokens.length >= 2) {
        const lbl = tokens[1];
        gotoRefs.push({
          label: lbl.text.toLowerCase(),
          range: new vscode.Range(lineIndex, lbl.col, lineIndex, lbl.col + lbl.text.length),
        });
      }

      // debug blok
      if (canonFirst === 'debug') {
        // Label kontrolü: 'debug:' veya yerel karşılığı ':' ile bitiyorsa etikettir
        const afterFirst = line.substring(firstCol + firstToken.length).trimStart();
        if (!afterFirst.startsWith(':')) {
          debugDepth++;
          blockStack.push({ token: 'debug', origLen: firstToken.length, line: lineIndex, col: firstCol });
        }
        continue;
      }

      // E9: struct/enum/interface bloğu içinde function tanımı yasak
      const isTypePrefixedFn = TYPE_PREFIXES.has(canonFirst) && tokens.length >= 2 && toEn(tokens[1].text.toLowerCase()) === 'function';
      if (canonFirst === 'function' || canonFirst === 'method' || isTypePrefixedFn) {
        const enclosingBlock = blockStack.length > 0 ? blockStack[blockStack.length - 1].token : null;
        if (enclosingBlock === 'struct' || enclosingBlock === 'enum' || enclosingBlock === 'interface') {
          const d = new vscode.Diagnostic(
            new vscode.Range(lineIndex, firstCol, lineIndex, firstCol + firstToken.length),
            `'${enclosingBlock}' bloğu içinde 'function' tanımlanamaz — method'ları struct dışında 'function ${enclosingBlock === 'struct' ? 'StructAdi' : 'Tip'}.metod() as tip' şeklinde tanımlayın.`,
            vscode.DiagnosticSeverity.Error
          );
          d.code = 'E9';
          d.source = 'MLP';
          diagnostics.push(d);
        }
      }

      // Blok açıcı
      if (canonFirst in BLOCK_PAIRS && canonFirst !== 'debug') {
        blockStack.push({ token: canonFirst, origLen: firstToken.length, line: lineIndex, col: firstCol });

        // E8: scope/spawn isim zorunlu
        if ((canonFirst === 'scope' || canonFirst === 'spawn') && tokens.length < 2) {
          const d = new vscode.Diagnostic(
            new vscode.Range(lineIndex, firstCol, lineIndex, firstCol + firstToken.length),
            `'${firstToken}' bloğu bir isim gerektirir — '${firstToken} <isim>' şeklinde yazın.`,
            vscode.DiagnosticSeverity.Error
          );
          d.code = 'E8';
          d.source = 'MLP';
          diagnostics.push(d);
        }
      }
      // Tip-önce sözdizimi: numeric function greet() → function bloğu aç
      if (isTypePrefixedFn) {
        blockStack.push({ token: 'function', origLen: firstToken.length, line: lineIndex, col: firstCol });
      }

      // Blok kapayıcı
      const closerEntry =
        Object.entries(BLOCK_PAIRS).find(([, closer]) => closer === twoTokenKey) ??
        Object.entries(BLOCK_PAIRS).find(([, closer]) => closer === normalizedSingleCloser);
      if (closerEntry) {
        const [openerToken, closerToken] = closerEntry;
        // Belgedeki gerçek kapanış metninin uzunluğu (tek-token Arapça vs iki-token İngilizce)
        const docCloserLen = (closerToken === twoTokenKey && canonSecond)
          ? tokens[0].text.length + 1 + tokens[1].text.length
          : tokens[0].text.length;
        if (openerToken === 'debug') { debugDepth = Math.max(0, debugDepth - 1); }

        if (blockStack.length === 0) {
          const d = new vscode.Diagnostic(
            new vscode.Range(lineIndex, firstCol, lineIndex, firstCol + docCloserLen),
            `Açılmamış '${openerToken}' bloğu için kapanış: '${closerToken}'.`,
            vscode.DiagnosticSeverity.Error
          );
          d.code = 'E4';
          d.source = 'MLP';
          diagnostics.push(d);
        } else {
          const top = blockStack[blockStack.length - 1];
          if (top.token !== openerToken) {
            const d = new vscode.Diagnostic(
              new vscode.Range(lineIndex, firstCol, lineIndex, firstCol + docCloserLen),
              `Yanlış kapanış: '${closerToken}' — beklenen '${BLOCK_PAIRS[top.token]}' (açılış: satır ${top.line + 1}).`,
              vscode.DiagnosticSeverity.Error
            );
            d.code = 'E4';
            d.source = 'MLP';
            diagnostics.push(d);
          } else {
            blockStack.pop();
          }
        }
        continue;
      }

      // İsimli kapanış: end_<isim> — function bloğu için geçerli alternatif kapanış
      // Örn: numeric function greet() ... end_greet  veya  function greet() as numeric ... end_greet
      if (/^end_[a-zA-Z_][a-zA-Z0-9_]*$/.test(firstToken)) {
        if (blockStack.length > 0 && blockStack[blockStack.length - 1].token === 'function') {
          blockStack.pop();
          continue;
        } else if (blockStack.length === 0) {
          const d = new vscode.Diagnostic(
            new vscode.Range(lineIndex, firstCol, lineIndex, firstCol + firstToken.length),
            `Açılmamış 'function' bloğu için kapanış: '${firstToken}'.`,
            vscode.DiagnosticSeverity.Error
          );
          d.code = 'E4';
          d.source = 'MLP';
          diagnostics.push(d);
          continue;
        }
      }


      // E1: if/else if → then zorunlu
      if (canonFirst === 'if' || canonFirst === 'else if' || (canonFirst === 'else' && canonSecond === 'if')) {
        if (!tokens.some(t => toEn(t.text.toLowerCase()) === 'then')) {
          const d = new vscode.Diagnostic(
            new vscode.Range(lineIndex, firstCol, lineIndex, firstCol + firstToken.length),
            `'${firstToken}' ifadesinde 'then' zorunludur.`,
            vscode.DiagnosticSeverity.Error
          );
          d.code = 'E1';
          d.source = 'MLP';
          diagnostics.push(d);
        }
      }

      // W3: pause debug dışında
      if (canonFirst === 'pause' && debugDepth === 0) {
        const d = new vscode.Diagnostic(
          new vscode.Range(lineIndex, firstCol, lineIndex, firstCol + firstToken.length),
          `'pause' yalnızca 'debug / end debug' bloğu içinde kullanılabilir.`,
          vscode.DiagnosticSeverity.Warning
        );
        d.code = 'W3';
        d.source = 'MLP';
        diagnostics.push(d);
      }

      // W7: function main() as <tip> — 'as' artık geçersiz sözdizimi (dönüş tipi önde yazılır)
      if (canonFirst === 'function' || isTypePrefixedFn) {
        const asIdx = tokens.findIndex(t => toEn(t.text.toLowerCase()) === 'as');
        if (asIdx >= 0 && asIdx + 1 < tokens.length) {
          const asToken = tokens[asIdx];
          const typeToken = tokens[asIdx + 1];
          const d = new vscode.Diagnostic(
            new vscode.Range(lineIndex, asToken.col, lineIndex, typeToken.col + typeToken.text.length),
            `'as ${typeToken.text}' eski sözdizimi — dönüş tipini fonksiyon adından önce yazın: '${typeToken.text} function ...'`,
            vscode.DiagnosticSeverity.Warning
          );
          d.code = 'W7';
          d.source = 'MLP';
          diagnostics.push(d);
        }
      }

      // ── W8: Bilinmeyen keyword ─────────────────────────────────────────
      // Satırın ilk token'ı bir MLP keyword'ü gibi görünüyorsa ama hiçbir
      // bilinen sete girmeyen bir token ise uyarı üretir.
      // Muafiyetler:
      //   * Bilinen kanonik keyword  (KNOWN_EN_KEYWORDS)
      //   * Atama   : ikinci token '='
      //   * Member  : ikinci token '.'
      //   * Etiket  : ikinci token ':'
      //   * Latin ASCII identifier : İngilizce kullanıcı tanımlayıcıları
      //   * Belge-yerel tanımlı isim + fonksiyon çağrısı (ikinci token '(')
      {
        // Yalnızca [\p{L}_] ile başlayan sözcük token'larını kontrol et
        const isWordToken = /^[\p{L}_][\p{L}\p{N}_]*$/u.test(firstToken);
        if (isWordToken) {
          const isKnown      = KNOWN_EN_KEYWORDS.has(canonFirst);
          const isAssign     = tokens.length >= 2 && tokens[1].text === '=';
          const isMember     = tokens.length >= 2 && tokens[1].text === '.';
          const isLabelDef   = tokens.length >= 2 && tokens[1].text === ':';
          const isLatinIdent = /^[a-zA-Z_][a-zA-Z0-9_.]*$/.test(firstToken);
          const isFnCall     = tokens.length >= 2 && tokens[1].text === '(';
          const isLocalDef   = definedFnNames.has(firstToken);

          if (!isKnown && !isAssign && !isMember && !isLabelDef && !isLatinIdent && !(isFnCall && isLocalDef)) {
            const d = new vscode.Diagnostic(
              new vscode.Range(lineIndex, firstCol, lineIndex, firstCol + firstToken.length),
              `'${tokens[0].text}' tanımsız keyword — yazım hatası veya geçersiz sözdizimi.`,
              vscode.DiagnosticSeverity.Warning
            );
            d.code = 'W8';
            d.source = 'MLP';
            diagnostics.push(d);
          }
        }
      }
    }

    // ── E5: Kapanmamış bloklar ────────────────────────────────────────
    for (const unclosed of blockStack) {
      const d = new vscode.Diagnostic(
        new vscode.Range(unclosed.line, unclosed.col, unclosed.line, unclosed.col + unclosed.origLen),
        `'${unclosed.token}' bloğu kapatılmamış — '${BLOCK_PAIRS[unclosed.token]}' eksik.`,
        vscode.DiagnosticSeverity.Error
      );
      d.code = 'E5';
      d.source = 'MLP';
      diagnostics.push(d);
    }

    // ── W4: goto hedef doğrulama ──────────────────────────────────────
    for (const ref of gotoRefs) {
      if (!definedLabels.has(ref.label)) {
        const d = new vscode.Diagnostic(
          ref.range,
          `goto hedefi '${ref.label}' tanımlı değil — 'etiket_adı:' etiketi bulunamadı.`,
          vscode.DiagnosticSeverity.Warning
        );
        d.code = 'W4';
        d.source = 'MLP';
        diagnostics.push(d);
      }
    }

    this.collection.set(document.uri, diagnostics);
  }

  analyzeAll(): void {
    for (const document of vscode.workspace.textDocuments) {
      this.analyze(document);
    }
  }

  dispose(): void {
    this.collection.dispose();
  }
}