import re
from collections import Counter

cnt = Counter()

with open("mbox-short.txt") as f:
    for line in f:
        if line.startswith("From "):
            cnt[int(re.search(r"([\d]+):", line).group(1))] += 1

for i, j in cnt.most_common():
    print(i, j)
