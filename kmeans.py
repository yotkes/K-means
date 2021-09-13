import argparse
import sys
import math


def distance(x, y):
    s = 0
    d = len(x)
    for i in range(d):
        s += (x[i] - y[i]) ** 2
    return s


def best_index(v, centroids):
    l = [(x, distance(centroids[x], v)) for x in range(len(centroids))]
    r = min(l, key=lambda x: x[1])
    return r[0]


def calc_new_centroid(cluster):
    if len(cluster) == 0:
        return None
    d = len(cluster[0])
    n = len(cluster)
    res = [0 for _ in range(d)]
    for v in cluster:
        for i in range(d):
            res[i] += v[i]

    for i in range(d):
        res[i] /= n
    return res


def K_MEANS(datapoints, K, max_iter):
    centroids = [[] for _ in range(K)]
    it = 0
    balanced = False
    for i in range(K):
        centroids[i] = datapoints[i]
    while it < max_iter and not balanced:
        balanced = True
        clusters = [[] for _ in range(K)]
        for v in datapoints:
            i = best_index(v, centroids)
            clusters[i].append(v)
        for i in range(len(clusters)):
            cluster = clusters[i]
            new_centroid = calc_new_centroid(cluster)
            if distance(new_centroid, centroids[i]) != 0:
                balanced = False
            centroids[i] = new_centroid
        it += 1
    return centroids


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("K", type=int, help="Number of centroids - integer >=1")
    if len(sys.argv) == 3:
        parser.add_argument("MAX_ITER", type=int, help="Maximum number of iterations to reach convergence - integer >=1")
    elif len(sys.argv) == 2:
        max_iter = sys.maxsize
    else:
        print("K has to be input")
        exit(1)

    args = parser.parse_args()
    K = args.K 
    if len(sys.argv) == 3:
         max_iter = args.MAX_ITER
    
    flag = False
    if K < 1:
        print("K has to be >=1")
        flag = True
    if max_iter < 1:
        print("MAX_ITER has to be >=1")
        flag = True
    if flag:
        exit(1)

    datapoints = []

    while True:
        try:
            row = input()
            row = row.split(',')
            row = [float(v) for v in row]
            datapoints.append(row)    
        except EOFError:
            break


    centroids = K_MEANS(datapoints, K, max_iter)
    for v in centroids:
        v = ['{:.4f}'.format(x) for x in v]
        print(','.join(v))
