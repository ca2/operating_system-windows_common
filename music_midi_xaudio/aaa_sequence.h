#pragma once


#define  VERSION_MINOR              0x00
#define  VERSION_MAJOR              0x04
#define  SEQ_VERSION                ((u32)(::u16)((::u8)VERSION_MINOR | (((::u16)(::u8)VERSION_MAJOR) << 8)))

#define MMSG_DONE                   (WM_USER+20)


namespace music
{


   namespace midi
   {


      namespace xaudio
      {


         class buffer;
         class sequence;
         class sequence_thread;


         class CLASS_DECL_MUSIC_MIDI_MMSYSTEM midi_callback_data
         {
         public:


            sequence *          m_psequence;


         };


         class CLASS_DECL_MUSIC_MIDI_MMSYSTEM sequence :
            virtual public ::music::midi::sequence
         {
         public:

            class buffer_array;


            class buffer
            {
            public:


               //MIDIHDR              m_midihdr;
               memory    m_storage;
               bool                 m_bPrepared;

               bool IsPrepared();
               void Reset();
               void Initialize(i32 iSize, uptr dwUser);

               //void     midiStreamOut(HMIDISTRM hmidiout);
               //void     midiOutPrepareHeader(HMIDIOUT hmidiout);
               //void     midiOutUnprepareHeader(HMIDIOUT hmidiout);


               //inline LPMIDIHDR GetMidiHdr() { return &m_midihdr; }

               //void SetNextMidiHdr(LPMIDIHDR lpNext);


            };


            class buffer_array :
               public array < buffer, buffer >
            {
            public:


               void Reset();
               void Initialize(i32 iCount, i32 iSize, uptr dwUser);


     /*          void     midiStreamOut(HMIDISTRM hmidiout);
               void     midiOutPrepareHeader(HMIDIOUT hmidiout);
               void     midiOutUnprepareHeader(HMIDIOUT hmidiout);*/

            };

            class event :
               public ::music::midi::sequence::event
            {
            public:


               //LPMIDIHDR                     m_lpmh;


            };


            buffer_array                  m_buffera;

            HMIDISTRM                     m_hstream;

            midi_callback_data            m_midicallbackdata;




            sequence(::particle * pparticle);
            virtual ~sequence();


            inline u32 GetState();
            inline u32 GetPreviousState();
            u32 SetState(u32 uiState);

            void SetSpecialModeV001Flag(bool bSet = true);

            void MuteAll(bool bMute = true, i32 iExcludeTrack = -1);
            void MuteTrack(i32 iIndex, bool bMute = true);


            virtual i32 GetDefaultCodePage();

            void Prepare(::ikaraoke::data & data);
            void Prepare(i32 iTrack, ::ikaraoke::data & data);
            void Prepare(
            string_array_array & straa,
            imedia_position_2darray & tickaaTokensTicks,
            i32 iMelodyTrack,
            int2a & ia2TokenLine,
            ::ikaraoke::data & data);

            ::duration GetPositionTicks();
            void SetLevelMeter(i32 iLevel);
            void     CloseStream();
            bool SetMidiOutDevice(u32 uiDevice);
            i32 SetKeyShift(i32 iKeyShift);
            i32 GetKeyShift();

            void on_midi_playback_end(::music::midi::sequence::event * pevent);
            i32 SetTempoShift(i32 iTempoShift);

            void OnPositionCB(LPMIDIHDR lpmidihdr);
            void OnDone(HMIDISTRM hmidistream, LPMIDIHDR lpmidihdr);
            virtual void GetTimeLength(::duration & time);
            virtual void GetPositionLength(::duration & position);

            using ::ikaraoke::karaoke::TimeToPosition;
            using ::ikaraoke::karaoke::PositionToTime;

            virtual ::duration TimeToPosition(::duration time);
            virtual ::duration PositionToTime(::duration position);

            virtual bool IsOpened();

            virtual bool IsNull();
            virtual void SetTempoChangeFlag(bool bSet = true);
            virtual bool IsChangingTempo();

            virtual i32 GetTempoShift();
            virtual void GetMidiDoneData(::music::midi::LPMIDIDONEDATA lpmdd);
            virtual bool IsInSpecialModeV001();
            virtual bool WasInSpecialModeV001();
            virtual void set_operation(e_operation eoperation);

            virtual void OnEvent(::music::midi::sequence::event * pevent);


            void     AllocBuffers();

            VOID FreeBuffers();

            //void     OpenFile(const ::string & lpFileName, i32 openMode);
            void     OpenFile(::music::midi::sequence & sequence, i32 iOpenMode);
            void     OpenFile(::file::file & ar, i32 openMode);
            void     OpenFile(const ::string & lpFileName, i32 openMode);
            void     OpenFile(memory * pmemorystorage, i32 openMode, ::music::e_storage estorage);

            void     CloseFile();
            void     SaveFile(const ::string & lpFileName);
            void     SaveFile();
            void     SaveFile(file_pointer &ar);

            void     Preroll(::thread * pthread, ::music::midi::LPPREROLL lpPreroll, bool bThrow);
            void     Start();

            void     Pause();

            void     Restart();

            //void     Stop(u32 dwEllapse);
            void     Stop();

            void get_position(::duration  & time);
            void get_time(::duration  & time);

            void     get_ticks(::duration & time);
            void     get_millis(::duration & time);


            ::duration MillisecsToTicks(::duration msOffset);

            ::duration TicksToMillisecs(::duration tickOffset);

            bool IsPlaying();

            static void CALLBACK MidiOutProc(HMIDIOUT hmo, ::u32 wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);


            bool IsSettingPosition();
            void SetSettingPositionFlag(bool bSet = true);

            ::duration GetQuarterNote();


            inline ::pointer<::music::midi::mmsystem::buffer>file()
            {
               return get_file();
            }

            inline ::pointer<::music::midi::mmsystem::sequence_thread>thread()
            {
               return m_pthread;
            }

            using ::music::midi::sequence::create_new_event;
            virtual ::music::midi::sequence::event * create_new_event(::music::midi::sequence::e_event eevent, LPMIDIHDR lpmidihdr);

         };


      } // namespace mmsystem


   } // namespace midi


} // namespace music







