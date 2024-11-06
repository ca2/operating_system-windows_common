#include "framework.h"
#include "out.h"
#include "acme/exception/extended_status.h"
#include "acme/parallelization/synchronous_lock.h"
#include "aqua/platform/system.h"
#include "audio/audio/audio.h"
#include "audio/audio/wave/player.h"
#include "audio/audio/wave/wave.h"
#include "audio/audio/iaudio/wave_stream_effect.h"
#include "multimedia_xaudio/translation.h"


namespace multimedia
{


   namespace audio_xaudio
   {


      out::out()
      {

         m_eoutstate             = ::wave::e_out_state_initial;
         m_pthreadCallback    = nullptr;

      }


      out::~out()
      {

      }


      void out::install_message_routing(::channel * pchannel)
      {

         ::wave::out::install_message_routing(pchannel);

      }


      void out::init_task()
      {

         ::wave::out::init_task();

         //if (!::wave::out::init_thread())
         //{

         //   return false;

         //}

         set_thread_priority(::e_priority_time_critical);

         //return true;

      }


      void out::out_open_ex(thread * pthreadCallback, unsigned int uiSamplesPerSec, unsigned int uiChannelCount, unsigned int uiBitsPerSample, ::wave::enum_purpose epurpose)
      {

         single_lock sLock(synchronization(), true);

         if (m_pxaudio.is_set() && m_pvoice != nullptr && m_psourcevoice != nullptr && m_eoutstate != ::wave::e_out_state_initial)
         {

            return;

         }

         m_iBuffer = 0;

         m_pthreadCallback = pthreadCallback;
         ::e_status     estatus;
         ASSERT(m_pxaudio.is_null());
         ASSERT(m_pvoice == nullptr);
         ASSERT(m_psourcevoice == nullptr);
         ASSERT(m_eoutstate == ::wave::e_out_state_initial);

         CoInitializeEx(nullptr,COINIT_MULTITHREADED);

         if(FAILED(XAudio2Create(&m_pxaudio,0,XAUDIO2_DEFAULT_PROCESSOR)))
         {

            throw ::exception(error_failed);

         }

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG)
         // To see the trace output, you need to impact ETW logs for this application:
         //    Go to Control Panel, Administrative Tools, Event Viewer.
         //    View->Show Analytic and Debug Logs.
         //    Applications and Services Logs / Microsoft / Windows / XAudio2.
         //    Right click on Microsoft Windows XAudio2 debug logging, Properties, then Enable Logging, and hit OK
         XAUDIO2_DEBUG_CONFIGURATION debug = {0};
         debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
         debug.BreakMask = XAUDIO2_LOG_ERRORS;
         m_pxaudio->SetDebugConfiguration(&debug,0);
#endif


         HRESULT hr;

         if(FAILED(hr = m_pxaudio->CreateMasteringVoice(&m_pvoice,uiChannelCount, uiSamplesPerSec)))
         {
            throw ::exception(error_failed);
         }
         int iSampleRate = XAUDIO2_MIN_SAMPLE_RATE;
         estatus = ::success;
         m_pwaveformat->m_waveformat.wFormatTag = WAVE_FORMAT_PCM;
         m_pwaveformat->m_waveformat.nChannels = uiChannelCount;
         m_pwaveformat->m_waveformat.nSamplesPerSec = uiSamplesPerSec;
         m_pwaveformat->m_waveformat.wBitsPerSample = sizeof(::wave::WAVEBUFFERDATA) * 8 ;
         m_pwaveformat->m_waveformat.nBlockAlign = m_pwaveformat->m_waveformat.wBitsPerSample  * uiChannelCount / 8;
         m_pwaveformat->m_waveformat.nAvgBytesPerSec = m_pwaveformat->m_waveformat.nSamplesPerSec * m_pwaveformat->m_waveformat.nBlockAlign;
         //m_pwaveformat->cbSize = 0;

         auto psystem = system();

         auto paudio = psystem->audio()->m_paudio;

         ::pointer<::wave::wave>audiowave = paudio->audiowave();

