cmake_minimum_required(VERSION 2.6)

option(EGL "Set to ON if targeting an EGL device" ${EGL})
option(PANDORA "Set to ON if targeting an OpenPandora" ${PANDORA})
option(ODROID "Set to ON if targeting an Odroid" ${ODROID})
option(MUPENPLUSAPI "Set to ON for Mupen64Plus plugin" ${MUPENPLUSAPI})
option(MESA "Set to ON to disable Raspberry Pi autodetection" ${MESA})
option(VERO4K "Set to ON if targeting a Vero4k" ${VERO4K})
option(ANDROID "Set to ON if targeting an Android device" ${ANDROID})
option(GL_PROFILE "Set to ON to turn on GL profiling" ${GL_PROFILE})

# run script to generate Revision.h
set(PATH_REVISION "Revision.h")
add_custom_command(
        OUTPUT ${PATH_REVISION}
        COMMAND ${CMAKE_SOURCE_DIR}/getRevision.sh
        COMMENT "Generate Git version"
)

project( GLideN64 )

set(GLideN64_SOURCES
  3DMath.cpp
  Combiner.cpp
  CombinerKey.cpp
  CommonPluginAPI.cpp
  Config.cpp
  convert.cpp
  CRC32.cpp
  DebugDump.cpp
  Debugger.cpp
  DepthBuffer.cpp
  DisplayWindow.cpp
  DisplayLoadProgress.cpp
  FrameBuffer.cpp
  FrameBufferInfo.cpp
  GBI.cpp
  gDP.cpp
  GLideN64.cpp
  GraphicsDrawer.cpp
  gSP.cpp
  Keys.cpp
  Log.cpp
  N64.cpp
  NoiseTexture.cpp
  PaletteTexture.cpp
  Performance.cpp
  PostProcessor.cpp
  RDP.cpp
  RSP.cpp
  RSP_LoadMatrix.cpp
  SoftwareRender.cpp
  TexrectDrawer.cpp
  TextDrawer.cpp
  TextureFilterHandler.cpp
  Textures.cpp
  VI.cpp
  ZlutTexture.cpp
  BufferCopy/ColorBufferToRDRAM.cpp
  BufferCopy/DepthBufferToRDRAM.cpp
  BufferCopy/RDRAMtoColorBuffer.cpp
  DepthBufferRender/ClipPolygon.cpp
  DepthBufferRender/DepthBufferRender.cpp
  common/CommonAPIImpl_common.cpp
  Graphics/Context.cpp
  Graphics/ColorBufferReader.cpp
  Graphics/CombinerProgram.cpp
  Graphics/ObjectHandle.cpp
  Graphics/OpenGLContext/GLFunctions.cpp
  Graphics/OpenGLContext/ThreadedOpenGl/opengl_Command.cpp
  Graphics/OpenGLContext/ThreadedOpenGl/opengl_ObjectPool.cpp
  Graphics/OpenGLContext/ThreadedOpenGl/opengl_Wrapper.cpp
  Graphics/OpenGLContext/ThreadedOpenGl/opengl_WrappedFunctions.cpp
  Graphics/OpenGLContext/ThreadedOpenGl/RingBufferPool.cpp
  Graphics/OpenGLContext/opengl_Attributes.cpp
  Graphics/OpenGLContext/opengl_BufferedDrawer.cpp
  Graphics/OpenGLContext/opengl_BufferManipulationObjectFactory.cpp
  Graphics/OpenGLContext/opengl_CachedFunctions.cpp
  Graphics/OpenGLContext/opengl_ColorBufferReaderWithBufferStorage.cpp
  Graphics/OpenGLContext/opengl_ColorBufferReaderWithPixelBuffer.cpp
  Graphics/OpenGLContext/opengl_ColorBufferReaderWithReadPixels.cpp
  Graphics/OpenGLContext/opengl_ContextImpl.cpp
  Graphics/OpenGLContext/opengl_GLInfo.cpp
  Graphics/OpenGLContext/opengl_Parameters.cpp
  Graphics/OpenGLContext/opengl_TextureManipulationObjectFactory.cpp
  Graphics/OpenGLContext/opengl_UnbufferedDrawer.cpp
  Graphics/OpenGLContext/opengl_Utils.cpp
  Graphics/OpenGLContext/GLSL/glsl_CombinerInputs.cpp
  Graphics/OpenGLContext/GLSL/glsl_CombinerProgramBuilder.cpp
  Graphics/OpenGLContext/GLSL/glsl_CombinerProgramImpl.cpp
  Graphics/OpenGLContext/GLSL/glsl_CombinerProgramUniformFactory.cpp
  Graphics/OpenGLContext/GLSL/glsl_FXAA.cpp
  Graphics/OpenGLContext/GLSL/glsl_ShaderStorage.cpp
  Graphics/OpenGLContext/GLSL/glsl_SpecialShadersFactory.cpp
  Graphics/OpenGLContext/GLSL/glsl_Utils.cpp
  uCodes/F3D.cpp
  uCodes/F3DBETA.cpp
  uCodes/F3DDKR.cpp
  uCodes/F3DEX.cpp
  uCodes/F3DAM.cpp
  uCodes/F3DEX2.cpp
  uCodes/F3DEX2ACCLAIM.cpp
  uCodes/F3DEX2CBFD.cpp
  uCodes/F3DZEX2.cpp
  uCodes/F3DFLX2.cpp
  uCodes/F3DGOLDEN.cpp
  uCodes/F3DTEXA.cpp
  uCodes/F3DPD.cpp
  uCodes/F3DSETA.cpp
  uCodes/F5Indi_Naboo.cpp
  uCodes/F5Rogue.cpp
  uCodes/L3D.cpp
  uCodes/L3DEX2.cpp
  uCodes/L3DEX.cpp
  uCodes/S2DEX2.cpp
  uCodes/S2DEX.cpp
  uCodes/T3DUX.cpp
  uCodes/Turbo3D.cpp
  uCodes/ZSort.cpp
  uCodes/ZSortBOSS.cpp
)

