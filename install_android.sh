#!/bin/bash
set -e

cd build

if [[ -z "${ANDROID_NDK_HOME}" ]]; then
	echo "Please first set the enviroment variable ANDROID_NDK_HOME to your NDK path (Usually ~/Android/Sdk/ndk)"
else
	echo "ANDROID_NDK_HOME=${ANDROID_NDK_HOME}"
	echo "VCPKG_ROOT=${VCPKG_ROOT}"
fi

if [ -d "vcpkg.android" ]
then
	echo "Vcpkg repo already cloned"
	cd vcpkg.android
else
	echo "Cloning vcpkg to install android packages"
	git clone https://github.com/Microsoft/vcpkg.git vcpkg.android
	cd vcpkg.android
	git checkout 2020.07
	./bootstrap-vcpkg.sh

	# Create triplets to android
	echo "
set(VCPKG_TARGET_ARCHITECTURE arm)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)
set(VCPKG_CMAKE_SYSTEM_NAME Android)
	" > triplets/community/arm-android.cmake

	echo "
set(VCPKG_TARGET_ARCHITECTURE arm64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)
set(VCPKG_CMAKE_SYSTEM_NAME Android)
	" > triplets/community/arm64-android.cmake

	echo "
set(VCPKG_TARGET_ARCHITECTURE x86)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)
set(VCPKG_CMAKE_SYSTEM_NAME Android)
	" > triplets/community/x86-android.cmake

	echo "
set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)
set(VCPKG_CMAKE_SYSTEM_NAME Android)
	" > triplets/community/x64-android.cmake
fi
./vcpkg install jsoncpp:arm-android  jsoncpp:arm64-android  jsoncpp:x64-android  jsoncpp:x86-android

#./vcpkg install \
#	glm:arm-android --debug
	#glfw3:arm-android \
	#glm:arm-android \
	#imgui:arm-android \
	#stb:arm-android \
	#tinyobjloader:arm-android 
	
