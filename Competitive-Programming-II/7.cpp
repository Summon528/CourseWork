#include <iostream>
#include <vector>
#include <utility>
#include <queue>

using namespace std;

typedef long long ll;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<int> vi;

#define INF 1000000000 
		
int main(){
	int T;
	cin>>T;
	cin.ignore();
	while (T--){
		int idx=0;
		queue<int> q[4];
		char c;
		
		while (c = cin.get(), c != '\n'){
			if (c == 'L') q[0].push(idx);
			else if (c == 'O') q[1].push(idx);
			else if (c == 'V') q[2].push(idx);
			else if (c == 'E') q[3].push(idx);
			idx ++;
		}
		int now,ans = 0;
		bool end = false;
		while (!q[0].empty()){
			now=q[0].front();
			q[0].pop();
			for (int i = 1; i < 4; i++){
				if(q[i].empty())  {
					end = true;
					break;
				}
				while (q[i].front() < now){
					q[i].pop();
					if(q[i].empty()){
						end = true;
						break;
					}
				}
				if (end) break;
				now = q[i].front();
				q[i].pop();
				
			}
			if(end) break;
			ans++;
		}
		cout<<ans<<endl;
	}
}
