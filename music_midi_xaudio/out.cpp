#include "framework.h"


namespace music
{


   namespace midi
   {


      namespace xaudio
      {


         out::out()
         {

            //m_hmidiout = nullptr;

         }


         out::~out()
         {

            close();

         }


         ::e_status     out::open()
         {

            //MMRESULT estatus;

            //u32 uDeviceID = 0;

            //m_hmidiout = nullptr;

            //estatus = midiOutOpen(&m_hmidiout, uDeviceID, 0, 0, CALLBACK_NULL);

            //if (estatus != MMSYSERR_NOERROR)
            //   return error_failed;


            return ::success;

         }

         ::e_status     out::close()
         {

      /*      if (m_hmidiout != nullptr)
            {

               midiOutClose(m_hmidiout);

            }*/

            return ::success;

         }

         ::e_status     out::send_short_message(::music::midi::e_message emessage, int iChannel, int iData1, int iData2)
         {

            // return midiOutShortMsg(m_hmidiout, MIDIMSG(((int)etype) >> 4, iChannel, iData1, iData2));
            return error_not_implemented;

         }


      } // namespace xaudio


   } // namespace midi


} // namespace music




