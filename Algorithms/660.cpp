#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    using ll = long long;
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m, tmp;
    cin >> n >> m;
    vector<ll> v(1000005);
    for (int i = 0; i < n; i++) cin >> tmp, v[tmp]++;
    for (int i = 1; i <= 1000000; i++) v[i] += v[i - 1];
    for (int i = 0; i < m; i++) {
        int l, r;
        cin >> l >> r;
        cout << v[r] - v[l - 1] << '\n';
    }
}
