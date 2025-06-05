// From gpu_directx11::device by
// camilo on 2025-05-27 04:54 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "approach.h"
#include "buffer.h"
#include "device.h"
#include "device.h"
#include "physical_device.h"
#include "program.h"
#include "renderer.h"
#include "shader.h"
#include "acme/filesystem/filesystem/file_context.h"
#include "acme/platform/application.h"
#include "aura/graphics/image/image.h"
#include "aura/graphics/gpu/types.h"
#include "gpu_directx11/descriptors.h"
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include "glm/mat4x4.hpp"
#include "initializers.h"
#include "windowing_win32/window.h"
#pragma comment(lib, "dcomp")
#include <glm/glm.hpp>
#include <DirectXMath.h>

DirectX::XMMATRIX GLMToDX_Transposed(const glm::mat4& m) {
   return DirectX::XMMatrixTranspose(
      DirectX::XMMATRIX(reinterpret_cast<const float*>(&m))
   );
}
DirectX::XMMATRIX GLMToDX(const glm::mat4& m) {
   return DirectX::XMMATRIX(reinterpret_cast<const float*>(&m));
}
using namespace directx11;


namespace gpu_directx11
{


   device::device()
   {
      //m_vksampler001 = nullptr;
      //m_bOffscreen = true;
      //      m_emode = e_mode_none;
            //m_itaskGpu = 0;
            //m_iLastBitmap1Scan = -1;

            //m_gluTextureBitmap1 = 0;
            //m_VAO = 0;
            //m_VBO = 0;

      m_bMesa = false;

      //m_emode = e_mode_system;

      m_estatus = error_not_initialized;

      //m_physicaldevicefeaturesCreate = {};
      //m_physicaldevicefeaturesEnabled = {};
      //m_vkdevice = VK_NULL_HANDLE;
      //m_vkcommandpool = VK_NULL_HANDLE;

      //m_vkqueuePresent = nullptr;
      //m_vkqueueGraphics = nullptr;



   }


   device::~device()
   {
#if defined(_DEBUG)

      ::directx11::dxgi_debug_terminate();

#endif

   }




   //void device::initialize(::particle * pparticle)
   //{

   //   ::e_status estatus = ::object::initialize(pparticle);

   //   if (!estatus)
   //   {

   //      return estatus;

   //   }

   //   return estatus;

   //}


//   void device::draw()
//   {
//
//      ASSERT(m_itaskGpu == ::current_itask());
//
//      //      if (m_VAO != 0)
//      //      {
//      //
//      //         //return ::success;
//      //         return;
//      //
//      //      }
//      //      //glGenBuffers(1, &VAO);
//      ////      float vertices[] = {f
//      //         // positions         // colors
//      //  //        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
//      //    //     -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
//      //      //    0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
//      //      //};
//      //      float vertices[] = {
//      //         // first triangle
//      //          1.f,  1.f, 0.0f,  // top right
//      //          1.f, -1.f, 0.0f,  // bottom right
//      //         -1.f,  1.f, 0.0f,  // top left
//      //         // second triangle
//      //          1.f, -1.f, 0.0f,  // bottom right
//      //         -1.f, -1.f, 0.0f,  // bottom left
//      //         -1.f,  1.f, 0.0f   // top left
//      //      };
//      //
//      //#if defined(__APPLE__)
//      //
//      //      glGenVertexArrays(1, &m_VAO);
//      //      glGenBuffers(1, &m_VBO);
//      //      // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//      //      glBindVertexArray(m_VAO);
//      //
//      //      //#elif defined(__ANDROID__)
//      //      //
//      //      //      glGenVertexArrays(1, &m_VAO);
//      //      //      glGenBuffers(1, &m_VBO);
//      //      //      // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//      //      //      glBindVertexArray(m_VAO);
//      //
//      //#else
//      //
//      ////      glGenVertexArrays(1, &m_VAO);
//      ////      glGenBuffers(1, &m_VBO);
//      ////      // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//      ////      glBindVertexArray(m_VAO);
//      ////
//      //#endif
//      ////
//      ////      glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
//      ////      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//      ////
//      ////      // position attribute
//      ////      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
//      ////      glEnableVertexAttribArray(0);
//      ////      // color attribute
//      ////      //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//      ////      //glEnableVertexAttribArray(1);
//      ////
//      ////      //return ::success;
//   }
//
//
//   void device::start_drawing()
//   {
//
//
//      //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_elements);
////int iError16 = glGetError();
//
////int size = 0; 
////glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
////int iError17 = glGetError();
//
////glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
////int iError18 = glGetError();
//
//
//
//
//      //ASSERT(m_itaskGpu == ::current_itask());
//
//      ////      glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
//
//      //      // Clear the screen
//      //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//      ////glClear(GL_COLOR_BUFFER_BIT);
//
//      //if (m_pprogram && m_pprogram->m_pshader)
//      //{
//
//      //   m_pprogram->m_pshader->use();
//
//      //}
//      //// Use our shader
//      ////glUseProgram(programID);
//
//      //// be sure to activate the shader
//      ////glUseProgram(shaderProgram);
//
//      //// update the uniform color
//      ////float timeValue = glfwGetTime();
//      ////float greenValue = sin(timeValue) / 2.0f + 0.5f;
//      ////int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
//      ////glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
//
//      ////return ::success;
//
//   }
//
//
//
//   void device::global_transform()
//   {
//
//
//      //// Get a handle for our "MVP" uniform
//      ////GLint MatrixID = glGetUniformLocation(programID, "MVP");
//
//      //if (m_iMatrixUniform >= 0)
//      //{
//
//      //   // Compute the MVP matrix from keyboard and mouse input
//      //   //computeMatricesFromInputs();
//      //   ::glm::mat4 matrixProjection = (::glm::mat4 &)projection_matrix();
//      //   ::glm::mat4 matrixView = (::glm::mat4 &)view_matrix();
//      //   ::glm::mat4 matrixModel = glm::mat4(1.0);
//      //   ::glm::mat4 matrixMVP = matrixProjection * matrixView * matrixModel;
//
//      //   // Send our transformation to the currently bound shader, 
//      //   // in the "MVP" uniform
//      //   //glUniformMatrix4fv(m_iMatrixUniform, 1, GL_FALSE, &matrixMVP[0][0]);
//
//      //}
//
//      ////glm::mat4 getViewMatrix() {
//      ////   return ViewMatrix;
//      ////}
//      ////glm::mat4 getProjectionMatrix() {
//      ////   return ProjectionMatrix;
//      ////}
//
//
//      ////// Initial position : on +Z
//      ////glm::vec3 position = glm::vec3(0, 0, 5);
//      ////// Initial horizontal angle : toward -Z
//      ////float horizontalAngle = 3.14f;
//      ////// Initial vertical angle : none
//      ////float verticalAngle = 0.0f;
//      ////// Initial Field of View
//      ////float initialFoV = 45.0f;
//
//      ////float speed = 3.0f; // 3 units / second
//      ////float mouseSpeed = 0.005f;
//
//
//
//   }


   void device::defer_shader_memory(::memory& memory, const ::file::path& pathShader)
   {

      if (memory.is_empty())
      {

         auto path = m_pgpuapproach->shader_path(pathShader);

         memory = file()->as_memory(path);

      }

   }


   //void device::render()
   //{

   //   ASSERT(m_itaskGpu == ::current_itask());

   //   //      //#ifdef __APPLE__
   //   //
   //   //            //glBindVertexArrayAPPLE(m_VAO);
   //   //
   //   //      //#else
   //   //
   //   //      if (m_gluTextureBitmap1)
   //   //      {
   //   //
   //   //         //glMatrixMode(GL_PROJECTION);
   //   //         //glOrtho(0, 800, 0, 600, -1, 1);
   //   //         // glMatrixMode(GL_MODELVIEW);
   //   //
   //   //         //clear and draw quad with texture (could be in display callback)
   //   //         //glClear(GL_COLOR_BUFFER_BIT);
   //   //         //glBindTexture(GL_TEXTURE_2D, m_gluTextureBitmap1);
   //   //         ////glEnable(GL_TEXTURE_2D);
   //   //         //glBegin(GL_QUADS);
   //   //         //glTexCoord2i(0, 0); glVertex2f(-1.0, -1.0);
   //   //         //glTexCoord2i(0, 1); glVertex2f(-1.0, 1.0);
   //   //         //glTexCoord2i(1, 1); glVertex2f(1.0, 1.0);
   //   //         //glTexCoord2i(1, 0); glVertex2f(1.0, -1.0);
   //   //         //glEnd();
   //   //         //glDisable(GL_TEXTURE_2D);
   //   //         //glBindTexture(GL_TEXTURE_2D, 0);
   //   //         //glFlush();
   //   //
   //   //         /*GLuint readFboId = 0;
   //   //         glGenFramebuffers(1, &readFboId);
   //   //         glBindFramebuffer(GL_READ_FRAMEBUFFER, readFboId);
   //   //         glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
   //   //            GL_TEXTURE_2D, m_gluTextureBitmap1, 0);
   //   //         glBlitFramebuffer(0, 0, m_sizeBitmap1.cx(), m_sizeBitmap1.cy(),
   //   //            0, 0, m_size.cx(), m_size.cy(),
   //   //            GL_COLOR_BUFFER_BIT, GL_LINEAR);
   //   //         glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
   //   //         glDeleteFramebuffers(1, &readFboId);*/
   //   //      }
   //   //      else
   //   //      {
   //   //
   //   ////#ifdef __APPLE__
   //   ////         glBindVertexArrayAPPLE(m_VAO);
   //   ////#else
   //   ////         glBindVertexArray(m_VAO);
   //   ////#endif
   //   ////
   //   ////         glDrawArrays(GL_TRIANGLES, 0, 6);
   //   //
   //   //   }
   //   //
   //   //      //return ::success;

