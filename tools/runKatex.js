const katex = require('./katex.min.js');

var input = '';

process.stdin.on('data', function(chunk) {
    input += chunk.toString();
});

process.stdin.on('end', () => {
    const html = katex.renderToString(input, {
        throwOnError: false
    });
    console.log(html);
});
