#include "framework.h"
// Co-creating with V0idsEmbrace@Twitch with
// camilo on 2025-05-19 04:59 <3ThomasBorregaardSorensen!!
#include "approach.h"
#include "shader.h"
#include "context.h"
#include "descriptors.h"
#include "input_layout.h"
#include "memory_buffer.h"
#include "renderer.h"
#include "texture.h"
#include "offscreen_render_target_view.h"
#include "bred/gpu/context_lock.h"
#include "bred/gpu/frame.h"
#include "bred/gpu/types.h"
#include "acme/operating_system/windows_common/com/hresult_exception.h"
#include <d3dcompiler.h>
//#include "bred/user/user/graphics3d.h"



namespace gpu_directx11
{


   shader::shader()
   {

      //m_vktopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

   }


   shader::~shader()
   {

      ::cast < context > pgpucontext = m_pgpurenderer->m_pgpucontext;

      // vkDestroyPipelineLayout(pgpucontext->logicalDevice(), m_vkpipelinelayout, nullptr);

   }


   ::comptr < ID3DBlob> shader::create_vertex_shader_blob(const ::block& block)
   {

      comptr <ID3DBlob> pblobShader;
      comptr <ID3DBlob> pblobError;

      auto data = (const char*)block.data();

      auto size = block.size();

      HRESULT hr = D3DCompile(
         data,            // pointer to shader source
         size,             // size of shader source
         nullptr,                       // optional source name
         nullptr,                       // macro definitions
         nullptr,                       // include handler
         "main",                      // entry point
         "vs_5_0",                      // target profile (e.g., vs_5_0, ps_5_0)
         0,                             // compile flags
         0,                             // effect flags
         &pblobShader,                   // compiled shader
         &pblobError                     // error messages
      );

      if (FAILED(hr))
      {

         if (pblobError)
         {

            ::string strError((const char *) pblobError->GetBufferPointer(),
               pblobError->GetBufferSize());

            throw ::exception(error_failed);

         }
      }

      return pblobShader;

   }


   ::comptr < ID3DBlob> shader::create_pixel_shader_blob(const ::block& block)
   {

      comptr <ID3DBlob> pblobShader;
      comptr <ID3DBlob> pblobError;

      auto data = (const char*)block.data();

      auto size = block.size();

      HRESULT hr = D3DCompile(
         data,            // pointer to shader source
         size,             // size of shader source
         nullptr,                       // optional source name
         nullptr,                       // macro definitions
         nullptr,                       // include handler
         "main",                      // entry point
         "ps_5_0",                      // target profile (e.g., vs_5_0, ps_5_0)
         0,                             // compile flags
         0,                             // effect flags
         &pblobShader,                   // compiled shader
         &pblobError                     // error messages
      );

      if (FAILED(hr))
      {

         if (pblobError)
         {

            ::string strError((const char*)pblobError->GetBufferPointer(),
               pblobError->GetBufferSize());

            throw ::exception(error_failed);

         }
      }

      return pblobShader;

   }



