// Created by camilo on 2021-06-05 23:48 BRT <3ThomasBS_!!
#pragma once


namespace directx
{


   class CLASS_DECL_DIRECTX directx :
      virtual public ::matter
   {
   public:


      comptr<ID3D11DeviceContext>        m_pd3devicecontext;
      comptr<ID3D11DeviceContext1>       m_pd3devicecontext1;
      comptr<ID3D11Device>               m_pd3device;
      comptr<ID3D11Device1>              m_pd3device1;
      comptr<IDXGIDevice>                m_pdxgidevice;


      __pointer(::user::interaction_impl)                m_pimpl;

      
      D3D_FEATURE_LEVEL                                  m_featurelevel;


      static directx *                                   s_pdirectx;


      directx();
      ~directx() override;


      ::e_status initialize(::object * pobject) override;


      ID3D11Device * draw_get_d3d11_device();
      ID3D11Device1 * draw_get_d3d11_device1();
      ID3D11DeviceContext * draw_get_d3d11_device_context();
      ID3D11DeviceContext1 * draw_get_d3d11_device_context1();
      IDXGIDevice * draw_get_dxgi_device();


   };


   inline directx * directx() 
   { 

      return directx::s_pdirectx; 
   
   }

   
   CLASS_DECL_DIRECTX void defer_initialize(::object * pobject);
   CLASS_DECL_DIRECTX void finalize();


} // namespace directx



