#include <iostream>
#include <vector>
#include "lp_solver.h"

using namespace std;

int main() {
    int n, m, v;
    cin >> n >> m >> v;
    vector<vector<double>> A(4 * n + 2, vector<double>(2 * n));
    for (int k = 0; i < 2; k++) {
        for (int i = 0; i < n; i++) {
            int p;
            cin >> p;
            A[k][2 * i] = p;
            A[k][2 * i + 1] = p;
        }
    }
    for (int i = 2; i < 2 * n + 2; i++) {
        A[i][i - 2] = 1;
    }
    for (int i = 2 * n + 2; i < 4 * n + 2; i++) {
        A[i][i - (2 * n + 2)] = -1;
    }
    vector<double> b(4 * n + 2), y(2 * n), c(2 * n);
    b[0] = m, b[1] = v;
    for (int i = 2; i < 2 * n + 2; i++) b[i] = 1;
    for (int i = 0; i < n; i++) {
        int p;
        cin >> p;
        c[2 * i] = p;
        c[(2 * i) + 1] = p * 0.8;
    }
    cout << lp_solver(GLP_MAX, c, A, y, b) << endl;
    for (int i = 0; i < n; i++) {
        cout << y[2 * i] + y[2 * i + 1] << " ";
    }
    return 0;
}
