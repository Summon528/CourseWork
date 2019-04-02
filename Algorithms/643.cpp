#include <bits/stdc++.h>

using namespace std;

int main() {
    char c;
    vector<pair<int, int>> vp(100);
    while (c = cin.get(), c != '\r' && c != '\n' && c != -1) {
        vp[c].first--;
        vp[c].second = c;
    }
    sort(vp.begin(), vp.end());
    for (auto &i : vp) {
        if (!i.first) continue;
        cout << i.second << ' ' << -i.first << endl;
    }
}
