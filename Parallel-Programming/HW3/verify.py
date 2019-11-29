import random
import subprocess

x = random.randint(1, 6)
y = random.randint(1, 5)

a = str(random.randint(1, 10 ** x))
b = str(random.randint(1, 10 ** y))
c = str(random.randint(1, 10000000))

print(a, b, c)

s1 = subprocess.run(
    [
        "/home/PP-f19/MPI/bin/mpiexec",
        "-npernode",
        "1",
        "--hostfile",
        "hostfile",
        "./my.out",
        a,
        b,
        c,
    ],
    stdout=subprocess.PIPE,
)

s2 = subprocess.run(["./ans.out", a, b, c], stdout=subprocess.PIPE)
print(s1.stdout.decode())
print(s2.stdout.decode())

if s1.stdout.decode() == s2.stdout.decode():
    print("AC")
else:
    print("WA")
