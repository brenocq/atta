#!/bin/bash
set -e

mkdir --parents build/linux
cd build/linux
cmake -D CMAKE_BUILD_TYPE=Release -D VCPKG_TARGET_TRIPLET=x64-linux -D CMAKE_TOOLCHAIN_FILE=../vcpkg.linux/scripts/buildsystems/vcpkg.cmake ../..
make -j
source /home/breno/Programs/Vulkan/1.2.154.0/setup-env.sh
./bin/simulator