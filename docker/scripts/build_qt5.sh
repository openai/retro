#!/bin/bash

case $CROSS in
win32)
	CROSS_COMPILE=i686-w64-mingw32-
	;;
win64)
	CROSS_COMPILE=x86_64-w64-mingw32-
	;;
esac

if [ -n "$CROSS_COMPILE" ]; then
	CROSS_OPTS="-xplatform win32-g++ -device-option CROSS_COMPILE=$CROSS_COMPILE"
fi

REMOVE_QT="
3d
activeqt
canvas3d
connectivity
declarative
doc
docgallery
enginio
feedback
graphicaleffects
imageformats
location
pim
purchasing
qa
quick1
quickcontrols
quickcontrols2
repotools
script
sensors
serialbus
serialport
svg
systems
wayland
webchannel
webengine
webkit
webkit-examples
websockets
webview
xmlpatterns
"

if [ -d qt5 ]; then
	cd qt5
	git clean -dfx
	git submodule foreach git clean -dfx
else
	git clone https://github.com/qt/qt5.git -b v5.6.3
	cd qt5
	git submodule init
	for dir in $REMOVE_QT; do
	    git submodule deinit -f qt$dir
	done
	git submodule update
fi

./configure \
	-prefix $ROOT \
	-opensource \
	-confirm-license \
	-platform linux-g++ \
	$CROSS_OPTS \
	-release \
	-I $ROOT/include \
	-L $ROOT/lib \
	-v \
	-static \
	-c++std c++14 \
	-opengl desktop \
	-no-pch \
	-no-avx2 \
	-nomake examples \
	-nomake tools \
	-nomake tests \
	-no-compile-examples \
	-no-freetype \
	-no-icu \
	-no-gif \
	-no-sql-odbc \
	-no-harfbuzz \
	-no-openssl \
	-no-dbus && make -j2 install
