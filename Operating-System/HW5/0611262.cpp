/*
Student No.: 0611262
Student Name: Chang-Yen Tseng
Email: vanilla.cv06@nctu.edu.tw
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not
supposed to be posted to a public server, such as a
public GitHub repository or a public web page.
*/

#include <sys/time.h>
#include <ext/pb_ds/priority_queue.hpp>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <array>
#include <vector>

using namespace std;

struct Node {
    int freq, ts, page, idx, frame;
    Node(int idx, int page) : freq{1}, ts{idx}, page{page} {}
};

int main(int argc, char* argv[]) {
    using pbds_pq =
        __gnu_pbds::priority_queue<Node, function<bool(Node&, Node&)>>;
    const array<function<bool(Node&, Node&)>, 2> policies = {
        [](Node& a, Node& b) {
            return a.freq != b.freq ? a.freq > b.freq : a.ts > b.ts;
        },
        [](Node& a, Node& b) { return a.ts > b.ts; }};
    const array<string, 2> policy_names = {"LFU", "LRU"};

    for (auto policy_idx = 0; policy_idx < policies.size(); policy_idx++) {
        struct timeval st, ed;
        gettimeofday(&st, 0);
        const auto& cmp = policies[policy_idx];
        if (policy_idx != 0) cout << endl;
        cout << policy_names[policy_idx] << " policy:" << endl;
        cout << "Frame\tHit\t\tMiss\t\tPage fault ratio" << endl;
        for (auto frame_size = 64; frame_size <= 512; frame_size <<= 1) {
            ifstream ifs(argv[1]);
            unordered_map<int, pbds_pq::point_iterator> table;
            pbds_pq pq(cmp);

            int page_num, hit = 0, miss = 0;
            for (auto idx = 0; ifs >> page_num; idx++) {
                if (table.find(page_num) == table.end()) {
                    miss++;
                    int free_frame;
                    if (pq.size() >= frame_size) {
                        const auto victim_node = pq.top();
                        pq.pop();
                        table.erase(victim_node.page);
                    }
                    const auto node = Node(idx, page_num);
                    table[page_num] = pq.push(node);
                } else {
                    hit++;
                    auto target = table[page_num];
                    auto tmp(*target);
                    tmp.freq++;
                    tmp.ts = idx;
                    pq.modify(target, tmp);
                }
            }
            cout << frame_size << "\t" << hit << "\t\t" << miss << "\t\t"
                 << fixed << setprecision(10)
                 << static_cast<double>(miss) / (hit + miss) << endl;
            ifs.close();
        }
        gettimeofday(&ed, 0);
        int sec = ed.tv_sec - st.tv_sec;
        int usec = ed.tv_usec - st.tv_usec;
        cout << "Total elapsed time " << fixed << setprecision(4)
             << sec + usec * 1e-6 << " sec" << endl;
    }
}
