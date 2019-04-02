def main():
    while True:
        try:
            a = list(map(int,input().split()))
        except EOFError:
            break
        if len(a) == 0:
            continue
        sun = 0
        for i in a:
            sun+=i*i
        print(sun ** 0.5)

if __name__ == "__main__":
    main()
