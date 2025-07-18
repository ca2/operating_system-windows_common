#include "framework.h"
#include "context.h"
#include "cpu_buffer.h"
#include "device.h"
#include "renderer.h"
#include "offscreen_render_target_view.h"
#include "acme/parallelization/synchronous_lock.h"
#include "aura/graphics/image/image.h"
#include "aura/graphics/image/target.h"
#include "bred/gpu/context.h"


namespace gpu_directx11
{


   cpu_buffer::cpu_buffer()
   {

   }


   cpu_buffer::~cpu_buffer()
   {

   }


//   void cpu_buffer::gpu_read()
//   {
//
//      _synchronous_lock synchronouslock(this->synchronization());
//
//      if (m_pimagetarget->m_pimage.nok())
//      {
//
//         return;
//
//      }
//
//      //m_pixmap.map();
//
//      auto cx = m_pimagetarget->m_pimage->width();
//
//      auto cy = m_pimagetarget->m_pimage->height();
//
//      //auto sizeNeeded = cx * cy * 4;
//
//      //m_pixmap.create(m_memory, sizeNeeded);
//
//      auto data = m_pimagetarget->m_pimage->data();
//
//#if defined(__APPLE__) || defined(__ANDROID__)
//
//      if (data != nullptr)
//      {
//         glReadBuffer(GL_FRONT);
//
//         //if(0)
//         {
//            glReadPixels(
//               0, 0,
//               cx, cy,
//               GL_RGBA,
//               GL_UNSIGNED_BYTE,
//               data);
//
//         }
//
//      }
//
//      //m_pixmap.mult_alpha();
//      information() << "after glReadPixels cx,cy : " << cx << ", " << cy;
//
//      //::memory_set(m_pixmap.m_pimage32Raw, 127, cx * cy * 4);
//
//#elif defined(LINUX) || defined(__BSD__)
//
//      glReadBuffer(GL_FRONT);
//
//
//      glReadPixels(
//         0, 0,
//         cx, cy,
//         GL_BGRA,
//         GL_UNSIGNED_BYTE,
//         m_pixmap.m_pimage32Raw);
//
//      //m_pixmap.mult_alpha();
//
//#else
//
//
//      //glReadBuffer(GL_FRONT);
//      //
//      ////if (m_pgpucontext->is_mesa())
//      //if(!glReadnPixels)
//      //{
//
//      //   glReadPixels(
//      //      0, 0,
//      //      cx, cy,
//      //      GL_BGRA,
//      //      GL_UNSIGNED_BYTE,
//      //      m_pixmap.m_pimage32Raw);
//
//      //}
//      //else
//      //{
//
//      //   glReadnPixels(
//      //      0, 0,
//      //      cx, cy,
//      //      GL_BGRA,
//      //      GL_UNSIGNED_BYTE,
//      //      cx * cy * 4,
//      //      data);
//
//      //}
//
//      //int iError = glGetError();
//
//      //if(iError != 0)
//      //{
//
//      //   printf("glReadnPixels error");
//
//      //}
//
//      //::memory_set(m_pixmap.m_pimage32Raw, 127, cx * cy * 4);
//
//#endif
//
//      {
//
//         auto dst = (unsigned char*)data;
//         auto size = cx * cy;
//
//         while (size > 0)
//         {
//            dst[0] = byte_clip(((int)dst[0] * (int)dst[3]) / 255);
//            dst[1] = byte_clip(((int)dst[1] * (int)dst[3]) / 255);
//            dst[2] = byte_clip(((int)dst[2] * (int)dst[3]) / 255);
//            dst += 4;
//            size--;
//         }
//
//      }
//
//      //::copy_image32(m_pixmap.m_pimage32,
//      //   cx, cy,
//      //   m_pixmap.m_iScan,
//      //   (const ::image32_t*) data, cx * 4);
//
//   }
//

   void cpu_buffer::gpu_write()
   {

      _synchronous_lock synchronouslock(this->synchronization());

      if (m_pimagetarget->m_pimage.nok())
      {

         return;

      }

      // //     m_pixmap.map();
      //      //
      ////      glDrawPixels(
      ////         m_pixmap.m_size.cx(), m_pixmap.m_size.cy(),
      ////         GL_BGRA,
      ////         GL_UNSIGNED_BYTE,
      ////         m_pixmap.m_pimage32Raw);
      //      
      //      glTexImage2D(GL_TEXTURE_2D, 0, 0, 0,
      //                   m_pixmap.m_size.cx(), m_pixmap.m_size.cy(),
      //                   GL_RGBA, GL_UNSIGNED_BYTE,
      //                   m_pixmap.m_pimage32Raw);

   }


   // Utility function to read RGBA pixel data from a GPU texture
   void cpu_buffer::gpu_read()
   {



   }



} // namespace gpu_directx11



