#include <iostream>
using namespace std;

int co[10001][10001]={};

int main(){
	co[0][1] = 1;
	for (int i = 1; i < 10001; i++){
		for (int j = 1; j < 10001; j++){
			co[i][j] = (co[i-1][j] + co[i-1][j-1]) %1000000007;
		}
	}
	int T;
	cin>>T;
	while (T--){
		long long int a,b,c,d,ans=1,M;
		cin>>a>>b>>M;
		if (b>a)
			cout<<0<<endl;
		else{
			stack <int> st1,st2;
			while (1){
	            c = a % M;
	            d = b % M;
	            ans = (ans * co[c][d+1]) %1000000007;
	            a /= M;
	            b /= M;
	            if (a <= 0)
	                break;
			}
		cout<<ans<<endl;
		}
	}
}