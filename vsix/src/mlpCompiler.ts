import * as fs from 'fs';
import * as os from 'os';
import * as path from 'path';
import { spawn, spawnSync } from 'child_process';

export interface CompilerDiagnostic {
  filePath?: string;
  line?: number;
  column?: number;
  severity: 'error' | 'warning';
  message: string;
  raw: string;
}

export class MlpCompilationError extends Error {
  readonly output: string;
  readonly diagnostics: CompilerDiagnostic[];

  constructor(message: string, output: string, diagnostics: CompilerDiagnostic[] = []) {
    super(message);
    this.name = 'MlpCompilationError';
    this.output = output;
    this.diagnostics = diagnostics;
  }
}

export interface CompileMlpProgramOptions {
  compilerPath: string;
  normalizerPath: string;
  sourcePath: string;
  outputPath: string;
  runtimeBitcodePath?: string;
  compilerArgs?: string[];
  debug?: boolean;
  extensionPath?: string;
}

function parseCompilerDiagnosticLine(line: string, defaultSourcePath?: string): CompilerDiagnostic | undefined {
  const trimmed = line.trim();
  if (!trimmed) {
    return undefined;
  }

  const fileWithLineAndColumn = trimmed.match(/^(.*?):(\d+):(\d+):\s*(error|warning):\s*(.+)$/i);
  if (fileWithLineAndColumn) {
    return {
      filePath: fileWithLineAndColumn[1],
      line: Number(fileWithLineAndColumn[2]),
      column: Number(fileWithLineAndColumn[3]),
      severity: fileWithLineAndColumn[4].toLowerCase() === 'warning' ? 'warning' : 'error',
      message: fileWithLineAndColumn[5].trim(),
      raw: trimmed,
    };
  }

  const fileWithLine = trimmed.match(/^(.*?):(\d+):\s*(error|warning):\s*(.+)$/i);
  if (fileWithLine) {
    return {
      filePath: fileWithLine[1],
      line: Number(fileWithLine[2]),
      severity: fileWithLine[3].toLowerCase() === 'warning' ? 'warning' : 'error',
      message: fileWithLine[4].trim(),
      raw: trimmed,
    };
  }

  const compilationError = trimmed.match(/^❌?\s*Compilation error:\s*(.+?)\s+at line\s+(\d+)(?:\s*(?:,|:)?\s*(?:column|col)\s+(\d+))?\.?$/i);
  if (compilationError) {
    return {
      filePath: defaultSourcePath,
      line: Number(compilationError[2]),
      column: compilationError[3] ? Number(compilationError[3]) : undefined,
      severity: 'error',
      message: compilationError[1].trim(),
      raw: trimmed,
    };
  }

  const genericLineMessage = trimmed.match(/^(.+?)\s+at line\s+(\d+)(?:\s*(?:,|:)?\s*(?:column|col)\s+(\d+))?\.?$/i);
  if (genericLineMessage) {
    return {
      filePath: defaultSourcePath,
      line: Number(genericLineMessage[2]),
      column: genericLineMessage[3] ? Number(genericLineMessage[3]) : undefined,
      severity: 'error',
      message: genericLineMessage[1].trim(),
      raw: trimmed,
    };
  }

  return undefined;
}

export function parseCompilerDiagnostics(output: string, defaultSourcePath?: string): CompilerDiagnostic[] {
  return output
    .split(/\r?\n/)
    .map(line => parseCompilerDiagnosticLine(line, defaultSourcePath))
    .filter((diagnostic): diagnostic is CompilerDiagnostic => diagnostic !== undefined);
}

function createCompilationError(message: string, output: string, defaultSourcePath?: string): MlpCompilationError {
  const normalizedOutput = output.trim() || message;
  return new MlpCompilationError(
    normalizedOutput,
    normalizedOutput,
    parseCompilerDiagnostics(normalizedOutput, defaultSourcePath)
  );
}

function runProcess(command: string, args: string[], cwd?: string, allowFailure = false): Promise<string> {
  return new Promise((resolve, reject) => {
    const child = spawn(command, args, { cwd });
    let output = '';

    child.stdout.on('data', (chunk: Buffer) => {
      output += chunk.toString();
    });

    child.stderr.on('data', (chunk: Buffer) => {
      output += chunk.toString();
    });

    child.on('error', error => {
      reject(error);
    });

    child.on('close', code => {
      if (code === 0 || allowFailure) {
        resolve(output);
        return;
      }

      reject(new Error(output || `${command} exited with code ${code}`));
    });
  });
}

function findExecutable(candidates: string[]): string {
  for (const candidate of candidates) {
    const result = spawnSync(candidate, ['--version'], { encoding: 'utf-8' });
    if (!result.error) {
      return candidate;
    }
  }

  throw new Error(`Gerekli derleme araci bulunamadi: ${candidates.join(', ')}`);
}

