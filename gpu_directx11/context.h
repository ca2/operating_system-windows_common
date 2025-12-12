#pragma once


#include "bred/gpu/hlsl_context.h"
#include "acme/prototype/prototype/memory.h"
#include "gpu_directx11/device.h"
#include "gpu/context.h"


namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 context :
      virtual public ::gpu::hlsl_context, 
      virtual public ::gpu_gpu::context,
      virtual public ::dxgi_device_source
   {
   public:


//      comptr < ID3D11Buffer>              m_pbufferGlobalUbo;
      comptr<ID3D11DeviceContext>         m_pcontext;
      comptr<ID3D11DeviceContext1>        m_pcontext1;

      //comptr < ID3D11RasterizerState> m_prasterizerstate;
      ::comptr < ID3D11RasterizerState > m_prasterizerstateMergeLayers;

      ::comptr<ID3D10Multithread> m_pmultithread;

      //::comptr < ID3D11DepthStencilState > m_pdepthstencilstateDisabled;

      ::comptr < ID3DUserDefinedAnnotation > m_puserdefinedannotation;
      //itask									m_itaskGpu;
      //VkSampler m_vksampler001;

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
      ::pointer < device >                m_pgpudevice;
//      /** @brief Logical device representation (application's view of the device) */
//      VkDevice m_vkdevice;
//      ///** @brief Properties of the physical device including limits that the application can check against */
//      //VkPhysicalDeviceProperties m_physicaldeviceproperties;
//      ///** @brief Features of the physical device that an application can use to check if a feature is supported */
//      //VkPhysicalDeviceFeatures m_physicaldevicefeatures;
//      /** @brief Features that have been enabled for use on the physical device */
//      VkPhysicalDeviceFeatures m_physicaldevicefeaturesCreate;
//      /** @brief Features that have been enabled for use on the physical device */
//      VkPhysicalDeviceFeatures m_physicaldevicefeaturesEnabled;
//      ///** @brief Memory types and heaps of the physical device */
//      //VkPhysicalDeviceMemoryProperties m_physicaldevicememoryproperties;
//      ///** @brief Queue family properties of the physical device */
//      //::array<VkQueueFamilyProperties> m_queuefamilypropertya;
//      /** @brief List of extensions supported by the device */
//      string_array_base m_straSupportedExtensions;
//      ///** @brief Default command pool for the graphics queue family index */
//      VkCommandPool m_vkcommandpool;
//
//      /** @brief Contains queue family indices */
//      directx11::QueueFamilyIndices m_queuefamilyindices;
//      //struct
//      //{
//      //   uint32_t graphics;
//      //   uint32_t compute;
//      //   uint32_t transfer;
//      //} m_queuefamilyindices;
//
//      //graphics3d_directx11::context
//#if defined(NDEBUG)
//      const bool enableValidationLayers = false;
//#else
//      const bool enableValidationLayers = true;
//#endif
//
//      VkInstance m_vkinstance;
//      VkDebugUtilsMessengerEXT debugMessenger;
//      //VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
//      //::pointer < ::user::graphics3d > m_pimpact;
//
//      //VkCommandPool m_vkcommandpool;
//
//      //VkDevice m_vkdevice;
//      //VkSurfaceKHR m_vksurfacekhr;
      //VkQueue m_vkqueueGraphics;
      //VkQueue m_vkqueuePresent;

      //::array<const_char_pointer >validationLayers;
      //::array<const_char_pointer >deviceExtensions;

      ::pointer<::gpu_directx11::descriptor_set_layout>           m_psetdescriptorlayoutGlobal;
      //::array<VkDescriptorSet>                                 m_descriptorsetsGlobal;
      //::pointer_array<::gpu_directx11::buffer>							m_uboBuffers;
      ::pointer <::gpu_directx11::descriptor_pool>                m_pdescriptorpoolGlobal;

      ::pointer <::gpu_directx11::shader>                m_pshaderBlend3;
      ::comptr < ID3D11BlendState >                      m_pd3d11blendstateBlend3;

      ::pointer <::gpu_directx11::shader>                m_pshaderCopyUsingShader;
      ::comptr <ID3D11Buffer >                           m_pd3d11bufferVertexCopyUsingShader;

      int m_iVertexBufferSizeCopyUsingShader;

      context();
      ~context() override;


      //virtual ID3D11DepthStencilState* depth_stencil_state_disabled();


      void _context_lock() override;
      void _context_unlock() override;
      IDXGIDevice* _get_dxgi_device() override;

      string _001GetIntroProjection() override;
      string _001GetIntroFragment() override;

      void draw() override;
      void start_drawing() override;
      void global_transform() override;
      void render() override;

      void set_bitmap_1(::image::image *pimage) override;

      
      void copy(::gpu::texture* pgputextureTarget, ::gpu::texture* pgputextureSource) override;

      virtual void copy_using_shader(::gpu::texture* pgputextureTarget, ::gpu::texture* pgputextureSource);

      void merge_layers(::gpu::texture* ptextureTarget, ::pointer_array < ::gpu::layer >* playera) override;


      void on_start_layer(::gpu::layer* player) override;
      void on_end_layer(::gpu::layer* player) override;


      void start_debug_happening(::gpu::command_buffer * pgpucommandbuffer, const ::scoped_string& scopedstrDebugHappening) override;
      void end_debug_happening(::gpu::command_buffer * pgpucommandbuffer) override;

      //void defer_bind(::gpu::shader* pgpushader) override;
      //void swap_buffers() override;

      //VkSampler _001VkSampler();

      //descriptor_set_layout* get_set_descriptor_layout();
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

      virtual void _translate_shader(string_array_base& stra);*/

      //virtual void make_current();

      ::pointer<::gpu::texture> load_cube_map(const ::scoped_string &scopedstrName, const ::file::path &path,
                                              bool b32) override;


      ::pointer<::gpu::texture> loadCubemap(const ::scoped_string &name, const ::scoped_string &scopedstrFileName,
                                            bool b32);

      // virtual string load_fragment(const ::scoped_string & scopedstrPath, enum_shader_source& eshadersource);

      //virtual string get_shader_version_text();
      
      void set_matrix_uniform(const ::gpu::payload & uniformMatrix) override;


      bool create_offscreen_graphics_for_swap_chain_blitting(::gpu::graphics* pgraphics, const ::int_size& size) override;


      virtual void _create_context_directx11(::gpu::device* pgpudevice, const ::gpu::enum_output& eoutput, ::windowing::window* pwindow, const ::int_size& size);

      void on_create_context(::gpu::device* pgpudevice, const ::gpu::enum_output& eoutput, ::windowing::window* pwindow, const ::int_size& size) override;


      void __bind_draw2d_compositor(::gpu::compositor* pgpucompositor, ::gpu::layer* player) override;

      //VkDevice logicalDevice();

      //virtual HRESULT createLogicalDevice(
      //   VkPhysicalDeviceFeatures enabledFeatures,
      //   ::array<const_char_pointer >enabledExtensions,
      //   void * pNextChain,
      //   bool useSwapChain = true,
      //   VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
      ////virtual uint32_t getQueueFamilyIndex(VkQueueFlags queueFlags) const;
      //virtual VkCommandPool createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
      //virtual bool isExtensionSupported(const ::scoped_string & scopedstrExtension);

      virtual void _create_offscreen_window(const ::int_size & size);

      void defer_create_window_context(::windowing::window * pwindow) override;
      void _defer_create_window_context(::windowing::window * pwindow) override;
      virtual void _create_window_context(::windowing::window * pwindow);

      //virtual void _create_window_buffer();
      void _create_cpu_buffer(const ::int_size & size) override;
      void resize_cpu_buffer(const ::int_size & size) override;
      void destroy_cpu_buffer() override;

      //void make_current() override;

      bool is_mesa() override;

      //void render();

      string get_shader_version_text() override;

      ::memory rectangle_shader_vert() override;
      ::memory rectangle_shader_frag() override;


      void _translate_shader(string_array_base & straFragment) override;

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
      //   VkBuffer & buffer,
      //   VkDeviceMemory & bufferMemory);
      //VkCommandBuffer beginSingleTimeCommands();
      //void endSingleTimeCommands(VkCommandBuffer commandBuffer);
      //void endSingleTimeCommands(VkCommandBuffer commandBuffer, int iSubmitCount, VkSubmitInfo * psubmitinfo);
      //void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
      //void copyBufferToImage(
      //   VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);
      //void createImageWithInfo(
      //   const VkImageCreateInfo & imageInfo,
      //   VkMemoryPropertyFlags properties,
      //   VkImage & image,
      //   VkDeviceMemory & imageMemory);

      //VkPhysicalDeviceProperties properties;


      //void submitWork(VkCommandBuffer cmdBuffer, VkQueue queue);


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
      //bool checkValidationLayerSupport();
      //void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT & createInfo);
      //void hasGflwRequiredInstanceExtensions();
      //bool checkDeviceExtensionSupport(VkPhysicalDevice pvkcdevice);

      //      //descriptor_set_layout* get_set_descriptor_layout(::gpu::context * pgpucontext);
      //virtual VkDescriptorSet getGlobalDescriptorSet(::gpu_directx11::renderer* prenderer);

      ::gpu_directx11::descriptor_pool* get_global_pool(int iFrameCount);

      void layout_push_constants(::gpu::properties &properties, bool bGlobalUbo) override;
      ////descriptor_set_layout* get_set_descriptor_layout(::gpu::context * pgpucontext);
      //virtual VkDescriptorSet getGlobalDescriptorSet(::gpu::context* pgpucontext, ::gpu_directx11::renderer* prenderer);

      //::gpu_directx11::descriptor_pool* get_global_pool(::gpu::context* pgpucontext, int iFrameCount);
      void layout_global_ubo(::gpu::properties *pproperties) override;

      //void create_global_ubo(int iSize, int iFrameCount) override;
      void update_global_ubo1(::gpu::block * pblockGlobalUbo1) override;

      void set_viewport(::gpu::command_buffer *pgpucommandbuffer, const ::int_rectangle &rectangle) override;
      void clear(::gpu::texture * pgputexture, const ::color::color &color) override; 
      void engine_on_frame_context_initialization() override;

      ID3D11DeviceContext* draw_get_d3d11_device_context();
      ID3D11DeviceContext1* draw_get_d3d11_device_context1();
      floating_matrix4 defer_transpose(const floating_matrix4 &m) override;
      floating_matrix4 defer_clip_remap_projection(const floating_matrix4 &m) override;
      floating_matrix4 defer_remap_impact_matrix(const floating_matrix4 &m) override;
      floating_sequence3 front(const ::graphics3d::floating_rotation &rotation) override;
   };


} // namespace gpu



