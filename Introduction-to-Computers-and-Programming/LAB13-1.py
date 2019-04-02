import urllib.request
import re
from collections import Counter

resp = urllib.request.urlopen(
    'https://docs.python.org/3/library/index.html').read().decode()

gen = (j for i in re.findall(r'.*/code> —(.*)', resp)
       for j in re.compile(r'<[^>]+>').sub('', i).split())

for i, j in Counter(gen).most_common(10):
    print(i, j)
