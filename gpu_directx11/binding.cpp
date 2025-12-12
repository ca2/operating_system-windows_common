// Created by camilo on 2025-12-11 08:28 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "binding.h"
#include "command_buffer.h"
#include "descriptors.h"
#include "render_target.h"
#include "renderer.h"


namespace gpu_vulkan
{


   ::gpu_vulkan::descriptor_set_layout *binding_set::descriptor_set_layout(::gpu::command_buffer *pgpucommandbuffer)
   {

      if (!m_pdescriptorsetlayout1)
      {

         defer_update_binding_set(pgpucommandbuffer);

      }

      return m_pdescriptorsetlayout1;

   }


   void binding_set::defer_update_binding_set(::gpu::command_buffer * pgpucommandbuffer)
   {

      
      if (!m_pdescriptorsetlayout1)
      {

         defer_update_binding_set(pgpucommandbuffer);

      }

   }


   void binding_set::create_descriptor_set_layout(::gpu::command_buffer *pgpucommandbuffer)
   {

      auto pcontext = pgpucommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;

      auto builder = ::gpu_vulkan::descriptor_set_layout::Builder(pcontext);

      for (auto &pbinding: *this)
      {

         auto flags = (VkShaderStageFlags)0;

         if (!pbinding->m_bVertexShader && !pbinding->m_bFragmentShader)
         {
            if (pbinding->is_image_sampler())
            {
               pbinding->m_bFragmentShader = true;
            }
            else
            {
               pbinding->m_bVertexShader = true;
               pbinding->m_bFragmentShader = true;
            }
         }

         if (pbinding->m_bVertexShader)
         {

            flags |= VK_SHADER_STAGE_VERTEX_BIT;
         }

         if (pbinding->m_bFragmentShader)
         {

            flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
         }

         VkDescriptorType type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

         if (pbinding->is_image_sampler())
         {

            type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
         }

         builder.addBinding(pbinding->m_iSlot, type, flags);
      }

      auto pdescriptorsetlayout = builder.build();

      //pdescriptorsetlayout->m_iIndex = pbindingset.m_iSet;

      m_pdescriptorsetlayout1 = pdescriptorsetlayout;

      auto pdescriptorpoolbuilder = øallocate::gpu_vulkan::descriptor_pool::Builder();

      int iFrameCount = pgpucommandbuffer->m_pgpurendertarget->get_frame_count();

      pdescriptorpoolbuilder->initialize_builder(pcontext);
      pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, iFrameCount * 10);
      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

      auto pdescriptorpool = pdescriptorpoolbuilder->build();

      //pdescriptorpool->m_iIndex = pbindingset.m_iSet;


      //m_descriptorpoola.ø(pdescriptorpool->m_iIndex) = pdescriptorpool;

      m_pdescriptorpool = pdescriptorpool;

   }


   void binding_set::defer_create_update_descriptor_set(::gpu::command_buffer * pgpucommandbuffer,
                                                        ::array_base<VkDescriptorSet> &descriptorseta,
                                                        ::array_base<VkDescriptorBufferInfo> &bufferinfoa)
   {

      for (int i = 0; i < bufferinfoa.size(); i++)
      {

         descriptor_writer(*m_pdescriptorsetlayout1, *m_pdescriptorpool)
            .writeBuffer(0, bufferinfoa[i])
            .build(descriptorseta.ø(i));
      }

   }


   void binding_set::defer_create_update_descriptor_set(::gpu::command_buffer * pgpucommandbuffer,
                                                     ::array_base<VkDescriptorSet> & descriptorseta)

   {



      }


//      //m_psetdescriptorlayout =
//      //
//      //      .addBinding(m_bindingUbo.m_uBinding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
//      //                  VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
//      //      .addBinding(m_bindingSampler.m_uBinding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
//      //                  VK_SHADER_STAGE_FRAGMENT_BIT)
//      //      .build();

//      int iFrameCount = m_pgpurenderer->m_pgpurendertarget->get_frame_count();

//      auto pdescriptorpoolbuilder = øallocate::gpu_vulkan::descriptor_pool::Builder();

//      pdescriptorpoolbuilder->initialize_builder(m_pgpurenderer->m_pgpucontext);
//      pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
//      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, iFrameCount * 10);
//      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

//      m_pdescriptorpool = pdescriptorpoolbuilder->build();
//   }
//   else
//   {

//      m_psetdescriptorlayout =
//         ::gpu_vulkan::descriptor_set_layout::Builder(m_pgpurenderer->m_pgpucontext)
//            .addBinding(m_bindingSampler.m_uBinding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
//                        VK_SHADER_STAGE_FRAGMENT_BIT)
//            .build();

//      int iFrameCount = m_pgpurenderer->m_pgpurendertarget->get_frame_count();

//      auto pdescriptorpoolbuilder = øallocate::gpu_vulkan::descriptor_pool::Builder();

//      pdescriptorpoolbuilder->initialize_builder(m_pgpurenderer->m_pgpucontext);
//      pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
//      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

//      m_pdescriptorpool = pdescriptorpoolbuilder->build();
//   }
//}
// else if (m_bindingSampler.is_set())
//{

//   {

//      m_psetdescriptorlayout =
//         ::gpu_vulkan::descriptor_set_layout::Builder(m_pgpurenderer->m_pgpucontext)
//            .addBinding(m_bindingSampler.m_uBinding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
//                        VK_SHADER_STAGE_FRAGMENT_BIT)
//            .build();

//      int iFrameCount = m_pgpurenderer->m_pgpurendertarget->get_frame_count();

//      auto pdescriptorpoolbuilder = øallocate::gpu_vulkan::descriptor_pool::Builder();

//      pdescriptorpoolbuilder->initialize_builder(m_pgpurenderer->m_pgpucontext);
//      pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
//      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

//      m_pdescriptorpool = pdescriptorpoolbuilder->build();
//   }
//}
// else if (m_bindingCubeSampler.is_set())
//{

//   {

//      m_psetdescriptorlayout =
//         ::gpu_vulkan::descriptor_set_layout::Builder(m_pgpurenderer->m_pgpucontext)
//            .addBinding(m_bindingCubeSampler.m_uBinding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
//                        VK_SHADER_STAGE_FRAGMENT_BIT)
//            .build();

//      int iFrameCount = m_pgpurenderer->m_pgpurendertarget->get_frame_count();

//      auto pdescriptorpoolbuilder = øallocate::gpu_vulkan::descriptor_pool::Builder();

//      pdescriptorpoolbuilder->initialize_builder(m_pgpurenderer->m_pgpucontext);
//      pdescriptorpoolbuilder->setMaxSets(iFrameCount * 10);
//      pdescriptorpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, iFrameCount * 10);

//      m_pdescriptorpool = pdescriptorpoolbuilder->build();
//   }
//}

//_create_pipeline_layout((int)m_propertiesPush.m_memory.size());

// ::cast < context > pgpucontext = m_pgpurenderer->m_pgpucontext;
//
// ::cast < device > pgpudevice = pgpucontext->m_pgpudevice;


// for (int i = 0; i < this->size(); i++)
//{


//
//      .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
//      .build();
//}


} // namespace gpu_vulkan
