#include <iostream>
#include <vector>
#define INF 1000000000
using namespace std;
using ll = long long;

bool dfs(int i, int j, vector<vector<ll>> &v, int d, vector<vector<int>> &mem) {
    if (mem[i][j] != -1) return mem[i][j];
    if (i == v.size() - 2 && j == v.size() - 2) return mem[i][j] = true;
    return mem[i][j] = v[i + 1][j] - v[i][j] == d && dfs(i + 1, j, v, d, mem) ||
                       v[i][j + 1] - v[i][j] == d && dfs(i, j + 1, v, d, mem);
}

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    int n;
    cin >> n;
    vector<vector<ll>> v(n + 2, vector<ll>(n + 2, INF));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> v[i][j];
        }
    }

    vector<vector<int>> mem1(n + 2, vector<int>(n + 2, -1));
    vector<vector<int>> mem2(n + 2, vector<int>(n + 2, -1));
    dfs(1, 1, v, v[2][1] - v[1][1], mem1) ||
            dfs(1, 1, v, v[1][2] - v[1][1], mem2)
        ? cout << "Yes" << endl
        : cout << "No" << endl;
}
