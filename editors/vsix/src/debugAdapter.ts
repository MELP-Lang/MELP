/**
 * debugAdapter.ts — MLP Debug Adapter
 *
 * VS Code DAP ↔ GDB/MI köprüsü.
 * Ayrı bir Node.js process olarak çalışır (webpack ile dist/debugAdapter.js).
 *
 * Akış:
 *   1. VS Code "launch" gönderir
 *   2. mlp dosyası --debug -g ile derlenir → geçici binary
 *   3. gdb --interpreter=mi2 başlatılır
 *   4. VS Code'dan gelen breakpoint'ler GDB'ye iletilir
 *   5. SIGTRAP / breakpoint → StoppedEvent gönderilir
 */

import {
  DebugSession,
  InitializedEvent,
  TerminatedEvent,
  StoppedEvent,
  OutputEvent,
  Thread,
  StackFrame,
  Scope,
  Source,
  Handles,
  Breakpoint,
} from '@vscode/debugadapter';
import { DebugProtocol } from '@vscode/debugprotocol';
import { spawn, ChildProcess } from 'child_process';
import * as path from 'path';
import * as fs from 'fs';
import * as os from 'os';
import { compileMlpProgram } from './mlpCompiler';

interface LaunchArgs extends DebugProtocol.LaunchRequestArguments {
  program: string;
  binaryPath?: string;
  compilerPath?: string;
  normalizerPath?: string;
  compilerArgs?: string[];
  runtimeBitcodePath?: string;
  stopOnEntry?: boolean;
  cwd?: string;
}

// Bekleyen MI komutları için callback kuyruğu
type MIResolve = (result: string) => void;

export class MlpDebugSession extends DebugSession {
  private static THREAD_ID = 1;

  private _gdb: ChildProcess | null = null;
  private _variableHandles = new Handles<string>();
  private _tmpBinary = '';
  private _tmpSource = '';
  private _sourceFile = '';
  private _miBuffer = '';
  private _miQueue: MIResolve[] = [];
  private _token = 1;

  constructor() {
    super();
    this.setDebuggerLinesStartAt1(true);
    this.setDebuggerColumnsStartAt1(true);
  }

  // ── Initialize ─────────────────────────────────────────────────────────────
  protected override initializeRequest(
    response: DebugProtocol.InitializeResponse,
    _args: DebugProtocol.InitializeRequestArguments
  ): void {
    response.body = response.body ?? {};
    response.body.supportsConfigurationDoneRequest = true;
    response.body.supportsEvaluateForHovers = false;
    response.body.supportsSetVariable = false;
    this.sendResponse(response);
    this.sendEvent(new InitializedEvent());
  }

  // ── Launch ─────────────────────────────────────────────────────────────────
  protected override async launchRequest(
    response: DebugProtocol.LaunchResponse,
    args: LaunchArgs
  ): Promise<void> {
    try {
      this._sourceFile = args.program;

      if (!fs.existsSync(this._sourceFile)) {
        throw new Error(`Kaynak dosya bulunamadı: ${this._sourceFile}`);
      }

      const compiler = args.compilerPath ?? 'melp_compiler';
      const normalizer = args.normalizerPath ?? 'melp_normalizer_bin';
      if (args.binaryPath && fs.existsSync(args.binaryPath)) {
        this._tmpBinary = args.binaryPath;
      } else {
        const tmpId = Date.now();
        this._tmpSource = path.join(os.tmpdir(), `mlp_dbg_${tmpId}.mlp`);
        this._tmpBinary = path.join(os.tmpdir(), `mlp_dbg_${tmpId}`);

        fs.copyFileSync(this._sourceFile, this._tmpSource);

        await this._compile(compiler, normalizer, this._tmpSource, this._tmpBinary, args.compilerArgs ?? [], args.runtimeBitcodePath);
      }

      // GDB başlat
      await this._launchGDB(this._tmpBinary, args.cwd ?? path.dirname(this._sourceFile));

      if (args.stopOnEntry) {
        await this._mi('-break-insert main');
      }

      await this._mi('-exec-run');

      this.sendResponse(response);
    } catch (err) {
      this.sendErrorResponse(response, 1001, `Başlatma hatası: ${err}`);
      this._cleanup();
    }
  }

  // ── Breakpoints ────────────────────────────────────────────────────────────
  protected override async setBreakPointsRequest(
    response: DebugProtocol.SetBreakpointsResponse,
    args: DebugProtocol.SetBreakpointsArguments
  ): Promise<void> {
    // Önce bu dosyadaki tüm breakpoint'leri sil
    await this._mi('-break-delete');

    const bps: DebugProtocol.Breakpoint[] = [];
    const srcFile = args.source.path ?? '';

    for (const bp of args.breakpoints ?? []) {
      try {
        await this._mi(`-break-insert ${srcFile}:${bp.line}`);
        bps.push(new Breakpoint(true, bp.line));
      } catch {
        bps.push(new Breakpoint(false, bp.line));
      }
    }

    response.body = { breakpoints: bps };
    this.sendResponse(response);
  }

  // ── ConfigurationDone ──────────────────────────────────────────────────────
  protected override configurationDoneRequest(
    response: DebugProtocol.ConfigurationDoneResponse,
    _args: DebugProtocol.ConfigurationDoneArguments
  ): void {
    this.sendResponse(response);
  }

  // ── Threads ────────────────────────────────────────────────────────────────
  protected override threadsRequest(response: DebugProtocol.ThreadsResponse): void {
    response.body = { threads: [new Thread(MlpDebugSession.THREAD_ID, 'main')] };
    this.sendResponse(response);
  }

