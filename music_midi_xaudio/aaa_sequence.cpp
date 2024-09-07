#include "framework.h"


namespace music
{


   namespace midi
   {


      namespace mmsystem
      {


         sequence::sequence(::particle * pparticle) :
            ::object(pobject),
            ::ikaraoke::karaoke(pobject),
            ::music::midi::sequence(pobject)
         {

            m_hstream = nullptr;

            m_buffera.Initialize(16, m_cbPreroll, (uptr) (void *) &m_midicallbackdata);
            m_midicallbackdata.m_psequence = this;


         };

         sequence::~sequence()
         {

            CloseFile();

         }


         /***************************************************************************
         *
         * seqAllocBuffers
         *
         * allocate buffers for this instance.
         *
         * pSeq                      - The sequencer instance to allocate buffers for.
         *
         * Returns
         *   success If the operation was successful.
         *
         *   MCIERR_OUT_OF_MEMORY  If there is insufficient memory for
         *     the requested number and size_i32 of buffers.
         *
         * seqAllocBuffers allocates playback buffers based on the
         * cbBuffer and cBuffer fields of pSeq. cbBuffer specifies the
         * number of bytes in each buffer, and cBuffer specifies the
         * number of buffers to allocate.
         *
         * seqAllocBuffers must be called before any other sequencer call
         * on a newly allocted SEQUENCE structure. It must be paired with
         * a call to seqFreeBuffers, which should be the last call made
         * before the SEQUENCE structure is discarded.
         *
         ***************************************************************************/
         void     sequence::AllocBuffers()
         {
            ASSERT(false);
            /*
            u32                   dwEachBufferSize;
            u32                   dwAlloc;
            u32                    i;
            LPBYTE                  lpbWork;

            //    assert(pSeq != nullptr);

            SetState(e_state_no_file);
            m_lpmhFree  = nullptr;
            m_lpbAlloc  = nullptr;
            //    m_hSmf      = (HSMF)nullptr;

            // First make sure we can allocate the buffers they asked for
            //
            dwEachBufferSize = sizeof(MIDIHDR) + (u32)(m_cbBuffer);
            dwAlloc          = dwEachBufferSize * (u32)(m_cBuffer);

            //m_lpbAlloc = (uchar *) GlobalAllocPtr(GMEM_MOVEABLE|GMEM_SHARE, dwAlloc);
            m_lpbAlloc = (uchar *) HeapAlloc(GetProcessHeap(), 0, dwAlloc);
            if (nullptr == m_lpbAlloc)
            return MCIERR_OUT_OF_MEMORY;

            // Initialize all MIDIHDR's and throw ::exception( them into a free list
            //
            m_lpmhFree = nullptr;

            lpbWork = m_lpbAlloc;
            for (i=0; i < m_cBuffer; i++)
            {
            ((LPMIDIHDR)lpbWork)->lpNext            = m_lpmhFree;

            ((LPMIDIHDR)lpbWork)->lpData            = (char *) (lpbWork + sizeof(MIDIHDR));
            ((LPMIDIHDR)lpbWork)->dwBufferLength    = m_cbBuffer;
            ((LPMIDIHDR)lpbWork)->dwBytesRecorded   = 0;
            ((LPMIDIHDR)lpbWork)->dwUser            = (u32)(u32)this;
            ((LPMIDIHDR)lpbWork)->dwFlags           = 0;

            m_lpmhFree = (LPMIDIHDR)lpbWork;

            lpbWork += dwEachBufferSize;
            }

            return ::success;*/

            return error_failed;

         }

         /***************************************************************************
         *
         * seqFreeBuffers
         *
         * Free buffers for this instance.
         *
         * pSeq                      - The sequencer instance to free buffers for.
         *
         * seqFreeBuffers frees all allocated memory belonging to the
         * given sequencer instance pSeq. It must be the last call
         * performed on the instance before it is destroyed.
         *
         ****************************************************************************/
         VOID sequence::FreeBuffers()
         {
            ASSERT(false);
         }

         /***************************************************************************
         *
         * seqOpenFile
         *
         * Associates a MIDI file with the given sequencer instance.
         *
         * pSeq                      - The sequencer instance.
         *
         * Returns
         *   success If the operation is successful.
         *
         *   error_unsupported_function If there is already a file open
         *     on this instance.
         *
         *   MCIERR_OUT_OF_MEMORY If there was insufficient memory to
         *     allocate internal buffers on the file.
         *
         *   MCIERR_INVALID_FILE If initial attempts to parse the file
         *     failed (such as the file is not a MIDI or RMI file).
         *
         * seqOpenFile may only be called if there is no currently open file
         * on the instance. It must be paired with a call to seqCloseFile
         * when operations on this file are complete.
         *
         * The pstrFile field of pSeq contains the name of the file
         * to open. This name will be passed directly to mmioOpen; it may
         * contain a specifcation for a custom MMIO file handler. The task
         * context used for all I/O will be the task which calls seqOpenFile.
         *
         ***************************************************************************/

         void     sequence::OpenFile(::music::midi::sequence & sequence, i32 openMode)
         {

            void                    rc = ::success;
            SMFFILEINFO             sfi;
            void                    smfrc;
            u32                cbBuffer;

            if (GetState() != e_state_no_file)
            {

               return error_unsupported_function;

            }

            m_iOpenMode = openMode;

            smfrc = file()->OpenFile(*sequence.get_file(), openMode);


            if(::success != smfrc)
            {

               goto Seq_Open_File_Cleanup;

            }

            file()->GetFileInfo(&sfi);

            m_MusicTempoTimeDivision = sfi.dwTimeDivision;
            m_tkLength       = sfi.tickLength;
            if(m_iOpenMode == file::open_for_playing)
            {
               m_msLength      = TicksToMillisecs(m_tkLength);
            }
            /* Track buffers must be big enough to hold the state data returned
            ** by smfSeek()
            */
            cbBuffer = minimum(m_cbBuffer, ::music::midi::GetStateMaxSize());


Seq_Open_File_Cleanup:
            if (::success != rc)
               CloseFile();
            else
               SetState(e_state_opened);

            return rc;
         }


         void     sequence::OpenFile(const ::string & lpFileName, i32 openMode)
         {

            file_pointer file;

            try
            {

               file = file().get_file(lpFileName, ::file::e_open_read | ::file::e_open_share_deny_write | ::file::e_open_binary);

            }
            catch(...)
            {

            }

            return OpenFile(*file, openMode);

         }


         void     sequence::OpenFile(memory * pmemorystorage, i32 openMode, e_storage estorage)
         {

            SMFFILEINFO                sfi;
            void                       smfrc;
            u32                   cbBuffer;

            if (GetState() != e_state_no_file)
            {

               CloseFile();

            }

            m_iOpenMode = openMode;

            smfrc = file()->OpenFile(pmemorystorage, openMode, estorage);

            if (::success != smfrc)
            {

            }
            else
            {
               file()->GetFileInfo(&sfi);

               m_MusicTempoTimeDivision = sfi.dwTimeDivision;
               m_tkLength       = sfi.tickLength;
               if(m_iOpenMode == file::open_for_playing)
               {
                  m_msLength      = TicksToMillisecs(m_tkLength);
               }
               /* Track buffers must be big enough to hold the state data returned
               ** by smfSeek()
               */
               cbBuffer = minimum(m_cbBuffer, ::music::midi::GetStateMaxSize());
            }

            if(::success != smfrc)
               CloseFile();
            else
               SetState(e_state_opened);

            return smfrc;

         }

         void     sequence::OpenFile(::file::file & ar, i32 openMode)
         {

            SMFFILEINFO                sfi;
            void                       smfrc = ::success;
            u32                   cbBuffer;

            if (GetState() != e_state_no_file)
            {

               return error_unsupported_function;

            }



            //   m_pstrFile = _tcsdup(lpFileName);
            //   m_strFile = lpFileName;

            //    ASSERT(m_pstrFile != nullptr);

            m_iOpenMode = openMode;

            //    sofs.pstrName     = m_pstrFile;

            //PSMF pSmf = new SMF();

            //smfrc = file()->OpenFile(&sofs);
            smfrc = file()->OpenFile(ar, openMode);

            //smfrc = smfOpenFile(&sofs);
            if (::success != smfrc)
            {
            }
            else
            {
               //    m_hSmf = sofs.hSmf;
               //    ((PSMF) m_hSmf)->GetFileInfo(&sfi);
               file()->GetFileInfo(&sfi);

               m_MusicTempoTimeDivision = sfi.dwTimeDivision;
               m_tkLength       = sfi.tickLength;
               if(m_iOpenMode == file::open_for_playing)
               {
                  m_msLength      = TicksToMillisecs(m_tkLength);
               }
               /* Track buffers must be big enough to hold the state data returned
               ** by smfSeek()
               */
               cbBuffer = minimum(m_cbBuffer, ::music::midi::GetStateMaxSize());
            }

            if(::success != smfrc)
               CloseFile();
            else
               SetState(e_state_opened);

            return smfrc;

         }