   //}


   //void device::set_bitmap_1(::image::image* pimage)
   //{

   //   ASSERT(m_itaskGpu == ::current_itask());

   //   //      if (!m_gluTextureBitmap1)
   //   //      {
   //   //
   //   ////         glGenTextures(1, &m_gluTextureBitmap1); // generate texture id for your texture (can skip this line)
   //   ////         glEnable(GL_TEXTURE_2D);
   //   ////         glBindTexture(GL_TEXTURE_2D, m_gluTextureBitmap1);
   //   ////
   //   ////         glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // set alignment of data in memory (a good thing to do before glTexImage)
   //   ////
   //   ////#if defined(__APPLE__) || defined(__ANDROID__)
   //   ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   //   ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // set clamp (GL_CLAMP_TO_EDGE would be better)
   //   ////#else
   //   ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   //   ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); // set clamp (GL_CLAMP_TO_EDGE would be better)
   //   ////#endif
   //   ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   //   ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // set linear filtering (so you can scale your image)
   //   //
   //   //      }
   //   //
   //   //      if (pimage->is_ok())
   //   //      {
   //   //
   //   //         pimage->map();
   //   //
   //   //         //glBindTexture(GL_TEXTURE_2D, m_gluTextureBitmap1);
   //   //
   //   //         //if (m_iLastBitmap1Scan != pimage->m_iScan)
   //   //         //{
   //   //
   //   //         //   m_iLastBitmap1Scan = pimage->m_iScan;
   //   //
   //   //         //   glPixelStorei(GL_UNPACK_ROW_LENGTH, pimage->m_iScan / 4);
   //   //
   //   //         //}
   //   //
   //   //         m_sizeBitmap1 = pimage->size();
   //   //
   //   //
   //   //         m_memorySwap.set_size(m_sizeBitmap1.area() * 4);
   //   //
   //   //         //vertical_swap_copy_image32_swap_red_blue(
   //   //            ((image32_t *)m_memorySwap.data())->vertical_swap_copy_swap_red_blue(
   //   //            m_sizeBitmap1.cx(),
   //   //            m_sizeBitmap1.cy(),
   //   //            m_sizeBitmap1.cx() * 4,
   //   //            pimage->get_data(),
   //   //            pimage->m_iScan);
   //   //
   //   //         //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
   //   //         //   m_sizeBitmap1.cx(),
   //   //         //   m_sizeBitmap1.cy(),
   //   //         //   0, GL_RGBA, GL_UNSIGNED_BYTE,
   //   //         //   m_memorySwap.data()); // upload image data to the textur
   //   //
   //   //
   //   //
   //   //
   //   //
   //   //      }

   //}


   //void device::swap_buffers()
   //{


   //}


   //VkSampler device::_001VkSampler()
   //{

   //   if (!m_vksampler001)
   //   {

   //      VkSamplerCreateInfo samplerInfo = {
   //.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
   //.magFilter = VK_FILTER_LINEAR,
   //.minFilter = VK_FILTER_LINEAR,
   //   .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,

   //.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
   //.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
   //.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
   //.mipLodBias = 0.0f,
   //.anisotropyEnable = VK_FALSE,
   //.maxAnisotropy = 1.0f,
   //.compareEnable = VK_FALSE,
   //.compareOp = VK_COMPARE_OP_ALWAYS,
   //.minLod = 0.0f,
   //.maxLod = 0.0f,
   //   .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
   //.unnormalizedCoordinates = VK_FALSE,

   //      };


   //      if (vkCreateSampler(this->logicalDevice(), &samplerInfo, NULL, &m_vksampler001) != VK_SUCCESS) {
   //         // Handle error
   //      }

   //   }

   //   return m_vksampler001;

   //}





   //void device::create_offscreen_buffer(const ::int_size& size)
   //{

   //   auto pgpu = application()->get_gpu();

   //   if (::is_null(pgpu))
   //   {

   //      return ::error_failed;

   //   }

   //   ::e_status estatus = __øconstruct(m_pbuffer);

   //   if (!estatus)
   //   {

   //      return false;

   //   }

   //   m_pbuffer->m_pimage = create_image(size);

   //   if (!::is_ok(m_pbuffer->m_pimage))
   //   {

   //      return false;

   //   }

   //   estatus = _create_offscreen_buffer(size);

   //   if(!estatus)
   //   {

   //      return estatus;

   //   }

   //   return ::success_none;

   //}


   //void device::_create_offscreen_buffer(const ::int_size& size)
   //{

   //   return ::success_none;

   //}


   //void device::resize_offscreen_buffer(const ::int_size& size)
   //{

   //   return ::success_none;

   //}



   //void device::make_current()
   //{

   //   return ::success_none;

   //}


   //void device::destroy_offscreen_buffer()
   //{

   //   return ::success_none;

   //}


   //string device::_001GetIntroProjection()
   //{

   //   string strVersion = get_shader_version_text();


   //   string strProjection =
   //      "layout(location = 0) in vec3 aPos;\n"
   //      "out vec3 ourPosition;\n"
   //      "\n"
   //      "void main()\n"
   //      "{\n"
   //      "   gl_Position = vec4(aPos, 1.0);\n"
   //      "   ourPosition = aPos;\n"
   //      "}\n";

   //   return strProjection;
   //}


   //string device::_001GetIntroFragment()
   //{

   //   string strVersion = get_shader_version_text();

   //   string strFragment =
   //      "uniform vec2 resolution;\n"
   //      "uniform float time;\n"
   //      "uniform vec2 mouse;\n"
   //      "uniform sampler2D backbuffer;\n"
   //      "\n"
   //      "void main(void) {\n"
   //      "float base_res = min(resolution.x(), resolution.y());\n"
   //      "vec2 uv = (gl_FragCoord.xy * 2.0 - resolution.xy) / base_res;\n"
   //      "\n"
   //      //"gl_FragColor = vec4(uv, (uv.x() * uv.x()) / 2.0, ((uv.x() + (base_res - uv.y())) *(uv.x() + (base_res - uv.y()))) / 2.0);\n"
   //      "float posx = max(0.f, uv.x());\n"
   //      "float posy = max(0.f, uv.y());\n"
   //      "gl_FragColor = vec4(uv, (posx * posx) / 4.0, ((posx + posy) * (posx + posy)) / 4.0);\n"
   //      "}\n";

   //   return strFragment;

   //}

   //   string device::load_fragment(const ::string & pszPath, enum_shader & eshader)
   //   {
   //
   //      ::file::path path(pszPath);
   //
   //      auto & app = papp;
   //
   //      auto & file = app.file();
   //
   //      string strFragment = file.as_string(path);
   //
   //      string strExtension = path.extension();
   //
   //      string strVersion = get_shader_version_text();
   //
   //      if (strExtension.case_insensitive_begins("shadertoy"))
   //      {
   //
   //         eshader = e_shader_shadertoy;
   //
   //         strFragment =
   //            //"#" + strVersion + "\n"
   //            //"\n"
   //            //"precision highp float;\n"
   //            "\n"
   //            "uniform vec2 iResolution;\n"
   //            "uniform float iTime;\n"
   //            "uniform vec2 iMouse;\n"
   //            "uniform sampler2D backbuffer;\n"
   //            "\n"
   //            "\n"
   //            + strFragment;
   //
   //
   //         strFragment +=
   //            "\n\n"
   //            "void main(void)\n"
   //            "{\n"
   //            "   mainImage(gl_FragColor, gl_FragCoord.xy);\n"
   //            "}\n";
   //
   //      }
   //      else
   //      {
   //
   //         strFragment =
   ////            "#" + strVersion + "\n"
   ////            "\n"
   //            + strFragment;
   //
   //         eshader = e_shader_neort;
   //
   //      }
   //
   //      return strFragment;
   //
   //   }


      //string device::get_shader_version_text()
      //{

      //   return "version 330 core";

      //}

      //void device::translate_shader(string& strFragment)
      //{

      //   string_array stra;

      //   stra.add_lines(strFragment);

      //   auto pFind = stra.case_insensitive_find_first_begins("#version ");

      //   if (::is_set(pFind))
      //   {

      //      stra[iFind] = get_shader_version_text();

      //   }
      //   else
      //   {

      //      stra.insert_at(0, get_shader_version_text());

      //   }

      //   _translate_shader(stra);

      //   strFragment = stra.implode("\n");

      //}


      //void device::_translate_shader(string_array& stra)
      //{

      //}


   //void device::set_matrix_uniform(const ::gpu::payload& uniformMatrix)
   //{

   //   //m_iMatrixUniform = uniformMatrix.m_iUniform;

   //}

   ::pointer < ::gpu::device > allocate_system_context(::particle* pparticle)
   {

      return pparticle->__create_new <device>();

   }


   //device::device()
   //{



   //}


   //device::~device()
   //{

   //}


   bool device::is_mesa()
   {

      return m_bMesa;

   }


