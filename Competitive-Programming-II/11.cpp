#include <iostream>
#include <vector>

using namespace std;

typedef long long ll;
#define INF 1000000000 
		
int main(){
	int T;
	cin>>T;
	while (T--){
		int n,m;
		cin>>n>>m;
		vector<vector<int>> graph(n,vector<int>(n,INF));
		for (int i = 0;i < n; i++)
			graph[i][i] = 0;

		while (m--){
			int ta,tb,tc;
			cin>>ta>>tb>>tc;
			ta--,tb--;
			if (tc < graph[ta][tb]) graph[ta][tb] = tc;
		}


		vector<vector<int>> sp(graph);
		for (int i = 0; i < n; i++){
			for (int u = 0; u < n; u++)
				for (int v = 0; v < n; v++)
					if(sp[u][i]!=INF &&  sp[i][v] != INF)
						if (sp[u][i] == -INF || sp[i][v] == -INF || sp[i][i] < 0)
							sp[u][v] = -INF;
						else if (sp[u][i] + sp[i][v] < sp[u][v]) 
							sp[u][v] = sp[u][i] + sp[i][v];
		}

		int ans1=0,ans2=0,ans3=0;
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++){
				if (sp[i][j] == INF) ans1++;
				else if (sp[i][j] == -INF) ans2++;
				else ans3++;
			}
		cout<<ans1<<' '<<ans2<<' '<<ans3<<endl;
	}
}