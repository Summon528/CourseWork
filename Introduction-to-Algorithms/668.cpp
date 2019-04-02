#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;
int main() {
    string tmp, s, rs;
    cin >> tmp;
    int n = tmp.length();
    s = " " + tmp;
    reverse(tmp.begin(), tmp.end());
    rs = " " + tmp;
    vector<vector<int>> dp(2, vector<int>(n + 1));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (s[i] == rs[j]) {
                dp[i % 2][j] = dp[(i % 2) ^ 1][j - 1] + 1;
            } else {
                dp[i % 2][j] = max(dp[(i % 2) ^ 1][j], dp[i % 2][j - 1]);
            }
        }
        cout << dp[i % 2][n - i + 1] * 2 - 1 << ' ';
    }
    cout << endl;
}
