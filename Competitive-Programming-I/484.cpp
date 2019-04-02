#include <iostream>
#include <numeric>
#include <vector>
#ifdef LOCAL
#include "stloutput.h"
#endif
#define INF 1000000000

typedef long long ll;
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<ll> v(n);
    for (int i = 0; i < n; i++) cin >> v[i];
    ll sum = accumulate(v.begin(), v.end(), 0);
    ll target = sum / 2;
    vector<ll> dp(target + 1);
    for (int i = 0; i < n; i++) {
        for (int j = target; j >= v[i]; j--) {
            dp[j] = max(dp[j], dp[j - v[i]] + v[i]);
        }
    }
    cout << sum - dp[target] - dp[target] << endl;
}
