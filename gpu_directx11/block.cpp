// Created by camilo on 2025-12-11 16:03 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "binding.h"
#include "block.h"
//#include "command_buffer.h"
#include "context.h"
//#include "render_target.h"
#include "renderer.h"


namespace gpu_directx11
{


   block::block() {}


   block::~block() {}


   void block::create_gpu_block(::gpu::context *pgpucontext)
   {

      ::gpu::block::create_gpu_block(pgpucontext);

      D3D11_BUFFER_DESC cbd = {};
      cbd.Usage = D3D11_USAGE_DYNAMIC;
      cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
      cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
      ::cast<device> pgpudevice = m_pgpucontext->m_pgpudevice;
      int iSize = this->size(false);
      // GlobalUbo
      cbd.ByteWidth = (iSize + 15) & ~15;
      pgpudevice->m_pd3d11device->CreateBuffer(&cbd, nullptr, &m_pbuffer);


   }


   ID3D11Buffer * block::buffer()
   {


      return m_pbuffer;

   }


//    VkDescriptorSet block::descriptor_set(::gpu::binding_set_pointer pgpubindingset,
//                                          ::gpu::command_buffer *pgpucommandbuffer)
//    {
//
//       if (m_vkdescriptorset)
//       {
//
//          return m_vkdescriptorset;
//
//       }
//
// //      VkDescriptorBufferInfo bufferinfo;
//
//       ::cast<::gpu_vulkan::context> pcontext = m_pgpucontext;
//
//       //bufferinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//       //bufferinfo.imageView = get_image_view();
//       //bufferinfo.sampler = pcontext->_001VkSampler();
//
//       unsigned int uSamplerBinding = 0;
//
//       //auto pgpubindingset = pshader->get_first_image_sampler_binding_set();
//
//       ::cast<::gpu_vulkan::binding_set> pbindingset = pgpubindingset.m_pbindingset;
//
//       auto playout = pbindingset->descriptor_set_layout(pgpucommandbuffer);
//
//       auto ppool = pbindingset->m_pdescriptorpool;
//
//       auto iFrameIndex = pgpucommandbuffer->m_pgpurendertarget->get_frame_index();
//
//       auto bufferInfo = m_uboBuffers[iFrameIndex]->descriptorInfo();
//
//       //   descriptor_writer(*globalSetLayout, *m_pglobalpool)
//       //      .writeBuffer(0, &bufferInfo)
//       //      .build(globalDescriptorSets[i]);
//       ::gpu_vulkan::descriptor_writer(*playout, *ppool)
//          .writeBuffer(uSamplerBinding, &bufferInfo)
//          .build(m_vkdescriptorset);
//
//       return m_vkdescriptorset;
//
//    }


} // namespace gpu_directx11


