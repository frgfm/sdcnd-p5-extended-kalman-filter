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
    wget https://gitlab.com/libeigen/eigen/-/archive/3.3.7/eigen-3.3.7.tar.gz
    tar -xvzf eigen-3.3.7.tar.gz
    mv eigen-3.3.7/Eigen include/
    rm -rf eigen-3.3.7
fi

# spdlog
if [ ! -d "include/spdlog" ]; then
    wget https://github.com/gabime/spdlog/archive/v1.5.0.tar.gz
    tar -xvzf v1.5.0.tar.gz
    mv spdlog-1.5.0/include/spdlog include/
    rm -rf v1.5.0.tar.gz
fi

# Catch
if [ ! -d "lib" ]; then
    mkdir lib
fi
if [ ! -f "lib/catch.hpp" ]; then
    wget -P lib/ https://github.com/catchorg/Catch2/releases/download/v2.11.3/catch.hpp
fi
