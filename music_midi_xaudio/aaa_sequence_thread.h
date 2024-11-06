#pragma once



namespace music
{


   namespace midi
   {


      namespace mmsystem
      {


         class CLASS_DECL_MUSIC_MIDI_MMSYSTEM sequence_thread :
            virtual public ::music::midi::sequence_thread
         {
         public:


            sequence_thread(::particle * pparticle);
            virtual ~sequence_thread();


            void install_message_routing(::channel * pchannel);

            ::music::midi::sequence * get_sequence();
            void Stop(::duration msEllapse);

            bool PostMidiSequenceEvent(::music::midi::sequence * pseq, ::music::midi::sequence::e_event event);

            bool PostMidiSequenceEvent(::music::midi::sequence * pseq, ::music::midi::sequence::e_event event, LPMIDIHDR lpmh);

            void PostNotifyEvent(::music::midi::player::e_notify_event eevent);

            void PrerollAndWait(double rate = 0.0);
            void PrerollAndWait(::duration tickStart);
            void Play(double dRate = 0.0);
            void Play(::duration tickStart);
            void PostGMReset();
            void PostTempoChange();
            void SendTempoChange();

            void ExecuteCommand(::pointer<::music::midi::player::command>pcommand);
            void _ExecuteCommand(::pointer<::music::midi::player::command>pcommand);

            virtual bool initialize_thread() override;
            virtual int exit_instance();

            DECLARE_MESSAGE_HANDLER(OnCommand)
            DECLARE_MESSAGE_HANDLER(OnMidiSequenceEvent)

         };


      } // namespace mmsystem


   } // namespace midi_mmsystem


} // namespace music