   void device::_create_offscreen_window(const ::int_size& size)
   {
      //if (::IsWindow(m_hwnd))
      //{

      //   if (!::SetWindowPos(m_hwnd,
      //      nullptr, 0, 0,
      //      size.cx()
      //      , size.cy(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE
      //      | SWP_NOCOPYBITS | SWP_NOSENDCHANGING
      //      | SWP_NOREPOSITION | SWP_NOREDRAW))
      //   {

      //      information() << "SetWindowPos Failed";

      //   }


      //   //return m_hwnd;

      //}
      //else
      //{

      //   LPCTSTR lpClassName = L"draw2d_directx11_offscreen_buffer_window";
      //   LPCTSTR lpWindowName = L"draw2d_directx11_offscreen_buffer_window";
      //   //unsigned int dwStyle = WS_CAPTION | WS_POPUPWINDOW; // | WS_VISIBLE
      //   unsigned int dwExStyle = 0;
      //   unsigned int dwStyle = WS_OVERLAPPEDWINDOW;
      //   dwStyle |= WS_POPUP;
      //   //dwStyle |= WS_VISIBLE;
      //   //dwStyle |= WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
      //   dwStyle &= ~WS_CAPTION;
      //   //dwStyle = 0;
      //   dwStyle &= ~WS_THICKFRAME;
      //   dwStyle &= ~WS_BORDER;
      //   int x = 0;
      //   int y = 0;
      //   int nWidth = size.cx();
      //   int nHeight = size.cy();
      //   HWND hWndParent = nullptr;
      //   HMENU hMenu = nullptr;
      //   HINSTANCE hInstance = ::GetModuleHandleW(L"gpu_directx11.dll");
      //   LPVOID lpParam = nullptr;

      //   m_hwnd = CreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

      //   if (!m_hwnd)
      //   {

      //      informationf("MS GDI - CreateWindow failed");

      //      informationf("last-error code: %d\n", GetLastError());

      //      throw ::exception(error_failed);

      //   }

      //   //return m_hwnd;

      //}


   }


   void device::initialize_gpu_device(::gpu::approach* pgpuapproachParam, ::windowing::window * pwindow, const ::int_rectangle & rectanglePlacement, bool bAddSwapChainSupport)
   {


      //createInstance();
      //setupDebugMessenger();
      //createSurface();
      //pickPhysicalDevice();
      //createLogicalDevice();
      //createCommandPool();

      if (bAddSwapChainSupport)
      {

         initialize_swap_chain(pwindow);

      }
      else
      {

         initialize_cpu_buffer(pwindow);

      }

      ::cast < approach > pgpuapproach = pgpuapproachParam;

      if (!pgpuapproach)
      {

         throw ::exception(error_failed);

      }

      m_pgpuapproach = pgpuapproach.m_p;

      auto pphysicaldevice = pgpuapproach->m_pphysicaldevice;

      //assert(pphysicaldevice && pphysicaldevice->m_physicaldevice);

      m_pphysicaldevice = pphysicaldevice;

      if (m_papplication->m_bUseSwapChainWindow)
      {

         m_pphysicaldevice->createWindowSurface(pwindow);

      }



      //if (startcontext.m_eoutput == ::gpu::e_output_swap_chain)
      //{

      //   m_pphysicaldevice->createWindowSurface(startcontext.m_pwindow);

      //}

      //auto physicaldevice = pphysicaldevice->m_physicaldevice;

      //// Get list of supported extensions
      //uint32_t extCount = 0;
      //vkEnumerateDeviceExtensionProperties(physicaldevice, nullptr, &extCount, nullptr);
      //if (extCount > 0)
      //{
      //   ::array<VkExtensionProperties> extensions(extCount);
      //   if (vkEnumerateDeviceExtensionProperties(physicaldevice, nullptr, &extCount, extensions.data()) == VK_SUCCESS)
      //   {
      //      for (auto& ext : extensions)
      //      {
      //         m_straSupportedExtensions.add(ext.extensionName);
      //      }
      //   }
      //}

      // Derived examples can enable extensions based on the list of supported extensions read from the physical device
      //getEnabledExtensions();

      //bool useSwapChain = m_eoutput == ::gpu::e_output_swap_chain;

      bool useSwapChain = bAddSwapChainSupport;

      //m_itaskGpu = ::current_itask();
//
//      VkPhysicalDeviceScalarBlockLayoutFeatures scalarBlockLayoutSupport = {
//.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES,
//      .scalarBlockLayout = TRUE };
//      pgpuapproach->m_pDeviceCreatepNextChain = &scalarBlockLayoutSupport;
//      m_physicaldevicefeaturesCreate.logicOp = TRUE;
//      m_physicaldevicefeaturesCreate.independentBlend = TRUE;
//      HRESULT result = createLogicalDevice(
//         m_physicaldevicefeaturesCreate,
//         pgpuapproach->m_pszaEnabledDeviceExtensions,
//         pgpuapproach->m_pDeviceCreatepNextChain,
//         useSwapChain);
//
//      if (result != VK_SUCCESS)
//      {
//
//         //m_itaskGpu = {};
//
//         exitFatal("Could not create DirectX11 device: \n" + errorString(result) + " HRESULT=" + ::as_string(result), result);
//
//         throw ::exception(error_failed);
//
//      }
//

      //device = directx11Device->logicalDevice;

   }


   //void device::on_create_context(const ::gpu::start_context_t& startcontext)
   //{

   //   UNREFERENCED_PARAMETER(startcontext);

   //   _create_context_win32(startcontext);

   //}

//
//   HRESULT device::createLogicalDevice(
//      VkPhysicalDeviceFeatures enabledFeatures,
//      ::array<const char*> enabledExtensions,
//      void* pNextChain,
//      bool useSwapChain,
//      VkQueueFlags requestedQueueTypes)
//   {
//
//      ::cast < approach > pgpuapproach = application()->get_gpu();
//
//      ::cast < physical_device > pphysicaldevice = pgpuapproach->m_pphysicaldevice;
//
//      // Desired queues need to be requested upon logical device creation
//      // Due to differing queue family configurations of DirectX11 implementations this can be a bit tricky, especially if the application
//      // requests different queue types
//
//      ::array<VkDeviceQueueCreateInfo> queueCreateInfos{};
//
//      // Get queue family indices for the requested queue family types
//      // Note that the indices may overlap depending on the implementation
//
//      const float defaultQueuePriority(0.0f);
//
//      m_queuefamilyindices = pphysicaldevice->findQueueFamilies();
//
//      // Graphics queue
//      if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT
//         && m_queuefamilyindices.graphicsFamilyHasValue)
//      {
//         //m_queuefamilyindices.graphics = pphysicaldevice->getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
//         VkDeviceQueueCreateInfo queueInfo{};
//         queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//         queueInfo.queueFamilyIndex = m_queuefamilyindices.graphicsFamily;
//         queueInfo.queueCount = 1;
//         queueInfo.pQueuePriorities = &defaultQueuePriority;
//         queueCreateInfos.add(queueInfo);
//      }
//      else
//      {
//         m_queuefamilyindices.graphicsFamily = 0;
//      }
//
//      // Dedicated compute queue
//      if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT
//         && m_queuefamilyindices.computeFamilyHasValue)
//      {
//         //m_queuefamilyindices.compute = pphysicaldevice->getQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
//         if (m_queuefamilyindices.computeFamily != m_queuefamilyindices.graphicsFamily)
//         {
//            // If compute family index differs, we need an additional queue create info for the compute queue
//            VkDeviceQueueCreateInfo queueInfo{};
//            queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//            queueInfo.queueFamilyIndex = m_queuefamilyindices.computeFamily;
//            queueInfo.queueCount = 1;
//            queueInfo.pQueuePriorities = &defaultQueuePriority;
//            queueCreateInfos.add(queueInfo);
//         }
//      }
//      else
//      {
//         // Else we use the same queue
//         m_queuefamilyindices.computeFamily = m_queuefamilyindices.graphicsFamily;
//      }
//
//      // Dedicated transfer queue
//      if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT
//         && m_queuefamilyindices.transferFamilyHasValue)
//      {
//         //m_queuefamilyindices.transfer = pphysicaldevice->getQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT);
//         if ((m_queuefamilyindices.transferFamily != m_queuefamilyindices.graphicsFamily)
//            && (m_queuefamilyindices.transferFamily != m_queuefamilyindices.computeFamily))
//         {
//            // If transfer family index differs, we need an additional queue create info for the transfer queue
//            VkDeviceQueueCreateInfo queueInfo{};
//            queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//            queueInfo.queueFamilyIndex = m_queuefamilyindices.transferFamily;
//            queueInfo.queueCount = 1;
//            queueInfo.pQueuePriorities = &defaultQueuePriority;
//            queueCreateInfos.add(queueInfo);
//         }
//      }
//      else
//      {
//         // Else we use the same queue
//         m_queuefamilyindices.transferFamily = m_queuefamilyindices.graphicsFamily;
//      }
//
//      // Create the logical device representation
//      ::array<const char*> deviceExtensions(enabledExtensions);
//      if (useSwapChain)
//      {
//
//         // If the device will be used for presenting to a display via a swapchain we need to request the swapchain extension
//         deviceExtensions.add(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
//
//      }
//
//      VkDeviceCreateInfo deviceCreateInfo = {};
//      deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//      deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());;
//      deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
//      deviceCreateInfo.pEnabledFeatures = &enabledFeatures;
//
//      // If a pNext(Chain) has been passed, we need to add it to the device creation info
//      VkPhysicalDeviceFeatures2 physicalDeviceFeatures2{};
//      if (pNextChain) {
//         physicalDeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
//         physicalDeviceFeatures2.features = enabledFeatures;
//         physicalDeviceFeatures2.pNext = pNextChain;
//         deviceCreateInfo.pEnabledFeatures = nullptr;
//         deviceCreateInfo.pNext = &physicalDeviceFeatures2;
//      }
//
//#if (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK) || defined(VK_USE_PLATFORM_METAL_EXT)) && defined(VK_KHR_portability_subset)
//      // SRS - When running on iOS/macOS with MoltenVK and VK_KHR_portability_subset is defined and supported by the device, enable the extension
//      if (extensionSupported(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME))
//      {
//         deviceExtensions.add(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
//      }
//#endif
//
//      if (deviceExtensions.size() > 0)
//      {
//         for (const char* enabledExtension : deviceExtensions)
//         {
//            if (!isExtensionSupported(enabledExtension)) {
//               information() << "Enabled device extension \"" << enabledExtension << "\" is not present at device level\n";
//            }
//         }
//
//         deviceCreateInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
//         deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
//      }
//
//      this->m_physicaldevicefeaturesEnabled = enabledFeatures;
//
//      auto physicaldevice = pphysicaldevice->m_physicaldevice;
//
//      HRESULT result = vkCreateDevice(physicaldevice, &deviceCreateInfo, nullptr, &m_vkdevice);
//      if (result != VK_SUCCESS)
//      {
//         return result;
//      }
//
//      // Create a default command pool for graphics command buffers
//      m_vkcommandpool = createCommandPool(m_queuefamilyindices.graphicsFamily);
//
//      return result;
//
//   }


