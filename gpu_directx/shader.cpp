#include "framework.h"
// Co-creating with V0idsEmbrace@Twitch with
// camilo on 2025-05-19 04:59 <3ThomasBorregaardSorensen!!
#include "approach.h"
#include "shader.h"
#include "context.h"
#include "descriptors.h"
#include "renderer.h"
#include "pipeline.h"
#include "renderer.h"
//#include "cube/impact.h"


namespace gpu_directx
{


   shader::shader()
   {

      m_vktopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

   }


   shader::~shader()
   {

      ::cast < context > pgpucontext = m_pgpurenderer->m_pgpucontext;

      vkDestroyPipelineLayout(pgpucontext->logicalDevice(), m_vkpipelinelayout, nullptr);

   }


   void shader::_create_pipeline_layout(int iSize)
   {

      ::cast < context > pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast < device > pgpudevice = pgpucontext->m_pgpudevice;

      VkPushConstantRange pushConstantRange{};
      pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
      pushConstantRange.offset = 0;
      //pushConstantRange.size = sizeof(PointLightPushConstants);
      pushConstantRange.size = iSize;

      ::array<VkDescriptorSetLayout> descriptorSetLayouts;

      if (m_edescriptorsetslota.contains(e_descriptor_set_slot_global))
      {

         auto globalSetLayout = pgpucontext->m_psetdescriptorlayoutGlobal->getDescriptorSetLayout();

         descriptorSetLayouts.add(globalSetLayout);

      }

      if (m_pLocalDescriptorSet)
      {

         ::cast < ::gpu_directx::set_descriptor_layout > pset = m_pLocalDescriptorSet;

         auto setLayout = pset->getDescriptorSetLayout();

         descriptorSetLayouts.add(setLayout);

      }

      VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
      pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
      pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();

      if (iSize > 0)
      {
         pipelineLayoutInfo.pushConstantRangeCount = 1;
         pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
      }
      else
      {
         pipelineLayoutInfo.pushConstantRangeCount = 0;
         pipelineLayoutInfo.pPushConstantRanges = NULL;


      }

      //pipelineLayoutInfo.pPushConstantRanges = nullptr;

      if (vkCreatePipelineLayout(
         pgpucontext->logicalDevice(),
         &pipelineLayoutInfo,
         nullptr,
         &m_vkpipelinelayout) !=
         VK_SUCCESS)
      {

         throw ::exception(error_failed, "failed to create pipeline layout!");

      }

   }


