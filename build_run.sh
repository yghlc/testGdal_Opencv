#!/usr/bin/env bash

rm -r build

# build
mkdir build
cd build
cmake ..
make
cd ..


# run



