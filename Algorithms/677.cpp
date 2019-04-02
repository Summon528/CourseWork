#include <iostream>
#include <vector>
#define INF 1000000000
using namespace std;

bool dfs(int i, int j, vector<vector<char>> &v, char c,
         vector<vector<int>> &mem) {
    if (mem[i][j] != -1) return mem[i][j];
    if (i == v.size() - 2 && j == v.size() - 2) {
        return mem[i][j] = v[i][j] == c || v[i][j] == v[1][1];
    }
    return mem[i][j] = (v[i + 1][j] == c || v[i + 1][j] == v[1][1]) &&
                           dfs(i + 1, j, v, c, mem) ||
                       (v[i][j + 1] == c || v[i][j + 1] == v[1][1]) &&
                           dfs(i, j + 1, v, c, mem);
}

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    int n;
    cin >> n;
    vector<vector<char>> v(n + 2, vector<char>(n + 2, '*'));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> v[i][j];
        }
    }

    for (char c = 'a'; c <= 'z'; c++) {
        vector<vector<int>> mem(n + 2, vector<int>(n + 2, -1));
        if (dfs(1, 1, v, c, mem)) {
            cout << "Yes" << endl;
            return 0;
        }
    }
    cout << "No" << endl;
}
