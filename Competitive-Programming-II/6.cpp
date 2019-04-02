#include <iostream>

using namespace std;

int main(){
	int T;
	cin>>T;
	while (T--){
		int a[204]={},n,b;
		cin>>n;
		while (n--){
			cin>>b;
			a[b]++;
		}
		int cnt = 0,ans = 0;
		for (int i = 2; i <= 203; i++){
			if (a[i] != 0) cnt++;
			else ans += (cnt+1)/2,cnt = 0;
		}
		cout<<ans<<endl;
	}
}
