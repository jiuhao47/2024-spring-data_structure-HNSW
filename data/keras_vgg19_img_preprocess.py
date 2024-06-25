import tensorflow as tf
from keras._tf_keras.keras.preprocessing import image
from keras._tf_keras.keras.applications.vgg19 import VGG19, preprocess_input
import numpy as np
import os

"""
source_directory = [
    "flower",
    "logo",
    "aircraft",
    "food",
    "clothing",
    "custom",
]
"""
source_directory = [
    "aircraft",
]
file_path = "./imgInput/"
search_path = "./imgInput/search/"
# 加载预训练的VGG19模型，不包括顶部的全连接层
model = VGG19(weights="imagenet", include_top=False)


def extract_features(img_path):
    # 加载图像，调整大小为224x224（VGG19的输入大小）
    img = image.load_img(img_path, target_size=(224, 224))
    # 将图像转换为NumPy数组
    x = image.img_to_array(img)
    # 添加一个维度，使其变为(1, 224, 224, 3)
    x = np.expand_dims(x, axis=0)
    # 预处理图像（归一化等）
    x = preprocess_input(x)
    # 提取特征
    features = model.predict(x)
    # 展平特征为一维数组
    return features.flatten()


# 提取两个图像的特征
for name in source_directory:
    directory = file_path + name
    os.makedirs("./preprocess/" + name, exist_ok=True)
    for file in sorted(os.listdir(directory)):
        if file.endswith(".jpg"):
            img_path = os.path.join(directory, file)
            features = extract_features(img_path)
            index = file[:-4]
            print(index)
            with open("./preprocess/" + name + "/" + index + ".txt", "w") as f:
                f.write(" ".join(str(x) for x in features))
                f.close()
directory = search_path
for file in sorted(os.listdir(directory)):
    if file.endswith(".jpg"):
        img_path = os.path.join(directory, file)
        features = extract_features(img_path)
        index = file[:-4]
        print(index)
        with open("./preprocess/search/" + index + ".txt", "w") as f:
            f.write(" ".join(str(x) for x in features))
            f.close()
