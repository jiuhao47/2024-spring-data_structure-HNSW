from PIL import Image
import numpy as np
import os

file_path = (
    "/home/jiuhao/Workspace/2024-Spring/data_structure/Ex/projects/NSW/data/input"
)


def process_image(file):
    img = Image.open(file)
    img = img.resize((128, 128))
    img = img.convert("L")
    img_array = np.array(img)
    img_vector = img_array.flatten()
    return img_vector


def distance_Euclidean(v1, v2):
    return np.linalg.norm(v1 - v2)


def distance_cosine(v1, v2):
    return np.dot(v1, v2) / (np.linalg.norm(v1) * np.linalg.norm(v2))


L = []
for file in sorted(os.listdir(file_path)):
    if file.endswith(".jpg"):
        img_vector = process_image(os.path.join(file_path, file))
        L.append(img_vector)
        print(file)
        print(img_vector)
for i in range(len(L)):
    for j in range(i + 1, len(L)):
        print(i, j, "D_E_ij =", int(distance_Euclidean(L[i], L[j])))

for i in range(len(L)):
    for j in range(i + 1, len(L)):
        print(i, j, "D_Cos_ij =", distance_cosine(L[i], L[j]))
# TODO: 距离函数仍需考虑
