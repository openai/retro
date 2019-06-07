#!/bin/sh

DIR=tmp.$$

mkdir -p $DIR/32 $DIR/64

(cd $DIR/32; ../../../configure CFLAGS=-m32)
(cd $DIR/64; ../../../configure CFLAGS=-m64)

diff -D __LP64__ $DIR/32/config.h $DIR/64/config.h > config.h

rm -r $DIR
