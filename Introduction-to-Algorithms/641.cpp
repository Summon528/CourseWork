#include <gmpxx.h>
#include <iostream>
#include <vector>
using namespace std;
int main() {
    mpf_set_default_prec(80000);
    mpz_class d = 6;
    mpf_class m = 1;
    for (int i = 0; i < 18000; i++) {
        d *= 2;
        m = 2 - sqrt(4 - m);
    }
    mp_exp_t exp;
    m = d * sqrt(m) / 2;
    string s = m.get_str(exp);
    int T, l, r;
    cin >> T;
    while (T--) {
        cin >> l >> r;
        cout << s.substr(l, r - l + 1) << '\n';
    }
}
