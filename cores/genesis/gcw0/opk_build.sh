#!/bin/sh
rm -rf genplus.opk
mksquashfs gen_gcw0 opk-data/* genplus.opk -all-root -noappend -no-exports -no-xattrs