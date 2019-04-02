#include <iostream>
#include <vector>
#ifdef LOCAL
#include "stloutput.h"
#endif
#define INF 1000000000
#define M 1000000007
using namespace std;

typedef long long ll;

ll n, m;
int solve(int x, int state, vector<vector<ll>> &dp, vector<vector<ll>> &cake,
          vector<int> &path) {
    if (x == 1 && dp[x][state] != 0) {
        int f = __lg(state) + 1;
        for (int i = 0; i < m; i++) {
            if (cake[0][i] == f) {
                path.push_back(i + 1);
                break;
            }
        }
        for (auto it = path.rbegin(); it != path.rend(); it++)
            cout << *it << ' ';
        cout << endl;
        exit(0);
    }
    if (dp[x][state] != -1) return dp[x][state];
    dp[x][state] = 0;
    for (int i = 0; i < m; i++) {
        int tmp = 1 << (cake[x - 1][i] - 1);
        if ((tmp & state) == 0) continue;
        path.push_back(i + 1);
        dp[x][state] =
            (dp[x][state] + solve(x - 1, state ^ tmp, dp, cake, path)) % M;
        path.pop_back();
    }
    return dp[x][state];
}

int main() {
    cin >> n >> m;
    vector<vector<ll>> cake(n, vector<ll>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) cin >> cake[i][j];
    }

    vector<vector<ll>> dp(n + 1, vector<ll>(1 << n, -1));

    for (int i = 0; i < (1 << n); i++) dp[1][i] = 0;

    for (int i = 0; i < m; i++) {
        dp[1][1 << (cake[0][i] - 1)] += 1;
    }

    vector<int> path;
    cout << solve(n, (1 << n) - 1, dp, cake, path) << endl;
}
