#include <iostream>
#include <string>

using namespace std;

bool check(string &s, string &t) {
    if (s.length() % 2) {
        return s == t;
    }
    int mid = s.length() / 2;
    string s1 = s.substr(0, mid);
    string s2 = s.substr(mid, mid);
    string t1 = t.substr(0, mid);
    string t2 = t.substr(mid, mid);
    if (check(s1, t1) && check(s2, t2) || check(s1, t2) && check(s2, t1)) {
        return true;
    } else {
        return false;
    }
}

int main() {
    string s, t;
    cin >> s >> t;
    if (s.length() != t.length()) {
        cout << "NO" << endl;
        return 0;
    }
    check(s, t) ? cout << "YES" << endl : cout << "NO" << endl;
}
