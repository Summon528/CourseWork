def sqrt(x):
    tmp = int(x**0.5 + 1e-9)
    if tmp**2 == x:
        return tmp
    return False


def main():
    while True:
        good = True
        try:
            s = list(map(int, input().split()))
        except EOFError:
            break
        n = sqrt(len(s))
        s.sort()
        ans = []
        if not sqrt(s[0]):
            print('No')
            continue
        ans.append(sqrt(s[0]))
        s.pop(0)
        for i in range(1, n):
            if s[0] != s[1]:
                good = False
                break
            ans.append(s[0] // ans[0])
            s.pop(0)
            s.pop(0)
            try:
                s.remove(ans[-1] * ans[-1])
            except ValueError:
                good = False
                break
            for j in range(1, i):
                try:
                    s.remove(ans[i] * ans[j])
                    s.remove(ans[i] * ans[j])
                except ValueError:
                    good = False
                    break
        if good:
            print('Yes')
            print(*ans)
        else:
            print('No')


if __name__ == "__main__":
    main()
