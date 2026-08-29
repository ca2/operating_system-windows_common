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


      comptr < IDWriteFontCollection >    m_pdwritefontcollection;
      comptr < IDWriteFontFamily >        m_pdwritefontfamily;
      comptr < IDWriteTextFormat >        m_pdwritetextformat;
      comptr < IDWriteFont >              m_pdwritefont;


      font();
      ~font() override;


      //void dump(dump_context& dumpcontext) const override;

      // virtual IDWriteTextFormat* get_os_data(::draw2d::graphics * pdraw2dgraphics, ::collection::index i) const;
      //virtual void * _get_os_data(::draw2d::graphics * pdraw2dgraphics, ::collection::index i) const override;

      void clear_node_data() override;
      // void destroy_os_data() override;
      void update(::draw2d::graphics * pdraw2dgraphics) override;
      void create_text_metrics(::draw2d::graphics * pdraw2dgraphics) override;



      DWRITE_FONT_STYLE _dwrite_font_style();
      DWRITE_FONT_STRETCH _dwrite_font_stretch();
      DWRITE_FONT_WEIGHT _dwrite_font_weight();
      ::f32 _dwrite_font_size(::draw2d::graphics * pdraw2dgraphics);

      /*      virtual void construct(const ::write_text::font & pfontParam);

            virtual bool CreateFontIndirect(const LOGFONTW* lpLogFont);
            virtual bool CreateFont(::i32 nHeight, ::i32 nWidth, ::i32 nEscapement,
                  ::i32 nOrientation, ::i32 nWeight, ::u8 bItalic, ::u8 bUnderline,
                  ::u8 cStrikeOut, ::u8 nCharSet, ::u8 nOutPrecision,
                  ::u8 nClipPrecision, ::u8 nQuality, ::u8 nPitchAndFamily,
                  const_char_pointer pszFacename);
            virtual bool CreatePointFont(::i32 nPointSize, const ::scoped_string & scopedstrFaceName, const ::draw2d::graphics * pdraw2dgraphics = nullptr);
            virtual bool CreatePointFontIndirect(const LOGFONTW* lpLogFont, const ::draw2d::graphics * pdraw2dgraphics = nullptr);

            virtual ::i32 GetLogFont(LOGFONTW* pLogFont);*/

      

      //bool destroy();


   };


} // namespace draw2d_direct2d