         /***************************************************************************
         *
         * seqCloseFile
         *
         * Deassociates a MIDI file with the given sequencer instance.
         *
         * pSeq                      -  The sequencer instance.
         *
         * Returns
         *   success If the operation is successful.
         *
         *   error_unsupported_function If the sequencer instance is not
         *     stopped.
         *
         * A call to seqCloseFile must be paired with a prior call to
         * seqOpenFile. All buffers associated with the file will be
         * freed and the file will be closed. The sequencer must be
         * stopped before this call will be accepted.
         *
         ***************************************************************************/
         void     sequence::CloseFile()
         {

            single_lock synchronouslock(&m_mutex, true);

            //if (e_state_no_file == GetState())
            //   return error_unsupported_function;

            file()->CloseFile();

            /* If we were prerolled, need to clean up -- have an open MIDI handle
            ** and buffers in the ready queue
            */

            //    single_lock slStream(&m_csStream, false);

            //    for (lpmh = m_lpmhFree; lpmh; lpmh = lpmh->lpNext)
            //    for (lpmh = m_buffera[0]; lpmh != nullptr; lpmh = lpmh->lpNext)


            /*   m_lpmhFree = nullptr;

            if (m_lpmhPreroll != nullptr)
            {
            slStream.lock();
            if(m_hstream != nullptr)
            {
            midiOutUnprepareHeader((HMIDIOUT) m_hstream, m_lpmhPreroll, sizeof(*m_lpmhPreroll));
            }
            slStream.unlock();
            }
            m_lpmhPreroll = nullptr;*/
            //    slStream.lock();
            if (m_hstream != nullptr)
            {
               m_buffera.midiOutUnprepareHeader((HMIDIOUT) m_hstream);
               midiStreamClose( m_hstream);
               m_hstream = nullptr;
            }
            //  slStream.unlock();

            SetState(e_state_no_file);

            return ::success;

         }


         /***************************************************************************
         *
         * seqPreroll
         *
         * Prepares the file for playback at the given position.
         *
         * pSeq                      - The sequencer instance.
         *
         * lpPreroll                 - Specifies the starting and ending ::duration
         *                             positions to play between.
         *
         * Returns
         *   success If the operation is successful.
         *
         *   error_unsupported_function If the sequencer instance is not
         *     opened or prerolled.
         *
         * open the device so we can initialize channels.
         *
         * Loop through the tracks. For each track, seek to the given position and
         * send the init data SMF gives us to the handle.
         *
         * Wait for all init buffers to finish.
         *
         * Unprepare the buffers (they're only ever sent here; the sequencer
         * engine merges them into a single stream during normal playback) and
         * refill them with the first chunk of data from the track.
         *
         *
         ****************************************************************************/
         void     sequence::Preroll(::thread * pthread, ::music::midi::LPPREROLL lpPreroll, bool bThrow)
         {

            __UNREFERENCED_PARAMETER(pthread);

            single_lock synchronouslock(&m_mutex, true);

            i32                 i;
            void                    smfrc;
            void      estatus = ::success;
            MIDIPROPTIMEDIV         mptd;
            LPMIDIHDR               lpmh = nullptr;
            u32                uDeviceID;


            ASSERT(m_iOpenMode == file::open_for_playing || IsInSpecialModeV001());

            m_flags.erase(e_flag_end_of_file);

            m_estatusLastError = ::success;

            if(GetState() != e_state_opened &&
                  GetState() != e_state_pre_rolled &&
                  !IsInSpecialModeV001())
               return error_unsupported_function;

            m_tkBase = lpPreroll->tickBase;
            m_tkEnd = lpPreroll->tickEnd;
            if(m_hstream != nullptr)
            {
               // Recollect buffers from MMSYSTEM back into free queue
               //
               SetState(::music::midi::sequence::e_state_reset);
               midiOutReset((HMIDIOUT) m_hstream);
               while (m_uBuffersInMMSYSTEM)
                  sleep(0_ms);
            }
            else
            {
            }

            m_uBuffersInMMSYSTEM = 0;
            SetState(::music::midi::sequence::e_state_pre_rolling);

            //
            // We've successfully opened the file and all of the tracks; now
            // open the MIDI device and set the time division.
            //
            // NOTE: seqPreroll is equivalent to seek; device might already be open
            //

            if (m_hstream == nullptr)
            {
               uDeviceID = m_uiDeviceID;
               //uDeviceID = MIDI_MAPPER;
               estatus = translate_mmr(midiStreamOpen(&m_hstream,
                                                      &uDeviceID,
                                                      1,
                                                      (uptr) &MidiOutProc,
                                                      0,
                                                      CALLBACK_FUNCTION));
               if(estatus != ::success)
               {
                  m_hstream = nullptr;
                  if(bThrow)
                  {
                     SetState(e_state_opened);
                     throw ::exception(multimedia::exception(multimedia::exception_music, EMidiPlayerPrerollStreamOpen));
                  }
                  informationf("midiStreamOpenError %d\n", estatus);
                  //goto seq_Preroll_Cleanup;
                  if(estatus == MMSYSERR_BADDEVICEID)
                     goto seq_Preroll_Cleanup;
                  else if(estatus == MMSYSERR_INVALPARAM)
                     goto seq_Preroll_Cleanup;
                  else if(estatus == MMSYSERR_NOMEM)
                     goto seq_Preroll_Cleanup;
                  else
                     goto seq_Preroll_Cleanup;
               }

               mptd.cbStruct  = sizeof(mptd);

               mptd.dwTimeDiv = m_MusicTempoTimeDivision;

               estatus = midiStreamProperty(m_hstream, (LPBYTE) &mptd, MIDIPROP_SET | MIDIPROP_TIMEDIV);

               if (estatus != ::success)
               {

                  information( "midiStreamProperty() -> %04X", (::u16)estatus);

                  midiStreamClose(m_hstream);

                  m_hstream = nullptr;

                  estatus = error_not_ready;

                  if(bThrow)
                  {

                     SetState(e_state_opened);

                     throw ::exception(multimedia::exception(multimedia::exception_midi, estatus));

                  }

                  goto seq_Preroll_Cleanup;

               }

            }

            estatus = ::success;

            m_buffera.Reset();

            lpmh = &m_buffera[0].m_midihdr;

            if(IsInSpecialModeV001())
            {
               if(m_eoperation == operation_general_midi_reset)
               {
                  const uchar gmModeOn[] =
                  {
                     //        0x00, 0x00, 0x00, 0x00,
                     //        0x00, 0x00, 0x00, 0x00,
                     //        0x1b, 0x8a, 0x06, MEVT_TEMPO,
                     0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00,
                     0x06, 0x00, 0x00, MEVT_LONGMSG,
                     0xf0, 0x7e, 0x7f, 0x09,
                     0x01, 0xf7, 0x00, 0x00
                  };
                  char * lpch = lpmh->lpData + lpmh->dwBytesRecorded;
                  lpmh->dwBytesRecorded += sizeof(gmModeOn);
                  ::memory_copy(lpch, gmModeOn, sizeof(gmModeOn));
                  SetSpecialModeV001Flag(false);
               }
            }

            smfrc = file()->WorkSeek(m_tkBase, lpmh);

            m_tkPrerollBase = get_file()->GetPosition();




            m_flags.erase(e_flag_end_of_file);
            file()->GetFlags().erase(file::EndOfFile);
            for(i = 1; i < m_buffera.get_size(); i++)
            {
               lpmh = m_buffera[i].GetMidiHdr();

               smfrc = file()->WorkStreamRender(lpmh, m_tkEnd, m_cbPrerollNominalMax);

               if(::success != smfrc && success_end_of_file != smfrc)
               {

                  information( "SFP: smfReadEvents() -> %u", (u32)smfrc);

                  estatus, smfrc;

                  goto seq_Preroll_Cleanup;

               }


               if (success_end_of_file == smfrc)
               {
                  m_flags.add(e_flag_end_of_file);
                  break;
               }
            }

            estatus = m_buffera.midiOutPrepareHeader((HMIDIOUT) m_hstream);
            if (estatus != ::success)
            {

               information( "midiOutPrepare(preroll) -> %lu!", (u32)estatus);

               estatus = error_not_ready;

               if(bThrow)
               {

                  SetState(e_state_opened);

                  throw ::exception(multimedia::exception(multimedia::exception_midi, estatus));

               }
               else
               {
                  goto seq_Preroll_Cleanup;
               }
            }


            estatus = m_buffera.midiStreamOut(m_hstream);
            if (estatus != ::success)
            {
               information( "midiOutPrepare(preroll) -> %lu!", (u32)estatus);
               estatus = error_not_ready;
               if(bThrow)
               {
                  SetState(e_state_opened);
                  throw ::exception(multimedia::exception(multimedia::exception_midi, EMidiPlayerPrerollPrepareHeader2));
               }
               else
               {
                  goto seq_Preroll_Cleanup;
               }
            }
            m_uBuffersInMMSYSTEM +=(u32)  m_buffera.get_size();

seq_Preroll_Cleanup:

            if (estatus != ::success)
            {
               SetState(e_state_opened);
               m_flags.erase(::music::midi::sequence::e_flag_waiting);
            }
            else
            {
               SetState(e_state_pre_rolled);
            }

            return estatus;
         }

