#include <iostream>
#include <queue>
#include <utility>
#include <vector>
#ifdef LOCAL
#include "stloutput.h"
#endif
#define INF 1000000000

using namespace std;

inline int to_idx(int x, int y, int m) { return (x - 1) * m + y; }

inline void add_edge(int u, int v, int w,
                     vector<vector<pair<int, int>>> &graph) {
    graph[u].push_back(make_pair(v, w));
    graph[v].push_back(make_pair(u, 0));
}

int main() {
    int T;
    cin >> T;
    while (T--) {
        int n, m;
        cin >> n >> m;
        cin.ignore();
        vector<vector<char>> matrix(n + 2, vector<char>(m + 2));

        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++) {
                char tmp = cin.get();
                if (tmp == '\n') tmp = cin.get();
                matrix[i][j] = tmp;
            }

        int source = 0, sink = m * n + 1;
        vector<vector<pair<int, int>>> graph(m * n + 2,
                                             vector<pair<int, int>>());

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                int type, dx[4] = {-1, 0, 1, 0}, dy[4] = {0, -1, 0, 1};
                if (matrix[i][j] == 'H') {
                    add_edge(source, to_idx(i, j, m), 1, graph);
                    for (int k = 0; k < 4; k++)
                        if (matrix[i + dx[k]][j + dy[k]] == 'B')
                            add_edge(to_idx(i, j, m),
                                     to_idx(i + dx[k], j + dy[k], m), 1, graph);
                } else if (matrix[i][j] == 'B') {
                    for (int k = 0; k < 4; k++)
                        if (matrix[i + dx[k]][j + dy[k]] == 'T')
                            add_edge(to_idx(i, j, m),
                                     to_idx(i + dx[k], j + dy[k], m), 1, graph);
                } else
                    add_edge(to_idx(i, j, m), sink, 1, graph);
            }
        }
        // cout << graph << endl;

        int ans = 0;
        while (true) {
            queue<int> q;
            q.push(source);
            vector<pair<int, int>> pre(n * m + 2, make_pair(-1, -1));
            while (!q.empty()) {
                int x = q.front();
                q.pop();
                for (int i = 0; i < graph[x].size(); i++) {
                    if (pre[graph[x][i].first].first == -1 &&
                        graph[x][i].second > 0 && pre[sink].first == -1) {
                        pre[graph[x][i].first] =
                            make_pair(x, graph[x][i].second);
                        q.push(graph[x][i].first);
                    }
                }
            }
            // cout << "pre:" << pre << endl;
            if (pre[sink].first != -1) {
                int Min = INF;
                for (int i = 0; i < pre.size(); i++) {
                    if (pre[i].first != -1 && Min > pre[i].second)
                        Min = pre[i].second;
                }
                // cout << "Min:" << Min << endl;
                int pos = sink;
                while (pos != source) {
                    for (auto &&j : graph[pre[pos].first])
                        if (j.first == pos) j.second -= Min;
                    for (auto &&j : graph[pos])
                        if (j.first == pre[pos].first) j.second += Min;
                    pos = pre[pos].first;
                }
                // cout << "graph:" << graph << endl;
                ans += Min;
            } else
                break;
        }
        cout << ans << endl;
    }
}
