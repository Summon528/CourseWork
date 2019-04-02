#include <iostream>
#include <queue>
#include <vector>
#define INF 1023456789
#ifdef LOCAL
#include "stloutput.h"
#endif
using namespace std;

using vvpii = vector<vector<pair<int, int>>>;

bool bfs(vvpii &g, int dst, vector<pair<int, int>> &pre) {
    vector<bool> vis(dst + 1);
    queue<int> Q;
    Q.push(0);
    while (!Q.empty()) {
        int x = Q.front();
        Q.pop();
        for (auto &&i : g[x]) {
            if (!vis[i.first] && i.second > 0) {
                vis[i.first] = true;
                pre[i.first] = make_pair(x, i.second);
                Q.push(i.first);
            }
        }
    }
    return vis[dst];
}

inline void add_edge(int u, int v, int w, vvpii &g) {
    g[u].push_back(make_pair(v, w));
    g[v].push_back(make_pair(u, 0));
}

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    int n, m;
    cin >> n >> m;
    int dst = 2 * n + m + 1;
    vvpii g(dst + 1);
    for (int i = 1; i <= 2 * n; i++) {
        int tmp;
        cin >> tmp;
        add_edge(0, i, tmp, g);
    }
    int sun = 0;
    for (int i = 0, idx = 2 * n + 1; i < m; i++, idx++) {
        int a, b, c;
        cin >> a >> b >> c;
        add_edge(a, idx, INF, g);
        add_edge(b + n, idx, INF, g);
        add_edge(idx, dst, c, g);
        sun += c;
    }
    vector<pair<int, int>> pre(dst + 1);
    int flow = 0;
    while (bfs(g, dst, pre)) {
        int neck = INF;
        for (int i = dst; i != 0; i = pre[i].first) {
            neck = min(neck, pre[i].second);
        }
        for (int i = dst; i != 0; i = pre[i].first) {
            for (auto &&j : g[pre[i].first])
                if (j.first == i) j.second -= neck;
            for (auto &&j : g[i])
                if (j.first == pre[i].first) j.second += neck;
        }
        flow += neck;
        if (sun - flow <= 0) break;
    }
    cout << (sun - flow <= 0 ? "Yes" : "No") << '\n';
}