#check if we're running on Raspberry Pi
if(EXISTS "${CMAKE_FIND_ROOT_PATH}/opt/vc/include/bcm_host.h" AND NOT MESA)
  message("bcm_host.h found")
  set(BCMHOST ON)
  set(EGL ON)
  set(EGL_LIB -lbrcmEGL -lbrcmGLESv2)
  add_definitions(
    -DVC
  )
  include_directories(
    "${CMAKE_FIND_ROOT_PATH}/opt/vc/include"
    "${CMAKE_FIND_ROOT_PATH}/opt/vc/include/interface/vcos"
    "${CMAKE_FIND_ROOT_PATH}/opt/vc/include/interface/vmcs_host/linux"
    "${CMAKE_FIND_ROOT_PATH}/opt/vc/include/interface/vcos/pthreads"
  )
  link_directories(
    "${CMAKE_FIND_ROOT_PATH}/opt/vc/lib"
    "${CMAKE_FIND_ROOT_PATH}/opt/vc/lib/GL"
  )
else(EXISTS "${CMAKE_FIND_ROOT_PATH}/opt/vc/include/bcm_host.h" AND NOT MESA)
  set(EGL_LIB -lEGL)
endif(EXISTS "${CMAKE_FIND_ROOT_PATH}/opt/vc/include/bcm_host.h" AND NOT MESA)

if(MUPENPLUSAPI)
  add_definitions(
	-DMUPENPLUSAPI
		-DTXFILTER_LIB
  )
  include_directories( . inc )
  set(GLideN64_SOURCES_UNIX
	MupenPlusPluginAPI.cpp
	mupenplus/Config_mupenplus.cpp
	mupenplus/CommonAPIImpl_mupenplus.cpp
	mupenplus/MemoryStatus_mupenplus.cpp
	mupenplus/MupenPlusAPIImpl.cpp
	Graphics/OpenGLContext/mupen64plus/mupen64plus_DisplayWindow.cpp
  )

  set(GLideN64_SOURCES_WIN ${GLideN64_SOURCES_UNIX}
  )
  if (ANDROID)
      set(GLideN64_DLL_NAME libmupen64plus-video-GLideN64)
  else()
      set(GLideN64_DLL_NAME mupen64plus-video-GLideN64)
  endif()
