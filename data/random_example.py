import os
import random
import shutil


def list_all_files(directory):
    for dirpath, dirnames, filenames in os.walk(directory):
        for filename in filenames:
            yield os.path.join(dirpath, filename)


"""
source_directory = [
    "flower",
    "logo",
    "aircraft",
    "food",
    "clothing",
]
"""
source_directory = [
    "clothing",
]
root_directory = "/home/jiuhao/Workspace/Downloads/dataset/"

target_directory = "./input/"
for name in source_directory:
    directory = root_directory + name
    target = target_directory + name
    all_files = list(list_all_files(directory))
    selected_files = random.sample(all_files, 1000)

    for i, file in enumerate(selected_files):
        new_file_name = os.path.join(target, f"{i}.jpg")
        shutil.copy(file, new_file_name)
