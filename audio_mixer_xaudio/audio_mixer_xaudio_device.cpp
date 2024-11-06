#include "framework.h"


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      device::device(::pointer<base_application>papp) :
         ::object(pobject),
         ::audio_mixer::device(pobject)
         //m_mixerdestinationa(pobject)
      {

         //m_mixerdestinationa.set_app(this);

         m_pmixer = nullptr;

         m_hMixer = nullptr;

      }


      device::~device()
      {

         close();

      }

      bool device::initialize(::audio_mixer::audio_mixer * pmixer)
      {

         m_pmixer = pmixer;

         return true;

      }

      void     device::open(unsigned int uiMixerId, unsigned int dwCallback, unsigned int dwInstance, unsigned int fdwOpen)
      {

         void                mmrc;
         HMIXER              hmx;
         MIXERCAPS           mxcaps;


         if (m_hMixer != nullptr)
         {

            void     mmrct = close();

            if(::success != mmrct)
            {

               message_box(nullptr, e_message_box_ok | e_message_box_icon_exclamation, "mixerClose() failed on hmx=%.04Xh, mmr=%u!", m_hMixer, mmrct);

            }

         }

         mmrc = mmsystem::translate(mixerGetDevCaps(uiMixerId, &mxcaps, sizeof(mxcaps)));

         if(::success != mmrc)
         {

            message_box(nullptr, e_message_box_ok | e_message_box_icon_exclamation, "mixerGetDevCaps() failed on uMxId=%u, mmr=%u!", uiMixerId, mmrc);

            return mmrc;

         }

         mmrc = mmsystem::translate(mixerOpen(&hmx, uiMixerId, dwCallback, dwInstance, fdwOpen));

         if(::success != mmrc)
         {

            message_box(nullptr, e_message_box_ok | e_message_box_icon_exclamation, "mixerOpen() failed on uMxId=%u, mmr=%u!", uiMixerId, mmrc);

            return mmrc;

         }



         m_hMixer = hmx;

         m_uiMixerID = uiMixerId;

         //    Appset_window_text(oswindow, "::audio_mixer::audio_mixer Device: %s", (char *)mxcaps.szPname);

         return mmrc;

      }


      void     device::initialize_capabilities()
      {

         void     mmrc;

         mmrc = mmsystem::translate(mixerGetDevCaps((unsigned int) m_hMixer, &m_mixercaps, sizeof(MIXERCAPS)));

         if(::success != mmrc)
         {

            message_box(nullptr, e_message_box_ok | e_message_box_icon_exclamation, "mixerGetDevCaps() failed on uMxId=%u, mmr=%u!", m_uiMixerID, mmrc);

            return mmrc;

         }

         return mmrc;

      }


      void     device::initialize_destinations()
      {

         ::pointer<::audio_mixer_mmsystem::destination>   lpDestination;

         m_mixerdestinationa.set_size_create(m_mixercaps.cDestinations);

         for (int i = 0; (unsigned int) i < m_mixercaps.cDestinations; i++)
         {
            lpDestination = m_mixerdestinationa(i);
            lpDestination->set_device(this);
            lpDestination->mixerGetLineInfo(0, i, MIXER_GETLINEINFOF_DESTINATION);
            //        ::audio_mixer::audio_mixer::get_component_name(lpmxl, lpDestination->m_strComponent);

            //        //
            //      //
            //    //
            //  wsprintf(ach, gszLineFormatList,
            //         (char *)szLineTypeDst,
            //       (MIXERLINE_LINEF_ACTIVE & mxl.fdwLine) ? '*' : ' ',
            //     (char *)szComponent,
            //   mxl.dwLineID,
            //   mxl.fdwLine,
            // mxl.cControls,
            // mxl.cConnections,
            //(char *)mxl.szName);

            //        if (0 != (APP_OPTF_DEBUGLOG & gfuAppOptions))
            //      {
            //        MixAppDebugLog(ach);
            //      MixAppDebugLog(gszCRLF);
            //  }

            //        nIndex = ListBox_AddString(ptlb->hlb, ach);
            //      ListBox_SetItemData(ptlb->hlb, nIndex, mxl.dwLineID);

         }


         //
         //
         //
         //    SetWindowRedraw(ptlb->hlb, true);
         return ::success;

      }

      void     device::get_destination(::audio_mixer::e_destination edestination, ::audio_mixer::destination **ppDestination)
      {

         unsigned int dwComponentType;

         switch(edestination)
         {
         case ::audio_mixer::destination_speakers:
            dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
            break;

         default:
            return error_failed;
         };


         if(m_mixerdestinationa.get_size() <= 0)
            initialize_destinations();

         if(m_mixerdestinationa.get_size() <= 0)
            return error_failed;

         for(int i = 0; i < m_mixerdestinationa.get_size(); i++)
         {

            ::pointer<::audio_mixer_mmsystem::destination>destination = m_mixerdestinationa(i);

            unsigned int dw = destination->m_mixerline.dwComponentType;

            if(dw == dwComponentType)
            {

               *ppDestination = m_mixerdestinationa(i);

               return ::success;

            }

         }

         return error_failed;

      }

      void device::map_controls()
      {

         m_mapIDToControl.erase_all();

         for(int i = 0; i < m_mixerdestinationa.get_size(); i++)
         {

            ::pointer<::audio_mixer_mmsystem::destination>destination = m_mixerdestinationa(i);

            MapLineControls(destination);

            ::audio_mixer::source_array & sourcea = destination->get_source_info();

            for(int j = 0; j < sourcea.get_size(); j++)
            {

               ::pointer<::audio_mixer::source>source = sourcea(j);

               MapLineControls(source);

            }

         }

      }


      void device::map_lines()
      {

         m_mapIDToLine.erase_all();

         for(int i = 0; i < m_mixerdestinationa.get_size(); i++)
         {

            ::pointer<::audio_mixer_mmsystem::destination>destination = m_mixerdestinationa(i);

            m_mapIDToLine.set_at(destination->get_mixer_line().dwLineID, destination);

            ::audio_mixer::source_array & sourcea = destination->get_source_info();

            for(int j = 0; j < sourcea.get_size(); j++)
            {

               ::pointer<::audio_mixer_mmsystem::source>source = sourcea(j);

               m_mapIDToLine.set_at(source->m_mixerline.dwLineID, source);

            }

         }

      }

      void device::OnMixerLineChange(unsigned int dwLineID)
      {
         ::audio_mixer::source * pSource;
         if(m_mapIDToLine.lookup(dwLineID, pSource))
         {
            pSource->OnMixerLineChange();
         }
      }

      void device::OnMixerControlChange(unsigned int dwControlID)
      {
         ::audio_mixer::control * pControl;
         if(m_mapIDToControl.lookup(dwControlID, pControl))
         {
            pControl->OnMixerControlChange();
         }
      }

      void device::MapLineControls(::audio_mixer::source * psource)
      {
         ::audio_mixer::control_array & controla = psource->get_control_array();
         for(int k = 0; k < controla.get_size(); k++)
         {
            ::pointer<::audio_mixer_mmsystem::control>control = controla(k);
            m_mapIDToControl.set_at(control->GetMixerControl().dwControlID, control);
            for(int l = 0; l < control->get_size(); l++)
            {
               ::pointer<::audio_mixer::user_control>pinteraction = control->operator ()(l);
               m_mapDlgItemIDToControl.set_at(pinteraction->_GetDlgCtrlID(), control);
            }
         }
      }

      void device::MapDlgCtrlIDToControls()
      {
         m_mapDlgItemIDToControl.erase_all();

         ::audio_mixer::destination_array & destinationa = m_mixerdestinationa;
         for(int i = 0; i < destinationa.get_size(); i++)
         {
            ::pointer<::audio_mixer_mmsystem::destination>destination = destinationa(i);
            MapDlgCtrlIDToLineControls(destination);
            ::audio_mixer::source_array & sourcea = destination->get_source_info();
            for(int j = 0; j < sourcea.get_size(); j++)
            {
               ::audio_mixer::source & source = sourcea[j];
               MapDlgCtrlIDToLineControls(&source);
            }
         }

      }

      void device::MapDlgCtrlIDToLineControls(::audio_mixer::source * psource)
      {
         ::audio_mixer::control_array & controla = psource->get_control_array();
         for(int k = 0; k < controla.get_size(); k++)
         {
            ::pointer<::audio_mixer::control>control = controla(k);
            for(int l = 0; l < control->get_size(); l++)
            {
               ::pointer<::audio_mixer::user_control>pinteraction = control->operator()(l);
               m_mapDlgItemIDToControl.set_at(pinteraction->_GetDlgCtrlID(), control);
            }
         }
      }


      void     device::close()
      {

         void     mmrc = ::success;

         if(m_hMixer != nullptr)
         {

            mmrc = mmsystem::translate(mixerClose(m_hMixer));

            m_mixerdestinationa.erase_all();

            m_mapIDToControl.erase_all();

            m_mapDlgItemIDToControl.erase_all();

            m_mapIDToLine.erase_all();

            m_hMixer = nullptr;

         }

         return mmrc;

      }

      bool device::OnCommand(wparam wparam, lparam lparam)
      {

         unsigned int uiID = LOWORD(wparam);

         ::audio_mixer::control * pinteraction;

         if(m_mapDlgItemIDToControl.lookup(uiID, pinteraction)
               && pinteraction->OnCommand(wparam, lparam))
            return true;

         return false;

      }


      string device::get_product_name()
      {

         return m_mixercaps.szPname;

      }


      ::audio_mixer::audio_mixer * device::get_mixer()
      {
         return m_pmixer;
      }


   } // namespace audio_mixer_mmsystem


} // namespace multimedia


