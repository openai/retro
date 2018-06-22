cmake_minimum_required(VERSION 3.1)
if(POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif()

find_package(Git QUIET)
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/third-party/pybind11/include)
    message(STATUS "Submodules seem to be missing. Attempting to clone now.")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init RESULT_VARIABLE SUCCESS)
    if(SUCCESS EQUAL 0)
        message(STATUS "Submodules cloned!")
    else()
        message(FATAL_ERROR "Failed to check out submodules. Aborting.")
    endif()
    unset(SUCCESS)
endif()

find_package(PythonInterp REQUIRED)

file(READ "${CMAKE_SOURCE_DIR}/VERSION" PROJECT_VERSION)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "$CMAKE_SOURCE_DIR}/VERSION")

project("Gym Retro" VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_MACOSX_DEPLOYMENT_TARGET 10.7)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-sign-compare -Wno-missing-field-initializers -fvisibility=hidden")
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mssse3")
endif()
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
option(BUILD_TESTS "Should tests be built" ON)
option(BUILD_MANYLINUX "Should use static libraries compatible with manylinux1" OFF)

set(BUILD_PYTHON ON CACHE BOOL "Build Python module")
mark_as_advanced(BUILD_PYTHON)

if(BUILD_PYTHON)
    find_package(PythonLibs REQUIRED)
endif()
if(WIN32 OR BUILD_MANYLINUX)
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
endif()
find_package(ZLIB REQUIRED)
find_package(PkgConfig)

pkg_search_module(LIBZIP QUIET libzip)

if(APPLE)
    find_package(Lua 5.1 EXACT REQUIRED)
    set(LUA_INCLUDE_DIRS "${LUA_INCLUDE_DIR}")
