// Created by camilo on 2026-09-07 01:50 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#include "platform.h"
#include "command_buffer.h"
#include "context.h"
#include "viewport_scissor_restore.h"
#include "bred/gpu/render_target.h"
#include "bred/gpu/renderer.h"


namespace gpu_directx11
{

   viewport_scissor_restore::viewport_scissor_restore()
   {


   }


   viewport_scissor_restore::~viewport_scissor_restore()
   {

      if (!m_pdevicecontext)
      {

         return;

      }


      if (m_pcommandbuffer && m_pcommandbuffer->m_estate == ::gpu::command_buffer::e_state_recording)
      {

      // Restore viewports only while the original command buffer is recording.

      m_pdevicecontext->RSSetViewports(
         m_uViewportCount,
         m_viewporta);


      // Restore scissor rectangles.

      m_pdevicecontext->RSSetScissorRects(
         m_uScissorCount,
         m_scissora);


      // Restore rasterizer state, including ScissorEnable.

      m_pdevicecontext->RSSetState(
         m_prasterizerstate);

      }

      if (m_prasterizerstate)
      {

         m_prasterizerstate->Release();

         m_prasterizerstate = nullptr;

      }

   }


   void viewport_scissor_restore::initialize(::gpu::command_buffer * pgpucommandbuffer)
   {

      ::cast < ::gpu_directx11::command_buffer > pcommandbuffer = pgpucommandbuffer;

      if (!pcommandbuffer || !pcommandbuffer->m_pgpurendertarget ||
          !pcommandbuffer->m_pgpurendertarget->m_pgpurenderer)
         throw ::exception(error_bad_argument, "Viewport/scissor restore requires a DirectX 11 command buffer");
      if (m_pcommandbuffer || pcommandbuffer->m_estate != ::gpu::command_buffer::e_state_recording)
         throw ::exception(error_wrong_state, "Viewport/scissor restore requires a fresh guard and recording buffer");

      ::cast < ::gpu_directx11::context > pcontext = pcommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;

      if (!pcontext)
         throw ::exception(error_wrong_state, "DirectX 11 command buffer has no context");

      ID3D11DeviceContext * pdevicecontext = pcommandbuffer->m_pd3d11devicecontextCommandBufferDeferred
         ? pcommandbuffer->m_pd3d11devicecontextCommandBufferDeferred.m_p : pcontext->m_pd3d11devicecontext.m_p;
      if (!pdevicecontext)
         throw ::exception(error_wrong_state, "DirectX 11 native context is unavailable");

      m_pcommandbuffer = pcommandbuffer;

      m_pdevicecontext = pdevicecontext;
      // Save current viewports.

      m_uViewportCount =
         D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;

      m_pdevicecontext->RSGetViewports(
         &m_uViewportCount,
         m_viewporta);


      // Save current scissor rectangles.

      m_uScissorCount =
         D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;

      m_pdevicecontext->RSGetScissorRects(
         &m_uScissorCount,
         m_scissora);


      // Save rasterizer state.
      //
      // This is necessary because ScissorEnable belongs to
      // D3D11_RASTERIZER_DESC.

      m_pdevicecontext->RSGetState(
         &m_prasterizerstate);

   }


} // namespace gpu_directx11




