#include <cstdlib>
#include <iostream>
#include <vector>
#include "lp_solver.h"
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<double> b(m + 2 * n);
    vector<vector<double>> A(m + 2 * n, vector<double>(n));
    for (int i = m; i < m + n; i++) b[i] = 26;
    for (int i = m; i < m + n; i++) A[i][i - m] = 1;
    for (int i = m + n; i < m + 2 * n; i++) A[i][i - m - n] = -1;
    for (int i = 0; i < m; i++) {
        int q;
        char k, j;
        cin >> k >> q >> j;
        A[i][k - 97] = 1;
        A[i][j - 97] = -1;
        b[i] = -q;
    }
    vector<double> y(n), c(n);
    for (int i = 0; i < n; i++) c[i] = i + 1;
    double z = lp_solver(GLP_MAX, c, A, y, b);
    for (int i = 0; i < n; i++) cout << y[i] << " ";
    return 0;
}
