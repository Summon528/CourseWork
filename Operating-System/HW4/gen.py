#!/usr/bin/env python3
# encoding: utf-8

import random

k = 100000
n = random.randint(int(0.9 * k), k)

data = [random.randint(1, 1000000000) for i in range(n)]


with open("input.txt", "w") as f:
    f.write(str(n))
    f.write("\n")
    f.write(" ".join(map(str, data)))
