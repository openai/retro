set(CMAKE_SYSTEM_NAME Windows CACHE INTERNAL "system name")
set(CMAKE_SYSTEM_PROCESSOR i686)
set(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
set(CMAKE_RC_COMPILER i686-w64-mingw32-windres)

set(CMAKE_PREFIX_PATH /usr/i686-w64-mingw32/)
set(CMAKE_FIND_ROOT_PATH /usr/i686-w64-mingw32/)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(cross_prefix i686-w64-mingw32-)
set(link_flags "-static-libgcc -static-libstdc++")

set(PYTHON_LIBRARY /usr/i686-w64-mingw32/lib/python$ENV{PYVER}/libpython.a)
set(PYTHON_INCLUDE_DIR /usr/i686-w64-mingw32/include/python$ENV{PYVER}/)

set(CMAKE_EXE_LINKER_FLAGS ${link_flags} CACHE INTERNAL "exe link flags")
set(CMAKE_MODULE_LINKER_FLAGS ${link_flags} CACHE INTERNAL "module link flags")
set(CMAKE_SHARED_LINKER_FLAGS "${link_flags} -Wl,--export-all-symbols" CACHE INTERNAL "shared link flags")
add_definitions(-DHAVE_INTPTR_T -DHAVE_UINTPTR_T)
