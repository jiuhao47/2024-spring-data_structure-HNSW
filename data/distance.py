import numpy as np


def Euclidean_distance(index_x, index_y):
    x = np.loadtxt("../data/preprocess/" + str(index_x) + ".txt")
    y = np.loadtxt("../data/preprocess/" + str(index_y) + ".txt")
    return int(np.linalg.norm(x - y))


def Cosine_similarity(index_x, index_y):
    x = np.loadtxt("../data/preprocess/" + str(index_x) + ".txt")
    y = np.loadtxt("../data/preprocess/" + str(index_y) + ".txt")
    return np.dot(x, y) / (np.linalg.norm(x) * np.linalg.norm(y))