   void shader::create_vertex_shader(const ::block& block)
   {

      auto pblobShader = create_vertex_shader_blob(block);

      ::cast < device > pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      auto hresult = pgpudevice->m_pdevice->CreateVertexShader(
         pblobShader->GetBufferPointer(),
         pblobShader->GetBufferSize(),
         nullptr,
         &m_pvertexshader
      );

      if (FAILED(hresult))
      {

         throw ::hresult_exception(hresult);

      }

      //::array < D3D11_INPUT_ELEMENT_DESC > layout;

      //auto countInputLayout = m_propertiesInputLayout.count();

      //if (countInputLayout > 0)
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

      //UINT uOffset0 = offsetof(gpu::Vertex, position);
      //UINT uOffset1 = offsetof(gpu::Vertex, color);
      //UINT uOffset2 = offsetof(gpu::Vertex, normal);
      //UINT uOffset3 = offsetof(gpu::Vertex, uv);
      
      //layout.add({ "POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
      //layout.add({ "COLOR"    , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset1, D3D11_INPUT_PER_VERTEX_DATA, 0 });
      //layout.add({ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, uOffset2, D3D11_INPUT_PER_VERTEX_DATA, 0 });
      //layout.add({ "TEXCOORD" , 0, DXGI_FORMAT_R32G32_FLOAT    , 0, uOffset3, D3D11_INPUT_PER_VERTEX_DATA, 0 });
      
      ::cast < input_layout > pinputlayout = m_pinputlayout;

      if (pinputlayout)
      {

         m_pd3d11inputlayout = pinputlayout->_get_d3d11_input_layout(pblobShader);

      }



   }

   
   void shader::create_pixel_shader(const ::block& block)
   {

      auto pblobShader = create_pixel_shader_blob(block);

      ::cast < device > pgpudevice = m_pgpurenderer->m_pgpucontext->m_pgpudevice;

      pgpudevice->m_pdevice->CreatePixelShader(
         pblobShader->GetBufferPointer(),
         pblobShader->GetBufferSize(),
         nullptr,
         &m_ppixelshader
      );

   }



   void shader::on_initialize_shader()
   {

      //_create_pipeline_layout(m_properties.m_memory.size());

      //__construct_new(m_ppipeline);

      ::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast <device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast <renderer> prenderer = m_pgpurenderer;

      //PipelineConfigInfo pipelineConfig{};

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

      HRESULT hrCreateDepthStencilState = pgpudevice->m_pdevice->CreateDepthStencilState(
         &dsDesc, &m_pdepthstencilstate);

      ::defer_throw_hresult(hrCreateDepthStencilState);

      //if (!pgpucontext->m_prasterizerstate)
      {

         D3D11_RASTERIZER_DESC rasterizerDesc = {};
         rasterizerDesc.FillMode = D3D11_FILL_SOLID;
         if (m_ecullmode == ::gpu::e_cull_mode_back)
         {
            rasterizerDesc.CullMode = D3D11_CULL_BACK;        // Cull back faces
         }
         else if (m_ecullmode == ::gpu::e_cull_mode_front)
         {
            rasterizerDesc.CullMode = D3D11_CULL_FRONT;        // Cull back faces
         }
         else
         {
            rasterizerDesc.CullMode = D3D11_CULL_NONE;        // Cull back faces
         }
         //rasterizerDesc.CullMode = D3D11_CULL_FRONT; 
         rasterizerDesc.FrontCounterClockwise = false; // Treat CCW as front-facing
         //rasterizerDesc.FrontCounterClockwise = true;
         //rasterizerDesc.DepthClipEnable = TRUE;

         HRESULT hr = pgpucontext->m_pgpudevice->m_pdevice->CreateRasterizerState(
            &rasterizerDesc,
            &pgpucontext->m_prasterizerstate);

         ::defer_throw_hresult(hr);

      }




   }


