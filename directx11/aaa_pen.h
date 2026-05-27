#pragma once


namespace draw2d_directx11
{


   class CLASS_DECL_DRAW2D_DIRECTX11 pen :
      virtual public ::draw2d_directx11::object,
      virtual public ::draw2d::pen
   {
   public:


      comptr<ID2D1SolidColorBrush>    m_pbrush;
      bool                                            m_bMetroColor;
      color32_t                                        m_colorMetro;


      pen();
      virtual ~pen();



      virtual void destroy() override;
      virtual bool create(::draw2d::graphics * pgraphics, ::i8 iCreate) override;

      /*virtual void construct(::i32 nPenStyle, ::f64 nWidth, color32_t crColor);
      virtual void construct(::i32 nPenStyle, ::f64 nWidth, const LOGBRUSH* pLogBrush, ::i32 nStyleCount = 0, const ::u32* lpStyle = nullptr);
      bool CreatePen(::i32 nPenStyle, ::f64 nWidth, color32_t crColor);
      bool CreatePen(::i32 nPenStyle, ::f64 nWidth, const LOGBRUSH* pLogBrush, ::i32 nStyleCount = 0, const ::u32* lpStyle = nullptr);*/

      //virtual bool create_solid(::draw2d::graphics * pgraphics, ::f64 dWidth, color32_t crColor);

      //virtual ID2D1Brush * get_os_pen_brush(::draw2d_directx11::graphics * pgraphics) const;


      //bool destroy();

      void dump(dump_context & dumpcontext) const override;

      static HRESULT s_RenderPatternToCommandList(ID2D1RenderTarget * pgraphics,D2D1_COLOR_F *pcr);
      static HRESULT s_CreatePatternBrush(ID2D1DeviceContext *pDeviceContext, D2D1_COLOR_F * pcr, ID2D1ImageBrush **ppImageBrush);


   };

} // namespace draw2d_directx11
