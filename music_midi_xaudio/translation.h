#pragma once


#define MIDIMSG(status,channel,data1,data2) ( (unsigned int)((status<<4) | channel | (data1<<8) | (data2<<16)) )


namespace music
{


   namespace midi
   {


      namespace mmsystem
      {


         //CLASS_DECL_MUSIC_MIDI_XAUDIO void     MMRESULT mmr;


      } // namespace mmsystem


   } // namespace midi


} // namespace music
