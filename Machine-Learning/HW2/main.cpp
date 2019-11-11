#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <unordered_set>
#include <variant>
#include <vector>
#include "csv.hpp"

using namespace std;

constexpr int NCOL = 14;
constexpr unsigned long long RAND_SEED = 0611262611262;
using CellType = variant<int, string>;
using RowType = array<CellType, NCOL>;
using XType = vector<RowType>;
using yType = vector<pair<int, int>>;
using XyType = vector<pair<RowType, int>>;

XType read_X(string file_name = "X_train.csv") {
    csv::CSVFormat format;
    format.trim({' ', '\t'});
    csv::CSVReader reader(file_name, format);
    XType data;
    for (csv::CSVRow& row : reader) {
        data.emplace_back(RowType{
            row[1].get<int>(), row[2].get<string>(), row[3].get<int>(),
            row[4].get<string>(), row[5].get<int>(), row[6].get<string>(),
            row[7].get<string>(), row[8].get<string>(), row[9].get<string>(),
            row[10].get<string>(), row[11].get<int>(), row[12].get<int>(),
            row[13].get<int>(), row[14].get<string>()});
    };
    return data;
}

yType read_y(string file_name = "y_train.csv") {
    csv::CSVFormat format;
    format.trim({' ', '\t'});
    csv::CSVReader reader(file_name, format);
    yType data;
    for (csv::CSVRow& row : reader) {
        data.emplace_back(make_pair(row[0].get<int>(), row[1].get<int>()));
    };
    return data;
}

XyType mergeXy(const XType& X, const yType& y) {
    XyType Xy;
    assert(X.size() == y.size());
    for (unsigned int i = 0; i < X.size(); i++) {
        Xy.emplace_back(make_pair(X[i], y[i].second));
    }
    return Xy;
}

class Question {
   private:
    int col_idx;
    CellType thre;

   public:
    Question() : col_idx{-1}, thre{-1} {}
    Question(int a, CellType b) : col_idx{a}, thre{move(b)} {}
    bool match(const RowType& r) const {
        if (thre.index() == 0) {
            return r[col_idx] >= thre;
        }
        return r[col_idx] == thre;
    }
};

class Node {
   public:
    unique_ptr<Node> left, right;
    Question q;
    int predict;
    explicit Node(int predict)
        : left{nullptr}, right{nullptr}, q{Question()}, predict{predict} {}
    Node(unique_ptr<Node>& l, unique_ptr<Node>& r, const Question q)
        : left{move(l)}, right{move(r)}, q{move(q)}, predict{-1} {};
};

class DecisionTree {
   private:
    unique_ptr<Node> root;

    static unordered_map<int, int> count_labels(const XyType& Xy) {
        unordered_map<int, int> cnt;
        for (const auto& i : Xy) cnt[i.second]++;
        return cnt;
    }

    static double calc_gini(const unordered_map<int, int>& cnt,
                            const int size) {
        double result = 1.;
        for (auto const& [key, val] : cnt) {
            double prob = static_cast<double>(val) / size;
            result -= prob * prob;
        }
        return result;
    }

    static double calc_gain(const XyType& Xy, const Question& q,
                            const double cur_gini) {
        unordered_map<int, int> cnt_left, cnt_right;
        int left_size = 0, right_size = 0;
        for (const auto& i : Xy) {
            if (q.match(i.first)) {
                cnt_right[i.second]++;
                right_size++;
            } else {
                cnt_left[i.second]++;
                left_size++;
            }
        }

        const auto left_gini = calc_gini(cnt_left, left_size);
        const auto right_gini = calc_gini(cnt_right, right_size);

        double left_p =
            static_cast<double>(left_size) / (left_size + right_size);

        double gain = cur_gini - left_p * left_gini - (1 - left_p) * right_gini;

        return gain;
    }

    static pair<XyType, XyType> split(const XyType& Xy, const Question& q) {
        XyType left, right;
        for (const auto& i : Xy) {
            if (q.match(i.first)) {
                right.emplace_back(i);
            } else {
                left.emplace_back(i);
            }
        }
        return {left, right};
    }

    static pair<double, Question> find_best(const XyType& Xy) {
        const auto cur_gini = calc_gini(count_labels(Xy), Xy.size());
        Question best_q = Question();
        double max_gain = -1;

        for (int i = 0; i < NCOL; i++) {
            set<CellType> values;
            for (const auto& j : Xy) values.insert(j.first[i]);

            vector<CellType> values_vector;
            if (Xy[0].first[i].index() == 0 && values.size() > 100) {
                int min_val = get<int>(*values.begin());
                int max_val = get<int>(*values.rbegin());
                int step = (max_val - min_val) / 100;
                for (int j = min_val; j < max_val; j += step) {
                    values_vector.emplace_back(j);
                }
                values_vector.emplace_back(max_val);
            } else {
                copy(values.begin(), values.end(),
                     back_inserter(values_vector));
            }

#pragma omp parallel for
            for (unsigned int j = 0; j < values_vector.size(); j++) {
                auto q = Question(i, values_vector[j]);
                double gain = calc_gain(Xy, q, cur_gini);

#pragma omp critical
                if (gain > max_gain) {
                    best_q = q;
                    max_gain = gain;
                }
            }
        }

        return {max_gain, best_q};
    }

