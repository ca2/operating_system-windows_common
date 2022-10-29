#pragma once


#include "music_midi_mmsystem_player_window.h"
#include "music_midi_mmsystem_player_callback.h"
#include "music_midi_mmsystem_player_interface.h"





namespace music
{


   namespace midi
   {


      namespace mmsystem
      {


         class sequence_thread;


         namespace player
         {

            class player_callback;
            class player_interface;




            class CLASS_DECL_MUSIC_MIDI_MMSYSTEM player :
               virtual public ::music::midi::player::player
            {
            public:


               player(::particle * pparticle);
               virtual ~player();


               void install_message_routing(::channel * pchannel);


               void SendMmsgDone(::music::midi::sequence *pSeq, ::music::midi::LPMIDIDONEDATA lpmdd);
               void SetCallbackWindow(::pointer<::user::interaction>puie);
               bool Play(double dRate = 0.0, u32 dwEllapse = 584);
               bool Play(::duration tkStart, u32 dwEllapse = 584);

               virtual bool initialize_thread() override;
               virtual i32 exit_instance();
               virtual void pre_translate_message(::message::message * pmessage);
               void OnMmsgDone(::music::midi::sequence *pSeq);
               DECLARE_MESSAGE_HANDLER(OnUserMessage)
               void SaveFile(const ::string & lpszPathName);
               void SetPosition(double dRate);
               void Pause();
               void CloseFile();
               void SendReset();
               bool ExecuteCommand(::music::midi::player::enum_command ecommand, u32 dwEllapse);
               virtual void OnMidiOutDeviceChange();

               u32 GetMidiOutDevice();
               void PostNotifyEvent(::music::midi::player::e_notify_event eevent);

               ::duration RateToTicks(double dRate);

               bool SetTempoShift(i32 iTempoShift);

               bool SetMidiOutDevice(u32 uiDevice);

               void     SetInterface(player_interface * pinterface);

               void     Initialize(::thread * pthread);

               bool IsPlaying();

               void PostGMReset();
               void PostTempoChange();
               void SendTempoChange(); // verificar


               DECLARE_MESSAGE_HANDLER(OnNotifyEvent)
               DECLARE_MESSAGE_HANDLER(OnMultimediaMidiOutputMessageDone)
               DECLARE_MESSAGE_HANDLER(OnMultimediaMidiOutputMessagePositionCB)

               // midi central listener
               DECL_GEN_VSIGNAL(on_attribute_change);


            };


         } // namespace player


      } // namespace mmsystem


   } // namespace midi_mmsystem


} // namespace music






