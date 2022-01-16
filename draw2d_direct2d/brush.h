#pragma once


namespace draw2d_direct2d
{


   class CLASS_DECL_DRAW2D_DIRECT2D brush :
      virtual public ::draw2d_direct2d::object,
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
      ~brush() override;


      void dump(dump_context& dumpcontext) const override;


      void destroy_os_data() override;
      void create(::draw2d::graphics * pgraphics, i8 iCreate) override;

      //virtual ID2D1Brush * get_os_brush(::draw2d_direct2d::graphics * pgraphics) const;

      //bool CreateSolidBrush(::color::color crColor);
      //bool CreateHatchBrush(int nIndex, ::color::color crColor);
      //bool CreatePatternBrush(::draw2d::bitmap* pBitmap);
      //bool CreateDIBPatternBrush(HGLOBAL hPackedDIB, ::u32 nUsage);
      //bool CreateDIBPatternBrush(const void * lpPackedDIB, ::u32 nUsage);
      //bool CreateSysColorBrush(int nIndex);


      //bool destroy();

   };


} // namespace draw2d_direct2d