export function getBundledRuntimeBitcodePath(extensionPath: string): string | undefined {
  const runtimeBitcodePath = path.join(extensionPath, 'bin', 'mlp_runtime.bc');
  return fs.existsSync(runtimeBitcodePath) ? runtimeBitcodePath : undefined;
}

export async function compileMlpProgram(options: CompileMlpProgramOptions): Promise<string> {
  const workDir = await fs.promises.mkdtemp(path.join(os.tmpdir(), 'mlp-build-'));
  const outputLlPath = path.join(workDir, 'output.ll');
  const linkedBitcodePath = path.join(workDir, 'linked.bc');
  const objectPath = path.join(workDir, `${path.basename(options.outputPath)}.o`);
  const normalizedSourcePath = path.join(workDir, 'normalized.pmlp');
  const compilerArgs = options.compilerArgs ?? [];
  const lang = options.sourcePath.endsWith('.tr.mlp')
    ? 'turkish'
    : options.sourcePath.endsWith('.ru.mlp')
      ? 'russian'
      : options.sourcePath.endsWith('.ar.mlp')
        ? 'arabic'
        : options.sourcePath.endsWith('.zh.mlp')
          ? 'chinese'
          : 'english';
  const compileArgs = [
    normalizedSourcePath,
    ...compilerArgs,
    ...(options.debug ? ['--debug', '-g'] : []),
    '-o',
    options.outputPath,
  ];

  let compileOutput = '';

  try {
    fs.writeFileSync('/tmp/.melp_norm_src', fs.readFileSync(options.sourcePath));
    fs.writeFileSync('/tmp/.melp_norm_lang', lang);
    const langCode = lang === 'turkish' ? 'tr'
      : lang === 'russian' ? 'ru'
      : lang === 'arabic' ? 'ar'
      : lang === 'chinese' ? 'zh'
      : 'en';
    const normDillerPath = options.extensionPath
      ? path.join(options.extensionPath, 'language', langCode, 'keywords.json')
      : `/home/pardus/PROJELER/MELP/ORTAK/dil/${langCode}/keywords.json`;
    fs.writeFileSync('/tmp/.melp_norm_diller', normDillerPath);

    const normalizerResult = spawnSync(options.normalizerPath, [], { encoding: 'utf-8' });
    if (normalizerResult.error) {
      throw createCompilationError(
        `Normalizer calistirilamadi: ${normalizerResult.error.message}`,
        normalizerResult.error.message,
        options.sourcePath
      );
    }
    if (normalizerResult.status !== 0) {
      const normalizeOutput = `${normalizerResult.stdout ?? ''}${normalizerResult.stderr ?? ''}`.trim();
      throw createCompilationError(
        'Normalizer asamasi basarisiz oldu.',
        normalizeOutput || 'Normalizer asamasi basarisiz oldu.',
        options.sourcePath
      );
    }

    const normalizedSource = normalizerResult.stdout ?? '';
    if (!normalizedSource.trim()) {
      throw createCompilationError('Normalizer cikti uretmedi.', 'Normalizer cikti uretmedi.', options.sourcePath);
    }
    fs.writeFileSync(normalizedSourcePath, normalizedSource, 'utf-8');

    compileOutput = await runProcess(options.compilerPath, compileArgs, workDir, true);

    if (!fs.existsSync(outputLlPath)) {
      throw createCompilationError(compileOutput || 'Derleyici LLVM IR uretmedi.', compileOutput || 'Derleyici LLVM IR uretmedi.', options.sourcePath);
    }

    if (!options.runtimeBitcodePath || !fs.existsSync(options.runtimeBitcodePath)) {
      throw createCompilationError('mlp_runtime.bc bulunamadi.', 'mlp_runtime.bc bulunamadi.', options.sourcePath);
    }

    const llvmLink = findExecutable(['llvm-link', 'llvm-link-19', 'llvm-link-14']);
    const llc = findExecutable(['llc', 'llc-19', 'llc-14']);
    const clang = findExecutable(['clang', 'clang-19', 'clang-14']);

    compileOutput += await runProcess(llvmLink, ['--only-needed', outputLlPath, options.runtimeBitcodePath, '-o', linkedBitcodePath], workDir);
    compileOutput += await runProcess(llc, ['-filetype=obj', linkedBitcodePath, '-o', objectPath], workDir);
    compileOutput += await runProcess(clang, ['-no-pie', objectPath, '-o', options.outputPath], workDir);

    return compileOutput;
  } catch (error) {
    if (error instanceof MlpCompilationError) {
      throw error;
    }

    const detail = error instanceof Error ? error.message : String(error);
    const combinedOutput = [compileOutput.trim(), detail.trim()].filter(Boolean).join('\n');
    throw createCompilationError(detail, combinedOutput || detail, options.sourcePath);
  } finally {
    await fs.promises.rm(workDir, { recursive: true, force: true });
  }
}