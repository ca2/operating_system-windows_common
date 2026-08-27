#pragma once


#include "operating_system-windows_common/draw2d_direct2d/brush.h"
#include "object.h"


namespace draw2d_direct2d_for_directx11
{


   class CLASS_DECL_DRAW2D_DIRECT2D_FOR_DIRECTX11 brush :
      virtual public ::draw2d_direct2d_for_directx11::object,
      virtual public ::draw2d_direct2d::brush
   {
   public:


      //comptr<ID2D1Brush>                 m_pdraw2dbrush;
      //comptr<ID2D1SolidColorBrush>       m_psolidbrush;
      //comptr<ID2D1LinearGradientBrush>   m_plineargradientbrush;
      //comptr<ID2D1RadialGradientBrush>   m_pradialgradientbrush;
      //comptr<ID2D1ImageBrush>            m_pimagebrush;


      brush();
      ~brush() override;


      // void dump(dump_context& dumpcontext) const override;


      void destroy() override;
      // void destroy_os_data() override;
      void update(::draw2d::graphics * pdraw2dgraphics) override;

      //virtual ID2D1Brush * get_os_brush(::draw2d_direct2d_for_directx11::graphics * pdraw2dgraphics) const;

      //bool CreateSolidBrush(::color::color crColor);
      //bool CreateHatchBrush(::i32 nIndex, ::color::color crColor);
      //bool CreatePatternBrush(::draw2d::bitmap* pBitmap);
      //bool CreateDIBPatternBrush(HGLOBAL hPackedDIB, ::u32 nUsage);
      //bool CreateDIBPatternBrush(const void * lpPackedDIB, ::u32 nUsage);
      //bool CreateSysColorBrush(::i32 nIndex);


      //bool destroy();

   };


} // namespace draw2d_direct2d_for_directx11



