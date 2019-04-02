#include <bits/stdc++.h>

using namespace std;
int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    string s;
    while (getline(cin, s)) {
        stringstream ss(s);
        int tmp;
        vector<int> v;
        while (ss >> tmp) v.push_back(tmp);
        reverse(v.begin(), v.end());
        int ans = 1;
        for (int i : v) {
            int dead = ans * 2;
            int eat = max(i + 1, ans - (i / 2 + i % 2));
            int go = max(ans, i);
            ans = min(dead, min(eat, go));
        }
        cout << ans << endl;
    }
}
