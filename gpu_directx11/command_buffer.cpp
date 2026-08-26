// Created by camilo on 2026-08-24 02:05 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
// Created by camilo on 2025-06-23 00:16 <3ThomasBorregaardSørensen!!
#include "platform.h"
#include "approach.h"
#include "command_buffer.h"
//#include "depth_stencil.h"
#include "descriptors.h"
#include "fence.h"
#include "frame.h"
//#include "queue.h"
#include "renderer.h"
#include "semaphore.h"
#include "texture.h"
#include "offscreen_render_target_view.h"
#include "physical_device.h"
#include "swap_chain.h"
#include "initializers.h"
#include "bred/gpu/buffer.h"
#include "bred/gpu/layer.h"
//#include "bred/gpu/render_state.h"
#include "gpu_directx11/shader.h"
#include "acme/parallelization/synchronous_lock.h"
#include "acme/platform/application.h"
#include "aura/graphics/image/aaa_target.h"
#include "aura/user/user/interaction.h"
#include "aura/windowing/window.h"
#include "bred/gpu/context_lock.h"


using namespace directx11;



namespace gpu_directx11
{


   command_buffer::command_buffer()
   {

      //m_uFence = 0;

      //m_hFenceEvent = nullptr;

   }


   command_buffer::~command_buffer()
   {

      //if (m_hFenceEvent)
      //{

      //   ::CloseHandle(m_hFenceEvent);

      //   m_hFenceEvent = nullptr;

      //}

   }


   void command_buffer::initialize_command_buffer(::gpu::render_target * pgpurendertarget, ::gpu::queue * pqueue, ::gpu::enum_command_buffer ecommandbuffer)
   {

      ::gpu::command_buffer::initialize_command_buffer(pgpurendertarget, pqueue, ecommandbuffer);

      ::cast < renderer > prenderer = pgpurendertarget->m_pgpurenderer;

      ::cast < context > pcontext = prenderer->m_pgpucontext;

      if (pcontext->m_pd3d11devicecontextDeferred)
      {

         m_pd3d11devicecontextCommandBufferDeferred = pcontext->m_pd3d11devicecontextDeferred;

         m_pd3d11devicecontextCommandBufferDeferred.as(m_pd3d11devicecontext1CommandBufferDeferred);

      }
      else
      {

         information("context isn't deferred");

      }



      //::cast < device > pdevice = pcontext->m_pgpudevice;


      //if (::is_null(pqueue))
      //{

      //   throw ::exception(error_wrong_state);

      //}

      //m_prenderer = prenderer;

      //m_pgpuqueue = pqueue;

      //::cast < ::gpu_directx12::queue > pgpuqueue = pqueue;

      //auto ecommandlisttype = pgpuqueue->m_ecommandlisttype;

      // D3D12_COMMAND_LIST_TYPE_DIRECT
      // D3D12_COMMAND_LIST_TYPE_DIRECT

      ///::cast<gpu_directx12::device> pdevice = prenderer->m_pgpucontext->m_pgpudevice;

      //HRESULT hr = pdevice->m_pd3d12device->CreateCommandAllocator(
      //   ecommandlisttype,  // Type: DIRECT for graphics
      //   __interface_of(m_pcommandallocator)
      //);

      //::defer_throw_hresult(hr);

      ////auto& pcommandlist = m_framea.element_at_grow(iFrame);
      //// 4. Create command list (can be reused)
      //::defer_throw_hresult(pdevice->m_pd3d12device->CreateCommandList(
      //   0,
      //   ecommandlisttype,
      //   m_pcommandallocator, // initial allocator
      //   nullptr, // No PSO yet
      //   __interface_of(m_pcommandlist)
      //));

      //::defer_throw_hresult(m_pcommandlist->Close()); // Must be closed before Reset()


      //m_pgpufence = pdevice->create_gpu_fence();

      // 5. Create fence + event for GPU sync
      //m_hFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

      //m_uFence = 1;

     /* if (ecommandbuffer == ::gpu::e_command_buffer_graphics)
      {

         _initialize_command_buffer(
            pdevice->_main_d3d12_command_queue(),
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            prenderer);

      }
      else if (ecommandbuffer == ::gpu::e_command_buffer_copy)
      {

         _initialize_command_buffer(
            pdevice->_copy_d3d12_command_queue(),
            D3D12_COMMAND_LIST_TYPE_COPY,
            prenderer);

      }*/

   }