   void shader::on_initialize_shader()
   {

      _create_pipeline_layout(m_properties.m_memory.size());

      __construct_new(m_ppipeline);

      ::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast <device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast <renderer> prenderer = m_pgpurenderer;

      PipelineConfigInfo pipelineConfig{};

      ::cast < shader_vertex_input > pshadervertexinput = m_pVertexInput;

      if (pshadervertexinput)
      {

         pipelineConfig.bindingDescriptions.copy(pshadervertexinput->m_bindings);
         pipelineConfig.attributeDescriptions.copy(pshadervertexinput->m_attribs);

      }

      pipeline::defaultPipelineConfigInfo(pipelineConfig);

      if (m_eflag & e_flag_clear_default_bindings_and_attributes_descriptions)
      {

         pipelineConfig.attributeDescriptions.clear();
         pipelineConfig.bindingDescriptions.clear();

      }
      if (m_bDepthTestButNoDepthWrite)
      {
         pipelineConfig.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
         pipelineConfig.depthStencilInfo.depthTestEnable = VK_TRUE;
         pipelineConfig.depthStencilInfo.depthWriteEnable = VK_FALSE;
         pipelineConfig.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
            pipelineConfig.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
         pipelineConfig.depthStencilInfo.stencilTestEnable = VK_FALSE;


      }
      else if (m_bDisableDepthTest)
      {

         pipelineConfig.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
         pipelineConfig.depthStencilInfo.depthTestEnable = VK_FALSE;
         pipelineConfig.depthStencilInfo.depthWriteEnable = VK_FALSE;
         pipelineConfig.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_ALWAYS; // doesn't matter since test is disabled
         pipelineConfig.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
         pipelineConfig.depthStencilInfo.stencilTestEnable = VK_FALSE;

      }
      pipelineConfig.colorBlendAttachments.clear();
      if (m_bAccumulationEnable)
      {

         VkPipelineColorBlendAttachmentState state;
         state.blendEnable = VK_TRUE;
         state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // use alpha blending
         state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
         state.colorBlendOp = VK_BLEND_OP_ADD;
         state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
         state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
         state.alphaBlendOp = VK_BLEND_OP_ADD;
         state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
         pipelineConfig.colorBlendAttachments.add(state);
         if (m_iColorAttachmentCount > 1)
         {

            state.blendEnable = VK_TRUE;
            state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // use alpha blending
            state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
            state.colorBlendOp = VK_BLEND_OP_ADD;
            state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            state.alphaBlendOp = VK_BLEND_OP_ADD;
            state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT;
            pipelineConfig.colorBlendAttachments.add(state);
         }

         pipelineConfig.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
         pipelineConfig.colorBlendInfo.logicOpEnable = VK_FALSE;
         pipelineConfig.colorBlendInfo.attachmentCount = pipelineConfig.colorBlendAttachments.get_count();
         pipelineConfig.colorBlendInfo.pAttachments = pipelineConfig.colorBlendAttachments.data();

      }
      else if (m_bEnableBlend)
      {

         for (int i = 0; i < m_iColorAttachmentCount; i++)
         {
            VkPipelineColorBlendAttachmentState state;
            state.blendEnable = VK_TRUE;
            state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
            state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            state.colorBlendOp = VK_BLEND_OP_ADD;
            state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            state.alphaBlendOp = VK_BLEND_OP_ADD;
            //state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // use alpha blending
            //state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            //state.colorBlendOp = VK_BLEND_OP_ADD;
            //state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            //state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
            //state.alphaBlendOp = VK_BLEND_OP_ADD;
            state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
               VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            pipelineConfig.colorBlendAttachments.add(state);
         }

         pipelineConfig.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
         pipelineConfig.colorBlendInfo.logicOpEnable = VK_FALSE;
         pipelineConfig.colorBlendInfo.attachmentCount = pipelineConfig.colorBlendAttachments.get_count();
         pipelineConfig.colorBlendInfo.pAttachments = pipelineConfig.colorBlendAttachments.data();

      }
      else
      {

         for (int i = 0; i < m_iColorAttachmentCount; i++)
         {
            VkPipelineColorBlendAttachmentState state;
            state.colorWriteMask =
               VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
               VK_COLOR_COMPONENT_A_BIT;
            state.blendEnable = VK_FALSE;
            state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
            state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
            state.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
            state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
            state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
            state.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional
            pipelineConfig.colorBlendAttachments.add(state);
         }

         pipelineConfig.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
         pipelineConfig.colorBlendInfo.logicOpEnable = VK_FALSE;
         pipelineConfig.colorBlendInfo.attachmentCount = pipelineConfig.colorBlendAttachments.get_count();
         pipelineConfig.colorBlendInfo.pAttachments = pipelineConfig.colorBlendAttachments.data();

      }

      pipelineConfig.inputAssemblyInfo.topology = m_vktopology;
      pipelineConfig.dynamicStateEnables.append_unique(m_dynamicstateaEnable);
      pipelineConfig.dynamicStateInfo.dynamicStateCount = pipelineConfig.dynamicStateEnables.size();
      auto prenderpass = prenderer->m_pvkcrenderpass;
      pipelineConfig.renderPass = prenderpass->m_vkrenderpass;
      pipelineConfig.pipelineLayout = m_vkpipelinelayout;

      pgpudevice->defer_shader_memory(m_memoryVertex, m_pathVertex);
      pgpudevice->defer_shader_memory(m_memoryFragment, m_pathFragment);

      m_ppipeline->initialize_pipeline(m_pgpurenderer,
         m_memoryVertex,
         m_memoryFragment,
         pipelineConfig);


   }


   void shader::bind()
   {

      ::cast <context> pgpucontext = m_pgpurenderer->m_pgpucontext;

      ::cast <device> pgpudevice = pgpucontext->m_pgpudevice;

      ::cast <renderer> prenderer = m_pgpurenderer;

      auto commandBuffer = prenderer->getCurrentCommandBuffer();

      m_ppipeline->bind(commandBuffer);

      if (m_edescriptorsetslota.contains(e_descriptor_set_slot_global))
      {

         auto globalDescriptorSet = pgpucontext->getGlobalDescriptorSet(prenderer);

         vkCmdBindDescriptorSets(
            commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_vkpipelinelayout,
            0,
            1,
            &globalDescriptorSet,
            0,
            nullptr);

      }

   }


   void shader::push_properties()
   {

      ::cast < renderer > prenderer = m_pgpurenderer;

      auto commandBuffer = prenderer->getCurrentCommandBuffer();

      vkCmdPushConstants(
         commandBuffer,
         m_vkpipelinelayout,
         VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
         0,
         m_properties.size(),
         m_properties.data());

   }


   void shader::draw()
   {

      ::cast < renderer > prenderer = m_pgpurenderer;

      auto commandBuffer = prenderer->getCurrentCommandBuffer();

      vkCmdDraw(commandBuffer, 6, 1, 0, 0);

   }


} // namespace gpu_directx



//#include "shader.h"
//
//
//namespace gpu_directx
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
//      //   auto errString = directx_error_string(eerror);
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
//} // namespace gpu_directx
//
//
//
