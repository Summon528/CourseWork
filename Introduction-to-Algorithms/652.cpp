#include <iostream>
#include <vector>

using namespace std;

int main() {
    cin.ignore();
    long long n, sun = -1, cur = 0;
    while (cin >> n) {
        cur += n;
        sun = max(sun, cur);
        if (cur < 0) cur = 0;
    }
    cout << sun << endl;
}