   //void command_buffer::_initialize_command_buffer(ID3D12CommandQueue * pcommandqueue, D3D12_COMMAND_LIST_TYPE ecommandlisttype, ::gpu_directx12::renderer* prenderer)
   //{

   //   if (::is_null(pcommandqueue))
   //   {

   //      throw ::exception(error_wrong_state);

   //   }

   //   m_prenderer = prenderer;

   //   m_pcommandqueue = pcommandqueue;

   //   m_ecommandlisttype = ecommandlisttype;

   //   // D3D12_COMMAND_LIST_TYPE_DIRECT
   //   // D3D12_COMMAND_LIST_TYPE_DIRECT

   //   ::cast<gpu_directx12::device> pdevice = prenderer->m_pgpucontext->m_pgpudevice;

   //   HRESULT hr = pdevice->m_pd3d12device->CreateCommandAllocator(
   //      m_ecommandlisttype,  // Type: DIRECT for graphics
   //      __interface_of(m_pcommandallocator)
   //   );

   //   ::defer_throw_hresult(hr);

   //   //auto& pcommandlist = m_framea.element_at_grow(iFrame);
   //   // 4. Create command list (can be reused)
   //   ::defer_throw_hresult(pdevice->m_pd3d12device->CreateCommandList(
   //      0,
   //      m_ecommandlisttype,
   //      m_pcommandallocator, // initial allocator
   //      nullptr, // No PSO yet
   //      __interface_of(m_pcommandlist)
   //   ));

   //   ::defer_throw_hresult(m_pcommandlist->Close()); // Must be closed before Reset()

   //   HRESULT hrCreateFeence =
   //      pdevice->m_pd3d12device->CreateFence(m_uFence, D3D12_FENCE_FLAG_NONE,
   //         __interface_of(m_pfence));

   //   ::defer_throw_hresult(hrCreateFeence);

   //   // 5. Create fence + event for GPU sync
   //   m_hFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

   //   ///m_uFence = 1;

   //}


   void command_buffer::begin_command_buffer(bool bOneTime)
   {

      //reset();

      if (m_estate == e_state_recording)
      {
         
         throw ::exception(error_wrong_state);

      }

      //m_bRecording = true;

      m_estate = ::gpu::command_buffer::e_state_recording;

   }


   //void command_buffer::_copy_buffer(d3d12_resource * pd3d12resourceTargetBuffer,
   //                                  d3d12_resource * pd3d12resourceSourceBuffer, memsize size)
   //{
   //   pd3d12resourceTargetBuffer->_set_state(this, D3D12_RESOURCE_STATE_COPY_DEST);
   //   if (!pd3d12resourceSourceBuffer->m_bUpload)
   //   {
   //      pd3d12resourceSourceBuffer->_set_state(this, D3D12_RESOURCE_STATE_COPY_SOURCE);
   //   }


   //   m_pcommandlist->CopyBufferRegion(pd3d12resourceTargetBuffer->m_presource, 0, pd3d12resourceSourceBuffer->m_presource, 0,
   //                                    size);



   //}


   //void command_buffer::_copy_resource(texture * ptextureTarget, texture * ptextureSource)
   //{



   //   m_pcommandlist->CopyResource(ptextureTarget->m_pd3d12resourceTexture->m_presource, ptextureSource->m_pd3d12resourceTexture->m_presource);


   //}