         /***************************************************************************
         *
         * seqStart
         *
         * Starts playback at the current position.
         *
         * pSeq                      - The sequencer instance.
         *
         * Returns
         *   success If the operation is successful.
         *
         *   error_unsupported_function If the sequencer instance is not
         *     stopped.
         *
         *   error_not_ready If the underlying MIDI device could
         *     not be opened or fails any call.
         *
         * The sequencer must be prerolled before seqStart may be called.
         *
         * Just feed everything in the ready queue to the device.
         *
         ***************************************************************************/
         void     sequence::Start()
         {

            single_lock synchronouslock(&m_mutex, true);

            if (::music::midi::sequence::e_state_pre_rolled != GetState())
            {

               information( "seqStart(): State is wrong! [%u]", GetState());

               return error_unsupported_function;

            }

            SetState(::music::midi::sequence::e_state_playing);

            m_evMmsgDone.ResetEvent();

            void     estatus = ::success;
            if(m_hstream != nullptr)
            {
               estatus = midiStreamRestart(m_hstream);
            }
            synchronouslock.unlock();
            if(estatus == ::success)
            {
               thread()->PostMidiSequenceEvent(this, ::music::midi::sequence::e_event_midi_playback_start);

            }
            return estatus;
         }

         /***************************************************************************
         *
         * seqPause
         *
         * Pauses playback of the instance.
         *
         * pSeq                      - The sequencer instance.
         *
         * Returns
         *   success If the operation is successful.
         *
         *   error_unsupported_function If the sequencer instance is not
         *     playing.
         *
         * The sequencer must be playing before seqPause may be called.
         * Pausing the sequencer will cause all currently on notes to be turned
         * off. This may cause playback to be slightly inaccurate on restart
         * due to missing notes.
         *
         ***************************************************************************/
         void     sequence::Pause()

         {
            single_lock synchronouslock(&m_mutex, true);

            //    assert(nullptr != pSeq);

            if (e_state_playing != GetState())
               return error_unsupported_function;

            SetState(e_state_paused);

            void     estatus = ::success;
            //    single_lock slStream(&m_csStream, false);
            //  slStream.lock();
            if(m_hstream != nullptr)
            {
               estatus = midiStreamPause(m_hstream);
            }
            //slStream.unlock();

            SetLevelMeter(0);

            return ::success;
         }

         /***************************************************************************
         *
         * seqRestart
         *
         * Restarts playback of an instance after a pause.
         *
         * pSeq                      - The sequencer instance.
         *
         * Returns
         *    success If the operation is successful.
         *
         *    error_unsupported_function If the sequencer instance is not
         *     paused.
         *
         * The sequencer must be paused before seqRestart may be called.
         *
         ***************************************************************************/
         void     sequence::Restart()
         {
            //    assert(nullptr != pSeq);

            single_lock synchronouslock(&m_mutex, true);

            if (e_state_paused != GetState())
               return error_unsupported_function;

            SetState(e_state_playing);
            m_evMmsgDone.ResetEvent();

            //    void     estatus = 0;
            //    single_lock slStream(&m_csStream, false);
            //  slStream.lock();
            if(m_hstream != nullptr)
            {
               midiStreamRestart(m_hstream);
            }
            //slStream.unlock();
            return ::success;
         }

         /***************************************************************************
         *
         * seqStop
         *
         * Totally stops playback of an instance.
         *
         * pSeq                      - The sequencer instance.
         *
         * Returns
         *   success If the operation is successful.
         *
         *   error_unsupported_function If the sequencer instance is not
         *     paused or playing.
         *
         * The sequencer must be paused or playing before seqStop may be called.
         *
         ***************************************************************************/
         void     sequence::Stop()
         {

            single_lock synchronouslock(&m_mutex, true);

            if(GetState() == e_state_stopping)
               return ::success;

            // Automatic success if we're already stopped
            if (GetState() != e_state_playing
                  && GetState() != e_state_paused)
            {
               m_flags.erase(::music::midi::sequence::e_flag_waiting);
               GetPlayerLink().OnFinishCommand(::music::midi::player::command_stop);
               return ::success;
            }

            SetState(e_state_stopping);
            m_flags.add(::music::midi::sequence::e_flag_waiting);

            m_eventMidiPlaybackEnd.ResetEvent();

            if(m_hstream != nullptr)
            {

               m_estatusLastError = midiStreamStop(m_hstream);

               if(::success != m_estatusLastError)
               {

                  information( "::music::midi::sequence::Stop() -> midiOutStop() returned %lu in seqStop()!\n", (u32)m_estatusLastError);

                  m_flags.erase(e_flag_waiting);

                  return error_not_ready;

               }

            }

            //m_eventMidiPlaybackEnd.lock();

            SetLevelMeter(0);

            return ::success;

         }

         /***************************************************************************
         *
         * seqTime
         *
         * Determine the current position in playback of an instance.
         *
         * pSeq                      - The sequencer instance.
         *
         & ticka                    - A pointer to a u32 where the current position
         *                             in ticks will be returned.
         *
         * Returns
         *   success If the operation is successful.
         *
         *   error_not_ready If the underlying device fails to report
         *     the position.
         *
         *   error_unsupported_function If the sequencer instance is not
         *     paused or playing.
         *
         * The sequencer must be paused, playing or prerolled before seqTime
         * may be called.
         *
         ***************************************************************************/
         void     sequence::get_ticks(::duration &  pTicks)
         {
            single_lock synchronouslock(&m_mutex);
            if(!synchronouslock.lock(::duration(200)))
               return ::multimedia::result_internal;

            void                    mmr;
            MMTIME                  mmt;

            if (::music::midi::sequence::e_state_playing != GetState() &&
                  ::music::midi::sequence::e_state_paused != GetState() &&
                  ::music::midi::sequence::e_state_pre_rolling != GetState() &&
                  ::music::midi::sequence::e_state_pre_rolled != GetState() &&
                  ::music::midi::sequence::e_state_opened != GetState() &&
                  ::music::midi::sequence::e_state_stopping != GetState())
            {
               information( "seqTime(): State wrong! [is %u]", GetState());
               return error_unsupported_function;
            }

            pTicks = 0;
            if (e_state_opened != GetState())
            {
               pTicks = m_tkBase;
               if (e_state_pre_rolled != GetState())
               {
                  mmt.wType = TIME_TICKS;
                  //            single_lock slStream(&m_csStream, false);
                  //          slStream.lock();
                  if(m_hstream == nullptr)
                  {
                     informationf("m_hmidi == nullptr!!!!");
                     return error_not_ready;
                  }
                  else
                  {
                     try
                     {

                        mmr = midiStreamPosition(m_hstream, &mmt, sizeof(mmt));

                        if (::success != mmr)
                        {

                           information( "midiStreamPosition() returned %lu", (u32)mmr);

                           return error_not_ready;

                        }

                     }
                     catch(...)
                     {

                        return error_not_ready;

                     }

                     pTicks += mmt.u.ticks;

                  }

                  //        slStream.unlock();

               }

            }

            return ::success;

         }

         void sequence::get_time(::duration & time)
         {
            get_millis(time);
         }

         void     sequence::get_millis(::duration & time)
         {
            single_lock synchronouslock(&m_mutex);
            if(!synchronouslock.lock(::duration(200)))
               return ::multimedia::result_internal;

            void                    mmr;
            MMTIME                  mmt;

            if (e_state_playing != GetState() &&
                  e_state_paused != GetState() &&
                  e_state_pre_rolling != GetState() &&
                  e_state_pre_rolled != GetState() &&
                  e_state_opened != GetState() &&
                  e_state_stopping != GetState())
            {
               information( "seqTime(): State wrong! [is %u]", GetState());
               return error_unsupported_function;
            }

            time = 0;
            if (e_state_opened != GetState())
            {
               time = (iptr) TicksToMillisecs(m_tkBase);
               if (e_state_pre_rolled != GetState())
               {
                  mmt.wType = TIME_MS;
                  //            single_lock slStream(&m_csStream, false);
                  //          slStream.lock();
                  if(m_hstream == nullptr)
                  {
                     informationf("m_hmidi == nullptr!!!!");
                     return error_not_ready;
                  }
                  else
                  {
                     try
                     {

                        mmr = midiStreamPosition(m_hstream, &mmt, sizeof(mmt));
                        if (::success != mmr)
                        {
                           information( "midiStreamPosition() returned %lu", (u32)mmr);
                           return error_not_ready;
                        }
                     }
                     catch(...)
                     {
                        return error_not_ready;
                     }
                     time += mmt.u.ms;
                  }
                  //        slStream.unlock();
               }
            }

            return ::success;
         }

