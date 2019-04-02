#include <stdlib.h>
#include <stdio.h>
#include <string.h>

long long magic[1048577];

int main(){
	int T;
	scanf("%d",&T);
	while (T--){
		int n,m,x,y,block[21];
		scanf("%d %d",&n,&m);
		memset (magic,0,1 << (n+3));
		memset (block,0,84);
		while (m--){
			scanf("%d %d",&x,&y);
			block[x] |= (1 << y-1);
		}
		int max = (1 << n) -1;
		unsigned int i;
		magic[0] = 1;
		for (i = 1; i <= max; i++){
			int temp = i^block[__builtin_popcount(i)];
			while (temp){
				int temp2 = temp&(-temp);
				magic[i] += magic[temp2 ^ i];
				temp^=temp2;
			}
		}
		printf("%lld\n",magic[max]);
	}
	return 0;
}