#include <iostream>
#include <vector>
#ifdef LOCAL
#include "stloutput.h"
#endif
#define INF (int)1e9 + 7
#define MAXX 1 << 15

using namespace std;

int main() {
    cout << "INF:" << INF << endl;
    vector<bool> not_prime(MAXX + 1);
    for (int i = 2; i <= MAXX; i++) {
        if (!not_prime[i]) {
            for (int j = i * 2; j <= MAXX; j += i) {
                not_prime[j] = true;
            }
        }
    }

    vector<int> prime;
    for (int i = 2; i <= MAXX; i++) {
        if (!not_prime[i]) prime.push_back(i);
    }

    vector<int> ans(MAXX + 1);
    ans[0] = 1;
    for (int i = 0; i < prime.size(); i++) {
        for (int j = prime[i]; j <= MAXX; j++) {
            ans[j] = (ans[j - prime[i]] + ans[j]) % INF;
        }
    }

    int T;
    cin >> T;
    while (T--) {
        int a;
        cin >> a;
        cout << ans[a] << endl;
    }
}
