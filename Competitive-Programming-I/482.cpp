#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>
#define M 1000000007
using namespace std;
typedef long long ll;

inline int lowbit(int x) { return x & (-x); }

ll prefix(vector<ll> &v, int idx) {
    ll sun = 0;
    for (int i = idx; i > 0; i -= lowbit(i)) sun = (sun + v[i]) % M;
    return sun;
}

void update(vector<ll> &v, int idx, int val) {
    for (int i = idx; i < v.size(); i += lowbit(i)) v[i] = (v[i] + val) % M;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        vector<ll> v(n);
        for (int i = 0; i < n; i++) cin >> v[i];

        vector<ll> lu(v);
        sort(lu.begin(), lu.end());
        auto it = unique(lu.begin(), lu.end());

        unordered_map<int, int> um;
        int tmp = 1;
        for (auto i = lu.begin(); i != it; i++) um[*i] = tmp++;

        vector<ll> ans1(n), bit1(n + 1);  // increase
        for (int i = 0; i < n; i++) {
            int idx = um[v[i]];
            ll pre = prefix(bit1, idx - 1);
            ans1[i] = pre;
            update(bit1, idx, pre + 1);
        }

        vector<ll> ans2(n), bit2(n + 1);  // decrease
        for (int i = n - 1; i >= 0; i--) {
            int idx = um[v[i]];
            ll pre = prefix(bit2, idx - 1);
            ans2[i] = pre;
            update(bit2, idx, pre + 1);
        }

        ll sun = 0;
        for (int i = 0; i < n; i++) sun = (sun + ((ans1[i] * ans2[i]) % M)) % M;
        cout << sun << endl;
    }
}
