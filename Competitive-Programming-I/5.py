n, q = map(int, input().split())
a = list(map(int, input().split()))
for _ in range(q):
    l, r, k = map(int, input().split())
    print(sorted(a[l - 1:r])[k - 1])
