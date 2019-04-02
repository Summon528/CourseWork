#include <iostream>
#include <utility>
#include <vector>

using namespace std;

void build(int x, int L, int R, vector<int>& v, vector<pair<int, int>>& t) {
    if (L == R) {
        t[x].first = v[L];
        t[x].second = -L;
        return;
    }
    int mid = (L + R) / 2;
    build(2 * x, L, mid, v, t);
    build(2 * x + 1, mid + 1, R, v, t);
    t[x] = max(t[2 * x], t[2 * x + 1]);
}

pair<int, int> query(int x, int L, int R, int askL, int askR,
                     vector<pair<int, int>>& t) {
    if (R < askL || L > askR) return make_pair(-1, -1);
    if (askR >= R && askL <= L) return t[x];
    int mid = (L + R) / 2;
    auto a = query(2 * x, L, mid, askL, askR, t);
    auto b = query(2 * x + 1, mid + 1, R, askL, askR, t);
    return max(a, b);
}

int main() {
    int n, m;
    cin >> n >> m;
    vector<int> v(n + 1);
    vector<pair<int, int>> t(n * 4 + 5);
    for (int i = 1; i <= n; i++) cin >> v[i];
    build(1, 1, n, v, t);
    for (int i = 0; i < m; i++) {
        int l, r;
        cin >> l >> r;
        cout << -query(1, 1, n, l, r, t).second << endl;
    }
}
