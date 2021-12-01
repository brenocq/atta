#!/bin/bash

echo "---------- Building ----------"
cd build/
cmake -DATTA_BUILD_DOCS=ON -DATTA_BUILD_TESTS=OFF ..
make -j
