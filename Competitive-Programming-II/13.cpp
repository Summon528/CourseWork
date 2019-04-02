#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

typedef long long ll;
#define INF 1000000000

class diset {
  private:
    vector<int> v;

  public:
    diset(int size) { v.resize(size, -1); }

    int _find(int x) { return v[x] < 0 ? x : v[x] = _find(v[x]); }

    void _union(int x, int y) {
        int a = _find(x);
        int b = _find(y);
        if (v[b] < v[a])
            swap(b, a);
        v[a] += v[b];
        v[b] = a;
    }
};

class edge {
  public:
    int u, v, w;
    bool used;
    edge() { u = -1, v = -1, w = 0, used = false; }
    edge(int x, int y, int z) {
        u = x;
        v = y;
        w = z;
        used = false;
    }
    bool operator<(const edge &other) { return this->w < other.w; }
};

void dfs(int from, int x, int d, int w, vector<vector<edge>> &mst,
         vector<bool> &visited, vector<int> &depth, vector<vector<int>> &parent,
         vector<vector<int>> &longest) {
    // cout << "x: " << x << endl;
    visited[x] = true;
    depth[x] = d;
    parent[x][0] = from;
    longest[x][0] = w;
    for (edge &i : mst[x])
        if (!visited[i.v])
            dfs(x, i.v, d + 1, i.w, mst, visited, depth, parent, longest);
}

int main() {
    ios::sync_with_stdio(0);
    int T;
    cin >> T;
    while (T--) {
        int N, M;
        cin >> N >> M;
        vector<edge> elist;
        while (M--) {
            int t1 = 0, t2 = 0, t3 = 0;
            cin >> t1 >> t2 >> t3;
            elist.push_back(edge(t1, t2, t3));
        }

        // kruskal
        sort(elist.begin(), elist.end());
        vector<vector<edge>> mst(N);
        diset dset(N);
        int cnt = 0, mst_len = 0;
        for (edge &i : elist) {
            if (dset._find(i.u) != dset._find(i.v)) {
                i.used = true;
                dset._union(i.u, i.v);
                cnt++;
                mst[i.u].push_back(edge(i.u, i.v, i.w));
                mst[i.v].push_back(edge(i.v, i.u, i.w));
                mst_len += i.w;
            }
            if (cnt == N - 1)
                break;
        }

        // dfs
        vector<int> depth(N);
        vector<vector<int>> parent(N, vector<int>(20)),
            longest(N, vector<int>(20));
        vector<bool> visited(N, false);
        depth.resize(N);

        dfs(0, 0, 0, 0, mst, visited, depth, parent, longest);

        // LCA binary lift
        for (int j = 1; j < 20; j++) {
            for (int i = 0; i < N; i++) {
                if (parent[i][j - 1] != -1) {
                    parent[i][j] = parent[parent[i][j - 1]][j - 1];
                    longest[i][j] = max(longest[i][j - 1],
                                        longest[parent[i][j - 1]][j - 1]);
                }
            }
        }

        // edge not in mst
        int ans = -INF;
        for (edge &i : elist) {
            if (i.used == false) {
                int u = i.u, v = i.v, maxl = -INF;
                if (depth[u] < depth[v])
                    swap(u, v);
                // cout<<"u: "<<u<<endl;
                // cout<<"v: "<<v<<endl;
                for (int j = 19; j >= 0; j--)
                    if (depth[parent[u][j]] >= depth[v]) {
                        maxl = max(maxl, longest[u][j]);
                        u = parent[u][j];
                    }
                if (u == v) {
                    // cout<<"maxl: "<<maxl<<endl;
                    ans == -INF ? ans = -maxl + i.w
                                : ans = min(-maxl + i.w, ans);
                    continue;
                }
                for (int j = 19; j >= 0; j--) {
                    if (parent[u][j] != parent[v][j]) {
                        maxl = max(maxl, max(longest[u][j], longest[v][j]));
                        u = parent[u][j], v = parent[v][j];
                    }
                }
                maxl = max(maxl, max(longest[u][0], longest[v][0]));
                ans == -INF ? ans = -maxl + i.w : ans = min(-maxl + i.w, ans);
            }
        }
        ans == -INF ? cout << 0 << endl : cout << ans << endl;
    }
}
