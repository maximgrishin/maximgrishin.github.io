import sys


def escape(text):
    return (text
        .replace('&', '&amp;')
        .replace('<', '&lt;')
        .replace('>', '&gt;'))


for article in sys.stdin.read().strip().split('\n\n\n\n'):
    for i, block in enumerate(article.split('\n\n')):
        if i == 0:
            print('<h2>' + escape(block) + '</h2>')
        else:
            if all(line.startswith(' ' * 4) for line in block.split('\n')):
                print(
                    '<pre><code>' +
                    '\n'.join(escape(line[4:]) for line in block.split('\n')) +
                    '</code></pre>'
                )
            elif len(block.split('\n')) == 1:
                print('<p>' + escape(block) + '</p>')
            else:
                raise ValueError('Bad block', block)
