#!/usr/bin/env bash

rm -r build
rm test_gdal_opencv
# build
mkdir build
cd build
cmake ..
make
cd ..


# run



