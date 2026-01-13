#include "framework.h"
// Co-creating with V0idsEmbrace@Twitch with
// camilo on 2025-05-19 04:59 <3ThomasBorregaardSorensen!!
#include <d3dcompiler.h>
#include "acme/operating_system/windows_common/com/hresult_exception.h"
#include "approach.h"
#include "bred/gpu/binding.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/context_lock.h"
#include "bred/gpu/frame.h"
#include "bred/gpu/types.h"
#include "context.h"
#include "descriptors.h"
#include "input_layout.h"
#include "memory_buffer.h"
#include "offscreen_render_target_view.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"

#include "block.h"
#include "bred/graphics3d/engine.h"
#include "bred/graphics3d/immersion_layer.h"
#include "bred/graphics3d/scene_base.h"
// #include "bred/user/user/graphics3d.h"


namespace gpu_directx11
{


   shader::shader()
   {

      // m_vktopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
   }


   shader::~shader()
   {

      ::cast<context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      // vkDestroyPipelineLayout(pgpucontext->logicalDevice(), m_vkpipelinelayout, nullptr);
   }


   ::comptr<ID3DBlob> shader::create_vertex_shader_blob(const ::block &block)
   {

      comptr<ID3DBlob> pblobShader;
      comptr<ID3DBlob> pblobError;

      auto data = (const_char_pointer)block.data();

      auto size = block.size();

      HRESULT hr = D3DCompile(data, // pointer to shader source
                              size, // size of shader source
                              nullptr, // optional source name
                              nullptr, // macro definitions
                              nullptr, // include handler
                              "main", // entry point
                              "vs_5_0", // target profile (e.g., vs_5_0, ps_5_0)
                              0, // compile flags
                              0, // effect flags
                              &pblobShader, // compiled shader
                              &pblobError // error messages
      );

      if (FAILED(hr))
      {

         if (pblobError)
         {

            ::string strError((const_char_pointer)pblobError->GetBufferPointer(), pblobError->GetBufferSize());

            warning() << strError;

            throw ::exception(error_failed);
         }
      }

      return pblobShader;
   }


   ::comptr<ID3DBlob> shader::create_pixel_shader_blob(const ::block &block)
   {

      comptr<ID3DBlob> pblobShader;
      comptr<ID3DBlob> pblobError;

      auto data = (const_char_pointer)block.data();

      auto size = block.size();

      HRESULT hr = D3DCompile(data, // pointer to shader source
                              size, // size of shader source
                              nullptr, // optional source name
                              nullptr, // macro definitions
                              nullptr, // include handler
                              "main", // entry point
                              "ps_5_0", // target profile (e.g., vs_5_0, ps_5_0)
                              0, // compile flags
                              0, // effect flags
                              &pblobShader, // compiled shader
                              &pblobError // error messages
      );

      if (FAILED(hr))
      {

         if (pblobError)
         {

            ::string strError((const_char_pointer)pblobError->GetBufferPointer(), pblobError->GetBufferSize());

            warning() << strError;

            throw ::exception(error_failed);
         }
      }

      return pblobShader;
   }


