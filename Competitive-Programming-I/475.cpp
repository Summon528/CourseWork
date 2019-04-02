#include <bits/stdc++.h>
#define INF 1000000000

using namespace std;

int main() {
    unordered_map<int, pair<int, int>> ump;
    ump[1] = make_pair(1, 2);

    unordered_map<char, vector<vector<int>>> tetris = {
        {'L', {{0, -1}, {1, -1}, {2, -1}, {2, 0}}},
        {'J', {{0, 0}, {1, 0}, {2, 0}, {2, -1}}},
        {'O', {{0, 0}, {0, -1}, {1, 0}, {1, -1}}},
        {'I', {{0, 0}, {1, 0}, {2, 0}, {3, 0}}},
        {'Z', {{0, -2}, {0, -1}, {1, -1}, {1, 0}}},
        {'S', {{0, -1}, {0, 0}, {1, -1}, {1, -2}}},
        {'T', {{0, -2}, {0, -1}, {0, 0}, {1, -1}}}};
    int P, W, safe;
    cin >> P >> W;
    safe = P * 4 + 5;
    vector<vector<int>> board(safe + 1, vector<int>(W));
    vector<int> top(W, safe);
    while (P--) {
        char L;
        string S;
        cin >> L >> S;
        vector<vector<int>> block(4, vector<int>(2));
        for (int i = 0; i < 4; i++) {
            block[i][0] = tetris[L][i][0];
            block[i][1] = tetris[L][i][1] + W - 1;
        }
        for (int i = 0; i < S.length(); i++) {
            int dx = S[i] == '>' ? 1 : -1;
            bool flag = true;
            for (auto j : block) {
                if (j[1] + dx >= W or j[1] + dx < 0) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                for (auto &&j : block) j[1] += dx;
            }
        }
        int dis = INF;
        for (auto i : block) dis = min(dis, top[i[1]] - i[0]);
        for (auto i : block) board[i[0] + dis][i[1]] = 1;
        for (auto i : block) top[i[1]] = min(top[i[1]], i[0] + dis - 1);
        int line = 0;
        unordered_set<int> dropIdx;
        for (int i = 0; i < board.size(); i++) {
            if (all_of(board[i].begin(), board[i].end(),
                       [](int i) { return i; })) {
                line += 1;
                dropIdx.insert(i);
            }
        }
        vector<vector<int>> tmp(dropIdx.size(), vector<int>(W));
        for (int i = 0; i < board.size(); i++) {
            if (dropIdx.find(i) == dropIdx.end()) {
                tmp.push_back(board[i]);
            }
        }
        board = tmp;
        if (line) {
            for (int i = 0; i < W; i++) {
                top[i] = safe;
                for (int j = safe; j >= 0; j--) {
                    if (board[j][i]) top[i] = j - 1;
                }
            }
        }
    }
    cout << safe - *min_element(top.begin(), top.end()) << endl;
}
