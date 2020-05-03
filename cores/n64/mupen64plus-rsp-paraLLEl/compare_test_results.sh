#!/bin/bash

./build_android_aarch64.sh || exit 1
cd build-android-aarch64
echo "Running Android test suite ..."
ctest --no-label-summary --verbose >android-aarch64.log || exit 1
cd ..

./build_native.sh || exit 1
cd build-native
echo "Running native test suite ..."
ctest --no-label-summary --verbose >native.log || exit 1
cd ..

echo "Comparing output ..."
echo "=============================="
diff -Naur build-native/native.log build-android-aarch64/android-aarch64.log
echo "=============================="
