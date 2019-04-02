#include <algorithm>
#include <iostream>
#include <vector>
#ifdef LOCAL
#include "stloutput.h"
#endif
#define INF 1000000000

using namespace std;

void dfs(int x, vector<vector<bool>> &choice, string &path, vector<string> &v,
         int m) {
    if (x == m) {
        if (binary_search(v.begin(), v.end(), path)) return;
        cout << path << endl;
        exit(0);
    }
    for (int i = 0; i < 26; i++) {
        if (choice[x][i]) {
            path += i + 'a';
            dfs(x + 1, choice, path, v, m);
            path.pop_back();
        }
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m;
    cin >> n >> m;
    vector<string> v(n);
    for (int i = 0; i < n; i++) cin >> v[i];
    vector<vector<bool>> choice(m, vector<bool>(26));
    int sum = 1;
    for (int i = 0; i < m; i++) {
        int cnt = 0;
        for (int j = 0; j < n; j++) {
            if (!choice[i][v[j][i] - 'a']) {
                cnt++;
                choice[i][v[j][i] - 'a'] = 1;
            }
        }
        sum *= cnt;
    }
    if (sum == n) {
        cout << -1 << endl;
    } else {
        string path;
        sort(v.begin(), v.end());
        dfs(0, choice, path, v, m);
    }
}
