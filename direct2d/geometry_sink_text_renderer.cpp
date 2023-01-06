// From direct2d_draw2d by camilo on ThomasBirthday2021 15:58 BRT
// Renamed from CustomTextRenderer to geometry_sink_text_renderer by camilo just after ThomasBirthday2021 03:07 BRT
#include "framework.h"
#include "geometry_sink_text_renderer.h"

//#undef new
//#ifdef _UWP
//#include <wrl/module.h>
//#endif


#ifdef _UWP


//
// Copyright (C) Microsoft Corporation
// All rights reserved.
//

#pragma warning( disable: 4073 )  // initializers put in library initialization area
#pragma init_seg( lib )

extern int __abi___threading_model;
//__abi_Module* __abi_module = nullptr;

#undef Platform

namespace Platform {
   namespace Details {

      //class InProcModule :
      //	public Microsoft::WRL::Module<Microsoft::WRL::InProcDisableCaching, InProcModule>,
      //	public __abi_Module
      //{
      //public:
      //	InProcModule()
      //	{
      //		__abi_module = this;
      //	}

      //	virtual ~InProcModule()
      //	{
      //		__abi_module = nullptr;
      //	}

      //	virtual unsigned long __stdcall __abi_IncrementObjectCount()
      //	{
      //		return Module::IncrementObjectCount();
      //	}

      //	virtual unsigned long __stdcall __abi_DecrementObjectCount()
      //	{
      //		return Module::DecrementObjectCount();
      //	}
      //};

      //auto &module_ = InProcModule::Create();


      // Forward declarations from vccorlib DLL
      HRESULT InitializeData(int __threading_model);
      void UninitializeData(int __threading_model);

      //Initializaton forwarders
      void __cdecl Cleanup();

      // Initialize MoCOM data and clean up handlers
//		int __cdecl Initialize()
//		{
//			//Global initialization
///*			HRESULT hr = InitializeData(__abi___threading_model);
//			if (FAILED(hr))
//			{
//				Cleanup();
//				return hr;
//			}*/
//
//			// Register cleaning handler
//			atexit(Cleanup);
//			return 0;
//		}
//
//		// Global clean up
//		void __cdecl Cleanup()
//		{
//			UninitializeData(__abi___threading_model);
//		}

   }
} // namespace Details

//Put initialization data into the department that we can return failure
//#pragma section(".CRT$XIY",long,read)
//extern "C" __declspec(allocate(".CRT$XIY")) void* __abi__initialize = Details::Initialize;

// All required libraries must be pulled in in init.cpp file because it always referenced
// The librairies should not be pulled in vccorlib.h unless it's vccorlib*.lib


#endif










/************************************************************************
*
* File: geometry_sink_text_renderer.cpp
*
* Description:
*
*
*  This file is part of the Microsoft Windows SDK Code Samples.
*
*  Copyright (C) Microsoft Corporation.  All rights reserved.
*
* This source code is intended only as a supplement to Microsoft
* Development Tools and/or on-line documentation.  See these other
* materials for detailed information regarding Microsoft code samples.
*
* THIS CODE AND INFORMATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY
* KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
* PARTICULAR PURPOSE.
*
************************************************************************/

//#include "DWriteHelloWorld.h"

/******************************************************************
*                                                                 *
*  geometry_sink_text_renderer::geometry_sink_text_renderer                         *
*                                                                 *
*  The constructor stores the Direct2D factory_item, the render        *
*  target, and the outline and fill brushes used for drawing the  *
*  glyphs, underlines, and strikethroughs.                        *
*                                                                 *
******************************************************************/

geometry_sink_text_renderer::geometry_sink_text_renderer(
   //ID2D1Factory * pfactory,
   //ID2D1GeometrySink * pgeometrysink //,
   //ID2D1Brush* pbrushOutline,
   //ID2D1Brush* pbrushFill
)
   :
   m_cReference(0) //,
   //m_pfactory(pfactory),
   //m_pgeometrysink(pgeometrysink)
   //,
   //m_pbrushOutline(pbrushOutline),
   //m_pbrushFill(pbrushFill)
{
}


void geometry_sink_text_renderer::SetFillMode(
   D2D1_FILL_MODE fillMode
)
{
   //m_pgeometrysink->SetFillMode(fillMode);
}

void geometry_sink_text_renderer::SetSegmentFlags(
   D2D1_PATH_SEGMENT vertexFlags
)
{
   m_pgeometrysink->SetSegmentFlags(vertexFlags);

}