   ///**
   //* Create a command pool for allocation command buffers from
   //*
   //* @param queueFamilyIndex Family index of the queue to create the command pool for
   //* @param createFlags (Optional) Command pool creation flags (Defaults to VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)
   //*
   //* @note Command buffers allocated from the created pool can only be submitted to a queue with the same family index
   //*
   //* @return A handle to the created command buffer
   //*/
   //VkCommandPool device::createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags)
   //{
   //   VkCommandPoolCreateInfo cmdPoolInfo = {};
   //   cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
   //   cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
   //   cmdPoolInfo.flags = createFlags;
   //   VkCommandPool cmdPool;
   //   VK_CHECK_RESULT(vkCreateCommandPool(m_vkdevice, &cmdPoolInfo, nullptr, &cmdPool));
   //   return cmdPool;
   //}


   /**
   * Check if an extension is supported by the (physical device)
   *
   * @param extension Name of the extension to check
   *
   * @return True if the extension is supported (present in the list read at device creation time)
   */
   bool device::isExtensionSupported(const ::scoped_string& scopedstrExtension)
   {

      return m_straSupportedExtensions.contains(scopedstrExtension);

   }


   //void device::_create_window_buffer()
   //{

   //   if (!m_hdc || !m_hrc)
   //   {

   //      auto psystem = system();

   //      auto pgpu = application()->get_gpu();

   //      ::pointer < ::directx11::directx11 > pdirectx11 = pgpu;

   //      if (!pdirectx11->m_atomClass)
   //      {

   //         informationf("MS GDI - RegisterClass failed");

   //         informationf("last-error code: %d\n", GetLastError());

   //         throw ::exception(error_failed);

   //      }

   //      // create WGL device, make current

   //      //xxxopengl>>>>directx11 PIXELFORMATDESCRIPTOR pixformat;

   //      int chosenformat;

   //      HDC hdc = GetDC(m_hwnd);

   //      if (!hdc)
   //      {

   //         informationf("MS GDI - GetDC failed");

   //         informationf("last-error code: %d\n", GetLastError());

   //         throw ::exception(error_failed);

   //      }

   //      ////xxxopengl>>>>directx11  zero(pixformat);
   //      //pixformat.nSize = sizeof(pixformat);
   //      //pixformat.nVersion = 1;
   //      //pixformat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_DIRECTX11 | PFD_DOUBLEBUFFER;
   //      //pixformat.iPixelType = PFD_TYPE_RGBA;
   //      //pixformat.cColorBits = 32;
   //      //pixformat.cRedShift = 16;
   //      //pixformat.cGreenShift = 8;
   //      //pixformat.cBlueShift = 0;
   //      //pixformat.cAlphaShift = 24;
   //      //pixformat.cAlphaBits = 8;
   //      //pixformat.cDepthBits = 24;
   //      //pixformat.cStencilBits = 8;

   //      //xxxopengl>>>>directx11 chosenformat = ChoosePixelFormat(hdc, &pixformat);

   //      //if (chosenformat == 0)
   //      //{

   //      //   informationf("MS GDI - ChoosePixelFormat failed");

   //      //   informationf("last-error code: %d\n", GetLastError());

   //      //   ReleaseDC(m_hwnd, hdc);

   //      //   throw ::exception(error_failed);

   //      //}

   //      //bool spfok = SetPixelFormat(hdc, chosenformat, &pixformat);

   //      //if (!spfok)
   //      //{

   //      //   informationf("MS GDI - SetPixelFormat failed");

   //      //   informationf("last-error code: %d\n", GetLastError());

   //      //   ReleaseDC(m_hwnd, hdc);

   //      //   throw ::exception(error_failed);

   //      //}

   //      //HGLRC hglrc = wglCreateContext(hdc);

   //      //if (!hglrc)
   //      //{

   //      //   informationf("MS WGL - wglCreateContext failed");

   //      //   informationf("last-error code: %d\n", GetLastError());

   //      //   ReleaseDC(m_hwnd, hdc);

   //      //   throw ::exception(error_failed);

   //      //}

   //      //bool bMakeCurrentOk = wglMakeCurrent(hdc, hglrc);

   //      //if (!bMakeCurrentOk)
   //      //{

   //      //   informationf("MS WGL - wglMakeCurrent failed");

   //      //   informationf("last-error code: %d\n", GetLastError());

   //      //   ReleaseDC(m_hwnd, hdc);

   //      //   throw ::exception(error_failed);

   //      //}


   //      pdirectx11->defer_init_gpu_library();

   //      //auto pszVersion = (const char *)glGetString(GL_VERSION);
   //      ////::e_status estatus = 

   //      //::string strVersion(pszVersion);

   //      //if (strVersion.case_insensitive_contains("mesa"))
   //      //{

   //      //   m_bMesa = true;

   //      //}

   //      ////if (!estatus)
   //      ////{

   //      ////   ReleaseDC(window, hdc);

   //      ////   return estatus;

   //      ////}

   //      m_hwnd = m_hwnd;
   //      m_hdc = hdc;
   //      //xxxopengl>>>>directx11 m_hrc = hglrc;

   //   }

   //   RECT rectClient;

   //   ::GetClientRect(m_hwnd, &rectClient);

   //   m_size = { rectClient.right - rectClient.left,
   //      rectClient.bottom - rectClient.top };

   //   m_itaskGpu = ::current_itask();

   //   m_estatus = ::success;

   //   set_ok_flag();
   //   
   //}


   //void device::_create_window_context(::windowing::window* pwindowParam)
   //{

   //   m_itaskGpu = ::current_itask();

   //   //   ::cast < ::windowing_win32::window > pwindow = pwindowParam;

   //   //   if (!m_hdc || !m_hrc)
   //   //   {

   //   //      auto psystem = system();

   //   //      auto pgpu = application()->get_gpu();

   //   //      ::pointer < ::directx11::directx11 > pdirectx11 = pgpu;

   //   //      //if (!pdirectx11->m_atomClass)
   //   //      //{

   //   //      //   informationf("MS GDI - RegisterClass failed");

   //   //      //   informationf("last-error code: %d\n", GetLastError());

   //   //      //   throw ::exception(error_failed);

   //   //      //}

   //   //      if (!m_hwnd)
   //   //      {

   //   //         auto hwnd = pwindow->m_hwnd;


   //   //         m_hwnd = hwnd;


   //   //         //// create WGL device, make current

   //   //         //PIXELFORMATDESCRIPTOR pixformat;

   //   //         //int chosenformat;

   //   //         HDC hdc = GetDC(m_hwnd);

   //   //         //if (!hdc)
   //   //         //{

   //   //         //   informationf("MS GDI - GetDC failed");

   //   //         //   informationf("last-error code: %d\n", GetLastError());

   //   //         //   throw ::exception(error_failed);

   //   //         //}

   //   //         //zero(pixformat);
   //   //         //pixformat.nSize = sizeof(pixformat);
   //   //         //pixformat.nVersion = 1;
   //   //         //pixformat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_DIRECTX11 | PFD_DOUBLEBUFFER;
   //   //         //pixformat.iPixelType = PFD_TYPE_RGBA;
   //   //         //pixformat.cColorBits = 32;
   //   //         //pixformat.cRedShift = 16;
   //   //         //pixformat.cGreenShift = 8;
   //   //         //pixformat.cBlueShift = 0;
   //   //         //pixformat.cAlphaShift = 24;
   //   //         //pixformat.cAlphaBits = 8;
   //   //         //pixformat.cDepthBits = 24;
   //   //         //pixformat.cStencilBits = 8;

   //   //         //chosenformat = ChoosePixelFormat(hdc, &pixformat);

   //   //         //if (chosenformat == 0)
   //   //         //{

   //   //         //   informationf("MS GDI - ChoosePixelFormat failed");

   //   //         //   informationf("last-error code: %d\n", GetLastError());

   //   //         //   ReleaseDC(m_hwnd, hdc);

   //   //         //   throw ::exception(error_failed);

   //   //         //}

   //   //         //bool spfok = SetPixelFormat(hdc, chosenformat, &pixformat);

   //   //         //if (!spfok)
   //   //         //{

   //   //         //   informationf("MS GDI - SetPixelFormat failed");

   //   //         //   informationf("last-error code: %d\n", GetLastError());

   //   //         //   ReleaseDC(m_hwnd, hdc);

   //   //         //   throw ::exception(error_failed);

   //   //         //}

   //   ////         auto hglrc = wglCreateContext(hdc);

   //   ////         pwindow->m_hglrcProto = hglrc;

   //   //////         int context_attribs[] = {
   //   //////WGL_CONTEXT_MAJOR_VERSION_ARB, 2,
   //   //////WGL_CONTEXT_MINOR_VERSION_ARB, 1,
   //   //////0, 0
   //   //////         };
   //   //////         auto hglrc = wglCreateContextAttribsARB(hdc, NULL, context_attribs);
   //   //////         if (!hglrc) {
   //   //////            //ReleaseDC(hWnd, hDC);
   //   //////            //DestroyWindow(hWnd);

