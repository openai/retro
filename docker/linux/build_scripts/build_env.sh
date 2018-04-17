# source me

PYTHON_DOWNLOAD_URL=https://www.python.org/ftp/python
CPYTHON_VERSIONS="3.5.5 3.6.5"

# openssl version to build, with expected sha256 hash of .tar.gz
# archive.
OPENSSL_ROOT=openssl-1.0.2o
OPENSSL_HASH=ec3f5c9714ba0fd45cb4e087301eb1336c317e0d20b575a125050470e8089e4d
OPENSSL_DOWNLOAD_URL=https://www.openssl.org/source

# Update to slightly newer, verified Git commit:
# https://github.com/NixOS/patchelf/commit/2a9cefd7d637d160d12dc7946393778fa8abbc58
PATCHELF_VERSION=2a9cefd7d637d160d12dc7946393778fa8abbc58
PATCHELF_HASH=12da4727f09be42ae0b54878e1b8e86d85cb7a5b595731cdc1a0a170c4873c6d

CURL_ROOT=curl-7.59.0
CURL_HASH=099d9c32dc7b8958ca592597c9fabccdf4c08cfb7c114ff1afbbc4c6f13c9e9e
CURL_DOWNLOAD_URL=https://curl.haxx.se/download

AUTOCONF_ROOT=autoconf-2.69
AUTOCONF_HASH=954bd69b391edc12d6a4a51a2dd1476543da5c6bbf05a95b59dc0dd6fd4c2969
AUTOCONF_DOWNLOAD_URL=http://ftp.gnu.org/gnu/autoconf
AUTOMAKE_ROOT=automake-1.15
AUTOMAKE_HASH=7946e945a96e28152ba5a6beb0625ca715c6e32ac55f2e353ef54def0c8ed924
AUTOMAKE_DOWNLOAD_URL=http://ftp.gnu.org/gnu/automake
LIBTOOL_ROOT=libtool-2.4.6
LIBTOOL_HASH=e3bd4d5d3d025a36c21dd6af7ea818a2afcd4dfc1ea5a17b39d7854bcd0c06e3
LIBTOOL_DOWNLOAD_URL=http://ftp.gnu.org/gnu/libtool

SQLITE_AUTOCONF_VERSION=sqlite-autoconf-3210000
# Homebrew saw the same hash: https://github.com/Homebrew/homebrew-core/blob/e3a8622111ecefe444194cade5cca3c69165e26c/Formula/sqlite.rb#L6
SQLITE_AUTOCONF_HASH=d7dd516775005ad87a57f428b6f86afd206cb341722927f104d3f0cf65fbbbe3

GIT_ROOT=2.16.2
GIT_HASH=cbdc2398204c7b7bed64f28265870aabe40dd3cd5c0455f7d315570ad7f7f5c8
GIT_DOWNLOAD_URL=https://github.com/git/git/archive

GET_PIP_URL=https://bootstrap.pypa.io/get-pip.py
EPEL_RPM_HASH=0dcc89f9bf67a2a515bad64569b7a9615edc5e018f676a578d5fd0f17d3c81d4
DEVTOOLS_HASH=a8ebeb4bed624700f727179e6ef771dafe47651131a00a78b342251415646acc
