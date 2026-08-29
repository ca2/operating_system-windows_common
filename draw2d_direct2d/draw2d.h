#pragma once


#include "aura/graphics/draw2d/draw2d.h"
#include "direct2d/object.h"


class dxgi_device_source;


namespace draw2d_direct2d
{


   class CLASS_DECL_DRAW2D_DIRECT2D draw2d :
      virtual public ::draw2d::draw2d,
      virtual public ::direct2d::object
   {
   public:

      
      //comptr<ID2D1DCRenderTarget> m_pd2d1dcrendertarget;
      //comptr<ID2D1DeviceContext> m_pd2d1devicecontext;
      //::pointer<::mutex> m_pmutexDeviceContext;
      //::image32_t *m_pimage32Raw;
      //HDC m_hdcMemory;
      //HBITMAP m_hbitmap;
      //HBITMAP m_hbitmapOld;


      draw2d();
      ~draw2d() override;

      virtual void initialize(::particle * pparticle) override;

      ::pointer < ::draw2d::window_attachment > allocate_draw2d_window_attachment(::acme::windowing::window * pacmewindowingwindow) override;


      virtual string write_text_get_default_implementation_name() override;

      bool lock_device() override;
      void unlock_device() override;

      virtual ::dxgi_device_source * _dxgi_device_source();

      virtual ::particle * default_device_context_mutex();

      virtual ID2D1DeviceContext * default_d2d1_device_context();

      static ::draw2d_direct2d::draw2d * get();
      //virtual mutex *_generic_d2d1_device_context_mutex();
      //virtual ID2D1DeviceContext *_generic_d2d1_device_context();

      //void on_create_window(::windowing::window* pwindow) override;
      
      void adjust_composited_window_styles(::u32& nExStyle, ::u32& nStyle) override;
      ::draw2d::graphics_pointer do_allocation_strategy(::acme::user::interaction * pacmeuserinteractionAffinity, ::image::image *pimage,
                                                                const ::i32_size &size) override;
      void do_release_to_pool_strategy(::draw2d::graphics_pointer &pdraw2dgraphics, ::image::image *pimage) override;

   };


} // namespace draw2d_direct2d