   void shader::create_vertex_shader(const ::block &block)
   {

      auto pblobShader = create_vertex_shader_blob(block);

      ::cast<device> pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      auto hresult = pgpudevice->m_pd3d11device->CreateVertexShader(pblobShader->GetBufferPointer(),
                                                               pblobShader->GetBufferSize(), nullptr, &m_pvertexshader);

      if (FAILED(hresult))
      {

         throw ::hresult_exception(hresult);
      }

      //::array < D3D11_INPUT_ELEMENT_DESC > layout;

      // auto countInputLayout = m_propertiesInputLayout.count();

      // if (countInputLayout > 0)
      //{

      //   int iSemanticIndex = 0;
      //   int iInputSlot = 0;
      //   D3D11_INPUT_CLASSIFICATION classification = D3D11_INPUT_PER_VERTEX_DATA;
      //   UINT DataStepRate = 0;
      //   int iOffset = 0;
      //   int iNextOffset = 0;

      //   for (::collection::index iInputLayout = 0; iInputLayout < countInputLayout; iInputLayout++)
      //   {

      //      auto pproperty = m_propertiesInputLayout.m_pproperties + iInputLayout;

      //      auto name = pproperty->m_pszName;
      //      auto type = pproperty->m_etype;
      //      auto offset = iNextOffset;
      //      iNextOffset = offset + input_layout_aligned_property_size(pproperty->get_item_size());

      //      D3D11_INPUT_ELEMENT_DESC desc{};

      //      desc.SemanticName = input_layout_semantic_name_from_gpu_property_name(name);
      //      desc.SemanticIndex = iSemanticIndex;
      //      desc.Format = input_layout_format_from_gpu_property_type(type);
      //      desc.InputSlot = iInputSlot;
      //      desc.AlignedByteOffset = offset;
      //      desc.InputSlotClass = classification;
      //      desc.InstanceDataStepRate = DataStepRate;

      //      layout.add(desc);

      //   }

      //   //// Input layout
      //   //D3D12_INPUT_ELEMENT_DESC layout[] = {
      //   //    {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
      //   //    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
      //   //};

      //}


      ///::array < D3D11_INPUT_ELEMENT_DESC > layout;

      // UINT uOffset0 = offsetof(gpu::Vertex, position);
      // UINT uOffset1 = offsetof(gpu::Vertex, color);
      // UINT uOffset2 = offsetof(gpu::Vertex, normal);
      // UINT uOffset3 = offsetof(gpu::Vertex, uv);

      // layout.add({ "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
      // layout.add({ "COLOR"    , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset1, D3D11_INPUT_PER_VERTEX_DATA, 0 });
      // layout.add({ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset2, D3D11_INPUT_PER_VERTEX_DATA, 0 });
      // layout.add({ "TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT    , 0, uOffset3, D3D11_INPUT_PER_VERTEX_DATA, 0 });

      ::cast<input_layout> pinputlayout = m_pinputlayout;

      if (pinputlayout)
      {

         m_pd3d11inputlayout = pinputlayout->_get_d3d11_input_layout(pblobShader);
      }
   }


   void shader::create_pixel_shader(const ::block &block)
   {

      auto pblobShader = create_pixel_shader_blob(block);

      ::cast<device> pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      pgpudevice->m_pd3d11device->CreatePixelShader(pblobShader->GetBufferPointer(), pblobShader->GetBufferSize(), nullptr,
                                               &m_ppixelshader);
   }


   void shader::on_initialize_shader()
   {

      //_create_pipeline_layout(m_properties.m_memory.size());

      // øconstruct_new(m_ppipeline);

      ::cast<context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast<device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast<renderer> prenderer = m_pgpurenderer;

      // PipelineConfigInfo pipelineConfig{};

      //::cast < input_layout > pshadervertexinput = m_pinputlayout;


      pgpudevice->defer_shader_memory(m_memoryVertex, m_pathVertex);
      pgpudevice->defer_shader_memory(m_memoryFragment, m_pathFragment);


      create_vertex_shader(m_memoryVertex);
      create_pixel_shader(m_memoryFragment);

      D3D11_DEPTH_STENCIL_DESC dsDesc = {};
      if (m_bDisableDepthTest)
      {
         dsDesc.DepthEnable = FALSE; // ✅ disable depth test
      }
      else
      {
         dsDesc.DepthEnable = TRUE; // ✅ disable depth test
      }
      if (m_bDepthTestButNoDepthWrite || m_bDisableDepthTest)
      {
         dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // also disables writing to depth buffer
      }
      else
      {
         dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
      }
      if (m_bLequalDepth)
      {
         dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; // not used since DepthEnable = FALSE
      }
      else
      {
         dsDesc.DepthFunc = D3D11_COMPARISON_LESS; // not used since DepthEnable = FALSE
      }

      // (optional) Stencil test settings
      dsDesc.StencilEnable = FALSE;

      //::cast < device > pgpudevice = pgpucontext->m_pgpudevice;

      HRESULT hrCreateDepthStencilState =
         pgpudevice->m_pd3d11device->CreateDepthStencilState(&dsDesc, &m_pdepthstencilstate2);

      ::defer_throw_hresult(hrCreateDepthStencilState);

      // if (!pgpucontext->m_prasterizerstate)
      {

         D3D11_RASTERIZER_DESC rasterizerDesc = {};
         rasterizerDesc.FillMode = D3D11_FILL_SOLID;
         if (m_ecullmode == ::gpu::e_cull_mode_back)
         {
            rasterizerDesc.CullMode = D3D11_CULL_BACK; // Cull back faces
         }
         else if (m_ecullmode == ::gpu::e_cull_mode_front)
         {
            rasterizerDesc.CullMode = D3D11_CULL_FRONT; // Cull back faces
         }
         else
         {
            rasterizerDesc.CullMode = D3D11_CULL_NONE; // Cull back faces
         }
         // rasterizerDesc.CullMode = D3D11_CULL_FRONT;
         rasterizerDesc.FrontCounterClockwise = false; // Treat CCW as front-facing
         // rasterizerDesc.FrontCounterClockwise = true;
         // rasterizerDesc.DepthClipEnable = TRUE;

         HRESULT hr =
            pgpucontext->m_pgpudevice->m_pd3d11device->CreateRasterizerState(&rasterizerDesc, &m_prasterizerstate2);

         ::defer_throw_hresult(hr);
      }
   }


