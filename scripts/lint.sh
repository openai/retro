#!/bin/bash

autopep8 -ir -j2 . &
find src \( -name \*.cpp -o -name \*.h \) -a \( -not -name libretro.h -a -not -name *.capnp.* -a -not -path \*CMake\* -a -not -path \*autogen\* \) -print0 | \
	xargs -0 -P2 clang-format -i &

wait
