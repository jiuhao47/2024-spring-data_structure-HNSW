import tensorflow as tf
from keras._tf_keras.keras.preprocessing import image
from keras._tf_keras.keras.applications.vgg19 import VGG19, preprocess_input
import numpy as np
import os

file_path = "./input"
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
for file in sorted(os.listdir(file_path)):
    if file.endswith(".jpg"):
        img_path = os.path.join(file_path, file)
        features = extract_features(img_path)
        print(len(features))
        index = file[:-4]
        with open("./preprocess/" + index + ".txt", "w") as f:
            f.write(" ".join(str(x) for x in features))
            f.close()


# TODO: 是否需要建立文件表来查找距离，而不是计算输出（性能考虑）
# TODO: C调用python函数与C单独实现的性能考虑
# TODO: 命令行前端接口，调试接口，批处理脚本，大类评测，小类评测，数据处理接口，距离函数
