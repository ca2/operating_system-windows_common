#include "platform.h"
#include "font.h"
#include "direct2d/direct2d.h"
//#include "aura/graphics/draw2d/host.h"


#include "acme_windows_common/_.h"
#include "acme/_operating_system.h"
#include "acme/operating_system/windows_common/com/hresult_exception.h"


namespace draw2d_direct2d
{


   font::font()
   {


   }


   font::~font()
   {

      destroy();

   }


   DWRITE_FONT_STYLE font::_dwrite_font_style()
   {

      DWRITE_FONT_STYLE style;

      if (m_bItalic)
      {

         style = DWRITE_FONT_STYLE_ITALIC;

      }
      else
      {

         style = DWRITE_FONT_STYLE_NORMAL;

      }

      return style;

   }


   DWRITE_FONT_STRETCH font::_dwrite_font_stretch()
   {

      DWRITE_FONT_STRETCH stretch;

      stretch = DWRITE_FONT_STRETCH_NORMAL;

      return stretch;

   }


   DWRITE_FONT_WEIGHT font::_dwrite_font_weight()
   {

      return dwrite_font_weight(m_fontweight);
         
   }


   ::f32 font::_dwrite_font_size(::draw2d::graphics * pdraw2dgraphics)
   {
      ::f32 fFontSize;

      //::acme::windowing::window * pacmewindowingwindow = nullptr;
      //
      //if (::is_set(pdraw2dgraphics))
      //{
      // 
      //   oswindow = pdraw2dgraphics->get_window_handle();

      //}

      if (m_fontsize.eunit() == ::e_unit_point)
      {

         fFontSize = (::f32)pdraw2dgraphics->m_pacmeuserinteractionAffinity->point_dpi(m_fontsize.as_f64());

      }
      else
      {

         fFontSize = (::f32)pdraw2dgraphics->m_pacmeuserinteractionAffinity->dpiy(m_fontsize.as_f64());

      }

      if (::is_set(pdraw2dgraphics))
      {

         fFontSize *= (::f32)pdraw2dgraphics->m_dSizeScaler;

      }

      if (fFontSize <= 0.000001)
      {

         //return false;
         throw ::exception(error_failed);


      }

      return fFontSize;

   }


   void font::update(::draw2d::graphics * pdraw2dgraphics)
   {

      //if(m_pdwritetextformat == nullptr || is_modified())
      //{

         //if(m_pdwritetextformat)
         //{

         //   clear_node_data();

         //}

         IDWriteFactory * pfactory = direct2d()->dwrite_factory();

         if (!defer_load_internal_font(pdraw2dgraphics))
         {

            HRESULT hr = pfactory->CreateTextFormat(
               wstring(m_pfontfamily->family_name(this)),
               nullptr,
               _dwrite_font_weight(),
               _dwrite_font_style(),
               _dwrite_font_stretch(),
               _dwrite_font_size(pdraw2dgraphics),
               L"",
               &m_pdwritetextformat);

            if (FAILED(hr) || m_pdwritetextformat == nullptr)
            {

               warning() << "font::get_os_font: " << hresult_text(hr);

               //return false;

               throw ::exception(error_failed);

            }

         }

         create_text_metrics(pdraw2dgraphics);

      //}

      //m_osdata[0] = m_pdwritetextformat;

      //m_baCalculated[0] = true;

      //return (IDWriteTextFormat *) m_pdwritetextformat;

   }


