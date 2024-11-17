import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("granularity", type=int)
    args = parser.parse_args()

    x = args.granularity

    ret = []
    for i in range(x):
        ret.append(round((((x - 1 - i) / (x - 1)) ** (2 / 3)) * (x - 1)))

    print(ret)
