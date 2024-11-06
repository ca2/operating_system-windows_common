#include "framework.h"


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {

      destination::destination(::pointer<base_application>papp) :
         ::object(pobject),
         ::audio_mixer::source(pobject),
         ::audio_mixer_mmsystem::source(pobject),
         ::audio_mixer::destination(pobject)
      {

         m_mixersourcea.set_app(pobject);
         m_pdevice = nullptr;
         m_pdestination = this;

      }

      destination::destination(const destination & destination) :
         ::matter(((class destination &) destination).get_application()),
         ::audio_mixer::source(((class destination &)destination).get_application()),
         ::audio_mixer_mmsystem::source(((class destination &)destination).get_application()),
         ::audio_mixer::destination(((class destination &)destination).get_application())
      {

         m_mixersourcea.set_app(((class destination &)destination).get_application());

         operator = (destination);

      }

      destination::~destination()
      {
         //    Carray<::audio_mixer::source, ::audio_mixer::source &>::~Carray<::audio_mixer::source, ::audio_mixer::source &>();
         //CMixerLine::~CMixerLine();
      }

      void destination::operator delete(void *it)
      {
         ((destination *) it)->~destination();
         free(it);

      }


      void     destination::initialize_source_info()
      {

         ::pointer<::audio_mixer_mmsystem::source>    lpSource;

         int iConnections = (int) m_mixerline.cConnections;

         m_mixersourcea.set_size_create(iConnections);

         for (int i = 0; i < iConnections; i++)
         {

            lpSource = m_mixersourcea(i);

            lpSource->SetDestination(this);

            lpSource->mixerGetLineInfo(i, this);

         }

         return ::success;

      }


      void     destination::initialize_all_controls()
      {

         GetLineControls();

         ::audio_mixer::source_array & sourcea = m_mixersourcea;

         for(int i = 0; i < sourcea.get_size(); i++)
         {

            sourcea[i].GetLineControls();

         }

         return ::success;

      }

      void destination::update_all_controls()
      {

         ::audio_mixer::source::update_all_controls();

         ::audio_mixer::source_array & sourcea = m_mixersourcea;

         for(int i = 0; i < sourcea.get_size(); i++)
         {

            sourcea[i].update_all_controls();

         }

      }


      destination & destination::operator = (const destination & destination)
      {

         m_pdevice = destination.m_pdevice;

         return *this;

      }


      unsigned int destination::get_component_type()
      {

         return m_mixerline.dwComponentType;

      }


      ::audio_mixer::device * destination::get_device()
      {

         return m_pdevice;

      }


      void destination::set_device(::audio_mixer::device * pdevice)
      {

         m_pdevice = pdevice;

      }


      ::audio_mixer::source_array & destination::get_source_info()
      {

         return m_mixersourcea;

      }

      unsigned int destination::get_mixer_line_id()
      {
         return m_mixerline.dwLineID;
      }

   } // namespace audio_mixer_mmsystem


} // namespace multimedia







