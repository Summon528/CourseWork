from collections import defaultdict
import math
import random
import statistics


def norm(x, std, mean):
    a = (x - mean)**2
    b = 2 * std**2
    c = math.exp(-(a / b))
    d = (2 * math.pi)**0.5
    e = 1 / (std * d)
    return e * c


class IrisModel:
    def __init__(self):
        self.labels = ()
        self.feature_cnt = 0
        self.prop = {}
        self.mean = {}
        self.std = {}

    def loaddata(self):
        data = []
        with open("iris.data", "r") as f:
            for line in f:
                line = line.rstrip()
                if line == "":
                    continue
                data.append(line.split(','))

        labels = set([])
        result = []
        for i in data:
            result.append((tuple(i[:-1]), i[-1]))
            labels.add(i[-1])
        self.labels = tuple(labels)

        self.feature_cnt = len(result[0][0])
        return result

    def fit(self, data):
        stats = {x: [[] for i in range(self.feature_cnt)] for x in self.labels}
        for key, val in data:
            for idx, event in enumerate(key):
                stats[val][idx].append(float(event))

        self.mean = {x: [0] * self.feature_cnt for x in self.labels}
        self.std = {x: [0] * self.feature_cnt for x in self.labels}

        for l in self.labels:
            for i in range(self.feature_cnt):
                self.mean[l][i] = statistics.mean(stats[l][i])
                self.std[l][i] = statistics.stdev(stats[l][i])

        count = {}
        for l in self.labels:
            count[l] = len(stats[l][0])
        total = sum(count.values())

        self.prop = defaultdict(int)
        for l in self.labels:
            self.prop[l] = count[l] / total

    def predict(self, item):
        def score(label):
            sun = 0
            for i in range(self.feature_cnt):
                prop = norm(float(item[i]), self.std[label][i],
                            self.mean[label][i])
                if prop == 0:
                    sun += -1e9
                else:
                    sun += math.log(prop)
            return math.log(self.prop[label]) + sun

        return max(self.labels, key=score)

def main():
    model = IrisModel()
    data = model.loaddata()
    random.shuffle(data, lambda: 0.611262)
    data_size = len(data)

    print("Holdout validation")
    train_end_idx = math.floor(data_size * 0.7)
    model.fit(data[:train_end_idx])
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
        model.fit(to_train[0] + to_train[1])
        for i in to_test:
            ans = model.predict(i[0])
            result[i[1]][ans] += 1
    print_result(result)


def print_result(result):
    good, bad = 0, 0
    print("", end="\t\t")
    keys = list(result.keys())
    for key in keys:
        print(key, end="\t")
    print("Precision", end="\t\t")
    print("")
    for key, val in result.items():
        print(key, end="\t")
        sun = 0
        for key2 in keys:
            if key2 == key:
                good += val[key2]
            else:
                bad += val[key2]
            print(val[key2], end="\t\t")
            sun += val[key2]
        print("{:.4}".format(val[key] / sun))
    print("Recall", end="\t\t")
    for key in keys:
        sun = 0
        for key2 in keys:
            sun += result[key2][key]
        print("{:.4}".format(result[key][key] / sun), end="\t\t")
    print('')
    print("Accuracy: ", good / (good + bad))
    print("=====")



if __name__ == "__main__":
    main()
