#include <algorithm>
#include <functional>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

void dfs1(int x, vector<vector<int>> &g, vector<bool> &vis, stack<int> &st) {
    if (vis[x]) return;
    vis[x] = true;
    for (auto i : g[x]) dfs1(i, g, vis, st);
    st.push(x);
}

void dfs2(int x, vector<vector<int>> &g, vector<bool> &vis, string &s,
          vector<bool> &chk) {
    if (vis[x]) return;
    vis[x] = true;
    chk[s[x] - 'a'] = true;
    for (auto i : g[x]) {
        dfs2(i, g, vis, s, chk);
    }
}

int main() {
    cin.sync_with_stdio(0);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    string s;
    cin >> s;
    vector<vector<int>> g(n), gt(n);
    for (int i = 0; i < m; i++) {
        int t1, t2;
        cin >> t1 >> t2;
        t1--, t2--;
        g[t1].push_back(t2);
        gt[t2].push_back(t1);
    }

    vector<bool> tt(26), vis(n);
    stack<int> st;
    string t;
    cin >> t;
    for (auto i : t) tt[i - 'a'] = true;

    for (int i = 0; i < n; i++) {
        if (vis[i]) continue;
        dfs1(i, g, vis, st);
    }

    bool flag = false;
    fill(vis.begin(), vis.end(), false);
    while (!st.empty()) {
        int i = st.top();
        st.pop();
        vector<bool> chk(26);
        if (vis[i]) continue;
        dfs2(i, gt, vis, s, chk);
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
