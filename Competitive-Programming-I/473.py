def main():
    EOF = False
    input()
    while not EOF:
        input()
        input()
        s = input().split('|')
        n = len(s) - 2
        table = {}
        idx1 = 0
        while idx1 + 1 < n:
            for idx2, i in enumerate(s[1:-1]):
                table[int(i)] = (idx1, idx2)
            input()
            s = input().split('|')
            idx1 += 1
        a = [0] * n
        b = [0] * n
        ans1, ans2 = 0, 0
        for idx2, i in enumerate(s[1:-1]):
            table[int(i)] = (idx1, idx2)
        input()
        print(table)
        while True:
            try:
                num = int(input())
            except ValueError:
                break
            except EOFError:
                EOF = True
                break
            try:
                a[table[num][0]] += 1
                if a[table[num][0]] >= n:
                    a[table[num][0]] = -1
                    ans1 += 1
                b[table[num][1]] += 1
                if b[table[num][1]] >= n:
                    b[table[num][1]] = -1
                    ans2 += 1
            except KeyError:
                continue
            print(ans1, ans2)


if __name__ == "__main__":
    main()
