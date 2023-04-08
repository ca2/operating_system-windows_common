#pragma once


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      class CLASS_DECL_AUDIO_MIXER_MMSYSTEM line_info :
         virtual public ::audio_mixer::line_info
      {
      public:


         MIXERLINE       m_mixerLine;


         line_info(::pointer<base_application>papp);
         virtual ~line_info();


      };


   } // namespace audio_mixer_mmsystem


} // namespace multimedia