         //if(FAILED(hr = m_pxaudio->CreateSourceVoice(&m_psourcevoice,wave_format(),XAUDIO2_VOICE_NOSRC | XAUDIO2_VOICE_NOPITCH,1.0f,this)))
         if(FAILED(hr = m_pxaudio->CreateSourceVoice(&m_psourcevoice,wave_format(),0,1.0f,this)))
         {
            throw ::exception(error_failed);
         }

         if(estatus != ::success)
         {

            informationf(status_short_description(estatus));

            throw ::exception(estatus);

         }
         ::collection::count iBufferCount;
//Opened:

         ::collection::count iBufferSampleCount = 4096;

         if(epurpose == ::wave::e_purpose_playback)
         {

            iBufferCount = 8;
            iBufferSampleCount = (1 << 9);

         }
         else if(epurpose == ::wave::e_purpose_playground)
         {

            iBufferCount = 2;
            iBufferSampleCount = (1 << 10);

         }
         else
         {

            iBufferCount = 4;
            iBufferSampleCount = (1 << 9);

         }

         int iAlign = 2048;

         auto uiBufferSize = iBufferSampleCount * m_pwaveformat->m_waveformat.nChannels * 2;

         uiBufferSize = maximum(uiBufferSize,2048);

         ASSERT((uiBufferSize % 2048) == 0);// Streaming int_size must be 2K aligned to use for async I/O

         iBufferSampleCount = uiBufferSize / (m_pwaveformat->m_waveformat.nChannels * 2);



         out_get_buffer()->PCMOutOpen(this, uiBufferSize, iBufferCount,iAlign, m_pwaveformat, m_pwaveformat);

         m_pprebuffer->open(m_pwaveformat->m_waveformat.nChannels, iBufferCount, iBufferSampleCount);

         m_eoutstate = ::wave::e_out_state_opened;

         m_epurpose = epurpose;

         //return ::success;

      }



      void     out::out_close()
      {

         single_lock sLock(synchronization(), true);

         if(m_eoutstate == ::wave::e_out_state_playing)
         {
            out_stop();
         }

         if (m_eoutstate != ::wave::e_out_state_opened)
         {

            //return ::success;

            return;

         }

         ::e_status estatus;

//         int i, iSize;
         //int iSize;

         auto iSize =  out_get_buffer()->GetBufferCount();

         //for(i = 0; i < iSize; i++)
         //{

         HRESULT hresult = m_psourcevoice->Stop();

         estatus = ::windows::hresult_status(hresult);


         //for(i = 0; i < iSize; i++)
         //{

         //   delete m_bufferptra[i];

         //}


         //m_psourcevoice->DestroyVoice();

         try
         {

            m_psourcevoice->DestroyVoice();

         }
         catch(...)
         {

         }

         //m_bufferptra.erase_all();

         m_psourcevoice = nullptr;


         try
         {


            if(m_pvoice != nullptr)
            {

               m_pvoice->DestroyVoice();

            }
         }
         catch (...)
         {

         }

         m_pvoice = nullptr;

         try
         {


            if(m_pxaudio.is_set())
            {

               m_pxaudio->StopEngine();

            }

         }
         catch (...)
         {

         }

         m_pxaudio.Release();

         //estatus = xaudio::translate(waveOutClose(m_hwaveout));

         //m_hwaveout = nullptr;

         ::wave::out::out_close();

         //return ::success;

      }



      void out::out_filled(::collection::index iBuffer)
      {

         _synchronous_lock synchronouslock(synchronization());

         if(out_get_state() != ::wave::e_out_state_playing)
         {

            informationf("ERROR out::BufferReady while out_get_state() != e_out_state_playing");

            return;

         }

         ::wave::buffer * pwbuffer = out_get_buffer();

         ::wave::buffer::item * pbuffer = pwbuffer->get_buffer(iBuffer);

         ::e_status estatus;

         XAUDIO2_BUFFER b;

         zero(b);

         b.pContext = pbuffer;
         b.AudioBytes = (unsigned int) (pwbuffer->m_uiBufferSize);
         b.pAudioData = (const unsigned char *)pbuffer->m_pData;
         b.Flags = m_bEOS ? XAUDIO2_END_OF_STREAM : 0;

         //single_lock sLock(mutex(),true);

#if 0

         char sz[256];

         zero(sz);

         if(memory_find(b.pAudioData,pwbuffer->m_uiBufferSize,sz,sizeof(sz)))
         {

            output_debug_string("too much zeros in audio buffer\n");

         }

#endif

         if(m_timeLastBuffer.elapsed() > 50_ms)
         {

            output_debug_string("too much delay for submitting audio buffer\n");

         }

         m_timeLastBuffer.Now();

         m_iBufferedCount++;

         HRESULT hresult = m_psourcevoice->SubmitSourceBuffer(&b);

         estatus = ::windows::hresult_status(hresult);

         //VERIFY(::success == estatus);

         if(estatus != ::success)
         {

            m_iBufferedCount--;

         }

      }


