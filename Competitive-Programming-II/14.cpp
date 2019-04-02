#include <cstdio>
#include <iostream>
using namespace std;

typedef long long ll;
#define INF 1000000000


int v[20010];

int _find(int x) {
	return v[x] == 0 ? x : v[x] = _find(v[x]);
}

int _union(int x, int y) {
	int a = _find(x);
	int b = _find(y);
	if (a != b) v[a] = b;
}

int main() {
	int T;
	scanf("%d", &T);
	for (int t = 1; t <= T; t++) {

		int N, K, tmp1, tmp2;
		char c;
		scanf("%d %d", &N, &K);

		for (int i = 1; i <= N; i++)
			scanf("%d", &v[i]);
		c = getchar();


		int st[5010][3];
		int Q1 = 0;
		bool cut[20010] = {};
		for (int i = 0; i < K; i++) {
			char c;
			c = getchar();
			if (c == 'Q') {
				scanf("%d%d", &tmp1, &tmp2);
				st[Q1][0] = c, st[Q1][1] = tmp1, st[Q1][2] = tmp2, Q1++;
			}
			else if (c == 'C') {
				scanf("%d", &tmp1);
				if (v[tmp1]) st[Q1][0] = c, st[Q1][1] = v[tmp1], st[Q1][2] = tmp1, Q1++;
				cut[tmp1] = true;
				v[tmp1] = 0;
			}
			c = getchar();
		}

		bool ans[5010] = {};
		int Q2 = 0;
		for (int i = Q1 - 1; i >= 0; i--) {
			if (st[i][0] == 'Q') {
				if (_find(st[i][1]) == _find(st[i][2])) ans[Q2] = 1;
				Q2++;
			}
			else
				_union(st[i][1], st[i][2]);
		}

		printf("Case #%d:\n", t);
		for (int i = Q2 - 1 ; i >= 0 ; i--)
			ans[i] ? puts("YES") : puts("NO");
	}
}
