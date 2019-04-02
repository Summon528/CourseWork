#include <iostream>
#include <vector>
#include <queue>
#include <utility>
using namespace std;

typedef long long ll;
#define INF 1023456789

int main() {
	int T;
	cin >> T;
	while (T--) {
		int n, m, s, t;
		cin >> n >> m >> s >> t;
		s--, t--;
		vector<int> h(n);
		for (int i = 0; i < n; i++)
			cin >> h[i];
		vector<vector <int>> graph(n, vector<int>(n, INF));
		for (int i = 0; i < m; i++) {
			int u, v, w;
			cin >> u >> v >> w;
			u--, v--;
			if (graph[u][v] > h[u] - h[v] + w)
				graph[u][v] = h[u] - h[v] + w;
			if (graph[v][u] > h[v] - h[u] + w)
				graph[v][u] = h[v] - h[u] + w;
		}

		/*
				for (auto i : graph){
					for (auto j : i)
						cout<<j<<' ';
					cout<<endl;
				}
		*/
		vector<int> dist(n, INF);
		vector<bool> inq(n, false);
		queue<int> q;
		dist[t] = 0;
		q.push(t);
		while (!q.empty()) {
			inq[q.front()] = false;
			for (int i = 0; i < n; i++) {
				if (graph[q.front()][i] != INF &&
				        dist[i] > max(dist[q.front()] + graph[q.front()][i], 0)) {
					dist[i] = max(dist[q.front()] + graph[q.front()][i], 0);
					if (!inq[i]) q.push(i), inq[i] = true;
				}
			}
			//cout<<"q.front(): "<<q.front()<<endl;
			//for (auto i : dist)
			//		cout<<i<<' ';
			//	cout<<endl;
			q.pop();
		}
		//for (auto i : dist)
		//	cout<<i<<' ';
		//cout<<endl;
		cout << dist[s] << endl;
	}


}