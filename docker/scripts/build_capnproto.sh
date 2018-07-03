#!/bin/bash

if [ -n "$CROSS" ]; then
	TOOLCHAIN=-DCMAKE_TOOLCHAIN_FILE=../../cmake/$CROSS.cmake
else
	CROSS=native
fi

cd capnproto/c++
mkdir build-$CROSS
cd build-$CROSS
cmake .. $TOOLCHAIN \
         -DCMAKE_INSTALL_PREFIX=$ROOT \
         -DBUILD_SHARED_LIBS=OFF \
         -DBUILD_STATIC_LIBS=ON \
         -DBUILD_TESTING=OFF
sed -i -E -e 's,\$<TARGET_FILE:CapnProto::capnp_tool>,/usr/bin/capnp,' -e 's,\$<TARGET_FILE:CapnProto::capnpc_cpp>,/usr/bin/capnpc-c++,' cmake/CapnProtoConfig.cmake
make -j2 install
cd ..
rm -rf build-$CROSS
