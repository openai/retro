#!/bin/bash

if [ -n "$CROSS" ]; then
	TOOLCHAIN=-DCMAKE_TOOLCHAIN_FILE=../../cmake/$CROSS.cmake
else
	CROSS=native
fi

cd libzip
sed -i -Ee "/ADD_SUBDIRECTORY\\((examples|regress|src)\\)/d" CMakeLists.txt
mkdir build-$CROSS
cd build-$CROSS
rm -f $ROOT/lib/libz.dll.a # Prevent shared builds
cmake .. $TOOLCHAIN \
         -DCMAKE_INSTALL_PREFIX=$ROOT \
         -DBUILD_SHARED_LIBS=OFF \
         -DBUILD_STATIC_LIBS=ON
make -j2 install
cd ..
rm -rf build-$CROSS
