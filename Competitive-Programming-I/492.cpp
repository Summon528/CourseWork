#include <algorithm>
#include <iostream>
#include <vector>
#ifdef LOCAL
#include "stloutput.h"
#endif
#define INF 1000000000

using namespace std;

void dfs(int x, int y, vector<vector<int>> &g, vector<bool> &visit,
         vector<vector<int>> &ch, vector<int> &fa, vector<int> &h) {
    h[x] = y;
    visit[x] = true;
    for (auto i : g[x]) {
        if (!visit[i]) {
            ch[x].push_back(i);
            fa[i] = x;
            dfs(i, y + 1, g, visit, ch, fa, h);
        }
    }
}

pair<int, int> dfs2(int x, int &d, vector<vector<int>> &ch, int broke, int &drt,
                    int &ch1, int &ch2) {
    int Max1 = 0, Max2 = 0, M1ch = x, M2ch = x;
    for (auto i : ch[x]) {
        if (i == broke) continue;
        auto [tmp, tch] = dfs2(i, d, ch, broke, drt, ch1, ch2);
        if (tmp > Max1) {
            Max2 = Max1;
            Max1 = tmp;
            M2ch = M1ch;
            M1ch = tch;
        } else if (tmp > Max2) {
            Max2 = tmp;
            M2ch = tch;
        }
    }

    if (Max1 + Max2 + 1 > d) {
        d = Max1 + Max2 + 1;
        drt = x;
        ch1 = M1ch;
        ch2 = M2ch;
    }

    return make_pair(Max1 + 1, M1ch);
}

int find_mid(int ch1, int ch2, int drt, int idx, vector<int> &fa) {
    vector<int> path;
    while (ch1 != drt) {
        path.push_back(ch1);
        ch1 = fa[ch1];
    }
    path.push_back(drt);
    int len = path.size();
    while (ch2 != drt) {
        path.push_back(ch2);
        ch2 = fa[ch2];
    }
    reverse(path.begin() + len, path.end());
    return path[idx];
}

int main() {
    int n;
    cin >> n;
    vector<vector<int>> g(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    vector<bool> visit(n + 1);
    vector<int> h(n + 1), fa(n + 1);
    vector<vector<int>> ch(n + 1);
    dfs(1, 0, g, visit, ch, fa, h);
    int ans = INF, p1, p2;

    for (int i = 2; i <= n; i++) {
        int d1 = 0, d2 = 0;
        int drt1 = 0, ch11 = 0, ch12 = 0;
        int drt2 = 0, ch21 = 0, ch22 = 0;
        dfs2(i, d1, ch, -1, drt1, ch11, ch12);
        dfs2(1, d2, ch, i, drt2, ch21, ch22);
        if (ans > max(d1 / 2, d2 / 2)) {
            ans = max(d1 / 2, d2 / 2);
            p1 = find_mid(ch11, ch12, drt1, d1 / 2, fa);
            p2 = find_mid(ch21, ch22, drt2, d2 / 2, fa);
        }
    }
    cout << p1 << ' ' << p2 << ' ' << ans << endl;
}
