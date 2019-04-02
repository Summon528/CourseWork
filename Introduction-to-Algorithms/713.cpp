#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

void dfs(int x, vector<vector<int>> &g, vector<bool> &vis) {
    if (vis[x]) return;
    vis[x] = true;
    for (int i : g[x]) {
        dfs(i, g, vis);
    }
}

void solve() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> g(n);
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        g[x].push_back(y);
        g[y].push_back(x);
    }
    if (m < 4 || m == 4 && any_of(g.begin(), g.end(),
                                  [](auto &x) { return x.size() == 4; })) {
        cout << "No\n";
        return;
    }
    vector<bool> vis(n);
    dfs(0, g, vis);
    if (all_of(vis.begin(), vis.end(), [](bool x) { return x; })) {
        cout << "Yes\n";
    } else {
        cout << "No\n";
    }
}

int main() {
    cin.tie(0);
    cin.sync_with_stdio(0);
    string s;
    cin >> s;
    int T;
    cin >> T;
    while (T--) solve();
}
