import re
T = int(input())
for _ in range(T):
    print('{:08d}'.format(sum(map(int, re.findall('[\\d]{8}', input())))))