else(MUPENPLUSAPI)
  if(UNIX)
    message(FATAL_ERROR "UNIX build requires MUPENPLUSAPI!")
  endif(UNIX)
  set(GLideN64_SOURCES_WIN
	ZilmarPluginAPI.cpp
	windows/Config_windows.cpp
	windows/CommonAPIImpl_windows.cpp
	windows/GLideN64_windows.cpp
	windows/MemoryStatus_windows.cpp
	common/GLFunctions.cpp
	windows/ZilmarAPIImpl_windows.cpp
	Graphics/OpenGLContext/windows/windows_DisplayWindow.cpp
  )
  set(GLideN64_DLL_NAME GLideN64)
endif(MUPENPLUSAPI)

add_subdirectory( osal )
include_directories( osal )

if (NOHQ)
  list(APPEND GLideN64_SOURCES
    TxFilterStub.cpp
  )
else (NOHQ)
  add_subdirectory( GLideNHQ )
  include_directories( GLideNHQ )
endif(NOHQ)

if(UNIX)
  list(APPEND GLideN64_SOURCES ${GLideN64_SOURCES_UNIX})
  if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	add_definitions(
	  -DOS_MAC_OS_X
	)
	if (NOT CMAKE_VERSION VERSION_LESS 2.8.12)
	  set(CMAKE_MACOSX_RPATH 0)
	endif()
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	add_definitions(
	  -DOS_LINUX
	)
  endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
endif(UNIX)

if(MINGW)
  add_definitions(
    -DMINGW
  )
endif(MINGW)

if(WIN32)
  list(APPEND GLideN64_SOURCES ${GLideN64_SOURCES_WIN})
  add_definitions(
    -DOS_WINDOWS
    -D__WIN32__
    -DWIN32
    -D_WIN32_ASM
    -D_CRT_SECURE_NO_WARNINGS
    -D__MSC__
  )
endif(WIN32)

if(SDL)
  include(FindPkgConfig)
  pkg_check_modules(SDL REQUIRED sdl)
  include_directories(${SDL_INCLUDE_DIRS})
  add_definitions(
	-DUSE_SDL
  )
endif(SDL)

if(PANDORA)
  #Pandora as a SGX530, but it should share the bugs and limitations as SGX540
  add_definitions(
  -DPANDORA
  -DPowerVR_SGX_540
  -DEGL
   )
endif(PANDORA)

if(ODROID)
  add_definitions(
  -DODROID
   )
endif(ODROID)

if(VERO4K)
  add_definitions(
  -DVERO4K
   )
endif(VERO4K)

if(ANDROID)
   add_definitions(-DOS_ANDROID -DEGL_EGLEXT_PROTOTYPES -DEGL)
   set(EGL ON)

   if(ANDROID_ABI STREQUAL "armeabi-v7a" OR ANDROID_ABI STREQUAL "arm64-v8a")
       set(NEON_OPT ON)
       set(VEC4_OPT ON)
       set(CRC_NEON ON)
   elseif(ANDROID_ABI STREQUAL "x86" OR ANDROID_ABI STREQUAL "x86_64")
       set(CRC_OPT ON)
       set(VEC4_OPT ON)
   endif()

   list(APPEND GLideN64_SOURCES
           Log_android.cpp
           Graphics/OpenGLContext/opengl_ColorBufferReaderWithEGLImage.cpp
           Graphics/OpenGLContext/GraphicBuffer/PrivateApi/GraphicBuffer.cpp
           Graphics/OpenGLContext/GraphicBuffer/PrivateApi/libhardware.cpp
           Graphics/OpenGLContext/GraphicBuffer/PublicApi/android_hardware_buffer_compat.cpp
           Graphics/OpenGLContext/GraphicBuffer/GraphicBufferWrapper.cpp
           )

   list(REMOVE_ITEM GLideN64_SOURCES
           Log.cpp
           )

   find_library( # Defines the name of the path variable that stores the
           # location of the NDK library.
           LOG_LIB

           # Specifies the name of the NDK library that
           # CMake needs to locate.
           log )
endif(ANDROID)

if(ANDROID)
  set( FREETYPE_INCLUDE_DIRS "${ANDROID_LIB_PATH}/freetype/include" )
