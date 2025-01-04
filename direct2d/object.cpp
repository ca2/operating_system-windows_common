// From draw2d_direct2d/pen.cpp by camilo
// on 2014-12-30 01:51 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "object.h"
#include "direct2d.h"


namespace direct2d
{

   object::object()
   {
    
   }

   object::~object()
   {

   }

   comptr < ID2D1StrokeStyle1 > object::_create_stroke_style(
      ::direct2d::graphics * pgraphics,
               ::draw2d::enum_line_cap elinecapBeg,
         ::draw2d::enum_line_cap elinecapEnd
   )
   {

      bool bProperties = false;

      D2D1_STROKE_STYLE_PROPERTIES1 properties1{};

      if (elinecapBeg == ::draw2d::e_line_cap_round)
      {

         properties1.startCap = D2D1_CAP_STYLE_ROUND;

         bProperties = true;

      }


      if (elinecapEnd == ::draw2d::e_line_cap_round)
      {

         properties1.endCap = D2D1_CAP_STYLE_ROUND;

         bProperties = true;

      }


      if (!bProperties)
      {

         return nullptr;

      }

      comptr < ID2D1StrokeStyle1 > pstrokestyle;

      HRESULT hr = ::direct2d::direct2d()->d2d1_factory1()->CreateStrokeStyle(&properties1, nullptr, 0, &pstrokestyle);

      return pstrokestyle;

   }


} // namespace direct2d


