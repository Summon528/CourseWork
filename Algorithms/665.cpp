#include <iostream>
#include <unordered_set>

using namespace std;

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m;
    string s;
    cin >> n >> m;
    unordered_set<string> us;
    while (n--) {
        cin >> s;
        if (us.find(s) != us.end()) {
            cout << s << '\n';
            return 0;
        }
        us.insert(s);
    }
}
