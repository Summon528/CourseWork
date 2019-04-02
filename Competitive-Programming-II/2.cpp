#include <iostream>
#include <vector>
#include <utility>
#include <stack>
#include <queue>

using namespace std;

typedef long long ll;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<int> vi;

#define INF 1000000000 
		
int main(){
	int t;
	cin>>t;
	while (t--){
		int n;
		cin>>n;
		queue <pair <char, int> > q;
		stack <pair <char, int> > st;
		while (n--){
			char a;
			int b;
			cin>>a>>b;
			q.push(make_pair(a,b));
		}
		while (!q.empty()){
			char ch;
			int cnt;
			ch = q.front().first;
			cnt = q.front().second;			q.pop();
			while (!st.empty() && st.top().first == ch){
				cnt += st.top().second;
				st.pop();
			}
			while (!q.empty() && q.front().first == ch){
				cnt += q.front().second;
				q.pop();
			}

			int a = cnt / 3;
			int b = cnt % 3;
			
			if (a != 0) {
				q.push(make_pair(ch,a));
				//cout<<"push to queue"<<ch<<','<<a<<endl;
			}
			if (b != 0){
				//cout<<"2push to stack"<<ch<<','<<b<<endl;
				st.push(make_pair(ch,b));
			}
		}
		int ans = 0;
		while(!st.empty()){
			ans+=st.top().second;
			st.pop();
		}
		cout<<ans<<endl;
	}
}