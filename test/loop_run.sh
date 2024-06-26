#!/bin/bash
# {"all", "aircraft", "clothing", "flower", "food", "logo", "custom"};
Dataset=$1
inputDataSet=""
cd ..
cd build
if [ "$Dataset" = "all" ]; then
    inputDataSet="1"
elif [ "$Dataset" = "aircraft" ]; then
    inputDataSet="2"
elif [ "$Dataset" = "clothing" ]; then
    inputDataSet="3"
elif [ "$Dataset" = "flower" ]; then
    inputDataSet="4"
elif [ "$Dataset" = "food" ]; then
    inputDataSet="5"
elif [ "$Dataset" = "logo" ]; then
    inputDataSet="6"
elif [ "$Dataset" = "custom" ]; then
    inputDataSet="7"
else
    echo "Invalid dataset"
    exit 1
fi

cmake .
make

input="1 "$inputDataSet" 2 "$inputDataSet" 1 "
for((search_index=2;search_index<11;search_index++))
do
    input=$input" 2 "$inputDataSet" "$search_index
done
input=$input" 0"

echo $input | ./HNSW
# for((round=0;round<3;round++))
# do
#     python3 ../data/random_example.py
#     python3 ../data/keras_vgg19_img_preprocess.py
#     for((inputDataSet=1;inputDataSet<8;inputDataSet++))
#     do
#         input="1 "$inputDataSet" 2 "$inputDataSet" 1 "
#         for((search_index=2;search_index<11;search_index++))
#         do
#             input=$input" 2 "$inputDataSet" "$search_index
#         done
#         input=$input" 0"
#         echo $input | ./HNSW > ../output/output_$inputDataSet"_"$search_index"_"$round".txt"
#     done
# done
# 1 4 2 4 1 2 2 2 3 2 4 2 5 2 6 2 7 2 8 2 9 2 10 0
# build graph : 1 -> inputDataSet
# search graph : 2 -> inputDataSet -> 1...10