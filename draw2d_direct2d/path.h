#pragma once


#include "aura/graphics/draw2d/path.h"
#include "object.h"


namespace draw2d_direct2d
{


   enum e_path
   {

      path_hollow,
      path_filled,
      path_hollow_geometry_realization,
      path_filled_geometry_realization,

   };


   class CLASS_DECL_DRAW2D_DIRECT2D path :
      virtual public ::draw2d_direct2d::object,
      virtual public ::draw2d::path
   {
   public:


      comptr<ID2D1PathGeometry>                       m_pd2d1pathgeometryHollow1;
      comptr<ID2D1PathGeometry>                       m_pd2d1pathgeometryFilled1;
      comptr<ID2D1PathGeometry>                       m_pd2d1pathgeometry;
      comptr<ID2D1GeometrySink>                       m_pd2d1geometrysink;
      D2D1_FIGURE_BEGIN                               m_d2d2figurebegin;
      // width 
      i32_map < comptr<ID2D1GeometryRealization> >    m_mapD2D1HollowGeometryRealization;
      comptr<ID2D1GeometryRealization>                m_pd2d1geometryrealizationFilled;


      path();
      ~path() override;


      void update(::draw2d::graphics * pdraw2dgraphics) override;
      void destroy() override;
      // void destroy_os_data() override;

      virtual ID2D1PathGeometry * _hollow_path_geometry(::draw2d::graphics * pdraw2dgraphics);
      virtual ID2D1PathGeometry * _filled_path_geometry(::draw2d::graphics * pdraw2dgraphics);

      //virtual bool internal_begin_figure(bool bFill, ::draw2d::enum_fill_mode efillmode);
      virtual bool internal_end_figure(bool bClose);
      //void * detach() override;

      //virtual bool is_empty();
      //virtual bool has_current_point();
      //virtual i32_point current_point();

      ID2D1GeometryRealization * _get_stroked_geometry_realization(::draw2d::graphics * pdraw2dgraphics, ::i32 iWidth);
      ID2D1GeometryRealization * _get_filled_geometry_realization(::draw2d::graphics * pdraw2dgraphics);

      virtual bool internal_add_arc(::draw2d::graphics * pdraw2dgraphics, const ::f64_arc & arc);

      virtual bool internal_add_line(::draw2d::graphics * pdraw2dgraphics, ::f64 x, ::f64 y);

      virtual bool internal_add_rectangle(::draw2d::graphics * pdraw2dgraphics, ::f64 x, ::f64 y, ::f64 cx, ::f64 cy);

      virtual bool internal_add_ellipse(::draw2d::graphics * pdraw2dgraphics, ::f64 x, ::f64 y, ::f64 cx, ::f64 cy);

      virtual bool internal_add_lines(::draw2d::graphics * pdraw2dgraphics, const ::i32_point_array & pointa, bool bClose);

      virtual bool internal_add_lines(::draw2d::graphics * pdraw2dgraphics, const ::f64_point_array& pointa, bool bClose);

      virtual bool internal_add_string(::draw2d_direct2d::graphics * pdraw2dgraphics, ::f64 x, ::f64 y, const ::scoped_string & scopedstrText, ::write_text::font * pwritetextfont);

      virtual bool internal_start_figure(::draw2d::graphics * pdraw2dgraphics);

      virtual bool internal_start_figure(::draw2d::graphics * pdraw2dgraphics, ::f64 x, ::f64 y);

      virtual bool internal_get_arc(::f64_point & pointStart, D2D1_ARC_SEGMENT & arcseg, const ::f64_arc & arc);

      virtual void * detach(::draw2d::graphics * pdraw2dgraphics);

      virtual bool create();

      //virtual bool _set(::draw2d::graphics * pdraw2dgraphics, ::draw2d::path::begin * pbegin) override;
      //virtual bool _set(::draw2d::graphics * pdraw2dgraphics, ::draw2d::path::arc * parc) override;
      ////virtual bool _set(::draw2d::graphics * pdraw2dgraphics, const ::draw2d::path::move & move);
      //virtual bool _set(::draw2d::graphics * pdraw2dgraphics, ::draw2d::path::i32_rectangle* prectangle) override;
      //virtual bool _set(::draw2d::graphics * pdraw2dgraphics, ::draw2d::path::line * pline) override;
      //virtual bool _set(::draw2d::graphics * pdraw2dgraphics, ::draw2d::path::lines* plines) override;
      //virtual bool _set(::draw2d::graphics * pdraw2dgraphics, ::draw2d::path::i32_polygon* pline) override;
      //virtual bool _set(::draw2d::graphics * pdraw2dgraphics, ::draw2d::path::text_out * ptextout) override;
      //virtual bool _set(::draw2d::graphics * pdraw2dgraphics, ::draw2d::path::draw_text * pdrawtext) override;
      //virtual bool _set(::draw2d::graphics * pdraw2dgraphics, ::draw2d::path::close* pclose) override;


      virtual bool _set(::draw2d::graphics * pdraw2dgraphics, const ::draw2d::enum_item& eitem);

      virtual bool _set(::draw2d::graphics * pdraw2dgraphics, const ::f64_arc& parc);

      virtual bool _set(::draw2d::graphics * pdraw2dgraphics, const ::f64_line& pline);

      virtual bool _set(::draw2d::graphics * pdraw2dgraphics, const ::f64_lines& pline);

      virtual bool _set(::draw2d::graphics * pdraw2dgraphics, const ::f64_rectangle & rectangle);

      virtual bool _set(::draw2d::graphics * pdraw2dgraphics, const ::f64_ellipse & ellipse);

      virtual bool _set(::draw2d::graphics * pdraw2dgraphics, const ::f64_polygon & polygon);

      virtual bool _set(::draw2d::graphics * pdraw2dgraphics, const ::write_text::text_out& ptextout);

      virtual bool _set(::draw2d::graphics * pdraw2dgraphics, const ::write_text::draw_text& pdrawtext);


      //static void CreatePathTextRenderer(FLOAT pixelsPerDip,IDWriteTextRenderer **textRenderer);


   };


} // namespace draw2d_direct2d



