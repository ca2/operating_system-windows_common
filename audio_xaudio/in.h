#pragma once


#include "audio/audio/wave/in.h"


namespace multimedia
{


   namespace audio_xaudio
   {


      class  CLASS_DECL_AUDIO_XAUDIO in :
         virtual public ::wave::in
      {
      public:


         
         //HWAVEIN              m_hwavein;

         WAVEFORMATEX         m_waveformatex;


         in();
         ~in() override;


         void in_initialize_encoder() override;

         virtual void     in_add_buffer(i32 iBuffer);
         //virtual void     in_add_buffer(LPWAVEHDR lpwavehdr);

         //HWAVEIN in_get_safe_HWAVEIN();
         virtual void * get_os_data();

         void in_open(i32 iBufferCount, i32 iBufferSampleCount);
         void in_close() override;
         void in_stop() override;
         void in_start() override;
         void in_reset() override;

         virtual void translate_in_message(::message::message * pmessage);

         void init_task() override;
         void term_task() override;
         virtual void pre_translate_message(::message::message * pmessage);

         //static void CALLBACK in_proc(HWAVEIN hwi, ::u32 uMsg, ::u32 dwInstance, ::u32 dwParam1, ::u32 dwParam2);

         WAVEFORMATEX * wave_format();
         //LPWAVEHDR wave_hdr(int iBuffer);


      };


   } // namespace audio_xaudio


} // namespace multimedia




