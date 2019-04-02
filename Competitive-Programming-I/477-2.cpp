#include <bits/stdc++.h>
using namespace std;
struct node {
    node() : left{NULL}, right{NULL}, dist{0} {}
    node *left, *right;
    int val, dist;
};
inline int getd(node* a) { return a ? a->dist : -1; }

node* merge(node* a, node* b) {
    if (a == NULL) return b;
    if (b == NULL) return a;
    if (a->val > b->val) swap(a, b);
    a->right = merge(a->right, b);
    if (getd(a->right) > getd(a->left)) swap(a->left, a->right);
    a->dist = getd(a->right) + 1;
    return a;
}
int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n, m, i, j;
    string s;
    cin >> n >> m;
    vector<node*> v(n);
    while (m--) {
        cin >> s >> i;
        i--;
        if (s == "insert") {
            cin >> j;
            node* tmp = new node;
            tmp->val = j;
            v[i] = merge(v[i], tmp);
        } else if (s == "join") {
            cin >> j;
            j--;
            v[i] = merge(v[i], v[j]);
            v[j] = NULL;
        } else if (s == "extract") {
            if (v[i] == NULL) {
                cout << "NULL\n";
            } else {
                cout << v[i]->val << '\n';
                v[i] = merge(v[i]->left, v[i]->right);
            }
        } else {
            v[i] == NULL ? cout << "NULL\n" : cout << v[i]->val << '\n';
        }
    }
}