   // void shader::on_set_constant_buffer(const ::scoped_string& scopedstrName)
   //{

   //   auto p1 = m_mapConstantBuffer.find(scopedstrName);

   //   if(p1)
   //   {

   //      auto& constantbuffer = p1->m_element2;

   //      ::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

   //      auto size = p1->m_element2.m_memory.size();

   //      auto & poolmemorybuffer = pgpucontext->m_mapPoolMemoryBuffer[size];

   //      if (!poolmemorybuffer.m_ppoolgroup)
   //      {

   //         poolmemorybuffer.m_ppoolgroup = pgpucontext->m_pgpudevice->frame_pool_group(
   //            m_pgpurenderer->m_pgpurendertarget->get_frame_index());

   //      }

   //      ::cast < ::gpu_directx11::memory_buffer > pmemorybuffer = poolmemorybuffer.get();

   //      if (pmemorybuffer->m_bNew)
   //      {

   //         pmemorybuffer->initialize_memory_buffer_with_context(pgpucontext, size,
   //         ::gpu::memory_buffer::e_type_constant_buffer);

   //      }

   //      pmemorybuffer->assign(p1->m_element2.m_memory);


   //
   //      //D3D11_MAPPED_SUBRESOURCE mapped;
   //      //pgpucontext->m_pcontext->Map(pd3d11buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
   //      //memcpy(mapped.pData, p1->m_element2.m_memory.data(), p1->m_element2.m_memory.size());
   //      //pgpucontext->m_pcontext->Unmap(pd3d11buffer, 0);

   //      if (constantbuffer.m_i1FragmentShader >= 0
   //         && constantbuffer.m_i2FragmentShader >= 0)
   //      {

   //         pgpucontext->m_pcontext->PSSetConstantBuffers(
   //            constantbuffer.m_i1FragmentShader,
   //            constantbuffer.m_i2FragmentShader,
   //            pmemorybuffer->m_pbuffer.pp());

   //      }


   //      if (constantbuffer.m_i1VertexShader >= 0
   //         && constantbuffer.m_i2VertexShader >= 0)
   //      {

   //         pgpucontext->m_pcontext->VSSetConstantBuffers(
   //            constantbuffer.m_i1VertexShader,
   //            constantbuffer.m_i2VertexShader,
   //            pmemorybuffer->m_pbuffer.pp());

   //      }

   //   }

   //}


   // void shader::bind(::gpu::command_buffer *pgpucommandbuffer, ::gpu::texture *pgputextureTarget,
   //                   ::gpu::texture *pgputextureSource)
   //{

   //   bind(pgpucommandbuffer, pgputextureTarget);

   //   bind_source(pgpucommandbuffer, pgputextureSource, 0);

   //}