   //   //////            throw ::exception(error_failed);
   //   //////         }
   //   //////         //ReleaseDC(hWnd, hDC);

   //   ////         if (!pwindow->m_hglrcProto)
   //   ////         {

   //   ////            informationf("MS WGL - wglCreateContext failed");

   //   ////            informationf("last-error code: %d\n", GetLastError());

   //   ////            ReleaseDC(m_hwnd, hdc);

   //   ////            throw ::exception(error_failed);

   //   ////         }

   //   ////         bool bMakeCurrentOk = wglMakeCurrent(hdc, pwindow->m_hglrcProto);

   //   ////         if (!bMakeCurrentOk)
   //   ////         {

   //   ////            informationf("MS WGL - wglMakeCurrent failed");

   //   ////            informationf("last-error code: %d\n", GetLastError());

   //   ////            ReleaseDC(m_hwnd, hdc);

   //   ////            throw ::exception(error_failed);

   //   ////         }


   //   ////         pdirectx11->defer_init_gpu_library();

   //   //         //auto pszVersion = (const char *)glGetString(GL_VERSION);
   //   //         ////::e_status estatus = 

   //   //         //::string strVersion(pszVersion);

   //   //         //if (strVersion.case_insensitive_contains("mesa"))
   //   //         //{

   //   //         //   m_bMesa = true;

   //   //         //}

   //   //         ////if (!estatus)
   //   //         ////{

   //   //         ////   ReleaseDC(window, hdc);

   //   //         ////   return estatus;

   //   //         ////}

   //   //         m_hwnd = m_hwnd;
   //   //         m_hdc = hdc;
   //   //         m_hrc = pwindow->m_hglrcProto;

   //   //      }

   //   //   }

   //   //   RECT rectClient;

   //   //   ::GetClientRect(m_hwnd, &rectClient);

   //   //   ::int_size sizeNew = { rectClient.right - rectClient.left,
   //   //rectClient.bottom - rectClient.top };
   //   //   
   //   //   if (m_size != sizeNew)
   //   //   {
   //   //      m_size = sizeNew;


   //   //      //HDC pdcDIB;                      // контекст устройства в памяти
   //   //      //HBITMAP hbmpDIB;                 // и его текущий битмапvoid *pBitsDIB(NULL);            // содержимое битмапаint cxDIB(200); int cyDIB(300);  // его размеры (например для окна 200х300)
   //   //      //auto &BIH=pwindow->m_bitmapinfoheaderProto;            // и заголовок// …// создаем DIB section// создаем структуру BITMAPINFOHEADER, описывающую наш DIBint iSize = sizeof(BITMAPINFOHEADER);  // размер
   //   //      //memset(&BIH, 0, sizeof(pwindow->m_bitmapinfoheaderProto));

   //   //      //BIH.biSize = sizeof(pwindow->m_bitmapinfoheaderProto);        // размер структуры
   //   //      //BIH.biWidth = m_size.cx();       // геометрия
   //   //      //BIH.biHeight = m_size.cy();      // битмапа
   //   //      //BIH.biPlanes = 1;          // один план
   //   //      //BIH.biBitCount = 32;       // 24 bits per pixel
   //   //      //BIH.biCompression = BI_RGB;// без сжатия// создаем новый DC в памяти
   //   //      ////pdcDIB = CreateCompatibleDC(NULL);
   //   //      ////void * pBits = nullptr;
   //   //      //// создаем DIB-секцию
   //   //      //pwindow->m_hbitmapProto = CreateDIBSection(
   //   //      //  m_hdc,                  // контекст устройства
   //   //      //  (BITMAPINFO *)&BIH,       // информация о битмапе
   //   //      //  DIB_RGB_COLORS,          // параметры цвета
   //   //      //  &pwindow->m_pbitsProto,               // местоположение буфера (память выделяет система)
   //   //      //  NULL,                    // не привязываемся к отображаемым в память файлам
   //   //      //  0);

   //   //      //// выберем новый битмап (DIB section) для контекста устройства в памяти
   //   //      //SelectObject(m_hdc, pwindow->m_hbitmapProto);
   //   //      //pwindow->m_hdcProto = m_hdc;
   //   //   }

   //   //   m_itaskGpu = ::current_itask();

   //   //   m_estatus = ::success;

   //   //   set_ok_flag();

   //}


   //void device::_create_offscreen_buffer(const ::int_size& size)
   //{

   //   _create_offscreen_window(size);

   //   //_create_window_buffer();

   //   m_itaskGpu = ::current_itask();

   //}


   //void device::defer_create_window_context(::windowing::window* pwindow)
   //{

   //   //if (m_hrc)
   //   //{

   //   //   return;

   //   //}

   //   //::directx11::device::defer_create_window_context(pwindow);

   //}


   //void device::_defer_create_window_context(::windowing::window* pwindow)
   //{

   //   _create_window_context(pwindow);

   //}


   //void device::resize_offscreen_buffer(const ::int_size& sizeParam)
   //{

   //   if (m_papplication->m_bUseSwapChainWindow)
   //   {

   //      return;

   //   }

   //   auto size(sizeParam);

   //   send([this, size]()
   //      {
   //         //if (!m_pcpubuffer)

   //         create_offscreen_buffer(size);


   //         ///m_pcpubuffer->m_pixmap.create(m_pcpubuffer->m_memory, size);

   //   //#ifdef WINDOWS_DESKTOP
   //   //
   //   //      ::SetWindowPos(m_hwnd, 0, 0, 0, size.cx(), size.cy(), SWP_NOZORDER | SWP_NOMOVE | SWP_HIDEWINDOW);
   //   //
   //   //#else

   //   //      destroy_offscreen_buffer();
   //   //
   //   //      if(!create_offscreen_buffer(size))
   //   //      {
   //   //
   //   //         return error_failed;
   //   //
   //   //      }

   //   //#endif

   //         make_current();

   //         //glViewport(0, 0, size.cx(), size.cy());
   //         //glMatrixMode(GL_PROJECTION);
   //         //glLoadIdentity();
   //         //glOrtho(0, size.cx(), 0, size.cy(), -10, 10);
   //         //glMatrixMode(GL_MODELVIEW);
   //         //glutPostRedisplay();

   //         //return ::success;
   //      });


   //}


   //void device::make_current()
   //{

   //   ASSERT(m_itaskGpu == ::current_itask());

   //   ::e_status estatus = ::success;

   //   //bool bMakeCurrentOk = wglMakeCurrent(m_hdc, m_hrc);

   //   //if (!bMakeCurrentOk)
   //   //{

   //   //   informationf("MS WGL - wglMakeCurrent failed");

   //   //   informationf("last-error code: %d\n", GetLastError());

   //   //   throw ::exception(error_failed);

   //   //}

   //   //return estatus;

   //}


   //void device::destroy_offscreen_buffer()
   //{

   //   //ASSERT(m_itaskGpu == ::current_itask());

   //   //::e_status estatus = ::success;

   //   //if (m_hrc == NULL && m_hdc == NULL && m_hwnd == NULL)
   //   //{

   //   //   return;

   //   //}

   //   ////wglMakeCurrent(nullptr, nullptr);
   //   ////wglDeleteContext(m_hrc);
   //   //::ReleaseDC(m_hwnd, m_hdc);
   //   //::DestroyWindow(m_hwnd);
   //   //m_size.set(0, 0);
   //   //m_hrc = NULL;
   //   //m_hwnd = NULL;
   //   //m_hdc = NULL;

   //   ////return estatus;

   //}

   //void device::render()
   //{

   //   ::gpu_directx11::device::render();



   //}


   //void device::swap_buffers()
   //{

   //   //SwapBuffers(m_hdc);

   //}


   string device::get_shader_version_text()
   {

      return "#version 330 core";

   }


   void device::_translate_shader(string_array& stra)
   {

      ::gpu::device::_translate_shader(stra);

      character_count iFindPrecision = stra.case_insensitive_find_first_begins("precision ");

      if (iFindPrecision >= 0)
      {

         stra[iFindPrecision] = "precision highp float;";

      }
      else
      {

         stra.insert_at(1, "precision highp float;");

         iFindPrecision = 1;

      }

   }


   //// local callback functions
   //static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
   //   VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
   //   VkDebugUtilsMessageTypeFlagsEXT messageType,
   //   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
   //   void* pUserData) {
   //   //std::cerr << "validation layer: " << pCallbackData->pMessage;
   //   warning() << "validation layer: " << pCallbackData->pMessage;
   //   return VK_FALSE;
   //}

   //HRESULT CreateDebugUtilsMessengerEXT(
   //   VkInstance m_vkinstance,
   //   const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
   //   const VkAllocationCallbacks* pAllocator,
   //   VkDebugUtilsMessengerEXT* pDebugMessenger)
   //{
   //   auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
   //      m_vkinstance,
   //      "vkCreateDebugUtilsMessengerEXT");
   //   if (func != nullptr) {
   //      return func(m_vkinstance, pCreateInfo, pAllocator, pDebugMessenger);
   //   }
   //   else {
   //      return VK_ERROR_EXTENSION_NOT_PRESENT;
   //   }
   //}

   //void DestroyDebugUtilsMessengerEXT(
   //   VkInstance m_vkinstance,
   //   VkDebugUtilsMessengerEXT debugMessenger,
   //   const VkAllocationCallbacks* pAllocator) {
   //   auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
   //      m_vkinstance,
   //      "vkDestroyDebugUtilsMessengerEXT");
   //   if (func != nullptr) {
   //      func(m_vkinstance, debugMessenger, pAllocator);
   //   }
   //}

   //// class member functions
   ////device::device(::graphics3d_directx11::DirectX11Device* pgpudevice) : m_vkdevice{pgpudevice->logicalDevice} {
   //device::device()
   //{

