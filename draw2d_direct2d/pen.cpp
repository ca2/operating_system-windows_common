#include "platform.h"
#include "pen.h"
#include "graphics.h"
#include "direct2d/direct2d.h"


namespace draw2d_direct2d
{


   pen::pen()
   {

      //m_pthis = this;
      m_bMetroColor     = false;
      //m_colorMetro = {};

   }


   pen::~pen()
   {

      destroy();

   }


   //void pen::dump(dump_context & dumpcontext) const
   //{

   //   ::draw2d::object::dump(dumpcontext);

   //}


   //comptr < ID2D1StrokeStyle1 > pen::_create_stroke_style(::draw2d::graphics * pdraw2dgraphics)
   //{

   //   bool bProperties = false;

   //   D2D1_STROKE_STYLE_PROPERTIES1 properties1{};

   //   if (m_elinecapBeg == ::draw2d::e_line_cap_round)
   //   {

   //      properties1.startCap = D2D1_CAP_STYLE_ROUND;

   //      bProperties = true;

   //   }


   //   if (m_elinecapEnd == ::draw2d::e_line_cap_round)
   //   {

   //      properties1.endCap = D2D1_CAP_STYLE_ROUND;

   //      bProperties = true;

   //   }


   //   if (!bProperties)
   //   {

   //      return nullptr;

   //   }

   //   comptr < ID2D1StrokeStyle1 > pstrokestyle;

   //   HRESULT hr = direct2d()->d2d1_factory1()->CreateStrokeStyle(&properties1, nullptr, 0, &pstrokestyle);

   //   return pstrokestyle;

   //}


   void pen::update(::draw2d::graphics * pdraw2dgraphics)
   {

      ::cast < ::draw2d_direct2d::graphics > pdraw2ddirect2dgraphics = pdraw2dgraphics;

      if((!m_bMetroColor || m_colorMetro != m_color) || m_pdraw2dbrush == nullptr)
      {

         D2D1_COLOR_F color;

         copy(color, m_color);

         constructø(m_pdraw2dbrush);

         m_pdraw2dbrush->create_solid(m_color);

         //::cast < ::draw2d_direct2d::brush > pdraw2ddirect2dbrush;

         //pdraw2ddirect2dgraphics->m_pd2d1devicecontext->CreateSolidColorBrush(color, &pdraw2ddirect2dbrush->m_pd2);

         //if(m_pdraw2dbrush != nullptr)
         //{

         //m_osdata[0] = (ID2D1Brush *) m_pdraw2dbrush;
            
         //m_colorMetro = m_color;

         m_bMetroColor   = true;
            
 //

         m_pd2d1strokestyle1 = _create_stroke_style(pdraw2dgraphics,
            m_elinecapBeg,
            m_elinecapEnd);

         if (m_pd2d1strokestyle1 != nullptr)
         {

            //m_osdata[1] = (ID2D1StrokeStyle1 *)m_pstrokestyle;

         }

      }

      //return m_osdata[0] != nullptr;

   }


   void pen::clear_node_data()
   {

      //destroy_os_data();
      m_pdraw2dbrush = nullptr;

      m_pd2d1strokestyle1 = nullptr;

      //::draw2d::pen::destroy();

   }


   //void pen::destroy_os_data()
   //{

   //   object::destroy_os_data();

   //}


   HRESULT pen::s_RenderPatternToCommandList(ID2D1RenderTarget * pdraw2dgraphics,D2D1_COLOR_F *pcr)
   {

      HRESULT hr = S_OK;

      //pdraw2dgraphics->BeginDraw();

      pdraw2dgraphics->Clear(pcr);

      ID2D1SolidColorBrush * pbr = nullptr;

      //hr = pdraw2dgraphics->CreateSolidColorBrush(*pcr, &pbr);

      //pdraw2dgraphics->DrawRectangle(D2D1::RectF(0.f, 0.f, 256.f, 256.f), pbr, 0.f);

      //pbr->Release();

      ///hr = pdraw2dgraphics->EndDraw();

      return hr;

   }


   HRESULT pen::s_CreatePatternBrush(ID2D1DeviceContext *pDeviceContext, D2D1_COLOR_F * pcr, ID2D1ImageBrush **ppImageBrush)
   {

      //HRESULT hrEndDraw = pDeviceContext->EndDraw();

      //if (FAILED(hrEndDraw))
      //{

      //   throw ::exception(error_failed, "graphics::on_release_memory_graphics : EndDraw failed: {}", hresult_text(hrEndDraw));

      //   //warning("graphics::on_release_memory_graphics : EndDraw failed: {}", hresult_text(hrEndDraw));

      //}

      HRESULT hr = S_OK;

      ID2D1Image *pOldTarget = nullptr;

      pDeviceContext->GetTarget(&pOldTarget);

      ID2D1CommandList *pCommandList = nullptr;

      hr = pDeviceContext->CreateCommandList(&pCommandList);

      if(SUCCEEDED(hr))
      {
         
         pDeviceContext->SetTarget(pCommandList);
         
         hr = s_RenderPatternToCommandList(pDeviceContext,pcr);

      }

      pDeviceContext->SetTarget(pOldTarget);

      ID2D1ImageBrush *pImageBrush = nullptr;

      if(SUCCEEDED(hr))
      {
         
         hr = pCommandList->Close();

      }

      if(SUCCEEDED(hr))
      {

         D2D1_IMAGE_BRUSH_PROPERTIES props;

         props.sourceRectangle.left = 0.f;
         props.sourceRectangle.top = 0.f;
         props.sourceRectangle.right = 256.f;
         props.sourceRectangle.bottom = 256.f;

         props.extendModeX = D2D1_EXTEND_MODE_WRAP;
         props.extendModeY = D2D1_EXTEND_MODE_WRAP;

         props.interpolationMode = D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;

         hr = pDeviceContext->CreateImageBrush(
              pCommandList,&props,nullptr,&pImageBrush);
      }

      // Fill a i32_rectangle with the image brush.
      /*if (SUCCEEDED(hr))
      {
      pDeviceContext->fill_rectangle(
      D2D1::RectF(0, 0, 100, 100), pImageBrush);
      }*/

      //pImageBrush->Release();
      pCommandList->Release();
      pOldTarget->Release();

      //if(hrEndDraw == S_OK)
      //{
      //   pDeviceContext->BeginDraw();
      //}

      if(SUCCEEDED(hr))
      {

         *ppImageBrush = pImageBrush;

      }
      else
      {

         *ppImageBrush = nullptr;

      }

      return hr;

   }


} // namespace draw2d_direct2d