elseif(UNIX OR BCMHOST)
  set( FREETYPE_INCLUDE_DIRS "/usr/include/freetype2/" )
else(UNIX OR BCMHOST)
  set( ENV{FREETYPE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../freetype )
endif(ANDROID)

if(ANDROID)
    add_library( ANDROID_FREETYPE
            SHARED
            IMPORTED )

    if( CMAKE_BUILD_TYPE STREQUAL "Debug")
        set( BUILD_VARIANT "debug" )
    else()
        set( BUILD_VARIANT "release" )
    endif()

    set_target_properties( # Specifies the target library.
            ANDROID_FREETYPE

            # Specifies the parameter you want to define.
            PROPERTIES IMPORTED_LOCATION

            # Provides the path to the library you want to import.
            ${ANDROID_LIB_PATH}/libs/${BUILD_VARIANT}/${ANDROID_ABI}/libfreetype.so)
    set(FREETYPE_LIBRARIES ANDROID_FREETYPE)
else(ANDROID)
    FIND_PACKAGE( Freetype REQUIRED )
endif(ANDROID)

include_directories( ${FREETYPE_INCLUDE_DIRS} )

if(VEC4_OPT)
  add_definitions(
    -D__VEC4_OPT
  )
endif(VEC4_OPT)

if(CRC_ARMV8)
  list(REMOVE_ITEM GLideN64_SOURCES
	CRC32.cpp
  )
  list(APPEND GLideN64_SOURCES
	CRC32_ARMV8.cpp
  )
elseif(CRC_OPT)
  list(REMOVE_ITEM GLideN64_SOURCES
    CRC32.cpp
  )
  list(APPEND GLideN64_SOURCES
    CRC_OPT.cpp
    xxHash/xxhash.c
  )
elseif(CRC_NEON)
  list(REMOVE_ITEM GLideN64_SOURCES
    CRC32.cpp
  )
  list(APPEND GLideN64_SOURCES
    Neon/CRC_OPT_NEON.cpp
    xxHash/xxhash.c
  )
endif(CRC_ARMV8)

if(NEON_OPT)
  add_definitions(
    -D__NEON_OPT
  )
  list(APPEND GLideN64_SOURCES
    Neon/3DMathNeon.cpp
    Neon/gSPNeon.cpp
    Neon/RSP_LoadMatrixNeon.cpp
  )
  list(REMOVE_ITEM GLideN64_SOURCES
    3DMath.cpp
    RSP_LoadMatrix.cpp
  )
endif(NEON_OPT)

if(X86_OPT)
  list(APPEND GLideN64_SOURCES
    RSP_LoadMatrixX86.cpp
  )
  list(REMOVE_ITEM GLideN64_SOURCES
    RSP_LoadMatrix.cpp
  )
endif(X86_OPT)

# Build type

if( NOT CMAKE_BUILD_TYPE)
	set( CMAKE_BUILD_TYPE Release)
endif( NOT CMAKE_BUILD_TYPE)

if( CMAKE_BUILD_TYPE STREQUAL "Debug")
	set( CMAKE_BUILD_TYPE Debug)
	set( DEBUG_BUILD TRUE)
	add_definitions(
	-DGL_DEBUG
	)
endif( CMAKE_BUILD_TYPE STREQUAL "Debug")

if(GL_PROFILE)
    add_definitions(
            -DGL_PROFILE -DGL_DEBUG
    )
endif(GL_PROFILE)

if(EGL)
  add_definitions(
    -DEGL
  )
  set(OPENGL_LIBRARIES ${EGL_LIB})
else(EGL)
  find_package(OpenGL REQUIRED)
  include_directories(${OpenGL_INCLUDE_DIRS})
  link_directories(${OpenGL_LIBRARY_DIRS})
  add_definitions(${OpenGL_DEFINITIONS})
  if(NOT OPENGL_FOUND)
  	message(ERROR " OPENGL not found!")
  endif(NOT OPENGL_FOUND)
endif(EGL)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  #check for compiler version
  execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE G++_VERSION)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND G++_VERSION VERSION_LESS 4.8)
    message("Found compiler version " ${G++_VERSION})
    message(SEND_ERROR "You need at least G++ 4.8 to compile GLideN64!")
  elseif(G++_VERSION VERSION_LESS 3.3)
    message("Found compiler version " ${G++_VERSION})
    message(SEND_ERROR "You need at least Clang 3.3 to compile GLideN64!")
  endif()
  set(GCC_CPP11_COMPILE_FLAGS "-std=c++11")
  set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_CPP11_COMPILE_FLAGS}" )
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(GCC_STATIC_LINK_FLAGS "-static-libgcc")
  endif()
  set( CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS} ${GCC_STATIC_LINK_FLAGS}" )
