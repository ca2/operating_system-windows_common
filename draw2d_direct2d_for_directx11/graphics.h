#pragma once


#include "bred/gpu/graphics.h"
#include "draw2d_direct2d/graphics.h"
#include "acme_windows_common/dxgi_surface_bindable.h"
#include "direct2d/object.h"
#include <dxgi1_2.h>


namespace draw2d_direct2d_for_directx11
{


   class CLASS_DECL_DRAW2D_DIRECT2D_FOR_DIRECTX11 graphics :
      virtual public ::gpu::graphics,
      virtual public ::draw2d_direct2d::graphics,
      virtual public ::dxgi_surface_bindable
   {
   public:
      //i32_array_base m_iaPushLayer;
      //i32_array_base m_iaPushLayerCount;
      //class state
      //{
      //public:


      //   D2D1::Matrix3x2F                    m_m;
      //   ::i32                                 m_iLayerIndex;
      //   D2D1_LAYER_PARAMETERS               m_layerparameters;

      //};

      ::pointer < ::particle > m_pSwapChainForBlitting;
      ::array < ::array < ::comptr < IDXGISurface > > > m_dxgisurfaceaBound;

      //::i32                                                m_iLayerCount;

      //comptr<ID2D1Device>                                m_pdevice;
      //comptr<ID2D1DeviceContext>                         m_pdevicecontext; // 0
      //comptr<ID2D1RenderTarget>                          m_pd2d1rendertarget; // 1
      //array < array < comptr<ID2D1RenderTarget> > >      m_d2d1rendertargeta; // 1
      //comptr<ID2D1BitmapRenderTarget>                    m_pbitmaprendertarget; // 2
      //comptr<ID2D1DCRenderTarget>                        m_pdcrendertarget; // 3
      //comptr<ID2D1DeviceContext1>                        m_pdevicecontext1; // 4

      comptr<IDXGISurface>                               m_pdxgisurface;
      //comptr < ID2D1Bitmap1>                              m_pd2d1bitmap;

      //::array<::array <comptr < ID2D1Bitmap1>   > >          m_d2d1bitmapa;

      comptr<IDXGIAdapter>                               m_padapter;
      comptr<IDXGIFactory2>                              m_pfactory2;
      //comptr<ID2D1Layer>                                 m_player;
      //comptr<ID2D1PathGeometry>                          m_ppathgeometryClip;

      //::array < state >                                  m_statea;
      //state                                              m_state;
      //bool                                               m_bSaveClip;

      //::i32                                                m_iType;

      //D2D1_BITMAP_INTERPOLATION_MODE                     m_bitmapinterpolationmode;
      //D2D1_INTERPOLATION_MODE                            m_interpolationmode;

      //::write_text::enum_rendering                    m_etextrenderinghintDevice;


      //// advanced use and implementation
      //bool                                               m_bPrinting;

      //HDC                                                m_hdcAttach;
      //::draw2d::enum_alpha_mode                          m_ealphamodeDevice;


      graphics();
      ~graphics() override;



      void gpu_layer_on_after_begin_render() override;
      void gpu_layer_on_before_end_render() override;


      void on_acquire_memory_graphics(::image::image * pimage, const ::i32_size & size,
::acme::user::interaction * pacmeuserinteractionAffinity) override;


      bool TextOutAlphaBlend(::f64 x, ::f64 y, const ::scoped_string & scopedstr) override;
      bool _draw_blend(const ::image::image_drawing & imagedrawing) override;


      //bool IsPrinting() override;            // true if being used for printing

      //void start_layer(::e_graphics egraphics) override;
      //void end_layer(::e_graphics egraphics) override;
      void start_layer(bool bFirstLayer = false) override;
      void end_layer(bool bClosingLayer = false) override;
      
      //void on_begin_draw1() override;
      //void on_end_draw1() override;


      void just_after_new_frame() override;


      void set_target_image(::image::image * pimage) override;
      //void start_gpu_layer(::gpu::layer * pgpulayer) override;
      //::gpu::frame* end_gpu_layer(::gpu::layer* pgpulayer) override;


      using ::draw2d::graphics::set;
      void set(::draw2d::bitmap* pbitmap) override;


      //oswindow get_window_handle();

      void apply_primitive_blend();

      void apply_text_rendering_hint();

      inline void defer_primitive_blend() 
      {
         
         if (m_ealphamode != m_ealphamodeDevice)
         {

            apply_primitive_blend();

         }
      
      }

      inline void defer_text_rendering_hint()
      {

         if (m_ewritetextrendering != m_etextrenderinghintDevice)
         {

            apply_primitive_blend();

         }

      }

      ::draw2d::bitmap *get_target_bitmap() override;

      void defer_text_primitive_blend();

      // for bidi and mirrored localization
      ::u32 GetLayout() override;
      ::u32 SetLayout(::u32 dwLayout) override;

      void on_start_layer(::gpu::layer* pgpulayer) override;
      void on_end_layer(::gpu::layer* pgpulayer) override;

      //virtual bool save_clip() override;
      //virtual bool restore_clip() override;

      ::f64 get_dpix() override;

      //// Constructors
      //bool CreateDC(const ::scoped_string & scopedstrDriverName, const ::scoped_string & scopedstrDeviceName,
      //              const_char_pointer pszOutput, const void * lpInitData) override;
      //bool CreateIC(const ::scoped_string & scopedstrDriverName, const ::scoped_string & scopedstrDeviceName,
      //              const_char_pointer pszOutput, const void * lpInitData) override;
      //void create_compatible_graphics(::draw2d::graphics * pgraphics) override;
      virtual void _create_from_dxgi_surface(::i32 iIndex, ::i32 iLayerIndex, IDXGISurface* pdxgisurface);
      void _create_memory_graphics(const ::i32_size & size = {}, ::acme::user::interaction * pacmeuserinteractionAffinity = nullptr) override;
      void create_bitmap_graphics(::draw2d::bitmap *pbitmap) override;
      void defer_set_size(const ::i32_size& size = {}) override;

      //void create_offscreen_graphics_for_swap_chain_blitting(const ::i32_size& size) override;
      void create_for_window_draw2d(::user::interaction* puserinteraction, const ::i32_size & size) override;
      //void create_connector() override;

      void DeleteDC() override;

      // Device-Context Functions
      ::i32 save_graphics_context() override;
      void restore_graphics_context(::i32 iSavedGraphicsContext) override;
      //::i32 GetDeviceCaps(::i32 nIndex) override;
      ::u32 SetBoundsRect(const ::f64_rectangle & rectangleBounds, ::u32 flags) override;
      ::u32 GetBoundsRect(::f64_rectangle * rectangleBounds, ::u32 flags) override;
//      bool ResetDC(const DEVMODE* lpDevMode) override;


      //void gpu_layer_on_after_begin_render() override;
      //void gpu_layer_on_before_end_render() override;

      // Drawing-Tool Functions
      ::f64_point GetBrushOrg() override;
      ::f64_point SetBrushOrg(::f64 x, ::f64 y) override;
      ::f64_point SetBrushOrg(const ::f64_point & point) override;
//      ::i32 EnumObjects(::i32 nObjectType,
  //                    ::i32 (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData) override;

      //virtual ::draw2d::object* set_stock_object(::i32 nIndex) override;
      //virtual void set(::write_text::font * pfont) override;
      //virtual void set(::draw2d::pen* pPen) override;
      //virtual void set(::draw2d::brush* pBrush) override;
      //virtual void set(::write_text::font* pFont) override;
      //virtual void set(::draw2d::bitmap* pBitmap) override;
      //virtual ::i32 SelectObject(::draw2d::region* pRgn) override;       // special return for regions
      //::draw2d::object* SelectObject(::draw2d::object* pObject) override;
      // ::draw2d::object* provided so compiler doesn't use SelectObject(HGDIOBJ)

      //// color and color Palette Functions
      //::color::color GetNearestColor(const ::color::color & color) override;
      //// ::draw2d::palette* SelectPalette(::draw2d::palette* pPalette, bool bForceBackground) override;
      //::u32 RealizePalette() override;
      //void UpdateColors() override;

      //::i32 GetPolyFillMode() override;
      //::i32 GetROP2() override;
      //::i32 GetStretchBltMode() override;

      //::i32 SetPolyFillMode(::i32 nPolyFillMode) override;
      //::i32 SetROP2(::i32 nDrawMode) override;
      void set_interpolation_mode(::draw2d::enum_interpolation_mode einterpolationmode) override;


//#if (_WIN32_WINNT >= 0x0500)
//
//      ::color::color GetDCBrushColor() override;
//      ::color::color SetDCBrushColor(const ::color::color & color) override;
//
//      ::color::color GetDCPenColor() override;
//      ::color::color SetDCPenColor(const ::color::color & color) override;
//
//#endif

      // Graphics mode
      //::i32 SetGraphicsMode(::i32 iMode) override;
      //::i32 GetGraphicsMode() override;


      void _get(::geometry2d::matrix & matrix) override;
      void _set(const ::geometry2d::matrix & matrix) override;

      void clear_current_point() override;

      void draw(::draw2d::path * ppath) override;
      void fill(::draw2d::path * ppath) override;
      void path(::draw2d::path * ppath) override;

      virtual bool draw(ID2D1PathGeometry * pgeometry, ::draw2d::pen * ppen);
      virtual bool fill(ID2D1PathGeometry * pgeometry, ::draw2d::brush * pbrush);
      virtual bool draw(ID2D1GeometryRealization * prealization, ::draw2d::pen * ppen);
      virtual bool fill(ID2D1GeometryRealization * prealization, ::draw2d::brush * ppen);

      void draw(::draw2d::path * ppath, ::draw2d::pen * ppen) override;
      void fill(::draw2d::path * ppath, ::draw2d::brush * pbrush) override;

      // World transform
      //bool SetWorldTransform(const XFORM* pXform) override;
      //bool ModifyWorldTransform(const XFORM* pXform,::u32 iMode) override;
      //bool GetWorldTransform(XFORM* pXform) override;

      // Mapping Functions
      //virtual ::i32 GetMapMode() override;
      //::f64_point get_origin() override;
      //virtual ::i32 SetMapMode(::i32 nMapMode) override;
      // Viewport Origin
      //::f64_point set_origin(::f64 x, ::f64 y) override;
      //::f64_point set_origin(const ::f64_point & point) override;
      //::f64_point offset_origin(::f64 nWidth, ::f64 nHeight) override;

      // Viewport Extent
      ::f64_size get_extents() override;
      ::f64_size set_extents(::f64 cx, ::f64 cy) override;
      ::f64_size set_extents(const ::f64_size & size) override;
      ::f64_size scale_extents(::f64 xNum, ::f64 xDenom, ::f64 yNum, ::f64 yDenom) override;

      // Window Origin
      ::f64_point GetWindowOrg() override;
      ::f64_point SetWindowOrg(::f64 x, ::f64 y) override;
      ::f64_point SetWindowOrg(const ::f64_point & point) override;
      ::f64_point offset_window_org(::f64 nWidth, ::f64 nHeight) override;

      // Window extent
      ::f64_size GetWindowExt() override;
      //::f64_size set_window_ext(::f64 cx, ::f64 cy) override;
      //::f64_size set_window_ext(const ::f64_size & size) override;
      //::f64_size scale_window_ext(::f64 xNum, ::f64 xDenom, ::f64 yNum, ::f64 yDenom) override;

      // Coordinate Functions
      void DPtoLP(::f64_point * ppoints, ::collection::count nCount = 1) override;
      void DPtoLP(::f64_rectangle * prectangle) override;
      void DPtoLP(::f64_size * psize) override;
      void LPtoDP(::f64_point * ppoints, ::collection::count nCount = 1) override;
      void LPtoDP(::f64_rectangle * prectangle) override;
      void LPtoDP(::f64_size * psize) override;

      // Special Coordinate Functions (useful for dealing with metafiles and OLE)
      virtual void DPtoHIMETRIC(::f64_size * LPSIZE32) override;
      //void LPtoHIMETRIC(::f64_size * LPSIZE32) override;
      virtual void HIMETRICtoDP(::f64_size * LPSIZE32) override;
      //void HIMETRICtoLP(::f64_size * LPSIZE32) override;

      // Region Functions
      void fill_region(::draw2d::region* pRgn, ::draw2d::brush* pBrush) override;
      void frame_region(::draw2d::region* pRgn, ::draw2d::brush* pBrush, ::f64 nWidth, ::f64 nHeight) override;
      void invert_region(::draw2d::region* pRgn) override;
      void paint_region(::draw2d::region* pRgn) override;

      // Clipping Functions
      ::i32 get_clip_box(::f64_rectangle & rectangle) override;


      virtual void _push_layer(ID2D1Geometry * pgeometry);
      virtual void _push_layer(const ::i32_rectangle_array_base & rectanglea);
      virtual void _pop_layer();
      virtual void _pop_all_layers();


      //virtual void add_shapes(const shape_array& shapea);
      //virtual void reset_clip();

      void reset_clip() override;
      void _intersect_clip() override;
      void _add_shape(const ::f64_rectangle & rectangle) override;
      void _add_shape(const ::f64_ellipse & ellipse) override;
      void _add_shape(const ::f64_polygon_base & polygon) override;


      virtual void intersect_clip(const ::draw2d::clip_group & clipgroup);
      virtual void _add_clip_item(::draw2d::clip_item * pclipitem);



      virtual void _add_clip_item(ID2D1GeometrySink * pgeometrysink, ::draw2d::clip_item * pclipitem);
      //virtual void _add_shape(Gdiplus::GraphicsPath * ppath, const ::f64_rectangle & rectangle);
      //virtual void _add_shape(Gdiplus::GraphicsPath * ppath, const ::f64_ellipse & ellipse);
      //virtual void _add_shape(Gdiplus::GraphicsPath * ppath, const ::f64_polygon & polygon);

      //void _intersect_clip() override;
      //void _add_clipping_shape(const ::f64_rectangle & rectangle) override;
      //void _add_clipping_shape(const ::f64_ellipse & ellipse) override;
      //void _add_clipping_shape(const ::f64_polygon & polygon) override;
      void intersect_clip(const ::f64_rectangle& rectangle) override;
      //virtual void intersect_clip(const ::oval& oval);
      //virtual void intersect_clip(const ::i32_polygon& i32_polygon);

      //virtual bool PtVisible(::f64 x, ::f64 y) override;
      //bool PtVisible(const ::f64_point & point) override;
      //virtual bool RectVisible(const ::f64_rectangle & rectangle) override;
      //::i32 SelectClipRgn(::draw2d::region* pRgn) override;
      //::i32 ExcludeClipRect(::f64 x1, ::f64 y1, ::f64 x2, ::f64 y2) override;
      //::i32 ExcludeClipRect(const ::f64_rectangle & rectangle) override;
      ////::i32 ExcludeUpdateRgn(::user::prototype * pwindow) override;
      //::i32 IntersectClipRect(::f64 x1, ::f64 y1, ::f64 x2, ::f64 y2) override;
      //::i32 IntersectClipRect(const ::f64_rectangle & rectangle) override;
      //::i32 OffsetClipRgn(::f64 x, ::f64 y) override;
      //::i32 OffsetClipRgn(const ::f64_size & size) override;
      //::i32 SelectClipRgn(::draw2d::region* pRgn, ::draw2d::enum_combine ecombine) override;

      // Line-Output Functions
      ::f64_point current_position() override;
//      ::f64_point move_to(::f64 x, ::f64 y) override;
      //    ::f64_point move_to(const ::f64_point & point) override;
      using ::draw2d::graphics::line;
      void line(::f64 x1, ::f64 y1, ::f64 x2, ::f64 y2) override;
      void line(::f64 x1, ::f64 y1, ::f64 x2, ::f64 y2, ::draw2d::pen * ppen) override;
      //  bool line_to(const ::f64_point & point) override;
      void arc(::f64 x1, ::f64 y1, ::f64 w, ::f64 h, ::f64_angle start, ::f64_angle extends) override;
      void arc(::f64 x1, ::f64 y1, ::f64 x2, ::f64 y2, ::f64 x3, ::f64 y3, ::f64 x4, ::f64 y4) override;
      //bool Arc(::f64 x1, ::f64 y1, ::f64 x2, ::f64 y2, ::f64 x3, ::f64 y3, ::f64 x4, ::f64 y4) override;
      void arc(const ::f64_rectangle & rectangle, const ::f64_point & pointStart, const ::f64_point & pointEnd) override;
      void polyline(const ::f64_point* ppoints, ::collection::count nCount) override;

      void angle_arc(::f64 x, ::f64 y, ::f64 nRadius, ::f64_angle fStartAngle, ::f64_angle fSweepAngle) override;
      //void arc(::f64 x1, ::f64 y1, ::f64 x2, ::f64 y2, ::f64 x3, ::f64 y3, ::f64 x4, ::f64 y4) override;
      //void arc(const ::f64_rectangle & rectangle, const ::f64_point & pointStart, const ::f64_point & pointEnd) override;
      //::i32 GetArcDirection() override;
      //::i32 SetArcDirection(::i32 nArcDirection) override;

      void polydraw(const ::f64_point* ppoints, const ::u8* lpTypes, ::collection::count nCount) override;
      void polyline_to(const ::f64_point* ppoints, ::collection::count nCount) override;
      void poly_polyline(const ::f64_point* ppoints, const ::i32 * lpPolyPoints, ::collection::count nCount) override;

      void poly_bezier(const ::f64_point* ppoints, ::collection::count nCount) override;
      void poly_bezier_to(const ::f64_point* ppoints, ::collection::count nCount) override;


      void frame_rectangle(const ::f64_rectangle & rectangle, ::draw2d::brush* pBrush) override;


      void invert_rectangle(const ::f64_rectangle & rectangle) override;

      //virtual bool draw(const ::f64_rectangle & rectangleTarget, ::image::icon * picon) override;

      //bool DrawIcon(::f64 x, ::f64 y, ::image::icon * picon) override;
      //bool DrawIcon(const ::f64_point & point, ::image::icon * picon) override;
      //bool DrawIcon(::f64 x, ::f64 y, ::image::icon * picon, ::f64 cx, ::f64 cy, ::u32 istepIfAniCur, HBRUSH hbrFlickerFreeDraw, ::u32 diFlags) override;
//      bool DrawState(const ::f64_point & point, const ::f64_size & size, HBITMAP hBitmap, ::u32 nFlags,
//                     HBRUSH hBrush = nullptr) override;
//      bool DrawState(const ::f64_point & point, const ::f64_size & size, ::draw2d::bitmap* pBitmap, ::u32 nFlags,
//                     ::draw2d::brush* pBrush = nullptr) override;
//      bool DrawState(const ::f64_point & point, const ::f64_size & size, HICON hIcon, ::u32 nFlags,
//                     HBRUSH hBrush = nullptr) override;
//      bool DrawState(const ::f64_point & point, const ::f64_size & size, HICON hIcon, ::u32 nFlags,
//                     ::draw2d::brush* pBrush = nullptr) override;
//      bool DrawState(const ::f64_point & point, const ::f64_size & size, const ::scoped_string & scopedstrText, ::u32 nFlags,
//                     bool bPrefixText = true, ::i32 nTextLen = 0, HBRUSH hBrush = nullptr) override;
//      bool DrawState(const ::f64_point & point, const ::f64_size & size, const ::scoped_string & scopedstrText, ::u32 nFlags,
//                     bool bPrefixText = true, ::i32 nTextLen = 0, ::draw2d::brush* pBrush = nullptr) override;
//      bool DrawState(const ::f64_point & point, const ::f64_size & size, DRAWSTATEPROC lpDrawProc,
      //       LPARAM lData, ::u32 nFlags, HBRUSH hBrush = nullptr) override;
      //  bool DrawState(const ::f64_point & point, const ::f64_size & size, DRAWSTATEPROC lpDrawProc,
      //   LPARAM lData, ::u32 nFlags, ::draw2d::brush* pBrush = nullptr) override;

      // Ellipse and Polygon Functions
      void Chord(::f64 x1, ::f64 y1, ::f64 x2, ::f64 y2, ::f64 x3, ::f64 y3,
                 ::f64 x4, ::f64 y4) override;
      void Chord(const ::f64_rectangle & rectangle, const ::f64_point & pointStart, const ::f64_point & pointEnd) override;
      void DrawFocusRect(const ::f64_rectangle & rectangle) override;
      //bool DrawEllipse(::f64 x1, ::f64 y1, ::f64 x2, ::f64 y2) override;
      //bool DrawEllipse(const ::f64_rectangle & rectangle) override;
      void draw_ellipse(const ::f64_rectangle & rectangle) override;
      //bool FillEllipse(::f64 x1, ::f64 y1, ::f64 x2, ::f64 y2) override;
      //bool FillEllipse(const ::f64_rectangle & rectangle) override;
      void fill_ellipse(const ::f64_rectangle & rectangle) override;
      void Pie(::f64 x1, ::f64 y1, ::f64 x2, ::f64 y2, ::f64 x3, ::f64 y3, ::f64 x4, ::f64 y4) override;
      void Pie(const ::f64_rectangle & rectangle, const ::f64_point & pointStart, const ::f64_point & pointEnd) override;
      //bool i32_polygon(LPCPOINT lppoints, ::collection::count nCount);
      void polygon(const ::f64_point * lppoints, ::collection::count nCount) override;
      //bool draw_polygon(LPCPOINT ppoints, ::collection::count nCount) override;
      void draw_polygon(const ::f64_point * lppoints, ::collection::count nCount) override;
      //bool fill_polygon(LPCPOINT ppoints, ::collection::count nCount) override;
      void fill_polygon(const ::f64_point * lppoints, ::collection::count nCount) override;
      //      bool poly_polygon(LPCPOINT  lppoint, LPCINT lpPolyCounts, ::collection::count nCount) override;

      //virtual bool fill_polygon(const ::f64_point * ppoints,count nCount) override;

      using ::draw2d::graphics::rectangle;
      using ::draw2d::graphics::draw_rectangle;
      using ::draw2d::graphics::fill_rectangle;
      using ::draw2d::graphics::round_rectangle;
      using ::draw2d::graphics::draw_round_rectangle;
      using ::draw2d::graphics::fill_round_rectangle;
      void rectangle(const ::f64_rectangle & rectangle) override;
      void draw_rectangle(const ::f64_rectangle & rectangle, ::draw2d::pen * ppen) override;
      void fill_rectangle(const ::f64_rectangle & rectangle, ::draw2d::brush * pbrush) override;
      void draw_round_rectangle(const ::f64_rectangle & rectangle, ::draw2d::pen * ppen, ::f64 dRadius) override;
      void fill_round_rectangle(const ::f64_rectangle & rectangle, ::draw2d::brush * pbrush, ::f64 dRadius) override;


      //virtual bool _draw_raw(const ::f64_rectangle & rectangleTarget, ::image::image * pimage, const ::image::image_drawing_options & imagedrawingoptions, const ::f64_point & rectangleSource = ::f64_point()) override;

      using ::draw2d::graphics::_draw_raw;


      // Bitmap Functions
      //bool PatBlt(::f64 x, ::f64 y, ::f64 nWidth, ::f64 nHeight) override;
      void _draw_raw(const ::f64_rectangle & rectangleTarget, ::image::image * pimage, const ::image::image_drawing_options & imagedrawingoptions, const ::f64_point & pointSrc) override;
      void _stretch_raw(const ::f64_rectangle & rectangleTarget, ::image::image * pimage, const ::image::image_drawing_options & imagedrawingoptions, const ::f64_rectangle & rectangleSource) override;


      ::color::color GetPixel(::f64 x, ::f64 y) override;
      ::color::color GetPixel(const ::f64_point & point) override;
      ::color::color SetPixel(::f64 x, ::f64 y, const ::color::color & color) override;
      ::color::color SetPixel(const ::f64_point & point, const ::color::color & color) override;
//      bool FloodFill(::f64 x, ::f64 y, const ::color::color & color) override;
//      bool ExtFloodFill(::f64 x, ::f64 y, const ::color::color & color, ::u32 nFillType) override;
//      bool MaskBlt(::f64 x, ::f64 y, ::f64 nWidth, ::f64 nHeight, ::draw2d::graphics * pgraphicsSrc,
//                   ::f64 xSrc, ::f64 ySrc, ::draw2d::bitmap& maskBitmap, ::f64 xMask, ::f64 yMask,
//                   ::u32 dwRop) override;
//      bool PlgBlt(::f64_point * lpPoint, ::draw2d::graphics * pgraphicsSrc, ::f64 xSrc, ::f64 ySrc,
//                  ::f64 nWidth, ::f64 nHeight, ::draw2d::bitmap& maskBitmap, ::f64 xMask, ::f64 yMask) override;
      //bool SetPixelV(::f64 x, ::f64 y, const ::color::color & color) override;
      //bool SetPixelV(const ::f64_point & point, const ::color::color & color) override;
//      bool GradientFill(TRIVERTEX* pVertices, ULONG nVertices,
//                        void * pMesh, ULONG nMeshElements, ::u32 dwMode) override;
//      bool TransparentBlt(::f64 xDest, ::f64 yDest, ::i32 nDestWidth, ::i32 nDestHeight,
//                          ::draw2d::graphics * pgraphicsSrc, ::f64 xSrc, ::f64 ySrc, ::i32 nSrcWidth, ::i32 nSrcHeight,
//                          ::u32 clrTransparent) override;

      //virtual bool _alpha_blend_raw(const ::f64_rectangle & rectangleTarget, ::draw2d::graphics * pgraphicsSrc, const ::f64_rectangle & rectangleSource, ::f64 dOpacity) override;

      /*bool alpha_blend(::f64 xDest, ::f64 yDest, ::i32 nDestWidth, ::i32 nDestHeight,
        ::draw2d::graphics * pgraphicsSrc, ::f64 xSrc, ::f64 ySrc, ::i32 nSrcWidth, ::i32 nSrcHeight,
        BLENDFUNCTION blend) override;*/

      // Text Functions
      //virtual bool text_out(::f64 x, ::f64 y, const ::scoped_string & scopedstrString, character_count nCount) override;
      //virtual bool text_out(::f64 x, ::f64 y, const block & block) override;
      void text_out(::f64 x, ::f64 y, const ::scoped_string & scopedstr) override;
      //virtual bool text_out(::f64 x, ::f64 y, const ::scoped_string & scopedstr) override;
      //virtual bool ExtTextOut(::f64 x, ::f64 y, ::u32 nOptions, const ::f64_rectangle & rectangle, const ::scoped_string & scopedstrString, character_count nCount, LPINT lpDxWidths) override;
      //virtual bool ExtTextOut(::f64 x, ::f64 y, ::u32 nOptions, const ::f64_rectangle & rectangle, const ::scoped_string & scopedstr, LPINT lpDxWidths) override;
      //virtual ::f64_size TabbedTextOut(::f64 x, ::f64 y, const ::scoped_string & scopedstrString, character_count nCount, ::collection::count nTabPositions, LPINT lpnTabStopPositions, ::i32 nTabOrigin) override;
      //virtual ::f64_size TabbedTextOut(::f64 x, ::f64 y, const ::scoped_string & scopedstr, ::collection::count nTabPositions, LPINT lpnTabStopPositions, ::i32 nTabOrigin) override;

      //virtual bool draw_text(const ::scoped_string & scopedstrString, character_count nCount, const ::f64_rectangle & rectangle, const ::e_align & ealign = e_align_top_left, const ::e_draw_text & edrawtext = e_draw_text_none) override;
      void draw_text(const ::scoped_string & scopedstr,const ::f64_rectangle & rectangle, const ::e_align & ealign = e_align_top_left, const ::e_draw_text & edrawtext = e_draw_text_none) override;

//      virtual ::i32 draw_text_ex(::i8 * lpszString, ::i32 nCount, ::f64_rectangle * prectangle, const ::e_align & ealign = e_align_top_left, const ::e_draw_text & edrawtext = e_draw_text_none, LPDRAWTEXTPARAMS lpDTParams) override;
      //    virtual ::i32 draw_text_ex(const ::scoped_string & scopedstr, ::f64_rectangle * prectangle, const ::e_align & ealign = e_align_top_left, const ::e_draw_text & edrawtext = e_draw_text_none, LPDRAWTEXTPARAMS lpDTParams) override;

      ::f64_size _get_text_extent(const ::scoped_string & scopedstr, character_count iIndex) override;
      //virtual ::f64_size get_text_extent(const ::scoped_string & scopedstrString, character_count nCount) override;
      ::f64_size _get_text_extent(const ::scoped_string & scopedstr) override;
      //void get_text_extent(::f64_size & size, const_char_pointer pszString, character_count nCount, character_count iIndex) override;
      //void get_text_extent(::f64_size & size, const_char_pointer pszString, character_count nCount) override;
      //void get_text_extent(::f64_size & size, const ::scoped_string & scopedstr) override;
      //virtual ::f64_size GetOutputTextExtent(const_char_pointer pszString, character_count nCount) override;
      //virtual ::f64_size GetOutputTextExtent(const ::scoped_string & scopedstr) override;
      //virtual ::f64_size GetTabbedTextExtent(const ::scoped_string & scopedstrString, character_count nCount, ::collection::count nTabPositions, LPINT lpnTabStopPositions) override;
      //virtual ::f64_size GetTabbedTextExtent(const ::scoped_string & scopedstr, ::collection::count nTabPositions, LPINT lpnTabStopPositions) override;
      //virtual ::f64_size GetOutputTabbedTextExtent(const ::scoped_string & scopedstrString, character_count nCount, ::collection::count nTabPositions, LPINT lpnTabStopPositions) override;
      //virtual ::f64_size GetOutputTabbedTextExtent(const ::scoped_string & scopedstr, count  nTabPositions, LPINT lpnTabStopPositions) override;
      //virtual bool GrayString(::draw2d::brush* pBrush, bool (CALLBACK* lpfnOutput)(HDC, LPARAM, ::i32), LPARAM lpData, ::i32 nCount, ::f64 x, ::f64 y, ::f64 nWidth, ::f64 nHeight) override;
      ::u32 GetTextAlign() override;
      ::u32 SetTextAlign(::u32 nFlags) override;
      //virtual ::i32 GetTextFace(count nCount, ::i8 * lpszFacename) override;
      //virtual ::i32 GetTextFace(string & rString) override;
      void get_text_metrics(::write_text::text_metric * lpMetrics) override;
      void get_output_text_metrics(::write_text::text_metric * lpMetrics) override;
      //virtual ::i32 SetTextJustification(::i32 nBreakExtra, ::i32 nBreakCount) override;
      //virtual ::i32 GetTextCharacterExtra() override;
      //virtual ::i32 SetTextCharacterExtra(::i32 nCharExtra) override;

//      ::u32 GetCharacterPlacement(const ::string & lpString, ::i32 nCount, ::i32 nMaxExtent, LPGCP_RESULTS lpResults, ::u32 dwFlags) override;
      //    ::u32 GetCharacterPlacement(string & str, ::i32 nMaxExtent, LPGCP_RESULTS lpResults, ::u32 dwFlags) override;

//#if (_WIN32_WINNT >= 0x0500)
//
//      bool GetTextExtentExPointI(LPWORD pgiIn, ::i32 cgi, ::i32 nMaxExtent, LPINT lpnFit, LPINT alpDx, __out_opt LPSIZE32 LPSIZE32) override;
//      bool GetTextExtentPointI(LPWORD pgiIn, ::i32 cgi, __out_opt LPSIZE32 LPSIZE32) override;
//
//#endif

      void set_smooth_mode(::draw2d::enum_smooth_mode esmoothmode) override;


      // Advanced Drawing
      //bool DrawEdge(const ::f64_rectangle & rectangle, ::u32 nEdge, ::u32 nFlags) override;
      //bool DrawFrameControl(const ::f64_rectangle & rectangle, ::u32 nType, ::u32 nState) override;

//      // Scrolling Functions
//      bool ScrollDC(::i32 dx, ::i32 dy, const ::f64_rectangle & rectangleScroll, const ::f64_rectangle & rectangleClip,
//                    ::draw2d::region* pRgnUpdate, ::i32_rectangle * lpRectUpdate) override;
//
//      // font Functions
//      bool GetCharWidth(::u32 nFirstChar, ::u32 nLastChar, LPINT lpBuffer) override;
//      bool GetOutputCharWidth(::u32 nFirstChar, ::u32 nLastChar, LPINT lpBuffer) override;
//      ::u32 SetMapperFlags(::u32 dwFlag) override;
//      ::f64_size GetAspectRatioFilter() override;
//
////      bool GetCharABCWidths(::u32 nFirstChar, ::u32 nLastChar, LPABC lpabc) override;
//      ::u32 GetFontData(::u32 dwTable, ::u32 dwOffset, LPVOID lpData, ::u32 cbData) override;
////      ::i32 GetKerningPairs(::i32 nPairs, LPKERNINGPAIR lpkrnpair) override;
////      ::u32 GetOutlineTextMetrics(::u32 cbData, LPOUTLINETEXTMETRICW lpotm) override;
////      ::u32 GetGlyphOutline(::u32 nChar, const ::e_align & ealign = e_align_top_left, const ::e_draw_text & edrawtext = e_draw_text_none, LPGLYPHMETRICS lpgm,
////         ::u32 cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2) override;
//
////      bool GetCharABCWidths(::u32 nFirstChar, ::u32 nLastChar,
////         LPABCFLOAT lpABCF) override;
//      bool GetCharWidth(::u32 nFirstChar, ::u32 nLastChar,
//                        ::f32* lpFloatBuffer) override;
//
//      ::u32 GetFontLanguageInfo() override;
//
//#if (_WIN32_WINNT >= 0x0500)
//
////      bool GetCharABCWidthsI(::u32 giFirst, ::u32 cgi, LPWORD pgi, LPABC lpabc) override;
////      bool GetCharWidthI(::u32 giFirst, ::u32 cgi, LPWORD pgi, LPINT lpBuffer) override;
//
//#endif

      //// Printer/Device Escape Functions
      //virtual ::i32 Escape(::i32 nEscape, ::i32 nCount, const ::scoped_string & scopedstrInData, LPVOID lpOutData) override;
      //virtual ::i32 Escape(::i32 nEscape, ::i32 nInputSize, const ::scoped_string & scopedstrInputData, ::i32 nOutputSize, ::i8 * lpszOutputData) override;
      //virtual ::i32 DrawEscape(::i32 nEscape, ::i32 nInputSize, const ::scoped_string & scopedstrInputData) override;

      // Escape helpers
      //::i32 StartDoc(const ::scoped_string & scopedstrDocName) override;  // old Win3.0 version
//      ::i32 StartDoc(LPDOCINFO lpDocInfo) override;
      ::i32 StartPage() override;
      ::i32 EndPage() override;
      //::i32 SetAbortProc(bool (CALLBACK* lpfn)(HDC, ::i32)) override;
      ::i32 AbortDoc() override;
      ::i32 EndDoc() override;

      // MetaFile Functions
      //bool PlayMetaFile(HMETAFILE hMF) override;
      //bool PlayMetaFile(HENHMETAFILE hEnhMetaFile, const ::f64_rectangle & rectangleBounds) override;
      void AddMetaFileComment(::u32 nDataSize, const ::u8* pCommentData) override;
      // can be used for enhanced metafiles only

      // Path Functions
      void abort_path() override;
      void begin_path() override;
      void close_figure() override;
      void end_path() override;
      void fill_path() override;
      void flatten_path() override;
      void stroke_and_fill_path() override;
      void stroke_path() override;
      void widen_path() override;
      ::f32 GetMiterLimit() override;
      void SetMiterLimit(::f32 fMiterLimit) override;
      //virtual ::i32 GetPath(::f64_point * ppoints, LPBYTE lpTypes, ::collection::count nCount) override;
      void SelectClipPath(::i32 nMode) override;

      // Misc Helper Functions
      static ::draw2d::brush* GetHalftoneBrush(::particle * pparticle);
      //void DrawDragRect(const ::f64_rectangle & rectangle, const ::f64_size & size,
      //                  const ::f64_rectangle & rectangleLast, const ::f64_size & sizeLast,
      //                  ::draw2d::brush* pBrush = nullptr, ::draw2d::brush* pBrushLast = nullptr) override;

      void fill_rectangle(const ::f64_rectangle & rectangle, const ::color::color & color) override;
      //void fill_rectangle(const ::f64_rectangle & rectangle, const ::color::color & color) override;

      //bool draw_inset_3d_rectangle(const ::f64_rectangle & rectangle, const ::color::color & colorTopLeft, const ::color::color & colorBottomRight, const ::e_border & eborder = e_border_all) override;
      //void draw_inset_3d_rectangle(const ::f64_rectangle & rectangle, const ::color::color & colorTopLeft, const ::color::color & colorBottomRight, const ::e_border & eborder = e_border_all) override;


      //void assert_ok() const override;
      //void dump(dump_context & dumpcontext) const override;


      void set_alpha_mode(::draw2d::enum_alpha_mode ealphamode) override;


      //virtual HDC get_handle() const;
      //virtual HDC get_handle1() const;
      //virtual HDC get_handle2() const;

      virtual void attach(void * pdata) override;
      virtual void * detach() override;


      //virtual void __attach(ID2D1DeviceContext* pdevicecontext);

      void _bind(::i32 iIndex, ::i32 iLayerIndex, IDXGISurface* pdxgisurface) override;

      virtual void __attach(::i32 iIndex, ::i32 iLayerIndex, ID2D1Bitmap1* pd2d1bitmap);

#ifdef WINDOWS_DESKTOP
      //virtual bool attach_hdc(HDC hdc) override;
      //virtual HDC detach_hdc() override;
#endif

//      virtual Gdiplus::FillMode direct2d_get_fill_mode() override;

      void blur(bool bExpand, ::f64 dRadius, const ::f64_rectangle & rectangle) override;


      void destroy() override;

      void destroy_os_data() override;

      //IDWriteTextFormat * get_os_font(::write_text::font * pfont);
      //ID2D1Brush * get_os_brush(::draw2d::brush * pbrush);
      //ID2D1Brush * get_os_pen_brush(::draw2d::pen * ppen);

      using ::draw2d::graphics::draw;

      virtual bool draw(const ::write_text::text_out & textout, ::draw2d::pen * ppen);

      virtual bool fill(const ::write_text::text_out & textout, ::draw2d::brush * pbrush);

      virtual bool draw(const ::write_text::draw_text & drawtext, ::draw2d::pen* ppen);

      virtual bool fill(const ::write_text::draw_text & drawtext, ::draw2d::brush* pbrush);


      void flush() override;

      void debug() override;
      //protected:
      // used for implementation of non-virtual SelectObject calls
      //static ::draw2d::object* SelectGdiObject(application * papp, HDC hDC, HGDIOBJ h) override;

      //virtual void enum_fonts(::write_text::font_enumeration_item_array & itema) override;

//#ifdef UNIVERSAL_WINDOWS
//      virtual void set_direct2d_plugin(::draw2d_direct2d_for_directx11::plugin * pplugin);
//#endif

      //virtual bool update_window(::image::image *  pimage) override;

      comptr < ID2D1SolidColorBrush > _create_solid_brush(const ::color::color & color);
      comptr < ID2D1RadialGradientBrush > _create_simple_radial_gradient(const ::f64_rectangle & r, ID2D1GradientStopCollection * pcollection);
      comptr < ID2D1LinearGradientBrush > _create_simple_linear_gradient(const ::f64_point & p1, const ::f64_point & p2, ID2D1GradientStopCollection * pcollection);
      comptr < ID2D1GradientStopCollection > _create_simple_full_range_flat_gradient_stop_collection(const ::color::color & color1, const ::color::color & color2);


      ID2D1Geometry * defer_update_os_data(::pointer < ::geometry2d::region > & pregion);


   };


   class CLASS_DECL_DRAW2D_DIRECT2D_FOR_DIRECTX11 layer
   {
   public:

      comptr<ID2D1RenderTarget>                          m_prendertarget;



      layer(ID2D1RenderTarget * ptarget, const D2D1_LAYER_PARAMETERS & parameters) :
         m_prendertarget(ptarget)
      {

         m_prendertarget->PushLayer(parameters, nullptr);

      }

      layer(ID2D1RenderTarget * ptarget, ID2D1PathGeometry * ppathgeometry) :
         layer(ptarget, D2D1::LayerParameters(D2D1::InfiniteRect(), ppathgeometry))
      {

      }

      layer(ID2D1RenderTarget * ptarget, ID2D1PathGeometry1 * ppathgeometry) :
         layer(ptarget, D2D1::LayerParameters(D2D1::InfiniteRect(), ppathgeometry))
      {

      }

      ~layer()
      {

         m_prendertarget->PopLayer();

      }

   };


} // namespace draw2d_direct2d_for_directx11



