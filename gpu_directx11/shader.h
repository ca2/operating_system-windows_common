// Co-creating with V0idsEmbrace@Twitch with
// camilo on 2025-05-19 04:59 <3ThomasBorregaardSorensen!!
#pragma once



#include "bred/gpu/shader.h"


namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 shader :
      virtual public ::gpu::shader
   {
   public:

      ::comptr <ID3D11VertexShader> m_pvertexshader ;

      ::comptr < ID3D11PixelShader> m_ppixelshader;

      ::comptr <ID3D11InputLayout > m_pd3d11inputlayout;
      
      comptr < ID3D11Buffer> m_pbufferSharedPushConstants;
      comptr<ID3D11Buffer> m_pbufferVertexPushConstants;
      comptr<ID3D11Buffer> m_pbufferFragmentPushConstants;
      int m_iSizeSharedPushConstants = -1;
      int m_iSizeVertexPushConstants = -1;
      int m_iSizeFragmentPushConstants = -1;

      ::comptr < ID3D11DepthStencilState > m_pdepthstencilstate2;
      ::comptr < ID3D11RasterizerState> m_prasterizerstate2;
      //::pointer < pipeline > m_ppipeline;

      //struct d3d11_constant_buffer
      //{
      //   comptr < ID3D11Buffer> m_pd3d11buffer;

      //};

      ///::string_map_base < d3d11_constant_buffer > m_mapD3D11ConstantBuffer;

      //bool m_bDisableDepthTest = false;
      //bool m_bDepthTestButNoDepthWrite = false;
      //bool m_bEnableBlend = false;
      //bool m_bAccumulationEnable = false;
      //int m_iColorAttachmentCount = 1;
      //VkPrimitiveTopology m_vktopology;

      //s::comparable_array<VkDynamicState> m_dynamicstateaEnable;

      shader();
      ~shader();


      virtual ::comptr < ID3DBlob> create_vertex_shader_blob(const ::block& block);
      virtual ::comptr < ID3DBlob> create_pixel_shader_blob(const ::block& block);
      virtual void create_vertex_shader(const ::block& block);
      virtual void create_pixel_shader(const ::block& block);
      //void bind(VkCommandBuffer commandBuffer);

      ///static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
   ///private:
///		static ::array<char> readFile(const ::string& filepath);

      /*void createGraphicsPipeline(
         const ::string& vertFilepath,
         const ::string& fragFilepath,
         const PipelineConfigInfo& configInfo);

      void createShaderModule(const ::block& block, VkShaderModule* shaderModule);*/

      /*
      VkPipeline graphicsPipeline;
      VkShaderModule vertShaderModule;
      VkShaderModule fragShaderModule;*/
      //VkPipelineLayout		m_vkpipelinelayout;


      //void _create_pipeline_layout(int iSize);

      //void draw() override;

      //void initialize_shader(
      //	::gpu::context* pgpucontext,
      //	const ::file::path& pathVert,
      //	const ::file::path& pathFrag,
      //	const ::gpu::property* pproperties) override;

      void on_initialize_shader() override;
      void draw() override;


      //void on_set_constant_buffer(const ::scoped_string& scopedstrName) override;

      void bind(::gpu::command_buffer *pgpucommandbuffer, ::gpu::texture *pgputextureTarget) override;
      //void bind(::gpu::command_buffer *pgpucommandbuffer, ::gpu::texture *pgputextureTarget,
                //::gpu::texture *pgputextureSource) override;
      void bind_source(::gpu::command_buffer *pgpucommandbuffer, ::gpu::texture *pgputextureSource, int iSlot) override;
      //void bind(::gpu::command_buffer *pgpucommandbuffer) override;
      //void _bind(::gpu::command_buffer *pgpucommandbuffer, ::gpu::enum_scene escene) override;
      void unbind(::gpu::command_buffer *pgpucommandbuffer) override;
      
      void on_bind_already_bound(::gpu::command_buffer *pgpucommandbuffer,
                                         ::gpu::texture *pgputextureTarget) override;

      virtual void defer_bind_frame_buffer_layer(::gpu::command_buffer *pgpucommandbuffer,
                                                 ::gpu::texture *pgputextureTarget);
      void push_properties(::gpu::command_buffer * pgpucommandbuffer) override;
      void setModelViewProjection(const floating_matrix4 &model, const floating_matrix4 &view, const floating_matrix4 &projection) override;
      void bind_slot_set(::gpu::command_buffer *pgpucommandbuffer, int iSet,
                         ::gpu::binding_slot_set *pgpubindingslotset) override;

   };


} // namespace gpu_directx11 



