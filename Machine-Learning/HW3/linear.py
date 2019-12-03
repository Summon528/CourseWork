import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


def main():
    # Data Input
    n = int(input("Polynomial bases? "))
    df = pd.read_csv("data/linear_data.txt", header=None)

    # Create Matrix
    xsum = pd.Series([(df[0] ** i).sum() for i in range(n * 2 - 1)])
    y = pd.Series([(df[0] ** i * df[1]).sum() for i in range(n)])
    matrix = []
    for i in range(n):
        tmp = [xsum[i + j] for j in range(n)]
        tmp.append(y[i])
        matrix.append(tmp)
    matrix = pd.DataFrame(matrix)

    # Gaussian Jordan elimination
    for i in range(n):
        if matrix[i][i] == 0:
            continue
        matrix.loc[i] /= matrix[i][i]
        for j in range(n):
            if i == j:
                continue
            matrix.loc[j] -= matrix[i][j] * matrix.loc[i]
    coeff = matrix[n]
    
    # Print Fitting line
    print("Fitting line: ", end="")
    for idx, c in enumerate(coeff):
        print(f"{c}x^{idx}",end= " + " if idx != n-1 else "")
    print("")

    # Create function
    def f(x):
        ans = 0
        for idx, c in enumerate(coeff):
            ans += x ** idx * c
        return ans

    # Calculate Error
    print(f"Total error: {((f(df[0]) - df[1])**2).sum()}")

    # Plot
    x = np.linspace(df[0].min(), df[0].max(), 1000)
    plot = df.plot.scatter(x=0, y=1)
    plot.plot(x, f(x), color='tab:orange')
    plot.set_xlabel("x")
    plot.set_ylabel("y")
    plot.get_figure().savefig(f"./graph/linear-base{n}.png")
    


if __name__ == "__main__":
    main()
