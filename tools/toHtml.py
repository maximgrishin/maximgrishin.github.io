import os
import subprocess
import sys


def readBlocks():
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


def texToHtml(text):
    return subprocess.run(['node', os.path.join('tools', 'texToHtml.js')],
        input=text.encode(), capture_output = True).stdout.decode()


def processLine(line):
    result = ''
    mode = ''
    span = ''
    toHtml = {
        '': lambda text : text.replace('--', '—'),
        '*': lambda text : '<em>' + escape(text) + '</em>',
        '`': lambda text : '<code>' + escape(text) + '</code>',
        '$': texToHtml,
    }
    for char in line:
        if char in toHtml:
            result += toHtml[mode](span)
            if mode:
                mode = ''
            else:
                mode = char
            span = ''
        else:
            span += char
    result += toHtml[mode](span)
    return result


def processBlocks(blocks):
    result = '<!DOCTYPE html>'
    result += '<html lang="ru">'
    result += '<head>'
    result += '<meta charset="utf-8">'
    result += '<meta name="viewport" content="width=device-width, initial-scale=1">'
    result += '<link rel="stylesheet" href="css/style.css">'
    result += '<link rel="stylesheet" href="css/katex.min.css">'
    result += '<title>' + blocks[0][0] + '</title>'
    result += '</head>'
    result += '<body>'
    result += '<h1>' + blocks[0][0] + '</h1>'
    for block in blocks[1:]:
        if all(line.startswith(' ' * 4) for line in block):
            html = (
                '<pre><code>' +
                '\n'.join(escape(line[4:]) for line in block) +
                '</code></pre>'
            )
        elif len(block) == 2 and block[1] == '=' * len(block[0]):
            html = (
                '<h2>' + processLine(block[0]) + '</h2>'
            )
        elif len(block) == 2 and block[1] == '-' * len(block[0]):
            html = (
                '<h3>' + processLine(block[0]) + '</h3>'
            )
        else:
            html = (
                '<p>' +
                ' '.join(processLine(line) for line in block) +
                '</p>'
            )
        result += html
    result += '</body>'
    result += '</html>'
    return result


print(processBlocks(readBlocks()))
