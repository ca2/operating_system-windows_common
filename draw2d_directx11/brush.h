#pragma once


#include "aura/graphics/draw2d/brush.h"
#include "object.h"


namespace draw2d_directx11
{


   class CLASS_DECL_DRAW2D_DIRECTX11 brush :
      virtual public ::draw2d_directx11::object,
      virtual public ::draw2d::brush
   {
   public:


      //comptr<ID2D1Brush>                 m_pbrush;
      //comptr<ID2D1SolidColorBrush>       m_psolidbrush;
      //comptr<ID2D1LinearGradientBrush>   m_plineargradientbrush;
      //comptr<ID2D1RadialGradientBrush>   m_pradialgradientbrush;
      //comptr<ID2D1ImageBrush>            m_pimagebrush;


      brush();
      ~brush() override;


      // void dump(dump_context& dumpcontext) const override;


      void destroy() override;
      void destroy_os_data() override;
      void create(::draw2d::graphics * pgraphics, char iCreate) override;

      //virtual ID2D1Brush * get_os_brush(::draw2d_directx11::graphics * pgraphics) const;

      //bool CreateSolidBrush(::color::color crColor);
      //bool CreateHatchBrush(int nIndex, ::color::color crColor);
      //bool CreatePatternBrush(::draw2d::bitmap* pBitmap);
      //bool CreateDIBPatternBrush(HGLOBAL hPackedDIB, unsigned int nUsage);
      //bool CreateDIBPatternBrush(const void * lpPackedDIB, unsigned int nUsage);
      //bool CreateSysColorBrush(int nIndex);


      //bool destroy();

   };


} // namespace draw2d_directx11



