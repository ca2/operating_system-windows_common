#include "framework.h"
#include "context.h"
#include "cpu_buffer.h"
#include "device.h"
#include "renderer.h"
#include "offscreen_render_target_view.h"
#include "acme/parallelization/synchronous_lock.h"
#include "aura/graphics/image/image.h"
#include "aura/graphics/image/target.h"
#include "aura/graphics/gpu/context.h"


namespace gpu_directx
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

      auto& memory = m_pimagetarget->m_imagebuffer.m_memory;
      ::cast< context > pgpucontext = m_pgpucontext;
      ::cast< renderer > prenderer = pgpucontext->m_pgpurenderer;
      auto prendertargetview = prenderer->m_prendertargetview;
      ::cast < offscreen_render_target_view > poffscreenrendertargetview = prendertargetview;
      ::cast< device > pgpudevice = pgpucontext->m_pgpudevice;
      ID3D11Device* device = pgpudevice->m_pdevice;
      ID3D11DeviceContext* context = pgpucontext->m_pcontext;
      ID3D11Texture2D* offscreenTexture = poffscreenrendertargetview->m_ptextureOffscreen;
      if (!device || !context || !offscreenTexture)
      {
         throw ::exception(error_wrong_state);
      }


      D3D11_TEXTURE2D_DESC desc;
      offscreenTexture->GetDesc(&desc);

      //m_pimagetarget->m_imagebuffer.set_storing_fla
      //if (outWidth) *outWidth = desc.Width;
      //if (outHeight) *outHeight = desc.Height;

      if (desc.Format != DXGI_FORMAT_R8G8B8A8_UNORM) {
         printf("Unsupported format for readback.\n");
         throw ::exception(error_wrong_state);
      }

      D3D11_TEXTURE2D_DESC stagingDesc = desc;
      stagingDesc.BindFlags = 0;
      stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
      stagingDesc.Usage = D3D11_USAGE_STAGING;
      stagingDesc.MiscFlags = 0;

      comptr<ID3D11Texture2D>stagingTexture;
      if (FAILED(device->CreateTexture2D(&stagingDesc, NULL, &stagingTexture))) {
         printf("Failed to create staging texture.\n");
         throw ::exception(error_wrong_state);
      }

      context->CopyResource((ID3D11Resource*)stagingTexture, (ID3D11Resource*)offscreenTexture);

      D3D11_MAPPED_SUBRESOURCE mapped;
      if (FAILED(context->Map((ID3D11Resource*)stagingTexture, 0, D3D11_MAP_READ, 0, &mapped))) {
         printf("Failed to map staging texture.\n");
         throw ::exception(error_wrong_state);
      }

      int width = desc.Width;
      int height = desc.Height;
      UINT rowPitch = mapped.RowPitch;

      m_pimagetarget->m_pimage->create({ width, height });

      // Allocate RGBA buffer (contiguous memory)
      auto lock = m_pimagetarget->no_padded_lock(::image::e_copy_disposition_none);
         
      if (!lock.data()) {
         context->Unmap((ID3D11Resource*)stagingTexture, 0);
         throw ::exception(error_wrong_state);
      }

      // Copy row by row
      for (UINT y = 0; y < height; ++y) {
         memcpy(lock.data() + y * width, (unsigned char*)mapped.pData + y * rowPitch, width * 4);
      }

      context->Unmap((ID3D11Resource*)stagingTexture, 0);
      //stagingTexture->lpVtbl->Release(stagingTexture);

      //return rgbaData;
   }



} // namespace gpu_directx



