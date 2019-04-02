#include <iostream>
#include <map>
#include <vector>

using namespace std;

int main() {
    using ll = long long;
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    ll n, x;
    cin >> n >> x;
    vector<ll> v1(n), v2(n);
    for (int i = 0; i < n; i++) cin >> v1[i];
    for (int i = 0; i < n; i++) cin >> v2[i];
    map<ll, int> um;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            um[v1[i] + v2[j]]++;
        }
    }

    ll sun = 0, tmp;
    for (int i = 0; i < n; i++) cin >> v1[i];
    for (int i = 0; i < n; i++) {
        cin >> tmp;
        for (int j = 0; j < n; j++) {
            sun += um[x - tmp - v1[j]];
        }
    }
    cout << sun << endl;
}
