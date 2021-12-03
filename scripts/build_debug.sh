#!/bin/bash

echo "---------- Building ----------"
mkdir -p build/ && cd build/
cmake -DCMAKE_BUILD_TYPE=Debug ..
# cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ ..

make -j

echo "---------- Executing ----------"
./bin/atta
