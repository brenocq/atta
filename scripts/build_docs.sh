#!/bin/bash

echo "---------- Building ----------"
mkdir -p build/ && cd build/
cmake -DATTA_BUILD_DOCS=ON -DATTA_BUILD_TESTS=OFF ..
make -j
