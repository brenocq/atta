#!/bin/bash

echo "---------- Building ----------"
cd build/
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j

echo "---------- Executing ----------"
./bin/atta
