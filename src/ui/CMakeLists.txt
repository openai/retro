find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

file(GLOB SRC *.cpp)

set(RETRO_EXTRA)
if(CapnProto_FOUND)
    list(APPEND RETRO_EXTRA retro-capnp)
endif()
get_target_property(QT_TYPE Qt5::Core TYPE)
if(QT_TYPE STREQUAL STATIC_LIBRARY)
    set(QT_STATIC ON)
    list(APPEND RETRO_EXTRA ${Qt5Gui_PLUGINS})
    add_definitions(-DQT_STATIC)
endif()

if(WIN32)
    if(GIT_EXECUTABLE AND EXISTS ${CMAKE_SOURCE_DIR}/.git AND IS_DIRECTORY ${CMAKE_SOURCE_DIR}/.git)
        execute_process(COMMAND ${GIT_EXECUTABLE} rev-list HEAD --count
                        OUTPUT_VARIABLE GIT_REV
                        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
    else()
        set(GIT_REV 0)
    endif()
    string(REPLACE "." "," WIN32_VERSION "${PROJECT_VERSION}")
    configure_file(win32.rc.in "${CMAKE_CURRENT_BINARY_DIR}/win32.rc")
    list(APPEND SRC "${CMAKE_CURRENT_BINARY_DIR}/win32.rc")
endif()

add_executable(gym-retro-integration WIN32 MACOSX_BUNDLE ${SRC})
set_target_properties(gym-retro-integration PROPERTIES
    MACOSX_BUNDLE_BUNDLE_NAME "Gym Retro Integration"
    MACOSX_BUNDLE_ICON_FILE "logo.icns"
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
target_link_libraries(gym-retro-integration retro-base ${RETRO_EXTRA} Qt5::Widgets Qt5::Gui)
target_sources(gym-retro-integration PRIVATE "${CORES}" "${COREINFO}" "${CMAKE_CURRENT_SOURCE_DIR}/logo.icns")

foreach(CORE IN LISTS CORES COREINFO)
    set_source_files_properties("${CORE}" PROPERTIES MACOSX_PACKAGE_LOCATION PlugIns/cores GENERATED ON)
endforeach()
if(WIN32)
    install(FILES ${CORES} DESTINATION "cores" COMPONENT gym-retro-integration)
    set(CMAKE_INSTALL_BINDIR ".")
endif()

include(GNUInstallDirs)
install(TARGETS gym-retro-integration
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT gym-retro-integration
    BUNDLE DESTINATION "." COMPONENT gym-retro-integration)

if(WIN32)
    set_target_properties(gym-retro-integration PROPERTIES OUTPUT_NAME "Gym Retro Integration")
    if(QT_STATIC)
        find_library(QTPCRE NAMES qtpcre qtpcre2)
        target_link_libraries(gym-retro-integration -lQt5PlatformSupport -limm32)
        set_target_properties(Qt5::Core PROPERTIES INTERFACE_LINK_LIBRARIES "${QTPCRE};version;ws2_32;z")
        set_target_properties(Qt5::Gui PROPERTIES INTERFACE_LINK_LIBRARIES "opengl32;winmm")
    endif()
    find_program(DEPLOY windeployqt)
    install(CODE "
        execute_process(COMMAND \"${DEPLOY}\" --compiler-runtime \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/Gym Retro Integration.exe\")
    " COMPONENT gym-retro-integration)
endif()
if(APPLE)
    set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/logo.icns" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set_target_properties(gym-retro-integration PROPERTIES OUTPUT_NAME "Gym Retro Integration")
    find_program(DEPLOY macdeployqt)
    install(CODE "
        execute_process(COMMAND \"${DEPLOY}\" \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/Gym Retro Integration.app\")
    " COMPONENT gym-retro-integration)
endif()
