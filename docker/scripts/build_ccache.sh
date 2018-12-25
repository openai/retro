#!/bin/bash

curl -LO https://www.samba.org/ftp/ccache/ccache-3.4.2.tar.bz2
tar xjf ccache-*.tar.bz2
rm ccache-*.tar.bz2
pushd ccache*
./configure --prefix=/usr/local
make install
popd
rm -rf ccache

mkdir -p /usr/local/libexec/ccache
pushd /usr/local/libexec/ccache
ln -s /usr/local/bin/ccache cc
ln -s /usr/local/bin/ccache c++
popd
