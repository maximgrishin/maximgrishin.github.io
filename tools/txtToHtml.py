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


def processBlocks(blocks):
    result = ''
    for block in blocks:
        if all(line.startswith(' ' * 4) for line in block):
            html = (
                '<pre><code>' +
                '\n'.join(escape(line[4:]) for line in block) +
                '</code></pre>'
            )
        elif len(block) == 2 and block[1] == '=' * len(block[0]):
            html = (
                '<h2 after="' + '=' * len(block[0])  + '">' + escape(block[0]) + '</h2>'
            )
        elif len(block) == 2 and block[1] == '-' * len(block[0]):
            html = (
                '<h3 after="' + '-' * len(block[0]) + '">' + escape(block[0]) + '</h3>'
            )
        elif len(block) == 1:
            html = (
                '<p>' + escape(block[0]) + '</p>'
                
            )
        else:
            raise ValueError('Bad block', block)
        result += html
    return result


print(processBlocks(readBlocks()))
