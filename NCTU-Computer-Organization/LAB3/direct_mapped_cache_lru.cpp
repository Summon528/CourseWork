#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

struct cache_content {
    cache_content() : v(false), vis(0) {}
    bool v;
    int vis;
    uint32_t tag;
};

double simulate(int cache_size, int assoc, string file_name) {
    int offset_bit = 6;
    int assoc_bit = __lg(assoc);
    int line = cache_size >> assoc_bit >> offset_bit;
    int index_bit = __lg(cache_size) - offset_bit - assoc_bit;

    vector<vector<cache_content>> cache(line, vector<cache_content>(assoc));

    fstream file;
    file.open(file_name, ios::in);

    uint32_t addr, idx, tag;
    int cnt = 0, miss = 0;
    while (file >> hex >> addr) {
        cnt++;
        idx = (addr >> offset_bit) & (line - 1);
        tag = addr >> (index_bit + offset_bit);

        auto it = find_if(
            cache[idx].begin(), cache[idx].end(),
            [&tag](cache_content x) { return x.v == true && x.tag == tag; });

        if (it != cache[idx].end()) {
            for (auto &i : cache[idx]) {
                if (i.vis < it->vis) i.vis++;
            }
            it->vis = 0;
        } else {
            miss++;
            int max = -1;
            vector<cache_content>::iterator max_it;
            for (auto i = cache[idx].begin(); i != cache[idx].end(); i++) {
                if (!i->v) {
                    max_it = i;
                    break;
                }
                if (i->vis > max) max_it = i, max = i->vis;
                i->vis++;
            }
            max_it->vis = 0;
            max_it->v = true;
            max_it->tag = tag;
        }
    }
    return static_cast<double>(miss) / cnt * 100;
}

int main(int argc, char const *argv[]) {
    string files[] = {"LU.txt", "RADIX.txt"};
    for (auto file_name : files) {
        cout << "File name:" << file_name << endl;
        cout << '\t';

        for (int assoc = 1; assoc <= 8; assoc <<= 1) {
            cout << assoc << '\t';
        }
        cout << "(<- associativity)" << endl;

        for (int cache_size = 1; cache_size <= 32; cache_size <<= 1) {
            cout << cache_size << "K\t";
            for (int assoc = 1; assoc <= 8; assoc <<= 1) {
                cout << fixed << setprecision(4)
                     << simulate(cache_size * 1024, assoc, file_name) << '\t';
            }
            cout << endl;
        }
        cout << endl;
    }
    return 0;
}
