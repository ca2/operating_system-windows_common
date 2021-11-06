#pragma once


namespace draw2d_directx
{


   class CLASS_DECL_DRAW2D_DIRECTX font :
      virtual public ::draw2d_directx::object,
      virtual public ::write_text::font
   {
   public:


      comptr < IDWriteTextFormat >    m_pformat;


      font();
      virtual ~font();


      // virtual IDWriteTextFormat* get_os_data(::draw2d::graphics * pgraphics, index i) const;
      //virtual void * _get_os_data(::draw2d::graphics * pgraphics, index i) const override;

      virtual void destroy() override;
      virtual bool create(::draw2d::graphics * pgraphics, i8 iCreate) override;


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

      void dump(dump_context & dumpcontext) const override;

      //bool destroy();


   };


} // namespace draw2d_directx




