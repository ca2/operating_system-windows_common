#pragma once


#define  VERSION_MINOR              0x00
#define  VERSION_MAJOR              0x04
#define  SEQ_VERSION                ((unsigned int)(unsigned short)((unsigned char)VERSION_MINOR | (((unsigned short)(unsigned char)VERSION_MAJOR) << 8)))

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
               void Initialize(int iSize, uptr dwUser);

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
               void Initialize(int iCount, int iSize, uptr dwUser);


     /*          void     midiStreamOut(HMIDISTRM hmidiout);
               void     midiOutPrepareHeader(HMIDIOUT hmidiout);
               void     midiOutUnprepareHeader(HMIDIOUT hmidiout);*/

            };

            class happening :
               public ::music::midi::sequence::happening
            {
            public:


               //LPMIDIHDR                     m_lpmh;


            };


            buffer_array                  m_buffera;

            HMIDISTRM                     m_hstream;

            midi_callback_data            m_midicallbackdata;




            sequence(::particle * pparticle);
            virtual ~sequence();


            inline unsigned int GetState();
            inline unsigned int GetPreviousState();
            unsigned int SetState(unsigned int uiState);

            void SetSpecialModeV001Flag(bool bSet = true);

            void MuteAll(bool bMute = true, int iExcludeTrack = -1);
            void MuteTrack(int iIndex, bool bMute = true);


            virtual int GetDefaultCodePage();

            void Prepare(::ikaraoke::data & data);
            void Prepare(int iTrack, ::ikaraoke::data & data);
            void Prepare(
            string_array_array & straa,
            imedia_position_2darray & tickaaTokensTicks,
            int iMelodyTrack,
            int2a & ia2TokenLine,
            ::ikaraoke::data & data);

            ::duration GetPositionTicks();
            void SetLevelMeter(int iLevel);
            void     CloseStream();
            bool SetMidiOutDevice(unsigned int uiDevice);
            int SetKeyShift(int iKeyShift);
            int GetKeyShift();

            void on_midi_playback_end(::music::midi::sequence::happening * pevent);
            int SetTempoShift(int iTempoShift);

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

            virtual int GetTempoShift();
            virtual void GetMidiDoneData(::music::midi::LPMIDIDONEDATA lpmdd);
            virtual bool IsInSpecialModeV001();
            virtual bool WasInSpecialModeV001();
            virtual void set_operation(e_operation eoperation);

            virtual void OnHappening(::music::midi::sequence::happening * pevent);


            void     AllocBuffers();

            VOID FreeBuffers();

            //void     OpenFile(const ::string & lpFileName, int openMode);
            void     OpenFile(::music::midi::sequence & sequence, int iOpenMode);
            void     OpenFile(::file::file & ar, int openMode);
            void     OpenFile(const ::string & lpFileName, int openMode);
            void     OpenFile(memory * pmemorystorage, int openMode, ::music::e_storage estorage);

            void     CloseFile();
            void     SaveFile(const ::string & lpFileName);
            void     SaveFile();
            void     SaveFile(file_pointer &ar);

            void     Preroll(::thread * pthread, ::music::midi::LPPREROLL lpPreroll, bool bThrow);
            void     Start();

            void     Pause();

            void     Restart();

            //void     Stop(unsigned int dwEllapse);
            void     Stop();

            void get_position(::duration  & time);
            void get_time(::duration  & time);

            void     get_ticks(::duration & time);
            void     get_millis(::duration & time);


            ::duration MillisecsToTicks(::duration msOffset);

            ::duration TicksToMillisecs(::duration tickOffset);

            bool IsPlaying();

            static void CALLBACK MidiOutProc(HMIDIOUT hmo, unsigned int wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);


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
            virtual ::music::midi::sequence::happening * create_new_event(::music::midi::sequence::e_happening ehappening, LPMIDIHDR lpmidihdr);

         };


      } // namespace mmsystem


   } // namespace midi


} // namespace music