void geometry_sink_text_renderer::BeginFigure(
   D2D1_POINT_2F startPoint,
   D2D1_FIGURE_BEGIN figureBegin
)
{
   startPoint.x += m_xOffset;
   startPoint.y += m_yOffset;
   auto begin = m_figurebeginOverride == D2D1_FIGURE_BEGIN_HOLLOW ? D2D1_FIGURE_BEGIN_HOLLOW : figureBegin;
   m_pgeometrysink->BeginFigure(startPoint, begin);

}

void geometry_sink_text_renderer::AddLines(
   const D2D1_POINT_2F * points,
   UINT32 pointsCount)
{
   auto & m = m_memory;
   m.set_size(sizeof(D2D1_POINT_2F) * pointsCount);
   auto points2 = (D2D1_POINT_2F *)m.data();
   memcpy(points2, points, m.size());
   for (UINT32 u = 0; u < pointsCount; u++)
   {
      points2[u].x += m_xOffset;
      points2[u].y += m_yOffset;
   }
   m_pgeometrysink->AddLines(points2, pointsCount);
}

void geometry_sink_text_renderer::AddBeziers(const D2D1_BEZIER_SEGMENT * beziers, UINT32 beziersCount)
{
   auto & m = m_memory;
   m.set_size(sizeof(D2D1_BEZIER_SEGMENT) * beziersCount);
   auto beziers2 = (D2D1_BEZIER_SEGMENT *)m.data();
   memcpy(beziers2, beziers, m.size());
   for (UINT32 u = 0; u < beziersCount; u++)
   {
      beziers2[u].point1.x += m_xOffset;
      beziers2[u].point1.y += m_yOffset;
      beziers2[u].point2.x += m_xOffset;
      beziers2[u].point2.y += m_yOffset;
      beziers2[u].point3.x += m_xOffset;
      beziers2[u].point3.y += m_yOffset;
   }

   m_pgeometrysink->AddBeziers(beziers2, beziersCount);

}

void geometry_sink_text_renderer::EndFigure(D2D1_FIGURE_END figureEnd)
{
   m_pgeometrysink->EndFigure(figureEnd);


}


HRESULT geometry_sink_text_renderer::Close()
{

   return m_pgeometrysink->Close();

}



/******************************************************************
*                                                                 *
*  geometry_sink_text_renderer::~geometry_sink_text_renderer                        *
*                                                                 *
*  The destructor releases the member variables                   *
*                                                                 *
******************************************************************/

geometry_sink_text_renderer::~geometry_sink_text_renderer()
{
   //m_pfactory->Release();
   //if(m_pbrushOutline != nullptr)
   //{
   //   m_pbrushOutline->Release();
   //}
   //if(m_pbrushFill != nullptr)
   //{
   //   m_pbrushFill->Release();
   //}
}

/******************************************************************
*                                                                 *
*  geometry_sink_text_renderer::DrawGlyphRun                               *
*                                                                 *
*  Gets GlyphRun outlines via IDWriteFontFace::GetGlyphRunOutline *
*  and then draws and fills them using Direct2D path geometries   *
*                                                                 *
******************************************************************/

