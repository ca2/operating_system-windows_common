// Created by camilo on 2026-07-31 16:23 <3ThomasBorregaardSørensen!! Mummi!! Bilbo!!
#pragma once


#include "aura/graphics/draw2d/window_attachment.h"


namespace draw2d_direct2d
{


   class CLASS_DECL_DRAW2D_DIRECT2D window_attachment : 
      virtual public ::draw2d::window_attachment
   {
   public:


      class CLASS_DECL_DRAW2D_DIRECT2D swap_chain :
         virtual public ::particle
      {
      public:


         comptr<ID2D1HwndRenderTarget> m_pd2d1hwndrendertarget;


      };


      class CLASS_DECL_DRAW2D_DIRECT2D gdi_offscreen :
         virtual public ::particle
      {
      public:


         comptr<ID2D1DCRenderTarget> m_pd2d1dcrendertarget;
         //HDC m_hdcMemory;
         //HBITMAP m_hbitmap;
         //HBITMAP m_hbitmapOld;


      };


      comptr<ID2D1DeviceContext> m_pd2d1devicecontext;
      ::pointer<::mutex> m_pmutexDeviceContext;
      ::pointer< gdi_offscreen >m_pgdioffscreen;


      window_attachment();
      ~window_attachment() override;


      void initialize_window_attachment(::windowing::window * pwindow) override;


      virtual mutex *_d2d1_device_context_mutex();
      virtual ID2D1DeviceContext *_d2d1_device_context();


   };


} // namespace draw2d_direct2d