         /***************************************************************************
         *
         * seqMillisecsToTicks
         *
         * Given a ::duration offset in the output stream, returns the associated
         * ::duration position.
         *
         * pSeq                      - The sequencer instance.
         *
         * msOffset                  - The ::duration offset into the stream.
         *
         * Returns the number of ticks into the stream.
         *
         ***************************************************************************/
         ::duration sequence::MillisecsToTicks(::duration msOffset)
         {
            return file()->MillisecsToTicks(msOffset);
         }

         /***************************************************************************
         *
         * seqTicksToMillisecs
         *
         * Given a ::duration offset in the output stream, returns the associated
         * ::duration position.
         *
         * pSeq                      - The sequencer instance.
         *
         * tickOffset                  - The ::duration offset into the stream.
         *
         * Returns the number of ::durations into the stream.
         *
         ***************************************************************************/
         ::duration sequence::TicksToMillisecs(::duration tickOffset)
         {
            return file()->TicksToMillisecs(tickOffset);
         }

         void sequence::OnDone(HMIDISTRM hmidistream, LPMIDIHDR lpmidihdr)
         {
            __UNREFERENCED_PARAMETER(hmidistream);
            void                   smfrc;
            midi_callback_data *      lpData;
            ASSERT(lpmidihdr != nullptr);
            lpData = (midi_callback_data *) lpmidihdr->dwUser;
            ASSERT(lpData != nullptr);

            ::music::midi::sequence * psequence = lpData->m_psequence;

            sequence_thread * pthread = dynamic_cast < sequence_thread * > (psequence->m_pthread);

            ASSERT(nullptr != lpmidihdr);

            --m_uBuffersInMMSYSTEM;

            if(m_uBuffersInMMSYSTEM <= 0)
            {
               m_evBuffersZero.SetEvent();
            }

            if (e_state_reset == GetState())
            {
               // We're recollecting buffers from MMSYSTEM
               //
               //      if (lpmidihdr != m_lpmhPreroll)
               //    {
               //     lpmidihdr->lpNext    = m_lpmhFree;
               //   m_lpmhFree           = lpmidihdr;
               //      }
               return;
            }

            bool bStopping = e_state_stopping == GetState();
            bool bEndOfPlay = m_uBuffersInMMSYSTEM <= 0;
            bool bSpecialModeV001End = m_flags.has(e_flag_operation_end);

            if (bStopping || bEndOfPlay || bSpecialModeV001End)
            {
               //
               // Reached EOF, just put the buffer back on the free
               // list
               //
               if(bSpecialModeV001End)
               {
                  m_flags.erase(e_flag_operation_end);
                  informationf("void CALLBACK ::music::midi::sequence::MidiOutProc m_flags.has(e_flag_operation_end\n");
                  pthread->PostMidiSequenceEvent(
                  this,
                  ::music::midi::sequence::EventSpecialModeV001End,
                  lpmidihdr);
               }
               else if(bStopping)
               {
                  if(m_uBuffersInMMSYSTEM == 0)
                  {
                     informationf("void CALLBACK ::music::midi::sequence::MidiOutProc e_state_stopping == pSeq->GetState()\n");
                     pthread->PostMidiSequenceEvent(
                     this,
                     ::music::midi::sequence::EventStopped,
                     lpmidihdr);
                  }
               }
               else
               {
                  if(m_flags.has(e_flag_end_of_file))
                  {
                     informationf("void CALLBACK ::music::midi::sequence::MidiOutProc m_flags.has(e_flag_end_of_file\n");
                  }
                  if(m_uBuffersInMMSYSTEM <= 0)
                  {
                     pthread->PostMidiSequenceEvent(
                     this,
                     ::music::midi::sequence::e_event_midi_playback_end,
                     lpmidihdr);
                  }
               }
            }
            else
            {
               if(IsInSpecialModeV001())
               {
                  if(m_eoperation == operation_general_midi_reset)
                  {
                     const uchar gmModeOn[] =
                     {
                        //        0x00, 0x00, 0x00, 0x00,
                        //        0x00, 0x00, 0x00, 0x00,
                        //        0x1b, 0x8a, 0x06, MEVT_TEMPO,
                        0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00,
                        0x06, 0x00, 0x00, MEVT_LONGMSG,
                        0xf0, 0x7e, 0x7f, 0x09,
                        0x01, 0xf7, 0x00, 0x00
                     };
                     lpmidihdr->dwBytesRecorded = sizeof(gmModeOn);
                     ::memory_copy(lpmidihdr->lpData, gmModeOn, sizeof(gmModeOn));
                  }
                  else if(m_eoperation == operation_tempo_change)
                  {
                     ::music::midi::event event;
                     file()->GetTempoEvent(event);
                     file()->StreamEvent(event.GetDelta(), &event, lpmidihdr, I32_MAXIMUM, 256);
                     // lpmidihdr->dwBytesRecorded = sizeof(gmModeOn);
                     // ::memory_copy(lpmidihdr->lpData, gmModeOn, sizeof(gmModeOn));
                  }
                  else
                  {
                     ASSERT(false);
                  }
                  //post_thread_message(lpData->dwThreadID, MIDIPLAYERMESSAGE_STREAMOUT, (WPARAM) pSeq, (LPARAM) lpmidihdr);
                  pthread->PostMidiSequenceEvent(
                  this,
                  e_event_midi_playback_out,
                  lpmidihdr);
                  m_flags.add(e_flag_operation_end);
                  smfrc = ::success;
                  return;
               }
               //
               // Not EOF yet; attempt to fill another buffer
               //
               pthread->PostMidiSequenceEvent(
               this,
               e_event_midi_playback_out,
               lpmidihdr);
            }

         }

         void sequence::OnPositionCB(LPMIDIHDR lpmidihdr)
         {
            ASSERT(lpmidihdr != nullptr);
            LPBYTE lpbData = (LPBYTE) (lpmidihdr->lpData + lpmidihdr->dwOffset);
            MIDIEVENT * lpme = (MIDIEVENT *) lpbData;
            file::buffer::midi_stream_event_header * pheader = (file::buffer::midi_stream_event_header *) &lpme->dwParms[0];
            lpbData = (LPBYTE) pheader;
            LPDWORD lpdwParam;

            i32 iSize = pheader->m_dwLength;
            switch(pheader->m_dwType)
            {
            case 0:
            {
               array < ::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1 &> * plyriceventa = nullptr;
               array < ::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1 &> lyriceventa;
               for(i32 i = sizeof(file::buffer::midi_stream_event_header); i < iSize;)
               {
                  pheader = (file::buffer::midi_stream_event_header *) &lpbData[i];
                  lpdwParam = (LPDWORD) &lpbData[i + sizeof(file::buffer::midi_stream_event_header)];
                  ASSERT(*lpdwParam == pheader->m_dwType);
                  switch(pheader->m_dwType)
                  {
                  case EVENT_ID_LYRIC_V1:
                  {
                     if(plyriceventa == nullptr)
                     {
                        plyriceventa = new array <::ikaraoke::lyric_event_v1, ::ikaraoke::lyric_event_v1 &>;
                     }
                     ::memory_file memFile(get_application(), (LPBYTE) &lpdwParam[1], pheader->m_dwLength - sizeof(u32));
                     /* x2x                  CArchive ar(&memFile, CArchive::load);
                     lyriceventa.Serialize(ar);
                     plyriceventa->append(lyriceventa); */
                  }
                  break;
                  case EVENT_ID_PAD:
                  {
                  }
                  break;
                  case EVENT_ID_NOTE_ON:
                  {
                     ::file::byte_stream_memory_file memFile(get_application(), (LPBYTE) &lpdwParam[1], pheader->m_dwLength - sizeof(u32));
                     for(i32 i = 0; i < m_iaLevel.get_size(); i++)
                     {
                        ::u8 b;
                        memFile >> b;
                        m_iaLevel.element_at(i) = b;
                     }
                  }
                  break;
                  }
                  i += pheader->m_dwLength + sizeof(file::buffer::midi_stream_event_header);
               }
               /*         if(plyriceventa != nullptr)
               {
               ::PostMessage(m_midicallbackdata.oswindow, WM_APP + 3388, 3388, (LPARAM) plyriceventa);
               }*/
            }
            break;
            case EVENT_ID_PAD:
               break;
            default:
               ASSERT(false);
               break;
            }

         }

