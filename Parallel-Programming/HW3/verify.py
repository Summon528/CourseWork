import random
import subprocess

while True:
    x = random.randint(1, 6)
    y = random.randint(1, 5)

    a = str(random.randint(1, 10 ** x))
    b = str(random.randint(1, 10 ** y))
    c = str(random.randint(1, 10000000))

    print(a, b, c, flush=True)

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
    print(s1.stdout.decode().strip(), flush=True)
    print(s2.stdout.decode().strip(), flush=True)

    if s1.stdout.decode() == s2.stdout.decode():
        print("AC", flush=True)
    else:
        print("WA", flush=True)
