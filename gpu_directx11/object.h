// Created by camilo on 2022-04-25 20:42 <3ThomasBorregaardSorensen!!
#pragma once

#include "acme/prototype/geometry3d/location.h"
#include "bred/gpu/object.h"


namespace gpu_directx11
{


   class object :
      virtual public ::gpu::object
   {
   public:


      ::gpu::element             m_vao_vertices;
      ::gpu::element             m_vao_normals;

      ::gpu::element             m_vbo_vertices;
      ::gpu::element             m_vbo_normals;
      ::gpu::element             m_vbo_uvs;

      ::gpu::element             m_ibo_elements;

      ::array<unsigned short>    m_indices;
      ::array<::floating_sequence3>       m_vertices;
      ::array<::floating_sequence2>       m_uvs;
      ::array<::floating_sequence3>       m_normals;

      ::gpu::payload             m_payloadTexture;
      ::gpu::element             m_uniformTexture;


      ::file::path               m_path;


      object();
      ~object() override;


      void load(const ::file::path & path) override;


      //void prepare(::gpu::approach * pgpuapproach) override;


      void _prepare(::gpu::approach * pgpuapproach) override;


      void draw() override;

      
      void set_texture_uniform(const ::gpu::payload & uniformTexture) override;


   };



} // namespace gpu_directx11


