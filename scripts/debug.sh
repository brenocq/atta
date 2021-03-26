#!/bin/bash
set -e

mkdir --parents build/linux
cd build/linux
cmake -D CMAKE_BUILD_TYPE=Debug -D VCPKG_TARGET_TRIPLET=x64-linux -D CMAKE_TOOLCHAIN_FILE=../vcpkg.linux/scripts/buildsystems/vcpkg.cmake ../..
make -j
gdb ./bin/attad