#!/bin/bash

echo "---------- Building ----------"
cd build/
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j

echo "---------- Executing ----------"
./bin/atta
