#pragma once

#include "command_buffer.h"
#include "context.h"
#include "bred/gpu/render_target.h"
#include "bred/gpu/renderer.h"

namespace gpu_directx11
{
   // Used within the caller's GPU context lock; must end before submission.
   class CLASS_DECL_GPU_DIRECTX11 viewport_scissor_restore :
      virtual public ::gpu::viewport_scissor_restore
   {
   public:
      ::pointer<::gpu_directx11::command_buffer> m_pcommandbuffer;
      ::comptr<ID3D11DeviceContext> m_pdevicecontext;
      D3D11_VIEWPORT m_viewporta[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]{};
      D3D11_RECT m_scissora[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE]{};
      UINT m_uViewportCount = 0;
      UINT m_uScissorCount = 0;
      ID3D11RasterizerState * m_prasterizerstate = nullptr;

      viewport_scissor_restore();
      viewport_scissor_restore(const viewport_scissor_restore &) = delete;
      viewport_scissor_restore & operator=(const viewport_scissor_restore &) = delete;

      void initialize(::gpu::command_buffer * commands) override;
      ~viewport_scissor_restore() override;
   };
}