   void shader::bind_source(::gpu::command_buffer *pgpucommandbuffer, ::gpu::texture *pgputextureSource, int iSlot)
   {

      ::cast<context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::gpu::context_lock context_lock(pgpucontext);

      ::cast<device> pgpudevice = pgpucontext->m_pgpudevice;

      if (pgputextureSource)
      {

         ::cast<texture> ptextureSrc = pgputextureSource;

         if (!ptextureSrc->m_pshaderresourceview)
         {

            ptextureSrc->create_shader_resource_view();
         }

         auto pshaderresourceview = ptextureSrc->m_pshaderresourceview;

         if (pshaderresourceview)
         {

            ID3D11ShaderResourceView *shaderresourceviewa[] = {pshaderresourceview};
            pgpucontext->m_pcontext->PSSetShaderResources(0, 1, shaderresourceviewa);
         }

         auto psamplerstate = ptextureSrc->m_psamplerstate;

         if (psamplerstate)
         {

            ID3D11SamplerState *samplerstatea[] = {psamplerstate};

            pgpucontext->m_pcontext->PSSetSamplers(0, 1, samplerstatea);
         }
      }
   }


   void shader::bind(::gpu::command_buffer *pgpucommandbuffer, ::gpu::texture *pgputextureTarget)
   {


      ::cast<context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::gpu::context_lock context_lock(pgpucontext);

      ::cast<device> pgpudevice = pgpucontext->m_pgpudevice;


      // defer_throw_hresult(hr1);

      pgpucontext->m_pcontext->VSSetShader(m_pvertexshader, nullptr, 0);

      /// defer_throw_hresult(hr2);

      pgpucontext->m_pcontext->PSSetShader(m_ppixelshader, nullptr, 0);


      if (m_pinputlayout)
      {

         if (!m_pd3d11inputlayout)
         {

            throw ::exception(error_wrong_state);
         }

         pgpucontext->m_pcontext->IASetInputLayout(m_pd3d11inputlayout);
      }
      else
      {

         pgpucontext->m_pcontext->IASetInputLayout(nullptr);
      }

      if (m_pdepthstencilstate2)
      {

         // Bind the new depth-stencil state
         pgpucontext->m_pcontext->OMSetDepthStencilState(m_pdepthstencilstate2, 0);
      }

      if (m_prasterizerstate2)
      {

         pgpucontext->m_pcontext->RSSetState(m_prasterizerstate2);
      }

      auto etopology = ::directx11::as_d3d11_topology(m_etopology);

      pgpucontext->m_pcontext->IASetPrimitiveTopology(etopology);

      //::cast<::gpu_directx11::block> pblockGlobalUbo1 =
      //   pgpucontext->m_pengine->m_pimmersionlayer->m_pscene->global_ubo1(pgpucontext);

      // auto pbuffer = pblockGlobalUbo1->m_pbuffer.m_p;

      // pgpucontext->m_pcontext->VSSetConstantBuffers(0, 1, &pbuffer);
      // pgpucontext->m_pcontext->PSSetConstantBuffers(0, 1, &pbuffer);

      //_bind(pgpucommandbuffer, ::gpu::e_scene_none);

      ///::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      //::gpu::context_lock context_lock(pgpucontext);

      //::cast <device> pgpudevice = pgpucontext->m_pgpudevice;

      if (!pgputextureTarget)
      {

         ::cast<renderer> prenderer = m_pgpurenderer;

         if (prenderer)
         {

            auto pgpurendertarget = m_pgpurenderer->render_target();

            ::cast<render_target_view> pgpurendertargetview = pgpurendertarget;

            if (pgpurendertargetview)
            {

               ::cast<texture> ptexture = pgpurendertargetview->current_texture(::gpu::current_frame());

               pgputextureTarget = ptexture.m_p;
            }
         }
      }

      if (pgputextureTarget)
      {

         ::cast<texture> ptextureDst = pgputextureTarget;

         if (!ptextureDst->m_prendertargetview)
         {

            ptextureDst->create_render_target();
         }

         if (ptextureDst->m_iCurrentLayer < 0)
         {

            ID3D11RenderTargetView *rendertargetviewa[1];

            rendertargetviewa[0] = ptextureDst->m_prendertargetview;

            ID3D11DepthStencilView *pdepthstencilview = ptextureDst->m_pdepthstencilview;

            if (pdepthstencilview)
            {

               pgpucontext->m_pcontext->OMSetRenderTargets(1, rendertargetviewa, pdepthstencilview);
            }
            else
            {

               pgpucontext->m_pcontext->OMSetRenderTargets(1, rendertargetviewa, nullptr);
            }
         }
      }

      defer_bind_frame_buffer_layer(pgpucommandbuffer, pgputextureTarget);
   }


