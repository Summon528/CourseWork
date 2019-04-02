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
    int n, k;
    cin >> n >> k;
    vector<ll> v(n);
    for (int i = 0; i < n; i++) cin >> v[i];
    ll sum = accumulate(v.begin(), v.end(), 0);
    ll target = sum / 2;
    ll size = n - (n - k) / 2;

    vector<vector<ll>> dp(size + 1, vector<ll>(target + 1, -INF));
    for (int i = 0; i <= target; i++) dp[0][i] = 0;

    for (int i = 0; i < n; i++) {
        for (int j = target; j >= v[i]; j--) {
            for (int k = 1; k <= size; k++) {
                dp[k][j] = max(dp[k][j], dp[k - 1][j - v[i]] + v[i]);
            }
        }
    }

    ll Max = -INF;
    for (int i = (n - k) / 2; i <= size; i++) {
        Max = max(Max, dp[i][target]);
    }

    cout << sum - 2 * Max << endl;
}