    static unique_ptr<Node> build_tree(const XyType& Xy) {
        const auto [gain, question] = find_best(Xy);

        if (gain <= 0.01) {
            const auto cnt = count_labels(Xy);
            int max_lb = -1, max_lb_cnt = -1;
            for (auto const& [key, val] : cnt) {
                if (val > max_lb_cnt) {
                    max_lb_cnt = val;
                    max_lb = key;
                }
            }
            return make_unique<Node>(max_lb);
        }

        auto [left, right] = split(Xy, question);
        auto left_node = build_tree(left);
        auto right_node = build_tree(right);

        return make_unique<Node>(left_node, right_node, question);
    }

    int _predict(const RowType& r, const unique_ptr<Node>& n) const {
        if (n->predict != -1) {
            return n->predict;
        }
        if (n->q.match(r)) {
            return _predict(r, n->right);
        }
        return _predict(r, n->left);
    }

   public:
    DecisionTree(const XyType& Xy) { root = build_tree(Xy); }
    int predict(const RowType& r) const { return _predict(r, root); }
};

class RandomForest {
   private:
    vector<DecisionTree> trees;

   public:
    RandomForest(XyType Xy, int tree_cnt) {
        std::mt19937 g(RAND_SEED);
        for (int i = 0; i < tree_cnt; i++) {
            shuffle(Xy.begin(), Xy.end(), g);
            XyType train_set(Xy.begin(), Xy.begin() + (Xy.size() / 5 * 4));
            trees.push_back(DecisionTree(train_set));
        }
    }
    int predict(const RowType& r) const {
        unordered_map<int, int> um;
        int max_lb = -1, max_lb_cnt = -1;
        for (const auto& t : trees) {
            const auto ans = t.predict(r);
            um[ans]++;
            if (um[ans] > max_lb_cnt) {
                max_lb = ans;
            }
        }
        return max_lb;
    }
};

void kaggle() {
    const auto X = read_X();
    const auto y = read_y();
    auto Xy = mergeXy(X, y);
    shuffle(Xy.begin(), Xy.end(), mt19937(RAND_SEED));
    const auto tree = DecisionTree(Xy);

    const auto testX = read_X("X_test.csv");
    ofstream ofs("y_test.csv");
    auto writer = csv::make_csv_writer(ofs);
    int idx = 22792;
    writer << vector<string>{"Id", "Category"};
    for (const auto& i : testX) {
        const auto r = tree.predict(i);
        writer << vector<string>{to_string(idx), to_string(r)};
        idx++;
    }
}

void print_matrix(array<array<int, 2>, 2>& matrix) {
    cout << "\tPred 0\tPred 1" << endl;
    for (int i = 0; i < 2; i++) {
        cout << "Act " << i << "\t" << matrix[i][0] << "\t" << matrix[i][1]
             << endl;
    }
}

void hold_out(const XyType& Xy) {
    const auto train_size = Xy.size() / 10 * 7;
    const XyType train_set(Xy.begin(), Xy.begin() + train_size);
    const XyType test_set(Xy.begin() + train_size, Xy.end());
    const auto tree = DecisionTree(train_set);
    const auto forest = RandomForest(train_set, 3);

    array<array<int, 2>, 2> matrix1 = {}, matrix2 = {};
    int ac1 = 0, ac2 = 0;
    for (const auto& i : test_set) {
        const auto r1 = tree.predict(i.first);
        const auto r2 = forest.predict(i.first);
        matrix1[i.second][r1]++;
        matrix2[i.second][r2]++;
        if (i.second == r1) ac1++;
        if (i.second == r2) ac2++;
    }
    cout << "Hold out - DecisionTree" << endl;
    print_matrix(matrix1);
    cout << "Accuracy: " << static_cast<double>(ac1) / test_set.size() << endl
         << endl;

    cout << "Hold out - RandomForest" << endl;
    print_matrix(matrix2);
    cout << "Accuracy: " << static_cast<double>(ac2) / test_set.size() << endl
         << endl;
}

void k_fold(const XyType& Xy, int fold) {
    array<array<int, 2>, 2> matrix1 = {}, matrix2 = {};
    int ac1 = 0, ac2 = 0, total_test = 0;
    for (int i = 0; i < fold; i++) {
        XyType train_set, test_set;
        for (unsigned int j = 0; j < Xy.size(); j++) {
            if (j % fold == 0) {
                test_set.emplace_back(Xy[j]);
            } else {
                train_set.emplace_back(Xy[j]);
            }
        }
        total_test += test_set.size();
        const auto tree = DecisionTree(train_set);
        const auto forest = RandomForest(train_set, 3);
        for (const auto& i : test_set) {
            const auto r1 = tree.predict(i.first);
            const auto r2 = forest.predict(i.first);
            matrix1[i.second][r1]++;
            matrix2[i.second][r2]++;
            if (i.second == r1) ac1++;
            if (i.second == r2) ac2++;
        }
    }
    cout << "K-fold - DecisionTree" << endl;
    print_matrix(matrix1);
    cout << "Accuracy: " << static_cast<double>(ac1) / total_test << endl
         << endl;

    cout << "K-fold - RandomForest" << endl;
    print_matrix(matrix2);
    cout << "Accuracy: " << static_cast<double>(ac2) / total_test << endl
         << endl;
}

int main() {
    // kaggle();
    const auto X = read_X();
    const auto y = read_y();
    auto Xy = mergeXy(X, y);
    shuffle(Xy.begin(), Xy.end(), mt19937(RAND_SEED));
    hold_out(Xy);
    k_fold(Xy, 3);
}
