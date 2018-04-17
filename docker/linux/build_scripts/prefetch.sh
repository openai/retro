#!/bin/bash
# Prefetch tarballs so they don't need to be fetched in the container (which has
# very old tools). Requires Bash 4.0+, but this is not run inside the build context.
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
    root=${name^^}_ROOT
    ext=${name^^}_EXTENSION
    url=${name^^}_DOWNLOAD_URL
    file=${!root}${!ext:-.tar.gz}
    fetch_source $file ${!url}
done
