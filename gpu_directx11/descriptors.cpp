#include "framework.h"
#include "descriptors.h"



namespace gpu_directx11
{


   // *************** Descriptor Set Layout Builder *********************

   //descriptor_set_layout::Builder & descriptor_set_layout::Builder::addBinding(
   //    uint32_t binding,
   //    VkDescriptorType descriptorType,
   //    VkShaderStageFlags stageFlags,
   //    uint32_t count) {
   //   assert(bindings.count(binding) == 0 && "Binding already in use");
   //   VkDescriptorSetLayoutBinding layoutBinding{};
   //   layoutBinding.binding = binding;
   //   layoutBinding.descriptorType = descriptorType;
   //   layoutBinding.descriptorCount = count;
   //   layoutBinding.stageFlags = stageFlags;
   //   bindings[binding] = layoutBinding;
   //   return *this;
   //}

   //::pointer<descriptor_set_layout> descriptor_set_layout::Builder::build() const
   //{
   //   auto pvkcdevice = this->m_pgpucontext.m_p;
   //   __refdbg_this(pvkcdevice);
   //   return øallocate descriptor_set_layout(m_pgpucontext, bindings);
   //}

   //// *************** Descriptor Set Layout *********************

   //descriptor_set_layout::descriptor_set_layout(
   //    context * pvkcdevice, ::map_base<uint32_t, VkDescriptorSetLayoutBinding> bindings)
   //   : m_pgpucontext{ pvkcdevice }, bindings{ bindings } {
   //   ::array<VkDescriptorSetLayoutBinding> setLayoutBindings{};
   //   for (auto kv : bindings) {
   //      setLayoutBindings.add(kv.m_element2);
   //   }

   //   VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
   //   descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
   //   descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
   //   descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();
   //   ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;
   //   if (vkCreateDescriptorSetLayout(
   //      m_pgpucontext->logicalDevice(),
   //      &descriptorSetLayoutInfo,
   //      nullptr,
   //      &descriptorSetLayout) != VK_SUCCESS) {
   //      throw ::exception(error_failed, "failed to create descriptor set layout!");
   //   }
   //}

   descriptor_set_layout::~descriptor_set_layout() {
      //if (descriptorSetLayout != VK_NULL_HANDLE) {
      //   ::cast < device > pgpudevice = m_pgpucontext->m_pgpudevice;
      //   vkDestroyDescriptorSetLayout(m_pgpucontext->logicalDevice(), descriptorSetLayout, nullptr);
      //   descriptorSetLayout = VK_NULL_HANDLE;
      //}
   }

   // *************** Descriptor Pool Builder *********************

   //descriptor_pool::Builder & descriptor_pool::Builder::addPoolSize(
   //    VkDescriptorType descriptorType, uint32_t count) {
   //   poolSizes.add({ descriptorType, count });
   //   return *this;
   //}

   //descriptor_pool::Builder & descriptor_pool::Builder::setPoolFlags(
   //    VkDescriptorPoolCreateFlags flags) {
   //   poolFlags = flags;
   //   return *this;
   //}
   //descriptor_pool::Builder & descriptor_pool::Builder::setMaxSets(uint32_t count) {
   //   maxSets = count;
   //   return *this;
   //}

   //::pointer <descriptor_pool> descriptor_pool::Builder::build() const {
   //   return øallocate descriptor_pool(m_pgpucontext, maxSets, poolFlags, poolSizes);
   //}

   // *************** Descriptor Pool *********************
   descriptor_pool::descriptor_pool()
   {

   }
   //descriptor_pool::descriptor_pool(
   //    context * pvkcdevice,
   //    uint32_t maxSets,
   //    VkDescriptorPoolCreateFlags poolFlags,
   //    const ::array<VkDescriptorPoolSize> & poolSizes)
   //   : m_pgpucontext{ pvkcdevice } {
   //   VkDescriptorPoolCreateInfo descriptorPoolInfo{};
   //   descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
   //   descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
   //   descriptorPoolInfo.pPoolSizes = poolSizes.data();
   //   descriptorPoolInfo.maxSets = maxSets;
   //   descriptorPoolInfo.flags = poolFlags;