   void shader::defer_bind_frame_buffer_layer(::gpu::command_buffer *pgpucommandbuffer,
                                              ::gpu::texture *pgputextureTarget)
   {

      ::cast<texture> ptexture = pgputextureTarget;

      if (ptexture->m_textureattributes.m_etexture == ::gpu::e_texture_cube_map)
      {

         if (ptexture->m_iCurrentLayer >= 0)
         {

            ::cast<context> pgpucontext = pgpucommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;

            ID3D11RenderTargetView *rendertargetviewa[1];

            rendertargetviewa[0] = ptexture->m_rendertargetview2a[ptexture->m_iCurrentMip][ptexture->m_iCurrentLayer];

            ID3D11DepthStencilView *pdepthstencilview = ptexture->m_pdepthstencilview;
            if (pdepthstencilview)
            {
               pgpucontext->m_pcontext->OMSetRenderTargets(1, rendertargetviewa, pdepthstencilview);
            }
            else
            {

               pgpucontext->m_pcontext->OMSetRenderTargets(1, rendertargetviewa, nullptr);
            }
         }
      }
   }

   void shader::on_bind_already_bound(::gpu::command_buffer *pgpucommandbuffer, ::gpu::texture *pgputextureTarget)
   
   {
   
      defer_bind_frame_buffer_layer(pgpucommandbuffer, pgputextureTarget);
   
   }


   // void shader::bind(::gpu::command_buffer *pgpucommandbuffer)
   //{

   //   ::cast <texture> ptexture = m_pgpurenderer->current_render_target_texture(::gpu::current_frame());

   //   bind(pgpucommandbuffer, ptexture);

   //   //::gpu::context_lock context_lock(pgpucontext);

   //   //::cast <device> pgpudevice = pgpucontext->m_pgpudevice;


   //   ////defer_throw_hresult(hr1);

   //   //pgpucontext->m_pcontext->VSSetShader(m_pvertexshader, nullptr, 0);

   //   /////defer_throw_hresult(hr2);

   //   //pgpucontext->m_pcontext->PSSetShader(m_ppixelshader, nullptr, 0);


   //   //if (m_pinputlayout)
   //   //{

   //   //   if (!m_pd3d11inputlayout)
   //   //   {

   //   //      throw ::exception(error_wrong_state);

   //   //   }

   //   //   pgpucontext->m_pcontext->IASetInputLayout(m_pd3d11inputlayout);

   //   //}
   //   //else
   //   //{

   //   //   pgpucontext->m_pcontext->IASetInputLayout(nullptr);

   //   //}

   //}


   // void shader::_bind(::gpu::command_buffer *pgpucommandbuffer, ::gpu::enum_scene escene)
   //{

   //   ::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

   //   ::gpu::context_lock context_lock(pgpucontext);

   //   ::cast <device> pgpudevice = pgpucontext->m_pgpudevice;


   //   //defer_throw_hresult(hr1);

   //   pgpucontext->m_pcontext->VSSetShader(m_pvertexshader, nullptr, 0);

   //   ///defer_throw_hresult(hr2);

   //   pgpucontext->m_pcontext->PSSetShader(m_ppixelshader, nullptr, 0);


   //   if (m_pinputlayout)
   //   {

   //      if (!m_pd3d11inputlayout)
   //      {

   //         throw ::exception(error_wrong_state);

   //      }

   //      pgpucontext->m_pcontext->IASetInputLayout(m_pd3d11inputlayout);

   //   }
   //   else
   //   {

   //      pgpucontext->m_pcontext->IASetInputLayout(nullptr);

   //   }

   //   if (m_pdepthstencilstate2)
   //   {

   //      // Bind the new depth-stencil state
   //      pgpucontext->m_pcontext->OMSetDepthStencilState(m_pdepthstencilstate2, 0);

