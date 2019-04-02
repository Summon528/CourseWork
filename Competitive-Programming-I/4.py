_ = input()
a = list(map(int, input().split()))
Max = a[0]
Sum = 0
for i in a:
    Sum += i
    Max = max(Max, Sum)
    if Sum < 0:
        Sum = 0
print(Max)
