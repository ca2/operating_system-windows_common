//
// Created by camilo on 2026-06-08.
//
#include "platform.h"
#include "fence.h"

#include "context.h"
#include "device.h"


namespace gpu_directx11
{


   fence::fence()
   {

      m_bPending = false;

   }


   fence::~fence()
   {


   }


   void fence::initialize_gpu_fence(::gpu::context * pgpucontext, bool bCreateSignaled)
   {

      ::gpu::fence::initialize_gpu_fence(pgpucontext, bCreateSignaled);

      ::cast < ::gpu_directx11::device > pdevice = m_pgpucontextGpuFence->m_pgpudevice;

      D3D11_QUERY_DESC querydesc = {};

      querydesc.Query = D3D11_QUERY_EVENT;

      ::defer_throw_hresult(pdevice->m_pd3d11device->CreateQuery(&querydesc, &m_pquery));

      if (!bCreateSignaled)
      {

         reset_gpu_fence();

      }

   }


   void fence::reset_gpu_fence()
   {

      ::cast < ::gpu_directx11::device > pdevice = m_pgpucontextGpuFence->m_pgpudevice;

      pdevice->m_pd3d11devicecontextMain->End(m_pquery);

      m_bPending = true;

   }


   void fence::wait_gpu_fence()
   {

      if (!m_bPending)
      {

         return;

      }

      ::cast < ::gpu_directx11::device > pdevice = m_pgpucontextGpuFence->m_pgpudevice;

      while (true)
      {

         auto hresult = pdevice->m_pd3d11devicecontextMain->GetData(m_pquery, nullptr, 0, 0);

         if (hresult == S_OK)
         {

            m_bPending = false;

            return;

         }

         if (hresult != S_FALSE)
         {

            ::defer_throw_hresult(hresult);

         }

         ::Sleep(0);

      }

   }


} // namespace gpu_directx11
