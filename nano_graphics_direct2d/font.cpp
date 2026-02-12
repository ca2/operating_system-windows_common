//
// Created by camilo on 31/01/2022 16:06 <3ThomasBorregaardSorensen!!
//
#include "framework.h"
#include "font.h"
#include "device.h"
#include "acme/graphics/write_text/font_weight.h"
#include "direct2d/direct2d.h"


namespace universal_windows
{



   namespace nano
   {


      namespace graphics
      {

         font::font()
         {

            m_pthis = this;

         }


         font::~font()
         {

         }


         void font::update(::nano::graphics::device* pnanodevice)
         {

            if (m_bModified)
            {

               destroy();

               LOGFONTW logfontw = {};

               ::cast < ::universal_windows::nano::graphics::device > pdevice = pnanodevice;

               IDWriteFactory * pfactory = m_pdirect2d->dwrite_factory();

               DWRITE_FONT_STYLE style;

               //if (m_bItalic)
               //{

               //   style = DWRITE_FONT_STYLE_ITALIC;

               //}
               //else
               //{

                  style = DWRITE_FONT_STYLE_NORMAL;

               //}

               DWRITE_FONT_STRETCH stretch;

               stretch = DWRITE_FONT_STRETCH_NORMAL;

               float fFontSize;

               //::acme::windowing::window * pacmewindowingwindow = nullptr;
               //
               //if (::is_set(pgraphics))
               //{
               // 
               //   oswindow = pgraphics->get_window_handle();

               //}

               //if (m_fontsize.eunit() == ::e_unit_point)
               {

                 // fFontSize = (float)pgraphics->m_pdraw2dhost->point_dpi(m_fontsize.as_double());

               }
               //else
               {

                  //fFontSize = (float)pgraphics->m_pdraw2dhost->dpiy(m_fontsize.as_double());

                  //fFontSize = (float)pgraphics->m_pdraw2dhost->dpiy(m_iFontSize);

                  fFontSize = (float)m_iFontSize;

               }

               //if (::is_set(pgraphics))
               //{

               //   fFontSize *= (float)pgraphics->m_dFontFactor;

               //}

               if (fFontSize <= 0.000001)
               {

                  //return false;
                  throw ::exception(error_failed);


               }

               HRESULT hr = pfactory->CreateTextFormat(
                  wstring(m_strFontName),
                  nullptr,
                  dwrite_font_weight(
                     { m_bBold ?
                     e_font_weight_bold :
                     e_font_weight_normal }),
                  style,
                  stretch,
                  fFontSize,
                  L"",
                  &m_pformat);

               if (FAILED(hr) || m_pformat == nullptr)
               {

                  warning() << "font::get_os_font: " << hresult_text(hr);

                  //return false;

                  throw ::exception(error_failed);

               }
               //m_hgdiobj = _create_point_font(m_iFontSize * 10, m_strFontName, m_bBold, pwindowsnanodevice->m_hdc, &logfontw);

               m_bModified = false;

            }


         }


         //HFONT font::_create_point_font(int nPointSize, const ::scoped_string& scopedstrFaceName, bool bBold, HDC hdc, LOGFONTW* plf)
         //{

         //   LOGFONTW lF;

         //   if (plf == nullptr)
         //   {

         //      plf = &lF;

         //      ::memory_set(plf, 0, sizeof(*plf));

         //      plf->lfCharSet = DEFAULT_CHARSET;

         //   }

         //   plf->lfHeight = nPointSize;

         //   plf->lfWeight = bBold ? FW_BOLD : FW_NORMAL;

         //   wstring wstr(scopedstrFaceName);

         //   wstr = wstr.substr(0, sizeof(plf->lfFaceName));

         //   wcsncpy(plf->lfFaceName, wstr, sizeof(plf->lfFaceName) / sizeof(wchar_t));

         //   return _create_point_font_indirect(plf, hdc);

         //}


         //// pLogFont->nHeight is interpreted as PointSize * 10
         //HFONT font::_create_point_font_indirect(LOGFONTW* pLogFont, HDC hdc)
         //{

         //   LOGFONTW& logFont = *pLogFont;


         //   ::int_point point;
         //   // 72 points/inch, 10 decipoints/int_point
         //   point.y = ::MulDiv(::GetDeviceCaps(hdc, LOGPIXELSY), logFont.lfHeight, 720);
         //   point.x = 0;
         //   ::DPtoLP(hdc, (POINT*)&point, 1);
         //   ::int_point pointOrg = { 0, 0 };
         //   ::DPtoLP(hdc, (POINT*)&pointOrg, 1);
         //   logFont.lfHeight = -abs(point.y - pointOrg.y);

         //   logFont.lfQuality = CLEARTYPE_NATURAL_QUALITY;

         //   HFONT hfont = ::CreateFontIndirectW(&logFont);

         //   if (::GetObjectW(hfont, sizeof(logFont), pLogFont))
         //   {

         //      ::informationf("got log font");

         //   }

         //   return hfont;

         //}



      } // namespace graphics


   } // namespace nano


} // namespace universal_windows



