import os
import random
import shutil


def list_all_files(directory):
    for dirpath, dirnames, filenames in os.walk(directory):
        for filename in filenames:
            yield os.path.join(dirpath, filename)


source_directory = [
    "flower",
    "logo",
    "aircraft",
    "food",
    "clothing",
]
"""
source_directory = [
    "logo",
]
"""
root_directory = "/home/jiuhao/Workspace/Downloads/dataset/"

target_directory = "./imgInput/"
target_search_directory = "./imgInput/search/"
m = 1
for name in source_directory:
    os.makedirs(target_directory + name, exist_ok=True)
    directory = root_directory + name
    target = target_directory + name
    all_files = list(list_all_files(directory))
    selected_files = random.sample(all_files, 1050)
    for i, file in enumerate(selected_files):
        if i < 1000:
            new_file_name = os.path.join(target, f"{i}.jpg")
            shutil.copy(file, new_file_name)
        else:
            os.makedirs(target_search_directory + name, exist_ok=True)
            new_file_name = os.path.join(
                target_search_directory + name, f"{i-1000+1}.jpg"
            )
            if i < 1010:
                os.makedirs(target_search_directory + "all", exist_ok=True)
                new_all_search_file_name = (
                    target_search_directory + "all/" + str(m) + ".jpg"
                )
                m = m + 1
                shutil.copy(file, new_all_search_file_name)
            shutil.copy(file, new_file_name)
all_directory = target_directory + "all/"
j = 0
os.makedirs(all_directory, exist_ok=True)
for name in source_directory:
    directory = target_directory + name
    all_files = list(list_all_files(directory))
    for i, file in enumerate(all_files):
        new_file_name = all_directory + str(j) + ".jpg"
        j = j + 1
        # print(new_file_name)
        shutil.copy(file, new_file_name)
print("[LOG]: Random example done!")
