const path = require('path');
const NodePolyfillPlugin = require("node-polyfill-webpack-plugin");
const CopyPlugin = require("copy-webpack-plugin");

module.exports = function(_, argv) {
  return {
    entry: './index.js',
    output: {
      path: path.resolve(__dirname, '../out/static'),
      filename: 'bundle.js',
    },
    devtool: argv.mode === 'production' ? 'source-map' : 'eval',
    plugins: [
      new NodePolyfillPlugin(),
      new CopyPlugin({
        patterns: [
          "index.html",
          "stylesheet.css",
          "node_modules/leaflet/dist/leaflet.css",
        ],
        options: {
          concurrency: 100,
        },
      }),
    ],
  };
};
