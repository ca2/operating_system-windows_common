#include "framework.h"
#include "gpu_directx11/context.h"
/*
 * Encapsulates a directx11 memory_buffer
 *
 * Initially based off DirectX11Buffer by Sascha Willems -
 * https://github.com/SaschaWillems/DirectX11/blob/master/base/DirectX11Buffer.h
 */

#include "memory_buffer.h"


namespace gpu_directx11
{


   memory_buffer::memory_buffer()
   {


   }


   memory_buffer::~memory_buffer() 
   {

      unmap();

   }


   void memory_buffer::on_initialize_memory_buffer(const void* dataStatic, memsize sizeStatic)
   {

      auto etype = m_etype;

      D3D11_BUFFER_DESC bufferdesc  {};

      if (etype == ::gpu::memory_buffer::e_type_vertex_buffer)
      {

         bufferdesc.ByteWidth = (UINT) total_size_in_bytes();
         bufferdesc.Usage = dataStatic ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
         bufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
         if(!dataStatic)
            bufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

      }
      else if (etype == ::gpu::memory_buffer::e_type_index_buffer)
      {

         bufferdesc.ByteWidth = (UINT) total_size_in_bytes();
         bufferdesc.Usage = dataStatic ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
         bufferdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
         if (!dataStatic)
            bufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

      }
      else if (etype == ::gpu::memory_buffer::e_type_constant_buffer)
      {

         bufferdesc.ByteWidth = (UINT) (total_size_in_bytes()+15)&~15;
         bufferdesc.Usage = D3D11_USAGE_DYNAMIC;
         bufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
         bufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

      }
      else
      {

         throw ::not_implemented();

      }
      
      ::cast < ::gpu_directx11::context > pgpucontext = m_pcontext;

      ::cast < ::gpu_directx11::device > pgpudevice = pgpucontext->m_pgpudevice;

      auto pd3d11device = pgpudevice->m_pdevice;

      D3D11_SUBRESOURCE_DATA data{};

      data.pSysMem = dataStatic;

      auto hresult = pd3d11device->CreateBuffer(
         &bufferdesc, dataStatic? &data: nullptr, &m_pbuffer);

      if (FAILED(hresult))
      {

         throw ::hresult_exception(hresult);

      }

   }

   
   bool memory_buffer::is_initialized() const
   { 

      return m_pbuffer;
   
   }


   void* memory_buffer::_map(memsize start, memsize count)
   {

      ::cast < ::gpu_directx11::context > pcontext = m_pcontext;

      D3D11_MAPPED_SUBRESOURCE mapped;

      pcontext->m_pcontext->Map(
         m_pbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
//      memcpy(mapped.pData, &myData, sizeof(MyConstants));

      m_pMap = mapped.pData;

      return m_pMap;
  
   }


   void memory_buffer::_unmap()
   {
      
      ::cast < ::gpu_directx11::context > pcontext = m_pcontext;
      
      pcontext->m_pcontext->Unmap(m_pbuffer, 0);

   }


   void memory_buffer::bind()
   {

      if (m_etype == e_type_vertex_buffer)
      {

         UINT stride = type_size();

         UINT offset = 0;

         ::cast < ::gpu_directx11::context > pcontext = m_pcontext;

         pcontext->m_pcontext->IASetVertexBuffers(0, 1, m_pbuffer.pp(), &stride, &offset);

      }
      else if (m_etype == e_type_index_buffer)
      {

         ::cast < ::gpu_directx11::context > pcontext = m_pcontext;
         
         pcontext->m_pcontext->IASetIndexBuffer(m_pbuffer, DXGI_FORMAT_R32_UINT, 0);

      }

   }


   void memory_buffer::unbind()
   {


   }


}  // namespace graphics3d_directx11

