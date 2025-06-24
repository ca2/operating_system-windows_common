#include "framework.h"
#include "approach.h"
#include "buffer.h"
#include "context.h"
#include "device.h"
#include "physical_device.h"
#include "program.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "swap_chain.h"
#include "swap_chain_render_target_view.h"

#include "offscreen_render_target_view.h"
#include "acme/platform/application.h"
#include "aura/graphics/image/image.h"
#include "aura/user/user/interaction.h"
#include "bred/gpu/graphics.h"
#include "bred/gpu/layer.h"
#include "bred/gpu/types.h"
#include "gpu_directx11/descriptors.h"
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/mat4x4.hpp>
#include "initializers.h"
#include "acme_windows_common/dxgi_surface_bindable.h"


using namespace directx11;


namespace gpu_directx11
{


   extern thread_local device* t_pgpudevice;

   context::context()
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
      //this->logicalDevice() = VK_NULL_HANDLE;
      //m_vkcommandpool = VK_NULL_HANDLE;

      //m_vkqueuePresent = nullptr;
      //m_vkqueueGraphics = nullptr;



   }


   context::~context()
   {

   }


   void context::draw()
   {

      ASSERT(is_current_task());

      //      if (m_VAO != 0)
      //      {
      //
      //         //return ::success;
      //         return;
      //
      //      }
      //      //glGenBuffers(1, &VAO);
      ////      float vertices[] = {f
      //         // positions         // colors
      //  //        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
      //    //     -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
      //      //    0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
      //      //};
      //      float vertices[] = {
      //         // first triangle
      //          1.f,  1.f, 0.0f,  // top right
      //          1.f, -1.f, 0.0f,  // bottom right
      //         -1.f,  1.f, 0.0f,  // top left
      //         // second triangle
      //          1.f, -1.f, 0.0f,  // bottom right
      //         -1.f, -1.f, 0.0f,  // bottom left
      //         -1.f,  1.f, 0.0f   // top left
      //      };
      //
      //#if defined(__APPLE__)
      //
      //      glGenVertexArrays(1, &m_VAO);
      //      glGenBuffers(1, &m_VBO);
      //      // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
      //      glBindVertexArray(m_VAO);
      //
      //      //#elif defined(__ANDROID__)
      //      //
      //      //      glGenVertexArrays(1, &m_VAO);
      //      //      glGenBuffers(1, &m_VBO);
      //      //      // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
      //      //      glBindVertexArray(m_VAO);
      //
      //#else
      //
      ////      glGenVertexArrays(1, &m_VAO);
      ////      glGenBuffers(1, &m_VBO);
      ////      // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
      ////      glBindVertexArray(m_VAO);
      ////
      //#endif
      ////
      ////      glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
      ////      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      ////
      ////      // position attribute
      ////      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
      ////      glEnableVertexAttribArray(0);
      ////      // color attribute
      ////      //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
      ////      //glEnableVertexAttribArray(1);
      ////
      ////      //return ::success;
   }


   void context::start_drawing()
   {


      //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_elements);
//int iError16 = glGetError();

//int size = 0; 
//glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
//int iError17 = glGetError();

//glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
//int iError18 = glGetError();




      //ASSERT(is_current_task());

      ////      glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

      //      // Clear the screen
      //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      ////glClear(GL_COLOR_BUFFER_BIT);

      //if (m_pprogram && m_pprogram->m_pshader)
      //{

      //   m_pprogram->m_pshader->use();

      //}
      //// Use our shader
      ////glUseProgram(programID);

      //// be sure to activate the shader
      ////glUseProgram(shaderProgram);

      //// update the uniform color
      ////float timeValue = glfwGetTime();
      ////float greenValue = sin(timeValue) / 2.0f + 0.5f;
      ////int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
      ////glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

      ////return ::success;

   }



   void context::global_transform()
   {


      //// Get a handle for our "MVP" uniform
      ////GLint MatrixID = glGetUniformLocation(programID, "MVP");

      //if (m_iMatrixUniform >= 0)
      //{

      //   // Compute the MVP matrix from keyboard and mouse input
      //   //computeMatricesFromInputs();
      //   ::glm::mat4 matrixProjection = (::glm::mat4 &)projection_matrix();
      //   ::glm::mat4 matrixView = (::glm::mat4 &)view_matrix();
      //   ::glm::mat4 matrixModel = glm::mat4(1.0);
      //   ::glm::mat4 matrixMVP = matrixProjection * matrixView * matrixModel;

      //   // Send our transformation to the currently bound shader, 
      //   // in the "MVP" uniform
      //   //glUniformMatrix4fv(m_iMatrixUniform, 1, GL_FALSE, &matrixMVP[0][0]);

      //}

      ////glm::mat4 getViewMatrix() {
      ////   return ViewMatrix;
      ////}
      ////glm::mat4 getProjectionMatrix() {
      ////   return ProjectionMatrix;
      ////}


      ////// Initial position : on +Z
      ////glm::vec3 position = glm::vec3(0, 0, 5);
      ////// Initial horizontal angle : toward -Z
      ////float horizontalAngle = 3.14f;
      ////// Initial vertical angle : none
      ////float verticalAngle = 0.0f;
      ////// Initial Field of View
      ////float initialFoV = 45.0f;

      ////float speed = 3.0f; // 3 units / second
      ////float mouseSpeed = 0.005f;



   }


   void context::render()
   {

      ASSERT(is_current_task());

      //      //#ifdef __APPLE__
      //
      //            //glBindVertexArrayAPPLE(m_VAO);
      //
      //      //#else
      //
      //      if (m_gluTextureBitmap1)
      //      {
      //
      //         //glMatrixMode(GL_PROJECTION);
      //         //glOrtho(0, 800, 0, 600, -1, 1);
      //         // glMatrixMode(GL_MODELVIEW);
      //
      //         //clear and draw quad with texture (could be in display callback)
      //         //glClear(GL_COLOR_BUFFER_BIT);
      //         //glBindTexture(GL_TEXTURE_2D, m_gluTextureBitmap1);
      //         ////glEnable(GL_TEXTURE_2D);
      //         //glBegin(GL_QUADS);
      //         //glTexCoord2i(0, 0); glVertex2f(-1.0, -1.0);
      //         //glTexCoord2i(0, 1); glVertex2f(-1.0, 1.0);
      //         //glTexCoord2i(1, 1); glVertex2f(1.0, 1.0);
      //         //glTexCoord2i(1, 0); glVertex2f(1.0, -1.0);
      //         //glEnd();
      //         //glDisable(GL_TEXTURE_2D);
      //         //glBindTexture(GL_TEXTURE_2D, 0);
      //         //glFlush();
      //
      //         /*GLuint readFboId = 0;
      //         glGenFramebuffers(1, &readFboId);
      //         glBindFramebuffer(GL_READ_FRAMEBUFFER, readFboId);
      //         glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
      //            GL_TEXTURE_2D, m_gluTextureBitmap1, 0);
      //         glBlitFramebuffer(0, 0, m_sizeBitmap1.cx(), m_sizeBitmap1.cy(),
      //            0, 0, m_size.cx(), m_size.cy(),
      //            GL_COLOR_BUFFER_BIT, GL_LINEAR);
      //         glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
      //         glDeleteFramebuffers(1, &readFboId);*/
      //      }
      //      else
      //      {
      //
      ////#ifdef __APPLE__
      ////         glBindVertexArrayAPPLE(m_VAO);
      ////#else
      ////         glBindVertexArray(m_VAO);
      ////#endif
      ////
      ////         glDrawArrays(GL_TRIANGLES, 0, 6);
      //
      //   }
      //
      //      //return ::success;

   }


   void context::set_bitmap_1(::image::image* pimage)
   {

      ASSERT(is_current_task());

      //      if (!m_gluTextureBitmap1)
      //      {
      //
      ////         glGenTextures(1, &m_gluTextureBitmap1); // generate texture id for your texture (can skip this line)
      ////         glEnable(GL_TEXTURE_2D);
      ////         glBindTexture(GL_TEXTURE_2D, m_gluTextureBitmap1);
      ////
      ////         glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // set alignment of data in memory (a good thing to do before glTexImage)
      ////
      ////#if defined(__APPLE__) || defined(__ANDROID__)
      ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // set clamp (GL_CLAMP_TO_EDGE would be better)
      ////#else
      ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); // set clamp (GL_CLAMP_TO_EDGE would be better)
      ////#endif
      ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      ////         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // set linear filtering (so you can scale your image)
      //
      //      }
      //
      //      if (pimage->is_ok())
      //      {
      //
      //         pimage->map();
      //
      //         //glBindTexture(GL_TEXTURE_2D, m_gluTextureBitmap1);
      //
      //         //if (m_iLastBitmap1Scan != pimage->m_iScan)
      //         //{
      //
      //         //   m_iLastBitmap1Scan = pimage->m_iScan;
      //
      //         //   glPixelStorei(GL_UNPACK_ROW_LENGTH, pimage->m_iScan / 4);
      //
      //         //}
      //
      //         m_sizeBitmap1 = pimage->size();
      //
      //
      //         m_memorySwap.set_size(m_sizeBitmap1.area() * 4);
      //
      //         //vertical_swap_copy_image32_swap_red_blue(
      //            ((image32_t *)m_memorySwap.data())->vertical_swap_copy_swap_red_blue(
      //            m_sizeBitmap1.cx(),
      //            m_sizeBitmap1.cy(),
      //            m_sizeBitmap1.cx() * 4,
      //            pimage->get_data(),
      //            pimage->m_iScan);
      //
      //         //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
      //         //   m_sizeBitmap1.cx(),
      //         //   m_sizeBitmap1.cy(),
      //         //   0, GL_RGBA, GL_UNSIGNED_BYTE,
      //         //   m_memorySwap.data()); // upload image data to the textur
      //
      //
      //
      //
      //
      //      }

   }


   //void context::swap_buffers()
   //{

   //   ::cast < gpu_directx11::renderer > pgpurenderer = m_pgpurenderer;

   //   ::cast < swap_chain_render_target_view > pswapchainrendertargetview = pgpurenderer->m_prendertargetview;

   //   pswapchainrendertargetview->m_pdxgiswapchain->Present(1, 0);

   //}


   //VkSampler context::_001VkSampler()
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





   //void context::create_offscreen_buffer(const ::int_size& size)
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


   //void context::_create_offscreen_buffer(const ::int_size& size)
   //{

   //   return ::success_none;

   //}


   //void context::resize_offscreen_buffer(const ::int_size& size)
   //{

   //   return ::success_none;

   //}



   //void context::destroy_offscreen_buffer()
   //{

   //   return ::success_none;

   //}


   string context::_001GetIntroProjection()
   {

      string strVersion = get_shader_version_text();


      string strProjection =
         "layout(location = 0) in vec3 aPos;\n"
         "out vec3 ourPosition;\n"
         "\n"
         "void main()\n"
         "{\n"
         "   gl_Position = vec4(aPos, 1.0);\n"
         "   ourPosition = aPos;\n"
         "}\n";

      return strProjection;
   }


   string context::_001GetIntroFragment()
   {

      string strVersion = get_shader_version_text();

      string strFragment =
         "uniform vec2 resolution;\n"
         "uniform float time;\n"
         "uniform vec2 mouse;\n"
         "uniform sampler2D backbuffer;\n"
         "\n"
         "void main(void) {\n"
         "float base_res = min(resolution.x(), resolution.y());\n"
         "vec2 uv = (gl_FragCoord.xy * 2.0 - resolution.xy) / base_res;\n"
         "\n"
         //"gl_FragColor = vec4(uv, (uv.x() * uv.x()) / 2.0, ((uv.x() + (base_res - uv.y())) *(uv.x() + (base_res - uv.y()))) / 2.0);\n"
         "float posx = max(0.f, uv.x());\n"
         "float posy = max(0.f, uv.y());\n"
         "gl_FragColor = vec4(uv, (posx * posx) / 4.0, ((posx + posy) * (posx + posy)) / 4.0);\n"
         "}\n";

      return strFragment;

   }

   //   string context::load_fragment(const ::string & pszPath, enum_shader & eshader)
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


      //string context::get_shader_version_text()
      //{

      //   return "version 330 core";

      //}

      //void context::translate_shader(string& strFragment)
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


      //void context::_translate_shader(string_array& stra)
      //{

      //}


   void context::set_matrix_uniform(const ::gpu::payload& uniformMatrix)
   {

      //m_iMatrixUniform = uniformMatrix.m_iUniform;

   }

   ::pointer < ::gpu::context > allocate_system_context(::particle* pparticle)
   {

      return pparticle->__create_new <context>();

   }


   //context::context()
   //{



   //}


   //context::~context()
   //{

   //}


   bool context::is_mesa()
   {

      return m_bMesa;

   }


   void context::_create_offscreen_window(const ::int_size& size)
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


   void context::_create_context_directx11(::gpu::device* pgpudeviceParam, const ::gpu::enum_output& eoutput, ::windowing::window* pwindow, const ::int_size& size)
   {

      ::cast < device > pgpudevice = pgpudeviceParam;

      if (!pgpudevice)
      {

         throw ::exception(error_failed);

      }

      auto& pdevicecontext = pgpudevice->m_pdevicecontext;

      ::defer_throw_hresult(pdevicecontext.as(m_pcontext));

      ::defer_throw_hresult(pdevicecontext.as(m_pcontext1));

   }


   void context::on_create_context(::gpu::device* pgpudevice, const ::gpu::enum_output& eoutput, ::windowing::window* pwindow, const ::int_size& size)
   {

      m_pgpudevice = pgpudevice;

      _create_context_directx11(pgpudevice, eoutput, pwindow, size);

   }


   void context::__bind_draw2d_compositor(::gpu::compositor* pgpucompositor)
   {

      ::cast < ::dxgi_surface_bindable > pdxgisurfacebindable = pgpucompositor;

      ::cast < texture > ptexture = get_gpu_renderer()->m_pgpurendertarget->current_texture();

      auto iFrameIndex = m_pgpurenderer->m_pgpurendertarget->get_frame_index();

      auto pdxgisurface = ptexture->__get_dxgi_surface();

      pdxgisurfacebindable->_bind(iFrameIndex, pdxgisurface);

   }


   void context::_create_window_context(::windowing::window* pwindowParam)
   {

      //m_itaskGpu = ::current_itask();

      //   ::cast < ::windowing_win32::window > pwindow = pwindowParam;

      //   if (!m_hdc || !m_hrc)
      //   {

      //      auto psystem = system();

      //      auto pgpu = application()->get_gpu();

      //      ::pointer < ::directx11::directx11 > pdirectx11 = pgpu;

      //      //if (!pdirectx11->m_atomClass)
      //      //{

      //      //   informationf("MS GDI - RegisterClass failed");

      //      //   informationf("last-error code: %d\n", GetLastError());

      //      //   throw ::exception(error_failed);

      //      //}

      //      if (!m_hwnd)
      //      {

      //         auto hwnd = pwindow->m_hwnd;


      //         m_hwnd = hwnd;


      //         //// create WGL context, make current

      //         //PIXELFORMATDESCRIPTOR pixformat;

      //         //int chosenformat;

      //         HDC hdc = GetDC(m_hwnd);

      //         //if (!hdc)
      //         //{

      //         //   informationf("MS GDI - GetDC failed");

      //         //   informationf("last-error code: %d\n", GetLastError());

      //         //   throw ::exception(error_failed);

      //         //}

      //         //zero(pixformat);
      //         //pixformat.nSize = sizeof(pixformat);
      //         //pixformat.nVersion = 1;
      //         //pixformat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_DIRECTX11 | PFD_DOUBLEBUFFER;
      //         //pixformat.iPixelType = PFD_TYPE_RGBA;
      //         //pixformat.cColorBits = 32;
      //         //pixformat.cRedShift = 16;
      //         //pixformat.cGreenShift = 8;
      //         //pixformat.cBlueShift = 0;
      //         //pixformat.cAlphaShift = 24;
      //         //pixformat.cAlphaBits = 8;
      //         //pixformat.cDepthBits = 24;
      //         //pixformat.cStencilBits = 8;

      //         //chosenformat = ChoosePixelFormat(hdc, &pixformat);

      //         //if (chosenformat == 0)
      //         //{

      //         //   informationf("MS GDI - ChoosePixelFormat failed");

      //         //   informationf("last-error code: %d\n", GetLastError());

      //         //   ReleaseDC(m_hwnd, hdc);

      //         //   throw ::exception(error_failed);

      //         //}

      //         //bool spfok = SetPixelFormat(hdc, chosenformat, &pixformat);

      //         //if (!spfok)
      //         //{

      //         //   informationf("MS GDI - SetPixelFormat failed");

      //         //   informationf("last-error code: %d\n", GetLastError());

      //         //   ReleaseDC(m_hwnd, hdc);

      //         //   throw ::exception(error_failed);

      //         //}

      ////         auto hglrc = wglCreateContext(hdc);

      ////         pwindow->m_hglrcProto = hglrc;

      //////         int context_attribs[] = {
      //////WGL_CONTEXT_MAJOR_VERSION_ARB, 2,
      //////WGL_CONTEXT_MINOR_VERSION_ARB, 1,
      //////0, 0
      //////         };
      //////         auto hglrc = wglCreateContextAttribsARB(hdc, NULL, context_attribs);
      //////         if (!hglrc) {
      //////            //ReleaseDC(hWnd, hDC);
      //////            //DestroyWindow(hWnd);

      //////            throw ::exception(error_failed);
      //////         }
      //////         //ReleaseDC(hWnd, hDC);

      ////         if (!pwindow->m_hglrcProto)
      ////         {

      ////            informationf("MS WGL - wglCreateContext failed");

      ////            informationf("last-error code: %d\n", GetLastError());

      ////            ReleaseDC(m_hwnd, hdc);

      ////            throw ::exception(error_failed);

      ////         }

      ////         bool bMakeCurrentOk = wglMakeCurrent(hdc, pwindow->m_hglrcProto);

      ////         if (!bMakeCurrentOk)
      ////         {

      ////            informationf("MS WGL - wglMakeCurrent failed");

      ////            informationf("last-error code: %d\n", GetLastError());

      ////            ReleaseDC(m_hwnd, hdc);

      ////            throw ::exception(error_failed);

      ////         }


      ////         pdirectx11->defer_init_gpu_library();

      //         //auto pszVersion = (const char *)glGetString(GL_VERSION);
      //         ////::e_status estatus = 

      //         //::string strVersion(pszVersion);

      //         //if (strVersion.case_insensitive_contains("mesa"))
      //         //{

      //         //   m_bMesa = true;

      //         //}

      //         ////if (!estatus)
      //         ////{

      //         ////   ReleaseDC(window, hdc);

      //         ////   return estatus;

      //         ////}

      //         m_hwnd = m_hwnd;
      //         m_hdc = hdc;
      //         m_hrc = pwindow->m_hglrcProto;

      //      }

      //   }

      //   RECT rectClient;

      //   ::GetClientRect(m_hwnd, &rectClient);

      //   ::int_size sizeNew = { rectClient.right - rectClient.left,
      //rectClient.bottom - rectClient.top };
      //   
      //   if (m_size != sizeNew)
      //   {
      //      m_size = sizeNew;


      //      //HDC pdcDIB;                      // контекст устройства в памяти
      //      //HBITMAP hbmpDIB;                 // и его текущий битмапvoid *pBitsDIB(NULL);            // содержимое битмапаint cxDIB(200); int cyDIB(300);  // его размеры (например для окна 200х300)
      //      //auto &BIH=pwindow->m_bitmapinfoheaderProto;            // и заголовок// …// создаем DIB section// создаем структуру BITMAPINFOHEADER, описывающую наш DIBint iSize = sizeof(BITMAPINFOHEADER);  // размер
      //      //memset(&BIH, 0, sizeof(pwindow->m_bitmapinfoheaderProto));

      //      //BIH.biSize = sizeof(pwindow->m_bitmapinfoheaderProto);        // размер структуры
      //      //BIH.biWidth = m_size.cx();       // геометрия
      //      //BIH.biHeight = m_size.cy();      // битмапа
      //      //BIH.biPlanes = 1;          // один план
      //      //BIH.biBitCount = 32;       // 24 bits per pixel
      //      //BIH.biCompression = BI_RGB;// без сжатия// создаем новый DC в памяти
      //      ////pdcDIB = CreateCompatibleDC(NULL);
      //      ////void * pBits = nullptr;
      //      //// создаем DIB-секцию
      //      //pwindow->m_hbitmapProto = CreateDIBSection(
      //      //  m_hdc,                  // контекст устройства
      //      //  (BITMAPINFO *)&BIH,       // информация о битмапе
      //      //  DIB_RGB_COLORS,          // параметры цвета
      //      //  &pwindow->m_pbitsProto,               // местоположение буфера (память выделяет система)
      //      //  NULL,                    // не привязываемся к отображаемым в память файлам
      //      //  0);

      //      //// выберем новый битмап (DIB section) для контекста устройства в памяти
      //      //SelectObject(m_hdc, pwindow->m_hbitmapProto);
      //      //pwindow->m_hdcProto = m_hdc;
      //   }

      //   m_itaskGpu = ::current_itask();

      //   m_estatus = ::success;

      //   set_ok_flag();

   }


   void context::copy(::gpu::texture* pgputextureTarget, ::gpu::texture* pgputextureSource)
   {

      ::cast < texture > ptextureDst = pgputextureTarget;

      if (ptextureDst->m_prendertargetview)
      {

         copy_using_shader(pgputextureTarget, pgputextureSource);

      }
      else
      {

         ::cast < texture > ptextureSrc = pgputextureSource;

         m_pcontext->CopyResource(
            ptextureDst->m_ptextureOffscreen,
            ptextureSrc->m_ptextureOffscreen);

      }

   }


   BEGIN_GPU_PROPERTIES(copy_using_shader_input_layout)
      GPU_PROPERTY("position", ::gpu::e_type_seq2)
      GPU_PROPERTY("uv", ::gpu::e_type_seq2)
   END_GPU_PROPERTIES()


   void context::copy_using_shader(::gpu::texture* pgputextureTarget, ::gpu::texture* pgputextureSource)
   {

      if (!m_pshaderCopyUsingShader)
      {

         const char* copy_using_shader_vertex_shader = R"hlsl(
      // FullscreenQuadVS.hlsl
      struct VS_IN {
         float2 pos : POSITION;
         float2 uv : TEXCOORD;
      };

      struct VS_OUT {
         float4 pos : SV_POSITION;
         float2 uv : TEXCOORD;
      };

      VS_OUT main(VS_IN input) {
         VS_OUT output;
         output.pos = float4(input.pos, 0.0, 1.0);
         output.uv = input.uv;
         return output;
      }

)hlsl";



         const char* copy_using_shader_pixel_shader = R"hlsl(
      // CopyTexturePS.hlsl
Texture2D sourceTexture : register(t0);
SamplerState samp : register(s0);

float4 main(float2 uv : TEXCOORD) : SV_TARGET {
    return sourceTexture.Sample(samp, uv);
}

)hlsl";

         __defer_construct_new(m_pshaderCopyUsingShader);

         m_pshaderCopyUsingShader->initialize_shader_with_block(
            m_pgpurenderer,
            ::as_block(copy_using_shader_vertex_shader),
            ::as_block(copy_using_shader_pixel_shader),
            {},
            {},
            {},
            {},
            copy_using_shader_input_layout_properties()

         );


         ::cast < device > pgpudevice = m_pgpudevice;

         struct Vertex {
            float x, y;   // Position
            float u, v;   // Texture coordinates
         };

         Vertex vertices[] = {
            //   x     y     u     v
            { -1.0f, -1.0f, 0.0f, 1.0f }, // bottom-left
            {  1.0f, -1.0f, 1.0f, 1.0f }, // bottom-right
            { -1.0f,  1.0f, 0.0f, 0.0f }, // top-left
            {  1.0f,  1.0f, 1.0f, 0.0f }  // top-right
         };

         m_iVertexBufferSizeCopyUsingShader = sizeof(vertices);
         D3D11_BUFFER_DESC bufferdesc = {};
         bufferdesc.Usage = D3D11_USAGE_DEFAULT;
         bufferdesc.ByteWidth = m_iVertexBufferSizeCopyUsingShader;
         bufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
         bufferdesc.CPUAccessFlags = 0;

         D3D11_SUBRESOURCE_DATA subresourceddata = {};
         subresourceddata.pSysMem = vertices;

         HRESULT hr = pgpudevice->m_pdevice->CreateBuffer(
            &bufferdesc,
            &subresourceddata,
            &m_pd3d11bufferVertexCopyUsingShader);


         ::defer_throw_hresult(hr);

      }


      m_pshaderCopyUsingShader->bind(pgputextureTarget, pgputextureSource);

      ::cast <texture > ptextureDst = pgputextureTarget;
      float clearColor[4] = { 0, 0, 0, 0 }; // Clear to transparent
      m_pcontext->ClearRenderTargetView(ptextureDst->m_prendertargetview, clearColor);

      UINT stride = m_iVertexBufferSizeCopyUsingShader;
      UINT offset = 0;
      ID3D11Buffer* buffera[] = { m_pd3d11bufferVertexCopyUsingShader };

      m_pcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
      m_pcontext->IASetVertexBuffers(0, 1, buffera, &stride, &offset);
      //for (auto player : *playera)
      //{
      //   //            player->
      //   ::cast <texture > ptexture = player->texture();
      //   ID3D11SamplerState* samplerstatea[] =
      //   { ptexture->m_psamplerstate };
      //   ID3D11ShaderResourceView* sharedresourceviewa[] =
      //   { ptexture->m_pshaderresourceview };
      //   m_pcontext->PSSetSamplers(0, 1, samplerstatea);
      //   m_pcontext->PSSetShaderResources(0, 1, sharedresourceviewa);

      //ID3D11RenderTargetView* rendertargetview[] = { ptextureDst->m_prendertargetview };

      //m_p(1, rendertargetview, nullptr);

      //m_pcontext->OMSetBlendState(g_blendState, nullptr, 0xffffffff);
      //g_context->VSSetShader(g_vs, nullptr, 0);
      //g_context->PSSetShader(g_ps, nullptr, 0);
      //g_context->PSSetSamplers(0, 1, &g_sampler);

      m_pcontext->Draw(4, 0); // Fullscreen triangle
      m_pshaderCopyUsingShader->unbind();


      float clearColor2[4] = { 0.95f * 0.5f, 0.75f * 0.5f, 0.95f * 0.5f, 0.5f }; // Clear to transparent
      m_pcontext->ClearRenderTargetView(ptextureDst->m_prendertargetview, clearColor2);


   }



   void context::merge_layers(::gpu::texture* ptextureTarget, ::pointer_array < ::gpu::layer >* playera)
   {

      if (!m_pshaderBlend3)
      {

         const char* full_screen_triangle_vertex_shader = R"hlsl(
// vertex.hlsl
struct VSOut {
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};

VSOut main(uint id : SV_VertexID)
{
    float2 verts[3] = {
        float2(-1, -1),
        float2(-1,  3),
        float2( 3, -1)
    };

    float2 uv[3] = {
        float2(0, 1),
        float2(0, -1),
        float2(2, 1)
    };

    VSOut o;
    o.pos = float4(verts[id], 0, 1);
    o.uv  = uv[id];
    return o;
}
)hlsl";

         const char* full_screen_triangle_fragment_shader = R"hlsl(  
// pixel.hlsl
Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_TARGET
{
    return tex.Sample(samp, uv); // Assumes premultiplied alpha
}
)hlsl";

         __defer_construct_new(m_pshaderBlend3);

         m_pshaderBlend3->initialize_shader_with_block(
            m_pgpurenderer,
            ::as_block(full_screen_triangle_vertex_shader),
            ::as_block(full_screen_triangle_fragment_shader)
            //,
            //{},
            //{},
            //{},
            //{},
            //copy_using_shader_input_layout_properties()
         );

      }


      if (!m_pd3d11blendstateBlend3)
      {

         D3D11_BLEND_DESC blendDesc = { 0 };
         blendDesc.RenderTarget[0].BlendEnable = TRUE;
         blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;              // Premultiplied alpha
         blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;   // Use inverse of alpha
         blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

         blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;         // Alpha blending (optional)
         blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
         blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

         blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

         ::cast < ::gpu_directx11::device > pgpudevice = m_pgpudevice;

         HRESULT hr = pgpudevice->m_pdevice->CreateBlendState(&blendDesc, &m_pd3d11blendstateBlend3);
         ::defer_throw_hresult(hr);

      }


      m_pshaderBlend3->bind(ptextureTarget);

      ::cast <texture > ptextureDst = ptextureTarget;
      //float clearColor[4] = { 0.95f * 0.5f, 0.95f * 0.5f, 0.25f * 0.5f, 0.5f }; // Clear to transparent
      //m_pcontext->ClearRenderTargetView(ptextureDst->m_prendertargetview, clearColor);
      float clearColor[4] = { 0.f, 0.f, 0.f, 0.f }; // Clear to transparent
      m_pcontext->ClearRenderTargetView(ptextureDst->m_prendertargetview, clearColor);


      {
         float blendFactor[4] = { 0, 0, 0, 0 }; // Ignored with this blend mode
         UINT sampleMask = 0xFFFFFFFF;
         m_pcontext->OMSetBlendState(m_pd3d11blendstateBlend3, blendFactor, sampleMask);
      }



      //ID3D11RenderTargetView* rendertargetview[] = { ptextureDst->m_prendertargetview };

      //m_p(1, rendertargetview, nullptr);

      //m_pcontext->OMSetBlendState(g_blendState, nullptr, 0xffffffff);
      //g_context->VSSetShader(g_vs, nullptr, 0);
      //g_context->PSSetShader(g_ps, nullptr, 0);
      //g_context->PSSetSamplers(0, 1, &g_sampler);

      for (auto player : *playera)
      {
         
         ::cast <texture > ptexture = player->texture();

         m_pshaderBlend3->bind_source(ptexture);

         //ID3D11SamplerState* samplerstatea[] =
         //{ ptexture->m_psamplerstate };
         //ID3D11ShaderResourceView* sharedresourceviewa[] =
         //{ ptexture->m_pshaderresourceview };
         D3D11_VIEWPORT vp = {};
         vp.TopLeftX = ptexture->m_rectangleTarget.left();
         vp.TopLeftY = ptexture->m_rectangleTarget.top();
         vp.Width = static_cast<float>(ptexture->m_rectangleTarget.width());
         vp.Height = static_cast<float>(ptexture->m_rectangleTarget.height());
         vp.MinDepth = 0.0f;
         vp.MaxDepth = 1.0f;
         m_pcontext->RSSetViewports(1, &vp);

         //m_pcontext->PSSetSamplers(0, 1, samplerstatea);
         //m_pcontext->PSSetShaderResources(0, 1, sharedresourceviewa);

         m_pcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
         m_pcontext->Draw(3, 0); // Fullscreen triangle

      }
      //}

      m_pshaderBlend3->unbind();

      //::cast <texture > ptextureDst = ptextureTarget;
      //float clearColor2[4] = { 0.95f * 0.5f, 0.75f * 0.5f, 0.95f * 0.5f, 0.5f }; // Clear to transparent
      //m_pcontext->ClearRenderTargetView(ptextureDst->m_prendertargetview, clearColor2);


   }



   void context::_create_cpu_buffer(const ::int_size& size)
   {

      _create_offscreen_window(size);

      //_create_window_buffer();

      //m_itaskGpu = ::current_itask();

   }


   void context::defer_create_window_context(::windowing::window* pwindow)
   {

      //if (m_hrc)
      //{

      //   return;

      //}

      //::directx11::context::defer_create_window_context(pwindow);

   }


   void context::_defer_create_window_context(::windowing::window* pwindow)
   {

      _create_window_context(pwindow);

   }


   void context::resize_cpu_buffer(const ::int_size& sizeParam)
   {

      if (m_papplication->m_gpu.m_bUseSwapChainWindow)
      {

         return;

      }

      auto size(sizeParam);

      send([this, size]()
         {
            //if (!m_pcpubuffer)

            create_cpu_buffer(size);


            ///m_pcpubuffer->m_pixmap.create(m_pcpubuffer->m_memory, size);

      //#ifdef WINDOWS_DESKTOP
      //
      //      ::SetWindowPos(m_hwnd, 0, 0, 0, size.cx(), size.cy(), SWP_NOZORDER | SWP_NOMOVE | SWP_HIDEWINDOW);
      //
      //#else

      //      destroy_offscreen_buffer();
      //
      //      if(!create_offscreen_buffer(size))
      //      {
      //
      //         return error_failed;
      //
      //      }

      //#endif

            //make_current();

            //glViewport(0, 0, size.cx(), size.cy());
            //glMatrixMode(GL_PROJECTION);
            //glLoadIdentity();
            //glOrtho(0, size.cx(), 0, size.cy(), -10, 10);
            //glMatrixMode(GL_MODELVIEW);
            //glutPostRedisplay();

            //return ::success;
         });


   }


   //void context::make_current()
   //{

   //   ::gpu::context::make_current();

   //}


   void context::destroy_cpu_buffer()
   {

      //ASSERT(is_current_task());

      //::e_status estatus = ::success;

      //if (m_hrc == NULL && m_hdc == NULL && m_hwnd == NULL)
      //{

      //   return;

      //}

      ////wglMakeCurrent(nullptr, nullptr);
      ////wglDeleteContext(m_hrc);
      //::ReleaseDC(m_hwnd, m_hdc);
      //::DestroyWindow(m_hwnd);
      //m_size.set(0, 0);
      //m_hrc = NULL;
      //m_hwnd = NULL;
      //m_hdc = NULL;

      ////return estatus;

   }

   //void context::render()
   //{

   //   ::gpu_directx11::context::render();



   //}


   //void context::swap_buffers()
   //{

   //   //SwapBuffers(m_hdc);

   //}


   string context::get_shader_version_text()
   {

      return "#version 330 core";

   }


   void context::_translate_shader(string_array& stra)
   {

      ::gpu::context::_translate_shader(stra);

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




   //// class member functions
   ////context::context(::graphics3d_directx11::DirectX11Device* pgpudevice) : this->logicalDevice(){pgpudevice->logicalDevice} {
   //context::context()
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

   //context::~context()
   //{
   //   vkDestroyCommandPool(this->logicalDevice(), m_vkcommandpool, nullptr);
   //   vkDestroyDevice(this->logicalDevice(), nullptr);

   //   if (enableValidationLayers) {
   //      DestroyDebugUtilsMessengerEXT(m_vkinstance, debugMessenger, nullptr);
   //   }

   //   if (m_vksurfacekhr)
   //   {
   //      vkDestroySurfaceKHR(m_vkinstance, m_vksurfacekhr, nullptr);
   //   }
   //   vkDestroyInstance(m_vkinstance, nullptr);

   //}


   //void context::initialize_context(::user::graphics3d * pimpact)
   //{

   //   m_pimpact = pimpact;


   //   validationLayers.add("VK_LAYER_KHRONOS_validation");

   //   if (m_papplication->m_bUseSwapChainWindow)
   //   {

   //      deviceExtensions.add(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

   //   }


   //}


   //void context::createInstance()
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


   //void context::pickPhysicalDevice()
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


   //void context::createLogicalDevice()
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

   //   if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &this->logicalDevice()) != VK_SUCCESS)
   //   {
   //      throw ::exception(error_failed,"failed to create logical pvkcdevice!");
   //   }
   //   if (indices.graphicsFamilyHasValue)
   //   {
   //      vkGetDeviceQueue(this->logicalDevice(), indices.graphicsFamily, 0, &m_vkqueueGraphics);
   //   }
   //   if (indices.presentFamilyHasValue)
   //   {
   //      vkGetDeviceQueue(this->logicalDevice(), indices.presentFamily, 0, &m_vkqueuePresent);
   //   }
   //}


   //void context::createCommandPool()
   //{
   //   QueueFamilyIndices queueFamilyIndices = findPhysicalQueueFamilies();

   //   VkCommandPoolCreateInfo poolInfo = {};
   //   poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
   //   poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
   //   poolInfo.flags =
   //      VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

   //   if (vkCreateCommandPool(this->logicalDevice(), &poolInfo, nullptr, &m_vkcommandpool) != VK_SUCCESS) {
   //      throw ::exception(error_failed,"failed to create command pool!");
   //   }
   //}

   //void context::createSurface() { window.createWindowSurface(m_vkinstance, &m_vksurfacekhr); }

   //void context::_createSurface() {}


   //bool context::isDeviceSuitable(VkPhysicalDevice pvkcdevice)
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


   //void context::populateDebugMessengerCreateInfo(
   //   VkDebugUtilsMessengerCreateInfoEXT & createInfo)
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


   //void context::setupDebugMessenger()
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


   //bool context::checkValidationLayerSupport()
   //{
   //   uint32_t layerCount;
   //   vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

   //   ::array<VkLayerProperties> availableLayers(layerCount);
   //   vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

   //   for (const char * layerName : validationLayers)
   //   {
   //      bool layerFound = false;


   //      printf_line("checking for validation layer : %s", layerName);
   //      for (const auto & layerProperties : availableLayers)
   //      {
   //         printf_line("an available layer : %s", layerProperties.layerName);
   //         if (strcmp(layerName, layerProperties.layerName) == 0)
   //         {
   //            layerFound = true;
   //            break;
   //         }
   //      }

   //      if (!layerFound) {
   //         return false;
   //      }
   //   }

   //   return true;

   //}


   //::array<const char *> context::getRequiredExtensions()
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


   //void context::hasGflwRequiredInstanceExtensions()
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


   //bool context::checkDeviceExtensionSupport(VkPhysicalDevice pvkcdevice)
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


   //void context::createBuffer(
   //   VkDeviceSize size,
   //   VkBufferUsageFlags usage,
   //   VkMemoryPropertyFlags properties,
   //   VkBuffer & buffer,
   //   VkDeviceMemory & bufferMemory)
   //{

   //   VkBufferCreateInfo bufferInfo{};
   //   bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
   //   bufferInfo.size = size;
   //   bufferInfo.usage = usage;
   //   bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

   //   if (vkCreateBuffer(this->logicalDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
   //   {

   //      throw ::exception(error_failed, "failed to create vertex buffer!");

   //   }

   //   VkMemoryRequirements memRequirements;
   //   vkGetBufferMemoryRequirements(this->logicalDevice(), buffer, &memRequirements);

   //   VkMemoryAllocateInfo allocInfo{};
   //   allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
   //   allocInfo.allocationSize = memRequirements.size;
   //   allocInfo.memoryTypeIndex = m_pgpudevice->m_pphysicaldevice->findMemoryType(memRequirements.memoryTypeBits, properties);

   //   if (vkAllocateMemory(this->logicalDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
   //   {

   //      throw ::exception(error_failed, "failed to allocate vertex buffer memory!");

   //   }

   //   vkBindBufferMemory(this->logicalDevice(), buffer, bufferMemory, 0);

   //}


   //VkCommandBuffer context::beginSingleTimeCommands()
   //{

   //   VkCommandBufferAllocateInfo allocInfo{};
   //   allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
   //   allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
   //   allocInfo.commandPool = m_pgpudevice->getCommandPool();
   //   allocInfo.commandBufferCount = 1;

   //   VkCommandBuffer commandBuffer;
   //   vkAllocateCommandBuffers(this->logicalDevice(), &allocInfo, &commandBuffer);

   //   VkCommandBufferBeginInfo beginInfo{};
   //   beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
   //   beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

   //   vkBeginCommandBuffer(commandBuffer, &beginInfo);
   //   return commandBuffer;

   //}


   //void context::endSingleTimeCommands(VkCommandBuffer commandBuffer)
   //{

   //   vkEndCommandBuffer(commandBuffer);

   //   VkSubmitInfo submitInfo{};
   //   submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
   //   submitInfo.commandBufferCount = 1;
   //   submitInfo.pCommandBuffers = &commandBuffer;

   //   vkQueueSubmit(m_vkqueueGraphics, 1, &submitInfo, VK_NULL_HANDLE);
   //   vkQueueWaitIdle(m_vkqueueGraphics);

   //   vkFreeCommandBuffers(this->logicalDevice(), m_pgpudevice->getCommandPool(), 1, &commandBuffer);

   //}


   //void context::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
   //{
   //   VkCommandBuffer commandBuffer = beginSingleTimeCommands();

   //   VkBufferCopy copyRegion{};
   //   copyRegion.srcOffset = 0;  // Optional
   //   copyRegion.dstOffset = 0;  // Optional
   //   copyRegion.size = size;
   //   vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

   //   endSingleTimeCommands(commandBuffer);

   //}


   //void context::copyBufferToImage(
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


   //void context::createImageWithInfo(
   //   const VkImageCreateInfo & imageInfo,
   //   VkMemoryPropertyFlags properties,
   //   VkImage & image,
   //   VkDeviceMemory & imageMemory)
   //{

   //   if (vkCreateImage(this->logicalDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS)
   //   {

   //      throw ::exception(error_failed, "failed to create image!");

   //   }

   //   VkMemoryRequirements memRequirements;
   //   vkGetImageMemoryRequirements(this->logicalDevice(), image, &memRequirements);

   //   VkMemoryAllocateInfo allocInfo{};
   //   allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
   //   allocInfo.allocationSize = memRequirements.size;
   //   allocInfo.memoryTypeIndex = m_pgpudevice->m_pphysicaldevice->findMemoryType(memRequirements.memoryTypeBits, properties);

   //   if (vkAllocateMemory(this->logicalDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
   //      throw ::exception(error_failed, "failed to allocate image memory!");
   //   }

   //   if (vkBindImageMemory(this->logicalDevice(), image, imageMemory, 0) != VK_SUCCESS) {
   //      throw ::exception(error_failed, "failed to bind image memory!");
   //   }
   //}





   //void context::submitWork(VkCommandBuffer cmdBuffer, VkQueue queue)
   //{
   //   VkSubmitInfo submitInfo = initializers::submitInfo();
   //   submitInfo.commandBufferCount = 1;
   //   submitInfo.pCommandBuffers = &cmdBuffer;
   //   //m_submitInfo.commandBufferCount = 1;
   //   //m_submitInfo.pCommandBuffers = &cmdBuffer;
   //   VkFenceCreateInfo fenceInfo = initializers::fenceCreateInfo();
   //   VkFence fence;
   //   VK_CHECK_RESULT(vkCreateFence(this->logicalDevice(), &fenceInfo, nullptr, &fence));
   //   VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));
   //   VK_CHECK_RESULT(vkWaitForFences(this->logicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX));
   //   vkDestroyFence(this->logicalDevice(), fence, nullptr);
   //}


   //void context::submitSamplingWork(VkCommandBuffer cmdBuffer, VkQueue queue)
   //{
   //   VkSubmitInfo submitInfo = initializers::submit_info();
   //   submitInfo.commandBufferCount = 1;
   //   submitInfo.pCommandBuffers = &cmdBuffer;
   //   //m_submitInfo.commandBufferCount = 1;
   //   //m_submitInfo.pCommandBuffers = &cmdBuffer;
   //   VkFenceCreateInfo fenceInfo = initializers::fence_create_info();
   //   VkFence fence;
   //   VK_CHECK_RESULT(vkCreateFence(this->logicalDevice(), &fenceInfo, nullptr, &fence));
   //   VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));
   //   VK_CHECK_RESULT(vkWaitForFences(this->logicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX));
   //   vkDestroyFence(this->logicalDevice(), fence, nullptr);
   //}

   ::gpu_directx11::descriptor_pool* context::get_global_pool(int iFrameCount)
   {

      return m_pdescriptorpoolGlobal;

   }


   void context::create_global_ubo(int iGlobalUboSize, int iFrameCount)
   {

      m_uboBuffers.set_size(iFrameCount);

      D3D11_BUFFER_DESC cbd = {};
      cbd.Usage = D3D11_USAGE_DYNAMIC;
      cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
      cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
      ::cast < device > pgpudevice = m_pgpudevice;

      // GlobalUbo
      cbd.ByteWidth = iGlobalUboSize;
      pgpudevice->m_pdevice->CreateBuffer(&cbd, nullptr, &m_pbufferGlobalUbo);

      //for (int i = 0; i < m_uboBuffers.size(); i++)
      //{

      //   m_uboBuffers[i] = __allocate buffer();

      //   m_uboBuffers[i]->initialize_buffer(
      //      this,
      //      iGlobalUboSize,
      //      1,
      //      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      //      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

      //   m_uboBuffers[i]->map();

      //   auto bufferInfo = m_uboBuffers[i]->descriptorInfo();

      //   descriptor_writer(*m_psetdescriptorlayoutGlobal, *m_pdescriptorpoolGlobal)
      //      .writeBuffer(0, &bufferInfo)
      //      .build(m_descriptorsetsGlobal[i]);

      //}

      //auto globalSetLayout = m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();

   }


   void context::update_global_ubo(const ::block& block)
   {

      auto iFrameIndex = m_pgpurenderer->m_pgpurendertarget->get_frame_index();

      //m_uboBuffers[iFrameIndex]->writeToBuffer(block.data());

      //m_uboBuffers[iFrameIndex]->flush();

      //ID3D11Buffer* globalUBOBuffer = nullptr;
      //D3D11_BUFFER_DESC cbd = {};
      //cbd.Usage = D3D11_USAGE_DYNAMIC;
      //cbd.ByteWidth = block;
      //cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
      //cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

      //device->CreateBuffer(&cbd, nullptr, &globalUBOBuffer);
      if (m_pbufferGlobalUbo)
      {

         D3D11_MAPPED_SUBRESOURCE mapped;

         m_pcontext->Map(m_pbufferGlobalUbo, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

         memcpy(mapped.pData, block.data(), block.size());

         m_pcontext->Unmap(m_pbufferGlobalUbo, 0);

      }

      //m_pbufferGlobalUbo
      //m_pbufferGlobalUbo

   }


   void context::engine_on_frame_context_initialization()
   {

      //// Global UBO descriptors
      //if (!m_psetdescriptorlayoutGlobal)
      //{

      //   auto pgpurenderer = get_renderer(::gpu::e_scene_3d);

      //   m_psetdescriptorlayoutGlobal = set_descriptor_layout::Builder(this)
      //      .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
      //      .build();

      //   auto iFrameCount = pgpurenderer->get_frame_count();

      //   m_descriptorsetsGlobal.resize(iFrameCount);

      //   auto pdescriptorpoolbuilder = __allocate::gpu_directx11::descriptor_pool::Builder();

      //   pdescriptorpoolbuilder->initialize_builder(this);
      //   pdescriptorpoolbuilder->setMaxSets(iFrameCount);
      //   pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, iFrameCount);

      //   m_pdescriptorpoolGlobal = pdescriptorpoolbuilder->build();

      //}



   }


   ID3D11DeviceContext* context::draw_get_d3d11_device_context()
   {

      return m_pcontext;

   }


   ID3D11DeviceContext1* context::draw_get_d3d11_device_context1()
   {

      return m_pcontext1;

   }


   bool context::create_offscreen_graphics_for_swap_chain_blitting(::draw2d_gpu::graphics* pgraphics, const ::int_size& size)
   {

      ::cast < swap_chain > pswapchain = get_swap_chain();

      if (!pswapchain->m_bSwapChainInitialized)
      {

         pswapchain->initialize_gpu_swap_chain(m_pgpurenderer);

      }

      return true;

   }


} // namespace gpu_directx11