IFACEMETHODIMP geometry_sink_text_renderer::DrawGlyphRun(
   __maybenull void * clientDrawingContext,
   FLOAT baselineOriginX,
   FLOAT baselineOriginY,
   DWRITE_MEASURING_MODE measuringMode,
   __in DWRITE_GLYPH_RUN const * glyphRun,
   __in DWRITE_GLYPH_RUN_DESCRIPTION const * glyphRunDescription,
   IUnknown * clientDrawingEffect
)
{

   HRESULT hr = S_OK;

   //comptr<ID2D1PathGeometry>        pPathGeometry;

   //comptr<ID2D1GeometrySink>        pSink;

   //hr = m_pfactory->CreatePathGeometry(&pPathGeometry);

   //if(SUCCEEDED(hr))
   //{

   //   hr = pPathGeometry->Open(&pSink);

   //}

   // Get the glyph run outline geometries back from DirectWrite and place them within the
   // geometry sink.
//   if(SUCCEEDED(hr))
   {
      m_xOffset = baselineOriginX;
      m_yOffset = baselineOriginY;

      hr = glyphRun->fontFace->GetGlyphRunOutline(
         glyphRun->fontEmSize,
         glyphRun->glyphIndices,
         glyphRun->glyphAdvances,
         glyphRun->glyphOffsets,
         glyphRun->glyphCount,
         glyphRun->isSideways,
         glyphRun->bidiLevel % 2,
         this
      );

   }

   // Close the geometry sink
   //if(SUCCEEDED(hr))
   //{

   //   hr = pSink->Close();

   //}

   //// Initialize a matrix to translate the origin of the glyph run.
   //D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
   //   1.0f,0.0f,
   //   0.0f,1.0f,
   //   baselineOriginX,baselineOriginY
   //   );

   //// Create the transformed geometry
   //comptr<ID2D1TransformedGeometry> pTransformedGeometry;
   //if (SUCCEEDED(hr))
   //{

   //   hr = m_pfactory->CreateTransformedGeometry(pPathGeometry, &matrix, &pTransformedGeometry);

   //}

   //comptr<ID2D1PathGeometry> pGeometry;

   //if (SUCCEEDED(hr))
   //{

   //   hr = pTransformedGeometry.as(pGeometry);



   //}

   //if (SUCCEEDED(hr))
   //{

   //   pGeometry->Stream(m_pgeometrysink);

   //}
      //if (SUCCEEDED(hr))
   //{
   //   
   //   // Draw the outline of the glyph run
   //   if (m_pbrushOutline != nullptr)
   //   {
   //      
   //      m_prendertarget->DrawGeometry(pTransformedGeometry, m_pbrushOutline);

   //   }

   //}

   //if (SUCCEEDED(hr))
   //{

   //   if (m_pbrushFill != nullptr)
   //   {

   //      m_prendertarget->FillGeometry(pTransformedGeometry, m_pbrushFill);

   //   }

   //}

   return hr;

}


/******************************************************************
*                                                                 *
*  geometry_sink_text_renderer::DrawUnderline                              *
*                                                                 *
*  Draws underlines below the text using a Direct2D recatangle    *
*  geometry                                                       *
*                                                                 *
******************************************************************/

IFACEMETHODIMP geometry_sink_text_renderer::DrawUnderline(
   __maybenull void * clientDrawingContext,
   FLOAT baselineOriginX,
   FLOAT baselineOriginY,
   __in DWRITE_UNDERLINE const * underline,
   IUnknown * clientDrawingEffect
)
{
   /*HRESULT hr;

   D2D1_RECT_F rectangle = D2D1::RectF(
      0,
      underline->offset,
      underline->width,
      underline->offset + underline->thickness
      );

   ID2D1RectangleGeometry* pRectangleGeometry = nullptr;
   hr = m_pfactory->CreateRectangleGeometry(
      &rectangle,
      &pRectangleGeometry
      );

   // Initialize a matrix to translate the origin of the underline
   D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
      1.0f,0.0f,
      0.0f,1.0f,
      baselineOriginX,baselineOriginY
      );

   ID2D1TransformedGeometry* pTransformedGeometry = nullptr;
   if(SUCCEEDED(hr))
   {
      hr = m_pfactory->CreateTransformedGeometry(
         pRectangleGeometry,
         &matrix,
         &pTransformedGeometry
         );
   }

   // Draw the outline of the rectangle_i32
   m_prendertarget->DrawGeometry(
      pTransformedGeometry,
      m_pbrushOutline
      );

   // Fill in the rectangle_i32
   m_prendertarget->FillGeometry(
      pTransformedGeometry,
      m_pbrushFill
      );

      */
   return S_OK;
}

/******************************************************************
*                                                                 *
*  geometry_sink_text_renderer::DrawStrikethrough                          *
*                                                                 *
*  Draws strikethroughs below the text using a Direct2D           *
*  recatangle geometry                                            *
*                                                                 *
******************************************************************/