   void shader::on_set_constant_buffer(const ::scoped_string& scopedstrName)
   {

      auto p1 = m_mapConstantBuffer.plookup(scopedstrName);

      if(p1)
      {

         auto& constantbuffer = p1->m_element2;
 
         ::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

         auto size = p1->m_element2.m_memory.size();
 
         auto & poolmemorybuffer = pgpucontext->m_mapPoolMemoryBuffer[size];

         if (!poolmemorybuffer.m_ppoolgroup)
         {

            poolmemorybuffer.m_ppoolgroup = pgpucontext->m_pgpudevice->frame_pool_group(
               m_pgpurenderer->m_pgpurendertarget->get_frame_index());

         }

         ::cast < ::gpu_directx11::memory_buffer > pmemorybuffer = poolmemorybuffer.get();

         if (pmemorybuffer->m_bNew)
         {

            pmemorybuffer->initialize_memory_buffer_with_context(pgpucontext, size, ::gpu::memory_buffer::e_type_constant_buffer);

         }

         pmemorybuffer->assign(p1->m_element2.m_memory);


         
         //D3D11_MAPPED_SUBRESOURCE mapped;
         //pgpucontext->m_pcontext->Map(pd3d11buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
         //memcpy(mapped.pData, p1->m_element2.m_memory.data(), p1->m_element2.m_memory.size());
         //pgpucontext->m_pcontext->Unmap(pd3d11buffer, 0);

         if (constantbuffer.m_i1FragmentShader >= 0
            && constantbuffer.m_i2FragmentShader >= 0)
         {

            pgpucontext->m_pcontext->PSSetConstantBuffers(
               constantbuffer.m_i1FragmentShader, 
               constantbuffer.m_i2FragmentShader,
               pmemorybuffer->m_pbuffer.pp());

         }


         if (constantbuffer.m_i1VertexShader >= 0
            && constantbuffer.m_i2VertexShader >= 0)
         {

            pgpucontext->m_pcontext->VSSetConstantBuffers(
               constantbuffer.m_i1VertexShader,
               constantbuffer.m_i2VertexShader,
               pmemorybuffer->m_pbuffer.pp());

         }

      }

   }


   void shader::bind(::gpu::texture* pgputextureTarget, ::gpu::texture* pgputextureSource)
   {

      bind(pgputextureTarget);

      bind_source(pgputextureSource);

   }


   void shader::bind_source( ::gpu::texture* pgputextureSource)
   {

      ::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::gpu::context_lock context_lock(pgpucontext);

      ::cast <device> pgpudevice = pgpucontext->m_pgpudevice;

      if (pgputextureSource)
      {

         ::cast < texture > ptextureSrc = pgputextureSource;

         if (!ptextureSrc->m_pshaderresourceview)
         {

            ptextureSrc->create_shader_resource_view();

         }

         auto pshaderresourceview = ptextureSrc->m_pshaderresourceview;

         if (pshaderresourceview)
         {

            ID3D11ShaderResourceView* shaderresourceviewa[] =
            {
               pshaderresourceview
            };
            pgpucontext->m_pcontext->PSSetShaderResources(0, 1, shaderresourceviewa);

         }

         auto psamplerstate = ptextureSrc->m_psamplerstate;

         if (psamplerstate)
         {

            ID3D11SamplerState* samplerstatea[] =
            {
               psamplerstate
            };

            pgpucontext->m_pcontext->PSSetSamplers(
               0, 1, samplerstatea);

         }


      }

   }


   void shader::bind(::gpu::texture* pgputextureTarget)
   {

      _bind();

      ::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::gpu::context_lock context_lock(pgpucontext);

      ::cast <device> pgpudevice = pgpucontext->m_pgpudevice;

      if (!pgputextureTarget)
      {

         ::cast <renderer> prenderer = m_pgpurenderer;

         if (prenderer)
         {

            ::cast < render_target_view > pgpurendertargetview = prenderer->m_pgpurendertarget;

            if (pgpurendertargetview)
            {

               ::cast < texture > ptexture = pgpurendertargetview->current_texture(::gpu::current_frame());

               pgputextureTarget = ptexture.m_p;

            }

         }

      }

      if (pgputextureTarget)
      {

         ::cast < texture > ptextureDst = pgputextureTarget;

         if (!ptextureDst->m_prendertargetview)
         {

            ptextureDst->create_render_target_view();

         }

         ID3D11RenderTargetView* rendertargetviewa[] =
         {
            ptextureDst->m_prendertargetview
         };

         ID3D11DepthStencilView* pdepthstencilview = ptextureDst->m_pdepthstencilview;
         pgpucontext->m_pcontext->OMSetRenderTargets(
            1,
            rendertargetviewa,
            pdepthstencilview);

      }

   }


