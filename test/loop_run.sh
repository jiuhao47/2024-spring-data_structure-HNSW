#!/bin/bash
# {"all", "aircraft", "clothing", "flower", "food", "logo", "custom"};
# Dataset=$1
# inputDataSet=""
echo "[LOG]: Script Start!"
cd ..
cd build
# if [ "$Dataset" = "all" ]; then
#     inputDataSet="1"
# elif [ "$Dataset" = "aircraft" ]; then
#     inputDataSet="2"
# elif [ "$Dataset" = "clothing" ]; then
#     inputDataSet="3"
# elif [ "$Dataset" = "flower" ]; then
#     inputDataSet="4"
# elif [ "$Dataset" = "food" ]; then
#     inputDataSet="5"
# elif [ "$Dataset" = "logo" ]; then
#     inputDataSet="6"
# elif [ "$Dataset" = "custom" ]; then
#     inputDataSet="7"
# else
#     echo "Invalid dataset"
#     exit 1
# fi

# cmake .
# make
# echo "[LOG]: Build Done!"
# input="1 "$inputDataSet" 2 "$inputDataSet" 1 "
# for((search_index=2;search_index<11;search_index++))
# do
#     input=$input" 2 "$inputDataSet" "$search_index
# done
# input=$input" 0"

# echo $input | ./HNSW
rootPath="../data/output/output_"
for((round=0;round<3;round++))
do
    cd ..
    cd data
    python3 random_example.py
    python3 keras_vgg19_img_preprocess.py
    cd ..
    cd build
    for((inputDataSet=2;inputDataSet<7;inputDataSet++))
    do
        input="1 "$inputDataSet" 2 "$inputDataSet" 1 "
        for((search_index=2;search_index<51;search_index++))
        do
            input=$input" 2 "$inputDataSet" "$search_index
        done
        input=$input" 0"
        suffix="_"$inputDataSet"_"$search_index"_"$round".txt"
        # echo $input
        echo $input | ./HNSW_2 > $rootPath"2"$suffix
        echo $input | ./HNSW_3 > $rootPath"3"$suffix
        echo $input | ./HNSW_5 > $rootPath"5"$suffix
        echo $input | ./HNSW_10 > $rootPath"10"$suffix
        echo $input | ./HNSW_15 > $rootPath"15"$suffix
        echo $input | ./HNSW_20 > $rootPath"20"$suffix

        echo "[LOG]: "$inputDataSe"_"$round" Done!"
    done
done
echo "[LOG]: All Done!"
# 1 4 2 4 1 2 2 2 3 2 4 2 5 2 6 2 7 2 8 2 9 2 10 0
# build graph : 1 -> inputDataSet
# search graph : 2 -> inputDataSet -> 1...10