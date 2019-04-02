tetris = {
    'L': [(0, -1), (1, -1), (2, -1), (2, 0)],
    'J': [(0, 0), (1, 0), (2, 0), (2, -1)],
    'O': [(0, 0), (0, -1), (1, 0), (1, -1)],
    'I': [(0, 0), (1, 0), (2, 0), (3, 0)],
    'Z': [(0, -2), (0, -1), (1, -1), (1, 0)],
    'S': [(0, -1), (0, 0), (1, -1), (1, -2)],
    'T': [(0, -2), (0, -1), (0, 0), (1, -1)]
}


def main():
    P, W = map(int, input().split())
    safe = P*4+99
    board = [[0]*W for _ in range(safe+1)]
    top = [safe] * W
    for p in range(P):
        L = input()
        S = list(input())
        block = list(map(lambda x: [x[0], x[1]+W-1], tetris[L]))
        for i in S:
            dx = 1 if i == '>' else -1
            for j in block:
                if j[1] + dx >= W or j[1] + dx < 0:
                    break
            else:
                for j in block:
                    j[1] = j[1] + dx
        dis = 10000000
        for i in block:
            dis = min(dis, top[i[1]]-i[0])
        for i in block:
            board[i[0]+dis][i[1]] = 1
        for i in block:
            top[i[1]] = min(top[i[1]], i[0]+dis-1)
        line = 0
        dropIdx = []
        for idx, i in enumerate(board[max]):
            if all(i):
                line += 1
                dropIdx.append(idx)
        for i in dropIdx:
            board = [[0]*W] + board[:i] + board[i+1:]
        if line:
            for i in range(W):
                top[i] = safe
                for j in range(safe, -1, -1):
                    if board[j][i]:
                        top[i] = j - 1
    print(safe-min(top))


if __name__ == "__main__":
    main()
