import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("granularity", type=int)
    args = parser.parse_args()

    x = args.granularity

    ret = []
    for i in range(x + 1):
        ret.append(round((((x - i) / x) ** (2 / 3)) * x))

    print(ret)
