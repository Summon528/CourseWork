#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
#ifdef LOCAL
#include "stloutput.h"
#endif
#define INF 1000000000

using namespace std;

void add_edge(int a, int b, vector<vector<int>> &graph) {
    graph[a].push_back(b);
    graph[b].push_back(a);
}

bool dfs(int x, vector<int> &inpath, vector<int> &match,
         vector<vector<int>> &graph) {
    for (auto i : graph[x]) {
        if (!inpath[i]) {
            inpath[i] = true;
            if (match[i] == -1 || dfs(match[i], inpath, match, graph)) {
                match[x] = i;
                match[i] = x;
                return true;
            }
        }
    }
    return false;
}

int main() {
    int T;
    cin >> T;
    while (T--) {
        map<int, int> circle_map;
        map<pair<int, int>, int> line_map;
        vector<vector<int>> graph(50000, vector<int>());
        int cnt = 0, n;
        cin >> n;
        for (int i = 0; i < n; i++) {
            int r, s, t;
            cin >> r >> s >> t;
            if (circle_map.count(r) == 0) circle_map[r] = cnt++;
            int GCD = __gcd(s, t);
            s /= GCD, t /= GCD;
            if (line_map.count(make_pair(s, t)) == 0)
                line_map[make_pair(s, t)] = cnt++;
            add_edge(circle_map[r], line_map[make_pair(s, t)], graph);
        }

        int ans = 0;
        vector<int> match(2 * n, -1);
        vector<int> inpath;
        for (auto &i : circle_map) {
            inpath.clear();
            inpath.resize(2 * n, 0);
            if (dfs(i.second, inpath, match, graph)) ans++;
        }
        cout << ans << endl;
    }
}
