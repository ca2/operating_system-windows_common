// From github:/tristancalderbank/OpenGL-PBR-Renderer/mesh.h by
// camilo on 2025-09-26 18:28 <3ThomasBorregaardSorensen!!
// From gpu_directx11 by camilo on 2025-10-16 16:44 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "mesh.h"
#include "bred/gltf/vertex.h"
#include "gpu_directx11/context.h"
#include "gpu_directx11/texture.h"
#include "bred/gpu/command_buffer.h"
#include "bred/gpu/context.h"
#include "bred/gpu/model_buffer.h"
#include "bred/gpu/render_target.h"
#include "bred/gpu/renderer.h"
#include "bred/graphics3d/render_system/pbr_with_ibl_render_system_base.h"
#include "bred/graphics3d/render_system.h"
#include "bred/graphics3d/scene_renderable.h"


//#include <glad/glad.h>


namespace gpu_directx11
{


   namespace gltf
   {


      mesh::mesh()
      {


      }


      mesh::~mesh()
      {


      }


      //void mesh::initialize_gpu_gltf_mesh(const ::array_base<::gpu::gltf::vertex> &vertexa,
      //                             const ::unsigned_int_array &indexa, ::gpu::model::material *pmaterial)
      //{

      //   m_vertexa = vertexa;
      //   m_indexa = indexa;
      //   m_pmaterial = pmaterial;

      //   init();

      //}


