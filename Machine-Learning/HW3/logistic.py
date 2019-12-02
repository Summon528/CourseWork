import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from tabulate import tabulate

LR = 0.02


def main():
    dataset = int(input("Which dataset [1, 2]? "))
    method = int(input("Which method [0 = L2-norm, 1 = Cross Entrophy]? "))
    print("")

    # Data Input
    df1 = pd.read_csv(f"data/Logistic_data{dataset}-1.txt", header=None)
    df2 = pd.read_csv(f"data/Logistic_data{dataset}-2.txt", header=None)
    df = pd.concat([df1, df2])
    df[df.shape[1]] = 1
    label = pd.Series([0] * len(df1) + [1] * len(df2))

    # Gradient Descent
    w = np.random.random_sample((df.shape[1],))
    data = df.values
    sigmoid = lambda x, w: 1 / (1 + np.exp(-np.dot(x, w)))
    for _ in range(5000):
        pred = sigmoid(data, w)
        if method == 0:
            loss = (label - pred) * pred * (1 - pred)
        else:
            loss = -(pred - label) / len(label)
        grad = np.dot(data.T, loss)
        w += grad * LR

    # Plot Ground Truth
    label_color = label.map({0: "tab:blue", 1: "tab:orange"})
    plot = df.plot.scatter(x=0, y=1, c=label_color)
    plot.set_xlabel("x")
    plot.set_ylabel("y")
    plot.get_figure().savefig(f"./graph/logistic-d{dataset}-gnd.png")

    # Print statistic
    result = pd.Series(np.rint(sigmoid(data, w)))
    tp = ((result == 1) & (label == 1)).sum()
    fn = ((result == 0) & (label == 1)).sum()
    fp = ((result == 1) & (label == 0)).sum()
    tn = ((result == 0) & (label == 0)).sum()
    print(
        tabulate(
            pd.DataFrame(
                [[tn, fp], [fn, tp]],
                columns=["Predicted: 0", "Predicted: 1"],
                index=["Actual: 0", "Actual: 1"],
            ),
            tablefmt="pipe", headers="keys"
        )
    )
    print("")
    print("Precision: ", tp / (tp + fp))
    print("Recall: ", tp / (tp + fn))
    print("Accuracy: ", (tp + tn) / (tp + tn + fp + fn))
    print("")

    # Print weight
    print("Weight: ", w)

    # Plot result
    result_color = result.map({0: "tab:blue", 1: "tab:orange"})
    plot = df.plot.scatter(x=0, y=1, c=result_color)
    plot.set_xlabel("x")
    plot.set_ylabel("y")
    plot.get_figure().savefig(f"./graph/logistic-d{dataset}-m{method}-pre.png")


if __name__ == "__main__":
    main()
