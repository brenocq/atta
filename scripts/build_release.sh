#!/bin/bash

echo "---------- Building ----------"
mkdir -p build/ && cd build/
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j

echo "---------- Executing ----------"
./bin/atta
