#!/bin/bash

echo "---------- Building ----------"
cd build/
emcmake cmake -DCMAKE_BUILD_TYPE=Debug -DATTA_WEB_BUILD_MODULE=OFF ..
make -j

echo "---------- Running ----------"
emrun bin/atta.html
