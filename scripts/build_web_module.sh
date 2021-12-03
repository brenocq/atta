#!/bin/bash

# Check if emscripten is installed
if ! command -v emcmake &> /dev/null
then
    echo -e "\033[1m\033[31m[Error] \033[0m\033[31mEmscripten is not installed, please follow the instruction here: \033[37mhttps://emscripten.org/docs/getting_started/downloads.html"
    exit
fi

echo "---------- Building ----------"
mkdir -p build/ && cd build/
emcmake cmake -DCMAKE_BUILD_TYPE=Release -DATTA_WEB_BUILD_MODULE=ON ..
make -j
