// Created by camilo on 2025-08-16 01:25 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "device.h"
#include "frame_storage.h"
#include "memory_buffer.h"

//
// namespace gpu_directx11
// {
//
//
//    frame_storage::frame_storage()
//    {
//
//
//    }
//
//
//    frame_storage::~frame_storage()
//    {
//
//
//    }
//
//
//    void frame_storage::map_allocate(::gpu::memory_buffer* pgpumemorybuffer, int size)
//    {
//
//       if (size > m_iBufferSize)
//       {
//
//          throw ::exception(error_wrong_state);
//
//       }
//
//       bool bDiscard;
//
//       if (size > m_iBufferSize - m_iBufferOffset)
//       {
//
//          bDiscard = true;
//
//          m_iBuffer++;
//
//       }
//       else
//       {
//
//          bDiscard = false;
//
//       }
//
//       auto& pbuffer = m_buffera.ø(m_iBuffer);
//
//       if (!pbuffer)
//       {
//
//          ::cast < ::gpu_directx11::device > pgpudevice = m_pgpudevice;
//
//          auto pd3d11device = pgpudevice->m_pdevice;
//
//          UINT bufSize = m_iBufferSize; // number of vertices the buffer can hold
//
//          D3D11_BUFFER_DESC bufferdesc = {};
//          bufferdesc.ByteWidth = bufSize; // total buffer size in bytes
//          bufferdesc.Usage = D3D11_USAGE_DYNAMIC;         // allows CPU write
//          bufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//          bufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     // required for Map()
//          bufferdesc.MiscFlags = 0;
//          //         bufferdesc.StructureByteStride = sizeof(Vertex);             // optional, helpful for structured buffers
//
//          auto hresult = pd3d11device->CreateBuffer(
//             &bufferdesc, nullptr, &pbuffer);
//
//          if (FAILED(hresult))
//          {
//
//             throw ::hresult_exception(hresult);
//
//          }
//
//          bDiscard = true;
//
//       }
//
//       //m_iBufferOffset = m_iBufferOffset;
//
//       //m_pmodelbuffer->m_strDebugString.formatf("24*th:%d", m_iBufferOffset / 24);
//
//       //m_pbuffer = pbuffer;
//       //buffer buffer;
//
//       ::cast < memory_buffer > pmemorybuffer = pgpumemorybuffer;
//
//       pmemorybuffer->m_pbuffer = pbuffer;
//       pmemorybuffer->m_iBufferOffset = m_iBufferOffset;
//       pmemorybuffer->m_iSizeMapped = size;
//
//       D3D11_MAPPED_SUBRESOURCE mapped;
//
//       ::cast < context > pcontext = pgpumemorybuffer->m_pcontext;
//
//       pcontext->m_pcontext->Map(
//          pbuffer, 0, bDiscard ? D3D11_MAP_WRITE_DISCARD :
//          D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped);
//       //      memcpy(mapped.pData, &myData, sizeof(MyConstants));
//       pmemorybuffer->m_pMap = ((char*)mapped.pData) + m_iBufferOffset;
//
//       m_iBufferOffset += size;
//
//       //return buffer;
//
//    }
//
//
// } // namespace gpu_directx11
//
//

