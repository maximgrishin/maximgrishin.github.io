import glob
import os
import sys

print('<!DOCTYPE html>')
print('<html lang="ru">')
print('<head>')
print('<meta charset="utf-8">')
print('<meta name="viewport" content="width=device-width, initial-scale=1">')
print('<link rel="stylesheet" href="css/style.css">')
print('</head>')
print('<body>')
print('<ul>')
headers = []
for fullname in glob.glob(os.path.join('posts', '*')):
    with open(fullname) as text:
        header = text.readline().rstrip()
        link = os.path.splitext(os.path.basename(fullname))[0]
        headers.append((header, link))
for header, link in sorted(headers):
    print('<li><a href="' + link + '.html">' + header + '</a></li>')
print('</ul>')
print('</body>')
print('</html>')
