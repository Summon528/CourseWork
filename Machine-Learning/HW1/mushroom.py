from collections import defaultdict
import math
import random


class MushroomModel:
    def __init__(self):
        self.labels = ()
        self.feature_cnt = 0
        self.prop = {}
        self.events = []

    def loaddata(self):
        data = []
        with open("agaricus-lepiota.data", "r") as f:
            for line in f:
                line = line.rstrip()
                if line == "":
                    continue
                data.append(line.split(","))

        prev_data = list(data[0])
        skip = set([])
        for feature in data:
            for idx, event in enumerate(feature):
                if event == "?":
                    skip.add(idx)
                if event != prev_data[idx]:
                    prev_data[idx] = None

        for idx, val in enumerate(prev_data):
            if val != None:
                skip.add(idx)
        
        skip = sorted(list(skip))
        skip_cnt = 0
        for s in skip:
            for i in data:
                i.pop(s - skip_cnt)
            skip_cnt += 1

        labels = set([])
        result = []
        for i in data:
            result.append((tuple(i[1:]), i[0]))
            labels.add(i[0])
        self.labels = tuple(labels)

        self.feature_cnt = len(result[0][0])
        self.events = [set([]) for i in range(self.feature_cnt)]
        for i in result:
            for jdx, j in enumerate(i[0]):
                self.events[jdx].add(j)
        return result

    def fit(self, data, k=0):
        tao = [len(i) for i in self.events]
        stats = {
            x: [defaultdict(int) for i in range(self.feature_cnt)] for x in self.labels
        }
        for key, label_name in data:
            for feature_idx, feature_val in enumerate(key):
                stats[label_name][feature_idx][feature_val] += 1

        count = {}
        for l in self.labels:
            count[l] = sum(stats[l][0].values())
        total = sum(count.values())

        self.prop = defaultdict(int)
        for l in self.labels:
            self.prop[l] = count[l] / total

        for label_name in self.labels:
            for feature_idx, feature in enumerate(stats[label_name]):
                for feature_val in self.events[feature_idx]:
                    self.prop[(feature_val, feature_idx, label_name)] = (
                        (feature[feature_val] + k) / (count[label_name] + k * tao[feature_idx])
                    )

    def predict(self, item):
        def score(label):
            sun = 0
            for i in range(self.feature_cnt):
                target = (item[i], i, label)
                if target not in self.prop or self.prop[target] == 0:
                    sun += -1e9
                else:
                    sun += math.log(self.prop[target])
            return math.log(self.prop[label]) + sun

        return max(self.labels, key=score)


def main():
    model = MushroomModel()
    data = model.loaddata()
    random.shuffle(data, lambda: 0.611262)
    data_size = len(data)

    for k in range(2):
        print(f"k = {k}")
        print("Holdout validation")
        train_end_idx = math.floor(data_size * 0.7)
        model.fit(data[:train_end_idx], k)
        result = defaultdict(lambda: defaultdict(int))

        for i in data[train_end_idx:]:
            ans = model.predict(i[0])
            result[i[1]][ans] += 1
        print_result(result)
        print()

        print("K-fold cross-validation")
        part = data_size // 3
        splitted_data = [data[:part], data[part : part * 2], data[part * 2 :]]
        result = defaultdict(lambda: defaultdict(int))
        for i in range(3):
            to_train = list(splitted_data)
            to_test = to_train.pop(i)
            model.fit(to_train[0] + to_train[1], k)
            for i in to_test:
                ans = model.predict(i[0])
                result[i[1]][ans] += 1
        print_result(result)
        print('')


def print_result(result):
    good, bad = 0, 0
    print("", end="\t")
    keys = list(result.keys())
    for key in keys:
        print(key, end="\t")
    print("Precision", end="\t")
    print("")
    for key, val in result.items():
        print(key, end="\t")
        sun = 0
        for key2 in keys:
            if key2 == key:
                good += val[key2]
            else:
                bad += val[key2]
            print(val[key2], end="\t")
            sun += val[key2]
        print("{:.4}".format(val[key] / sun))
    print("Recall", end="\t")
    for key in keys:
        sun = 0
        for key2 in keys:
            sun += result[key2][key]
        print("{:.4}".format(result[key][key] / sun), end="\t")
    print('')
    print("Accuracy: ", good / (good + bad))
    print("=====")


if __name__ == "__main__":
    main()
