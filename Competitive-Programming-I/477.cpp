#include <ext/pb_ds/priority_queue.hpp>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m, i, j;
    string s;
    cin >> n >> m;
    vector<__gnu_pbds::priority_queue<int, greater<int>>> v(n);
    while (m--) {
        cin >> s >> i;
        i--;
        if (s == "insert") {
            cin >> j;
            v[i].push(j);
        } else if (s == "join") {
            cin >> j;
            j--;
            v[i].join(v[j]);
        } else if (s == "extract") {
            if (v[i].empty()) {
                cout << "NULL\n";
            } else {
                cout << v[i].top() << '\n';
                v[i].pop();
            }
        } else {
            v[i].empty() ? cout << "NULL\n" : cout << v[i].top() << '\n';
        }
    }
}