   void command_buffer::clear_rectangle(::gpu::texture * pgputexture, const ::i32_rectangle & rectangle, const ::color::color & color)
   {

      float clearColor[4] = {
         color.f32_blue() * color.f32_opacity(),
         color.f32_green() * color.f32_opacity(),
         color.f32_red() * color.f32_opacity(),
         color.f32_opacity() }; // Clear to transparent

      D3D11_RECT r2[1];
      r2[0].left = rectangle.left;
      r2[0].top = rectangle.top;
      r2[0].right = rectangle.right;
      r2[0].bottom = rectangle.bottom;

      ID3D11DeviceContext1 * pd3d11devicecontext1 = nullptr;

      if (m_pd3d11devicecontext1CommandBufferDeferred)
      {

         pd3d11devicecontext1 = m_pd3d11devicecontext1CommandBufferDeferred;

      }
      else
      {

         ::cast < ::gpu_directx11::context > pgpucontext = m_pgpurendertarget->m_pgpurenderer->m_pgpucontext->m_pgpudevice->main_gpu_context();

         pd3d11devicecontext1 = pgpucontext->m_pd3d11devicecontext1;

      }

      ::cast < ::gpu_directx11::texture > ptexture = pgputexture;

      auto prendertargetview = ptexture->m_prendertargetview;

      pd3d11devicecontext1->ClearView(prendertargetview, clearColor, r2, 1);

      ////pcommandlist->ClearRenderTargetView(ptextureDst->current_layer().m_handleRenderTargetView, clearColor, 0, r2);


      //::cast < texture > ptexture = pgputexture;

      //D3D12_RECT r2[1];

      //r2[0].left = rectangle.left;
      //r2[0].top = rectangle.top;
      //r2[0].right = rectangle.right;
      //r2[0].bottom = rectangle.bottom;

      ////pcommandlist->ClearRenderTargetView(ptextureDst->current_layer().m_handleRenderTargetView, clearColor, 0, r2);

      //m_pcommandlist->ClearRenderTargetView(ptexture->current_layer().m_handleRenderTargetView, clearColor, 1, r2);

   }


   void command_buffer::clear(::gpu::texture * pgputexture, const ::color::color & color)
   {

      float clearColor[4] = {
         color.f32_blue() * color.f32_opacity(),
         color.f32_green() * color.f32_opacity(),
         color.f32_red() * color.f32_opacity(),
         color.f32_opacity() }; // Clear to transparent

      ::cast < ::gpu_directx11::texture > ptexture = pgputexture;
      //::cast < ::gpu_directx11::texture > ptexture = pgputexturesite->gpu_texture();
      //D3D11_RECT rect = {};
      //rect.left = 100;
      //rect.top = 100;
      //rect.right = 200;
      //rect.bottom = 200;

      //::f32 clearColor[4] = { 0.5f * 0.5f,0.75f * 0.5f, 0.95f * 0.5f, 0.5f };

      //::f32 clearColor[4] = { 0.1f ,0.1f, 0.1f, 0.1f };

      auto prendertargetview = ptexture->m_prendertargetview;
      //::cast < texture > ptexture = pgputexture;

      //::cast < ::gpu_directx11::context > pgpucontext = m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;

      //pgpucontext->m_pd3

      ID3D11DeviceContext * pd3d11devicecontext = nullptr;

      if (m_pd3d11devicecontextCommandBufferDeferred)
      {

         pd3d11devicecontext = m_pd3d11devicecontextCommandBufferDeferred;

      }
      else
      {

         ::cast < ::gpu_directx11::context > pgpucontext = m_pgpurendertarget->m_pgpurenderer->m_pgpucontext->m_pgpudevice->main_gpu_context();

         pd3d11devicecontext = pgpucontext->m_pd3d11devicecontext;

      }

      pd3d11devicecontext->ClearRenderTargetView(prendertargetview, clearColor);

      ////pcommandlist->ClearRenderTargetView(ptextureDst->current_layer().m_handleRenderTargetView, clearColor, 0, r2);

      //m_pcommandlist->ClearRenderTargetView(ptexture->current_layer().m_handleRenderTargetView, clearColor, 0, nullptr);

   }


