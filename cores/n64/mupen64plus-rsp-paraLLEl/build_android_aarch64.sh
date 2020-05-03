#!/bin/bash

if [ -z $ANDROID_HOME ]; then
	echo "Please define ANDROID_HOME environment variable to where Android SDK is installed, usually ~/Android/Sdk."
	exit 1
fi

mkdir -p build-android-aarch64
cd build-android-aarch64
cmake .. -DCMAKE_TOOLCHAIN_FILE="$ANDROID_HOME/ndk-bundle/build/cmake/android.toolchain.cmake" \
	-DANDROID_ABI=arm64-v8a \
	-DCMAKE_BUILD_TYPE=Release \
	-DANDROID_PLATFORM=android-26 \
	-DPARALLEL_RSP_DEBUG_JIT=OFF
cmake --build . --parallel
cd ..
