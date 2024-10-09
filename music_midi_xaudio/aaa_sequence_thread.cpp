#include "framework.h"


namespace music
{


   namespace midi
   {


      namespace mmsystem
      {


         sequence_thread::sequence_thread(::particle * pparticle) :
            ::object(pobject),
            thread(pobject),
            ::music::midi::sequence_thread(pobject)
         {
         }

         sequence_thread::~sequence_thread()
         {
         }

         bool sequence_thread::initialize_thread()
         {
            set_thread_priority(::aura::scheduling_priority_highest);
            return true;
         }

         i32 sequence_thread::exit_thread()
         {
            return thread::exit_thread();
         }

         void sequence_thread::install_message_routing(::channel * pchannel)
         {
            MESSAGE_LINK(::music::midi::player::message_command, pchannel, this, &sequence_thread::OnCommand);
            MESSAGE_LINK(e_message_midi_sequence_event, pchannel, this, &sequence_thread::OnMidiSequenceEvent);
         }

         void sequence_thread::Stop(::duration msEllapse)
         {
            get_sequence()->Stop();
            m_eventStop.wait(::duration(msEllapse));
         }

         ::music::midi::sequence * sequence_thread::get_sequence()
         {
            return m_psequence;
         }


         bool sequence_thread::PostMidiSequenceEvent(::music::midi::sequence * pseq, ::music::midi::sequence::e_event eevent)
         {

            return post_thread_message(::music::midi::sequenceEVENT_MESSAGE,  (WPARAM) pseq, (LPARAM) pseq->create_new_event(eevent));

         }


         bool sequence_thread::PostMidiSequenceEvent(::music::midi::sequence * pseq, ::music::midi::sequence::e_event eevent, LPMIDIHDR lpmh)
         {

            ::pointer<sequence>seq = pseq;

            return post_thread_message(::music::midi::sequenceEVENT_MESSAGE,  (WPARAM) pseq, (LPARAM) seq->create_new_event(eevent, lpmh));

         }

         void sequence_thread::OnMidiSequenceEvent(::message::message * pmessage)
         {

            ::pointer<::user::message>pusermessage(pmessage);

            ::music::midi::sequence::event * pevent = (::music::midi::sequence::event *) pusermessage->m_lparam.m_lparam;
            ::music::midi::sequence * pseq = (::music::midi::sequence *) ptopic->m_psequence;

            pseq->OnEvent(pevent);

            switch(ptopic->m_atom)
            {
            case ::music::midi::sequence::EventStopped:
            {
               ::music::midi::sequence::PlayerLink & link = get_sequence()->GetPlayerLink();
               if(link.TestFlag(::music::midi::sequence::e_flag_stop))
               {
                  link.ModifyFlag(::music::midi::sequence::e_flag_null, ::music::midi::sequence::e_flag_stop);
                  link.OnFinishCommand(::music::midi::player::command_stop);
                  PostNotifyEvent(::music::midi::player::notify_event_playback_stop);

               }
               else if(link.TestFlag(::music::midi::sequence::e_flag_tempo_change))
               {
                  PrerollAndWait(link.m_tkRestart);
                  get_sequence()->SetTempoChangeFlag(false);
                  get_sequence()->Start();
                  link.ModifyFlag(::music::midi::sequence::e_flag_null, ::music::midi::sequence::e_flag_tempo_change);
               }
               else if(link.TestFlag(::music::midi::sequence::e_flag_setting_position))
               {
                  link.ModifyFlag(::music::midi::sequence::e_flag_null, ::music::midi::sequence::e_flag_setting_position);
                  try
                  {
                     PrerollAndWait(link.m_tkRestart);
                  }
                  catch(const ::exception & e)
                  {
                     return;
                  }
                  get_sequence()->Start();
                  PostNotifyEvent(::music::midi::player::notify_event_position_set);
               }
               else if(link.TestFlag(
                       ::music::midi::sequence::e_flag_midi_out_device_change))
               {
                  link.ModifyFlag(::music::midi::sequence::e_flag_null, ::music::midi::sequence::e_flag_midi_out_device_change);
                  try
                  {
                     PrerollAndWait(link.m_tkRestart);
                  }
                  catch(const ::exception & e)
                  {
                     return;
                  }
                  get_sequence()->Start();
                  /*CNotifyEventData data;
                  data.m_pplayer = this;
                  data.m_enotifyevent = player::notify_event_position_set;
                  SendMessage(
                  m_oswindow_,
                  MIDIPLAYERMESSAGE_NOTIFYEVENT,
                  (WPARAM) &data,
                  0);      */

               }
               else if(link.TestFlag(::music::midi::sequence::e_flag_midi_out_and_restart))
               {
                  link.ModifyFlag(::music::midi::sequence::e_flag_null, ::music::midi::sequence::e_flag_midi_out_and_restart);
                  try
                  {
                     PrerollAndWait(link.m_tkRestart);
                  }
                  catch(const ::exception & e)
                  {
                     return;
                  }
                  get_sequence()->Start();
                  //PostNotifyEvent(player::notify_event_position_set);
               }

            }
            break;
            case ::music::midi::sequence::EventSpecialModeV001End:
            {
               PostNotifyEvent(::music::midi::player::notify_event_generic_mmsg_done);
            }
            break;
            case ::music::midi::sequence::e_event_midi_playback_start:
            {
               PostNotifyEvent(::music::midi::player::notify_event_playback_start);
            }
            break;
            case ::music::midi::sequence::e_event_midi_playback_out:
            {
               PostNotifyEvent(::music::midi::player::notify_event_midi_stream_out);
            }
            break;
            case ::music::midi::sequence::e_event_midi_playback_end:
            {
               PostNotifyEvent(::music::midi::player::notify_event_playback_end);
            }
            break;

            }

            delete pevent;


         }