   void font::create_text_metrics(::draw2d::graphics * pdraw2dgraphics)
   {

      WCHAR name[256];
      ::u32 findex;
      BOOL exists;

      if (::is_null(m_pdwritetextformat))
      {

         m_textmetric2.m_dAscent = 0;
         m_textmetric2.m_dDescent = 0;
         m_textmetric2.m_dHeight = m_fontsize.as_f64();
         m_textmetric2.m_dInternalLeading = 0;
         m_textmetric2.m_dExternalLeading = 0;

         //return true;

         return;

      }

      if (!m_pdwritefontcollection)
      {

         m_pdwritetextformat->GetFontFamilyName(name, 256);

         m_pdwritetextformat->GetFontCollection(&m_pdwritefontcollection);

         if (!m_pdwritefontcollection)
         {

            m_textmetric2.m_dAscent = 0;
            m_textmetric2.m_dDescent = 0;
            m_textmetric2.m_dHeight = m_fontsize.as_f64();
            m_textmetric2.m_dInternalLeading = 0;
            m_textmetric2.m_dExternalLeading = 0;

            return;

            //return true;

         }

      }

      if (!m_pdwritefontfamily)
      {

         auto iFontFamilyCount = m_pdwritefontcollection->GetFontFamilyCount();

         m_pdwritefontcollection->FindFamilyName(name, &findex, &exists);

         if (!exists)
         {

            m_pdwritefontcollection->FindFamilyName(L"Arial", &findex, &exists);

            if (!exists)
            {

               m_textmetric2.m_dAscent = 0;
               m_textmetric2.m_dDescent = 0;
               m_textmetric2.m_dHeight = m_fontsize.as_f64();
               m_textmetric2.m_dInternalLeading = 0;
               m_textmetric2.m_dExternalLeading = 0;

               //return true;

               return;

            }

         }

         m_pdwritefontcollection->GetFontFamily(findex, &m_pdwritefontfamily);

         if (!m_pdwritefontfamily)
         {

            m_textmetric2.m_dAscent = 0;
            m_textmetric2.m_dDescent = 0;
            m_textmetric2.m_dHeight = m_fontsize.as_f64();
            m_textmetric2.m_dInternalLeading = 0;
            m_textmetric2.m_dExternalLeading = 0;

            //return true;

            return;

         }

      }

      HRESULT hrFindFont = E_FAIL;

      if (!m_pdwritefont)
      {

         auto weight = m_pdwritetextformat->GetFontWeight();

         auto stretch = m_pdwritetextformat->GetFontStretch();

         auto style = m_pdwritetextformat->GetFontStyle();

         while (true)
         {

            hrFindFont = m_pdwritefontfamily->GetFirstMatchingFont(
              weight,
              stretch,
              style, &m_pdwritefont);

            if (SUCCEEDED(hrFindFont) && m_pdwritefont)
            {

               break;

            }

            if (stretch != DWRITE_FONT_STRETCH_NORMAL
               && stretch != DWRITE_FONT_STRETCH_UNDEFINED)
            {

               stretch = DWRITE_FONT_STRETCH_NORMAL;

               continue;

            }

            if (stretch != DWRITE_FONT_STRETCH_UNDEFINED)
            {

               stretch = DWRITE_FONT_STRETCH_UNDEFINED;

               continue;

            }

            if (style != DWRITE_FONT_STYLE_NORMAL)
            {

               style = DWRITE_FONT_STYLE_NORMAL;

               continue;

            }

            if (weight >= DWRITE_FONT_WEIGHT_BOLD)
            {

               weight = DWRITE_FONT_WEIGHT_NORMAL;

               continue;

            }

            if (weight >= DWRITE_FONT_WEIGHT_NORMAL)
            {

               weight = DWRITE_FONT_WEIGHT_LIGHT;

               continue;

            }

            break;

         }

      }

      if (FAILED(hrFindFont) || !m_pdwritefont)
      {

         m_textmetric2.m_dAscent = 0;
         m_textmetric2.m_dDescent = 0;
         m_textmetric2.m_dHeight = m_fontsize.as_f64();
         m_textmetric2.m_dInternalLeading = 0;
         m_textmetric2.m_dExternalLeading = 0;

         //return true;

         return;

      }

      DWRITE_FONT_METRICS metrics;

      m_pdwritefont->GetMetrics(&metrics);

      ::f64 ratio = m_pdwritetextformat->GetFontSize() / (::f32)metrics.designUnitsPerEm;

      m_textmetric2.m_dAscent = (::f64) (metrics.ascent * ratio);
      m_textmetric2.m_dDescent = (::f64)(metrics.descent * ratio);
      m_textmetric2.m_dInternalLeading = (::f64)0;
      m_textmetric2.m_dExternalLeading = (::f64)(metrics.lineGap * ratio);
      m_textmetric2.m_dHeight = (::f64)((metrics.ascent + metrics.descent + metrics.lineGap) * ratio);

   }




   //void font::destroy()
   //{

   //   destroy_os_data();

   //   ::write_text::font::destroy();

   //}


   void font::clear_node_data()
   {

      m_pdwritetextformat = nullptr;
      m_pdwritefontcollection = nullptr;
      m_pdwritefontfamily = nullptr;
      m_pdwritefont = nullptr;

      //object::destroy_os_data();

   }


} // namespace draw2d_direct2d


