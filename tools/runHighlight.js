const hljs = require('./highlight.min.js');

var input = '';

process.stdin.on('data', function(chunk) {
    input += chunk.toString();
});

process.stdin.on('end', () => {
    const html = hljs.highlightAuto(input, ['cpp', 'python', 'html']);
    console.log(html.value);
});
