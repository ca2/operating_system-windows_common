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
      int_map < comptr<ID2D1GeometryRealization> > m_mapGeometryHollowRealization;
      comptr<ID2D1GeometryRealization>    m_geometryFilledRealization;


      path();
      ~path() override;


      void create(::draw2d::graphics * pgraphics, char iCreate) override;
      void destroy() override;
      void destroy_os_data() override;


      //virtual bool internal_begin_figure(bool bFill, ::draw2d::enum_fill_mode efillmode);
      virtual bool internal_end_figure(bool bClose);
      void * detach() override;

      //virtual bool is_empty();
      //virtual bool has_current_point();
      //virtual int_point current_point();

      ID2D1GeometryRealization * _get_stroked_geometry_realization(::draw2d::graphics * pgraphics, int iWidth);
      ID2D1GeometryRealization * _get_filled_geometry_realization(::draw2d::graphics * pgraphics);

      virtual bool internal_add_arc(::draw2d::graphics * pgraphics, const ::double_arc & arc);

      virtual bool internal_add_line(::draw2d::graphics* pgraphics, double x, double y);

      virtual bool internal_add_rectangle(::draw2d::graphics* pgraphics, double x, double y, double cx, double cy);

      virtual bool internal_add_ellipse(::draw2d::graphics * pgraphics, double x, double y, double cx, double cy);

      virtual bool internal_add_lines(::draw2d::graphics* pgraphics, const ::int_point_array & pointa, bool bClose);

      virtual bool internal_add_lines(::draw2d::graphics* pgraphics, const ::double_point_array& pointa, bool bClose);

      virtual bool internal_add_string(::draw2d_direct2d::graphics * pgraphics, double x, double y, const ::scoped_string & scopedstrText, ::write_text::font * pfont);

      virtual bool internal_start_figure(::draw2d::graphics * pgraphics);

      virtual bool internal_start_figure(::draw2d::graphics* pgraphics, double x, double y);

      virtual bool internal_get_arc(::double_point & pointStart, D2D1_ARC_SEGMENT & arcseg, const ::double_arc & arc);

      virtual void * detach(::draw2d::graphics * pgraphics);

      virtual bool create();

      //virtual bool _set(::draw2d::graphics* pgraphics, ::draw2d::path::begin * pbegin) override;
      //virtual bool _set(::draw2d::graphics* pgraphics, ::draw2d::path::arc * parc) override;
      ////virtual bool _set(::draw2d::graphics* pgraphics, const ::draw2d::path::move & move);
      //virtual bool _set(::draw2d::graphics* pgraphics, ::draw2d::path::int_rectangle* prectangle) override;
      //virtual bool _set(::draw2d::graphics* pgraphics, ::draw2d::path::line * pline) override;
      //virtual bool _set(::draw2d::graphics* pgraphics, ::draw2d::path::lines* plines) override;
      //virtual bool _set(::draw2d::graphics* pgraphics, ::draw2d::path::int_polygon* pline) override;
      //virtual bool _set(::draw2d::graphics * pgraphics, ::draw2d::path::text_out * ptextout) override;
      //virtual bool _set(::draw2d::graphics* pgraphics, ::draw2d::path::draw_text * pdrawtext) override;
      //virtual bool _set(::draw2d::graphics* pgraphics, ::draw2d::path::close* pclose) override;


      virtual bool _set(::draw2d::graphics* pgraphics, const ::draw2d::enum_item& eitem);

      virtual bool _set(::draw2d::graphics* pgraphics, const ::double_arc& parc);

      virtual bool _set(::draw2d::graphics* pgraphics, const ::double_line& pline);

      virtual bool _set(::draw2d::graphics* pgraphics, const ::double_lines& pline);

      virtual bool _set(::draw2d::graphics* pgraphics, const ::double_rectangle & rectangle);

      virtual bool _set(::draw2d::graphics* pgraphics, const ::double_ellipse & ellipse);

      virtual bool _set(::draw2d::graphics* pgraphics, const ::double_polygon & polygon);

      virtual bool _set(::draw2d::graphics* pgraphics, const ::write_text::text_out& ptextout);

      virtual bool _set(::draw2d::graphics* pgraphics, const ::write_text::draw_text& pdrawtext);


      //static void CreatePathTextRenderer(FLOAT pixelsPerDip,IDWriteTextRenderer **textRenderer);


   };


} // namespace draw2d_direct2d



