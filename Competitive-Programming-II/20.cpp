#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#ifdef LOCAL
#include "stloutput.h"
#endif
#define INF 1000000000
#define EPS 1E-9

using namespace std;

double area(pair<double, double> a, pair<double, double> b,
            pair<double, double> c) {
    return abs(
        ((a.first * b.second + b.first * c.second + c.first * a.second) -
         (b.first * a.second + c.first * b.second + a.first * c.second)) *
        .5);
}

int main() {
    cin.sync_with_stdio(0);
    cin.tie(0);
    int T;
    cin >> T;
    while (T--) {
        int N;
        cin >> N;
        vector<pair<double, double>> vp;
        for (int i = 0; i < N; i++) {
            double x, y;
            cin >> x >> y;
            vp.push_back(make_pair(x, y));
        }
        double ans = 0;
        auto _next = [N](int x) { return (x + 1) % N; };
        for (int a = 0; a < N; a++) {
            int b = _next(a);
            int c = _next(b);
            while (1) {
                while (area(vp[a], vp[b], vp[_next(c)]) >=
                       area(vp[a], vp[b], vp[c]))
                    c = _next(c);
                if (area(vp[a], vp[_next(b)], vp[c]) >=
                    area(vp[a], vp[b], vp[c]))
                    b = _next(b);
                else
                    break;
            }
            ans = max(ans, area(vp[a], vp[c], vp[b]));
        }
        cout << setprecision(10) << fixed << ans << endl;
    }
}
