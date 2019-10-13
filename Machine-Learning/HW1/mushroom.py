from collections import defaultdict
import math
import random


class MushroomModel:
    def __init__(self):
        self.labels = ()
        self.feature_cnt = 0
        self.prop = {}

    def loaddata(self):
        data = []
        with open("agaricus-lepiota.data", "r") as f:
            for line in f:
                line = line.rstrip()
                if line == "":
                    continue
                data.append(line.split(','))

        skip = set([])
        for feature in data:
            for idx, event in enumerate(feature):
                if event == '?':
                    skip.add(idx)
                    break

        for i in data:
            for s in skip:
                i.pop(s)

        labels = set([])
        result = []
        for i in data:
            result.append((tuple(i[1:]), i[0]))
            labels.add(i[0])
        self.labels = tuple(labels)

        self.feature_cnt = len(result[0][0])
        return result

    def fit(self, data):
        stats = {
            x: [defaultdict(int) for i in range(self.feature_cnt)]
            for x in self.labels
        }
        for key, val in data:
            for idx, event in enumerate(key):
                stats[val][idx][event] += 1

        count = {}
        for l in self.labels:
            count[l] = sum(stats[l][0].values())
        total = sum(count.values())

        self.prop = defaultdict(int)
        for l in self.labels:
            self.prop[l] = count[l] / total

        for l in self.labels:
            for featureIdx, feature in enumerate(stats[l]):
                for eventKey, eventVal in feature.items():
                    self.prop[(eventKey, featureIdx, l)] = eventVal / count[l]

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
    random.shuffle(data)
    model.fit(data[1000:])
    good, bad = 0, 0
    for i in data[:1000]:
        ans = model.predict(i[0])
        if ans == i[1]:
            good += 1
        else:
            bad += 1
    print(good, bad)


if __name__ == "__main__":
    main()