   //   m_vksurfacekhr = nullptr;
   //   m_vkqueuePresent = nullptr;
   //   m_vkqueueGraphics = nullptr;

   //   createInstance();
   //   setupDebugMessenger();
   //   createSurface();
   //   pickPhysicalDevice();
   //   createLogicalDevice();
   //   createCommandPool();
   //}

   //device::~device()
   //{
   //   vkDestroyCommandPool(m_vkdevice, m_vkcommandpool, nullptr);
   //   vkDestroyDevice(m_vkdevice, nullptr);

   //   if (enableValidationLayers) {
   //      DestroyDebugUtilsMessengerEXT(m_vkinstance, debugMessenger, nullptr);
   //   }

   //   if (m_vksurfacekhr)
   //   {
   //      vkDestroySurfaceKHR(m_vkinstance, m_vksurfacekhr, nullptr);
   //   }
   //   vkDestroyInstance(m_vkinstance, nullptr);

   //}


   //void device::initialize_context(::user::graphics3d * pimpact)
   //{

   //   m_pimpact = pimpact;


   //   validationLayers.add("VK_LAYER_KHRONOS_validation");

   //   if (m_papplication->m_bUseSwapChainWindow)
   //   {

   //      deviceExtensions.add(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

   //   }


   //}


   //void device::createInstance()
   //{

   //   if (enableValidationLayers && !checkValidationLayerSupport()) {
   //      throw ::exception(error_failed,"validation layers requested, but not available!");
   //   }

   //   VkApplicationInfo appInfo = {};
   //   appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
   //   appInfo.pApplicationName = "LittleDirectX11Engine App";
   //   appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
   //   appInfo.pEngineName = "No Engine";
   //   appInfo.approachVersion = VK_MAKE_VERSION(1, 0, 0);
   //   appInfo.apiVersion = VK_API_VERSION_1_0;

   //   VkInstanceCreateInfo createInfo = {};
   //   createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
   //   createInfo.pApplicationInfo = &appInfo;

   //   auto extensions = getRequiredExtensions();
   //   createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
   //   createInfo.ppEnabledExtensionNames = extensions.data();

   //   VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
   //   if (enableValidationLayers) {
   //      createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
   //      createInfo.ppEnabledLayerNames = validationLayers.data();

   //      populateDebugMessengerCreateInfo(debugCreateInfo);
   //      createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
   //   }
   //   else {
   //      createInfo.enabledLayerCount = 0;
   //      createInfo.pNext = nullptr;
   //   }

   //   if (vkCreateInstance(&createInfo, nullptr, &m_vkinstance) != VK_SUCCESS) {
   //      throw ::exception(error_failed,"failed to create m_vkinstance!");
   //   }

   //   hasGflwRequiredInstanceExtensions();
   //}


   //void device::pickPhysicalDevice()
   //{
   //   uint32_t deviceCount = 0;
   //   vkEnumeratePhysicalDevices(m_vkinstance, &deviceCount, nullptr);
   //   if (deviceCount == 0) {
   //      throw ::exception(error_failed,"failed to find GPUs with DirectX11 support!");
   //   }
   //   information() << "Device count: " << deviceCount;
   //   ::array<VkPhysicalDevice> devices(deviceCount);
   //   vkEnumeratePhysicalDevices(m_vkinstance, &deviceCount, devices.data());

   //   for (const auto & pvkcdevice : devices) {
   //      if (isDeviceSuitable(pvkcdevice)) {
   //         physicalDevice = pvkcdevice;
   //         break;
   //      }
   //   }

   //   if (physicalDevice == VK_NULL_HANDLE) {
   //      throw ::exception(error_failed,"failed to find a suitable GPU!");
   //   }

   //   vkGetPhysicalDeviceProperties(physicalDevice, &properties);
   //   information() << "physical pvkcdevice: " << properties.deviceName;
   //}


   //void device::createLogicalDevice()
   //{

   //   QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

   //   ::array<VkDeviceQueueCreateInfo> queueCreateInfos;
   //   std::set<uint32_t> uniqueQueueFamilies;
   //   if (indices.graphicsFamilyHasValue)
   //   {
   //      uniqueQueueFamilies.insert(indices.graphicsFamily);
   //   }
   //   if (indices.presentFamilyHasValue)
   //   {
   //      uniqueQueueFamilies.insert(indices.presentFamily);
   //   }

   //   float queuePriority = 1.0f;
   //   for (uint32_t queueFamily : uniqueQueueFamilies)
   //   {
   //      VkDeviceQueueCreateInfo queueCreateInfo = {};
   //      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
   //      queueCreateInfo.queueFamilyIndex = queueFamily;
   //      queueCreateInfo.queueCount = 1;
   //      queueCreateInfo.pQueuePriorities = &queuePriority;
   //      queueCreateInfos.add(queueCreateInfo);
   //   }

   //   VkPhysicalDeviceFeatures deviceFeatures = {};
   //   deviceFeatures.samplerAnisotropy = VK_TRUE;

   //   VkDeviceCreateInfo createInfo = {};
   //   createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

   //   createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
   //   createInfo.pQueueCreateInfos = queueCreateInfos.data();

   //   createInfo.pEnabledFeatures = &deviceFeatures;
   //   createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
   //   createInfo.ppEnabledExtensionNames = deviceExtensions.data();

   //   // might not really be necessary anymore because pvkcdevice specific validation layers
   //   // have been deprecated
   //   if (enableValidationLayers)
   //   {
   //      createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
   //      createInfo.ppEnabledLayerNames = validationLayers.data();
   //   }
   //   else
   //   {
   //      createInfo.enabledLayerCount = 0;
   //   }

   //   if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_vkdevice) != VK_SUCCESS)
   //   {
   //      throw ::exception(error_failed,"failed to create logical pvkcdevice!");
   //   }
   //   if (indices.graphicsFamilyHasValue)
   //   {
   //      vkGetDeviceQueue(m_vkdevice, indices.graphicsFamily, 0, &m_vkqueueGraphics);
   //   }
   //   if (indices.presentFamilyHasValue)
   //   {
   //      vkGetDeviceQueue(m_vkdevice, indices.presentFamily, 0, &m_vkqueuePresent);
   //   }
   //}


   //void device::createCommandPool()
   //{
   //   QueueFamilyIndices queueFamilyIndices = findPhysicalQueueFamilies();

   //   VkCommandPoolCreateInfo poolInfo = {};
   //   poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
   //   poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
   //   poolInfo.flags =
   //      VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

   //   if (vkCreateCommandPool(m_vkdevice, &poolInfo, nullptr, &m_vkcommandpool) != VK_SUCCESS) {
   //      throw ::exception(error_failed,"failed to create command pool!");
   //   }
   //}

   //void device::createSurface() { window.createWindowSurface(m_vkinstance, &m_vksurfacekhr); }

   //void device::_createSurface() {}


   //bool device::isDeviceSuitable(VkPhysicalDevice pvkcdevice)
   //{

   //   QueueFamilyIndices indices = findQueueFamilies(pvkcdevice);

   //   bool extensionsSupported = checkDeviceExtensionSupport(pvkcdevice);

   //   bool swapChainAdequate = false;
   //   if (m_vksurfacekhr)
   //   {
   //      if (extensionsSupported)
   //      {
   //         SwapChainSupportDetails swapChainSupport = querySwapChainSupport(pvkcdevice);
   //         swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
   //      }
   //   }
   //   else
   //   {

   //      swapChainAdequate = true;

   //   }

   //   VkPhysicalDeviceFeatures supportedFeatures;
   //   vkGetPhysicalDeviceFeatures(pvkcdevice, &supportedFeatures);

   //   return (!m_vksurfacekhr || indices.isComplete()) && extensionsSupported && swapChainAdequate &&
   //      supportedFeatures.samplerAnisotropy;

   //}


   //void device::populateDebugMessengerCreateInfo(
   //   VkDebugUtilsMessengerCreateInfoEXT& createInfo)
   //{

   //   createInfo = {};
   //   createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
   //   createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
   //      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
   //   createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
   //      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
   //      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
   //   createInfo.pfnUserCallback = debugCallback;
   //   createInfo.pUserData = nullptr;  // Optional

   //}


   //void device::setupDebugMessenger()
   //{

   //   if (!enableValidationLayers)
   //   {

   //      return;

   //   }

   //   VkDebugUtilsMessengerCreateInfoEXT createInfo;

   //   populateDebugMessengerCreateInfo(createInfo);

   //   if (CreateDebugUtilsMessengerEXT(m_vkinstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
   //   {

   //      throw ::exception(error_failed,"failed to set up debug messenger!");

   //   }

   //}


   bool device::checkValidationLayerSupport()
   {
      //uint32_t layerCount;
      //vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

      //::array<VkLayerProperties> availableLayers(layerCount);
      //vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

      //for (const char* layerName : validationLayers)
      //{
      //   bool layerFound = false;


      //   printf_line("checking for validation layer : %s", layerName);
      //   for (const auto& layerProperties : availableLayers)
      //   {
      //      printf_line("an available layer : %s", layerProperties.layerName);
      //      if (strcmp(layerName, layerProperties.layerName) == 0)
      //      {
      //         layerFound = true;
      //         break;
      //      }
      //   }

      //   if (!layerFound) {
      //      return false;
      //   }
      //}

      return true;

   }


   //::array<const char *> device::getRequiredExtensions()
   //{
   //   uint32_t glfwExtensionCount = 0;
   //   const char ** glfwExtensions;
   //   //glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

   //   //::array<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
   //   ::array<const char *> extensions;

   //   if (enableValidationLayers)
   //   {
   //      extensions.add(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
   //   }