      void out::out_stop()
      {

         single_lock sLock(synchronization(), true);

         if (m_eoutstate != ::wave::e_out_state_playing && m_eoutstate != ::wave::e_out_state_paused)
         {

            throw ::exception(error_wrong_state);

         }

         //m_pprebuffer->stop();

         m_eoutstate = ::wave::e_out_state_stopping;

         //// waveOutReset
         //// The waveOutReset function stops playback on the given
         //// waveform-audio_xaudio output device and resets the current position
         //// to zero. All pending playback buffers are marked as done and
         //// returned to the application.
         HRESULT hresult = m_psourcevoice->Stop();

         m_estatusWave = ::windows::hresult_status(hresult);

         if(m_estatusWave == ::success)
         {

            m_eoutstate = ::wave::e_out_state_opened;

         }

         //return m_estatusWave;

      }


      void out::out_pause()
      {

         single_lock sLock(synchronization(), true);

         ASSERT(m_eoutstate == ::wave::e_out_state_playing);

         if (m_eoutstate != ::wave::e_out_state_playing)
         {

            throw ::exception(error_wrong_state);

         }

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_xaudio output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.

          HRESULT hresult = m_psourcevoice->Stop();

          m_estatusWave = ::windows::hresult_status(hresult);

         ASSERT(m_estatusWave == ::success);

         if(m_estatusWave == ::success)
         {
            
            m_eoutstate = ::wave::e_out_state_paused;

         }

         //return m_estatusWave;

      }


      void out::out_start(const class ::time & position)
      {

         _synchronous_lock synchronouslock(synchronization());

         //if(m_eoutstate == e_out_state_playing)
         //   return ::success;

         //ASSERT(m_eoutstate == e_out_state_opened || m_eoutstate == state_stopped);

         //m_bEOS = false;

         //m_eoutstate = e_out_state_playing;

         //m_pprebuffer->start(position);

         //for(::collection::index i = 0; i < out_get_buffer()->GetBufferCount(); i++)
         //{

         //   out_free(i);

         //}

         ::wave::out::out_start(position);

         if(m_estatusWave != ::success)
         {

            //return m_estatusWave;

            return;

         }

         HRESULT hresult = m_psourcevoice->Start(0,XAUDIO2_COMMIT_NOW);

         m_estatusWave = ::windows::hresult_status(hresult);

         //return ::success;

      }


      void     out::out_restart()
      {

         single_lock sLock(synchronization(), true);

         ASSERT(m_eoutstate == ::wave::e_out_state_paused);

         if (m_eoutstate != ::wave::e_out_state_paused)
         {

            throw ::exception(error_wrong_state, "Expected to be paused.");

         }

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_xaudio output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         HRESULT hresult = m_psourcevoice->Start(0,XAUDIO2_COMMIT_NOW);

         m_estatusWave = ::windows::hresult_status(hresult);

         ASSERT(m_estatusWave == ::success);

         if(m_estatusWave == ::success)
         {

            m_eoutstate = ::wave::e_out_state_playing;

         }

         //return m_estatusWave;

      }


      class ::time out::out_get_position()
      {

         _synchronous_lock sLock(synchronization());

//         void                    estatus;

         XAUDIO2_VOICE_STATE s;

         if(m_psourcevoice != nullptr)
         {

            m_psourcevoice->GetState(&s);

            //huge_integer i = s.SamplesPlayed;
            //i *= 1000;
            //i /= m_pwaveformat->m_waveformat.nSamplesPerSec;
            return second_time((double)s.SamplesPlayed/(double)m_pwaveformat->m_waveformat.nSamplesPerSec);

         }
         else
         {
            return 0_s;

         }

      }

