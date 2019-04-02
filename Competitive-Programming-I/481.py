import math


def main():
    n = int(input())
    for _ in range(n):
        a, c = map(int, input().split())
        if c < a or c % a != 0:
            print(-1)
            continue
        div = c//a
        Min = math.gcd(div, a)
        ans = div*Min
        while math.gcd(ans, a) != Min:
            Min = math.gcd(ans, a)
            ans = div*Min
        print(ans)


if __name__ == "__main__":
    main()
