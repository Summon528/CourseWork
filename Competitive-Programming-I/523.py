from collections import defaultdict
import sys
T = int(sys.stdin.readline())
for _ in range(T):
    n, k = map(int, sys.stdin.readline().split())
    row = defaultdict(int)
    col = defaultdict(int)
    ansa, ansb = 0, 0
    for _ in range(n):
        s = sys.stdin.readline()
        a, b = s[:10], s[10:]
        row[a] += 1
        col[b] += 1
        ansa += row[a] == k
        ansb += col[b] == k
        print(ansb, ansa)
