//
// Created by camilo on 2024-09-13 13:59 <3ThomasBorregaardSorensen!!
//
#pragma once


#include "acme/nano/graphics/graphics.h"
#undef USUAL_OPERATING_SYSTEM_SUPPRESSIONS
#include "acme/_operating_system.h"


namespace universal_windows
{


   namespace nano
   {


      namespace graphics
      {


         class CLASS_DECL_NANO_GRAPHICS_DIRECT2D graphics :
            virtual public ::nano::graphics::graphics
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


            // advanced use and implementation
            bool                                               m_bPrinting;

            HDC                                                m_hdcAttach;
            ::draw2d::enum_alpha_mode                          m_ealphamodeDevice;



            graphics();
            ~graphics() override;


            //virtual void _defer_show_system_menu(HWND hwnd, HMENU * phmenuSystem, const ::int_point & pointAbsolute);
            //virtual bool _on_command(LRESULT & lresult, HWND hwnd, WPARAM wparam, LPARAM lparam);
            //virtual void _erase_minimize_box_style(HWND hwnd);

         };



      } // namespace graphics


   } // namespace nano

} // namespace universal_windows



