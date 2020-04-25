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
	mkdir -p $ROOT/lib/python$MAJOR.$MINOR include include/cpython include/internal
	mv libs_python.lib $ROOT/lib/python$MAJOR.$MINOR/python$MAJOR$MINOR.lib
	mv libs_python_stable.lib $ROOT/lib/python$MAJOR.$MINOR/python$MAJOR.lib
	mv libs_libpython.a $ROOT/lib/python$MAJOR.$MINOR/libpython.a
	find . -maxdepth 1 -name include_cpython_\*.h -print0 | xargs -0 -n1 -I% bash -c 'mv % $(echo % | sed -e "s,_,/," | sed -e "s,_,/,")'
	find . -maxdepth 1 -name include_internal_\*.h -print0 | xargs -0 -n1 -I% bash -c 'mv % $(echo % | sed -e "s,_,/," | sed -e "s,_,/,")'
	find . -maxdepth 1 -name include_\*.h -print0 | xargs -0 -n1 -I% bash -c 'mv % $(echo % | sed -e "s,_,/,")'
	mv include $ROOT/include/python$MAJOR.$MINOR
	rm dev.msi

	# python 3.8 is missing this file, create it from the dll
	# https://github.com/openai/retro/pull/201#issuecomment-619326417
	if [[ ! -f $ROOT/lib/python$MAJOR.$MINOR/libpython.a ]]
	then
		curl -LO https://www.python.org/ftp/python/$MAJOR.$MINOR.$PATCH/$PLATFORM/core.msi
		7z x core.msi
		apt-get install --yes mingw-w64-tools
		gendef python.dll
		/usr/bin/x86_64-w64-mingw32-dlltool --as-flags=--64 -m i386:x86-64 -k --output-lib $ROOT/lib/python$MAJOR.$MINOR/libpython.a --input-def python.def
		rm *.dll *.def core.msi
	fi
}

install_python 3 5 4
install_python 3 6 6
install_python 3 7 0
install_python 3 8 2
