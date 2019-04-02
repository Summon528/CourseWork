#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
struct segtree {
    struct node {
        ll val, lazy;
    };
    vector<node> vnd;
    segtree(vector<ll> &in) {  // 1-base
        vnd.resize(in.size() * 4 + 10);
        build(1, 1, in.size() - 1, in);
    }
    void build(int x, int L, int R, vector<ll> &in) {
        if (L == R) {
            vnd[x].val = in[L];
            return;
        }
        build(2 * x, L, (L + R) / 2, in);
        build(2 * x + 1, (L + R) / 2 + 1, R, in);
        vnd[x].val = vnd[2 * x].val + vnd[2 * x + 1].val;
        vnd[x].lazy = 0;
    }

    void push_down(int x) {
        if (vnd[x].lazy) {
            vnd[2 * x].val = 0;
            vnd[2 * x + 1].val = 0;
            vnd[2 * x].lazy = 1;
            vnd[2 * x + 1].lazy = 1;
            vnd[x].lazy = 0;
        }
    }

    ll query(int x, int L, int R, int askL, int askR) {
        if (askR < L || askL > R) return 0;
        if (askL <= L && askR >= R) return vnd[x].val;
        int mid = (L + R) / 2;
        push_down(x);
        return query(2 * x, L, mid, askL, askR) +
               query(2 * x + 1, mid + 1, R, askL, askR);
    }

    void update(int x, int L, int R, int idx, ll val) {
        if (idx < L || idx > R) return;
        if (idx == L && idx == R) {
            vnd[x].val = (val);
            return;
        }
        int mid = (L + R) / 2;
        push_down(x);
        update(2 * x, L, mid, idx, val);
        update(2 * x + 1, mid + 1, R, idx, val);
        vnd[x].val = vnd[2 * x].val + vnd[2 * x + 1].val;
    }

    void updatez(int x, int L, int R, int askL, int askR) {
        if (askR < L || askL > R) return;
        if (askL <= L && askR >= R) {
            vnd[x].val = 0;
            vnd[x].lazy = 1;
            return;
        }
        int mid = (L + R) / 2;
        push_down(x);
        updatez(2 * x, L, mid, askL, askR);
        updatez(2 * x + 1, mid + 1, R, askL, askR);
        vnd[x].val = vnd[2 * x].val + vnd[2 * x + 1].val;
    }
};

int bsearch(segtree &st, int x, int n) {
    int l = 1, r = n;
    while (l < r) {
        int mid = (l + r) / 2;
        if (st.query(1, 1, n, 1, mid) < x) {
            l = mid + 1;
        } else {
            r = mid;
        }
    }
    return l;
}

int main() {
    int T;
    cin >> T;
    while (T--) {
        int n, m;
        cin >> n >> m;

        vector<ll> v(n + 1);
        for (int i = 1; i <= n; i++) cin >> v[i];

        segtree num(v);
        vector<ll> al(n + 1, 1);
        segtree alive(al);
        for (int i = 0; i < m; i++) {
            int a, b;
            cin >> a >> b;
            int l = bsearch(alive, a, n);
            int r = bsearch(alive, b, n);
            ll sum = num.query(1, 1, n, l, r);
            if (a != b) {
                int lp = bsearch(alive, a + 1, n);
                num.update(1, 1, n, l, sum);
                num.updatez(1, 1, n, lp, r);
                alive.updatez(1, 1, n, lp, r);
            }
            cout << sum << endl;
        }
    }
}