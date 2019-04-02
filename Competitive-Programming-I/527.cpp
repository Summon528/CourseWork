#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int main() {
    vector<ll> n3;
    for (ll i = 0; i <= 10000; i++) n3.push_back(i * i * i);
    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        vector<pair<ll, bool> > v;
        for (int i = 0; i < n * n * n; i++) {
            ll tmp;
            cin >> tmp;
            v.push_back(make_pair(tmp, true));
        }

        sort(begin(v), end(v));

        vector<ll> x;
        x.push_back(distance(n3.begin(),
                             lower_bound(n3.begin(), n3.end(), v[0].first)));
        v.erase(v.begin());

        while (!v.empty()) {
            int k = x.size();
            x.push_back(v[0].first / x[0] / x[0]);
            for (int i = 0; i <= k; i++) {
                for (int j = 0; j <= i; j++) {
                    int cnt = 0;
                    if (i != j) cnt++;
                    if (i != k) cnt++;
                    if (j != k) cnt++;
                    if (cnt == 0) {
                        cnt = 1;
                    } else if (cnt == 1 || cnt == 2) {
                        cnt = 3;
                    } else {
                        cnt = 6;
                    }
                    auto it = lower_bound(v.begin(), v.end(),
                                          make_pair(x[i] * x[j] * x[k], true));
                    while (cnt--) {
                        (*it).second = false;
                        it++;
                    }
                }
            }
            auto ed = remove_if(v.begin(), v.end(),
                                [](pair<ll, bool> x) { return !x.second; });
            v.resize(ed - v.begin());
        }
        for (auto i : x) {
            cout << i << ' ';
        }
        cout << endl;

        // v.erase(0);
    }
}
