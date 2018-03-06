#!/bin/bash

case $CROSS in
win32)
	PLATFORM=win32
	;;
win64)
	PLATFORM=amd64
	;;
esac

curl -LO https://www.python.org/ftp/python/3.6.4/$PLATFORM/dev.msi
7z x dev.msi
mkdir -p $ROOT/lib/python3.6 include
mv libs_python.lib $ROOT/lib/python3.6/python36.lib
mv libs_python_stable.lib $ROOT/lib/python3.6/python3.lib
mv libs_libpython.a $ROOT/lib/python3.6/libpython.a
find . -maxdepth 1 -name include_\*.h -print0 | xargs -0 -n1 -I% bash -c 'mv % $(echo % | sed -e "s,_,/,")'
mv include $ROOT/include/python3.6
