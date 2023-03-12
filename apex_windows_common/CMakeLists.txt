cmake_minimum_required(VERSION 3.17)
project(apex_windows_common CXX)


list(APPEND library_source
   _.cpp
   _.h
   _factory.cpp
   framework.cpp
   framework.h)



add_library(apex_windows_common SHARED ${library_source})

target_precompile_headers(apex_windows_common PRIVATE framework.h)

set_target_properties(apex_windows_common PROPERTIES LINKER_LANGUAGE CXX)

if (MSVC)
    target_compile_options(apex_windows_common PRIVATE "/MD$<$<CONFIG:Debug>:d>")
    target_link_options(apex_windows_common PRIVATE "/DEFAULTLIB:msvcrt$<$<CONFIG:Debug>:d>")
    target_link_options(apex_windows_common PRIVATE "/IMPLIB:${LIBRARY_OUTPUT_PATH}/apex_windows_common.lib")
    target_link_options(apex_windows_common PRIVATE "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/apex_windows_common.def")
    target_link_libraries(apex_windows_common PRIVATE acme apex aqua aura Gdiplus)
elseif(LINUX)

    if(NOT DONT_USE_PKG_CONFIG)
        #pkg_check_modules(PKGCONFIG glib-2.0 gtk+-3.0 gdk-3.0 appindicator-0.1)
        pkg_check_modules(PKGCONFIG glib-2.0 freetype2 gtk+-3.0 gdk-3.0)
    endif()
    target_compile_options(apex_windows_common PRIVATE ${PKGCONFIG_CFLAGS})
    #target_compile_options(apex_windows_common PRIVATE -std=c++14)
    #target_link_options(apex_windows_common PRIVATE ${GTK_LDFLAGS})
    target_link_options(apex_windows_common PRIVATE -Wl,-rpath=\$ORIGIN)
    target_link_libraries(apex_windows_common PRIVATE z dl pthread ncurses X11 Xrender Xinerama Xft ${PKGCONFIG_LIBRARIES})
    target_link_options(apex_windows_common PRIVATE -Wl,-z,defs)
    message("PKGCONFIG_LIBRARIES is " ${PKGCONFIG_LIBRARIES})
endif()

target_link_directories(apex_windows_common PRIVATE ${LIBRARY_OUTPUT_PATH})
target_link_directories(apex_windows_common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../operating_system/storage-${OPERATING_SYSTEM_NAME}/library/x64/basis)
target_link_directories(apex_windows_common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../operating_system/storage-${OPERATING_SYSTEM_NAME}/third/library/x64/basis)


target_include_directories(apex_windows_common PRIVATE $ENV{HOME}/__config)
target_include_directories(apex_windows_common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../..)
target_include_directories(apex_windows_common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(apex_windows_common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../source)
target_include_directories(apex_windows_common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../source/app)
target_include_directories(apex_windows_common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../source/app/_include)
target_include_directories(apex_windows_common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../source/include)
target_include_directories(apex_windows_common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../operating_system/operating_system-${OPERATING_SYSTEM_NAME})
target_include_directories(apex_windows_common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../operating_system/operating_system-${OPERATING_SYSTEM_NAME}/_include)
target_include_directories(apex_windows_common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../operating_system/third-${OPERATING_SYSTEM_NAME}/include)
target_include_directories(apex_windows_common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../operating_system/third-${OPERATING_SYSTEM_NAME}/third/include)
target_include_directories(apex_windows_common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../operating_system/third-${OPERATING_SYSTEM_NAME}/third/include/include)
target_include_directories(apex_windows_common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


target_compile_definitions(apex_windows_common PRIVATE _APEX_WINDOWS_COMMON_LIBRARY)
target_compile_definitions(apex_windows_common PRIVATE UNICODE)
target_compile_definitions(apex_windows_common PRIVATE _UNICODE)
