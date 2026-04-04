//@ts-check
'use strict';

const path = require('path');

/** @type {import('webpack').Configuration} */
const base = {
  target: 'node',
  mode: 'none',
  externals: { vscode: 'commonjs vscode' },
  resolve: { extensions: ['.ts', '.js'] },
  module: {
    rules: [{ test: /\.ts$/, exclude: /node_modules/, use: 'ts-loader' }],
  },
  devtool: 'nosources-source-map',
};

module.exports = [
  // Extension ana süreci
  Object.assign({}, base, {
    entry: './src/extension.ts',
    output: {
      path: path.resolve(__dirname, 'dist'),
      filename: 'extension.js',
      libraryTarget: 'commonjs2',
    },
  }),
  // Debug adapter — ayrı Node işlemi (gdb/mi)
  Object.assign({}, base, {
    entry: './src/debugAdapter.ts',
    output: {
      path: path.resolve(__dirname, 'dist'),
      filename: 'debugAdapter.js',
      libraryTarget: 'commonjs2',
    },
  }),
];
