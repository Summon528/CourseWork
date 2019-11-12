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

constexpr unsigned long long RAND_SEED = 0611262611262;
using CellType = variant<int, string>;  // A cell is either a string or an int
using RowType = vector<CellType>;       // A row is an array of cell
using XType = vector<RowType>;          // X is a list of Row
using yType = vector<pair<int, int>>;   // y is a list of (id, label) pair
using XyType = vector<pair<RowType, int>>;  // XyType is a list of
                                            // (example, label) pair

/**
 * Read and parse example data provided by TA
 *
 * @param file_name
 * @return a vector of example
 */
XType read_X(string file_name = "X_train.csv") {
    csv::CSVFormat format;
    format.trim({' ', '\t'});
    csv::CSVReader reader(file_name, format);
    XType data;
    for (csv::CSVRow& row : reader) {
        RowType tmp;
        for (unsigned int i = 1; i < row.size(); i++) {
            if (row[i].is_int()) {
                tmp.emplace_back(row[i].get<int>());
            } else if (row[i].is_str()) {
                tmp.emplace_back(row[i].get<string>());
            }
        }
        data.emplace_back(tmp);
    };
    return data;
}

/**
 * Read and parse label data provided by TA
 *
 * @param file_name
 * @return a vector of (id, label) pair
 */
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

/**
 * Merge X and y, then shuffle the dataset
 *
 * @param X a vector of example (the result from read_X)
 * @param y a vector of (id, label) pair, (the result from read_y)
 * @return a list of shuffled (example, label) pair
 */
XyType preprocess(const XType& X, const yType& y) {
    XyType Xy;
    assert(X.size() == y.size());
    for (unsigned int i = 0; i < X.size(); i++) {
        Xy.emplace_back(make_pair(X[i], y[i].second));
    }
    shuffle(Xy.begin(), Xy.end(), mt19937(RAND_SEED));
    return Xy;
}

class Question {
   private:
    int col_idx;       // The index of the target feature
    CellType content;  // The content of this question
    static vector<string> header_names;

   public:
    Question() : col_idx{-1}, content{-1} {}
    Question(int a, CellType b) : col_idx{a}, content{move(b)} {}
    static void populate_header_names(vector<string> hn) { header_names = hn; }

    /**
     * Describe the question
     */
    void describe() const {
        if (col_idx < header_names.size() - 1) {
            cout << "Is " << header_names[col_idx];
            if (content.index() == 0) {
                cout << " >= " << get<int>(content);
            } else {
                cout << " == '" << get<string>(content) << "'";
            }
            cout << "? ";
        }
    }

    /**
     * Given the row, will this question answer yes or no
     *
     * If the target feature is a continuous feature, return true if the value
     * is larger or equal to `content`
     *
     * If the target feature is a categorical feature, returm true if the value
     * is eqaul to `content`
     *
     * @param X the result from read_X
     * @param y the result from read_y
     * @return true or false
     */
    bool match(const RowType& r) const {
        if (content.index() == 0) {
            return r[col_idx] >= content;
        }
        return r[col_idx] == content;
    }
};
vector<string> Question::header_names;

class Node {
   public:
    unique_ptr<Node> left, right;  // pointer to the left and right child
    Question q;                    // The question associated to this node
                                   //   (only for non-leaf nodes)
    int predict;                   // The prediction of this node
                                   //   (only for leaf nodes)
    explicit Node(int predict)
        : left{nullptr}, right{nullptr}, q{Question()}, predict{predict} {}
    Node(unique_ptr<Node>& l, unique_ptr<Node>& r, const Question q)
        : left{move(l)}, right{move(r)}, q{move(q)}, predict{-1} {};
};

class DecisionTree {
   private:
    unique_ptr<Node> root;  // a pointer to the root node

    /**
     * Count the frequency of each labels
     *
     * @param Xy a vector of (example, label) pair
     *           (example data are not used in this function)
     * @return an unordered_map of (label, count) pair
     */
    static unordered_map<int, int> count_labels(const XyType& Xy) {
        unordered_map<int, int> cnt;
        for (const auto& i : Xy) cnt[i.second]++;
        return cnt;
    }

    /**
     * Calculate gini impurity of the given data
     *
     * @param cnt an unordered_map of (label, count) pair
     * @return gini impurity of the given data
     */
    static double calc_gini(const unordered_map<int, int>& cnt) {
        // cacluate the size of the data
        int size = 0;
        for (auto const& [key, val] : cnt) {
            size += val;
        }
        // cacluate gini impurity
        double result = 1.;
        for (auto const& [key, val] : cnt) {
            double prob = static_cast<double>(val) / size;
            result -= prob * prob;
        }
        return result;
    }

    /**
     * For the given question, cacluate the gain with respect to the dataset
     *
     * @param Xy the dataset
     * @param q a qestion
     * @param cur_gini the gini impurity of the dataset
     * @return the gain if we split the dataset by the given question
     */
    static double calc_gain(const XyType& Xy, const Question& q,
                            const double cur_gini) {
        // find out the distribution of the labels if we split the data
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

        // skip this question if any side has size of zero
        if (right_size == 0 || left_size == 0) return -1;

        // cacluate gain
        const auto left_gini = calc_gini(cnt_left);
        const auto right_gini = calc_gini(cnt_right);

        double left_p =
            static_cast<double>(left_size) / (left_size + right_size);

        double gain = cur_gini - left_p * left_gini - (1 - left_p) * right_gini;

        return gain;
    }

