/************************************************************************
*
* File: CustomTextRenderer.h
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
* THIS CODE AND log_information( ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY
* KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
* PARTICULAR PURPOSE.
*
************************************************************************/
#pragma once


#include <d2d1.h>
#include <dwrite.h>



/******************************************************************
*                                                                 *
*  CustomTextRenderer                                             *
*                                                                 *
*  The IDWriteTextRenderer interface is an input parameter to     *
*  IDWriteTextLayout::Draw.  This interfaces defines a number of  *
*  callback functions that the client application implements for  *
*  custom text rendering.  This sample renderer implementation    *
*  renders text using text outlines and Direct2D.                 *
*  A more sophisticated client would also support bitmap          *
*  renderings.                                                    *
*                                                                 *
******************************************************************/

class CustomTextRenderer : public IDWriteTextRenderer
{
private:


   unsigned long                                m_cReference;
   comptr<ID2D1Factory>         m_pfactory;
   comptr<ID2D1RenderTarget>    m_prendertarget;
   comptr<ID2D1Brush>           m_pbrushOutline;
   comptr<ID2D1Brush>           m_pbrushFill;


public:


   IFACEMETHOD_(unsigned long, AddRef) ();
   IFACEMETHOD_(unsigned long, Release) ();
   IFACEMETHOD(QueryInterface) (IID const& riid, void** ppvObject);


   CustomTextRenderer(ID2D1Factory* pD2DFactory, ID2D1RenderTarget* pRT, ID2D1Brush* pOutlineBrush, ID2D1Brush* pFillBrush = nullptr);
   ~CustomTextRenderer();


   IFACEMETHOD(IsPixelSnappingDisabled)(__maybenull void* clientDrawingContext, __out BOOL* isDisabled);

   IFACEMETHOD(GetCurrentTransform)(__maybenull void* clientDrawingContext, __out DWRITE_MATRIX* transform);

   IFACEMETHOD(GetPixelsPerDip)(__maybenull void* clientDrawingContext, __out FLOAT* pixelsPerDip);

   IFACEMETHOD(DrawGlyphRun)(
      __maybenull void* clientDrawingContext,
      FLOAT baselineOriginX,
      FLOAT baselineOriginY,
      DWRITE_MEASURING_MODE measuringMode,
      __in DWRITE_GLYPH_RUN const* glyphRun,
      __in DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
      IUnknown* clientDrawingEffect
      );

   IFACEMETHOD(DrawUnderline)(
      __maybenull void* clientDrawingContext,
      FLOAT baselineOriginX,
      FLOAT baselineOriginY,
      __in DWRITE_UNDERLINE const* underline,
      IUnknown* clientDrawingEffect
      );

   IFACEMETHOD(DrawStrikethrough)(
      __maybenull void* clientDrawingContext,
      FLOAT baselineOriginX,
      FLOAT baselineOriginY,
      __in DWRITE_STRIKETHROUGH const* strikethrough,
      IUnknown* clientDrawingEffect
      );

   IFACEMETHOD(DrawInlineObject)(
      __maybenull void* clientDrawingContext,
      FLOAT originX,
      FLOAT originY,
      IDWriteInlineObject* inlineObject,
      BOOL isSideways,
      BOOL isRightToLeft,
      IUnknown* clientDrawingEffect
      );


};



