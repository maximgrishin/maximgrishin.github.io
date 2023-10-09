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
    htmlname = os.path.splitext(os.path.basename(txtname))[0] + '.html'
    with open(txtname) as source:
        with open(os.path.join('site', htmlname), 'w') as html:
            subprocess.run(['python3', os.path.join(currentDirectory, 'txtToHtml.py')],
                stdin=source, stdout=html)

with open(os.path.join('site', 'index.html'), 'wb') as index:
    subprocess.run(['python3', os.path.join(currentDirectory, 'makeIndex.py')],
        stdout=index)
