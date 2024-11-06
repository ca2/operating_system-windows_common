#pragma once


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      class CLASS_DECL_AUDIO_MIXER_MMSYSTEM destination :
         virtual public ::audio_mixer_mmsystem::source,
         virtual public ::audio_mixer::destination
      {
      public:


         destination(::pointer<base_application>papp);
         destination(const destination & destination);
         virtual ~destination();


         unsigned int get_component_type();
         void update_all_controls();
         void     initialize_all_controls();
         void     initialize_source_info();

         ::audio_mixer::source_array & get_source_info();
         void operator delete(void *);

         destination & operator = (const destination & device);

         ::audio_mixer::device * get_device();
         void set_device(::audio_mixer::device * pdevice);


         virtual unsigned int get_mixer_line_id();

      };


   } // namespace audio_mixer_mmsystem



} // namespace multimedia




