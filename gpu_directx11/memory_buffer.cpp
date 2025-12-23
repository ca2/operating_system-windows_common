#include "framework.h"
#include "frame.h"
#include "model_buffer.h"
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

      m_iBufferOffset = 0;
      m_iSizeMapped = 0;

   }


   memory_buffer::~memory_buffer()
   {

      unmap();

   }


   void memory_buffer::on_initialize_memory_buffer(const ::block &block)
   {

      auto etype = m_etype;

      D3D11_BUFFER_DESC bufferdesc{};

      if (etype == ::gpu::memory_buffer::e_type_shared_dynamic_vertex_buffer)
      {

         bufferdesc.ByteWidth = (UINT)m_size;
         bufferdesc.Usage = D3D11_USAGE_DYNAMIC;
         bufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
         bufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

      }
      else if (etype == ::gpu::memory_buffer::e_type_vertex_buffer)
      {

         bufferdesc.ByteWidth = (UINT)total_size_in_bytes();
         bufferdesc.Usage = block.data() ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
         bufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
         if (!block.data())
            bufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

      }
      else if (etype == ::gpu::memory_buffer::e_type_index_buffer)
      {

         bufferdesc.ByteWidth = (UINT)total_size_in_bytes();
         bufferdesc.Usage = block.data() ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
         bufferdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
         if (!block.data())
            bufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

      }
      else if (etype == ::gpu::memory_buffer::e_type_constant_buffer)
      {

         bufferdesc.ByteWidth = (UINT)(total_size_in_bytes() + 15) & ~15;
         bufferdesc.Usage = D3D11_USAGE_DYNAMIC;
         bufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
         bufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

      }
      else if (etype == ::gpu::memory_buffer::e_type_none)
      {

         return;

      }
      else
      {

         throw ::not_implemented();

      }

      ::cast < ::gpu_directx11::context > pgpucontext = m_pcontext;

      ::cast < ::gpu_directx11::device > pgpudevice = pgpucontext->m_pgpudevice;

      auto pd3d11device = pgpudevice->m_pdevice;

      D3D11_SUBRESOURCE_DATA data{};

      data.pSysMem = block.data();

      if (block.data() || etype == e_type_shared_dynamic_vertex_buffer)
      {

         if (block.data())
         {
            m_bStatic = true;
         }
         else
            {

   m_bStatic = false;
         }


         auto hresult = pd3d11device->CreateBuffer(
            &bufferdesc, block.data() ? &data : nullptr, &m_pbuffer);

         if (FAILED(hresult))
         {

            throw ::hresult_exception(hresult);

         }

         // if (etype == e_type_shared_dynamic_vertex_buffer)
         // {
         //
         //    D3D11_MAPPED_SUBRESOURCE mapped;
         //    HRESULT hresultResourceMap =pgpucontext->m_pcontext->Map(m_pbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
         //
         //    ::defer_throw_hresult(hresultResourceMap);
         //
         //    m_pMap = mapped.pData;
         //
         // }

      }
      else
      {

         m_bStatic = false;

      }

   }


   bool memory_buffer::is_initialized() const
   {

      return m_pbuffer;

   }


   void* memory_buffer::_map(memsize start, memsize count)
   {

      ::cast < ::gpu_directx11::context > pcontext = m_pcontext;

      bool bDiscard = true;

      if (!m_bStatic)
      {

         //::cast < frame > pframe = ::gpu::current_frame();

         ::cast < device > pdevice = pcontext->m_pgpudevice;

         //::cast < frame_storage > pframestorage = pdevice->current_frame_storage();

         auto pframestorage = pdevice->current_frame_storage();

         pframestorage->map_allocate(this, (int) count);

         //if (count > pframestorage->m_iBufferSize)
         //{

         //   throw ::exception(error_wrong_state);

         //}

         //if (count > pframestorage->m_iBufferSize + pframestorage->m_iBufferOffset)
         //{

         //   bDiscard = true;

         //   pframestorage->m_iBuffer++;

         //}
         //else
         //{

         //   bDiscard = false;

         //}

         //auto& pbuffer = pframestorage->m_buffera.ø(pframestorage->m_iBuffer);

         //if (!pbuffer)
         //{

         //   ::cast < ::gpu_directx11::context > pgpucontext = m_pcontext;

         //   ::cast < ::gpu_directx11::device > pgpudevice = pgpucontext->m_pgpudevice;

         //   auto pd3d11device = pgpudevice->m_pdevice;

         //   UINT bufSize = pframestorage->m_iBufferSize; // number of vertices the buffer can hold

         //   D3D11_BUFFER_DESC bufferdesc = {};
         //   bufferdesc.ByteWidth = bufSize; // total buffer size in bytes
         //   bufferdesc.Usage = D3D11_USAGE_DYNAMIC;         // allows CPU write
         //   bufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
         //   bufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     // required for Map()
         //   bufferdesc.MiscFlags = 0;
         //   //         bufferdesc.StructureByteStride = sizeof(Vertex);             // optional, helpful for structured buffers


         //   auto hresult = pd3d11device->CreateBuffer(
         //      &bufferdesc, nullptr, &pbuffer);

         //   if (FAILED(hresult))
         //   {

         //      throw ::hresult_exception(hresult);

         //   }

         //   bDiscard = true;

         //}

         //m_iSizeMapped = count;
         //m_iBufferOffset = pframestorage->m_iBufferOffset;
         //pframestorage->m_iBufferOffset += m_iSizeMapped;

         //m_pmodelbuffer->m_strDebugString.formatf("24*th:%d", m_iBufferOffset / 24);

         //m_pbuffer = pbuffer;

         //m_iBufferOffset = 0;

         D3D11_MAPPED_SUBRESOURCE mapped;

         pcontext->m_pcontext->Map(
            m_pbuffer, 0, m_iBufferOffset == 0 ? D3D11_MAP_WRITE_DISCARD :

            D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped);
         //      memcpy(mapped.pData, &myData, sizeof(MyConstants));
         m_pMap = ((unsigned char*)mapped.pData) + m_iBufferOffset;


      }
      else
      {

         m_iBufferOffset = 0;

         D3D11_MAPPED_SUBRESOURCE mapped;

         pcontext->m_pcontext->Map(
            m_pbuffer, 0, bDiscard ? D3D11_MAP_WRITE_DISCARD :

            D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped);
         //      memcpy(mapped.pData, &myData, sizeof(MyConstants));
         m_pMap = ((char*)mapped.pData) + m_iBufferOffset;

      }

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

         UINT stride = (UINT) type_size();

         UINT offset = m_iBufferOffset;

         ::cast < ::gpu_directx11::context > pcontext = m_pcontext;

         pcontext->m_pcontext->IASetVertexBuffers(0, 1, m_pbuffer.pp(), &stride, &offset);

      }
      else if (m_etype == e_type_index_buffer)
      {

         ::cast < ::gpu_directx11::context > pcontext = m_pcontext;

         pcontext->m_pcontext->IASetIndexBuffer(m_pbuffer, DXGI_FORMAT_R32_UINT, 0);

      }

   }


   void* memory_buffer::map(memsize start, memsize count)
   {

      if (!count)
      {

         throw ::exception(error_wrong_state);

      }

      if (!m_pMap)
      {

         _map(start, count);

         //vkMapMemory(
         //   m_pcontext->logicalDevice(),
         //   m_vkdevicememory,
         //   start,
         //   count > 0 ? count : (m_size + count + 1),
         //   0,
         //   &m_pMap);

      }

      return m_pMap;

   }


   void memory_buffer::unmap()
   {

      if (!m_pMap)
      {

         return;

      }

      _unmap();

      m_pMap = nullptr;

   }



   void memory_buffer::unbind()
   {


   }


   void memory_buffer::on_set_memory_buffer(const ::block &block)
   {

      gpu::memory_buffer::on_set_memory_buffer(block);

   }

   void memory_buffer::_complete_map_allocate(::gpu::memory_buffer* pmemorybufferSource, ::gpu::frame_storage* pgpuframestorage, int size)
   {

      gpu::memory_buffer::_complete_map_allocate(pmemorybufferSource, pgpuframestorage, size);

      ::cast < memory_buffer > pbufferSource = pmemorybufferSource;

      m_pbuffer = pbufferSource->m_pbuffer;

      m_bDynamic = true;

      m_pMap = ((unsigned char *) pbufferSource->m_pMap) + m_iBufferOffset;

   }


}  // namespace graphics3d_directx11

