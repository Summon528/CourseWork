#include <iostream>
#include <vector>
#ifdef LOCAL
#include "stloutput.h"
#endif
#define INF 1000000000

using namespace std;

struct segtree {
    vector<long> vnd;
    long m;
    segtree(vector<long>& in, long mm) {  // 1-base
        m = mm;
        vnd.resize(in.size() * 10, 1);
        build(1, 1, in.size() - 1, in);
    }
    void build(int x, int L, int R, vector<long>& in) {
        if (L == R) {
            vnd[x] = in[L];
            return;
        }
        build(2 * x, L, (L + R) / 2, in);
        build(2 * x + 1, (L + R) / 2 + 1, R, in);
        vnd[x] = (vnd[2 * x] * vnd[2 * x + 1]) % m;
    }

    long query(int x, int L, int R, int askL, int askR) {
        if (askR < L || askL > R) return 1;
        if (askL <= L && askR >= R) return vnd[x];
        int mid = (L + R) / 2;
        return (query(2 * x, L, mid, askL, askR) *
                query(2 * x + 1, mid + 1, R, askL, askR)) %
               m;
    }
    void update(int x, int L, int R, int idx, int val) {
        if (idx < L || idx > R) return;
        if (idx == L && idx == R) {
            vnd[x] = val;
            return;
        }
        int mid = (L + R) / 2;
        update(2 * x, L, mid, idx, val);
        update(2 * x + 1, mid + 1, R, idx, val);
        vnd[x] = (vnd[2 * x] * vnd[2 * x + 1]) % m;
    }
};

long egcd(long a, long b, long& x, long& y) {
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    int r = egcd(b, a % b, x, y);
    int t = y;
    y = x - (a / b) * y;
    x = t;
    return r;
}

long inv(long a, long m) {
    long x, y, r = egcd(m, a, x, y);
    if (r != 1) return -1;
    return y < 0 ? y + m : y % m;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    long n, q, m;
    cin >> n >> q >> m;
    vector<long> v(n + 1);
    for (int i = 1; i <= n; i++) cin >> v[i];
    segtree seg(v, m);
    while (q--) {
        int a, b, c;
        cin >> a >> b >> c;
        if (a == 1) {
            cout << inv(seg.query(1, 1, n, b, c), m) << endl;
        } else {
            seg.update(1, 1, n, b, c);
        }
    }
}
