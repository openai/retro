libzip uses [cmake](https://cmake.org) to build.

For running the tests, you need to have [perl](https://www.perl.org).

You'll need [zlib](http://www.zlib.net/) (at least version 1.1.2). It
comes with most operating systems.

For supporting bzip2-compressed zip archives, you need
[bzip2](http://bzip.org/).

The basic usage is
```sh
mkdir build
cd build
cmake ..
make
make test
make install
```

Some useful parameters you can pass to `cmake` with `-Dparameter=value`:
- `BUILD_SHARED_LIBS`: set to `ON` or `OFF` to enable/disable building
  of shared libraries, defaults to `ON`
- `CMAKE_INSTALL_PREFIX`: for setting the installation path

You can get verbose build output with by passing `VERBOSE=1` to `make`.

You can also check the [cmake FAQ](https://cmake.org/Wiki/CMake_FAQ).
