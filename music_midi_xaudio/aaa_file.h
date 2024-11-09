#pragma once


namespace music
{


   namespace midi
   {


      namespace mmsystem
      {


         class CLASS_DECL_MUSIC_MIDI_MMSYSTEM buffer :
            virtual public ::music::midi::file::buffer
         {
         public:



            buffer(::particle * pparticle);
            virtual ~buffer();


            double GetTempoShiftRate();

            bool IsTrackMute(int iIndex);

            void MuteAll(bool bMute = true, int iExcludeTrack = -1);

            void MuteTrack(int iIndex, bool bMute = true);

            void     WorkSeekBegin();

            void     CalcTkLength();

            //void     CreateTempoMap();

            void     ImmediatePutTempoChange();

            void     GetTempoEvent(::music::midi::happening & happening);

            void OnStop();

            unsigned int GetImageSize();

            LPBYTE GetImage();

            void     Format0();

            void     InsertTempoEvent();

            ::music::midi::tempo_map_entry * GetTempoMapEntry(const ::duration & position);

            int GetTempoShift();

            void     SetTempoShift(int iTempoShift);

            void ToWorkStorage();

            void FromWorkStorage();

            int GetKeyShift();

            int SetKeyShift(int iKeyShift);

            bool IsSoftKaraokeFile(bool bWork = false, string_array * pstra = nullptr);

            bool IsXFFile();

            bool IsTune1000File();

            bool IsOpened();

            void     RebuildIndex();

            void     WriteHeader(MIDIFILEHDR * lpmfh);

            void     ChangeEventDelta(::music::midi::happening *pEvent, ::duration tickNewDelta);

            void     ReplaceSameDeltaEvent(::music::midi::happening & pEventNew);

            void     GetPreviousEvent(::music::midi::happening * pPreviousEvent, ::music::midi::happening * pEvent);

            void     SaveFile(const ::string & lpFilePath);

            void     WorkWriteXFTracks(string_array & tokena, imedia_position_array & positiona, ::music::xf::info_header & xfinfoheader);

            //int CalcMelodyTrack(::music::midi::happenings ** ppEvents, imedia_position_array & ticka);

            //int WorkCalcMelodyTrack(::music::midi::happenings ** ppEvents, imedia_position_array & positiona, ::int_array & iaTokenLine);

            int GetFormat();

            void     MergeTracks();

            void     WorkMergeTracks();

            void     WorkStreamRender(LPMIDIHDR lpmh, ::duration tickMax, unsigned int cbPrerollNomimalMax);

            void     WorkGetNextEvent(::music::midi::happening * & pevent, ::duration tickMax, bool bTkMaxInclusive);

            void     WorkGetNextRawEvent(::music::midi::happening * & pevent, ::duration tickMax, bool bTkMaxInclusive);

            void     WorkGetNextRawMidiEvent(::music::midi::happening * & pevent, ::duration tickMax, bool bTkMaxInclusive);

            void     WorkSeek(::duration tickPosition, LPMIDIHDR lpmh);

            void delete_contents();

            int GetDivision();

            ::duration GetPosition();

            void AttachSeekSync(::ikaraoke::events_tracks_v1 * ptracksv1);

            bool IsNull();

            void WorkSeekEnd();

            void     OpenFile(::music::midi::file::buffer &file, int openMode);

            void     OpenFile(::file::file & file, int openMode);

            void     OpenFile(memory * pmemstorage, int openMode, ::music::e_storage);

            void     OpenFile(memory * pmemstorage, ::music::midi::file::e_open eopenmode, ::music::e_storage estorage);

            void     SaveFile(::file::file & file);

            void     CloseFile();

            void     GetFileInfo(PSMFFILEINFO psfi);

            iptr TicksToMillisecs(::duration tickOffset);

            void TicksToMillisecs(duration_array *pMillisArray, imedia_position_array *pTickArray, iptr tickOffset);

            void TicksToMillisecs(duration_2darray *ptimeaaArray, imedia_position_2darray & tickaaArray, iptr tickOffset);

            void PositionToTime(duration_2darray  & timea, imedia_position_2darray  & positiona, iptr tickOffset);

            void PositionToTime(duration_array  & timea, imedia_position_array  & positiona, iptr tickOffset);

            ::duration MillisecsToTicks(::duration msOffset);

            ::duration PositionToTime(::duration msOffset);

            ::duration TimeToPosition(::duration msOffset);

            void MillisecsToTicks(imedia_position_array * pTickArray, duration_array *pMillisArray, ::duration msOffset);

            void TimeToPosition(imedia_position_array & positiona, duration_array & timea, ::duration msOffset);

            void     StreamRender(LPMIDIHDR lpmh, ::duration tickMax, unsigned int cbPrerollNomimalMax);

            void     StreamEvent(::duration tickDelta, ::music::midi::happening * Event, LPMIDIHDR lpmh, ::duration tickMax, unsigned int cbPrerollNomimalMax);

            void     StreamEventF1(::duration tickDelta, array < ::music::midi::happening *, ::music::midi::happening * > & eventptra, LPMIDIHDR lpmh, ::duration tickMax, unsigned int cbPrerollNomimalMax);

            void     seek(::duration tickPosition, LPMIDIHDR lpmh);

            void     seek(::duration tickPosition);

            //void     Build();

            void     GetNextEvent(::music::midi::happening *& pevent, ::duration tickMax, bool   bTkMaxInclusive);

            void     GetNextEventTkPosition(::duration * pTkPosition, ::duration tickMax);

            void _SyncSeek(::duration tickPosition, ::ikaraoke::events_tracks_v1 * ptracksv1);

            void _SyncSeek(::duration tickPosition);

            void     InsertParmData(::duration tickDelta, LPMIDIHDR lpmh);

            void     InsertLyricEvents(::ikaraoke::lyric_event_v1 * pLyricEvent, LPMIDIHDR lpmh);

            void     InsertPadEvent(::duration tickDelta, LPMIDIHDR lpmh);

            void SetOpened(bool bOpened = true);


         };


      } // namespacemmsystem


   } // namespace midi


} // namespace music





