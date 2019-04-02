#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

typedef long long ll;
#define INF 1000000000 

int dfs(int vertex,vector<vector <int>>& v, 
	vector<bool>& discovered, int &length){
	if (!discovered[vertex]) length++;
	discovered[vertex] = true;
	for (int &i:v[vertex])
		if (!discovered[i])
			dfs(i,v,discovered,length);
}

int main(){
	ios_base::sync_with_stdio(0);
	int T;
	cin>>T;
	while (T--){
		int n,tmp;
		cin>>n;
		vector<vector <int>> v(n,vector<int>());
		int superman = -1,superman_pos;
		for (int i = 0; i < n; i++){
			int cnt = 0;
			for (int j = 0; j < n; j++){
				cin>>tmp;
				if (tmp == 1){
					v[j].push_back(i);
					cnt++;
				}
			}
			if (cnt > superman)
				superman = cnt, superman_pos = i;
		}

		vector<bool> discovered(n,0);
		int length = 0;
		dfs(superman_pos,v,discovered,length);
		cout<<length<<endl;
	}
}