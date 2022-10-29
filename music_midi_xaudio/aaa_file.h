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

            bool IsTrackMute(i32 iIndex);

            void MuteAll(bool bMute = true, i32 iExcludeTrack = -1);

            void MuteTrack(i32 iIndex, bool bMute = true);

            void     WorkSeekBegin();

            void     CalcTkLength();

            //void     CreateTempoMap();

            void     ImmediatePutTempoChange();

            void     GetTempoEvent(::music::midi::event & event);

            void OnStop();

            u32 GetImageSize();

            LPBYTE GetImage();

            void     Format0();

            void     InsertTempoEvent();

            ::music::midi::tempo_map_entry * GetTempoMapEntry(const ::duration & position);

            i32 GetTempoShift();

            void     SetTempoShift(i32 iTempoShift);

            void ToWorkStorage();

            void FromWorkStorage();

            i32 GetKeyShift();

            i32 SetKeyShift(i32 iKeyShift);

            bool IsSoftKaraokeFile(bool bWork = false, string_array * pstra = nullptr);

            bool IsXFFile();

            bool IsTune1000File();

            bool IsOpened();

            void     RebuildIndex();

            void     WriteHeader(MIDIFILEHDR * lpmfh);

            void     ChangeEventDelta(::music::midi::event *pEvent, ::duration tkNewDelta);

            void     ReplaceSameDeltaEvent(::music::midi::event & pEventNew);

            void     GetPreviousEvent(::music::midi::event * pPreviousEvent, ::music::midi::event * pEvent);

            void     SaveFile(const ::string & lpFilePath);

            void     WorkWriteXFTracks(string_array & tokena, imedia_position_array & positiona, ::music::xf::info_header & xfinfoheader);

            //i32 CalcMelodyTrack(::music::midi::events ** ppEvents, imedia_position_array * pTicks);

            //i32 WorkCalcMelodyTrack(::music::midi::events ** ppEvents, imedia_position_array & positiona, int_array & iaTokenLine);

            i32 GetFormat();

            void     MergeTracks();

            void     WorkMergeTracks();

            void     WorkStreamRender(LPMIDIHDR lpmh, ::duration tkMax, u32 cbPrerollNomimalMax);

            void     WorkGetNextEvent(::music::midi::event * & pevent, ::duration tkMax, bool bTkMaxInclusive);

            void     WorkGetNextRawEvent(::music::midi::event * & pevent, ::duration tkMax, bool bTkMaxInclusive);

            void     WorkGetNextRawMidiEvent(::music::midi::event * & pevent, ::duration tkMax, bool bTkMaxInclusive);

            void     WorkSeek(::duration tkPosition, LPMIDIHDR lpmh);

            void delete_contents();

            i32 GetDivision();

            ::duration GetPosition();

            void AttachSeekSync(::ikaraoke::events_tracks_v1 * ptracksv1);

            bool IsNull();

            void WorkSeekEnd();

            void     OpenFile(::music::midi::file::buffer &file, i32 openMode);

            void     OpenFile(::file::file & file, i32 openMode);

            void     OpenFile(memory * pmemstorage, i32 openMode, ::music::e_storage);

            void     OpenFile(memory * pmemstorage, ::music::midi::file::e_open eopenmode, ::music::e_storage estorage);

            void     SaveFile(::file::file & file);

            void     CloseFile();

            void     GetFileInfo(PSMFFILEINFO psfi);

            iptr TicksToMillisecs(::duration tkOffset);

            void TicksToMillisecs(duration_array *pMillisArray, imedia_position_array *pTickArray, iptr tkOffset);

            void TicksToMillisecs(duration_2darray *p2DMillisArray, imedia_position_2darray * p2DTicksArray, iptr tkOffset);

            void PositionToTime(duration_2darray  & timea, imedia_position_2darray  & positiona, iptr tkOffset);

            void PositionToTime(duration_array  & timea, imedia_position_array  & positiona, iptr tkOffset);

            ::duration MillisecsToTicks(::duration msOffset);

            ::duration PositionToTime(::duration msOffset);

            ::duration TimeToPosition(::duration msOffset);

            void MillisecsToTicks(imedia_position_array * pTickArray, duration_array *pMillisArray, ::duration msOffset);

            void TimeToPosition(imedia_position_array & positiona, duration_array & timea, ::duration msOffset);

            void     StreamRender(LPMIDIHDR lpmh, ::duration tkMax, u32 cbPrerollNomimalMax);

            void     StreamEvent(::duration tkDelta, ::music::midi::event * Event, LPMIDIHDR lpmh, ::duration tkMax, u32 cbPrerollNomimalMax);

            void     StreamEventF1(::duration tkDelta, array < ::music::midi::event *, ::music::midi::event * > & eventptra, LPMIDIHDR lpmh, ::duration tkMax, u32 cbPrerollNomimalMax);

            void     seek(::duration tkPosition, LPMIDIHDR lpmh);

            void     seek(::duration tkPosition);

            //void     Build();

            void     GetNextEvent(::music::midi::event *& pevent, ::duration tkMax, bool   bTkMaxInclusive);

            void     GetNextEventTkPosition(::duration * pTkPosition, ::duration tkMax);

            void _SyncSeek(::duration tkPosition, ::ikaraoke::events_tracks_v1 * ptracksv1);

            void _SyncSeek(::duration tkPosition);

            void     InsertParmData(::duration tkDelta, LPMIDIHDR lpmh);

            void     InsertLyricEvents(::ikaraoke::lyric_event_v1 * pLyricEvent, LPMIDIHDR lpmh);

            void     InsertPadEvent(::duration tkDelta, LPMIDIHDR lpmh);

            void SetOpened(bool bOpened = true);


         };


      } // namespacemmsystem


   } // namespace midi


} // namespace music





