#!/usr/bin/env python3
# encoding: utf-8

for i in range(1, 9):
    pre = None
    with open(f"output{i}.txt", "r") as f:
        cur = f.read()
        if pre is not None and cur != pre:
            print("WA{i}")
        pre = cur

pre = pre.strip()
with open("input.txt", "r") as f:
    ans = " ".join(map(str, sorted(map(int, f.read().split()[1:]))))

if ans == pre:
    print("AC")
else:
    print("WA")
