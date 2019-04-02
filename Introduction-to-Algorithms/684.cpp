#include <algorithm>
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    using ll = long long;
    const ll INF = 5000ll * 1000000 + 1000;
    int n, m;
    cin >> n >> m;
    vector<tuple<int, int, ll>> edge;
    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        edge.push_back(make_tuple(a, b, c));
    }
    for (int i = 1; i <= n; i++) {
        edge.push_back(make_tuple(0, i, 0));
    }
    vector<ll> d(n + 1, -INF);
    d[0] = 0;
    ll ans = -INF;
    for (int i = 0; i < n; i++) {
        for (auto&& e : edge) {
            int s = get<0>(e), t = get<1>(e);
            ll c = get<2>(e);
            if (d[s] != -INF) {
                d[t] = max(d[t], d[s] + c);
                ans = max(d[t], ans);
            }
        }
    }
    bool pcycle = false;
    for (auto&& e : edge) {
        int s = get<0>(e), t = get<1>(e);
        ll c = get<2>(e);
        if (d[t] < d[s] + c) pcycle = true;
    }
    pcycle ? cout << "INF" << endl : cout << ans << endl;
}
