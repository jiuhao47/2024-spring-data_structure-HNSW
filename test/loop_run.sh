#!/bin/zsh
# {"all", "aircraft", "clothing", "flower", "food", "logo", "custom"};
Dataset=$1
cd ..
cd build
if [ "$Dataset" = "all" ]; then
    echo "all"
elif [ "$Dataset" = "aircraft" ]; then
    echo "aircraft"
elif [ "$Dataset" = "clothing" ]; then
    echo "clothing"

#./run.sh