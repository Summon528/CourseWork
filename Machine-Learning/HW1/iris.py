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
                stats[val][idx].append(float(key[idx]))

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
                sun += math.log(prop)
            return math.log(self.prop[label]) + sun

        return max(self.labels, key=score)


def main():
    model = IrisModel()
    data = model.loaddata()
    random.shuffle(data)
    model.fit(data[20:])
    good, bad = 0, 0
    for i in data[:20]:
        ans = model.predict(i[0])
        if ans == i[1]:
            good += 1
        else:
            bad += 1
    print(good, bad)


if __name__ == "__main__":
    main()
