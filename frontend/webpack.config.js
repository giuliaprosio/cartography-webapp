const path = require('path');
const NodePolyfillPlugin = require("node-polyfill-webpack-plugin");
const CopyPlugin = require("copy-webpack-plugin");	

module.exports = {
  entry: './index.js',
  output: {
    path: path.resolve(__dirname, '../out/static'),
    filename: 'bundle.js',
  },
  plugins: [
    new NodePolyfillPlugin(),
    new CopyPlugin({
      patterns: [
        "index.html",
        "stylesheet.css",
      ],
      options: {
        concurrency: 100,
      },
    }),
]
};
