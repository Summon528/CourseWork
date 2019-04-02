#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

void dfs(int x, int d, vector<int> &dep, vector<vector<int>> &t,
         vector<vector<int>> &fa) {
    dep[x] = d;
    for (auto i : t[x]) {
        fa[i][0] = x;
        dfs(i, d + 1, dep, t, fa);
    }
}

int solve(vector<int> x, vector<int> &dep, vector<vector<int>> &fa,
          int MAX_ST) {
    sort(x.begin(), x.end(), [&](int a, int b) { return dep[a] < dep[b]; });
    for (int i = 1; i < x.size(); i++) {
        for (int j = MAX_ST - 1; j >= 0; j--) {
            if (dep[fa[x[i]][j]] >= dep[x[0]]) x[i] = fa[x[i]][j];
        }
    }
    if (equal(x.begin() + 1, x.end(), x.begin())) return x[0];
    for (int i = MAX_ST - 1; i >= 0; i--) {
        if (!equal(x.begin() + 1, x.end(), x.begin(),
                   [&](int a, int b) { return fa[a][i] == fa[b][i]; })) {
            for (int j = 0; j < x.size(); j++) x[j] = fa[x[j]][i];
        }
    }
    return fa[x[0]][0];
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    vector<vector<int>> t(n + 1);
    for (int i = 1; i <= n; i++) {
        int tmp;
        cin >> tmp;
        t[tmp].push_back(i);
    }

    const int MAX_ST = __lg(n + 1) + 1;
    vector<int> dep(n + 1);
    vector<vector<int>> fa(n + 1, vector<int>(MAX_ST, -1));
    dfs(0, 0, dep, t, fa);

    for (int j = 1; j < MAX_ST; j++) {
        for (int i = 0; i <= n; i++) {
            if (~fa[i][j - 1]) fa[i][j] = fa[fa[i][j - 1]][j - 1];
        }
    }

    vector<int> ask(3);
    while (m--) {
        int ans = 0;
        for (int i = 0; i < 3; i++) {
            cin >> ask[i];
            ans += dep[ask[i]];
        }
        ans += dep[solve(ask, dep, fa, MAX_ST)];
        vector<vector<int>> vv(3);
        vv[0].push_back(ask[0]);
        vv[0].push_back(ask[1]);
        vv[1].push_back(ask[1]);
        vv[1].push_back(ask[2]);
        vv[2].push_back(ask[0]);
        vv[2].push_back(ask[2]);
        for (auto &&i : vv) ans -= dep[solve(i, dep, fa, MAX_ST)];
        cout << ans << endl;
    }
}