         void sequence_thread::PostNotifyEvent(::music::midi::player::e_notify_event eevent)
         {
            if(m_pplayer != nullptr)
            {
               ::pointer<::music::midi::player::notify_event>pdata(__new ::music::midi::player::notify_event());
               pdata->m_enotifyevent = eevent;
               m_pplayer->post_object(::music::midi::player::message_notify_event, 0,pdata);
            }
         }

         void sequence_thread::Play(::duration tickStart)
         {
            ASSERT(get_sequence() != nullptr);
            ASSERT(get_sequence()->GetState() == ::music::midi::sequence::e_state_opened);

            PrerollAndWait(tickStart);
            get_sequence()->Start();
         }

         void sequence_thread::Play(double dRate)
         {
            ASSERT(get_sequence() != nullptr);
            ASSERT(get_sequence()->GetState() == ::music::midi::sequence::e_state_opened);

            PrerollAndWait(dRate);
            get_sequence()->Start();
         }


         void sequence_thread::PrerollAndWait(::duration tickStart)
         {

            ::music::midi::PREROLL                 preroll;

            preroll.tickBase = tickStart;
            preroll.tickEnd  = get_sequence()->m_tkLength;

            get_sequence()->SetMidiOutDevice(m_pplayer->GetMidiOutDevice());

            try
            {
               get_sequence()->Preroll(this, &preroll, true);
            }
            catch (::exception_pointer e)
            {
               string str;
               ASSERT(false);

               /* super merge module      CVmsMusDll::load_string(str, IDS_PREROLLUSERERROR001);
               pme->SetUserText(str);*/
               throw ::exception(pme);
            }

         }


         void sequence_thread::PrerollAndWait(double dRate)
         {
            ::music::midi::PREROLL                 preroll;

            ::math::math::MaxClip(&dRate, 1.0);
            ::math::math::MinClip(&dRate,  0.0);

            preroll.tickBase = (::duration) (i32) ((double) get_sequence()->m_tkLength * dRate);
            preroll.tickEnd  = get_sequence()->m_tkLength;

            get_sequence()->SetMidiOutDevice(m_pplayer->GetMidiOutDevice());

            try
            {
               get_sequence()->Preroll(this, &preroll, true);
            }
            catch (::exception_pointer e)
            {
               throw ::not_implemented();
               /*string str;
               str.load_string(IDS_PREROLLUSERERROR001);
               pme->SetUserText(str);*/
               throw ::exception(pme);
            }

            //    if(!get_sequence()->IsInSpecialModeV001())
            //  {
            //    SendMessage(m_oswindow_, WM_USER, 33, preroll.tickBase);
            //}
         }


