#!/bin/sh
test=`basename $0 | sed -e 's|\.arm$||'`
./lightning -mthumb=0 $srcdir/$test.tst | tr -d \\r > $test.out
if test $? != 0; then
  exit $?
fi

cmp -s $srcdir/$test.ok $test.out
result=$?
if test $result != 0; then
    diff $srcdir/$test.ok $test.out
    rm $test.out
    exit 1
fi
rm $test.out
