const path = require("path");
const HtmlWebpackPlugin = require('html-webpack-plugin');
const InlineChunkHtmlPlugin = require("react-dev-utils/InlineChunkHtmlPlugin");

module.exports = {
    mode: "development",

    entry: path.resolve(__dirname, 'src') + '/app.tsx',

    devtool: "source-map",

    resolve: {
        extensions: [".ts", ".tsx", ".js", ".jsx", ".css"]
    },

    output: {
        filename: 'index.js',
        path: path.resolve(__dirname, 'dist'),
    },

    module: {
        rules: [{
            test: /\.ts(x?)$/,
            exclude: /node_modules/,
            use: [{loader: "ts-loader"}]
        }, {
            enforce: "pre",
            test: /\.js$/,
            loader: "source-map-loader"
        }, {
            test: /\.css$/i,
            exclude: /node_modules/,
            use: ['style-loader', 'css-loader'],
        }, {
            test: /\.(jpg)|(png)$/,
            use: ['url-loader']
        }]
    },
    plugins: [
        new HtmlWebpackPlugin({
            title: "PvZ Emulator Debugger",
            template: path.resolve(__dirname, 'src') + "/index.html"
        }),
        new InlineChunkHtmlPlugin(HtmlWebpackPlugin, [/\.(js|css)$/])
    ]
};