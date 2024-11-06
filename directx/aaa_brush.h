#pragma once


namespace draw2d_directx
{


   class CLASS_DECL_DRAW2D_DIRECTX brush :
      virtual public ::draw2d_directx::object,
      virtual public ::draw2d::brush
   {
   public:


      //      Gdiplus::Brush * m_pbrush;


      comptr<ID2D1Brush>                 m_pbrush;
      comptr<ID2D1SolidColorBrush>       m_psolidbrush;
      comptr<ID2D1LinearGradientBrush>   m_plineargradientbrush;
      comptr<ID2D1RadialGradientBrush>   m_pradialgradientbrush;
      comptr<ID2D1ImageBrush>            m_pimagebrush;


      brush();
      virtual ~brush();


      virtual void destroy() override;
      virtual bool create(::draw2d::graphics * pgraphics, char iCreate) override;

      //virtual ID2D1Brush * get_os_brush(::draw2d_directx::graphics * pgraphics) const;

      //bool CreateSolidBrush(color32_t crColor);
      //bool CreateHatchBrush(int nIndex, color32_t crColor);
      //bool CreatePatternBrush(::draw2d::bitmap* pBitmap);
      //bool CreateDIBPatternBrush(HGLOBAL hPackedDIB, unsigned int nUsage);
      //bool CreateDIBPatternBrush(const void * lpPackedDIB, unsigned int nUsage);
      //bool CreateSysColorBrush(int nIndex);


      //bool destroy();

      void dump(dump_context & dumpcontext) const override;
   };


} // namespace draw2d_directx



