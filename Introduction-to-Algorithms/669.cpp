#include <iostream>
#include <vector>
#define MOD 1000000007
#ifdef LOCAL
#include "stloutput.h"
#endif
#define INF 1000000000

using namespace std;
using ll = long long;
ll dp[5005][5005][2];
int main() {
    ll ans[5005] = {};
    ans[1] = 1;
    dp[1][1][0] = 1;
    for (int i = 2; i <= 5000; i++) {
        for (int j = 1; j <= 5000; j++) {
            ll &b1 = dp[i - 1][j - 1][0];
            ll &b2 = dp[i - 2][j - 1][0];
            ll &r1 = dp[i - 1][j - 1][1];
            ll &r2 = dp[i - 2][j - 1][1];
            ll &b10 = dp[i - 1][j][0];
            ll &b20 = dp[i - 2][j][0];
            dp[i][j][0] = ((b1 + r1) % MOD * (b1 + r1) % MOD) % MOD +
                          ((b1 + r1) % MOD * (b2 + r2) % MOD) * 2 % MOD;
            dp[i][j][1] = (b10 % MOD * (b10 + b20 * 2) % MOD) % MOD;
            ans[i] = (ans[i] + dp[i][j][0] % MOD + dp[i][j][1] % MOD) % MOD;
        }
        for (int j = 1; j <= 5000; j++) {
            for (int k = 0; k < 2; k++) {
                dp[i - 1][j][k] = (dp[i - 1][j][k] + dp[i - 2][j][k]) % MOD;
            }
        }
    }
    int n;
    cin >> n;
    while (n--) {
        int x;
        cin >> x;
        cout << ans[x] << endl;
    }
}
