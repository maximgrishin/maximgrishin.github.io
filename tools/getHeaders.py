import sys


def escape(text):
    return (text
        .replace('&', '&amp;')
        .replace('<', '&lt;')
        .replace('>', '&gt;'))


for article in sys.stdin.read().split('\n\n\n\n'):
    print(escape(article.split('\n\n')[0]))
