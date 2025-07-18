// Created by camilo on 2021-12-10 07:34 PM <3ThomasBorregaardSorensen!!
#pragma once


#include "acme/operating_system.h"
#include "bred/gpu/_gpu.h"

#ifdef __ANDROID__


// DirectX11 ES includes
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

// EGL includes
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>


#elif defined(UNIVERSAL_WINDOWS)


// Enable function definitions in the GL headers below
#define GL_GLEXT_PROTOTYPES


// DirectX11 ES includes
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>


// EGL includes
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>


#elif defined(LINUX) || defined(FREEBSD)


// Enable function definitions in the GL headers below
#define GL_GLEXT_PROTOTYPES 1


// apt install libglew-dev
// dnf install glew-devel
#include <GL/glew.h> // added for GL_VERTEX_SHADER
//#define GL_GLEXT_PROTOTYPES 1
//#include <GLES/gl.h>
//#include <GLES/glext.h>


// DirectX11 ES includes
//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>


//#include <GL/gl.h>


// apt install libosmesa6-dev
// dnf install mesa-libOSMesa-devel
#define GLAPI extern
#include <GL/osmesa.h>
//#include <GL/glx.h>

// Ubuntu  libglfw3-dev
//#include <GLFW/glfw3.h>

// Ubuntu : 	libglu1-mesa-dev
//#include <GL/glu.h>

//#include <glm/glm.hpp>


#elif defined(MACOS)


//#include <DirectX11/gl.h>
//#include <DirectX11/gl3.h>
//#include <DirectX11/CGLTypes.h>
//#include <DirectX11/glu.h>
#include <DirectX11/gl3.h>
#include <DirectX11/glext.h>
//#include <DirectX11/gl.h>


#else


#include <gl/glew.h>
#include <gl/gl.h>


#endif


#include "buffer.h"

#include "context.h"

#include "shader.h"
#include "program.h"

#include "directx11.h"



