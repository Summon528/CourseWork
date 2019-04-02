#include <iostream>
#include <vector>
#define INF 1000000000
using namespace std;
using ll = long long;

ll solve(int i, int j, vector<vector<ll>> &v, vector<vector<ll>> &dp) {
    if (dp[i][j] != -INF) return dp[i][j];
    if (i == 1) {
        return dp[i][j] =
                   max(abs(v[i][j] - v[i][j - 1]), solve(i, j - 1, v, dp));
    }
    if (j == 1) {
        return dp[i][j] =
                   max(abs(v[i - 1][j] - v[i][j]), solve(i - 1, j, v, dp));
    }
    return dp[i][j] =
               min(max(abs(v[i][j] - v[i][j - 1]), solve(i, j - 1, v, dp)),
                   max(abs(v[i - 1][j] - v[i][j]), solve(i - 1, j, v, dp)));
}

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    int n;
    cin >> n;
    vector<vector<ll>> v(n + 1, vector<ll>(n + 1));
    vector<vector<ll>> dp(n + 1, vector<ll>(n + 1, -INF));
    dp[1][1] = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> v[i][j];
        }
    }
    cout << solve(n, n, v, dp) << endl;
}
