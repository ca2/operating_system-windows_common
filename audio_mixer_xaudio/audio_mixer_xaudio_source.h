#pragma once


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      class CLASS_DECL_AUDIO_MIXER_MMSYSTEM source :
         virtual public ::audio_mixer::source
      {
      public:


         MIXERLINE                     m_mixerline;
         MIXERLINECONTROLS             m_mixerlinecontrols;


         source(::pointer<base_application>papp);
         source(source & source);
         virtual ~source();


         void     mixerGetLineInfo(u32 dwSource, u32 dwDestination, u32 fdwInfo);
         void     mixerGetLineInfo(u32 dwSource, ::audio_mixer::destination * pdestination);

         u32 GetLineID();
         virtual string get_name();
         void update_all_controls();
         MIXERLINECONTROLS & get_mixer_line_controls();
         MIXERLINE & get_mixer_line();
         ::audio_mixer::control_array & get_control_array();

         ::audio_mixer::device * get_device();

         void SetDestination(::audio_mixer::destination * pdestination);
         ::audio_mixer::destination * get_destination();

         void OnMixerLineChange();
         //void OnArrayReallocation(void *pNewPointer);
         //void     GetControl(u32 dwControlType, u32 dwControlFlags, ::audio_mixer::control ** ppControl);
         void     GetLineControls();

         inline bool HasV001Controls();

         source & operator = (const source & source);


      };


   } // namespace audio_mixer_mmsystem


} // namespace multimedia







