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


      comptr<ID2D1PathGeometry>           m_ppathHollow;
      comptr<ID2D1PathGeometry>           m_ppathFilled;
      comptr<ID2D1PathGeometry>           m_ppath;
      comptr<ID2D1GeometrySink>           m_psink;
      D2D1_FIGURE_BEGIN                   m_d2d2figurebegin;
      // width 
      i32_map < comptr<ID2D1GeometryRealization> > m_mapGeometryHollowRealization;
      comptr<ID2D1GeometryRealization>    m_geometryFilledRealization;


      path();
      ~path() override;


      void create(::draw2d::graphics * pgraphics, i8 iCreate) override;
      void destroy() override;
      void destroy_os_data() override;


      //virtual bool internal_begin_figure(bool bFill, ::draw2d::enum_fill_mode efillmode);
      virtual bool internal_end_figure(bool bClose);
      void * detach() override;

      //virtual bool is_empty();
      //virtual bool has_current_point();
      //virtual point_i32 current_point();

      ID2D1GeometryRealization * _get_stroked_geometry_realization(::draw2d::graphics * pgraphics, int iWidth);
      ID2D1GeometryRealization * _get_filled_geometry_realization(::draw2d::graphics * pgraphics);

      virtual bool internal_add_arc(::draw2d::graphics * pgraphics, const ::arc_f64 & arc);

      virtual bool internal_add_line(::draw2d::graphics* pgraphics, double x, double y);

      virtual bool internal_add_rectangle(::draw2d::graphics* pgraphics, double x, double y, double cx, double cy);

      virtual bool internal_add_ellipse(::draw2d::graphics * pgraphics, double x, double y, double cx, double cy);

      virtual bool internal_add_lines(::draw2d::graphics* pgraphics, const ::point_i32_array & pointa, bool bClose);

      virtual bool internal_add_lines(::draw2d::graphics* pgraphics, const ::point_f64_array& pointa, bool bClose);

      virtual bool internal_add_string(::draw2d_direct2d::graphics * pgraphics, double x, double y, const ::string & strText, ::write_text::font * pfont);

      virtual bool internal_start_figure(::draw2d::graphics * pgraphics);

      virtual bool internal_start_figure(::draw2d::graphics* pgraphics, double x, double y);

      virtual bool internal_get_arc(::point_f64 & pointStart, D2D1_ARC_SEGMENT & arcseg, const ::arc_f64 & arc);

      virtual void * detach(::draw2d::graphics * pgraphics);

      virtual bool create();

      //virtual bool _set(::draw2d::graphics* pgraphics, ::draw2d::path::begin * pbegin) override;
      //virtual bool _set(::draw2d::graphics* pgraphics, ::draw2d::path::arc * parc) override;
      ////virtual bool _set(::draw2d::graphics* pgraphics, const ::draw2d::path::move & move);
      //virtual bool _set(::draw2d::graphics* pgraphics, ::draw2d::path::rectangle_i32* prectangle) override;
      //virtual bool _set(::draw2d::graphics* pgraphics, ::draw2d::path::line * pline) override;
      //virtual bool _set(::draw2d::graphics* pgraphics, ::draw2d::path::lines* plines) override;
      //virtual bool _set(::draw2d::graphics* pgraphics, ::draw2d::path::polygon_i32* pline) override;
      //virtual bool _set(::draw2d::graphics * pgraphics, ::draw2d::path::text_out * ptextout) override;
      //virtual bool _set(::draw2d::graphics* pgraphics, ::draw2d::path::draw_text * pdrawtext) override;
      //virtual bool _set(::draw2d::graphics* pgraphics, ::draw2d::path::close* pclose) override;


      virtual bool _set(::draw2d::graphics* pgraphics, const ::draw2d::enum_item& eitem);

      virtual bool _set(::draw2d::graphics* pgraphics, const ::arc_f64& parc);

      virtual bool _set(::draw2d::graphics* pgraphics, const ::line_f64& pline);

      virtual bool _set(::draw2d::graphics* pgraphics, const ::lines_f64& pline);

      virtual bool _set(::draw2d::graphics* pgraphics, const ::rectangle_f64 & rectangle);

      virtual bool _set(::draw2d::graphics* pgraphics, const ::ellipse_f64 & ellipse);

      virtual bool _set(::draw2d::graphics* pgraphics, const ::polygon_f64 & polygon);

      virtual bool _set(::draw2d::graphics* pgraphics, const ::write_text::text_out& ptextout);

      virtual bool _set(::draw2d::graphics* pgraphics, const ::write_text::draw_text& pdrawtext);


      //static void CreatePathTextRenderer(FLOAT pixelsPerDip,IDWriteTextRenderer **textRenderer);


   };


} // namespace draw2d_direct2d



