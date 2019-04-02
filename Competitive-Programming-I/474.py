from collections import Counter


def main():
    T = int(input())
    for _ in range(T):
        card, seq = [], []
        for _ in range(3):
            card.append(list(map(lambda x: int(x[:-1]) if(x[:-1]) else 'J',
                                 input().split()))[1:])
        for _ in range(3):
            seq.append(list(map(int, input().split()))[1:])

        for i in range(3):
            cnt = dict(Counter(card[i]))
            cnt = {k: v % 2 for k, v in cnt.items()}
            result = []
            for j in card[i][::-1]:
                if cnt[j] == 1:
                    result.append(j)
                    cnt[j] -= 1
            card[i] = result[::-1]

        for r in range(1, 10001):
            for i in range(3):
                if not card[0] or not card[1] or not card[2]:
                    break
                nxt = 0 if i == 2 else i+1
                t = len(card[nxt])
                n = len(seq[i])
                x = (seq[i][(r-1) % n]+t-1) % t
                try:
                    card[i].remove(card[nxt][x])
                except ValueError:
                    card[i].append(card[nxt][x])
                card[nxt].pop(x)
            if not card[0] or not card[1] or not card[2]:
                break
        else:
            print('Good Game')
            continue

        for idx, i in enumerate(card):
            if 'J' in i:
                where = idx
        print(['Alice', 'Bob', 'Carol'][where])


if __name__ == "__main__":
    main()