  // ── Stack ──────────────────────────────────────────────────────────────────
  protected override stackTraceRequest(
    response: DebugProtocol.StackTraceResponse,
    _args: DebugProtocol.StackTraceArguments
  ): void {
    response.body = {
      stackFrames: [
        new StackFrame(
          0,
          'main',
          new Source(path.basename(this._sourceFile), this._sourceFile),
          1
        ),
      ],
      totalFrames: 1,
    };
    this.sendResponse(response);
  }

  // ── Scopes ────────────────────────────────────────────────────────────────
  protected override scopesRequest(
    response: DebugProtocol.ScopesResponse,
    _args: DebugProtocol.ScopesArguments
  ): void {
    response.body = {
      scopes: [
        new Scope('Locals', this._variableHandles.create('locals'), false),
      ],
    };
    this.sendResponse(response);
  }

  // ── Variables ─────────────────────────────────────────────────────────────
  protected override variablesRequest(
    response: DebugProtocol.VariablesResponse,
    _args: DebugProtocol.VariablesArguments
  ): void {
    response.body = { variables: [] };
    this.sendResponse(response);
  }

  // ── Continue ──────────────────────────────────────────────────────────────
  protected override async continueRequest(
    response: DebugProtocol.ContinueResponse,
    _args: DebugProtocol.ContinueArguments
  ): Promise<void> {
    await this._mi('-exec-continue');
    response.body = { allThreadsContinued: true };
    this.sendResponse(response);
  }

  // ── Next (step over) ──────────────────────────────────────────────────────
  protected override async nextRequest(
    response: DebugProtocol.NextResponse,
    _args: DebugProtocol.NextArguments
  ): Promise<void> {
    await this._mi('-exec-next');
    this.sendResponse(response);
  }

  // ── Disconnect ────────────────────────────────────────────────────────────
  protected override disconnectRequest(
    response: DebugProtocol.DisconnectResponse,
    _args: DebugProtocol.DisconnectArguments
  ): void {
    this._cleanup();
    this.sendResponse(response);
  }

  // ── İç yardımcılar ────────────────────────────────────────────────────────

  private async _compile(
    compiler: string,
    normalizer: string,
    src: string,
    out: string,
    compilerArgs: string[],
    runtimeBitcodePath?: string
  ): Promise<void> {
    await compileMlpProgram({
      compilerPath: compiler,
      normalizerPath: normalizer,
      sourcePath: src,
      outputPath: out,
      runtimeBitcodePath,
      compilerArgs,
      debug: true,
    });
  }

  private _launchGDB(binary: string, cwd: string): Promise<void> {
    return new Promise((resolve, reject) => {
      this._gdb = spawn('gdb', ['--interpreter=mi2', '--quiet', binary], { cwd });

      this._gdb.stdout!.on('data', (data: Buffer) => {
        this._onMIData(data.toString());
      });

      this._gdb.stderr!.on('data', (data: Buffer) => {
        this.sendEvent(new OutputEvent(data.toString(), 'stderr'));
      });

      this._gdb.on('close', () => {
        this.sendEvent(new TerminatedEvent());
        this._cleanup();
      });

      this._gdb.on('error', err => reject(err));

      // GDB hazır olunca resolve
      setTimeout(resolve, 500);
    });
  }

  private _mi(cmd: string): Promise<string> {
    return new Promise((resolve) => {
      const token = this._token++;
      this._miQueue.push(resolve);
      this._gdb?.stdin!.write(`${token}${cmd}\n`);
    });
  }

  private _onMIData(raw: string): void {
    this._miBuffer += raw;
    const lines = this._miBuffer.split('\n');
    this._miBuffer = lines.pop() ?? '';

    for (const line of lines) {
      this._handleMILine(line.trim());
    }
  }

  private _handleMILine(line: string): void {
    if (!line) return;

    // Komut sonucu → bekleyen resolve'u çözümle
    if (/^\d*\^(done|running|error|exit)/.test(line)) {
      const resolve = this._miQueue.shift();
      if (resolve) resolve(line);
      return;
    }

    // Async stop olayı
    if (line.includes('*stopped')) {
      const reason = line.includes('breakpoint-hit')   ? 'breakpoint'
                   : line.includes('signal-received')  ? 'pause'
                   : line.includes('end-stepping-range')? 'step'
                   : 'stopped';

      this.sendEvent(new StoppedEvent(reason, MlpDebugSession.THREAD_ID));
    }

    // Program çıktısı
    if (line.startsWith('~"')) {
      const text = line.slice(2, -1).replace(/\\n/g, '\n').replace(/\\"/g, '"');
      this.sendEvent(new OutputEvent(text, 'stdout'));
    }
  }

  private _cleanup(): void {
    if (this._gdb) {
      try { this._gdb.stdin!.write('-gdb-exit\n'); } catch { /* ignore */ }
      try { this._gdb.kill(); } catch { /* ignore */ }
      this._gdb = null;
    }
    try {
      if (this._tmpBinary) fs.unlinkSync(this._tmpBinary);
    } catch { /* ignore */ }
    try {
      if (this._tmpSource) fs.unlinkSync(this._tmpSource);
    } catch { /* ignore */ }
  }
}

// ── Entry point ───────────────────────────────────────────────────────────────
// Bu dosya `node debugAdapter.js` ile ayrı process olarak çalışır.
MlpDebugSession.run(MlpDebugSession);
