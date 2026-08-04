// Created by camilo on 2026-08-04 01:32 <3ThomasBorregaardSørensen!! Mummi!! Bilbo!!
#include "framework.h"
#include "draw2d_window_attachment.h"



namespace gpu_directx11
{


   draw2d_window_attachment::draw2d_window_attachment()
   {

   }


   draw2d_window_attachment::~draw2d_window_attachment()
   {


   }

   //void device::on_new_frame()
   void draw2d_window_attachment::on_start_frame()
   {

      //gpu::device::on_new_frame();

      gpu::draw2d_window_attachment::on_start_frame();

      //auto& pframestorage = m_framestoragea.ø(m_iCurrentFrame2);

      //if (!pframestorage)
      //{

      //   defer_construct_newø(pframestorage);

      //}

      //pframestorage->m_iBuffer = 0;

      //pframestorage->m_iBufferOffset = 0;

   }


   // void device::on_top_end_frame()
   void draw2d_window_attachment::on_end_frame()
   {

      // gpu::device::on_top_end_frame();

      gpu::draw2d_window_attachment::on_end_frame();
   }


} // namespace gpu_directx11



