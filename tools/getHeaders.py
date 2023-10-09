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
        if len(block) == 2 and block[1] == '=' * len(block[0]):
            result += escape(block[0]) + '\n'
    return result


print(processBlocks(readBlocks()))
