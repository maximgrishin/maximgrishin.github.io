import glob
import os
import shutil
import subprocess


shutil.copytree('css', 'site/css', dirs_exist_ok=True)
shutil.copytree('katex', 'site/katex', dirs_exist_ok=True)

for txtname in glob.glob(os.path.join('posts', '*')):
    htmlname = os.path.splitext(os.path.basename(txtname))[0] + '.html'
    with open(txtname) as source:
        with open(os.path.join('site', htmlname), 'w') as html:
            subprocess.run(['python3', os.path.join('tools', 'toHtml.py')],
                stdin=source, stdout=html)


with open(os.path.join('site', 'index.html'), 'wb') as index:
    subprocess.run(['python3', os.path.join('tools', 'makeIndex.py')],
        stdout=index)
