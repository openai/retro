#!/bin/bash

case $CROSS in
win32)
	PLATFORM=win32
	;;
win64)
	PLATFORM=amd64
	;;
esac

function install_python {
	local MAJOR=$1
	local MINOR=$2
	local PATCH=$3

	curl -LO https://www.python.org/ftp/python/$MAJOR.$MINOR.$PATCH/$PLATFORM/dev.msi
	7z x dev.msi
	mkdir -p $ROOT/lib/python$MAJOR.$MINOR include
	mv libs_python.lib $ROOT/lib/python$MAJOR.$MINOR/python$MAJOR$MINOR.lib
	mv libs_python_stable.lib $ROOT/lib/python$MAJOR.$MINOR/python$MAJOR.lib
	mv libs_libpython.a $ROOT/lib/python$MAJOR.$MINOR/libpython.a
	find . -maxdepth 1 -name include_\*.h -print0 | xargs -0 -n1 -I% bash -c 'mv % $(echo % | sed -e "s,_,/,")'
	mv include $ROOT/include/python$MAJOR.$MINOR
	rm dev.msi
}

install_python 3 5 4
install_python 3 6 5
