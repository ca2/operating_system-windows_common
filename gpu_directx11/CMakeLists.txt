cmake_minimum_required(VERSION 3.16)
project(gpu_directx11 CXX)



message(STATUS "PROJECT_NAME is ${PROJECT_NAME}")

list(APPEND library_source
        _.cpp
        _.h
        _factory.cpp
        _gpu_directx11.cpp
        approach.cpp
        approach.h
        assimp.cpp
        assimp.h
        cpu_buffer.cpp
        cpu_buffer.h
        context.cpp
        context.h
        debug.cpp
        debug.h
        descriptors.cpp
        descriptors.h
        device.cpp
        device.h
        framework.cpp
        framework.h
        image_directx11.cpp
        input_layout.cpp
        input_layout.h
        memory_buffer.cpp
        memory_buffer.h
        model_buffer.cpp
        model_buffer.h
        object.cpp
        object.h
        offscreen_render_target_view.cpp
        offscreen_render_target_view.h
        physical_device.cpp
        physical_device.h
        pixmap.cpp
        pixmap.h
        program.cpp
        program.h
        render_target_view.cpp
        render_target_view.h
        renderer.cpp
        renderer.h
        shader.cpp
        shader.h
        swap_chain.cpp
        swap_chain.h
        texture.cpp
        texture.h
)


if(${LINUX} OR ${__BSD__})
    list(APPEND library_source
            context_egl.cpp
            context_egl.h
            #context_glx.cpp
            #context_glx.h
            )

endif ()


if (APPLE)
    list(APPEND library_source
            context_cgl.cpp
            context_cgl.h
            context_fbo.cpp
            context_fbo.h
            )

endif ()


list(APPEND library_references
        acme
        apex
        aqua
        aura
        axis
        base
        bred
        acme_windows_common
        directx11
        )


if(${LINUX} OR ${__BSD__})

    list(APPEND library_references
            aura_posix
       #windowing_system_x11
            )

else()

   #list(APPEND library_references
    #  glad
   #)


endif ()


add_library(${PROJECT_NAME} SHARED ${library_source})
add_library(static_${PROJECT_NAME} STATIC ${library_source})

if (NOT ${NO_PRECOMPILED_HEADER})

    target_precompile_headers(${PROJECT_NAME} PRIVATE framework.h)
    target_precompile_headers(static_${PROJECT_NAME} PRIVATE framework.h)

endif ()
if (NOT ${NO_PRECOMPILED_HEADER})

    target_precompile_headers(${PROJECT_NAME} PRIVATE framework.h)
    target_precompile_headers(static_${PROJECT_NAME} PRIVATE framework.h)

endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if (MSVC)


    target_compile_options(${PROJECT_NAME} PRIVATE "/MD$<$<CONFIG:Debug>:d>")
    target_link_options(${PROJECT_NAME} PRIVATE "/DEFAULTLIB:msvcrt$<$<CONFIG:Debug>:d>")
    target_link_options(${PROJECT_NAME} PRIVATE "/IMPLIB:${LIBRARY_OUTPUT_PATH}/${PROJECT_NAME}.lib")
    target_link_options(${PROJECT_NAME} PRIVATE "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.def")
    target_link_libraries(${PROJECT_NAME} PRIVATE assimp)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${library_references} ${global_library_references})


elseif (${LINUX} OR ${__BSD__} OR ${ANDROID})

   target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
   #set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)



   if(${OPENBSD})
      #list(APPEND extra_pkg_modules glm gl)
      list(APPEND extra_pkg_modules gl osmesa)
   elseif(${FREEBSD})
      list(APPEND extra_pkg_modules gl osmesa)
   endif()

    #if (USE_PKGCONFIG)
        pkg_check_modules(PKGCONFIG glew egl assimp ${extra_pkg_modules})

    #endif ()
    target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fpermissive  ${EXTRA_CXX_TARGET_COMPILER_OPTIONS}>)
    target_compile_options(static_${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fpermissive  ${EXTRA_CXX_TARGET_COMPILER_OPTIONS}>)
    target_compile_definitions(static_${PROJECT_NAME} PRIVATE CUBE)

    target_compile_options(${PROJECT_NAME} PRIVATE ${PKGCONFIG_CFLAGS})
    target_compile_options(static_${PROJECT_NAME} PRIVATE ${PKGCONFIG_CFLAGS})
    target_compile_options(${PROJECT_NAME} PRIVATE ${PKGCONFIG_CFLAGS})
    target_compile_options(static_${PROJECT_NAME} PRIVATE ${PKGCONFIG_CFLAGS})
    target_link_options(${PROJECT_NAME} PRIVATE ${PKGCONFIG_LDFLAGS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PKGCONFIG_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${library_references} ${global_library_references})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${default_windowing})
    target_link_options(${PROJECT_NAME} PRIVATE -Wl,-z,defs)
    # target_link_options(static_${PROJECT_NAME} PRIVATE --whole-archive)



endif ()


list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/..)
list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/../..)

target_include_directories(${PROJECT_NAME} PRIVATE ${library_include_directories})
target_include_directories(static_${PROJECT_NAME} PRIVATE ${library_include_directories})


target_compile_definitions(${PROJECT_NAME} PRIVATE _${PROJECT_NAME}_project)



