#pragma once


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      class CLASS_DECL_AUDIO_MIXER_MMSYSTEM device :
         virtual public ::audio_mixer::device
      {
      public:


         HMIXER                        m_hMixer;
         MIXERCAPS                     m_mixercaps;


         device(::pointer<base_application>papp);
         virtual ~device();

         virtual bool initialize(::audio_mixer::audio_mixer * pmixer);

         ::audio_mixer::audio_mixer * get_mixer();
         virtual string get_product_name();
         bool OnCommand(wparam wparam, lparam lparam);
         void     close();
         void MapDlgCtrlIDToLineControls(::audio_mixer::source * pSource);
         void MapDlgCtrlIDToControls();
         void MapLineControls(::audio_mixer::source * pSource);
         void OnMixerControlChange(unsigned int dwControlID);
         void OnMixerLineChange(unsigned int dwLineID);
         void map_lines();
         void map_controls();
         void     get_destination(::audio_mixer::e_destination edestination, ::audio_mixer::destination ** ppDestination);
         void     initialize_destinations();
         void     initialize_capabilities();
         void     open(unsigned int uiMixerId, unsigned int dwCallback, unsigned int dwInstance, unsigned int fdwOpen);


      };


   } // namespace audio_mixer_mmsystem


} // namespace multimedia





