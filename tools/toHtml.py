import sys


def read_blocks():
    block = []
    blocks = []
    for line in sys.stdin.read().splitlines():
        if line == '':
            blocks.append(block)
            block = []
        else:
            block.append(line)
    blocks.append(block)
    return blocks


def escape(text):
    return (text
        .replace('&', '&amp;')
        .replace('<', '&lt;')
        .replace('>', '&gt;'))


def addInlineTags(line):
    result = ''
    emphasis = False
    code = False
    for char in line:
        if char == '*' and code == False:
            if emphasis == False:
                emphasis = True
                result += '<em>'
            else:
                emphasis = False
                result += '</em>'
        elif char == '`':
            if code == False:
                code = True
                result += '<code>'
            else:
                code = False
                result += '</code>'
        else:
            result += char
    if emphasis or code:
        raise ValueError("Unclosed inline tag")
    return result


def processLine(line):
    return addInlineTags(escape(line).replace('--', '—'))


def print_html():
    blocks = read_blocks()
    print('<!DOCTYPE html>')
    print('<html>')
    print('<head>')
    print('<meta charset="utf-8">')
    print('<meta name="viewport" content="width=device-width, initial-scale=1">')
    print('<link rel="stylesheet" href="css/style.css">')

    print('<link rel="stylesheet" href="katex/katex.min.css">')
    print('<script src="katex/katex.min.js"></script>')
    print('<script defer src="katex/contrib/auto-render.min.js"')
    print('onload="renderMathInElement(document.body);"></script>')

    print('<title>' + blocks[0][0] + '</title>')
    print('</head>')
    print('<body>')
    print('<h1>' + blocks[0][0] + '</h1>')
    for block in blocks[1:]:
        if block[0].startswith(' ' * 4):
            html = (
                '<pre><code>' +
                '\n'.join(escape(line[4:]) for line in block) +
                '</code></pre>'
            )
        elif len(block) > 1 and block[1][0] == '=':
            html = (
                '<h2>' + processLine(block[0]) + '</h2>'
            )
        elif len(block) > 1 and block[1][0] == '-':
            html = (
                '<h3>' + processLine(block[0]) + '</h3>'
            )
        elif block[0].startswith('<svg'):
            html = (
                '\n'.join(block)
            )
        else:
            html = (
                '<p>' +
                ' '.join(processLine(line) for line in block) +
                '</p>'
            )
        print(html)
    print('</body>')
    print('</html>')


print_html()