endif()

add_library( ${GLideN64_DLL_NAME} SHARED ${GLideN64_SOURCES} ${PATH_REVISION})

if( CMAKE_BUILD_TYPE STREQUAL "Debug")
  SET_TARGET_PROPERTIES(
	${GLideN64_DLL_NAME}
	PROPERTIES
	LINKER_LANGUAGE CXX # Or else we get an error message, because cmake can't figure out from the ".o"-suffix that it is a C-linker we need.
	PREFIX ""
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/plugin/Debug
  )

  if(SDL)
        if (NOHQ)
	  target_link_libraries(${GLideN64_DLL_NAME} PRIVATE ${OPENGL_LIBRARIES} ${SDL_LIBRARIES} ${FREETYPE_LIBRARIES} ${LOG_LIB} osald )
	else (NOHQ)
	  target_link_libraries(${GLideN64_DLL_NAME} PRIVATE ${OPENGL_LIBRARIES} ${SDL_LIBRARIES} ${FREETYPE_LIBRARIES} ${LOG_LIB} osald GLideNHQd )
	endif (NOHQ)
  else(SDL)
	if (NOHQ)
	  target_link_libraries(${GLideN64_DLL_NAME} PRIVATE ${OPENGL_LIBRARIES} ${FREETYPE_LIBRARIES} ${LOG_LIB} osald )
	else (NOHQ)
	  target_link_libraries(${GLideN64_DLL_NAME} PRIVATE ${OPENGL_LIBRARIES} ${FREETYPE_LIBRARIES} ${LOG_LIB} osald GLideNHQd )
	endif (NOHQ)
  endif(SDL)
endif( CMAKE_BUILD_TYPE STREQUAL "Debug")

if( CMAKE_BUILD_TYPE STREQUAL "Release")
  if ((${CMAKE_VERSION} VERSION_EQUAL 3.9) OR (${CMAKE_VERSION} VERSION_GREATER 3.9))
    cmake_policy(SET CMP0069 NEW)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result)
    if(result)
      message("Interprocedural optimizations enabled")
      set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
      set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -O3")
    endif()
  endif()

  SET_TARGET_PROPERTIES(
	${GLideN64_DLL_NAME}
	PROPERTIES
	LINKER_LANGUAGE CXX # Or else we get an error message, because cmake can't figure out from the ".o"-suffix that it is a C-linker we need.
	PREFIX ""
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/plugin/Release
  )

  if(SDL)
	if (NOHQ)
	  target_link_libraries(${GLideN64_DLL_NAME} ${OPENGL_LIBRARIES} ${SDL_LIBRARIES} ${FREETYPE_LIBRARIES} ${LOG_LIB} osal )
	else (NOHQ)
	  target_link_libraries(${GLideN64_DLL_NAME} ${OPENGL_LIBRARIES} ${SDL_LIBRARIES} ${FREETYPE_LIBRARIES} ${LOG_LIB} osal GLideNHQ )
	endif (NOHQ)
  else(SDL)
	if (NOHQ)
	  target_link_libraries(${GLideN64_DLL_NAME} ${OPENGL_LIBRARIES} ${FREETYPE_LIBRARIES} ${LOG_LIB} osal )
	else (NOHQ)
	  target_link_libraries(${GLideN64_DLL_NAME} ${OPENGL_LIBRARIES} ${FREETYPE_LIBRARIES} ${LOG_LIB} osal GLideNHQ )
	endif (NOHQ)
  endif(SDL)
endif( CMAKE_BUILD_TYPE STREQUAL "Release")