IFACEMETHODIMP geometry_sink_text_renderer::DrawStrikethrough(
   __maybenull void * clientDrawingContext,
   FLOAT baselineOriginX,
   FLOAT baselineOriginY,
   __in DWRITE_STRIKETHROUGH const * strikethrough,
   IUnknown * clientDrawingEffect
)
{
   /*HRESULT hr;

   D2D1_RECT_F rectangle = D2D1::RectF(
      0,
      strikethrough->offset,
      strikethrough->width,
      strikethrough->offset + strikethrough->thickness
      );

   ID2D1RectangleGeometry* pRectangleGeometry = nullptr;
   hr = m_pfactory->CreateRectangleGeometry(
      &rectangle,
      &pRectangleGeometry
      );

   // Initialize a matrix to translate the origin of the strikethrough
   D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
      1.0f,0.0f,
      0.0f,1.0f,
      baselineOriginX,baselineOriginY
      );

   ID2D1TransformedGeometry* pTransformedGeometry = nullptr;
   if(SUCCEEDED(hr))
   {
      hr = m_pfactory->CreateTransformedGeometry(
         pRectangleGeometry,
         &matrix,
         &pTransformedGeometry
         );
   }

   // Draw the outline of the rectangle_i32
   m_prendertarget->DrawGeometry(
      pTransformedGeometry,
      m_pbrushOutline
      );

   // Fill in the rectangle_i32
   m_prendertarget->FillGeometry(
      pTransformedGeometry,
      m_pbrushFill
      );

   SafeRelease(&pRectangleGeometry);
   SafeRelease(&pTransformedGeometry);*/

   return S_OK;
}

/******************************************************************
*                                                                 *
*  geometry_sink_text_renderer::DrawInlineObject                           *
*                                                                 *
*  This function is not implemented for the purposes of this      *
*  sample.                                                        *
*                                                                 *
******************************************************************/

IFACEMETHODIMP geometry_sink_text_renderer::DrawInlineObject(
   __maybenull void * clientDrawingContext,
   FLOAT originX,
   FLOAT originY,
   IDWriteInlineObject * inlineObject,
   BOOL isSideways,
   BOOL isRightToLeft,
   IUnknown * clientDrawingEffect
)
{
   // Not implemented
   return E_NOTIMPL;
}

IFACEMETHODIMP_(unsigned long) geometry_sink_text_renderer::AddRef()
{
   return InterlockedIncrement(&m_cReference);
}

IFACEMETHODIMP_(unsigned long) geometry_sink_text_renderer::Release()
{
   unsigned long newCount = InterlockedDecrement(&m_cReference);
   if (newCount == 0)
   {
      delete this;
      return 0;
   }

   return newCount;
}

/******************************************************************
*                                                                 *
*  geometry_sink_text_renderer::IsPixelSnappingDisabled                    *
*                                                                 *
*  Determines whether pixel snapping is disabled. The recommended *
*  default is false, unless doing animation that requires         *
*  subpixel vertical placement.                                   *
*                                                                 *
******************************************************************/

IFACEMETHODIMP geometry_sink_text_renderer::IsPixelSnappingDisabled(
   __maybenull void * clientDrawingContext,
   __out BOOL * isDisabled
)
{
   *isDisabled = false;
   return S_OK;
}

/******************************************************************
*                                                                 *
*  geometry_sink_text_renderer::GetCurrentTransform                        *
*                                                                 *
*  Returns the current transform applied to the render target..   *
*                                                                 *
******************************************************************/
IFACEMETHODIMP geometry_sink_text_renderer::GetCurrentTransform(__maybenull void * clientDrawingContext, __out DWRITE_MATRIX * transform)
{
   
   *transform = m_matrixTransform;
   
   return S_OK;

}


/******************************************************************
*                                                                 *
*  geometry_sink_text_renderer::GetPixelsPerDip                            *
*                                                                 *
*  This returns the number of pixels per DIP.                     *
*                                                                 *
******************************************************************/
IFACEMETHODIMP geometry_sink_text_renderer::GetPixelsPerDip(__maybenull void * clientDrawingContext, __out FLOAT * pixelsPerDip)
{

   *pixelsPerDip = (FLOAT) (m_dDpi / 96.0);

   return S_OK;

}


/******************************************************************
*                                                                 *
*  geometry_sink_text_renderer::QueryInterface                             *
*                                                                 *
*  Query interface implementation                                 *
*                                                                 *
******************************************************************/

IFACEMETHODIMP geometry_sink_text_renderer::QueryInterface(
   IID const & riid,
   void ** ppvObject
)
{
   if (__uuidof(IDWriteTextRenderer) == riid)
   {
      *ppvObject = this;
   }
   else if (__uuidof(IDWritePixelSnapping) == riid)
   {
      *ppvObject = this;
   }
   else if (__uuidof(IUnknown) == riid)
   {
      *ppvObject = this;
   }
   else if (__uuidof(ID2D1PathGeometry) == riid)
   {
      *ppvObject = nullptr;
      return E_FAIL;
   }
   else
   {
      *ppvObject = nullptr;
      return E_FAIL;
   }

   this->AddRef();

   return S_OK;
}

