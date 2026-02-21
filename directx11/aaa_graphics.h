#pragma once


namespace draw2d_directx11
{


   class CLASS_DECL_DRAW2D_DIRECTX11 graphics :
      virtual public ::draw2d::graphics
   {
   public:


      class state :
         virtual public ::object
      {
      public:




         D2D1::Matrix3x2F                    m_m;
         array < D2D1::Matrix3x2F >          m_maRegion;
         pointer_array < ::draw2d::region >        m_sparegionClip;
         D2D1_LAYER_PARAMETERS               m_layerparameters;

         state();
         virtual ~state();


      };

      comptr<ID2D1DeviceContext>         m_pdevicecontext; // 0
      comptr<ID2D1RenderTarget>          m_prendertarget; // 1
      comptr<ID2D1BitmapRenderTarget>    m_pbitmaprendertarget; // 2
      comptr<ID2D1DCRenderTarget>        m_pdcrendertarget; // 3

      comptr<IDXGIAdapter>               m_padapter;
      comptr<IDXGIFactory2>              m_pfactory2;
      comptr<ID2D1Layer>                 m_player;
      comptr<ID2D1PathGeometry>          m_ppathgeometryClip;

      pointer_array < state >                             m_statea;
      ::pointer<state>                                  m_pstate;

      bool                                               m_bSaveClip;

      int                                                m_iType;

      D2D1_BITMAP_INTERPOLATION_MODE                     m_bitmapinterpolationmode;
      D2D1_INTERPOLATION_MODE                            m_interpolationmode;

      ::write_text::enum_rendering                    m_etextrenderinghintDevice;


      // advanced use and implementation
      bool                                               m_bPrinting;

      HDC                                                m_hdcAttach;
      ::draw2d::enum_alpha_mode                          m_ealphamodeDevice;


      graphics();
      virtual ~graphics();


      virtual bool TextOutAlphaBlend(double x, double y, const block & block) override;
      virtual bool _draw_blend(const ::image::image_drawing & imagedrawing) override;


      bool IsPrinting() override;            // true if being used for printing

      
      virtual bool on_begin_draw() override;

      using ::draw2d::graphics::set;
      virtual void set(::draw2d::bitmap* pbitmap) override;


      oswindow get_window_handle();

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

      void defer_text_primitive_blend();

      // for bidi and mirrored localization
      unsigned int GetLayout() override;
      unsigned int SetLayout(unsigned int dwLayout) override;


      //virtual bool save_clip() override;
      //virtual bool restore_clip() override;

      virtual double get_dpix() override;

      //// Constructors
      //bool CreateDC(const ::scoped_string & scopedstrDriverName, const ::scoped_string & scopedstrDeviceName,
      //              const char * lpszOutput, const void * lpInitData) override;
      //bool CreateIC(const ::scoped_string & scopedstrDriverName, const ::scoped_string & scopedstrDeviceName,
      //              const char * lpszOutput, const void * lpInitData) override;
      bool create_compatible_graphics(::draw2d::graphics * pgraphics) override;

      bool DeleteDC() override;

      // Device-Context Functions
      virtual int SaveDC() override;
      virtual bool RestoreDC(int nSavedDC) override;
      //int GetDeviceCaps(int nIndex) override;
      unsigned int SetBoundsRect(const ::double_rectangle & rectangleBounds, unsigned int flags) override;
      unsigned int GetBoundsRect(::double_rectangle * rectangleBounds, unsigned int flags) override;
//      bool ResetDC(const DEVMODE* lpDevMode) override;

      // Drawing-Tool Functions
      ::double_point GetBrushOrg() override;
      ::double_point SetBrushOrg(double x, double y) override;
      ::double_point SetBrushOrg(const ::double_point & point) override;
//      int EnumObjects(int nObjectType,
  //                    int (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData) override;

      //virtual ::draw2d::object* set_stock_object(int nIndex) override;
      //virtual void set(::write_text::font * pfont) override;
      //virtual void set(::draw2d::pen* pPen) override;
      //virtual void set(::draw2d::brush* pBrush) override;
      //virtual void set(::write_text::font* pFont) override;
      //virtual void set(::draw2d::bitmap* pBitmap) override;
      //virtual int SelectObject(::draw2d::region* pRgn) override;       // special return for regions
      //::draw2d::object* SelectObject(::draw2d::object* pObject) override;
      // ::draw2d::object* provided so compiler doesn't use SelectObject(HGDIOBJ)

      //// color and color Palette Functions
      //color32_t GetNearestColor(const ::color::color & color) override;
      //// ::draw2d::palette* SelectPalette(::draw2d::palette* pPalette, bool bForceBackground) override;
      //unsigned int RealizePalette() override;
      //void UpdateColors() override;

      int GetPolyFillMode() override;
      int GetROP2() override;
      int GetStretchBltMode() override;

      int SetPolyFillMode(int nPolyFillMode) override;
      int SetROP2(int nDrawMode) override;
      virtual bool set_interpolation_mode(::draw2d::enum_interpolation_mode einterpolationmode) override;


//#if (_WIN32_WINNT >= 0x0500)
//
//      color32_t GetDCBrushColor() override;
//      color32_t SetDCBrushColor(const ::color::color & color) override;
//
//      color32_t GetDCPenColor() override;
//      color32_t SetDCPenColor(const ::color::color & color) override;
//
//#endif

      // Graphics mode
      int SetGraphicsMode(int iMode) override;
      int GetGraphicsMode() override;


      virtual bool _get(::geometry2d::matrix & matrix) override;
      virtual bool _set(const ::geometry2d::matrix & matrix) override;

      virtual void clear_current_point() override;

      virtual bool draw_path(::draw2d::path * ppath) override;
      virtual bool fill_path(::draw2d::path * ppath) override;
      virtual bool path(::draw2d::path * ppath) override;

      virtual bool draw_path(ID2D1PathGeometry * pgeometry, ::draw2d::pen * ppen);
      virtual bool fill_path(ID2D1PathGeometry * pgeometry, ::draw2d::brush * pbrush);

      virtual bool draw_path(::draw2d::path * ppath, ::draw2d::pen * ppen) override;
      virtual bool fill_path(::draw2d::path * ppath, ::draw2d::brush * pbrush) override;

      // World transform
      //bool SetWorldTransform(const XFORM* pXform) override;
      //bool ModifyWorldTransform(const XFORM* pXform,unsigned int iMode) override;
      //bool GetWorldTransform(XFORM* pXform) override;

      // Mapping Functions
      virtual int GetMapMode() override;
      virtual ::double_point GetViewportOrg() override;
      virtual int SetMapMode(int nMapMode) override;
      // Viewport Origin
      virtual ::double_point SetViewportOrg(double x, double y) override;
      virtual ::double_point SetViewportOrg(const ::double_point & point) override;
      virtual ::double_point OffsetViewportOrg(double nWidth, double nHeight) override;

      // Viewport Extent
      virtual double_size GetViewportExt() override;
      virtual double_size SetViewportExt(double cx, double cy) override;
      virtual double_size SetViewportExt(const ::double_size & size) override;
      virtual double_size ScaleViewportExt(double xNum, double xDenom, double yNum, double yDenom) override;

      // Window Origin
      ::double_point GetWindowOrg() override;
      ::double_point SetWindowOrg(double x, double y) override;
      ::double_point SetWindowOrg(const ::double_point & point) override;
      ::double_point offset_window_org(double nWidth, double nHeight) override;

      // Window extent
      ::double_size GetWindowExt() override;
      virtual ::double_size set_window_ext(double cx, double cy) override;
      ::double_size set_window_ext(const ::double_size & size) override;
      virtual ::double_size scale_window_ext(double xNum, double xDenom, double yNum, double yDenom) override;

      // Coordinate Functions
      void DPtoLP(::double_point * ppoints, ::collection::count nCount = 1) override;
      void DPtoLP(::double_rectangle * prectangle) override;
      void DPtoLP(::double_size * psize) override;
      void LPtoDP(::double_point * ppoints, ::collection::count nCount = 1) override;
      void LPtoDP(::double_rectangle * prectangle) override;
      void LPtoDP(::double_size * psize) override;

      // Special Coordinate Functions (useful for dealing with metafiles and OLE)
      virtual void DPtoHIMETRIC(::double_size * LPSIZE32) override;
      //void LPtoHIMETRIC(::double_size * LPSIZE32) override;
      virtual void HIMETRICtoDP(::double_size * LPSIZE32) override;
      //void HIMETRICtoLP(::double_size * LPSIZE32) override;

      // Region Functions
      virtual bool fill_region(::draw2d::region* pRgn, ::draw2d::brush* pBrush) override;
      virtual bool frame_region(::draw2d::region* pRgn, ::draw2d::brush* pBrush, double nWidth, double nHeight) override;
      virtual bool invert_region(::draw2d::region* pRgn) override;
      virtual bool paint_region(::draw2d::region* pRgn) override;

      // Clipping Functions
      virtual int get_clip_box(::double_rectangle * prectangle) override;


      virtual void add_shapes(const shape_array& shapea);
      virtual void reset_clip();
      virtual void intersect_clip(const ::double_rectangle& rectangle);
      //virtual void intersect_clip(const ::double_rectangle& rectangle);
      //virtual void intersect_clip(const ::ellipse& ellipse);
      virtual void intersect_clip(const ::ellipse& ellipse);
      //virtual void intersect_clip(const ::int_polygon& int_polygon);
      virtual void intersect_clip(const ::double_polygon& int_polygon);

      //virtual bool PtVisible(double x, double y) override;
      //bool PtVisible(const ::double_point & point) override;
      //virtual bool RectVisible(const ::double_rectangle & rectangle) override;
      //int SelectClipRgn(::draw2d::region* pRgn) override;
      //int ExcludeClipRect(double x1, double y1, double x2, double y2) override;
      //int ExcludeClipRect(const ::double_rectangle & rectangle) override;
      ////int ExcludeUpdateRgn(::user::prototype * pwindow) override;
      //int IntersectClipRect(double x1, double y1, double x2, double y2) override;
      //int IntersectClipRect(const ::double_rectangle & rectangle) override;
      //int OffsetClipRgn(double x, double y) override;
      //int OffsetClipRgn(const ::double_size & size) override;
      //int SelectClipRgn(::draw2d::region* pRgn, ::draw2d::enum_combine ecombine) override;

      // Line-Output Functions
      double_point current_position() override;
//      ::double_point move_to(double x, double y) override;
      //    ::double_point move_to(const ::double_point & point) override;
      using ::draw2d::graphics::line_to;
      bool line_to(double x, double y) override;
      bool draw_line(double x1, double y1, double x2, double y2, ::draw2d::pen * ppen) override;
      //  bool line_to(const ::double_point & point) override;
      bool Arc(double x1, double y1, double w, double h, angle start, angle extends) override;
      bool Arc(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) override;
      //bool Arc(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) override;
      bool Arc(const ::double_rectangle & rectangle, const ::double_point & pointStart, const ::double_point & pointEnd) override;
      bool polyline(const ::double_point* ppoints,count nCount) override;

      bool AngleArc(double x, double y, double nRadius, angle fStartAngle, angle fSweepAngle) override;
      bool ArcTo(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) override;
      bool ArcTo(const ::double_rectangle & rectangle, const ::double_point & pointStart, const ::double_point & pointEnd) override;
      //int GetArcDirection() override;
      //int SetArcDirection(int nArcDirection) override;

      bool polydraw(const ::double_point* ppoints, const unsigned char* lpTypes,count nCount) override;
      bool polyline_to(const ::double_point* ppoints,count nCount) override;
      bool poly_polyline(const ::double_point* ppoints, const int * lpPolyPoints,count nCount) override;

      bool poly_bezier(const ::double_point* ppoints,count nCount) override;
      bool poly_bezier_to(const ::double_point* ppoints,count nCount) override;


      virtual bool frame_rectangle(const ::double_rectangle & rectangle, ::draw2d::brush* pBrush) override;


      virtual bool invert_rectangle(const ::double_rectangle & rectangle) override;

      //virtual bool draw(const ::double_rectangle & rectangleTarget, ::image::icon * picon) override;

      //bool DrawIcon(double x, double y, ::image::icon * picon) override;
      //bool DrawIcon(const ::double_point & point, ::image::icon * picon) override;
      //bool DrawIcon(double x, double y, ::image::icon * picon, double cx, double cy, unsigned int istepIfAniCur, HBRUSH hbrFlickerFreeDraw, unsigned int diFlags) override;
//      bool DrawState(const ::double_point & point, const ::double_size & size, HBITMAP hBitmap, unsigned int nFlags,
//                     HBRUSH hBrush = nullptr) override;
//      bool DrawState(const ::double_point & point, const ::double_size & size, ::draw2d::bitmap* pBitmap, unsigned int nFlags,
//                     ::draw2d::brush* pBrush = nullptr) override;
//      bool DrawState(const ::double_point & point, const ::double_size & size, HICON hIcon, unsigned int nFlags,
//                     HBRUSH hBrush = nullptr) override;
//      bool DrawState(const ::double_point & point, const ::double_size & size, HICON hIcon, unsigned int nFlags,
//                     ::draw2d::brush* pBrush = nullptr) override;
//      bool DrawState(const ::double_point & point, const ::double_size & size, const ::scoped_string & scopedstrText, unsigned int nFlags,
//                     bool bPrefixText = true, int nTextLen = 0, HBRUSH hBrush = nullptr) override;
//      bool DrawState(const ::double_point & point, const ::double_size & size, const ::scoped_string & scopedstrText, unsigned int nFlags,
//                     bool bPrefixText = true, int nTextLen = 0, ::draw2d::brush* pBrush = nullptr) override;
//      bool DrawState(const ::double_point & point, const ::double_size & size, DRAWSTATEPROC lpDrawProc,
      //       LPARAM lData, unsigned int nFlags, HBRUSH hBrush = nullptr) override;
      //  bool DrawState(const ::double_point & point, const ::double_size & size, DRAWSTATEPROC lpDrawProc,
      //   LPARAM lData, unsigned int nFlags, ::draw2d::brush* pBrush = nullptr) override;

      // Ellipse and Polygon Functions
      bool Chord(double x1, double y1, double x2, double y2, double x3, double y3,
                 double x4, double y4) override;
      bool Chord(const ::double_rectangle & rectangle, const ::double_point & pointStart, const ::double_point & pointEnd) override;
      void DrawFocusRect(const ::double_rectangle & rectangle) override;
      //bool DrawEllipse(double x1, double y1, double x2, double y2) override;
      //bool DrawEllipse(const ::double_rectangle & rectangle) override;
      bool draw_ellipse(const ::double_rectangle & rectangle) override;
      //bool FillEllipse(double x1, double y1, double x2, double y2) override;
      //bool FillEllipse(const ::double_rectangle & rectangle) override;
      bool fill_ellipse(const ::double_rectangle & rectangle) override;
      bool Pie(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) override;
      bool Pie(const ::double_rectangle & rectangle, const ::double_point & pointStart, const ::double_point & pointEnd) override;
      //bool int_polygon(LPCPOINT lppoints, ::collection::count nCount);
      bool int_polygon(const ::double_point * lppoints, ::collection::count nCount);
      //bool draw_polygon(LPCPOINT ppoints, ::collection::count nCount) override;
      bool draw_polygon(const ::double_point * lppoints, ::collection::count nCount) override;
      //bool fill_polygon(LPCPOINT ppoints, ::collection::count nCount) override;
      bool fill_polygon(const ::double_point * lppoints, ::collection::count nCount) override;
      //      bool poly_polygon(LPCPOINT  lppoint, LPCINT lpPolyCounts, ::collection::count nCount) override;

      //virtual bool fill_polygon(const ::double_point * ppoints,count nCount) override;

      using ::draw2d::graphics::rectangle;
      using ::draw2d::graphics::draw_rectangle;
      using ::draw2d::graphics::fill_rectangle;
      using ::draw2d::graphics::round_rectangle;
      virtual bool rectangle(const ::double_rectangle & rectangle) override;
      virtual bool draw_rectangle(const ::double_rectangle & rectangle, ::draw2d::pen * ppen) override;
      virtual bool fill_rectangle(const ::double_rectangle & rectangle, ::draw2d::brush * ppen) override;
      virtual bool round_rectangle(const ::double_rectangle & rectangle, double dRadius) override;


      //virtual bool _draw_raw(const ::double_rectangle & rectangleTarget, ::image::image * pimage, const ::image::image_drawing_options & imagedrawingoptions, const ::double_point & rectangleSource = ::double_point()) override;

      using ::draw2d::graphics::_draw_raw;


      // Bitmap Functions
      //bool PatBlt(double x, double y, double nWidth, double nHeight) override;
      virtual bool _draw_raw(const ::double_rectangle & rectangleTarget, ::image::image * pimage, const ::image::image_drawing_options & imagedrawingoptions, const ::double_point & pointSrc) override;
      virtual bool _stretch_raw(const ::double_rectangle & rectangleTarget, ::image::image * pimage, const ::image::image_drawing_options & imagedrawingoptions, const ::double_rectangle & rectangleSource) override;


      ::color::color GetPixel(double x, double y) override;
      ::color::color GetPixel(const ::double_point & point) override;
      ::color::color SetPixel(double x, double y, const ::color::color & color) override;
      ::color::color SetPixel(const ::double_point & point, const ::color::color & color) override;
//      bool FloodFill(double x, double y, const ::color::color & color) override;
//      bool ExtFloodFill(double x, double y, const ::color::color & color, unsigned int nFillType) override;
//      bool MaskBlt(double x, double y, double nWidth, double nHeight, ::draw2d::graphics * pgraphicsSrc,
//                   double xSrc, double ySrc, ::draw2d::bitmap& maskBitmap, double xMask, double yMask,
//                   unsigned int dwRop) override;
//      bool PlgBlt(::double_point * lpPoint, ::draw2d::graphics * pgraphicsSrc, double xSrc, double ySrc,
//                  double nWidth, double nHeight, ::draw2d::bitmap& maskBitmap, double xMask, double yMask) override;
      bool SetPixelV(double x, double y, const ::color::color & color) override;
      bool SetPixelV(const ::double_point & point, const ::color::color & color) override;
//      bool GradientFill(TRIVERTEX* pVertices, ULONG nVertices,
//                        void * pMesh, ULONG nMeshElements, unsigned int dwMode) override;
//      bool TransparentBlt(double xDest, double yDest, int nDestWidth, int nDestHeight,
//                          ::draw2d::graphics * pgraphicsSrc, double xSrc, double ySrc, int nSrcWidth, int nSrcHeight,
//                          unsigned int clrTransparent) override;

      //virtual bool _alpha_blend_raw(const ::double_rectangle & rectangleTarget, ::draw2d::graphics * pgraphicsSrc, const ::double_rectangle & rectangleSource, double dOpacity) override;

      /*bool alpha_blend(double xDest, double yDest, int nDestWidth, int nDestHeight,
        ::draw2d::graphics * pgraphicsSrc, double xSrc, double ySrc, int nSrcWidth, int nSrcHeight,
        BLENDFUNCTION blend) override;*/

      // Text Functions
      //virtual bool text_out(double x, double y, const ::scoped_string & scopedstrString, character_count nCount) override;
      //virtual bool text_out(double x, double y, const block & block) override;
      virtual bool TextOutRaw(double x, double y, const block & block) override;
      //virtual bool text_out(double x, double y, const ::scoped_string & scopedstr) override;
      //virtual bool ExtTextOut(double x, double y, unsigned int nOptions, const ::double_rectangle & rectangle, const ::scoped_string & scopedstrString, character_count nCount, LPINT lpDxWidths) override;
      //virtual bool ExtTextOut(double x, double y, unsigned int nOptions, const ::double_rectangle & rectangle, const ::scoped_string & scopedstr, LPINT lpDxWidths) override;
      //virtual ::double_size TabbedTextOut(double x, double y, const ::scoped_string & scopedstrString, character_count nCount, ::collection::count nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin) override;
      //virtual ::double_size TabbedTextOut(double x, double y, const ::scoped_string & scopedstr, ::collection::count nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin) override;

      //virtual bool draw_text(const ::scoped_string & scopedstrString, character_count nCount, const ::double_rectangle & rectangle, const ::e_align & ealign = e_align_top_left, const ::e_draw_text & edrawtext = e_draw_text_none) override;
      virtual bool draw_text(const ::scoped_string & scopedstr,const ::double_rectangle & rectangle, const ::e_align & ealign = e_align_top_left, const ::e_draw_text & edrawtext = e_draw_text_none) override;

//      virtual int draw_text_ex(char * lpszString, int nCount, ::double_rectangle * prectangle, const ::e_align & ealign = e_align_top_left, const ::e_draw_text & edrawtext = e_draw_text_none, LPDRAWTEXTPARAMS lpDTParams) override;
      //    virtual int draw_text_ex(const ::scoped_string & scopedstr, ::double_rectangle * prectangle, const ::e_align & ealign = e_align_top_left, const ::e_draw_text & edrawtext = e_draw_text_none, LPDRAWTEXTPARAMS lpDTParams) override;

      virtual double_size get_text_extent(const ::scoped_string & scopedstrString, character_count nCount, character_count iIndex) override;
      //virtual double_size get_text_extent(const ::scoped_string & scopedstrString, character_count nCount) override;
      virtual double_size get_text_extent(const ::scoped_string & scopedstr) override;
      virtual bool get_text_extent(double_size & size, const ::scoped_string & scopedstrString, character_count nCount, character_count iIndex) override;
      virtual bool get_text_extent(double_size & size, const ::scoped_string & scopedstrString, character_count nCount) override;
      virtual bool get_text_extent(double_size & size, const ::scoped_string & scopedstr) override;
      virtual ::double_size GetOutputTextExtent(const ::scoped_string & scopedstrString, character_count nCount) override;
      virtual ::double_size GetOutputTextExtent(const ::scoped_string & scopedstr) override;
      //virtual ::double_size GetTabbedTextExtent(const ::scoped_string & scopedstrString, character_count nCount, ::collection::count nTabPositions, LPINT lpnTabStopPositions) override;
      //virtual ::double_size GetTabbedTextExtent(const ::scoped_string & scopedstr, ::collection::count nTabPositions, LPINT lpnTabStopPositions) override;
      virtual ::double_size GetOutputTabbedTextExtent(const ::scoped_string & scopedstrString, character_count nCount, ::collection::count nTabPositions, LPINT lpnTabStopPositions) override;
      virtual ::double_size GetOutputTabbedTextExtent(const ::scoped_string & scopedstr, count  nTabPositions, LPINT lpnTabStopPositions) override;
      //virtual bool GrayString(::draw2d::brush* pBrush, bool (CALLBACK* lpfnOutput)(HDC, LPARAM, int), LPARAM lpData, int nCount, double x, double y, double nWidth, double nHeight) override;
      virtual unsigned int GetTextAlign() override;
      virtual unsigned int SetTextAlign(unsigned int nFlags) override;
      //virtual int GetTextFace(count nCount, char * lpszFacename) override;
      //virtual int GetTextFace(string & rString) override;
      virtual bool get_text_metrics(::write_text::text_metric * lpMetrics) override;
      virtual bool get_output_text_metrics(::write_text::text_metric * lpMetrics) override;
      //virtual int SetTextJustification(int nBreakExtra, int nBreakCount) override;
      //virtual int GetTextCharacterExtra() override;
      //virtual int SetTextCharacterExtra(int nCharExtra) override;

//      unsigned int GetCharacterPlacement(const ::string & lpString, int nCount, int nMaxExtent, LPGCP_RESULTS lpResults, unsigned int dwFlags) override;
      //    unsigned int GetCharacterPlacement(string & str, int nMaxExtent, LPGCP_RESULTS lpResults, unsigned int dwFlags) override;

//#if (_WIN32_WINNT >= 0x0500)
//
//      bool GetTextExtentExPointI(LPWORD pgiIn, int cgi, int nMaxExtent, LPINT lpnFit, LPINT alpDx, __out_opt LPSIZE32 LPSIZE32) override;
//      bool GetTextExtentPointI(LPWORD pgiIn, int cgi, __out_opt LPSIZE32 LPSIZE32) override;
//
//#endif



      // Advanced Drawing
      //bool DrawEdge(const ::double_rectangle & rectangle, unsigned int nEdge, unsigned int nFlags) override;
      //bool DrawFrameControl(const ::double_rectangle & rectangle, unsigned int nType, unsigned int nState) override;

//      // Scrolling Functions
//      bool ScrollDC(int dx, int dy, const ::double_rectangle & rectangleScroll, const ::double_rectangle & rectangleClip,
//                    ::draw2d::region* pRgnUpdate, ::int_rectangle * lpRectUpdate) override;
//
//      // font Functions
//      bool GetCharWidth(unsigned int nFirstChar, unsigned int nLastChar, LPINT lpBuffer) override;
//      bool GetOutputCharWidth(unsigned int nFirstChar, unsigned int nLastChar, LPINT lpBuffer) override;
//      unsigned int SetMapperFlags(unsigned int dwFlag) override;
//      ::double_size GetAspectRatioFilter() override;
//
////      bool GetCharABCWidths(unsigned int nFirstChar, unsigned int nLastChar, LPABC lpabc) override;
//      unsigned int GetFontData(unsigned int dwTable, unsigned int dwOffset, LPVOID lpData, unsigned int cbData) override;
////      int GetKerningPairs(int nPairs, LPKERNINGPAIR lpkrnpair) override;
////      unsigned int GetOutlineTextMetrics(unsigned int cbData, LPOUTLINETEXTMETRICW lpotm) override;
////      unsigned int GetGlyphOutline(unsigned int nChar, const ::e_align & ealign = e_align_top_left, const ::e_draw_text & edrawtext = e_draw_text_none, LPGLYPHMETRICS lpgm,
////         unsigned int cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2) override;
//
////      bool GetCharABCWidths(unsigned int nFirstChar, unsigned int nLastChar,
////         LPABCFLOAT lpABCF) override;
//      bool GetCharWidth(unsigned int nFirstChar, unsigned int nLastChar,
//                        float* lpFloatBuffer) override;
//
//      unsigned int GetFontLanguageInfo() override;
//
//#if (_WIN32_WINNT >= 0x0500)
//
////      bool GetCharABCWidthsI(unsigned int giFirst, unsigned int cgi, LPWORD pgi, LPABC lpabc) override;
////      bool GetCharWidthI(unsigned int giFirst, unsigned int cgi, LPWORD pgi, LPINT lpBuffer) override;
//
//#endif

      //// Printer/Device Escape Functions
      //virtual int Escape(int nEscape, int nCount, const ::scoped_string & scopedstrInData, LPVOID lpOutData) override;
      //virtual int Escape(int nEscape, int nInputSize, const ::scoped_string & scopedstrInputData, int nOutputSize, char * lpszOutputData) override;
      //virtual int DrawEscape(int nEscape, int nInputSize, const ::scoped_string & scopedstrInputData) override;

      // Escape helpers
      //int StartDoc(const ::scoped_string & scopedstrDocName) override;  // old Win3.0 version
//      int StartDoc(LPDOCINFO lpDocInfo) override;
      int StartPage() override;
      int EndPage() override;
      //int SetAbortProc(bool (CALLBACK* lpfn)(HDC, int)) override;
      int AbortDoc() override;
      int EndDoc() override;

      // MetaFile Functions
      //bool PlayMetaFile(HMETAFILE hMF) override;
      //bool PlayMetaFile(HENHMETAFILE hEnhMetaFile, const ::double_rectangle & rectangleBounds) override;
      bool AddMetaFileComment(unsigned int nDataSize, const unsigned char* pCommentData) override;
      // can be used for enhanced metafiles only

      // Path Functions
      bool abort_path() override;
      bool begin_path() override;
      bool close_figure() override;
      bool end_path() override;
      bool fill_path() override;
      bool flatten_path() override;
      bool stroke_and_fill_path() override;
      bool stroke_path() override;
      bool widen_path() override;
      float GetMiterLimit() override;
      bool SetMiterLimit(float fMiterLimit) override;
      virtual int GetPath(::double_point * ppoints, LPBYTE lpTypes, ::collection::count nCount) override;
      bool SelectClipPath(int nMode) override;

      // Misc Helper Functions
      static ::draw2d::brush* GetHalftoneBrush(::particle * pparticle);
      //void DrawDragRect(const ::double_rectangle & rectangle, const ::double_size & size,
      //                  const ::double_rectangle & rectangleLast, const ::double_size & sizeLast,
      //                  ::draw2d::brush* pBrush = nullptr, ::draw2d::brush* pBrushLast = nullptr) override;

      bool fill_rectangle(const ::double_rectangle & rectangle, const ::color::color & color) override;
      //void fill_rectangle(const ::double_rectangle & rectangle, const ::color::color & color) override;

      //bool draw_inset_3d_rectangle(const ::double_rectangle & rectangle, const ::color::color & colorTopLeft, const ::color::color & colorBottomRight, const ::e_border & eborder = e_border_all) override;
      //void draw_inset_3d_rectangle(const ::double_rectangle & rectangle, const ::color::color & colorTopLeft, const ::color::color & colorBottomRight, const ::e_border & eborder = e_border_all) override;


      void assert_ok() const override;
      void dump(dump_context & dumpcontext) const override;


      virtual bool set_alpha_mode(::draw2d::enum_alpha_mode ealphamode) override;


      virtual HDC get_handle() const;
      virtual HDC get_handle1() const;
      virtual HDC get_handle2() const;

      virtual bool attach(void * pdata) override;
      virtual void * detach() override;

#ifdef WINDOWS_DESKTOP
      //virtual bool attach_hdc(HDC hdc) override;
      //virtual HDC detach_hdc() override;
#endif

//      virtual Gdiplus::FillMode directx11_get_fill_mode() override;

      bool blur(bool bExpand, double dRadius, const ::double_rectangle & rectangle) override;


      virtual bool destroy();

      //IDWriteTextFormat * get_os_font(::write_text::font * pfont);
      //ID2D1Brush * get_os_brush(::draw2d::brush * pbrush);
      //ID2D1Brush * get_os_pen_brush(::draw2d::pen * ppen);

      using ::draw2d::graphics::draw;

      virtual bool draw(const ::text_out & textout, ::draw2d::pen * ppen);

      virtual bool fill(const ::text_out & textout, ::draw2d::brush * pbrush);

      virtual bool draw(const ::draw_text & drawtext, ::draw2d::pen* ppen);

      virtual bool fill(const ::draw_text & drawtext, ::draw2d::brush* pbrush);


      virtual bool flush() override;

      virtual void debug() override;
      //protected:
      // used for implementation of non-virtual SelectObject calls
      //static ::draw2d::object* SelectGdiObject(application * papp, HDC hDC, HGDIOBJ h) override;

      //virtual void enum_fonts(::write_text::font_enumeration_item_array & itema) override;

//#ifdef UNIVERSAL_WINDOWS
//      virtual void set_directx11_plugin(::draw2d_directx11::plugin * pplugin);
//#endif

      //virtual bool update_window(::image::image *  pimage) override;

   };


} // namespace draw2d_directx11



