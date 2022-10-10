#pragma once


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      class CLASS_DECL_AUDIO_MIXER_MMSYSTEM department :
         virtual public base_department,
         virtual public ::user::interaction
      {
      public:



         enum EIds
         {

            IdsMuteAll,
            IdsMute

         };


         department(::pointer<base_application>papp);
         virtual ~department();


         virtual bool init1();



         bool initialize();


         bool destroy();





      };


   } // namespace mixguserbase


} // namespace multimedia






