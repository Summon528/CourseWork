#include <iostream>
#include <vector>

#define M 1000000007
using namespace std;

typedef long long ll;

int solve(int x, int state, vector<ll> &dp, vector<vector<ll>> &cake) {
    if (!state) return 1;
    if (~dp[state]) return dp[state];
    dp[state] = 0;
    for (auto i : cake[x - 1]) {
        int tmp = 1 << (i - 1);
        if (!(tmp & state)) continue;
        dp[state] = (dp[state] + solve(x - 1, state ^ tmp, dp, cake)) % M;
    }
    return dp[state];
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    ll n, m;
    cin >> n >> m;
    vector<vector<ll>> cake(n, vector<ll>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) cin >> cake[i][j];
    }
    vector<ll> dp(1 << n, -1);
    cout << solve(n, (1 << n) - 1, dp, cake) << endl;
}
