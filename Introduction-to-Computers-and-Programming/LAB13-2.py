import urllib.request
from bs4 import BeautifulSoup

resp = urllib.request.urlopen(
    'https://docs.python.org/3/library/index.html').read().decode()

soup = BeautifulSoup(resp, 'html.parser')
for i, li in enumerate(soup.find(class_='toctree-wrapper').find('ul')('li', recursive=False)):
    ul2 = li.find('ul')
    if ul2:
        for j, el in enumerate(ul2('li')):
            print(f'{i+1}.{j+1}.{el.text}')
