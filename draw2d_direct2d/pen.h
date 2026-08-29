#pragma once


#include "aura/graphics/draw2d/pen.h"
#include "object.h"


namespace draw2d_direct2d
{


   class CLASS_DECL_DRAW2D_DIRECT2D pen :
      virtual public ::draw2d_direct2d::object,
      virtual public ::draw2d::pen
   {
   public:


      //comptr<ID2D1SolidColorBrush>                 m_pd2d1solidcolorbrush;
      comptr<ID2D1StrokeStyle1>                    m_pd2d1strokestyle1;
      bool                                         m_bMetroColor;
      ::color::color                               m_colorMetro;


      pen();
      ~pen() override;



      void clear_node_data() override;
      // void destroy_os_data() override;
      void update(::draw2d::graphics * pdraw2dgraphics) override;

      //comptr < ID2D1StrokeStyle1 > _create_stroke_style(::draw2d::graphics * pdraw2dgraphics);

      /*virtual void construct(::i32 nPenStyle, ::f64 nWidth, ::color::color crColor);
      virtual void construct(::i32 nPenStyle, ::f64 nWidth, const LOGBRUSH* pLogBrush, ::i32 nStyleCount = 0, const ::u32* lpStyle = nullptr);
      bool CreatePen(::i32 nPenStyle, ::f64 nWidth, ::color::color crColor);
      bool CreatePen(::i32 nPenStyle, ::f64 nWidth, const LOGBRUSH* pLogBrush, ::i32 nStyleCount = 0, const ::u32* lpStyle = nullptr);*/

      //virtual bool create_solid(::draw2d::graphics * pdraw2dgraphics, ::f64 dWidth, ::color::color crColor);

      //virtual ID2D1Brush * get_os_pen_brush(::draw2d_direct2d::graphics * pdraw2dgraphics) const;


      //bool destroy();

      //void dump(dump_context & dumpcontext) const override;

      static HRESULT s_RenderPatternToCommandList(ID2D1RenderTarget * pdraw2dgraphics,D2D1_COLOR_F *pcr);
      static HRESULT s_CreatePatternBrush(ID2D1DeviceContext *pDeviceContext, D2D1_COLOR_F * pcr, ID2D1ImageBrush **ppImageBrush);


   };

} // namespace draw2d_direct2d