      void mesh::draw2(::gpu::command_buffer *pcommandbuffer)
      {
         ::cast<::gpu_directx11::context> pcontext = m_pgpucontext;
         //
         // glActiveTexture(GL_TEXTURE0);
         //
         // // draw mesh
         // glBindVertexArray(mVAO);
         // glDrawElements(GL_TRIANGLES, m_indexa.size(), GL_UNSIGNED_INT, 0);
         // glBindVertexArray(0);

         auto pshader = pcommandbuffer->m_pgpurendertarget->m_pgpurenderer->m_pgpucontext->m_pshaderBound;
         auto erendersystem = pcommandbuffer->m_prendersystem->m_erendersystem;
         if (erendersystem == ::graphics3d::e_render_system_skybox_ibl)
         {


         }
         else  if (erendersystem == ::graphics3d::e_render_system_gltf_ibl)
         {
            // albedo
            ::cast<::graphics3d::pbr_with_ibl_render_system_base> prendersystem = pcommandbuffer->m_prendersystem;

            auto pscenerenderable = prendersystem->current_scene_renderable();

            auto m1 = pscenerenderable->model_matrix();

            auto m2 = uniformBlock.matrix;

            auto pszName = pscenerenderable->m_strName.c_str();

            //// Remap Z (linear transformation)
            //projD3D[2][0] *= 0.5f; // column-major indexing: projD3D[col][row]
            //projD3D[2][1] *= 0.5f;
            //projD3D[2][2] = projD3D[2][2] * 0.5f + 0.5f;
            //projD3D[2][3] = projD3D[2][3] * 0.5f + 0.5f;

            // -------------------------
            // Step 2: Handedness flip if needed (right-handed -> left-handed)
            // -------------------------
            //floating_matrix4 handednessFlip = glm::scale(floating_matrix4(1.0f), floating_sequence3(1.0f, 1.0f, -1.0f));
            //modelMatrixGL = modelMatrixGL * handednessFlip;
            //normalMatrixGL = normalMatrixGL * handednessFlip;


                        floating_matrix4 world =  m1*m2 ;
            //world = world * handednessFlip;
                        pshader->set_matrix4("modelMatrix", world);
                        floating_matrix4 normalMat = floating_matrix4(floating_matrix3(world).inversed());
                        // normalMat = normalMat * handednessFlip;
            pshader->set_matrix4("normalMatrix", normalMat);


            bool bAlbedo = !prendersystem->m_bDisableAlbedo && m_pmaterial->useTextureAlbedo;
            pshader->set_int("useTextureAlbedo", bAlbedo);
            floating_sequence3 seq3Albedo= {};
            if (prendersystem->m_bForceDefaultAlbedo)
            {

               seq3Albedo = prendersystem->m_seq3DefaultAlbedo;
            }
            else
            {

               seq3Albedo = m_pmaterial->m_seq3Albedo;
            }

            pshader->set_sequence3("albedo", seq3Albedo);
            if (bAlbedo)
            {
               int iTextureIndex = ::gpu::e_gltf_texture_albedo;
               //GLenum textureIndex = GL_TEXTURE0 + iTextureIndex;
               //glActiveTexture(textureIndex);
               //pshader->set_int("material.textureAlbedo", iTextureIndex);

               //::cast<::gpu_directx11::texture> ptextureAlbedo = m_pmaterial->m_ptextureAlbedo;
               //glBindTexture(GL_TEXTURE_2D, ptextureAlbedo->m_gluTextureID);
            }

            bool bMetallicRoughness =
               !prendersystem->m_bDisableMetallicRoughness && m_pmaterial->useTextureMetallicRoughness;

            pshader->set_int("useTextureMetallicRoughness", bMetallicRoughness);
            float fMetallic = 0.0f;
            if (prendersystem->m_bForceDefaultMetallicFactor)
            {

               fMetallic = prendersystem->m_fDefaultMetallicFactor;

            }
            else
            {

               fMetallic = m_pmaterial->m_fMetallic;

            }
            float fRoughness = 0.0f;
            if (prendersystem->m_bForceDefaultRoughnessFactor)
            {

               fRoughness = prendersystem->m_fDefaultRoughnessFactor;
            }
            else
            {

               fRoughness = m_pmaterial->m_fRoughness;
            }
            pshader->set_float("metallic", fMetallic);
            pshader->set_float("roughness", fRoughness);
            if (bMetallicRoughness)
            {
               int iTextureIndex = ::gpu::e_gltf_texture_metallic_roughness;
               //GLenum textureIndex = GL_TEXTURE0 + iTextureIndex;
               //glActiveTexture(textureIndex);
               //pshader->set_int("material.textureMetallicRoughness", iTextureIndex);
               //::cast<::gpu_directx11::texture> ptextureMetallicRoughness = m_pmaterial->m_ptextureMetallicRoughness;
               //glBindTexture(GL_TEXTURE_2D, ptextureMetallicRoughness->m_gluTextureID);
            }

            bool bNormal = !prendersystem->m_bDisableNormal && m_pmaterial->useTextureNormal;
            pshader->set_int("useTextureNormal", bNormal);
            if (bNormal)
            {
               //glActiveTexture(GL_TEXTURE0 + ::gpu::e_gltf_texture_normal);
               //pshader->set_int("material.textureNormal", ::gpu::e_gltf_texture_normal);
               //::cast<::gpu_directx11::texture> ptextureNormal = m_pmaterial->m_ptextureNormal;
               //glBindTexture(GL_TEXTURE_2D, ptextureNormal->m_gluTextureID);
            }

            bool bAmbientOcclusion =
               !prendersystem->m_bDisableAmbientOcclusion && m_pmaterial->useTextureAmbientOcclusion;
            pshader->set_int("useTextureAmbientOcclusion", bAmbientOcclusion);
            float fAmbientOcclusion = 0.0f;
            if (prendersystem->m_bForceDefaultAmbientOcclusionFactor)
            {

               fAmbientOcclusion = prendersystem->m_fDefaultAmbientOcclusionFactor;
            }
            else
            {

               fAmbientOcclusion = m_pmaterial->m_fAmbientOcclusion;
            }
            pshader->set_float("ambientOcclusion", fAmbientOcclusion);
            if (bAmbientOcclusion)
            {
               //glActiveTexture(GL_TEXTURE0 + ::gpu::e_gltf_texture_ambient_occlusion);
               //pshader->set_int("material.textureAmbientOcclusion", ::gpu::e_gltf_texture_ambient_occlusion);
               //::cast<::gpu_directx11::texture> ptextureAmbientOcclusion = m_pmaterial->m_ptextureAmbientOcclusion;
               //glBindTexture(GL_TEXTURE_2D, ptextureAmbientOcclusion->m_gluTextureID);
            }

            bool bEmissive = !prendersystem->m_bDisableEmissive && m_pmaterial->useTextureEmissive;
            pshader->set_int("useTextureEmissive", bEmissive);
            floating_sequence3 seq3Emission = {};
            if (prendersystem->m_bForceDefaultEmission)
            {

               seq3Emission = prendersystem->m_seq3DefaultEmission;
            }
            else
            {

               seq3Emission = m_pmaterial->m_seq3Emissive;
            }
            pshader->set_sequence3("emissive", seq3Emission);
            if (bEmissive)
            {
               //glActiveTexture(GL_TEXTURE0 + ::gpu::e_gltf_texture_emissive);
               //pshader->set_int("material.textureEmissive", ::gpu::e_gltf_texture_emissive);
               //::cast<::gpu_directx11::texture> ptextureEmissive = m_pmaterial->m_ptextureEmissive;
               //glBindTexture(GL_TEXTURE_2D, ptextureEmissive->m_gluTextureID);
            }

            ID3D11SamplerState *sampler = nullptr;
            ID3D11ShaderResourceView *srv[5] = {};
                     // // albedo
                     // 
            // shader.setBool("material.useTextureAlbedo", m_pmaterial->useTextureAlbedo);
            //pshader->set_sequence3("albedo", m_pmaterial->m_seq3Albedo);
            if (bAlbedo)
            {
               ::cast<::gpu_directx11::texture> ptexture = m_pmaterial->m_textureaPbr[::gpu::model::e_texture_albedo];
               if (ptexture)
               {
                  srv[0] = ptexture->m_pshaderresourceview;
                  //pcontext->m_pcontext->PSSetShaderResources(3, 1, srv);
                  // glActiveTexture(GL_TEXTURE0 + e_gltf_texture_albedo);
                  // shader.setInt("material.textureAlbedo", e_gltf_texture_albedo);
                  // glBindTexture(GL_TEXTURE_2D, m_pmaterial->textureAlbedo->mId);
                  if (!sampler && ptexture->m_psamplerstate)
                  {
                     sampler = ptexture->m_psamplerstate;
                  }
               }
            }
            //
            // shader.setBool("material.useTextureMetallicRoughness", m_pmaterial->useTextureMetallicRoughness);
            //pshader->set_float("metallic", m_pmaterial->m_fMetallic);
            //pshader->set_float("roughness", m_pmaterial->m_fRoughness);
            if (bMetallicRoughness)
            {
               // glActiveTexture(GL_TEXTURE0 + e_gltf_texture_metallic_roughness);
               // shader.setInt("material.textureMetallicRoughness", e_gltf_texture_metallic_roughness);
               // glBindTexture(GL_TEXTURE_2D, m_pmaterial->textureMetallicRoughness->mId);
               ::cast<::gpu_directx11::texture> ptexture = m_pmaterial->m_textureaPbr[::gpu::model::e_texture_metallic_roughness];
               if (ptexture)
               {
                  srv[1] = ptexture->m_pshaderresourceview;
                  //pcontext->m_pcontext->PSSetShaderResources(4, 1, srv);
                  // glActiveTexture(GL_TEXTURE0 + e_gltf_texture_albedo);
                  // shader.setInt("material.textureAlbedo", e_gltf_texture_albedo);
                  // glBindTexture(GL_TEXTURE_2D, m_pmaterial->textureAlbedo->mId);
               }
            }
            //
            // shader.setBool("material.useTextureNormal", m_pmaterial->useTextureNormal);
            if (bNormal)
            {
               //    glActiveTexture(GL_TEXTURE0 + e_gltf_texture_normal);
               //    shader.setInt("material.textureNormal", e_gltf_texture_normal);
               //    glBindTexture(GL_TEXTURE_2D, m_pmaterial->textureNormal->mId);
               ::cast<::gpu_directx11::texture> ptexture = m_pmaterial->m_textureaPbr[::gpu::model::e_texture_normal];
               if (ptexture)
               {
                  srv[2] = ptexture->m_pshaderresourceview;
                  //pcontext->m_pcontext->PSSetShaderResources(5, 1, srv);
                  // glActiveTexture(GL_TEXTURE0 + e_gltf_texture_albedo);
                  // shader.setInt("material.textureAlbedo", e_gltf_texture_albedo);
                  // glBindTexture(GL_TEXTURE_2D, m_pmaterial->textureAlbedo->mId);
               }
            }
            //
            // shader.setBool("material.useTextureAmbientOcclusion", m_pmaterial->useTextureAmbientOcclusion);
            //pshader->set_float("material.ambientOcclusion", m_pmaterial->m_fAmbientOcclusion);
            if (bAmbientOcclusion)
            {
               //    glActiveTexture(GL_TEXTURE0 + e_gltf_texture_ambient_occlusion);
               //    shader.setInt("material.textureAmbientOcclusion", e_gltf_texture_ambient_occlusion);
               //    glBindTexture(GL_TEXTURE_2D, m_pmaterial->textureAmbientOcclusion->mId);
               ::cast<::gpu_directx11::texture> ptexture = m_pmaterial->m_textureaPbr[::gpu::model::e_texture_ambient_occlusion];
               if (ptexture)
               {
                  srv[3] = ptexture->m_pshaderresourceview;
                  //pcontext->m_pcontext->PSSetShaderResources(6, 1, srv);
                  // glActiveTexture(GL_TEXTURE0 + e_gltf_texture_albedo);
                  // shader.setInt("material.textureAlbedo", e_gltf_texture_albedo);
                  // glBindTexture(GL_TEXTURE_2D, m_pmaterial->textureAlbedo->mId);
               }
            }
            //
            // shader.setBool("material.useTextureEmissive", m_pmaterial->useTextureEmissive);
            // shader.setVec3("material.emissive", m_pmaterial->emissive);
            if (bEmissive)
            {
               //    glActiveTexture(GL_TEXTURE0 + e_gltf_texture_emissive);
               //    shader.setInt("material.textureEmissive", e_gltf_texture_emissive);
               //    glBindTexture(GL_TEXTURE_2D, m_pmaterial->textureEmissive->mId);
               ::cast<::gpu_directx11::texture> ptexture = m_pmaterial->m_textureaPbr[::gpu::model::e_texture_emissive];
               if (ptexture)
               {
                  srv[4] = ptexture->m_pshaderresourceview;
                  //pcontext->m_pcontext->PSSetShaderResources(7, 1, srv);
                  // glActiveTexture(GL_TEXTURE0 + e_gltf_texture_albedo);
                  // shader.setInt("material.textureAlbedo", e_gltf_texture_albedo);
                  // glBindTexture(GL_TEXTURE_2D, m_pmaterial->textureAlbedo->mId);
               }
            }
            pshader->push_properties(pcommandbuffer);

            //            glActiveTexture(GL_TEXTURE0);
            if (sampler)
            {
               ID3D11SamplerState *samplers[1] = {sampler};
               pcontext->m_pcontext->PSSetSamplers(1, 1, samplers);
               pcontext->m_pcontext->PSSetShaderResources(3, 5, srv);
            }

         }
         else if (erendersystem == ::graphics3d::e_render_system_gltf_scene)
         {
            // albedo
                        //;
            //pshader->set_int("material.useTextureAlbedo", m_pmaterial->useTextureAlbedo);
            //pshader->set_sequence3("material.albedo", m_pmaterial->m_seq3Albedo);
            if (m_pmaterial->useTextureAlbedo)
            {
               int iTextureIndex = ::gpu::e_gltf_texture_albedo;
               //GLenum textureIndex = GL_TEXTURE0 + iTextureIndex;
               //glActiveTexture(textureIndex);
               //pshader->set_int("material.textureAlbedo", iTextureIndex);

               //::cast<::gpu_directx11::texture> ptextureAlbedo = m_pmaterial->m_ptextureAlbedo;
               //glBindTexture(GL_TEXTURE_2D, ptextureAlbedo->m_gluTextureID);
            }

            //pshader->set_int("material.useTextureMetallicRoughness", m_pmaterial->useTextureMetallicRoughness);
            //pshader->set_float("material.metallic", m_pmaterial->m_fMetallic);
            //pshader->set_float("material.roughness", m_pmaterial->m_fRoughness);
            //if (m_pmaterial->useTextureMetallicRoughness)
            //{
            //   int iTextureIndex = ::gpu::e_gltf_texture_metallic_roughness;
            //   GLenum textureIndex = GL_TEXTURE0 + iTextureIndex;
            //   glActiveTexture(textureIndex);
            //   pshader->set_int("material.textureMetallicRoughness", iTextureIndex);
            //   ::cast<::gpu_directx11::texture> ptextureMetallicRoughness = m_pmaterial->m_ptextureMetallicRoughness;
            //   glBindTexture(GL_TEXTURE_2D, ptextureMetallicRoughness->m_gluTextureID);
            //}

            pshader->set_int("material.useTextureNormal", m_pmaterial->useTextureNormal);
            if (m_pmaterial->useTextureNormal)
            {
               //glActiveTexture(GL_TEXTURE0 + ::gpu::e_gltf_texture_normal);
               //pshader->set_int("material.textureNormal", ::gpu::e_gltf_texture_normal);
               //::cast<::gpu_directx11::texture> ptextureNormal = m_pmaterial->m_ptextureNormal;
               //glBindTexture(GL_TEXTURE_2D, ptextureNormal->m_gluTextureID);
            }
            else
            {

               //information() << "are there gltf models without normal?";

            }

            //pshader->set_int("material.useTextureAmbientOcclusion", m_pmaterial->useTextureAmbientOcclusion);
            //pshader->set_float("material.ambientOcclusion", m_pmaterial->m_fAmbientOcclusion);
            //if (m_pmaterial->useTextureAmbientOcclusion)
            //{
            //   glActiveTexture(GL_TEXTURE0 + ::gpu::e_gltf_texture_ambient_occlusion);
            //   pshader->set_int("material.textureAmbientOcclusion", ::gpu::e_gltf_texture_ambient_occlusion);
            //   ::cast<::gpu_directx11::texture> ptextureAmbientOcclusion = m_pmaterial->m_ptextureAmbientOcclusion;
            //   glBindTexture(GL_TEXTURE_2D, ptextureAmbientOcclusion->m_gluTextureID);
            //}

            //pshader->set_int("material.useTextureEmissive", m_pmaterial->useTextureEmissive);
            //pshader->set_sequence3("material.emissive", m_pmaterial->m_seq3Emissive);
            //if (m_pmaterial->useTextureEmissive)
            //{
            //   glActiveTexture(GL_TEXTURE0 + ::gpu::e_gltf_texture_emissive);
            //   pshader->set_int("material.textureEmissive", ::gpu::e_gltf_texture_emissive);
            //   ::cast<::gpu_directx11::texture> ptextureEmissive = m_pmaterial->m_ptextureEmissive;
            //   glBindTexture(GL_TEXTURE_2D, ptextureEmissive->m_gluTextureID);
            //}

            //glActiveTexture(GL_TEXTURE0);



         }
         else
         {

            warning() << "unexpected enum_render_system in gpu_directx11::gltf::mesh::draw";

            throw ::exception(error_wrong_state, "unexpected enum_render_system in gpu_directx11::gltf::mesh::draw");

         }

         //   // draw mesh
         ////glBindVertexArray(m_uVAO);
         ////glDrawElements(GL_TRIANGLES, m_indexa.size(), GL_UNSIGNED_INT, 0);
         ////glBindVertexArray(0);
         //::cast<::gpu_directx11::context> pgpucontext = m_pgpucontext;
         //UINT stride = sizeof(::gpu::gltf::vertex);
         //UINT offset = 0;
         //pgpucontext->m_pcontext->IASetVertexBuffers(0, 1, m_pVertexBuffer.pp(), &stride, &offset);
         //pgpucontext->m_pcontext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
         ////pgpucontext->m_pcontext->IASetInputLayout(m_pInputLayout);
         //auto iIndexCount = m_pmodeldata->index_count();
         //pgpucontext->m_pcontext->DrawIndexed(iIndexCount, 0, 0);

         pcommandbuffer->draw(m_pmodelbuffer);

      }