   void shader::bind()
   {

      ::cast <texture> ptexture = m_pgpurenderer->current_render_target_texture(::gpu::current_frame());

      bind(ptexture);

      //::gpu::context_lock context_lock(pgpucontext);

      //::cast <device> pgpudevice = pgpucontext->m_pgpudevice;


      ////defer_throw_hresult(hr1);

      //pgpucontext->m_pcontext->VSSetShader(m_pvertexshader, nullptr, 0);

      /////defer_throw_hresult(hr2);

      //pgpucontext->m_pcontext->PSSetShader(m_ppixelshader, nullptr, 0);


      //if (m_pinputlayout)
      //{

      //   if (!m_pd3d11inputlayout)
      //   {

      //      throw ::exception(error_wrong_state);

      //   }

      //   pgpucontext->m_pcontext->IASetInputLayout(m_pd3d11inputlayout);

      //}
      //else
      //{

      //   pgpucontext->m_pcontext->IASetInputLayout(nullptr);

      //}

   }


   void shader::_bind()
   {

      ::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::gpu::context_lock context_lock(pgpucontext);

      ::cast <device> pgpudevice = pgpucontext->m_pgpudevice;


      //defer_throw_hresult(hr1);

      pgpucontext->m_pcontext->VSSetShader(m_pvertexshader, nullptr, 0);

      ///defer_throw_hresult(hr2);

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

      if (m_pdepthstencilstate)
      {

         // Bind the new depth-stencil state
         pgpucontext->m_pcontext->OMSetDepthStencilState(m_pdepthstencilstate, 0);

      }

      if (m_prasterizerstate)
      {
       
         pgpucontext->m_pcontext->RSSetState(m_prasterizerstate);

      }

   }


   void shader::unbind()
   {

      ::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::gpu::context_lock context_lock(pgpucontext);

      ::cast <device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast <renderer> prenderer = m_pgpurenderer;

      //pgpucontext->m_pcontext->IASetInputLayout(m_pinputlayout);

      //defer_throw_hresult(hr1);

      if (m_pvertexshader)
      {

         pgpucontext->m_pcontext->VSSetShader(nullptr, nullptr, 0);

      }

      ///defer_throw_hresult(hr2);

      if (m_ppixelshader)
      {

         pgpucontext->m_pcontext->PSSetShader(nullptr, nullptr, 0);

      }

      if (prenderer)
      {

         ::cast < render_target_view > pgpurendertargetview = prenderer->m_pgpurendertarget;

         if (pgpurendertargetview)
         {

            ::cast < texture > ptexture = pgpurendertargetview->current_texture(::gpu::current_frame());

            //::cast < offscreen_render_target_view > poffscreenrendertargetview = pgpurendertargetview;

            if (ptexture)
            {

               auto pshaderresourceview = ptexture->m_pshaderresourceview;

               if (pshaderresourceview)
               {

                  ID3D11ShaderResourceView* nullSRV[1] = { nullptr };

                  pgpucontext->m_pcontext->PSSetShaderResources(0, 1, nullSRV);

               }

            }

         }

      }

   }