   //   return extensions;
   //   return {};
   //}


   //void device::hasGflwRequiredInstanceExtensions()
   //{
   //   uint32_t extensionCount = 0;
   //   vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
   //   ::array<VkExtensionProperties> extensions(extensionCount);
   //   vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

   //   information() << "available extensions:";
   //   set<::string> available;
   //   for (const auto & extension : extensions) {
   //      information() << "\t" << extension.extensionName;
   //      available.set_at(extension.extensionName);
   //   }

   //   information() << "required extensions:";
   //   auto requiredExtensions = getRequiredExtensions();
   //   for (const auto & required : requiredExtensions)
   //   {
   //      information() << "\t" << required;
   //      if (!available.plookup(required)) {
   //         throw ::exception(error_failed,"Missing required glfw extension");
   //      }
   //   }

   //}


   //bool device::checkDeviceExtensionSupport(VkPhysicalDevice pvkcdevice)
   //{

   //   uint32_t extensionCount;
   //   vkEnumerateDeviceExtensionProperties(pvkcdevice, nullptr, &extensionCount, nullptr);

   //   ::array<VkExtensionProperties> availableExtensions(extensionCount);
   //   vkEnumerateDeviceExtensionProperties(
   //      pvkcdevice,
   //      nullptr,
   //      &extensionCount,
   //      availableExtensions.data());

   //   set<::string> requiredExtensions(deviceExtensions);

   //   for (const auto & extension : availableExtensions) {
   //      requiredExtensions.erase(extension.extensionName);
   //   }

   //   return requiredExtensions.empty();
   //}


   //void device::createBuffer(
   //   VkDeviceSize size,
   //   VkBufferUsageFlags usage,
   //   VkMemoryPropertyFlags properties,
   //   VkBuffer& buffer,
   //   VkDeviceMemory& bufferMemory)
   //{

   //   VkBufferCreateInfo bufferInfo{};
   //   bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
   //   bufferInfo.size = size;
   //   bufferInfo.usage = usage;
   //   bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

   //   if (vkCreateBuffer(m_vkdevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
   //   {

   //      throw ::exception(error_failed, "failed to create vertex buffer!");

   //   }

   //   VkMemoryRequirements memRequirements;
   //   vkGetBufferMemoryRequirements(m_vkdevice, buffer, &memRequirements);

   //   VkMemoryAllocateInfo allocInfo{};
   //   allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
   //   allocInfo.allocationSize = memRequirements.size;
   //   allocInfo.memoryTypeIndex = m_pphysicaldevice->findMemoryType(memRequirements.memoryTypeBits, properties);

   //   if (vkAllocateMemory(m_vkdevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
   //   {

   //      throw ::exception(error_failed, "failed to allocate vertex buffer memory!");

   //   }

   //   vkBindBufferMemory(m_vkdevice, buffer, bufferMemory, 0);

   //}


   ////VkCommandBuffer device::beginSingleTimeCommands()
   ////{

   ////   VkCommandBufferAllocateInfo allocInfo{};
   ////   allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
   ////   allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
   ////   allocInfo.commandPool = m_vkcommandpool;
   ////   allocInfo.commandBufferCount = 1;

   ////   VkCommandBuffer commandBuffer;
   ////   vkAllocateCommandBuffers(m_vkdevice, &allocInfo, &commandBuffer);

   ////   VkCommandBufferBeginInfo beginInfo{};
   ////   beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
   ////   beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

   ////   vkBeginCommandBuffer(commandBuffer, &beginInfo);
   ////   return commandBuffer;

   ////}



   //void device::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
   //{
   //   VkCommandBuffer commandBuffer = beginSingleTimeCommands();

   //   VkBufferCopy copyRegion{};
   //   copyRegion.srcOffset = 0;  // Optional
   //   copyRegion.dstOffset = 0;  // Optional
   //   copyRegion.size = size;
   //   vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

   //   endSingleTimeCommands(commandBuffer);

   //}


   //void device::copyBufferToImage(
   //   VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount)
   //{

   //   VkCommandBuffer commandBuffer = beginSingleTimeCommands();

   //   VkBufferImageCopy region{};
   //   region.bufferOffset = 0;
   //   region.bufferRowLength = 0;
   //   region.bufferImageHeight = 0;

   //   region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
   //   region.imageSubresource.mipLevel = 0;
   //   region.imageSubresource.baseArrayLayer = 0;
   //   region.imageSubresource.layerCount = layerCount;

   //   region.imageOffset = { 0, 0, 0 };
   //   region.imageExtent = { width, height, 1 };

   //   vkCmdCopyBufferToImage(
   //      commandBuffer,
   //      buffer,
   //      image,
   //      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
   //      1,
   //      &region);

   //   endSingleTimeCommands(commandBuffer);

   //}


   //void device::createImageWithInfo(
   //   const VkImageCreateInfo& imageInfo,
   //   VkMemoryPropertyFlags properties,
   //   VkImage& image,
   //   VkDeviceMemory& imageMemory)
   //{

   //   if (vkCreateImage(m_vkdevice, &imageInfo, nullptr, &image) != VK_SUCCESS)
   //   {

   //      throw ::exception(error_failed, "failed to create image!");

   //   }

   //   VkMemoryRequirements memRequirements;
   //   vkGetImageMemoryRequirements(m_vkdevice, image, &memRequirements);

   //   VkMemoryAllocateInfo allocInfo{};
   //   allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
   //   allocInfo.allocationSize = memRequirements.size;
   //   allocInfo.memoryTypeIndex = m_pphysicaldevice->findMemoryType(memRequirements.memoryTypeBits, properties);

   //   if (vkAllocateMemory(m_vkdevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
   //      throw ::exception(error_failed, "failed to allocate image memory!");
   //   }

   //   if (vkBindImageMemory(m_vkdevice, image, imageMemory, 0) != VK_SUCCESS) {
   //      throw ::exception(error_failed, "failed to bind image memory!");
   //   }
   //}





   //void device::submitWork(VkCommandBuffer cmdBuffer, VkQueue queue)
   //{
   //   VkSubmitInfo submitInfo = initializers::submitInfo();
   //   submitInfo.commandBufferCount = 1;
   //   submitInfo.pCommandBuffers = &cmdBuffer;
   //   //m_submitInfo.commandBufferCount = 1;
   //   //m_submitInfo.pCommandBuffers = &cmdBuffer;
   //   VkFenceCreateInfo fenceInfo = initializers::fenceCreateInfo();
   //   VkFence fence;
   //   VK_CHECK_RESULT(vkCreateFence(m_vkdevice, &fenceInfo, nullptr, &fence));
   //   VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));
   //   VK_CHECK_RESULT(vkWaitForFences(m_vkdevice, 1, &fence, VK_TRUE, UINT64_MAX));
   //   vkDestroyFence(m_vkdevice, fence, nullptr);
   //}


   //void device::submitSamplingWork(VkCommandBuffer cmdBuffer, VkQueue queue)
   //{
   //   VkSubmitInfo submitInfo = initializers::submit_info();
   //   submitInfo.commandBufferCount = 1;
   //   submitInfo.pCommandBuffers = &cmdBuffer;
   //   //m_submitInfo.commandBufferCount = 1;
   //   //m_submitInfo.pCommandBuffers = &cmdBuffer;
   //   VkFenceCreateInfo fenceInfo = initializers::fence_create_info();
   //   VkFence fence;
   //   VK_CHECK_RESULT(vkCreateFence(m_vkdevice, &fenceInfo, nullptr, &fence));
   //   VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));
   //   VK_CHECK_RESULT(vkWaitForFences(m_vkdevice, 1, &fence, VK_TRUE, UINT64_MAX));
   //   vkDestroyFence(m_vkdevice, fence, nullptr);
   //}


   //directx11::QueueFamilyIndices physical_device::findQueueFamilies()
   //{

   //   directx11::QueueFamilyIndices indices;

   //   uint32_t queueFamilyCount = 0;
   //   vkGetPhysicalDeviceQueueFamilyProperties(m_physicaldevice, &queueFamilyCount, nullptr);

   //   ::array<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
   //   vkGetPhysicalDeviceQueueFamilyProperties(m_physicaldevice, &queueFamilyCount, queueFamilies.data());

   //   int i = 0;
   //   for (const auto& queueFamily : queueFamilies)
   //   {
   //      if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
   //      {
   //         indices.graphicsFamily = i;
   //         indices.graphicsFamilyHasValue = true;
   //      }
   //      if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
   //      {
   //         indices.computeFamily = i;
   //         indices.computeFamilyHasValue = true;
   //      }
   //      if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
   //      {
   //         indices.transferFamily = i;
   //         indices.transferFamilyHasValue = true;
   //      }
   //      if (m_vksurfacekhr)
   //      {
   //         VkBool32 presentSupport = false;
   //         vkGetPhysicalDeviceSurfaceSupportKHR(m_physicaldevice, i, m_vksurfacekhr, &presentSupport);
   //         if (queueFamily.queueCount > 0 && presentSupport)
   //         {
   //            indices.presentFamily = i;
   //            indices.presentFamilyHasValue = true;
   //         }
   //      }
   //      //if (indices.isComplete()) {
   //        // break;
   //      //}

   //      i++;
   //   }

   //   return indices;

   //}


