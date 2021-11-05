#!/bin/bash

echo "---------- Building ----------"
cd build/
#cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++ ..
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ ..
make -j

echo "---------- Running atta ----------"
./bin/atta
