#pragma once


#include "aura/graphics/write_text/font.h"
#include "object.h"


namespace draw2d_direct2d
{


   class CLASS_DECL_DRAW2D_DIRECT2D font :
      virtual public ::draw2d_direct2d::object,
      virtual public ::write_text::font
   {
   public:


      comptr < IDWriteTextFormat >    m_pformat;


      font();
      ~font() override;


      void dump(dump_context& dumpcontext) const override;

      // virtual IDWriteTextFormat* get_os_data(::draw2d::graphics * pgraphics, index i) const;
      //virtual void * _get_os_data(::draw2d::graphics * pgraphics, index i) const override;

      void destroy() override;
      void destroy_os_data() override;
      void create(::draw2d::graphics * pgraphics, i8 iCreate) override;
      void create_text_metrics(::draw2d::graphics * pgraphics) override;

      /*      virtual void construct(const ::write_text::font & pfontParam);

            virtual bool CreateFontIndirect(const LOGFONTW* lpLogFont);
            virtual bool CreateFont(int nHeight, int nWidth, int nEscapement,
                  int nOrientation, int nWeight, byte bItalic, byte bUnderline,
                  byte cStrikeOut, byte nCharSet, byte nOutPrecision,
                  byte nClipPrecision, byte nQuality, byte nPitchAndFamily,
                  const char * lpszFacename);
            virtual bool CreatePointFont(int nPointSize, const ::string & lpszFaceName, const ::draw2d::graphics * pgraphics = nullptr);
            virtual bool CreatePointFontIndirect(const LOGFONTW* lpLogFont, const ::draw2d::graphics * pgraphics = nullptr);

            virtual int GetLogFont(LOGFONTW* pLogFont);*/

      

      //bool destroy();


   };


} // namespace draw2d_direct2d