   void shader::push_properties()
   {

      ::cast < renderer > prenderer = m_pgpurenderer;

      //   auto commandBuffer = prenderer->getCurrentCommandBuffer();

      //   vkCmdPushConstants(
      //      commandBuffer,
      //      m_vkpipelinelayout,
      //      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
      //      0,
      //      m_properties.size(),
      //      m_properties.data());

      if (m_propertiesPush.size() <= 0)
      {

         return;

      }

      ::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast <device> pgpudevice = pgpucontext->m_pgpudevice;

      auto iSetSize = m_propertiesPush.size();

      if (iSetSize != m_iSizePushConstants || !m_pbufferPushConstants)
      {

         m_pbufferPushConstants.Release();

         D3D11_BUFFER_DESC cbDesc = {};
         cbDesc.ByteWidth = (m_propertiesPush.size() + 15) & ~15;
         cbDesc.Usage = D3D11_USAGE_DYNAMIC;
         cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
         cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

         auto pdevice = pgpudevice->m_pdevice;

         HRESULT hr = pdevice->CreateBuffer(&cbDesc, nullptr, &m_pbufferPushConstants);

         if (FAILED(hr))
         {

            throw ::hresult_exception(hr);

         }

         m_iSizePushConstants = m_propertiesPush.size();



      }

      //PushConstants pc = { XMFLOAT4(1, 0, 0, 1), currentTime };

      D3D11_MAPPED_SUBRESOURCE mapped;
      HRESULT hrMap = pgpucontext->m_pcontext->Map(m_pbufferPushConstants, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

      if (FAILED(hrMap))
      {

         throw ::hresult_exception(hrMap);

      }
      
      memcpy(mapped.pData, m_propertiesPush.data(), m_propertiesPush.size());
      pgpucontext->m_pcontext->Unmap(m_pbufferPushConstants, 0);


      pgpucontext->m_pcontext->VSSetConstantBuffers(0, 1, pgpucontext->m_pbufferGlobalUbo.pp());
      pgpucontext->m_pcontext->PSSetConstantBuffers(0, 1, pgpucontext->m_pbufferGlobalUbo.pp());


      auto pVS = m_pbufferPushConstants.m_p;
      pgpucontext->m_pcontext->VSSetConstantBuffers(1, 1, &pVS);
      auto pPS = m_pbufferPushConstants.m_p;
      pgpucontext->m_pcontext->PSSetConstantBuffers(1, 1, &pPS);



   }


   void shader::draw()
   {

      ::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;
      pgpucontext->m_pcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      pgpucontext->m_pcontext->Draw(6, 0);
   //   ::cast < renderer > prenderer = m_pgpurenderer;

   //   auto commandBuffer = prenderer->getCurrentCommandBuffer();

   //   vkCmdDraw(commandBuffer, 6, 1, 0, 0);

   }


} // namespace gpu_directx11



//#include "shader.h"
//
//
//namespace gpu_directx11
//{
//
//
//   const char* shader_type_c_str(VkShaderStageFlagBits etype)
//   {
//
//      switch (etype)
//      {
//      case VK_SHADER_STAGE_VERTEX_BIT:
//         return "VERTEX";
//      case VK_SHADER_STAGE_FRAGMENT_BIT:
//         return "FRAGMENT";
//#if !defined(__APPLE__) && !defined(__ANDROID__)
//      case VK_SHADER_STAGE_GEOMETRY_BIT:
//         return "GEOMETRY";
//#endif
//      default:
//         return "(Unknown Shader Type)";
//      }
//
//   }
//
//
//   shader::shader()
//   {
//
//
//   }
//
//
//   shader::~shader()
//   {
//
//
//   }
//
//
//   unsigned int shader::create_shader(const ::string & strSource, VkShaderStageFlagBits type)
//   {
//
//      //unsigned int uShader;
//
//      //uShader = glCreateShader(type);
//
//      //if(!uShader)
//      //{
//
//      //   GLenum eerror = glGetError();
//      //   
//      //   auto errString = directx11_error_string(eerror);
//
//      //   informationf("error %d \"%s\"", eerror, errString);
//
//      //   return ::error_failed;
//
//      //}
//
//      //const char * sza[1];
//
//      //sza[0] = strSource;
//
//      //glShaderSource(uShader, 1, sza, NULL);
//
//      //glCompileShader(uShader);
//
//      //information() << "compiling shader : " << sza[0];
//
//      ////string strSummary;
//
//      ////::e_status estatus =
//      //
//      //shader_compile_errors(uShader, type, m_strError);
//
//      ////if (!estatus)
//      ////{
//
//      ////   return 0;
//
//      ////}
//      //m_strError.empty();
//
//      //return uShader;
//
//      return 0;
//
//
//   }
//
//
//   void shader::create_shader(const ::string & pszVertex, const ::string & pszFragment, const ::string & pszGeometry)
//   {
//
//      unsigned int uVertex = create_shader(pszVertex, VK_SHADER_STAGE_VERTEX_BIT);
//
//      unsigned int uFragment = create_shader(pszFragment, VK_SHADER_STAGE_FRAGMENT_BIT);
//      
//#if !defined(__APPLE__) && !defined(__ANDROID__)
//
//      unsigned int uGeometry = 0;
//
//      bool bGeometry = pszGeometry.trimmed().has_character();
//
//      if (bGeometry)
//      {
//
//         uGeometry = create_shader(pszGeometry, VK_SHADER_STAGE_GEOMETRY_BIT);
//
//      }
//      
//#endif
//
////      m_uId = glCreateProgram();
////
////      glAttachShader(m_uId, uVertex);
////
////      glAttachShader(m_uId, uFragment);
////      
////#if !defined(__APPLE__) && !defined(__ANDROID__)
////
////      if (bGeometry)
////      {
////
////         glAttachShader(m_uId, uGeometry);
////
////      }
////      
////#endif
////
////      glLinkProgram(m_uId);
////      
////      // Validate program
//////      glValidateProgram(m_uId);
////
////      string strSummary;
////
////      ///::e_status estatus =
////      
////      program_compile_errors(m_uId, strSummary);
////
////      //if(estatus.succeeded())
////      //{
////
////      //   return estatus;
////
////      //}
////
////      // delete the shaders
////      // they're linked into our program
////
////      glDeleteShader(uVertex);
////
////      glDeleteShader(uFragment);
////
////#if !defined(__APPLE__) && !defined(__ANDROID__)
////
////      if (bGeometry)
////      {
////
////         glDeleteShader(uGeometry);
////
////      }
////      
////#endif
////
//      //return ::success;
//
//   }
//
//
//   // activate the shader
//   // ------------------------------------------------------------------------
//   void shader::use()
//   {
//      
//      //glUseProgram(m_uId);
//      
//   }
//
//
//   void shader::setBool(const ::scoped_string & scopedstrName, bool b)
//   {
//
//      setInt(scopedstrName, b != false);
//
//      //GLint i = glGetUniformLocation(m_uId, pszName);
//
//      //glUniform1i(i, (int)value);
//
//   }
//
//
//   void shader::setInt(const ::scoped_string & scopedstrName, int i)
//   {
//
//      auto p = get_payload(scopedstrName);
//
//      if(!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, &i, sizeof(int));
//
//      //GLint i = glGetUniformLocation(m_uId, pszName);
//
//      //glUniform1i(i, value);
//
//      //int i = value != 0;
//      //m_memoryPushConstants.append(&i, sizeof(int));
//
//
//   }
//
//
//   void shader::setFloat(const ::scoped_string & scopedstrName, float f)
//   {
//
//      //GLint i = glGetUniformLocation(m_uId, pszName);
//
//      //glUniform1f(i, value);
//      //int i = value != 0;
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, &f, sizeof(float));
//
//   }
//
//
////   void shader::setVec2(const ::scoped_string & scopedstrName, const glm::vec2& value)
////   {
////
////      GLint i = glGetUniformLocation(m_uId, pszName);
////
////      glUniform2fv(i, 1, &value[0]);
////
////   }
//
//
//   void shader::setVec2(const ::scoped_string & scopedstrName, float x, float y)
//   {
//
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, &x, sizeof(float));
//      m_memoryPushConstants.set_at(p->m_iOffset + sizeof(float), &y, sizeof(float));
//
//   }
//
//
////   void shader::setVec3(const ::scoped_string & scopedstrName, const glm::vec3& value)
////   {
////
////      GLint i = glGetUniformLocation(m_uId, pszName);
////
////      glUniform3fv(i, 1, &value[0]);
////
////   }
//
//
//   void shader::setVec3(const ::scoped_string & scopedstrName, float x, float y, float z)
//   {
//      
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, &x, sizeof(float));
//      m_memoryPushConstants.set_at(p->m_iOffset + sizeof(float), &y, sizeof(float));
//      m_memoryPushConstants.set_at(p->m_iOffset + 2*sizeof(float), &z, sizeof(float));
//      
//   }
//
//
////   void shader::setVec4(const ::scoped_string & scopedstrName, const glm::vec4& value)
////   {
////
////      GLint i = glGetUniformLocation(m_uId, pszName);
////
////      glUniform4fv(i, 1, &value[0]);
////
////   }
//
//
//   void shader::setVec4(const ::scoped_string & scopedstrName, float x, float y, float z, float w)
//   {
//
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, &x, sizeof(float));
//      m_memoryPushConstants.set_at(p->m_iOffset + sizeof(float), &y, sizeof(float));
//      m_memoryPushConstants.set_at(p->m_iOffset + 2 * sizeof(float), &z, sizeof(float));
//      m_memoryPushConstants.set_at(p->m_iOffset + 3 * sizeof(float), &w, sizeof(float));
//
//   }
//
//
//   void shader::setMat2(const ::scoped_string & scopedstrName, const float a[2*2])
//   {
//
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, (void *) a, 2*2*sizeof(float));
//
//   }
//
//
//   void shader::setMat3(const ::scoped_string & scopedstrName, const float a[3*3])
//   {
//
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, a, 3 * 3 * sizeof(float));
//
//   }
//
//
//   void shader::setMat4(const ::scoped_string & scopedstrName, const float a[4*4])
//   {
//
//      auto p = get_payload(scopedstrName);
//
//      if (!p)
//      {
//
//         throw ::exception(error_not_found, "not known property");
//
//      }
//
//      m_memoryPushConstants.set_at(p->m_iOffset, a, 4 * 4 * sizeof(float));
//
//   }
//
//
//   void shader::shader_compile_errors(int shader, VkShaderStageFlagBits type, string & strSummary)
//   {
//
//      //::e_status estatus = ::success;
//
//      //GLint success = 0;
//
//      //GLchar infoLog[1024];
//
//      //glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//
//      //if (success)
//      //{
//
//      //   strSummary.formatf("SUCCESS::SHADER_COMPILATION (%s) \n -- --------------------------------------------------- -- \n", shader_type_c_str(type));
//
//      //}
//      //else
//      //{
//
//      //   glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
//
//      //   const char * psz = shader_type_c_str(type);
//
//      //   const char * pszLog = infoLog;
//
//      //   strSummary.formatf("error::SHADER_COMPILATION_ERROR of type: %s \n %s \n -- --------------------------------------------------- -- \n", psz, pszLog);
//
//      //   warning() << strSummary;
//
//      //   throw ::exception(error_failed, "Shader Compilation Error", strSummary);
//
//      //}
//
//      //information() << strSummary;
//
//      ////return estatus;
//
//   }
//
//
//   void shader::program_compile_errors(int program, string & strSummary)
//   {
//
//      ////::e_status estatus = ::success;
//
//      //GLint success;
//
//      //GLchar infoLog[1024];
//      //
//      //glGetProgramiv(program, GL_LINK_STATUS, &success);
//
//      //if (success)
//      //{
//
//      //   strSummary.formatf("SUCCESS::PROGRAM_LINKING \n -- --------------------------------------------------- -- \n");
//
//      //}
//      //else
//      //{
//
//      //   glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
//
//      //   strSummary.formatf("error::PROGRAM_LINKING_ERROR : \n %s \n -- --------------------------------------------------- -- \n", infoLog);
//
//      //   throw ::exception(error_failed);
//
//      //}
//
//      //information() << strSummary;
//
//      ////return estatus;
//
//   }
//
//
//   //::gpu::payload * shader::get_payload(const ::scoped_string & scopedstrPayload)
//   //{
//   //   
//   //   auto uniform = glGetUniformLocation(m_uId, strUniform);
//
//   //   return uniform;
//
//   //}
//
//
//} // namespace gpu_directx11
//
//
//