    /**
     * Actually split the dataset by the given question
     *
     * @param Xy the dataset
     * @param q a qestion
     * @return a pair of (XyType, XyType) where the first element is the set of
     *         data that cannot match the question and the second is those match
     *         the question
     */
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

    /**
     * Find the best question and its gain with respect to the dataset
     *
     * @param Xy the dataset
     * @return a pair of (best_gain, best_Question)
     */
    static pair<double, Question> find_best(const XyType& Xy) {
        // cacluate current gini
        const auto cur_gini = calc_gini(count_labels(Xy));
        Question best_q = Question();
        double max_gain = -1;

        // for every feature...
        for (unsigned int i = 0; i < Xy.front().first.size(); i++) {
            // find all unique value of the feature
            set<CellType> values;
            for (const auto& j : Xy) values.insert(j.first[i]);

            vector<CellType> values_vector;
            // Optimization: if the feature is a continuous feature and have
            //               more than 100 unique values, we do not want to try
            //               every of them, instead we find the max and mix,
            //               let step = (max - min) / 100 and try every step
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

// Optimization: process each value of the feature concurrently
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

    /**
     * Recursively build a decision tree
     *
     * @param Xy the dataset
     * @return the root node
     */
    static unique_ptr<Node> build_tree(const XyType& Xy) {
        // find the best question
        const auto [gain, question] = find_best(Xy);

        // Base case: if the gain is too small, we create a leaf node
        if (gain <= 0.01) {
            // find out the dominate label of the dataset
            const auto cnt = count_labels(Xy);
            int max_lb = -1, max_lb_cnt = -1;
            for (auto const& [key, val] : cnt) {
                if (val > max_lb_cnt) {
                    max_lb_cnt = val;
                    max_lb = key;
                }
            }
            // create a leaf node of the dominate label
            return make_unique<Node>(max_lb);
        }

        auto [left, right] = split(Xy, question);
        auto left_node = build_tree(left);
        auto right_node = build_tree(right);

        // create an intermediate node
        return make_unique<Node>(left_node, right_node, question);
    }

    /**
     * Recursively walk through the tree and find the predicted value
     *
     * @param r the given example
     * @param n the current node
     * @param describe do we want to describe the question we went through
     * @return the predicted label
     */
    int _predict(const RowType& r, const unique_ptr<Node>& n,
                 bool describe) const {
        if (n->predict != -1) {
            if (describe) {
                cout << "Its label is " << n->predict << endl << endl;
            }
            return n->predict;
        }
        if (describe) {
            n->q.describe();
        }
        if (n->q.match(r)) {
            if (describe) {
                cout << "Yes" << endl;
            }
            return _predict(r, n->right, describe);
        }
        if (describe) {
            cout << "No" << endl;
        }
        return _predict(r, n->left, describe);
    }

   public:
    DecisionTree(const XyType& Xy) { root = build_tree(Xy); }
    int predict(const RowType& r, bool describe = false) const {
        return _predict(r, root, describe);
    }
};

class RandomForest {
   private:
    vector<DecisionTree> trees;

   public:
    /**
     * Construct n decision trees, each using 80% of data with different order
     *
     * @param Xy the dataset
     * @param tree_cnt the number of trees to be create
     */
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
    auto Xy = preprocess(X, y);
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
    int size = matrix[0][0] + matrix[0][1] + matrix[1][0] + matrix[1][1];
    cout << "Accuracy: "
         << static_cast<double>(matrix[1][1] + matrix[0][0]) / size << endl
         << "Recall: "
         << static_cast<double>(matrix[1][1]) / (matrix[1][1] + matrix[1][0])
         << endl
         << "Precision: "
         << static_cast<double>(matrix[1][1]) / (matrix[1][1] + matrix[0][1])
         << endl
         << endl;
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

    cout << "Hold out - RandomForest" << endl;
    print_matrix(matrix2);
}

void k_fold(const XyType& Xy, int fold) {
    array<array<int, 2>, 2> matrix1 = {}, matrix2 = {};
    int ac1 = 0, ac2 = 0, total_test = 0;
    for (int i = 0; i < fold; i++) {
        XyType train_set, test_set;
        for (unsigned int j = 0; j < Xy.size(); j++) {
            if (j % fold == i) {
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

    cout << "K-fold - RandomForest" << endl;
    print_matrix(matrix2);
}

void describe_predict(const XyType& Xy, int cnt) {
    const auto train_size = Xy.size() / 10 * 7;
    const XyType train_set(Xy.begin(), Xy.begin() + train_size);
    const XyType test_set(Xy.begin() + train_size, Xy.end());
    const auto tree = DecisionTree(train_set);
    for (int i = 0; i < test_set.size() && i < 3 * cnt; i++) {
        if (i % 3 == 0) {
            for (const auto& j : Xy[i].first) {
                visit([](auto&& arg) { cout << arg << ' '; }, j);
            }
            cout << endl;
        }
        tree.predict(Xy[i].first, i % 3 == 0);
    }
}

int main() {
    // kaggle();
    csv::CSVReader reader("X_train.csv");
    auto col_names = reader.get_col_names();
    col_names.erase(col_names.begin());  // remove id col
    Question::populate_header_names(col_names);
    const auto X = read_X();
    const auto y = read_y();
    auto Xy = preprocess(X, y);
    hold_out(Xy);
    k_fold(Xy, 3);
    // describe_predict(Xy, 10);
}
