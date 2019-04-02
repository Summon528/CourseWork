#include <iostream>
#include <vector>
#ifdef LOCAL
#include "stloutput.h"
#endif
#define INF 1000000000

using namespace std;

void add(vector<long long> &a, vector<long long> &b) {
    for (long long i = 0; i < a.size(); i++) a[i] += b[i];
}
inline long long safe(vector<long long> &v, long long x) {
    return x >= v.size() || x < 0 ? 0 : v[x];
}

vector<long long> multi(vector<long long>::iterator st,
                        vector<long long>::iterator ed) {
    long long size = ed - st;
    if (size <= 32) {
        vector<long long> v(size * 2);
        for (long long i = 0; i < size; i++) {
            for (long long j = 0; j < size; j++) {
                v[i + j] += *(st + i) * *(st + j);
            }
        }
        return v;
    }
    long long mid = (size + 1) / 2;
    auto y = multi(st, st + mid);
    auto z = multi(st + mid, ed);
    vector<long long> add(mid);
    for (long long i = 0; i < mid; i++) {
        add[i] = *(st + i) + *(st + mid + i);
    }
    auto x = multi(add.begin(), add.end());
    vector<long long> ans(size * 2);
    for (long long i = 0; i < ans.size(); i++) {
        long long t1;
        ans[i] = safe(y, i) + safe(x, i - mid) - safe(y, i - mid) -
                 safe(z, i - mid) + safe(z, i - 2 * mid);
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    long long n, m, tmp;
    cin >> n >> m;
    vector<long long> v(1 << 17);
    for (long long i = 0; i < n; i++) {
        cin >> tmp;
        v[tmp]++;
    }
    auto ans = multi(v.begin(), v.end());
    for (long long i = 0; i < m; i++) {
        cin >> tmp;
        if (tmp >= ans.size())
            cout << 0 << '\n';
        else
            cout << (tmp % 2 ? ans[tmp] : ans[tmp] - v[tmp / 2]) / 2 << '\n';
    }
}
