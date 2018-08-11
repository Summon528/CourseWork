#include <math.h>
#include <stdio.h>
#include <iomanip>
#include <iostream>

using namespace std;

struct cache_content {
    bool v;
    unsigned int tag;
    // unsigned int	data[16];
};

const int K = 1024;

inline int log2(int n) {
    int l = 0;
    while (n >>= 1) l++;
    return l;
}

double simulate(int cache_size, int block_size, string file_name) {
    unsigned int tag, index, x;

    int offset_bit = (int)log2(block_size);
    int index_bit = (int)log2(cache_size / block_size);
    int line = cache_size >> (offset_bit);

    cache_content *cache = new cache_content[line];

    // cout << "cache line: " << line << endl;

    for (int j = 0; j < line; j++) cache[j].v = false;

    FILE *fp = fopen(file_name.c_str(), "r");  // read file

    int cnt = 0, miss = 0;
    while (fscanf(fp, "%x", &x) != EOF) {
        cnt++;
        // cout << hex << x << " ";
        index = (x >> offset_bit) & (line - 1);
        tag = x >> (index_bit + offset_bit);
        if (cache[index].v && cache[index].tag == tag) {
            cache[index].v = true;  // hit
        } else {
            miss++;
            cache[index].v = true;  // miss
            cache[index].tag = tag;
        }
    }
    fclose(fp);

    delete[] cache;
    return (double)miss / cnt * 100;
}

int main() {
    // Let us simulate 4KB cache with 16B blocks
    string files[] = {"DCACHE.txt", "ICACHE.txt"};
    for (auto file_name : files) {
        cout << "File name:" << file_name << endl;
        cout << '\t';

        for (int block_size = 4; block_size <= 128; block_size <<= 1) {
            cout << block_size << '\t';
        }
        cout << "(<- block size)" << endl;

        for (int cache_size = 32; cache_size <= 256; cache_size <<= 1) {
            cout << cache_size << '\t';
            for (int block_size = 4; block_size <= 128; block_size <<= 1) {
                if (block_size > cache_size) {
                    cout << "N/A\t";
                    continue;
                }
                cout << fixed << setprecision(4)
                     << simulate(cache_size, block_size, file_name) << '\t';
            }
            cout << endl;
        }
        cout << endl;
    }
}
