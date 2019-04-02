#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void dfs(int x, vector<vector<int>> &g, vector<bool> &vis, string &s,
         vector<bool> &chk) {
    if (vis[x]) return;
    vis[x] = true;
    chk[s[x] - 'a'] = true;
    for (auto i : g[x]) {
        dfs(i, g, vis, s, chk);
    }
}

int main() {
    cin.sync_with_stdio(0);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    string s;
    cin >> s;
    vector<vector<int>> g(n);
    for (int i = 0; i < m; i++) {
        int t1, t2;
        cin >> t1 >> t2;
        t1--, t2--;
        g[t1].push_back(t2);
        g[t2].push_back(t1);
    }
    string t;
    cin >> t;
    vector<bool> tt(26), vis(n);
    for (auto i : t) tt[i - 'a'] = true;
    bool flag = false;
    for (int i = 0; i < n; i++) {
        vector<bool> chk(26);
        if (!vis[i]) dfs(i, g, vis, s, chk);
        bool ok = true;
        for (int i = 0; i < 26; i++) {
            if (tt[i] && !chk[i]) {
                ok = false;
                break;
            }
        }
        if (ok) {
            flag = true;
            break;
        }
    }
    flag ? cout << "Yes" << endl : cout << "No" << endl;
}