   //   }

   //   if (m_prasterizerstate2)
   //   {
   //
   //      pgpucontext->m_pcontext->RSSetState(m_prasterizerstate2);

   //   }

   //   auto etopology = ::directx11::as_d3d11_topology(m_etopology);

   //   pgpucontext->m_pcontext->IASetPrimitiveTopology(etopology);

   //   ::cast < ::gpu_directx11::block >  pblockGlobalUbo1 =
   //   pgpucontext->m_pengine->m_pimmersionlayer->m_pscene->global_ubo1(pgpucontext);

   //   auto pbuffer = pblockGlobalUbo1->m_pbuffer.m_p;

   //   pgpucontext->m_pcontext->VSSetConstantBuffers(0, 1, &pbuffer);
   //   pgpucontext->m_pcontext->PSSetConstantBuffers(0, 1, &pbuffer);


   //}


   void shader::unbind(::gpu::command_buffer *pgpucommandbuffer)
   {

      ::cast<context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::gpu::context_lock context_lock(pgpucontext);

      ::cast<device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast<renderer> prenderer = m_pgpurenderer;

      // pgpucontext->m_pcontext->IASetInputLayout(m_pinputlayout);

      // defer_throw_hresult(hr1);

      if (m_pvertexshader)
      {

         pgpucontext->m_pcontext->VSSetShader(nullptr, nullptr, 0);
      }

      /// defer_throw_hresult(hr2);

      if (m_ppixelshader)
      {

         pgpucontext->m_pcontext->PSSetShader(nullptr, nullptr, 0);
      }

      if (prenderer)
      {

         ::cast<render_target_view> pgpurendertargetview = prenderer->render_target();

         if (pgpurendertargetview)
         {

            ::cast<texture> ptexture = pgpurendertargetview->current_texture(::gpu::current_frame());

            //::cast < offscreen_render_target_view > poffscreenrendertargetview = pgpurendertargetview;

            if (ptexture)
            {

               auto pshaderresourceview = ptexture->m_pshaderresourceview;

               if (pshaderresourceview)
               {

                  ID3D11ShaderResourceView *nullSRV[1] = {nullptr};

                  pgpucontext->m_pcontext->PSSetShaderResources(0, 1, nullSRV);
               }
            }
         }
      }
   }