else()
    set(LUA_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/third-party/luajit/src")
    if(CMAKE_CROSSCOMPILING)
        if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
            set(BITS 64)
        else()
            set(BITS 32)
        endif()
        set(LUA_FLAGS "HOST_CC=gcc -m${BITS}" CROSS="${cross_prefix}" TARGET_SYS="${CMAKE_SYSTEM_NAME}")
    endif()
    set(LUA_LIBRARY "${LUA_INCLUDE_DIRS}/libluajit.a")
    add_custom_command(OUTPUT "${LUA_LIBRARY}"
        COMMAND $(MAKE) CFLAGS=-fPIC libluajit.a ${LUA_FLAGS}
        WORKING_DIRECTORY "${LUA_INCLUDE_DIRS}")
endif()

if(NOT WIN32)
    set(PYEXT_SUFFIX ".so" CACHE STRING "Suffix for Python extension modules")
else()
    set(PYEXT_SUFFIX ".pyd" CACHE STRING "Suffix for Python extension modules" FORCE)
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(STATIC_LDFLAGS "-static-libstdc++ -Wl,--exclude-libs,ALL")
endif()

set(PYLIB_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "Build directory that contains retro module")
file(MAKE_DIRECTORY "${PYLIB_DIRECTORY}/retro/cores")

set(CORES)
set(CORE_TARGETS)

function(add_core PLATFORM CORE_NAME)
    if(GIT_EXECUTABLE AND EXISTS ${CMAKE_SOURCE_DIR}/.git AND IS_DIRECTORY ${CMAKE_SOURCE_DIR}/.git)
        add_custom_target(${PLATFORM}-submodule
            COMMAND ${GIT_EXECUTABLE} submodule update --init cores/${PLATFORM}
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/.git/index")
    endif()
    set(TARGET_NAME ${CORE_NAME}_libretro${CMAKE_SHARED_LIBRARY_SUFFIX})
    get_filename_component(TARGET_PATH "${PYLIB_DIRECTORY}/retro/cores/${TARGET_NAME}" ABSOLUTE)
    list(APPEND CORES "${TARGET_PATH}")
    set(CORES "${CORES}" PARENT_SCOPE)
    set(SUBDIR)
    if(EXISTS "cores/${PLATFORM}/Makefile.libretro")
        set(MAKEFILE Makefile.libretro)
    elseif(EXISTS "cores/${PLATFORM}/Makefile")
        set(MAKEFILE Makefile)
    elseif(EXISTS "cores/${PLATFORM}/libretro/Makefile")
        set(MAKEFILE Makefile)
        set(SUBDIR libretro)
    else()
        message(FATAL_ERROR "Could not find Makefile. Did you clone submodules?")
    endif()

    if(EXISTS ${CMAKE_SOURCE_DIR}/.git)
        add_custom_command(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/retro/cores/${CORE_NAME}-version"
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/.git/modules/${PLATFORM}/HEAD" "${CMAKE_CURRENT_SOURCE_DIR}/retro/cores/${CORE_NAME}-version"
            DEPENDS ${PLATFORM}-submodule)
    else()
        add_custom_command(OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/retro/cores/${CORE_NAME}-version"
            COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_SOURCE_DIR}/retro/cores/${CORE_NAME}-version")
    endif()
    if(WIN32)
        set(LIBRETRO_PLATFORM platform=win)
    endif()
    if(WIN32 AND NOT CMAKE_CROSSCOMPILING)
        set(CORE_LDFLAGS LDFLAGS="-w ${STATIC_LDFLAGS}")
    else()
        set(CORE_LDFLAGS "LDFLAGS=-w ${STATIC_LDFLAGS}")
    endif()
    add_custom_command(OUTPUT "${TARGET_PATH}"
        COMMAND CFLAGS=-w CXXFLAGS=-w "${CORE_LDFLAGS}" $(MAKE) -f ${MAKEFILE} CC="${CMAKE_C_COMPILER}" CXX="${CMAKE_CXX_COMPILER}" ${LIBRETRO_PLATFORM}
        COMMAND ${CMAKE_COMMAND} -E copy "${CORE_NAME}_libretro*" "${TARGET_PATH}"
        WORKING_DIRECTORY "cores/${PLATFORM}/${SUBDIR}"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/retro/cores/${CORE_NAME}-version")
    unset(CORE_LDFLAGS)

    add_custom_target(${PLATFORM} ALL DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/retro/cores/${CORE_NAME}-version" "${TARGET_PATH}")
    list(APPEND CORE_TARGETS "${PLATFORM}")
    set(CORE_TARGETS "${CORE_TARGETS}" PARENT_SCOPE)

    add_custom_target(clean-${PLATFORM}
        COMMAND $(MAKE) -f ${MAKEFILE} clean
        COMMAND ${CMAKE_COMMAND} -E remove "${TARGET_PATH}"
        WORKING_DIRECTORY "cores/${PLATFORM}/${SUBDIR}")
endfunction()

add_core(genesis genesis_plus_gx)
add_core(atari2600 stella)

set(CLEAN_CORES)
foreach(CORE IN LISTS CORE_TARGETS)
    list(APPEND CLEAN_CORES "clean-${CORE}")
endforeach()

add_custom_target(clean-cores DEPENDS ${CLEAN_CORES})
add_custom_target(cores DEPENDS ${CORE_TARGETS})
unset(CLEAN_CORES)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)

if(LIBZIP_FOUND AND NOT LIBZIP_VERSION VERSION_LESS 1.0.0)
    include_directories(${LIBZIP_INCLUDE_DIRS})
    link_directories(${LIBZIP_LIBRARY_DIRS})
else()
    set(LIBZIP_LIBRARIES zip)
    add_subdirectory(third-party/libzip)
    include_directories(third-party/libzip third-party/libzip/lib)
endif()

include_directories(${LUA_INCLUDE_DIRS})

add_library(retro-base STATIC
    src/coreinfo.cpp
    src/data.cpp
    src/emulator.cpp
    src/imageops.cpp
    src/memory.cpp
    src/movie.cpp
    src/movie-bk2.cpp
    src/script.cpp
    src/script-lua.cpp
    src/utils.cpp
    src/zipfile.cpp
    ${LUA_LIBRARY})
target_link_libraries(retro-base ${ZLIB_LIBRARY} ${LIBZIP_LIBRARIES} ${LUA_LIBRARY} ${LUA_LIBRRAY})
add_dependencies(retro-base ${CORE_TARGETS})
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(retro-base dl)
endif()

include_directories(src retro third-party/pybind11/include third-party third-party/gtest/googletest/include ${PYTHON_INCLUDE_DIRS})
if(BUILD_PYTHON)
    add_library(retro SHARED src/retro.cpp)
    set_target_properties(retro PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PYLIB_DIRECTORY}/retro"
        RUNTIME_OUTPUT_DIRECTORY "${PYLIB_DIRECTORY}/retro"
        OUTPUT_NAME _retro
        PREFIX ""
        SUFFIX ${PYEXT_SUFFIX})
    if(APPLE)
        set(PYBIND_LIBS "-undefined dynamic_lookup")
    elseif(WIN32)
        if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
            add_definitions(-DMS_WIN64)
        endif()
        set(PYBIND_LIBS "${PYTHON_LIBRARY}")
    endif()
    target_link_libraries(retro retro-base ${PYBIND_LIBS} ${STATIC_LDFLAGS})
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(third-party/gtest/googlemock)
    add_subdirectory(tests)
endif()
