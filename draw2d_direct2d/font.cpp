#include "framework.h"
#include "font.h"
#include "direct2d/direct2d.h"
#include "aura/graphics/draw2d/host.h"


#include "acme_windows_common/_.h"
#include "acme/_operating_system.h"
#include "acme/operating_system/windows_common/com/hresult_exception.h"


namespace draw2d_direct2d
{


   font::font()
   {

      m_pthis = this;

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


   float font::_dwrite_font_size(::draw2d::graphics * pgraphics)
   {
      float fFontSize;

      //::acme::windowing::window * pacmewindowingwindow = nullptr;
      //
      //if (::is_set(pgraphics))
      //{
      // 
      //   oswindow = pgraphics->get_window_handle();

      //}

      if (m_fontsize.eunit() == ::e_unit_point)
      {

         fFontSize = (float)pgraphics->m_pdraw2dhost->point_dpi(m_fontsize.as_double());

      }
      else
      {

         fFontSize = (float)pgraphics->m_pdraw2dhost->dpiy(m_fontsize.as_double());

      }

      if (::is_set(pgraphics))
      {

         fFontSize *= (float)pgraphics->m_dSizeScaler;

      }

      if (fFontSize <= 0.000001)
      {

         //return false;
         throw ::exception(error_failed);


      }

      return fFontSize;

   }


   void font::create(::draw2d::graphics * pgraphics, char iCreate)
   {

      if(m_pformat == nullptr || is_modified(::draw2d::e_default_object))
      {

         if(m_pformat)
         {

            destroy();

         }

         IDWriteFactory * pfactory = m_pdirect2d->dwrite_factory();

         if (!defer_load_internal_font(pgraphics))
         {

            HRESULT hr = pfactory->CreateTextFormat(
               wstring(m_pfontfamily->family_name(this)),
               nullptr,
               _dwrite_font_weight(),
               _dwrite_font_style(),
               _dwrite_font_stretch(),
               _dwrite_font_size(pgraphics),
               L"",
               &m_pformat);

            if (FAILED(hr) || m_pformat == nullptr)
            {

               warning() << "font::get_os_font: " << hresult_text(hr);

               //return false;

               throw ::exception(error_failed);

            }

         }

         create_text_metrics(pgraphics);

      }

      m_osdata[0] = m_pformat;

      m_baCalculated[0] = true;

      //return (IDWriteTextFormat *) m_pformat;

   }


   void font::create_text_metrics(::draw2d::graphics * pgraphics)
   {

      WCHAR name[256];
      unsigned int findex;
      BOOL exists;

      if (::is_null(m_pformat))
      {

         m_textmetric2.m_dAscent = 0;
         m_textmetric2.m_dDescent = 0;
         m_textmetric2.m_dHeight = m_fontsize.as_double();
         m_textmetric2.m_dInternalLeading = 0;
         m_textmetric2.m_dExternalLeading = 0;

         //return true;

         return;

      }

      if (!m_pcollection)
      {

         m_pformat->GetFontFamilyName(name, 256);

         m_pformat->GetFontCollection(&m_pcollection);

         if (!m_pcollection)
         {

            m_textmetric2.m_dAscent = 0;
            m_textmetric2.m_dDescent = 0;
            m_textmetric2.m_dHeight = m_fontsize.as_double();
            m_textmetric2.m_dInternalLeading = 0;
            m_textmetric2.m_dExternalLeading = 0;

            return;

            //return true;

         }

      }

      if (!m_pfamily)
      {

         auto iFontFamilyCount = m_pcollection->GetFontFamilyCount();

         m_pcollection->FindFamilyName(name, &findex, &exists);

         if (!exists)
         {

            m_pcollection->FindFamilyName(L"Arial", &findex, &exists);

            if (!exists)
            {

               m_textmetric2.m_dAscent = 0;
               m_textmetric2.m_dDescent = 0;
               m_textmetric2.m_dHeight = m_fontsize.as_double();
               m_textmetric2.m_dInternalLeading = 0;
               m_textmetric2.m_dExternalLeading = 0;

               //return true;

               return;

            }

         }

         m_pcollection->GetFontFamily(findex, &m_pfamily);

         if (!m_pfamily)
         {

            m_textmetric2.m_dAscent = 0;
            m_textmetric2.m_dDescent = 0;
            m_textmetric2.m_dHeight = m_fontsize.as_double();
            m_textmetric2.m_dInternalLeading = 0;
            m_textmetric2.m_dExternalLeading = 0;

            //return true;

            return;

         }

      }

      HRESULT hrFindFont = E_FAIL;

      if (!m_pfont)
      {

         auto weight = m_pformat->GetFontWeight();

         auto stretch = m_pformat->GetFontStretch();

         auto style = m_pformat->GetFontStyle();

         while (true)
         {

            hrFindFont = m_pfamily->GetFirstMatchingFont(
              weight,
              stretch,
              style, &m_pfont);

            if (SUCCEEDED(hrFindFont) && m_pfont)
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

      if (FAILED(hrFindFont) || !m_pfont)
      {

         m_textmetric2.m_dAscent = 0;
         m_textmetric2.m_dDescent = 0;
         m_textmetric2.m_dHeight = m_fontsize.as_double();
         m_textmetric2.m_dInternalLeading = 0;
         m_textmetric2.m_dExternalLeading = 0;

         //return true;

         return;

      }

      DWRITE_FONT_METRICS metrics;

      m_pfont->GetMetrics(&metrics);

      double ratio = m_pformat->GetFontSize() / (float)metrics.designUnitsPerEm;

      m_textmetric2.m_dAscent = (double) (metrics.ascent * ratio);
      m_textmetric2.m_dDescent = (double)(metrics.descent * ratio);
      m_textmetric2.m_dInternalLeading = (double)0;
      m_textmetric2.m_dExternalLeading = (double)(metrics.lineGap * ratio);
      m_textmetric2.m_dHeight = (double)((metrics.ascent + metrics.descent + metrics.lineGap) * ratio);

   }


   void font::destroy()
   {

      destroy_os_data();

      ::write_text::font::destroy();

   }


   void font::destroy_os_data()
   {

      m_pformat = nullptr;

      object::destroy_os_data();

   }


} // namespace draw2d_direct2d


