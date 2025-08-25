#pragma once


#include "context.h"
#include "acme/prototype/collection/map.h"


namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 descriptor_set_layout :
      virtual public ::particle
   {
   public:
      //class CLASS_DECL_GPU_DIRECTX11 Builder
      //{
      //public:
      //   Builder(::gpu::context * pgpucontext) : m_pgpucontext{ pgpucontext } {}

      //   Builder & addBinding(
      //       uint32_t binding,
      //       VkDescriptorType descriptorType,
      //       VkShaderStageFlags stageFlags,
      //       uint32_t count = 1);
      //   ::pointer<descriptor_set_layout> build() const;

      //private:
      //   ::pointer < context > m_pgpucontext;
      //   ::map_base < unsigned int, VkDescriptorSetLayoutBinding> bindings{};
      //};

      //descriptor_set_layout(
        //  context * pvkcdevice, ::map_base<unsigned int, VkDescriptorSetLayoutBinding> bindings);
      descriptor_set_layout();
      ~descriptor_set_layout();

      //VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

      ::pointer < context > m_pgpucontext;
      //VkDescriptorSetLayout descriptorSetLayout;
      //::map_base<unsigned int, VkDescriptorSetLayoutBinding> bindings;

   };


   class CLASS_DECL_GPU_DIRECTX11 descriptor_pool :
      virtual public ::particle
   {
   public:


      //class CLASS_DECL_GPU_DIRECTX11 Builder :
      //   virtual public ::particle
      //{
      //public:

      //   //Builder(context& m_pgpucontext) : m_pgpucontext{ m_pgpucontext } {}
      //   Builder() {}
      //   void initialize_builder(::gpu::context * pgpucontext)
      //   {
      //      m_pgpucontext = pgpucontext;

      //   }
      //   Builder & addPoolSize(VkDescriptorType descriptorType, uint32_t count);
      //   Builder & setPoolFlags(VkDescriptorPoolCreateFlags flags);
      //   Builder & setMaxSets(uint32_t count);
      //   ::pointer<descriptor_pool> build() const;

      //private:
      //   ::pointer < context > m_pgpucontext;
      //   ::array<VkDescriptorPoolSize> poolSizes{};
      //   uint32_t maxSets = 1000;
      //   VkDescriptorPoolCreateFlags poolFlags = 0;
      //};

      //descriptor_pool(
        //  context * pvkcdevice,
      // uint32_t maxSets,
       //   VkDescriptorPoolCreateFlags poolFlags,
      // const ::array<VkDescriptorPoolSize>& poolSizes);
      descriptor_pool();
      ~descriptor_pool();
      //descriptor_pool(const descriptor_pool &) = delete;
      //descriptor_pool & operator=(const descriptor_pool &) = delete;

      //bool allocateDescriptor(
      //    const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet & descriptor) const;

      //void freeDescriptors(::array<VkDescriptorSet> & descriptors) const;

      void resetPool();

      ::pointer < context > m_pgpucontext;
//      VkDescriptorPool descriptorPool;

   };


   class CLASS_DECL_GPU_DIRECTX11 descriptor_writer
   {
   public:
      descriptor_writer(descriptor_set_layout& setLayout, descriptor_pool& pool);

      //descriptor_writer& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
      //descriptor_writer& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

      //bool build(VkDescriptorSet& set);
      //void overwrite(VkDescriptorSet& set);

      descriptor_set_layout& setLayout;
      descriptor_pool& pool;
      //::array<VkWriteDescriptorSet> writes;
   };


} // namespace graphics3d_directx11