         void CALLBACK sequence::MidiOutProc(HMIDIOUT hmo, ::u32 wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
         {

            __UNREFERENCED_PARAMETER(hmo);
            __UNREFERENCED_PARAMETER(dwInstance);
            __UNREFERENCED_PARAMETER(dwParam2);

            LPMIDIHDR               lpmh         = (LPMIDIHDR) dwParam1;

            switch(wMsg)
            {
            case MOM_POSITIONCB:
               ((midi_callback_data *) lpmh->dwUser)->m_psequence->OnPositionCB(lpmh);
               break;
            case MOM_DONE:
               ((midi_callback_data *) lpmh->dwUser)->m_psequence->OnDone(nullptr,lpmh);
               return;
            }

         }


         bool sequence::IsPlaying()
         {
            return GetState() == e_state_playing ||
                   GetState() == e_state_stopping;
         }




         void     sequence::SaveFile()
         {

            return SaveFile(file()->m_strName);

         }


         void     sequence::SaveFile(const ::string & lpFileName)
         {

            return file()->SaveFile(lpFileName);

         }


         void     sequence::SaveFile(file_pointer &ar)
         {

            return file()->SaveFile(*ar);

         }


         u32 sequence::SetState(u32 uiState)
         {

            m_uiPreviousState = m_uiState;

            m_uiState = uiState;

            return m_uiPreviousState;

         }


         void sequence::SetSpecialModeV001Flag(bool bSet)
         {

            if(m_flags.has(e_flag_operation))
               m_flags.add(e_flag_was_operation);
            else
               m_flags.erase(e_flag_was_operation);

            if(bSet)
               m_flags.add(e_flag_operation);
            else
               m_flags.erase(e_flag_operation);

         }


         void sequence::set_operation(e_operation eoperation)
         {

            m_uiPreSpecialModeV001State = m_uiState;

            SetSpecialModeV001Flag();

            m_eoperation = eoperation;

         }


         bool sequence::IsInSpecialModeV001()
         {

            return m_flags.has(e_flag_operation);

         }


         bool sequence::WasInSpecialModeV001()
         {

            return m_flags.has(e_flag_was_operation);

         }


         void sequence::GetMidiDoneData(::music::midi::LPMIDIDONEDATA lpmdd)
         {

            lpmdd->m_flags = m_flags;

            lpmdd->m_uiSeqPreviousState = m_uiPreviousState;

            lpmdd->m_uiSeqState = m_uiState;

         }


         i32 sequence::GetKeyShift()
         {

            if(m_pfile == nullptr)
               return 0;

            return file()->GetKeyShift();

         }


         i32 sequence::SetKeyShift(i32 iKeyShift)
         {

            /*//    single_lock slStream(&m_csStream, false);
            //  slStream.lock();
            if(m_hstream)
            {
            midiStreamPause(m_hstream);
            if(!file()->SetKeyShift(iKeyShift))
            return false;
            midiStreamRestart(m_hstream);
            }
            //slStream.unlock();
            return true;
            void ::music::midi::sequence::SetKeyShift(i32 iShift)
            {*/

            bool bPlay = IsPlaying();

            ::duration ticks = 0;

            if(bPlay)
            {

               ticks = GetPositionTicks();

               Stop();

            }

            get_file()->SetKeyShift(iKeyShift);

            if(bPlay)
            {

               m_pthread->PrerollAndWait(ticks);

               Start();

            }

            //}

            return true;

         }


         i32 sequence::GetTempoShift()
         {

            return file()->GetTempoShift();

         }



         i32 sequence::SetTempoShift(i32 iTempoShift)
         {

            //midiStreamPause(m_hstream);

            if(failed(file()->SetTempoShift(iTempoShift)))
               return false;

            //midiStreamRestart(m_hstream);

            return true;

         }


         bool sequence::SetMidiOutDevice(u32 uiDevice)
         {

            if(uiDevice == ::music::midi::device_default)
            {

               m_uiDeviceID = MIDI_MAPPER;

            }
            else
            {

               m_uiDeviceID = uiDevice;

            }

            return true;
         }

         void     sequence::CloseStream()
         {
            single_lock synchronouslock(&m_mutex, true);
            if(IsPlaying())
            {
               Stop();
            }

            /* If we were prerolled, need to clean up -- have an open MIDI handle
            ** and buffers in the ready queue
            */

            m_buffera.midiOutUnprepareHeader((HMIDIOUT) m_hstream);

            if (m_hstream != nullptr)
            {
               midiStreamClose( m_hstream);
               m_hstream = nullptr;
            }

            SetState(e_state_opened);

            return ::success;
         }

         void sequence::SetLevelMeter(i32 iLevel)
         {
            for(i32 i = 0; i < m_iaLevel.get_size(); i++)
            {
               m_iaLevel.element_at(i) = iLevel;
            }
         }

         void sequence::on_midi_playback_end(::music::midi::sequence::event * pevent)
         {
            __UNREFERENCED_PARAMETER(pevent);
            single_lock synchronouslock(&m_mutex, true);
            //   LPMIDIHDR lpmh = ptopic->m_lpmh;
            //   midi_callback_data * lpData = &m_midicallbackdata;
            void     estatus;


            if(0 == m_uBuffersInMMSYSTEM)
            {
               information( "seqBufferDone: normal sequencer shutdown.");

               /* Totally done! Free device and notify.
               */
               if(m_hstream)
               {
                  if((estatus = m_buffera.midiOutUnprepareHeader((HMIDIOUT) m_hstream))
                        != ::success)
                  {
                     information( "midiOutUnprepareHeader failed in seqBufferDone! (%lu)", (u32)estatus);
                  }
                  midiStreamClose(m_hstream);
                  m_hstream = nullptr;
               }

               m_estatusLastError = ::success;
               m_flags.erase(e_flag_waiting);

               m_evMmsgDone.SetEvent();
            }
         }

         void sequence::OnEvent(::music::midi::sequence::event * pevent)
         {
            switch(ptopic->m_atom)
            {
            case EventSpecialModeV001End:
            {
               SetSpecialModeV001Flag(false);
               on_midi_playback_end(pevent);
               SetState(m_uiPreSpecialModeV001State);
            }
            break;
            case EventStopped:
            {
               on_midi_playback_end(pevent);
               SetState(e_state_opened);
            }
            break;
            case e_event_midi_playback_end:
            {
               on_midi_playback_end(pevent);
               SetState(e_state_opened);
            }
            break;
            case e_event_midi_playback_out:
            {



               single_lock synchronouslock(&m_mutex, true);

               ::music::midi::mmsystem::sequence::event * pev = (::music::midi::mmsystem::sequence::event *) pevent;

               if(m_flags.has(e_flag_end_of_file))
               {

                  return;

               }

               LPMIDIHDR lpmh = pev->m_lpmh;

               void     smfrc;

               if(IsInSpecialModeV001())
               {
                  informationf("::music::midi::sequence::OnEvent e_event_midi_playback_out IsInSpecialModeV001");
               }
               else
               {
                  smfrc = file()->WorkStreamRender(lpmh, m_tkEnd, m_cbPrerollNominalMax);
               }

               switch(smfrc)
               {
               case success:

                  break;

               case success_end_of_file:

                  m_flags.add(e_flag_end_of_file);

                  smfrc = ::success;

                  break;

               default:


                  information( "smfReadEvents returned %lu in callback!", (u32) smfrc);

                  SetState(e_state_stopping);

                  break;

               }

               if(::music::midi::sequence::e_state_stopping == GetState())
               {

                  thread()->PostMidiSequenceEvent(this, e_event_midi_playback_end, lpmh);

                  return;

               }

               void     estatus;

               if(m_hstream != nullptr)
               {

                  estatus = midiStreamOut(m_hstream, lpmh, sizeof(*lpmh));

                  if(estatus == ::success)
                  {

                     ++m_uBuffersInMMSYSTEM;

                  }
                  else
                  {

                     informationf("seqBufferDone(): midiStreamOut() returned %lu!", (u32)estatus);

                     SetState(::music::midi::sequence::e_state_stopping);

                  }


               }

            }
            default:
               break;
            }
         }

         ::duration sequence::GetPositionTicks()
         {
            single_lock synchronouslock(&m_mutex);
            if(!synchronouslock.lock(::duration(0)))
               return -1;
            MMTIME mmt;
            mmt.wType = TIME_TICKS;
            if(::success ==
                  midiStreamPosition(
                  m_hstream,
                  &mmt,
                  sizeof(mmt)))
               return mmt.u.ticks + m_tkPrerollBase;
            else
               return -1;
         }


         bool sequence::IsChangingTempo()
         {
            return m_flags.has(::music::midi::sequence::e_flag_tempo_change);
         }
         void sequence::SetTempoChangeFlag(bool bSet)
         {
            if(bSet)
               m_flags.add(::music::midi::sequence::e_flag_tempo_change);
            else
               m_flags.erase(::music::midi::sequence::e_flag_tempo_change);
         }




         bool sequence::IsNull()
         {
            if(this == nullptr)
               return true;
            return false;
         }

         ::duration sequence::TimeToPosition(::duration ::duration)
         {
            return ::duration(MillisecsToTicks((iptr) ::duration));
         }

         ::duration sequence::PositionToTime(::duration tick)
         {
            return ::duration(TicksToMillisecs((::duration) (iptr) tick));
         }

         void sequence::GetPosition(::duration & position)
         {
            get_ticks(position);
         }

         bool sequence::IsOpened()
         {
            return GetState() != e_state_no_file;
         }

         void sequence::GetTimeLength(::duration & time)
         {
            time = m_msLength;
         }



         void sequence::Prepare(
         string_array_array & straa,
         imedia_position_2darray & tickaaTokensTicks,
         i32 iMelodyTrack,
         int2a & ia2TokenLine,
         ::ikaraoke::data & data)
         {

            __UNREFERENCED_PARAMETER(straa);

            ::music::midi::file::buffer & file = *this->file();

            ::music::midi::tracks & tracks = file.GetTracks();

            ASSERT(!file.IsNull());
            file.GetTracks().seek_begin();
            ::duration               tickMax = file.m_tkLength;
            ::duration               tickLastPosition = 0;


            ::ikaraoke::static_data & staticdata = data.GetStaticData();

            if(staticdata.m_LyricsDisplayOffset < 480)
            {
               staticdata.m_LyricsDisplayOffset = 480;
            }
            if(staticdata.m_LyricsDisplayOffset > 720)
            {
               staticdata.m_LyricsDisplayOffset = 720;
            }
            staticdata.m_LyricsDisplay = 30;

            imedia_position_2darray tickaaNoteOnPositions(this);
            imedia_position_2darray tickaaNoteOffPositions(this);
            imedia_position_2darray tickaaBegPositions(this);
            imedia_position_2darray tickaaEndPositions(this);
            duration_2darray ms2DTokensMillis(this);
            duration_2darray ms2DNoteOnMillis(this);
            duration_2darray ms2DNoteOffMillis(this);
            duration_2darray ms2DBegMillis(this);
            duration_2darray ms2DEndMillis(this);
            ::music::midi::events midiEvents;




            // Note on and off events
            // and maximum and minimum
            // pitch bend peaks.
            ::music::midi::events midiEventsLevel2;

            ::music::midi::events noteOnEvents;
            ::music::midi::events noteOffEvents;

            ::music::midi::events eventsLevel2Beg;
            ::music::midi::events eventsLevel2End;
            ::ikaraoke::events_tracks_v1 lyricEventsForPositionCB;
            ::ikaraoke::events_tracks_v1 lyricEventsForBouncingBall;
            ::ikaraoke::events_tracks_v1 lyricEventsForScoring;
            ::ikaraoke::events_tracks_v1 lyricEvents;

            //   tracks.seek_begin();
            // tracks.GetXFInfoHeaders(
            //  &m_xfInfoHeaders);



            file.PositionToTime(
            ms2DTokensMillis,
            tickaaTokensTicks,
            0);

            ::ikaraoke::lyric_events_v2 *pLyricEventsV2;
            ::ikaraoke::lyric_events_v2 *pLyricEventsV2_;
            ::ikaraoke::lyric_events_v2 *pLyricEventsV2B;
            ::ikaraoke::lyric_events_v2 *pLyricEventsV2C;
            ::music::midi::events *pMidiEventsV1;

            tickaaNoteOnPositions.set_size_create(tickaaTokensTicks.get_size());
            tickaaNoteOffPositions.set_size_create(tickaaTokensTicks.get_size());
            tickaaBegPositions.set_size_create(tickaaTokensTicks.get_size());
            tickaaEndPositions.set_size_create(tickaaTokensTicks.get_size());
            i32 i;
            for(i = 0; i < tickaaTokensTicks.get_size(); i++)
            {
               pLyricEventsV2 = new ::ikaraoke::lyric_events_v2();
               pLyricEventsV2B = new ::ikaraoke::lyric_events_v2();
               pLyricEventsV2C = new ::ikaraoke::lyric_events_v2();
               pLyricEventsV2_ = new ::ikaraoke::lyric_events_v2();
               staticdata.m_eventsv1.add(pLyricEventsV2);
               staticdata.m_eventsv1.add(pLyricEventsV2B);
               staticdata.m_eventsv1.add(pLyricEventsV2C);
               staticdata.m_eventsv1.add(pLyricEventsV2_);
               lyricEvents.add(pLyricEventsV2_);
               lyricEventsForScoring.add(pLyricEventsV2);
               lyricEventsForPositionCB.add(pLyricEventsV2B);
               lyricEventsForBouncingBall.add(pLyricEventsV2C);
               pLyricEventsV2->m_iType = 0;
               pLyricEventsV2->m_iOrder = i;
               pLyricEventsV2->m_iType = 2;
               pLyricEventsV2->m_iOrder = i;
               pLyricEventsV2B->m_iType = ikaraoke::EventRunningElement;
               pLyricEventsV2B->m_iOrder = i;
               pLyricEventsV2C->m_iType = 4;
               pLyricEventsV2C->m_iOrder = i;
               pMidiEventsV1 = nullptr;
               if(iMelodyTrack < 0)
               {
                  pLyricEventsV2->m_iTrack =
                  file.WorkCalcMelodyTrack(
                  &pMidiEventsV1,
                  tickaaTokensTicks.operator[](i),
                  ia2TokenLine[i]);
               }
               else
               {
                  pLyricEventsV2->m_iTrack = iMelodyTrack;
               }
               pLyricEventsV2B->m_iTrack = pLyricEventsV2->m_iTrack;
               pLyricEventsV2C->m_iTrack = pLyricEventsV2->m_iTrack;
               pLyricEventsV2_->m_iTrack = pLyricEventsV2->m_iTrack;
               if(pLyricEventsV2->m_iTrack < 0)
               {
                  pLyricEventsV2->m_iTrack = tracks.m_iMelodyTrackTipA;
                  pLyricEventsV2B->m_iTrack = tracks.m_iMelodyTrackTipA;
                  pLyricEventsV2C->m_iTrack = tracks.m_iMelodyTrackTipA;
                  pLyricEventsV2_->m_iTrack = tracks.m_iMelodyTrackTipA;
               }
               staticdata.SetGuessMelodyTrack(pLyricEventsV2->m_iTrack);
               if(pLyricEventsV2->m_iTrack >= 0)
               {
                  if(file.GetFormat() == 0)
                  {
                     tracks.TrackAt(0)->WorkSeekBegin();
                     ((::music::midi::track *)tracks.TrackAt(0))->WorkGetNoteOnOffEventsV1(
                     &midiEvents,
                     (i32) pLyricEventsV2->m_iTrack,
                     file.GetFormat() == 1);
                     tracks.TrackAt(0)->WorkSeekBegin();
                     ((::music::midi::track *)tracks.TrackAt(0))->WorkGetLevel2Events(
                     &midiEventsLevel2,
                     (i32) pLyricEventsV2->m_iTrack,
                     file.GetFormat() == 1);
                  }
                  else
                  {
                     tracks.TrackAt(pLyricEventsV2->m_iTrack)->seek_begin();
                     ((::music::midi::track *)tracks.TrackAt(pLyricEventsV2->m_iTrack))->GetLevel2Events(
                     &midiEvents,
                     (i32) pLyricEventsV2->m_iTrack,
                     file.GetFormat() == 1);
                     tracks.TrackAt(pLyricEventsV2->m_iTrack)->seek_begin();
                     ((::music::midi::track *)tracks.TrackAt(pLyricEventsV2->m_iTrack))->GetLevel2Events(
                     &midiEventsLevel2,
                     (i32) pLyricEventsV2->m_iTrack,
                     file.GetFormat() == 1);
                  }
               }

               ::music::midi::util miditutil(this);

               miditutil.PrepareNoteOnOffEvents(
               &noteOnEvents,
               &noteOffEvents,
               (i32) pLyricEventsV2->m_iTrack,
               file.GetFormat(),
               &midiEvents,
               tickaaTokensTicks.operator[](i));

               miditutil.PrepareLevel2Events(
               &eventsLevel2Beg,
               &eventsLevel2End,
               (i32) pLyricEventsV2->m_iTrack,
               file.GetFormat(),
               &midiEventsLevel2,
               tickaaTokensTicks.operator[](i));


               tickaaNoteOnPositions[i]     = noteOnEvents.m_tickaEventsPosition;
               tickaaNoteOffPositions[i]    = noteOffEvents.m_tickaEventsPosition;
               tickaaBegPositions[i]        = eventsLevel2Beg.m_tickaEventsPosition;
               tickaaEndPositions[i]        = eventsLevel2End.m_tickaEventsPosition;
               pLyricEventsV2->m_dwaNotesData.copy(noteOnEvents.m_dwaEventsData);
               pLyricEventsV2B->m_dwaNotesData.copy(eventsLevel2Beg.m_dwaEventsData);
               pLyricEventsV2C->m_dwaNotesData.copy(eventsLevel2Beg.m_dwaEventsData);
               pLyricEventsV2_->m_dwaNotesData.copy(eventsLevel2Beg.m_dwaEventsData);
               midiEvents.erase_all();
               noteOnEvents.erase_all();
               noteOffEvents.erase_all();
               midiEventsLevel2.erase_all();
               eventsLevel2Beg.erase_all();
               eventsLevel2End.erase_all();
               delete pMidiEventsV1;
            }

            file.PositionToTime(
            ms2DNoteOnMillis,
            tickaaNoteOnPositions,
            0);

            file.PositionToTime(
            ms2DNoteOffMillis,
            tickaaNoteOffPositions,
            0);

            file.PositionToTime(
            ms2DBegMillis,
            tickaaBegPositions,
            0);

            file.PositionToTime(
            ms2DEndMillis,
            tickaaEndPositions,
            0);


            ::ikaraoke::lyric_events_v1 *pLyricEventsV1;


            for(i = 0; i < tickaaTokensTicks.get_size(); i++)
            {
               pLyricEventsV2 = (::ikaraoke::lyric_events_v2 *) lyricEvents.get_at(i);
               staticdata.m_eventstracks.add(pLyricEventsV2);
               file.TimeToPosition(
               pLyricEventsV2->m_tickaTokensPosition,
               ms2DTokensMillis[i],
               0);
               file.TimeToPosition(
               pLyricEventsV2->m_tickaNotesPosition,
               ms2DNoteOnMillis[i],
               0);

               ::duration time1(0);
               ::duration time2(0);

               pLyricEventsV2->m_timeaTokensPosition.CopySorted(
               ms2DTokensMillis[i],
               time1,
               time2);

               pLyricEventsV2->m_timeaNotesDuration.Diff(
               ms2DNoteOffMillis[i],
               ms2DNoteOnMillis[i]);

               ::duration time3(0);
               ::duration time4(0);

               pLyricEventsV2->m_timeaNotesPosition.CopySorted(
               ms2DNoteOnMillis[i],
               time3,
               time4);

               ::duration time5(I32_MAXIMUM);

               pLyricEventsV2->m_timeaTokensDuration.ElementDiff(
               ms2DTokensMillis[i],
               time5);

            }


            for(i = 0; i < tickaaTokensTicks.get_size(); i++)
            {
               pLyricEventsV1 = new ::ikaraoke::lyric_events_v1();
               pLyricEventsV1->m_iType = ikaraoke::EventAdvanceShow;
               pLyricEventsV1->m_iOrder = i;
               //staticdata.m_eventsTracksForPositionCB.add(pLyricEventsV1);
               file.TimeToPosition(
               pLyricEventsV1->m_tickaTokensPosition,
               ms2DTokensMillis[i],
               -1000);
               //lyric_track * pLyricTrk = file.GetTracks().CreateLyricTrack();
               //pLyricTrk->Prepare(*pLyricEventsV1);
            }


            for(i = 0; i < tickaaTokensTicks.get_size(); i++)
            {
               pLyricEventsV2 = (::ikaraoke::lyric_events_v2 *) lyricEventsForScoring.get_at(i);
               staticdata.m_eventsTracksForScoring.add(pLyricEventsV2);
               file.TimeToPosition(
               pLyricEventsV2->m_tickaTokensPosition,
               ms2DTokensMillis[i],
               0);
               file.TimeToPosition(
               pLyricEventsV2->m_tickaNotesPosition,
               ms2DNoteOnMillis[i],
               0);

               ::duration time1(-100);
               ::duration time2(0);

               pLyricEventsV2->m_timeaTokensPosition.CopySorted(
               ms2DTokensMillis[i],
               time1,
               time2);

               pLyricEventsV2->m_timeaNotesDuration.Diff(
               ms2DNoteOffMillis[i],
               ms2DNoteOnMillis[i]);

               ::duration time3(-100);
               ::duration time4(0);

               pLyricEventsV2->m_timeaNotesPosition.CopySorted(
               ms2DNoteOnMillis[i],
               time3,
               time4);

               ::duration time5(I32_MAXIMUM);

               pLyricEventsV2->m_timeaTokensDuration.ElementDiff(
               ms2DTokensMillis[i],
               time5);

               pLyricEventsV2->PrepareForScoring(this);
            }







            for(i = 0; i < tickaaTokensTicks.get_size(); i++)
            {
               pLyricEventsV2 = (::ikaraoke::lyric_events_v2 *) lyricEventsForPositionCB.get_at(i);
               staticdata.m_eventsTracksForPositionCB.add(pLyricEventsV2);

               staticdata.m_eventstracksV002.add(pLyricEventsV2);

               file.TimeToPosition(
               pLyricEventsV2->m_tickaTokensPosition,
               ms2DTokensMillis[i],
               -100);

               file.TimeToPosition(
               pLyricEventsV2->m_tickaNotesPosition,
               ms2DNoteOnMillis[i],
               -100);

               ::duration time1(-100);
               ::duration time2(0);

               pLyricEventsV2->m_timeaTokensPosition.CopySorted(
               ms2DTokensMillis[i],
               time1,
               time2);

               pLyricEventsV2->m_timeaNotesDuration.Diff(
               ms2DNoteOffMillis[i],
               ms2DNoteOnMillis[i]);

               /*
               pLyricEventsV2->m_timeaNotesDuration.Diff(
               ms2DNoteOffMillis[i],
               ms2DNoteOnMillis[i]);
               */


               ::duration time3(-100);
               ::duration time4(0);

               pLyricEventsV2->m_timeaNotesPosition.CopySorted(
               ms2DNoteOnMillis[i],
               time3,
               time4);

               ::duration time5(I32_MAXIMUM);

               pLyricEventsV2->m_timeaTokensDuration.ElementDiff(
               ms2DTokensMillis[i],
               time5);

               pLyricEventsV2->PrepareForLyricsDisplay(this);
               //lyric_track * pLyricTrk = file.GetTracks().CreateLyricTrack();
               //pLyricTrk->Prepare(*pLyricEventsV2);
            }










            for(i = 0; i < tickaaTokensTicks.get_size(); i++)
            {
               pLyricEventsV2 = (::ikaraoke::lyric_events_v2 *) lyricEventsForBouncingBall.get_at(i);
               staticdata.m_eventsTracksForBouncingBall.add(pLyricEventsV2);

               file.TimeToPosition(
               pLyricEventsV2->m_tickaTokensPosition,
               ms2DTokensMillis[i],
               -100);

               file.TimeToPosition(
               pLyricEventsV2->m_tickaNotesPosition,
               ms2DNoteOnMillis[i],
               -100);

               ::duration time1(-100);
               ::duration time2(0);

               pLyricEventsV2->m_timeaTokensPosition.CopySorted(
               ms2DTokensMillis[i],
               time1,
               time2);

               pLyricEventsV2->m_timeaNotesDuration.Diff(
               ms2DEndMillis[i],
               ms2DBegMillis[i]);

               /*
               pLyricEventsV2->m_timeaNotesDuration.Diff(
               ms2DNoteOffMillis[i],
               ms2DNoteOnMillis[i]);
               */

               ::duration time3(-100);
               ::duration time4(0);

               pLyricEventsV2->m_timeaNotesPosition.CopySorted(
               ms2DNoteOnMillis[i],
               time3,
               time4);

               ::duration time5(I32_MAXIMUM);

               pLyricEventsV2->m_timeaTokensDuration.ElementDiff(
               ms2DTokensMillis[i],
               time5);

               pLyricEventsV2->PrepareForBouncingBall(this);
            }

         }

         void sequence::Prepare(i32 iTrack, ::ikaraoke::data & data)
         {

            ::music::midi::mmsystem::buffer & file = *this->file();

            ::music::midi::tracks & tracks = file.GetTracks();

            string_array_array & straa = data.GetStaticData().m_straaRawTokens;

            imedia_position_2darray position2a;

            int2a ia2TokenLine;


            ia2TokenLine.add_new();

            tracks.WorkSeekBegin();
            tracks.WorkGetEmptyXFTokens(
            iTrack,
            straa,
            position2a,
            nullptr);

            Prepare(
            straa,
            position2a,
            iTrack,
            ia2TokenLine,
            data);

         }

         void sequence::Prepare(::ikaraoke::data & data)
         {
            ::music::midi::mmsystem::buffer & file = *this->file();
            ::music::midi::tracks & tracks = file.GetTracks();
            string_array_array & straa = data.GetStaticData().m_straaRawTokens;
            imedia_position_2darray position2a;
            int2a iaaTokenLine;

            ::music::xf::info_headers xfihs;
            get_file()->GetTracks().GetXFInfoHeaders(&xfihs);

            ::ikaraoke::static_data & staticdata = data.GetStaticData();
            string str;

            // add Title
            staticdata.m_straTitleFormat.add("%0");
            staticdata.m_straaTitle.set_app(this);
            staticdata.m_straaTitle.add_new();
            staticdata.m_straaTitle[0].add(xfihs.m_strSongName);

            if(xfihs.m_xfInfoHeader.m_straComposer.get_size() > 0)
            {
               // add Performer
               staticdata.m_straTitleFormat.add("Performer: %0");
               xfihs.m_xfInfoHeader.m_straPerformer.get_format_string(str, ", ");
               staticdata.m_straaTitle.add_new();
               staticdata.m_straaTitle[1].add(str);

               // add Composer
               staticdata.m_straTitleFormat.add("Composer: %0");
               xfihs.m_xfInfoHeader.m_straComposer.get_format_string(str, ", ");
               staticdata.m_straaTitle.add_new();
               staticdata.m_straaTitle[1].add(str);


               //      staticdata.m_straPerformer = xfihs.m_xfInfoHeader.m_straPerformer;
               //    staticdata.m_straArtist = xfihs.m_xfInfoHeader.m_straPerformer;
            }

            staticdata.m_dwDefaultCodePage = 1252; // Latin (Default of All Default Code Pages)

            if(xfihs.m_xfaInfoHeaderLS.get_count())
            {
               staticdata.m_dwDefaultCodePage = ::music::xf::SymbolCharCodeToCodePage(xfihs.m_xfaInfoHeaderLS[0].m_strLanguage);
            }

            tracks.WorkSeekBegin();
            //tracks.WorkGetXFTokens(staticdata.m_dwDefaultCodePage, straa, position2a, iaaTokenLine, nullptr);
            tracks.WorkGetXFTokens((u32) -1, straa, position2a, iaaTokenLine, nullptr, false);

            Prepare(
            straa,
            position2a,
            -1,
            iaaTokenLine,
            data);

            tracks.WorkSeekBegin();
            tracks.WorkGetLongestXFLyrics(staticdata.m_strLyrics, false);
            tracks.WorkGetLongestXFLyrics(staticdata.m_strLyricsEx1, true);



         }


         void sequence::GetPositionLength(::duration &position)
         {
            position = m_tkLength;
         }

         void sequence::buffer::Initialize(i32 iSize, uptr dwUser)
         {
            m_storage.allocate(iSize);
            m_midihdr.lpData           = (char *) m_storage.get_data();
            m_midihdr.dwBufferLength   = (u32) m_storage.get_size();
            m_midihdr.dwUser           = dwUser;
            m_bPrepared                = false;

         }

         void sequence::buffer_array::Initialize(i32 iCount, i32 iSize, uptr dwUser)
         {
            set_size(iCount);

            i32 i;

            /*   for(i32 i = 0; i < this->get_size() - 1; i++)
            {
            this->element_at(i).SetNextMidiHdr(this->element_at(i + 1).GetMidiHdr());
            }

            if(this->get_size() > 1)
            {
            this->element_at(get_upper_bound()).SetNextMidiHdr(this->element_at(0).GetMidiHdr());
            }
            else
            {
            this->element_at(0).SetNextMidiHdr(nullptr);
            }*/

            for(i = 0; i < this->get_size(); i++)
            {
               this->element_at(i).Initialize(iSize, dwUser);
            }
         }

         void sequence::buffer_array::Reset()
         {
            for(i32 i = 0; i < this->get_size(); i++)
            {
               this->element_at(i).Reset();
            }
         }

         void sequence::buffer::Reset()
         {
            m_midihdr.dwBytesRecorded  = 0;
            m_midihdr.dwFlags          = 0;
         }

         bool sequence::buffer::IsPrepared()
         {
            return m_bPrepared;
         }


         void     sequence::buffer::midiOutPrepareHeader(HMIDIOUT hmidiout)
         {

            void     mmr = ::success;

            if(hmidiout == nullptr)
               return mmr;

            if(m_bPrepared)
               return mmr;

            mmr = ::midiOutPrepareHeader(hmidiout, &m_midihdr, sizeof(m_midihdr));

            if(mmr == ::success)
            {

               m_bPrepared = true;

            }

            return mmr;

         }


         void     sequence::buffer::midiOutUnprepareHeader(HMIDIOUT hmidiout)
         {

            void     mmr = ::success;

            if(hmidiout == nullptr)
               return mmr;

            if(!m_bPrepared)
               return mmr;

            mmr = ::midiOutUnprepareHeader(hmidiout, &m_midihdr, sizeof(m_midihdr));

            if(mmr == ::success)
            {

               m_bPrepared = false;

            }

            return mmr;

         }

         void     sequence::buffer_array::midiOutUnprepareHeader(HMIDIOUT hmidiout)
         {
            void     mmr = ::success;

            for (i32 i = 0; i < this->get_size(); i++)
            {
               void     mmrBuffer = this->element_at(i).midiOutUnprepareHeader(hmidiout);
               if(mmrBuffer != ::success)
               {
                  mmr = mmrBuffer;
               }
            }
            return mmr;
         }

         void     sequence::buffer_array::midiOutPrepareHeader(HMIDIOUT hmidiout)
         {
            void     estatus = ::success;
            for(i32 i = 0; i < this->get_size(); i++)
            {
               estatus = this->element_at(i).midiOutPrepareHeader(
                         hmidiout);
               if(estatus != ::success)
               {
                  for(; i >= 0; i--)
                  {
                     this->element_at(i).midiOutUnprepareHeader(hmidiout);
                  }
                  return estatus;
               }
            }
            return estatus;
         }

         void sequence::buffer::SetNextMidiHdr(LPMIDIHDR lpNext)
         {
            m_midihdr.lpNext = lpNext;
         }


         void     sequence::buffer::midiStreamOut(HMIDISTRM hmidiout)
         {
            ASSERT(hmidiout != nullptr);
            return ::midiStreamOut(hmidiout, &m_midihdr, sizeof(m_midihdr));
         }

         void     sequence::buffer_array::midiStreamOut(HMIDISTRM hmidiout)
         {
            void     estatus = ::success;
            for(i32 i = 0; i < this->get_size(); i++)
            {
               estatus = this->element_at(i).midiStreamOut(
                         hmidiout);
               if(estatus != ::success)
               {
                  //         for(; i >= 0; i--)
                  //       {
                  //        this->element_at(i).midiOutUnprepareHeader(hmidiout);
                  //   }
                  return estatus;
               }
            }
            return estatus;
         }





         void sequence::MuteAll(bool bMute, i32 iExcludeTrack)
         {
            bool bPlay = IsPlaying();
            ::duration ticks = 0;
            if(bPlay)
            {
               ticks = GetPositionTicks();
               Stop();
            }
            get_file()->MuteAll(bMute, iExcludeTrack);
            if(bPlay)
            {
               m_pthread->PrerollAndWait(ticks);
               Start();
            }
         }

         void sequence::MuteTrack(i32 iIndex, bool bMute)
         {
            bool bPlay = IsPlaying();
            ::duration ticks = 0;
            if(bPlay)
            {
               ticks = GetPositionTicks();
               Stop();
            }
            get_file()->MuteTrack(iIndex, bMute);
            if(bPlay)
            {
               m_pthread->PrerollAndWait(ticks);
               Start();
            }
         }

         ::duration sequence::GetQuarterNote()
         {
            return get_file()->m_pFileHeader->GetQuarterNoteTicks();
         }


         i32 sequence::GetDefaultCodePage()
         {
            return 1252;
         }


         bool sequence::IsSettingPosition()
         {
            return m_flags.has(::music::midi::sequence::e_flag_setting_position);
         }


         void sequence::SetSettingPositionFlag(bool bSet)
         {
            if(bSet)
               m_flags.add(::music::midi::sequence::e_flag_setting_position);
            else
               m_flags.erase(::music::midi::sequence::e_flag_setting_position);
         }

         u32 sequence::GetPreviousState()
         {
            return m_uiPreviousState;
         }

         u32 sequence::GetState()
         {
            return m_uiState;
         }

         ::music::midi::sequence::event * sequence::create_new_event(::music::midi::sequence::e_event eevent, LPMIDIHDR lpmidihdr)
         {

            ASSERT(this != nullptr);

            event * pevent          = new event();

            ptopic->m_atom        = eevent;
            ptopic->m_psequence     = this;
            ptopic->m_lpmh          = lpmidihdr;

            return pevent;

         }


      } // namespace mmsystem


   } // namespace midi


} // namespace music









