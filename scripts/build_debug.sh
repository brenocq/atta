#!/bin/bash

echo "---------- Building ----------"
cd build/
#cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ ..

make -j

echo "---------- Executing ----------"
./bin/atta