         void sequence_thread::PostGMReset()
         {
            ASSERT(!get_sequence()->IsPlaying());
            get_sequence()->set_operation(::music::midi::operation_general_midi_reset);
            PrerollAndWait(0.0);
            get_sequence()->Start();

         }

         void sequence_thread::PostTempoChange()
         {
            ASSERT(!get_sequence()->IsPlaying());
            get_sequence()->set_operation(::music::midi::operation_tempo_change);
            PrerollAndWait(0.0);
            get_sequence()->Start();
         }

         void sequence_thread::SendTempoChange()
         {
            ASSERT(!get_sequence()->IsPlaying());
            get_sequence()->m_evMmsgDone.ResetEvent();
            PostTempoChange();
            get_sequence()->m_evMmsgDone.wait();
         }


         void sequence_thread::ExecuteCommand(::pointer<::music::midi::player::command>spcommand)
         {
            spcommand->increment_reference_count();
            post_thread_message(
            ::music::midi::player::message_command,
            0,
            (LPARAM) (::music::midi::player::command *) spcommand);
         }


         void sequence_thread::OnCommand(::message::message * pmessage)
         {
            ::pointer<::user::message>pusermessage(pmessage);
            ::pointer<::music::midi::player::command>spcommand;
            spcommand = (::music::midi::player::command *) pusermessage->m_lparam.m_lparam;
            try
            {
               _ExecuteCommand(spcommand);
            }
            catch(exception * pe)
            {
               ::exception_pointer esp(pe);
            }
            catch(...)
            {
            }
         }


         void sequence_thread::_ExecuteCommand(::pointer<::music::midi::player::command>spcommand)
         {
            switch(spcommand->GetCommand())
            {
            case ::music::midi::player::command_play:
            {
               if(spcommand->m_flags.has(::music::midi::player::command::flag_dRate))
               {
                  Play(spcommand->m_dRate);
               }
               else if(spcommand->m_flags.has(::music::midi::player::command::flag_ticks))
               {
                  Play(spcommand->m_ticks);
               }
               else
               {
                  Play();
               }
               spcommand->OnFinish();
            }
            break;
            case ::music::midi::player::command_close_device:
            {
               if(get_sequence() != nullptr)
               {
                  get_sequence()->CloseFile();
               }
               spcommand->OnFinish();
            }
            break;
            case ::music::midi::player::command_stop:
            {
               m_eventStop.ResetEvent();
               void                estatus;
               ::music::midi::sequence::PlayerLink & link = get_sequence()->GetPlayerLink();
               link.SetCommand(spcommand);
               link.ModifyFlag(::music::midi::sequence::e_flag_stop, ::music::midi::sequence::e_flag_null);
               if(MMSYSERR_NOERROR != (estatus = get_sequence()->Stop()))
               {
                  throw ::exception(multimedia::exception(multimedia::exception_midi, EMidiPlayerStop));
               }
            }
            break;
            case ::music::midi::player::command_stop_and_restart:
            {
               void                estatus;
               ::music::midi::sequence::PlayerLink & link = get_sequence()->GetPlayerLink();
               link.SetCommand(spcommand);
               link.ModifyFlag(
               ::music::midi::sequence::e_flag_midi_out_and_restart,
               ::music::midi::sequence::e_flag_null);
               link.m_tkRestart = get_sequence()->GetPositionTicks();
               if(::success != (estatus = get_sequence()->Stop()))
               {
                  throw ::exception(multimedia::exception(multimedia::exception_midi, EMidiPlayerStop));
               }
            }
            break;
            }
         }


      } // namespace mmsystem


   } // namespace midi


} // namespace music









