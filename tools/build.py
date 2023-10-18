import glob
import os
import shutil
import subprocess


currentDirectory = os.path.dirname(__file__)

try:
    os.mkdir('site')
except FileExistsError:
    pass

shutil.copyfile(
    os.path.join(currentDirectory, 'default.css'),
    os.path.join('site', 'default.css'),
)

shutil.copyfile(
    os.path.join(currentDirectory, '404.html'),
    os.path.join('site', '404.html'),
)

for txtname in glob.glob(os.path.join('posts', '*')):
    filename = os.path.splitext(os.path.basename(txtname))[0]
    htmlname = filename + '.html'
    with open(txtname) as source:
        with open(os.path.join('site', htmlname), 'w') as html:
            result = '<!DOCTYPE html>'
            result += '<html lang="ru">'
            result += '<head>'
            result += '<meta charset="utf-8">'
            result += '<meta name="viewport" content="width=device-width, initial-scale=1">'
            result += '<link rel="stylesheet" href="/default.css">'
            result += '</head>'
            result += '<body>'
            html.write(result)
            html.flush()
            subprocess.run(['python3', os.path.join(currentDirectory, 'txtToHtml.py')],
                stdin=source, stdout=html)
            result = '</body>'
            result += '</html>'
            html.write(result)

with open(os.path.join('site', 'index.html'), 'w') as index:
    result = '<!DOCTYPE html>'
    result += '<html lang="ru">'
    result += '<head>'
    result += '<meta charset="utf-8">'
    result += '<meta name="viewport" content="width=device-width, initial-scale=1">'
    result += '<link rel="stylesheet" href="/default.css">'
    result += '</head>'
    result += '<body>'
    headers = []
    for fullname in glob.glob(os.path.join('posts', '*')):
        with open(fullname) as html:
            fileHeaders = subprocess.run(['python3', os.path.join(currentDirectory, 'getHeaders.py')],
                stdin=html, capture_output=True).stdout.decode()
        headers.append((os.path.splitext(os.path.basename(fullname))[0], fileHeaders))
    for header in reversed(sorted(headers)):
        result += '<p><a href="/' + header[0] + '.html">' + ' | '.join(header[1].splitlines()) + '</a></p>'
    result += '</body>'
    result += '</html>'
    index.write(result)
