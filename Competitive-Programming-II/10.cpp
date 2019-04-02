#include <iostream>
#include <vector>
#include <queue>
using namespace std;

typedef long long ll;
#define INF 1000000000 

int kanpsack(int n, int w, vector<pair <int,int>>& items, vector<vector<int>>& dp){
	if (w < 0 || n < 0) return -INF;
	if (n == 0) return 0;
	if (dp[n][w] == -1)
		dp[n][w] = max((kanpsack(n-1,w-items[n-1].first,items,dp)+items[n-1].first),(kanpsack(n-1,w-items[n-1].second,items,dp)+items[n-1].second));
	return dp[n][w];
}

int main(){
	int T;
	cin>>T;
	while (T--)	{
		int n,m;
		cin>>n>>m;
		vector<vector<int>> edge(n,vector<int>());

		for (int i = 0; i < m; ++i){
			int a,b;
			cin>>a>>b;
			edge[a-1].push_back(b-1);
			edge[b-1].push_back(a-1);
		}

		vector<int> distance(n,-1);
		queue<int> q;
		vector<pair <int,int>> items;
		for (int i = 0; i < n; ++i)	{
			if (distance[i] == -1){
				int even = 0,odd = 0;
				q.push(i);
				distance[i] = 0;
				even++;
				while (!q.empty()){
					for (auto&& j:edge[q.front()]){
						if (distance[j] == -1){
							distance[j] = distance[q.front()] + 1;
							if ((distance[j] +1) % 2) even++;
							else odd++;
							q.push(j);
						}
					}
					q.pop();
				}
			items.push_back(make_pair(odd,even));
			}
		}

		int ans = 0;
		for (int i = 0; i < n; ++i)	{
			for(auto&& j : edge[i]) {
				if (distance[i] == distance[j]){
					ans = -1;
					break;
				}
			}
		}

		if (ans == -1)
			cout<<-1<<endl;
		else{
			vector<vector<int>> dp(items.size()+1,vector<int>(n/2+1,-1));
			cout<<n-kanpsack(items.size(),n/2,items,dp)<<endl;
		}
	}
}