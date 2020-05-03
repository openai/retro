project (mman-win32 C)

cmake_minimum_required (VERSION 2.8)

option (BUILD_SHARED_LIBS "shared/static libs" ON) 
option (BUILD_TESTS "tests?" OFF)

set (headers mman.h)
set (sources mman.c)

add_library (mman ${sources})

if (BUILD_SHARED_LIBS)
	target_compile_definitions(mman
		PUBLIC MMAN_LIBRARY_DLL
		PRIVATE MMAN_LIBRARY
	)
endif()

install (TARGETS mman RUNTIME DESTINATION bin
                      LIBRARY DESTINATION lib${LIB_SUFFIX}
                      ARCHIVE DESTINATION lib${LIB_SUFFIX})

install (FILES ${headers} DESTINATION include/sys)

if (BUILD_TESTS)
  enable_testing ()
  add_executable (t_mman test.c)
  target_link_libraries (t_mman mman)
  add_test (NAME t_mman COMMAND t_mman${CMAKE_EXECUTABLE_SUFFIX})
endif ()


