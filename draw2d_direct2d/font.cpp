#include "framework.h"
#include "font.h"
#include "direct2d/direct2d.h"
#include "aura/graphics/draw2d/host.h"


#include "acme_windows_common/_.h"
#include "acme/_operating_system.h"
#include "acme_windows_common/hresult_exception.h"


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


   //void font::dump(dump_context & dumpcontext) const
   //{

   //   ::draw2d::object::dump(dumpcontext);

   //}


   void font::create(::draw2d::graphics * pgraphics, char iCreate)
   {

      if(m_pformat == nullptr || is_modified(::draw2d::e_default_object))
      {

         if(m_pformat)
         {

            destroy();

         }

         IDWriteFactory * pfactory = ::direct2d::direct2d()->dwrite_factory();

         DWRITE_FONT_STYLE style;

         if (m_bItalic)
         {

            style = DWRITE_FONT_STYLE_ITALIC;

         }
         else
         {

            style = DWRITE_FONT_STYLE_NORMAL;

         }

         DWRITE_FONT_STRETCH stretch;

         stretch = DWRITE_FONT_STRETCH_NORMAL;

         float fFontSize;

         //oswindow oswindow = nullptr;
         //
         //if (::is_set(pgraphics))
         //{
         // 
         //   oswindow = pgraphics->get_window_handle();

         //}

         if(m_fontsize.eunit() == ::e_unit_point)
         {

            fFontSize = (float) pgraphics->m_pdraw2dhost->point_dpi(m_fontsize.as_double());

         }
         else
         {

            fFontSize = (float) pgraphics->m_pdraw2dhost->dpiy(m_fontsize.as_double());

         }

         if (::is_set(pgraphics))
         {

            fFontSize *= (float)pgraphics->m_dFontFactor;

         }

         if (fFontSize <= 0.000001)
         {

            //return false;
            throw ::exception(error_failed);


         }

         HRESULT hr = pfactory->CreateTextFormat(
            wstring(m_pfontfamily->family_name(this)),
            nullptr,
            dwrite_font_weight(m_fontweight),
            style,
            stretch,
            fFontSize,
            L"",
            &m_pformat);

         if(FAILED(hr) || m_pformat == nullptr)
         {

            warning() <<"font::get_os_font: " << hresult_text(hr);

            //return false;

            throw ::exception(error_failed);

         }

         create_text_metrics(pgraphics);

      }

      m_osdata[0] = m_pformat;

      m_baCalculated[0] = true;

      //return (IDWriteTextFormat *) m_pformat;

   }


   void font::create_text_metrics(::draw2d::graphics * pgraphics)
   {

      comptr<IDWriteFontCollection> pcollection;

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

      m_pformat->GetFontFamilyName(name, 256);

      m_pformat->GetFontCollection(&pcollection);

      if (pcollection == nullptr)
      {

         m_textmetric2.m_dAscent = 0;
         m_textmetric2.m_dDescent = 0;
         m_textmetric2.m_dHeight = m_fontsize.as_double();
         m_textmetric2.m_dInternalLeading = 0;
         m_textmetric2.m_dExternalLeading = 0;

         return;

         //return true;

      }

      pcollection->FindFamilyName(name, &findex, &exists);

      if (!exists)
      {

         pcollection->FindFamilyName(L"Arial", &findex, &exists);

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

      comptr<IDWriteFontFamily> ffamily;

      pcollection->GetFontFamily(findex, &ffamily);

      if (ffamily == nullptr)
      {

         m_textmetric2.m_dAscent = 0;
         m_textmetric2.m_dDescent = 0;
         m_textmetric2.m_dHeight = m_fontsize.as_double();
         m_textmetric2.m_dInternalLeading = 0;
         m_textmetric2.m_dExternalLeading = 0;

         //return true;

         return;

      }

      comptr<IDWriteFont> pfont;

      ffamily->GetFirstMatchingFont(m_pformat->GetFontWeight(), m_pformat->GetFontStretch(), m_pformat->GetFontStyle(), &pfont);

      if (pfont == nullptr)
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

      pfont->GetMetrics(&metrics);

      double ratio = m_pformat->GetFontSize() / (float)metrics.designUnitsPerEm;

      m_textmetric2.m_dAscent = (int)(metrics.ascent * ratio);
      m_textmetric2.m_dDescent = (int)(metrics.descent * ratio);
      m_textmetric2.m_dInternalLeading = (int)0;
      m_textmetric2.m_dExternalLeading = (int)(metrics.lineGap * ratio);
      m_textmetric2.m_dHeight = (int)((metrics.ascent + metrics.descent + metrics.lineGap) * ratio);

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