      void mesh::on_initialize_gpu_mesh()
      {

         // Assume:
         // ID3D11Device*           m_pd3dDevice;
         // ID3D11DeviceContext*    m_pImmediateContext;
         // std::vector<::gpu::gltf::vertex> m_vertexa;
         // std::vector<unsigned int> m_indexa;

         //      ::cast<::gpu_directx11::context> pgpucontext = m_pgpucontext;

         //::cast<::gpu_directx11::device> pgpudevice = pgpucontext->m_pgpudevice;

         //auto pd3d11device = pgpudevice->m_pdevice;

         //// --- Create Vertex Buffer ---
         //D3D11_BUFFER_DESC bd = {};
         //bd.Usage = D3D11_USAGE_DEFAULT;
         //bd.ByteWidth = static_cast<UINT>(m_pmodeldata->vertex_bytes());
         //bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
         //bd.CPUAccessFlags = 0;

         //D3D11_SUBRESOURCE_DATA initData = {};
         //initData.pSysMem = m_pmodeldata->vertex_data().data();

         //HRESULT hr = pd3d11device->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
         //defer_throw_hresult(hr);
         ////if (FAILED(hr))
         ////{
         //  // // handle error
         ////}

         //// --- Create Index Buffer ---
         //bd.Usage = D3D11_USAGE_DEFAULT;
         //bd.ByteWidth = static_cast<UINT>(m_pmodeldata->index_bytes());
         //bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
         //bd.CPUAccessFlags = 0;
         //initData.pSysMem = m_pmodeldata->index_data().data();

         //hr = pd3d11device->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
         //defer_throw_hresult(hr);
         ////if (FAILED(hr))
         ////{
         ////   // handle error
         ////}

         ////// --- Create Input Layout ---
         ////// Define the vertex input layout matching your ::gpu::gltf::vertex structure
         ////D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
         ////   {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(::gpu::gltf::vertex, position),
         ////    D3D11_INPUT_PER_VERTEX_DATA, 0},
         ////   {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(::gpu::gltf::vertex, normal),
         ////    D3D11_INPUT_PER_VERTEX_DATA, 0},
         ////   {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(::gpu::gltf::vertex, uv), D3D11_INPUT_PER_VERTEX_DATA,
         ////    0},
         ////   {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(::gpu::gltf::vertex, color),
         ////    D3D11_INPUT_PER_VERTEX_DATA, 0},
         ////   {"TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(::gpu::gltf::vertex, tangent),
         ////    D3D11_INPUT_PER_VERTEX_DATA, 0},
         ////};

         ////// Assume you have a compiled vertex shader bytecode in "pVSBlob" with size "vsBlobSize"
         ////hr = pd3d11device->CreateInputLayout(layoutDesc, _countof(layoutDesc), 
         ////   pVSBlob->GetBufferPointer(),
         ////                                     pVSBlob->GetBufferSize(), &m_pInputLayout);
         ////if (FAILED(hr))
         ////{
         ////   // handle error
         ////}

         ////// --- Binding for rendering ---
         ////UINT stride = sizeof(::gpu::gltf::vertex);
         ////UINT offset = 0;
         ////m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
         ////m_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
         ////m_pImmediateContext->IASetInputLayout(m_pInputLayout);

         ////// Set prototype topology
         ////m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
         ////// // create our data structures
         ////// glGenVertexArrays(1, &mVAO);
         ////// glGenBuffers(1, &mVBO);
         ////// glGenBuffers(1, &mEBO);
         //////
         ////// glBindVertexArray(mVAO); // use this VAO for subsequent calls
         //////
         ////// glBindBuffer(GL_ARRAY_BUFFER, mVBO); // use this VBO for subsequent calls
         ////// glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(::gpu::gltf::vertex), &mVertices[0],
         //////              GL_STATIC_DRAW); // copy over the vertex data
         //////
         ////// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO); // use this EBO for subsequent calls
         ////// glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexa.size() * sizeof(unsigned int), &m_indexa[0],
         //////              GL_STATIC_DRAW); // copy over the index data
         //////
         ////// // setup the locations of vertex data
         ////// // positions
         ////// glEnableVertexAttribArray(0);
         ////// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(::gpu::gltf::vertex), (void *)0);
         //////
         ////// // normals
         ////// glEnableVertexAttribArray(1);
         ////// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(::gpu::gltf::vertex), (void *)offsetof(::gpu::gltf::vertex, mNormal));
         //////
         ////// // texture coordinates
         ////// glEnableVertexAttribArray(2);
         ////// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(::gpu::gltf::vertex),
         //////                       (void *)offsetof(::gpu::gltf::vertex, mTextureCoordinates));
         //////
         ////// // tangents
         ////// glEnableVertexAttribArray(3);
         ////// glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(::gpu::gltf::vertex),
         //////                       (void *)offsetof(::gpu::gltf::vertex, mTangent));
         //////
         ////// // bitangents
         ////// glEnableVertexAttribArray(4);
         ////// glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(::gpu::gltf::vertex),
         //////                       (void *)offsetof(::gpu::gltf::vertex, mBitangent));
         //////
         ////// glBindVertexArray(0);
         //////
         ////// create our data structures
         //////glGenVertexArrays(1, &m_uVAO);
         //////glGenBuffers(1, &m_uVBO);
         //////glGenBuffers(1, &m_uEBO);

         //////glBindVertexArray(m_uVAO); // use this VAO for subsequent calls

         //////auto vertexSize =m_vertexa.size();
         //////auto vertexData = m_vertexa.data();

         //////glBindBuffer(GL_ARRAY_BUFFER, m_uVBO); // use this VBO for subsequent calls
         //////glBufferData(GL_ARRAY_BUFFER, vertexSize * sizeof(::gpu::gltf::vertex),
         //////   vertexData,GL_STATIC_DRAW); // copy over the vertex data

         //////auto indexSize = m_indexa.size();
         //////auto indexData = m_indexa.data();

         //////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uEBO); // use this EBO for subsequent calls
         //////glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(unsigned int),
         //////   indexData,GL_STATIC_DRAW); // copy over the index data

         //////// setup the locations of vertex data
         //////// positions
         //////glEnableVertexAttribArray(0);
         //////glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(::gpu::gltf::vertex), (void *)0);

         //////// normals
         //////glEnableVertexAttribArray(1);
         //////glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(::gpu::gltf::vertex), (void *)offsetof(::gpu::gltf::vertex, mNormal));

         //////// texture coordinates
         //////glEnableVertexAttribArray(2);
         //////glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(::gpu::gltf::vertex), (void *)offsetof(::gpu::gltf::vertex, mTextureCoordinates));

         //////// tangents
         //////glEnableVertexAttribArray(3);
         //////glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(::gpu::gltf::vertex), (void *)offsetof(::gpu::gltf::vertex, mColor));

         //////// bitangents
         //////glEnableVertexAttribArray(4);
         //////glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(::gpu::gltf::vertex), (void *)offsetof(::gpu::gltf::vertex, mTangent));

         //////glBindVertexArray(0);
      }

   } // namespace gltf


} // namespace gpu_directx11
