#!/bin/sh

#  make_zipconf.sh: create platform specific include file zipconf.h
#  Copyright (C) 1999-2011 Dieter Baron and Thomas Klausner
#
#  This file is part of libzip, a library to manipulate ZIP archives.
#  The authors can be contacted at <libzip@nih.at>
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in
#     the documentation and/or other materials provided with the
#     distribution.
#  3. The names of the authors may not be used to endorse or promote
#     products derived from this software without specific prior
#     written permission.
# 
#  THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS
#  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
#  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
#  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
#  IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
#  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
#  IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


set -e

define_type()
{
    short=$1
    long=$2
    bytes=$3
    infile="$4"
    outfile="$5"

    bits=`expr $bytes '*' 8`
    type="${short}int${bits}"
    TYPE=`echo $type | tr '[a-z]' '[A-Z]'`
    if grep "define HAVE_${TYPE}_T" "$infile" > /dev/null
    then
	echo "typedef ${type}_t zip_${type}_t;" >> "$outfile"
	LTYPE="$TYPE"
    else
	SHORT=`echo $short | tr '[a-z]' '[A-Z]'`
	if [ -z "$SHORT" ]
	then
	    SHORT='S'
	fi
	if [ "$bytes" -eq 1 ]
	then
	    if [ -z "$long" ]
	    then
		long='signed'
	    fi
	    echo "typedef $long char ${type}_t;" >> $outfile
	    LTYPE="${SHORT}CHAR"
	else
	    ctype=`sed -n "s/.define SIZEOF_\([A-Z_]*\) $bytes/\1/p" "$infile" \
		| head -1 | tr '[A-Z_]' '[a-z ]'`
	    if [ -z "$ctype" ]
	    then
		echo "$0: no $bits bit type found" >&2
		exit 1
	    fi
	    echo "typedef $long $ctype ${type}_t;" >> "$outfile"
	    case "$ctype" in
		short) LTYPE=${SHORT}SHRT;;
		int) LTYPE=${SHORT}INT;;
                long) LTYPE=${SHORT}LONG;;
		"long long") LTYPE=${SHORT}LLONG;;
            esac
	fi
    fi

    if [ -z "$long" ]
    then
	echo "#define ZIP_${TYPE}_MIN ${LTYPE}_MIN" >> "$outfile"
    fi
    echo "#define ZIP_${TYPE}_MAX ${LTYPE}_MAX" >> "$outfile"
    echo >> "$outfile"
}


if [ "$#" -ne 2 ]
then
    echo "Usage: $0 config_h_file out_file" >&2
    echo "       e.g. $0 ../config.h zip_err_str.c" >&2
    exit 1
fi

if [ "$1" = "$2" ]
then
    echo "$0: error: output file = input file" >&2
    exit 1
fi

cat <<EOF > "$2.$$"
#ifndef _HAD_ZIPCONF_H
#define _HAD_ZIPCONF_H

/*
   zipconf.h -- platform specific include file

   This file was generated automatically by $0
   based on $1.
 */

EOF

version=`sed -n 's/^#define VERSION "\(.*\)"/\1/p' "$1"`

version_major=`expr "$version" : '^\([0-9]*\)' || true`
version_minor=`expr "$version" : '^[0-9]*\.\([0-9]*\)' || true`
version_micro=`expr "$version" : '^[0-9]*\.[0-9]\.\([0-9]*\)' || true`

if [ -z "$version_major" ]
then
    version_major=0
fi
if [ -z "$version_minor" ]
then
    version_minor=0
fi
if [ -z "$version_micro" ]
then
    version_micro=0
fi

cat <<EOF >> "$2.$$"
#define LIBZIP_VERSION "$version"
#define LIBZIP_VERSION_MAJOR $version_major
#define LIBZIP_VERSION_MINOR $version_minor
#define LIBZIP_VERSION_MICRO $version_micro

EOF

if grep 'define HAVE_INTTYPES_H' "$1" > /dev/null
then
    echo '#include <inttypes.h>' >> "$2.$$"
else
    echo '#include <limits.h>' >> "$2.$$"
fi
echo >> "$2.$$"

for size in 1 2 4 8
do
    define_type '' '' $size "$1" "$2.$$"
    define_type u unsigned $size "$1" "$2.$$"
done

echo >> "$2.$$"
echo '#endif /* zipconf.h */' >> "$2.$$"

mv "$2.$$" "$2"