   void command_buffer::submit_command_buffer(::gpu::layer * pgpulayer)
   {


      if (m_estate != e_state_recording)
      {

         throw ::exception(error_wrong_state);

      }

      m_pd3d11commandlist.release();

      if (m_pd3d11devicecontextCommandBufferDeferred)
      {

         HRESULT hrFinishCommandList = m_pd3d11devicecontextCommandBufferDeferred->FinishCommandList(
               FALSE,
               &m_pd3d11commandlist);


         auto pgpudevice = m_pgpurendertarget->m_pgpurenderer->m_pgpucontext->m_pgpudevice;

         auto pcontext = pgpudevice->main_gpu_context();

         auto pd3d11commandlist = m_pd3d11commandlist;

         {
            
            ::cast < ::gpu_directx11::context > pgpucontext = pcontext;
         
            ::gpu::context_lock contextlock(pgpucontext);

            pgpucontext->m_pd3d11devicecontextImmediate->ExecuteCommandList(
                              pd3d11commandlist.m_p, FALSE);

         }

      }


      //::cast < ::gpu_directx12::device > pdevice = m_prenderer->m_pgpucontext->m_pgpudevice;

      ////informationf("Going to close Command List : 0x%016llx", m_pcommandlist.m_p);

      //HRESULT hrCloseCommandList = m_pcommandlist->Close();

      //pdevice->defer_throw_hresult(hrCloseCommandList);

      //for (auto & pgpusemaphore : m_semaphoreaWait)
      //{

      //   //::cast<::gpu_directx12::semaphore> psemaphore = pgpusemaphore;

      //   pgpusemaphore->wait(m_pgpuqueue);

      //   //psemaphore->wait(m_pcommandqueue);

      //}

      //m_pgpuqueue->execute_command_buffer(this);

      //for (auto & pgpusemaphore : m_semaphoreaSignal)
      //{

      //   //::cast<::gpu_directx12::semaphore> psemaphore = pgpusemaphore;

      //   pgpusemaphore->signal(m_pgpuqueue);

      //}

      ////::cast<::gpu_directx12::fence> pfence = m_pgpufence;

      //if (m_pgpufence)
      //{

      //   m_pgpufence->signal_gpu_fence(m_pgpuqueue);

      //}

      //::cast<::gpu_directx12::fence> pfence = m_pgpufence;

      //if (pfence)
      //{

      //   pfence->signal_gpu_dence(m_pgpuqueue);

      //}

      m_estate = ::gpu::command_buffer::e_state_submitted;

   }


   void command_buffer::wait_commands_to_execute()
   {

      //::cast < ::gpu_directx12::device > pdevice = m_prenderer->m_pgpucontext->m_pgpudevice;

      //UINT64 uploadFenceValue = ++m_uFence;

      if (m_pgpufence)
      {

         m_pgpufence->m_uFence++;

         m_pgpufence->signal_gpu_fence(m_pgpuqueue);

      }

      //auto hrSignalCommandQueue = m_pcommandqueue->Signal(m_pfence, m_uFence);

      //pdevice->defer_throw_hresult(hrSignalCommandQueue);

      wait_for_gpu();

   }


   void command_buffer::wait_for_gpu()
   {

      //const UINT64 fenceValue = ++m_uFence;

      //prenderer->m_pcommandqueue->Signal(m_pfence, m_uFence);

      if (m_pgpufence)
      {

         m_pgpufence->wait_gpu_fence();

      }

      //if (m_pgpufence->GetCompletedValue() < m_uFence)
      //{

      //   ::ResetEvent(m_hFenceEvent);

      //   m_pfence->SetEventOnCompletion(m_uFence, m_hFenceEvent);

      //   ::WaitForSingleObject(m_hFenceEvent, INFINITE);

      //}

      //m_iCurrentFrame2 = (m_iCurrentFrame2 + 1) % get_frame_count();


   }


   void command_buffer::reset()
   {

      //auto pcommandallocator = m_pcommandallocator;

      //HRESULT hrResetCommandAllocator = pcommandallocator->Reset();

      //::defer_throw_hresult(hrResetCommandAllocator);

      //m_pcommandlist->Reset(pcommandallocator, nullptr);

   }


   bool command_buffer::has_finished()
   {

      return true;
      //return m_pgpufence->has_finished();

   }



} // namespace gpu_directx11



