#! /bin/bash

if [ ! -d "include" ]; then
    mkdir include
fi

# JSON
if [ ! -f "include/json.hpp" ]; then
    wget -P include/ https://github.com/nlohmann/json/releases/download/v3.7.3/json.hpp
fi

# Eigen
if [ ! -d "include/Eigen" ]; then
    mkdir tmp
    wget -P tmp/ https://gitlab.com/libeigen/eigen/-/archive/3.3.7/eigen-3.3.7.tar.gz
    tar -xvzf tmp/eigen-3.3.7.tar.gz -C tmp/
    mv tmp/eigen-3.3.7/Eigen include/
    rm -r tmp/
fi
