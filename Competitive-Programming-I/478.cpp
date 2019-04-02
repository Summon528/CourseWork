#include <algorithm>
#include <iostream>
#include <vector>
#ifdef LOCAL
#include "stloutput.h"
#endif
#define INF 1000000000

using namespace std;

inline long lowbit(int x) { return x & (-x); }
long prefix(vector<long>& v, int x) {
    long sum = 0;
    for (long i = x; i > 0; i -= lowbit(i)) sum += v[i];
    return sum;
}
void update(vector<long>& v, long x, long val) {
    for (int i = x; i <= v.size(); i += lowbit(i)) v[i] += val;
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int T, n;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        vector<pair<long, int>> v(n + 1), s(n + 1);
        long all = 0, Min = INF, ans = 0, done = 0;
        for (int i = 1; i <= n; i++) {
            long tmp;
            cin >> tmp;
            s[i].first = v[i].first = tmp;
            s[i].second = i;
            all += tmp;
        }
        sort(begin(s), end(s));
        for (int i = 1; i <= n; i++) v[s[i].second].second = i;
        vector<long> bit(n + 1);
        for (int i = 1; i <= n; i++) bit[i] = s[i].first;
        for (int x = 1; x <= bit.size(); ++x) {
            long y = x - lowbit(x);
            for (int i = x - 1; i > y; i -= lowbit(i)) bit[x] += bit[i];
        }

        for (int i = n; i > 1; i--) {
            all -= v[i].first;
            if (v[i].first >= Min) {
                done -= v[i].first - Min;
                continue;
            }
            long pre = all - prefix(bit, v[i].second - 1) - done;
            int len = i - v[i].second;
            update(bit, v[i].second, -v[i].first);
            long tmp = pre - len * v[i].first;
            ans += tmp * v[i].first;
            done += tmp;
            Min = v[i].first;
        }
        cout << ans << '\n';
    }
}
