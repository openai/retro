#!/bin/bash
# Prefetch tarballs so they don't need to be fetched in the container (which has
# very old tools).
#
# usage: prefetch.sh <output_dir> [name ...]
set -ex

MY_DIR=$(dirname "${BASH_SOURCE[0]}")
. $MY_DIR/build_env.sh
. $MY_DIR/build_utils.sh

SOURCES=$MY_DIR/../sources

[ -d "$SOURCES" ] || mkdir "$SOURCES"
cd "$SOURCES"

for name in "$@"; do
    name_upper=$(echo "${name}" | tr [:lower:] [:upper:])
    root=${name_upper}_ROOT
    ext=${name_upper}_EXTENSION
    url=${name_upper}_DOWNLOAD_URL
    file=${!root}${!ext:-.tar.gz}
    fetch_source $file ${!url}
done
