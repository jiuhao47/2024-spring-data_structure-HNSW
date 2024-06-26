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

input="1 "$inputDataSet" 2 "$inputDataSet
for((search_index=1;search_index<11;search_index++))
do
    input=$input" "$search_index
done
echo $input 

#echo $input | ./HNSW

# build graph : 1 -> inputDataSet
# search graph : 2 -> inputDataSet -> 1...10