      /*::time out::get_position_for_synch()
      {
         ::time position = get_position();
         if(m_pprebuffer != nullptr && m_pprebuffer->m_pdecoder != nullptr)
            return m_pprebuffer->m_position + position - m_pprebuffer->m_pdecoder->audio_plugin_get_lost_position_offset(position) - m_dwLostSampleCount * m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
         else
            return m_pprebuffer->m_position + position - m_dwLostSampleCount * m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
      }*/

//      ::time out::out_get_time()
//      {
//
//         single_lock sLock(mutex(), true);
//
////         void                    estatus;
//
//         XAUDIO2_VOICE_STATE s;
//
//         if (m_psourcevoice != nullptr)
//         {
//
//            m_psourcevoice->GetState(&s);
//
//            return s.SamplesPlayed;
//
//         }
//         else
//         {
//
//            return 0;
//
//         }
//
//      }


      void out::out_free(::collection::index iBuffer)
      {

         ::wave::out::out_free(iBuffer);

      }


      void out::out_on_playback_end()
      {

         out_stop();

         if(m_pprebuffer->m_pstreameffectOut != nullptr)
         {
            ::iaudio::wave_stream_effect * peffect = m_pprebuffer->m_pstreameffectOut;
            m_pprebuffer->m_pstreameffectOut = nullptr;
            delete peffect;
         }

         m_pplayer->post_event(::wave::e_player_event_playback_end);

      }


      ::collection::count out::out_get_buffered_buffer_count()
      {

         return ::wave::out::out_get_buffered_buffer_count();

      }


      WAVEFORMATEX * out::wave_format()
      {

         xaudio::translate(m_waveformatex, m_pwaveformat);

         return &m_waveformatex;

      }


      void * out::get_os_data()
      {
         return m_psourcevoice;
      }


      //void out::out_run_step()
      //{

      //   int iPlay =  -1;


      //}

      void out::out_prebuffer_eof()
      {

         ::wave::out::out_prebuffer_eof();

      }


      void out::OnStreamEnd()
      {

         //out_on_playback_end();

      }


      //
      // Callback handlers, only implement the buffer events for maintaining play state
      //
      void out::OnVoiceProcessingPassStart(unsigned int /*bytesRequired*/)
      {
      }
      void out::OnVoiceProcessingPassEnd()
      {
      }

      void out::OnBufferStart(void* pBufferContext)
      {

         ::wave::buffer::item * pbuffer = (::wave::buffer::item *)pBufferContext;

         //pbuffer->m_bIsPlaying =  true;

      }

      void out::OnBufferEnd(void* pBufferContext)
      {

         //if(get_thread_priority() != ::e_priority_time_critical)
         //{

         //   set_thread_priority(::e_priority_time_critical);

         //}

         huge_natural freq;
         QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
         double timerFrequency = (1.0 / freq);
         huge_natural startTime;
         QueryPerformanceCounter((LARGE_INTEGER *)&startTime);


         ::wave::buffer::item * pbuffer = (::wave::buffer::item *)pBufferContext;

         //pbuffer->m_bIsPlaying = false;

         int iBuffer = (int)pbuffer->m_iIndex;

         m_psynthtask->on_free(iBuffer);

         huge_natural endTime;
         QueryPerformanceCounter((LARGE_INTEGER *)&endTime);
         double timeDifferenceInseconds = ((endTime - startTime) * timerFrequency);
         if(timeDifferenceInseconds > 0.040)
         {
            ::output_debug_string("too much delay to create audio\n");
         }



      }


      void out::OnLoopEnd(void* /*bufferContext*/)
      {
      }
      void out::OnVoiceError(void* /*bufferContext*/, HRESULT /*error*/)
      {
      }
      //out::run_step_thread::run_step_thread(out * pout):
      //   ::thread(pout->get_application())
      //{
      //}


      //int out::run_step_thread::run()
      //{

      //   while(m_bRun && m_pout->m_eoutstate == out::wave::e_out_state_playing)
      //   {
      //      m_pout->out_run_step();
      //   }


      //   return 0;
      //}


   } // namespace audio_xaudio


} // namespace multimedia