   //   if (vkCreateDescriptorPool(m_pgpucontext->logicalDevice(), &descriptorPoolInfo, nullptr, &descriptorPool) !=
   //       VK_SUCCESS) {
   //      throw ::exception(error_failed, "failed to create descriptor pool!");
   //   }
   //}

   descriptor_pool::~descriptor_pool() {
   }

   //bool descriptor_pool::allocateDescriptor(
   //    const aaaVkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet & descriptor) const {
   //   VkDescriptorSetAllocateInfo allocInfo{};
   //   allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
   //   allocInfo.descriptorPool = descriptorPool;
   //   allocInfo.pSetLayouts = &descriptorSetLayout;
   //   allocInfo.descriptorSetCount = 1;

   //   // Might want to create a "DescriptorPoolManager" class that handles this case, and builds
   //   // a new pool whenever an old pool fills up. But this is beyond our current scope
   //   if (vkAllocateDescriptorSets(m_pgpucontext->logicalDevice(), &allocInfo, &descriptor) != VK_SUCCESS) {
   //      return false;
   //   }
   //   return true;
   //}

   //void descriptor_pool::freeDescriptors(::array<VkDescriptorSet> & descriptors) const {
   //   aaavkFreeDescriptorSets(
   //       m_pgpucontext->logicalDevice(),
   //       descriptorPool,
   //       static_cast<uint32_t>(descriptors.size()),
   //       descriptors.data());
   //}

   void descriptor_pool::resetPool() {
   //   aaavkResetDescriptorPool(m_pgpucontext->logicalDevice(), descriptorPool, 0);
   }

   // *************** Descriptor Writer *********************

   descriptor_writer::descriptor_writer(descriptor_set_layout & setLayout, descriptor_pool & pool)
      : setLayout{ setLayout }, pool{ pool } {
   }

   //descriptor_writer & descriptor_writer::writeBuffer(
   //    uint32_t binding, VkDescriptorBufferInfo * bufferInfo) {
   //   assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

   //   auto & bindingDescription = setLayout.bindings[binding];

   //   assert(
   //       bindingDescription.descriptorCount == 1 &&
   //       "Binding single descriptor info, but binding expects multiple");

   //   VkWriteDescriptorSet write{};
   //   write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
   //   write.descriptorType = bindingDescription.descriptorType;
   //   write.dstBinding = binding;
   //   write.pBufferInfo = bufferInfo;
   //   write.descriptorCount = 1;

   //   writes.add(write);
   //   return *this;
   //}

   //descriptor_writer & descriptor_writer::writeImage(
   //    uint32_t binding, VkDescriptorImageInfo * imageInfo) {
   //   assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

   //   auto & bindingDescription = setLayout.bindings[binding];

   //   assert(
   //       bindingDescription.descriptorCount == 1 &&
   //       "Binding single descriptor info, but binding expects multiple");

   //   VkWriteDescriptorSet write{};
   //   write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
   //   write.descriptorType = bindingDescription.descriptorType;
   //   write.dstBinding = binding;
   //   write.pImageInfo = imageInfo;
   //   write.descriptorCount = 1;

   //   writes.add(write);
   //   return *this;
   //}

   //bool descriptor_writer::build(VkDescriptorSet & set) {
   //   bool success = pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set);
   //   if (!success) {
   //      return false;
   //   }
   //   overwrite(set);
   //   return true;
   //}

   //void descriptor_writer::overwrite(VkDescriptorSet & set) {
   //   for (auto & write : writes) {
   //      write.dstSet = set;
   //   }
   //   vkUpdateDescriptorSets(pool.m_pgpucontext->logicalDevice(), writes.size(), writes.data(), 0, nullptr);
   //}


} // namespace gpu_directx11



