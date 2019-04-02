#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

struct edge {
    ll u, v, w;
};

struct diset {
    vector<int> v;
    diset(int size) { v.resize(size, -1); }
    int Find(int x) { return v[x] < 0 ? x : v[x] = Find(v[x]); }
    void Union(int x, int y) {
        int a = Find(x), b = Find(y);
        if (a == b) return;
        if (v[b] < v[a]) swap(b, a);
        v[a] += v[b], v[b] = a;
    }
};

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    int n, m, k;
    cin >> n >> m >> k;
    vector<edge> e;
    while (m--) {
        ll u, v, w;
        cin >> u >> v >> w;
        e.push_back({u, v, w});
    }
    sort(e.begin(), e.end(), [](edge &a, edge &b) { return a.w < b.w; });
    vector<edge> ans;
    diset ds(n + 1);
    for (auto &&i : e) {
        if (ds.Find(i.u) == ds.Find(i.v)) continue;
        ds.Union(i.u, i.v);
        ans.push_back(i);
        if (ans.size() == n - 1) break;
    }
    sort(ans.begin(), ans.end(), [](edge &a, edge &b) { return a.w < b.w; });
    cout << ans[k - 1].w << endl;
}
