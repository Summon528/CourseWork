#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

typedef long long ll;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<int> vi;

#define INF 1000000000 

int main(){
	int T;
	cin>>T;
	while (T--){
		unsigned int n,a,b;
		vector< pair <unsigned int,bool> > v;
		cin>>n;
		while (n--){
			cin>>a>>b;
			v.push_back(make_pair(a,0));
			v.push_back(make_pair(b,1));
		}
		int cnt = 0,max = -1;
		sort(v.begin(), v.end());
		for(auto&& i : v) {
			if (i.second == 0) {
				cnt++;
				if (cnt > max) max = cnt;
			}
			else cnt--;
		}
		cout<<max<<endl;
	}
}