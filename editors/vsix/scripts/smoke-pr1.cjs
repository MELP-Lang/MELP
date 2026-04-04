const fs = require('fs');
const os = require('os');
const path = require('path');
const { spawnSync } = require('child_process');

const rootDir = path.resolve(__dirname, '..');
const compilerPath = path.join(rootDir, 'bin', 'melp_compiler_linux');

const cases = [
  {
    name: 'unexpected-at-line-2',
    source: [
      'function main() as numeric',
      '    @@@',
      'end function',
    ].join('\n'),
    expectedLine: 2,
    expectedFragment: "Unexpected character '@'",
  },
  {
    name: 'unexpected-hash-line-3',
    source: [
      'function main() as numeric',
      '    print("ok")',
      '    ###',
      'end function',
    ].join('\n'),
    expectedLine: 3,
    expectedFragment: "Unexpected character '#'",
  },
  {
    name: 'unexpected-question-line-4',
    source: [
      'function main() as numeric',
      '    print("ok")',
      '    print("devam")',
      '    ???',
      'end function',
    ].join('\n'),
    expectedFragment: 'Parser error: Expected expression',
  },
];

if (!fs.existsSync(compilerPath)) {
  console.error(`Bundled compiler not found: ${compilerPath}`);
  process.exit(1);
}

const tempDir = fs.mkdtempSync(path.join(os.tmpdir(), 'mlp-pr1-smoke-'));
let failed = false;

try {
  for (const testCase of cases) {
    const sourcePath = path.join(tempDir, `${testCase.name}.mlp`);
    const outputPath = path.join(tempDir, `${testCase.name}.out`);

    fs.writeFileSync(sourcePath, `${testCase.source}\n`, 'utf8');

    const result = spawnSync(compilerPath, [sourcePath, '-o', outputPath], {
      cwd: rootDir,
      encoding: 'utf8',
      timeout: 5000,
    });

    const combinedOutput = `${result.stdout || ''}${result.stderr || ''}`.trim();

    if (result.status === 0) {
      console.error(`[FAIL] ${testCase.name}: compiler unexpectedly succeeded`);
      failed = true;
      continue;
    }

    if (!combinedOutput.includes(testCase.expectedFragment)) {
      console.error(`[FAIL] ${testCase.name}: missing fragment '${testCase.expectedFragment}'`);
      console.error(combinedOutput || '(empty output)');
      failed = true;
      continue;
    }

    if (typeof testCase.expectedLine === 'number' && !combinedOutput.includes(`at line ${testCase.expectedLine}`)) {
      console.error(`[FAIL] ${testCase.name}: missing line marker 'at line ${testCase.expectedLine}'`);
      console.error(combinedOutput || '(empty output)');
      failed = true;
      continue;
    }

    console.log(`[PASS] ${testCase.name}`);
    console.log(`       ${combinedOutput}`);
  }
} finally {
  fs.rmSync(tempDir, { recursive: true, force: true });
}

if (failed) {
  process.exit(1);
}

console.log('PR-1 smoke checks passed.');