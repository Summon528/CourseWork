#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

struct Num {
    string s;
    bool neg = 0;
    Num(string &x) {
        s = x;
        reverse(s.begin(), s.end());
        if (s.back() == '-') {
            neg = 1;
            s.pop_back();
        }
        s.resize(s.find_last_not_of('0') + 1);
    }
    Num() {}
};

void add(Num &a, Num &b) {
    if (a.s.length() < b.s.length()) swap(a, b);
    auto ra = a.s.begin(), rb = b.s.begin();
    int carry = 0;
    while (rb != b.s.end()) {
        *ra += *rb + carry - '0';
        carry = *ra > '9';
        *ra -= carry * 10;
        ra++, rb++;
    }
    while (ra != a.s.end()) {
        *ra += carry;
        carry = *ra > '9';
        *ra -= carry * 10;
        ra++;
    }
    if (carry) a.s += "1";
}

void sub(Num &a, Num &b) {
    auto ra = a.s.begin(), rb = b.s.begin();
    int carry = 0;
    while (ra != a.s.end() && rb != b.s.end()) {
        *ra -= *rb + carry - '0';
        carry = *ra < '0';
        *ra += carry * 10;
        ra++, rb++;
    }
    while (ra != a.s.end()) {
        *ra -= carry;
        carry = *ra < '0';
        *ra += carry * 10;
        ra++;
    }
}

bool lt(Num &a, Num &b) {
    if (a.s.length() == b.s.length()) {
        return a.s > b.s;
    }
    return a.s.length() > b.s.length();
}

void calc(Num &a, Num &b) {
    if (a.neg == b.neg) {
        add(a, b);
    } else {
        auto &neg = a.neg ? a : b;
        auto &pos = a.neg ? b : a;
        if (lt(pos, neg)) {
            sub(pos, neg);
        }
    }
}

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    string s1, s2;
    while (getline(cin, s1)) {
        Num ans;
        stringstream ss(s1);
        while (ss >> s2) {
            Num num(s2);
            calc(ans, num);
        }
        reverse(ans.s.begin(), ans.s.end());
        cout << ans.s << endl;
    }
}
