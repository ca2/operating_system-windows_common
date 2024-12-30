//
// Created by camilo on 31/01/2022 14:35 <3ThomasBorregaardSorensen!!
//
#pragma once


#include "acme/nano/graphics/device.h"
#include "direct2d/graphics.h"
#undef USUAL_OPERATING_SYSTEM_SUPPRESSIONS
#include "acme/_operating_system.h"


namespace universal_windows
{


   namespace nano
   {


      namespace graphics
      {


         class CLASS_DECL_NANO_GRAPHICS_DIRECT2D device :
            virtual public ::nano::graphics::device,
            virtual public ::direct2d::graphics
         {
         public:


            class state
            {
            public:


               D2D1::Matrix3x2F                    m_m;
               int                                 m_iLayerIndex;
               D2D1_LAYER_PARAMETERS               m_layerparameters;

            };


            int                                                m_iLayerCount;

            comptr<ID2D1DeviceContext>                         m_pdevicecontext; // 0
            comptr<ID2D1RenderTarget>                          m_prendertarget; // 1
            comptr<ID2D1BitmapRenderTarget>                    m_pbitmaprendertarget; // 2
            comptr<ID2D1DCRenderTarget>                        m_pdcrendertarget; // 3
            comptr<ID2D1DeviceContext1>                        m_pdevicecontext1; // 4

            comptr<IDXGIAdapter>                               m_padapter;
            comptr<IDXGIFactory2>                              m_pfactory2;
            comptr<ID2D1Layer>                                 m_player;
            comptr<ID2D1PathGeometry>                          m_ppathgeometryClip;

            ::array < state >                                  m_statea;
            state                                              m_state;

            bool                                               m_bSaveClip;

            int                                                m_iType;

            D2D1_BITMAP_INTERPOLATION_MODE                     m_bitmapinterpolationmode;
            D2D1_INTERPOLATION_MODE                            m_interpolationmode;

            ::write_text::enum_rendering                    m_etextrenderinghintDevice;



            HDC                                                m_hdcAttach;
            ::draw2d::enum_alpha_mode                          m_ealphamodeDevice;


            device();
            
            ~device() override;


            void attach(void * posdata, const ::int_size & size) override;

            void _draw_text(const ::string& str, const ::int_rectangle& rectangleText, const ::e_align& ealign, const ::e_draw_text& edrawtext, ::nano::graphics::brush* pnanobrushBack, ::nano::graphics::brush* pnanobrushText, ::nano::graphics::font* pnanofont) override;
            ::int_size get_text_extents(const ::string& str, ::nano::graphics::font* pnanofont) override;
            void rectangle(const ::int_rectangle& rectangle, ::nano::graphics::brush* pnanobrush, ::nano::graphics::pen* pnanopen) override;


            void draw(::nano::graphics::icon * picon, int x, int y, int cx, int cy) override;


            void translate(int x, int y) override;


         };




      } // namespace graphics


   } // namespace nano



} // namespace universal_windows



