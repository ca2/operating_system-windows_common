// From gpu_directx11::context by
// camilo on 2025-05-27 04:54 <3ThomasBorregaardSorensen!!
#pragma once


#include "bred/gpu/device.h"
#include "acme/prototype/prototype/memory.h"
#include "acme_windows_common/dxgi_device_source.h"
//#include "directx11/directx11.h"


namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 device :
      virtual public ::gpu::device
   {
   public:


      comptr<ID3D11Device>                            m_pdevice;
      comptr<ID3D11Device1>                           m_pdevice1;
      comptr<IDXGIDevice>                             m_pdxgidevice;
      comptr<IDXGIFactory2>                           m_pdxgifactory2;
      //comptr<IDXGISwapChain1>                         m_pdxgiswapchain1;
      comptr<ID3D11DeviceContext>                     m_pdevicecontext;
      ::pointer<::windowing::window>                  m_pimpl;


      D3D_FEATURE_LEVEL                               m_featurelevel;



      //directx11();
      //~directx11() override;


      //itask									m_itaskGpu;
      //VkSampler                           m_vksampler001;

      //unsigned int                     m_VAO;
      //unsigned int                     m_VBO;

      //int                            m_iMatrixUniform;

      //int                           m_gluTextureBitmap1;
      //int                              m_iLastBitmap1Scan;
      //::int_size                       m_sizeBitmap1;

      //memory                           m_memorySwap;

      //bool                                m_bOffscreen;
      bool                             m_bMesa;
      //HGLRC                            m_hrc;
      //HDC                              m_hdc;
      //HWND                             m_hwnd;
      //HDC                              m_hdcGraphics;

         /** @brief Physical device representation */
      //VkPhysicalDevice m_physicaldevice;
      ::pointer < physical_device >       m_pphysicaldevice;
      /** @brief Logical device representation (application's view of the device) */
      //VkDevice m_vkdevice;
      ///** @brief Properties of the physical device including limits that the application can check against */
      //VkPhysicalDeviceProperties m_physicaldeviceproperties;
      ///** @brief Features of the physical device that an application can use to check if a feature is supported */
      //VkPhysicalDeviceFeatures m_physicaldevicefeatures;
      /** @brief Features that have been enabled for use on the physical device */
      //VkPhysicalDeviceFeatures m_physicaldevicefeaturesCreate;
      /** @brief Features that have been enabled for use on the physical device */
      //VkPhysicalDeviceFeatures m_physicaldevicefeaturesEnabled;
      ///** @brief Memory types and heaps of the physical device */
      //VkPhysicalDeviceMemoryProperties m_physicaldevicememoryproperties;
      ///** @brief Queue family properties of the physical device */
      //::array<VkQueueFamilyProperties> m_queuefamilypropertya;
      /** @brief List of extensions supported by the device */
      string_array m_straSupportedExtensions;
      ///** @brief Default command pool for the graphics queue family index */
      //VkCommandPool m_vkcommandpool;

      /** @brief Contains queue family indices */
      //directx11::QueueFamilyIndices m_queuefamilyindices;
      //struct
      //{
      //   uint32_t graphics;
      //   uint32_t compute;
      //   uint32_t transfer;
      //} m_queuefamilyindices;





      //graphics3d_directx11::context
#if defined(NDEBUG)
      const bool enableValidationLayers = false;
#else
      const bool enableValidationLayers = true;
#endif

      //VkInstance m_vkinstance;
      //VkDebugUtilsMessengerEXT debugMessenger;
      //VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
      //::pointer < ::user::graphics3d > m_pimpact;

      //VkCommandPool m_vkcommandpool;

      //VkDevice m_vkdevice;
      //VkSurfaceKHR m_vksurfacekhr;
      //VkQueue m_vkqueueGraphics;
      //VkQueue m_vkqueuePresent;

      ::array<const_char_pointer >  validationLayers;
      ::array<const_char_pointer >  deviceExtensions;
      ::procedure_array          m_procedureaOnTopFrameEnd;



      device();
      ~device() override;


      virtual void initialize_cpu_buffer(const ::int_rectangle& rectanglePlacement);

      virtual void initialize_d3d11_device();

      //svirtual void initialize_swap_chain(::windowing::window * pwindow);

      void initialize_gpu_device_for_swap_chain(::gpu::approach* pgpuapproach, ::windowing::window *pwindow) override;
      void initialize_gpu_device_for_off_screen(::gpu::approach* pgpuapproach, const ::int_rectangle& rectanglePlacement) override;

      //string _001GetIntroProjection() override;
      //string _001GetIntroFragment() override;

      //void draw() override;
      //void start_drawing() override;
      //void global_transform() override;
      //void render() override;

      //void set_bitmap_1(::image::image* pimage) override;

      //void swap_buffers() override;

      //VkSampler _001VkSampler();

      //set_descriptor_layout* get_set_descriptor_layout();
      //virtual VkDescriptorSet getCurrentDescriptorSet(::gpu_directx11::renderer* prenderer);

      //::gpu_directx11::descriptor_pool* get_global_pool(int iFrameCount);

      //virtual void create_global_ubo(int iSize, int iFrameCount);
      //virtual void update_global_ubo(const ::block& block);

      //void clear(const ::color::color& color);

      //virtual void create_offscreen_buffer(const ::int_size& size);
      //virtual void _create_offscreen_buffer(const ::int_size& size);

      //virtual void resize_offscreen_buffer(const ::int_size& size);
      //virtual void destroy_offscreen_buffer();

   /*   virtual void translate_shader(string & str);

      virtual void _translate_shader(string_array& stra);*/

      //virtual void make_current();

      // virtual string load_fragment(const ::scoped_string & scopedstrPath, enum_shader_source& eshadersource);

      //virtual string get_shader_version_text();

      //void set_matrix_uniform(const ::gpu::payload& uniformMatrix) override;


      virtual void defer_shader_memory(::memory& memory, const ::file::path& pathShader);



      //virtual void create_device();

      //void on_create_context(const ::gpu::start_context_t& startcontext) override;


  /*    VkDevice logicalDevice() const
      {

         return m_vkdevice;

      }*/

      //virtual HRESULT createLogicalDevice(
      //   VkPhysicalDeviceFeatures enabledFeatures,
      //   ::array<const_char_pointer >enabledExtensions,
      //   void* pNextChain,
      //   bool useSwapChain = true,
      //   VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
      ////virtual uint32_t getQueueFamilyIndex(VkQueueFlags queueFlags) const;
      //virtual VkCommandPool createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
      virtual bool isExtensionSupported(const ::scoped_string& scopedstrExtension);

      virtual void _create_offscreen_window(const ::int_size& size);

      //void defer_create_window_context(::windowing::window* pwindow) override;
      //void _defer_create_window_context(::windowing::window* pwindow) override;
      //virtual void _create_window_context(::windowing::window* pwindow);

      //virtual void _create_window_buffer();
      //void _create_offscreen_buffer(const ::int_size& size) override;
      //void resize_offscreen_buffer(const ::int_size& size) override;
      //void destroy_offscreen_buffer() override;

      //void make_current() override;

      bool is_mesa() override;

      //void render();

      string get_shader_version_text() override;

      void _translate_shader(string_array& straFragment) override;

      //void swap_buffers() override;


      //graphics3d_directx11::context


      //VkCommandPool getCommandPool() { return m_vkcommandpool; }
      //VkDevice logicalDevice() { return m_vkdevice; }

      //VkQueue graphicsQueue() { return m_vkqueueGraphics; }
      //VkQueue presentQueue() { return m_vkqueuePresent; }


      //// Buffer Helper Functions
      //void createBuffer(
      //   VkDeviceSize size,
      //   VkBufferUsageFlags usage,
      //   VkMemoryPropertyFlags properties,
      //   VkBuffer& buffer,
      //   VkDeviceMemory& bufferMemory);
      //void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
      //void copyBufferToImage(
      //   VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);
      //void createImageWithInfo(
      //   const VkImageCreateInfo& imageInfo,
      //   VkMemoryPropertyFlags properties,
      //   VkImage& image,
      //   VkDeviceMemory& imageMemory);


//      void submitWork(VkCommandBuffer cmdBuffer, VkQueue queue);


      //public:
      //void _createInstance();
      //void _setupDebugMessenger();
      //void _createSurface();
      //void _pickPhysicalDevice();
      //void _createLogicalDevice();
      //void _createCommandPool();

      // helper functions
      //bool isDeviceSuitable(VkPhysicalDevice pvkcdevice);
      //::array<const_char_pointer >getRequiredExtensions();
      bool checkValidationLayerSupport();
      //void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
      //void hasGflwRequiredInstanceExtensions();
      //bool checkDeviceExtensionSupport(VkPhysicalDevice pvkcdevice);


      //      //set_descriptor_layout* get_set_descriptor_layout(::gpu::context * pgpucontext);
      //virtual VkDescriptorSet getGlobalDescriptorSet(::gpu::context* pgpucontext, ::gpu_directx11::renderer* prenderer);

      //::gpu_directx11::descriptor_pool* get_global_pool(::gpu::context* pgpucontext, int iFrameCount);

      
      void on_top_end_frame() override;


      ID3D11Device* draw_get_d3d11_device();
      ID3D11Device1* draw_get_d3d11_device1();
      virtual IDXGIDevice* _get_dxgi_device();

      int get_type_size(::gpu::enum_type etype) override;
      void set_mat4(void* p, const ::glm::mat4& mat4) override;


      ::file::path shader_path(const ::file::path& pathShader) override;


      ::gpu::payload load_dds(const ::scoped_string& scopedstrImagePath) override;

      

   };


} // namespace gpu