   void shader::push_properties(::gpu::command_buffer *pgpucommandbuffer)
   {

      ::cast<renderer> prenderer = m_pgpurenderer;

      //   auto commandBuffer = prenderer->getCurrentCommandBuffer();

      //   vkCmdPushConstants(
      //      commandBuffer,
      //      m_vkpipelinelayout,
      //      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
      //      0,
      //      m_properties.size(),
      //      m_properties.data());

      // if (m_propertiesPush.size(false) <= 0)
      //{

      //   return;

      //}

      ::cast<context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast<device> pgpudevice = pgpucontext->m_pgpudevice;

      auto iSetSizeShared = m_propertiesPushShared.size(false);

      if (iSetSizeShared != m_iSizeSharedPushConstants || !m_pbufferSharedPushConstants)
      {

         m_pbufferSharedPushConstants.Release();

         D3D11_BUFFER_DESC cbDesc = {};
         cbDesc.ByteWidth = (m_propertiesPushShared.size(false) + 15) & ~15;
         cbDesc.Usage = D3D11_USAGE_DYNAMIC;
         cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
         cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

         auto pdevice = pgpudevice->m_pd3d11device;

         HRESULT hr = pdevice->CreateBuffer(&cbDesc, nullptr, &m_pbufferSharedPushConstants);

         if (FAILED(hr))
         {

            throw ::hresult_exception(hr);
         }

         m_iSizeSharedPushConstants = (int)m_propertiesPushShared.size(false);
      }

      // PushConstants pc = { XMFLOAT4(1, 0, 0, 1), currentTime };

      D3D11_MAPPED_SUBRESOURCE mapped;
      HRESULT hrMap =
         pgpucontext->m_pcontext->Map(m_pbufferSharedPushConstants, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

      if (FAILED(hrMap))
      {

         throw ::hresult_exception(hrMap);
      }

      memcpy(mapped.pData, m_propertiesPushShared.data(false), m_propertiesPushShared.size(false));
      pgpucontext->m_pcontext->Unmap(m_pbufferSharedPushConstants, 0);


      // pgpucontext->m_pcontext->VSSetConstantBuffers(0, 1, pgpucontext->m_pbufferGlobalUbo.pp());
      // pgpucontext->m_pcontext->PSSetConstantBuffers(0, 1, pgpucontext->m_pbufferGlobalUbo.pp());


      auto pVS = m_pbufferSharedPushConstants.m_p;
      pgpucontext->m_pcontext->VSSetConstantBuffers(1, 1, &pVS);
      auto pPS = m_pbufferSharedPushConstants.m_p;
      pgpucontext->m_pcontext->PSSetConstantBuffers(1, 1, &pPS);
   }


   void shader::setModelViewProjection(const floating_matrix4 &model, const floating_matrix4 &view,
                                       const floating_matrix4 &projection)
   {

      set_matrix4("model", model);
      // set_matrix4("view", m_pgpurenderer->m_pgpucontext->defer_remap_impact_matrix(view));
      set_matrix4("view", view);
      set_matrix4("projection", projection);
      // set_matrix4("view", m_pgpurenderer->m_pgpucontext->defer_remap_impact_matrix(view));
      // set_matrix4("projection", m_pgpurenderer->m_pgpucontext->defer_clip_remap_projection(projection));
   }


   void shader::draw()
   {

      ::cast<context> pgpucontext = m_pgpurenderer->m_pgpucontext;
      pgpucontext->m_pcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      pgpucontext->m_pcontext->Draw(6, 0);
      //   ::cast < renderer > prenderer = m_pgpurenderer;

      //   auto commandBuffer = prenderer->getCurrentCommandBuffer();

      //   vkCmdDraw(commandBuffer, 6, 1, 0, 0);
   }


   void shader::bind_slot_set(::gpu::command_buffer *pgpucommandbuffer, int iSet,
                              ::gpu::binding_slot_set *pgpubindingslotset)
   {

      int iSlotIndex = -1;

      for (auto &bindingslot: *pgpubindingslotset)
      {

         iSlotIndex++;

         if (bindingslot.m_pblock)
         {

            ::cast<::gpu_directx11::block> pblock = bindingslot.m_pblock;

            auto pbuffer = pblock->m_pbuffer;

            int iBinding = bindingslot.m_pbinding->m_iBindingPoint2;

            ::cast<::gpu_directx11::context> pcontext =
               pgpucommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext;

            auto pVS = pbuffer.m_p;
            pcontext->m_pcontext->VSSetConstantBuffers(iBinding, 1, &pVS);
            auto pPS = pbuffer.m_p;
            pcontext->m_pcontext->PSSetConstantBuffers(iBinding, 1, &pPS);
         }
         else if (bindingslot.m_ptexture)
         {

            ::cast<::gpu_directx11::texture> ptexture = bindingslot.m_ptexture;

            int iBinding = bindingslot.m_pbinding->m_iBindingPoint2;

            ::cast<context> pgpucontext = m_pgpurenderer->m_pgpucontext;

            ::cast<device> pgpudevice = pgpucontext->m_pgpudevice;

            if (!ptexture->m_pshaderresourceview)
            {

               ptexture->create_shader_resource_view();
            }

            auto pshaderresourceview = ptexture->m_pshaderresourceview;

            if (pshaderresourceview)
            {

               ID3D11ShaderResourceView *shaderresourceviewa[] = {pshaderresourceview};
               pgpucontext->m_pcontext->PSSetShaderResources(iBinding, 1, shaderresourceviewa);
            }

            if (iSlotIndex == 0)
            {

               auto psamplerstate = ptexture->m_psamplerstate;

               if (psamplerstate)
               {

                  ID3D11SamplerState *samplerstatea[] = {psamplerstate};

                  pgpucontext->m_pcontext->PSSetSamplers(iBinding, 1, samplerstatea);
               }

            }
         }

         //      }
      }
   }


} // namespace gpu_directx11
