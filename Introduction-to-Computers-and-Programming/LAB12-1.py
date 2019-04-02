def pack(names, ages):
    return dict(zip(names, ages))


def people(d, age):
    return [i for i, j in d.items() if j == age]


def main():
    NAMES = ['Alice', 'Bob', 'Cathy', 'Dan', 'Ed', 'Frank',
             'Gary', 'Helen', 'Irene', 'Jack', 'Kelly', 'Larry']
    AGES = [20, 21, 18, 18, 19, 20, 20, 19, 19, 19, 22, 19]
    d = pack(NAMES, AGES)
    print(d)
    print(people(d, 19))


if __name__ == "__main__":
    main()
