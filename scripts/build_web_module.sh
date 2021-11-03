#!/bin/bash

echo "---------- Building ----------"
cd build/
emcmake cmake -DCMAKE_BUILD_TYPE=Release -DATTA_WEB_BUILD_MODULE=ON ..
make -j
