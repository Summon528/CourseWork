#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

pair<ll, ll> solve(int x, vector<ll>& w, vector<vector<ll>>& g, vector<ll>& inc,
                   vector<ll>& exc) {
    inc[x] = w[x], exc[x] = 0;
    for (auto i : g[x]) {
        auto [inc2, exc2] = solve(i, w, g, inc, exc);
        inc[x] += exc2;
        exc[x] += max(inc2, exc2);
    }
    return make_pair(inc[x], exc[x]);
}

void solve2(int x, int fa, vector<ll>& inc, vector<ll>& exc, vector<bool>& ans,
            vector<vector<ll>>& g) {
    if (!ans[fa] && inc[x] > exc[x]) ans[x] = 1;
    for (auto i : g[x]) solve2(i, x, inc, exc, ans, g);
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    vector<ll> w(n + 1);
    for (int i = 1; i <= n; i++) cin >> w[i];
    vector<vector<ll>> g(n + 1);

    bitset<1000005> qq;
    for (int i = 1; i < n; i++) {
        int a, b;
        cin >> a >> b;
        g[b].push_back(a);
        qq.set(a);
    }

    int root;
    for (root = 1; qq[root]; root++)
        ;

    vector<ll> inc(n + 1), exc(n + 1);
    vector<bool> ans(n + 1);
    solve(root, w, g, inc, exc);
    solve2(root, 0, inc, exc, ans, g);
    cout << max(inc[root], exc[root]) << endl;
    for (int i = 1; i <= n; i++) {
        if (ans[i]) cout << i << ' ';
    }
}