   SwapChainSupportDetails physical_device::querySwapChainSupport()
   {

      SwapChainSupportDetails details{};

      //if (!m_vksurfacekhr)
      //{

      //   throw ::exception(error_wrong_state, "querying swap chain support but no vksurfacekhr");

      //}

      //SwapChainSupportDetails details{};

      //vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicaldevice, m_vksurfacekhr, &details.capabilities);

      //uint32_t formatCount;
      //vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicaldevice, m_vksurfacekhr, &formatCount, nullptr);

      //if (formatCount != 0) {
      //   details.formats.resize(formatCount);
      //   vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicaldevice, m_vksurfacekhr, &formatCount, details.formats.data());
      //}

      //uint32_t presentModeCount;
      //vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicaldevice, m_vksurfacekhr, &presentModeCount, nullptr);

      //if (presentModeCount != 0) {
      //   details.presentModes.resize(presentModeCount);
      //   vkGetPhysicalDeviceSurfacePresentModesKHR(
      //      m_physicaldevice,
      //      m_vksurfacekhr,
      //      &presentModeCount,
      //      details.presentModes.data());
      //}

      return details;

   }


   void device::on_top_end_frame()
   {

      auto procedureaOnTopFrameEnd = ::transfer(m_procedureaOnTopFrameEnd);

      for (auto& procedure : procedureaOnTopFrameEnd)
      {

         procedure();

      }

   }


   void device::initialize_swap_chain(::windowing::window* pwindow)
   {

      ::cast < ::windowing_win32::window > pwin32window = pwindow;

      auto r = pwindow->get_window_rectangle();

      //// Swap chain description
      //DXGI_SWAP_CHAIN_DESC dxgiswapchaindesc = {};
      //dxgiswapchaindesc.BufferDesc.Width = r.width();
      //dxgiswapchaindesc.BufferDesc.Height = r.height();
      //dxgiswapchaindesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      //dxgiswapchaindesc.BufferCount = 2;
      //dxgiswapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      //dxgiswapchaindesc.OutputWindow = pwin32window->m_hwnd;
      //dxgiswapchaindesc.SampleDesc.Count = 1;
      //dxgiswapchaindesc.Windowed = TRUE;
      //dxgiswapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
      //dxgiswapchaindesc.Flags = 0;

      //comptr<IDXGISwapChain> swapchain;
      //comptr<ID3D11Device> device;

      //comptr<ID3D11DeviceContext> context;
     
      //HRESULT hr = D3D11CreateDeviceAndSwapChain(
      //   nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
      //   D3D11_CREATE_DEVICE_BGRA_SUPPORT,
      //   nullptr, 0,
      //   D3D11_SDK_VERSION, 
      //   &dxgiswapchaindesc,
      //   &swapchain,
      //   &device,
      //   &m_featurelevel,
      //   &context
      //);

      ::defer_throw_hresult(D3D11CreateDevice(nullptr,    // Adapter
         D3D_DRIVER_TYPE_HARDWARE,
         nullptr,    // Module
         D3D11_CREATE_DEVICE_BGRA_SUPPORT,
         nullptr, 0, // Highest available feature level
         D3D11_SDK_VERSION,
         &m_pdevice,
         nullptr,    // Actual feature level
         nullptr));  // Device context
      ::defer_throw_hresult(m_pdevice.as(m_pdevice1));
      ::defer_throw_hresult(m_pdevice.as(m_pdxgidevice));

      ::defer_throw_hresult(CreateDXGIFactory2(
         DXGI_CREATE_FACTORY_DEBUG,
         __uuidof(m_pdxgifactory2),
         reinterpret_cast<void**>(&m_pdxgifactory2)));

      DXGI_SWAP_CHAIN_DESC1 dxgiswapchaindesc1 = {};
      dxgiswapchaindesc1.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      dxgiswapchaindesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      dxgiswapchaindesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
      dxgiswapchaindesc1.BufferCount = 2;
      dxgiswapchaindesc1.SampleDesc.Count = 1;
      dxgiswapchaindesc1.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
      RECT rect = {};
      GetWindowRect(pwin32window->m_hwnd, &rect);
      dxgiswapchaindesc1.Width = rect.right - rect.left;
      dxgiswapchaindesc1.Height = rect.bottom - rect.top;
      ::defer_throw_hresult(m_pdxgifactory2->CreateSwapChainForComposition(m_pdxgidevice,
         &dxgiswapchaindesc1,
         nullptr, // Don’t restrict
         &m_pdxgiswapchain1));
      //assert(SUCCEEDED(hr));
      //::defer_throw_hresult(hr);

      //::defer_throw_hresult(swapchain.as(m_pdxgiswapchain));

      // Get the Direct3D 11.1 API device and context interfaces.
      //::defer_throw_hresult(device.as(m_pdevice));

      m_pdevice->GetImmediateContext(&m_pdevicecontext);

      //::defer_throw_hresult(context.as(m_pdevicecontext));


      // Get the underlying DXGI device of the Direct3D device.
      

#if defined(_DEBUG)

      ::directx11::defer_dxgi_debug_initialize();

#endif

      m_pdxgiswapchain1->GetBuffer(0, IID_PPV_ARGS(&m_ptextureBackBuffer));
      
      m_pdevice->CreateRenderTargetView(m_ptextureBackBuffer, nullptr, &m_prendertargetviewBackBuffer);

      ::defer_throw_hresult(DCompositionCreateDevice(
         m_pdxgidevice,
         __uuidof(m_pdcompositiondevice),
         (void **) & m_pdcompositiondevice));


      ::defer_throw_hresult(m_pdcompositiondevice->CreateTargetForHwnd(pwin32window->m_hwnd,
         true, // Top most
         &m_pdcompositiontarget));

      ::defer_throw_hresult(m_pdcompositiondevice->CreateVisual(&m_pdcompositionvisual));
      ::defer_throw_hresult(m_pdcompositionvisual->SetContent(m_pdxgiswapchain1));
      ::defer_throw_hresult(m_pdcompositiontarget->SetRoot(m_pdcompositionvisual));
      ::defer_throw_hresult(m_pdcompositiondevice->Commit());

   }


   void device::initialize_cpu_buffer(::windowing::window * pwindow)
   {

      // This flag adds support for surfaces with a different color channel ordering
      // than the API default. It is required for compatibility with Direct2D.
      unsigned int creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(__DEBUG)

      // If the project is in a debug build, enable debugging via SDK Layers with this flag.
      creationFlags |= D3D11_CREATE_DEVICE_DEBUG;

#endif

      // This array defines the set of DirectX11 hardware feature levels this app will support.
      // Note the ordering should be preserved.
      // Don't forget to declare your application's minimum required feature level in its
      // description.  All applications are assumed to support 9.1 unless otherwise stated.
      D3D_FEATURE_LEVEL featureLevels[] =
      {

         D3D_FEATURE_LEVEL_11_1,
         D3D_FEATURE_LEVEL_11_0,
         D3D_FEATURE_LEVEL_10_1,
         D3D_FEATURE_LEVEL_10_0,
         D3D_FEATURE_LEVEL_9_3,
         D3D_FEATURE_LEVEL_9_2,
         D3D_FEATURE_LEVEL_9_1

      };

      // Create the Direct3D 11 API device object and a corresponding context.
      comptr<ID3D11Device> pdevice;

      comptr<ID3D11DeviceContext> pdevicecontext;

      HRESULT hr = D3D11CreateDevice(
         nullptr,                    // Specify nullptr to use the default adapter.
         D3D_DRIVER_TYPE_HARDWARE,
         0,
         creationFlags,              // Set debug and Direct2D compatibility flags.
         featureLevels,              // List of feature levels this app can support.
         ARRAYSIZE(featureLevels),
         D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Metro style apps.
         &pdevice,                    // Returns the Direct3D device created.
         &m_featurelevel,            // Returns feature level of device created.
         &pdevicecontext                    // Returns the device immediate context.
      );

      ::defer_throw_hresult(hr);

      // Get the Direct3D 11.1 API device and context interfaces.
      ::defer_throw_hresult(pdevice.as(m_pdevice));

      ::defer_throw_hresult(pdevice.as(m_pdevice1));

      ::defer_throw_hresult(pdevicecontext.as(m_pdevicecontext));


      // Get the underlying DXGI device of the Direct3D device.
      ::defer_throw_hresult(m_pdevice.as(m_pdxgidevice));

#if defined(_DEBUG)

      ::directx11::defer_dxgi_debug_initialize();

#endif

      //return ::success;

   }


   ID3D11Device* device::draw_get_d3d11_device()
   {

      return m_pdevice;

   }


   ID3D11Device1* device::draw_get_d3d11_device1()
   {

      return m_pdevice1;

   }



   IDXGIDevice* device::draw_get_dxgi_device()
   {

      return m_pdxgidevice;

   }


   typedef HRESULT WINAPI FN_DXGIGetDebugInterface(REFIID riid, void** ppDebug);


   typedef FN_DXGIGetDebugInterface* PFN_DXGIGetDebugInterface;


   //CLASS_DECL_DIRECTX11 void defer_initialize(::particle * pparticle)
   //{

   //   //if (::is_set(directx11::s_pdirectx11))
   //   //{

   //   //   return;

   //   //}

   //   directx11::s_pdirectx11 = ___new class directx11;

   //   directx11::s_pdirectx11->initialize(pparticle);

   //}


   //CLASS_DECL_DIRECTX11 void terminate()
   //{

   //   ::acme::del(directx11::s_pdirectx11);

   //}

   int device::get_type_size(::gpu::enum_type etype)
   {

      switch (etype)
      {
      case ::gpu::e_type_int: return sizeof(int);
      case ::gpu::e_type_float: return sizeof(float);
      case ::gpu::e_type_seq4: return sizeof(::glm::vec4);
      case ::gpu::e_type_mat4: return sizeof(::glm::mat4);
      default:
         throw ::exception(error_wrong_state);

      }


   }

   void device::set_mat4(void* p, const ::glm::mat4& mat4)
   {

      auto m = GLMToDX_Transposed(mat4);
      auto& mTarget = *((decltype(&m))p);
      mTarget = m;

   }


} // namespace gpu_directx11



