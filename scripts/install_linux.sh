#!/bin/bash
set -e

cd build
rm -rf vcpkg.linux
git clone https://github.com/Microsoft/vcpkg.git vcpkg.linux
cd vcpkg.linux
git checkout 2020.07
./bootstrap-vcpkg.sh

./vcpkg install \
	glfw3:x64-linux \
	stb:x64-linux \
	tinyobjloader:x64-linux
	#glm:x64-linux \
	#imgui:x64-linux \
