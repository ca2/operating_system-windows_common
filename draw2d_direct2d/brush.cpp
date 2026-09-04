#include "platform.h"
#include "bitmap.h"
#include "brush.h"
#include "graphics.h"
#include "aura/graphics/image/image.h"


namespace draw2d_direct2d
{


   brush::brush()
   {
      
   }


   brush::~brush()
   {

      destroy();

   }


   void brush::update(::draw2d::graphics * pdraw2dgraphics)
   {

      ::cast<::draw2d_direct2d::graphics> pdirect2ddraw2dgraphics = pdraw2dgraphics;

      if (pdirect2ddraw2dgraphics->m_pd2d1devicecontext == nullptr)
      {

         throw ::exception(error_null_pointer);

      }

      if(m_ebrush == ::draw2d::e_brush_solid)
      {

         if(m_pd2d1solidcolorbrush == nullptr)
         {

            D2D1_COLOR_F color;

            copy(color, m_color);

            pdirect2ddraw2dgraphics->m_pd2d1devicecontext->CreateSolidColorBrush(color, &m_pd2d1solidcolorbrush);

            m_pd2d1brush = m_pd2d1solidcolorbrush;

         }

      }
      else if(m_ebrush == ::draw2d::e_brush_linear_gradient_point_color)
      {

         if(m_pd2d1lineargradientbrush == nullptr)
         {

            D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES prop{};

            //auto pointViewport = pdraw2dgraphics->GetViewportOrg();
            ::i32_point pointViewport(0, 0);

            prop.startPoint.x    = (FLOAT) m_point1.x + pointViewport.x;
            prop.startPoint.y    = (FLOAT) m_point1.y + pointViewport.y;
            prop.endPoint.x      = (FLOAT) m_point2.x + pointViewport.x;
            prop.endPoint.y      = (FLOAT) m_point2.y + pointViewport.y;

            // Create an array of gradient stops to put in the gradient stop
            // collection that will be used in the gradient brush.
            ::comptr<ID2D1GradientStopCollection> pstopcollection;

            D2D1_GRADIENT_STOP gradientstops[2] = {};

            copy(gradientstops[0].color, m_color1);
            gradientstops[0].position = 0.0f;

            copy(gradientstops[1].color, m_color2);
            gradientstops[1].position = 1.0f;

            // Create the ID2D1GradientStopCollection from a previously
            // declared array of D2D1_GRADIENT_STOP structs.
            HRESULT hr = pdirect2ddraw2dgraphics->m_pd2d1devicecontext->CreateGradientStopCollection(gradientstops, 2, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &pstopcollection);

            D2D1_BRUSH_PROPERTIES brushproperties = {};

            brushproperties.opacity = 1.0f;
            brushproperties.transform =  D2D1::IdentityMatrix();

            hr = pdirect2ddraw2dgraphics->m_pd2d1devicecontext->CreateLinearGradientBrush(&prop, &brushproperties, pstopcollection, &m_pd2d1lineargradientbrush);

            m_pd2d1brush = m_pd2d1lineargradientbrush;

         }

      }
      else if(m_ebrush == ::draw2d::e_brush_radial_gradient_color)
      {

         if(m_pd2d1lineargradientbrush == nullptr)
         {

            // Create an array of gradient stops to put in the gradient stop
            // collection that will be used in the gradient brush.
            ID2D1GradientStopCollection *pgradientstops = nullptr;

            D2D1_GRADIENT_STOP gradientstops[2];

            copy(gradientstops[0].color, m_color1);
            gradientstops[0].position = 0.0f;

            copy(gradientstops[1].color, m_color2);
            gradientstops[1].position = 1.0f;

            // Create the ID2D1GradientStopCollection from a previously
            // declared array of D2D1_GRADIENT_STOP structs.
            HRESULT hr = pdirect2ddraw2dgraphics->m_pd2d1devicecontext->CreateGradientStopCollection(
                         gradientstops,
                         2,
                         D2D1_GAMMA_2_2,
                         D2D1_EXTEND_MODE_CLAMP,
                         &pgradientstops
                         );

            // The center of the gradient is in the center of the box.
            // The gradient origin offset was set to zero(0, 0) or center in this case.
            if(SUCCEEDED(hr))
            {

               ::f64 centerx = m_point.x;
               ::f64 centery = m_point.y;
               ::f64 offsetx = 0.0;
               ::f64 offsety = 0.0;
               ::f64 radiusx = m_size.cx;
               ::f64 radiusy = m_size.cy;

               hr = pdirect2ddraw2dgraphics->m_pd2d1devicecontext->CreateRadialGradientBrush(
                    D2D1::RadialGradientBrushProperties(
                    D2D1::Point2F((FLOAT) (centerx), (FLOAT)(centery)),
                    D2D1::Point2F((FLOAT)(offsetx), (FLOAT)(offsety)),
                    (FLOAT)(radiusx), (FLOAT)(radiusy)),
                    pgradientstops,
                    &m_pd2d1radialgradientbrush
                    );

            }

            m_pd2d1brush = m_pd2d1radialgradientbrush;

            pgradientstops->Release();

         }

      }
      else if (m_ebrush == ::draw2d::e_brush_pattern)
      {

         if (m_pimage.nok())
         {

            // succeeded and resume if and/or when there is a valid image.

            throw ::exception(error_failed);

         }

         if (m_pd2d1imagebrush == nullptr)
         {

            //m_pimage->unmap();

            ::i32 cx = m_pimage->width();

            ::i32 cy = m_pimage->height();

            auto rect = D2D1::RectF(0, 0, (::f32)cx, (::f32)cy);

            auto imagebrushproperties = D2D1::ImageBrushProperties(
               rect,
               D2D1_EXTEND_MODE_WRAP,
               D2D1_EXTEND_MODE_WRAP,
               D2D1_INTERPOLATION_MODE_LINEAR
            );

            ::cast < ::draw2d_direct2d::bitmap > pdraw2ddirect2dbitmap = m_pimage->m_pdraw2dbitmap;

            ID2D1Image* pd2d1image = pdraw2ddirect2dbitmap->m_pd2d1bitmap;

            HRESULT hr = pdirect2ddraw2dgraphics->m_pd2d1devicecontext->CreateImageBrush(
               pd2d1image,
               imagebrushproperties,
               &m_pd2d1imagebrush);

            m_pd2d1brush = m_pd2d1imagebrush;

            if (SUCCEEDED(hr) && m_pd2d1imagebrush != nullptr)
            {

               //m_osdata[0] = (ID2D1Brush*)m_pimagebrush;

               //m_baCalculated[0] = true;

            }

         }

         //return (ID2D1Brush*) m_pimagebrush;

      }
      else
      {

         //return (ID2D1Brush *) m_pdraw2dbrush;

      }

      //return false;

   }


   //void brush::destroy()
   //{

   //   destroy_os_data();

   //   ::draw2d::brush::destroy();

   //}


   void brush::clear_node_data()
   {

      m_pd2d1solidcolorbrush = nullptr;
      m_pd2d1lineargradientbrush = nullptr;
      m_pd2d1radialgradientbrush = nullptr;
      m_pd2d1brush = nullptr;
      m_pd2d1imagebrush = nullptr;
      
      //object::destroy();

   }


} // namespace draw2d_direct2d



