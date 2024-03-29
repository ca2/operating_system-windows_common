cmake_minimum_required(VERSION 3.17)
project(imaging_wic CXX)


add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")


if(LINUX)
elseif(WIN32)
elseif(UWP)
endif()


set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../time-${OPERATING_SYSTEM_NAME}/cmake)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../time-${OPERATING_SYSTEM_NAME}/cmake)


list(APPEND imaging_wic_source
        _.cpp
        _.h
        context_image.cpp
        context_image.h
        _factory.cpp
        #factory_exchange.h
        framework.cpp
        framework.h
        load.cpp
        load_multi_frame.cpp
        saving.cpp)


add_library(imaging_wic SHARED ${imaging_wic_source})


target_precompile_headers(imaging_wic PRIVATE framework.h)


set_target_properties(imaging_wic PROPERTIES LINKER_LANGUAGE CXX)


if (MSVC)
    target_compile_options(imaging_wic PRIVATE "/MD$<$<CONFIG:Debug>:d>")
    target_link_options(imaging_wic PRIVATE "/DEFAULTLIB:msvcrt$<$<CONFIG:Debug>:d>")
    target_link_options(imaging_wic PRIVATE "/IMPLIB:${LIBRARY_OUTPUT_PATH}/imaging_wic.lib")
    target_link_options(imaging_wic PRIVATE "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/imaging_wic.def")
    target_link_libraries(imaging_wic PRIVATE acme apex aqua aura windowscodecs)
elseif(LINUX)
    target_compile_options(imaging_wic PRIVATE -fpermissive)
    target_compile_options(imaging_wic PRIVATE ${PKGCONFIG_CFLAGS})
    target_link_options(imaging_wic PRIVATE -Wl,-rpath=\$ORIGIN)
    target_link_libraries(imaging_wic PRIVATE ${PKGCONFIG_LIBRARIES})
    target_link_libraries(imaging_wic PRIVATE acme apex aqua aura pcre2-8 sqlite3 bz2)
    target_link_options(imaging_wic PRIVATE -Wl,-z,defs)
endif()


target_link_directories(imaging_wic PRIVATE ${LIBRARY_OUTPUT_PATH})
target_link_directories(imaging_wic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../operating_system/storage-${OPERATING_SYSTEM_NAME}/library/x64/basis)
target_link_directories(imaging_wic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../operating_system/storage-${OPERATING_SYSTEM_NAME}/third/library/x64/basis)


target_include_directories(imaging_wic PRIVATE .)
target_include_directories(imaging_wic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(imaging_wic PRIVATE $ENV{HOME}/__config)
target_include_directories(imaging_wic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../source)
target_include_directories(imaging_wic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../archive)
target_include_directories(imaging_wic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../source)
target_include_directories(imaging_wic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../source/app)
target_include_directories(imaging_wic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../source/app/_include)
target_include_directories(imaging_wic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../source/include)
target_include_directories(imaging_wic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../operating_system/operating_system-${OPERATING_SYSTEM_NAME})
target_include_directories(imaging_wic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../operating_system/operating_system-${OPERATING_SYSTEM_NAME}/_include)
target_include_directories(imaging_wic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../operating_system/third-${OPERATING_SYSTEM_NAME}/include)
target_include_directories(imaging_wic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../operating_system/third-${OPERATING_SYSTEM_NAME}/third/include)
target_include_directories(imaging_wic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../operating_system/third-${OPERATING_SYSTEM_NAME}/third/include/include)


target_compile_definitions(${PROJECT_NAME} PRIVATE _${PROJECT_NAME}_project)


