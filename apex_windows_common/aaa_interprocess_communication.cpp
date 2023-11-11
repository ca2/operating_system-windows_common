// created by Camilo 2021-01-31 05:36 BRT <3CamiloSasukeThomasBorregaardSoerensen
#include "framework.h"
#include "apex/operating_system.h"
#include "apex/platform/app_core.h"
#include "inteprocess_channel.h"
#include "apex/platform/launcher.h"
#include "launcher.h"


namespace windows
{


   ATOM rx_register_class(HINSTANCE hInstance);
   LRESULT CALLBACK s_rx_message_queue_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);


   interprocess_communication_base::interprocess_communication_base()
   {

      set_hwnd(nullptr);

   }


   interprocess_communication_base::~interprocess_communication_base()
   {

      if (get_hwnd() != nullptr)
      {

         ::DestroyWindow((HWND)get_hwnd());

         set_hwnd(nullptr);

      }

   }


   interprocess_caller::interprocess_caller()
   {

   }


   interprocess_caller::~interprocess_caller()
   {

   }


   bool interprocess_caller::open(const ::string & pszKey, ::launcher * plauncher)
   {

      if (get_hwnd() != nullptr)
      {

         close();

      }

      int jCount = 23;
      int iCount;

      if (plauncher != nullptr)
      {

         iCount = plauncher->m_iStart + 1;

      }
      else
      {

         iCount = 2;

      }

      set_hwnd(nullptr);

      for (int i = 0; i < iCount; i++)
      {
         for (int j = 0; j < jCount; j++)
         {
            set_hwnd(::FindWindowW(nullptr, wstring(pszKey)));
            if (get_hwnd() != nullptr)
               break;
            //         get_hwnd() = FindDesktopWindow(pszKey);
            //       if(get_hwnd() != nullptr)
            //        break;
            if (i <= 0)
            {
               break;
            }
            int k = 40;
            while (k > 0)
            {
               if (!task_get_run())
               {
                  return false;
               }
               sleep(25_ms);
               k--;
            }
         }
         if (get_hwnd() != nullptr)
            break;
         if (plauncher != nullptr)
         {
            if (plauncher->m_iStart <= 0)
               return false;
            plauncher->start();
            plauncher->m_iStart--;
         }
      }
      m_strBaseChannel = pszKey;
      return true;

   }

   bool interprocess_caller::close()
   {

      if (get_hwnd() == nullptr)
         return true;

      set_hwnd(nullptr);

      m_strBaseChannel = "";

      return true;

   }


   bool interprocess_caller::send(const ::string & pszMessage, duration durationTimeout)
   {

      if (!is_tx_ok())
         return false;

      COPYDATASTRUCT cds;

      cds.dwData = I32_MINIMUM;
      cds.cbData = (unsigned int)strlen(pszMessage);
      cds.lpData = (void *)pszMessage;


      if (durationTimeout.is_pos_infinity())
      {

         SendMessage((HWND)get_hwnd(), WM_COPYDATA, (WPARAM)0, (LPARAM)&cds);

      }
      else
      {

         DWORD_PTR dwptr;

         if (!::SendMessageTimeout((HWND)get_hwnd(), WM_COPYDATA, (WPARAM)0, (LPARAM)&cds, SMTO_ABORTIFHUNG, (::u32)(durationTimeout.u32_millis()), &dwptr))
         {

            return false;

         }

         unsigned int dwError = ::GetLastError();

         if (dwError == ERROR_TIMEOUT)
            return false;

      }

      return true;
   }

   bool interprocess_caller::send(int message, void * pdata, int len, duration durationTimeout)
   {

      if (message == I32_MINIMUM)
         return false;

      if (!is_tx_ok())
         return false;

      COPYDATASTRUCT cds;

      cds.dwData = (unsigned int)message;
      cds.cbData = (unsigned int)maximum(0, len);
      cds.lpData = (void *)pdata;


      if (durationTimeout.is_pos_infinity())
      {

         if (message >= WM_APP)
         {

            SendMessage((HWND)get_hwnd(), message, 0, 0);

         }
         else
         {

            SendMessage((HWND)get_hwnd(), WM_COPYDATA, (WPARAM)0, (LPARAM)&cds);

         }

      }
      else
      {

         DWORD_PTR dwptr;

         if (!::SendMessageTimeout((HWND)get_hwnd(), WM_COPYDATA, (WPARAM)0, (LPARAM)&cds, SMTO_BLOCK, (::u32)(durationTimeout.u32_millis()), &dwptr))
         {

            return false;

         }

         unsigned int dwError = ::GetLastError();

         if (dwError == ERROR_TIMEOUT)
            return false;

      }

      return true;

   }



   bool interprocess_caller::is_tx_ok()
   {

      return ::IsWindow((HWND)get_hwnd()) != false;

   }




   interprocess_handler::interprocess_handler()
   {

      m_preceiver = nullptr;

   }


   interprocess_handler::~interprocess_handler()
   {

   }


   bool interprocess_handler::create(const ::string & pszKey)
   {


      /*    if(g_pfnChangeWindowMessageFilter != nullptr)
          {
             g_pfnChangeWindowMessageFilter(WM_COPYDATA,MSGFLT_ADD);
          }*/

          //HINSTANCE hinstance = ::GetModuleHandleA("apex.dll");

          //if(hinstance == nullptr)
          //{

          //   hinstance = ::GetModuleHandleA(nullptr);

          //}

          //auto pappcore = psystem->m_pappcore;

          //auto pmaindata = pappcore->m_pmaindata;

      ::pointer<::apex::system>psystem = system();

      HINSTANCE hinstance = (HINSTANCE)psystem->m_hinstance;

      ATOM atom = rx_register_class(hinstance);

      wstring wstrKey(pszKey);

      set_hwnd(::CreateWindowExW(0, L"small_ipc_rx_::color::e_channel_message_queue_class", wstrKey, 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, hinstance, nullptr));

      if (get_hwnd() == nullptr)
      {
         unsigned int dwLastError = ::GetLastError();
         return false;
      }

      if (!ChangeWindowMessageFilterEx((HWND)get_hwnd(), WM_COPYDATA, MSGFLT_ADD, NULL))
      {

         informationf("Failed to change WM_COPYDATA message filter");

      }

      SetTimer((HWND)get_hwnd(), 888888, 84, nullptr);

      SetWindowLongPtr((HWND)get_hwnd(), GWLP_USERDATA, (LONG_PTR)this);

      //m_strWindowProcModule = pszWindowProcModule;

      return true;

   }


   bool interprocess_handler::destroy()
   {

      if (get_hwnd() != nullptr)
      {
         
         ::DestroyWindow((HWND)get_hwnd());
         
         set_hwnd(nullptr);

      }

      return true;

   }



   void * interprocess_handler::on_interprocess_receive(::inteprocess::handler * prx, const ::string & pszMessage)
   {

      string strMessage(pszMessage);

      if (strMessage.case_insensitive_begins("synch_"))
      {

         if (m_preceiver != nullptr)
         {

            m_preceiver->on_interprocess_receive(prx, strMessage);

         }

      }
      else
      {

         get_application()->fork([=]()
            {

               if (m_preceiver != nullptr)
               {

                  m_preceiver->on_interprocess_receive(prx, strMessage);

               }

            });

      }

      // ODOW - on date of writing : return ignored by this windows implementation

      return nullptr;

   }


   void * interprocess_handler::on_interprocess_receive(::inteprocess::handler * prx, int message, void * pdata, memsize len)
   {

      if (m_preceiver != nullptr)
      {

         m_preceiver->on_interprocess_receive(prx, message, pdata, len);

      }

      // ODOW - on date of writing : return ignored by this windows implementation

      return nullptr;

   }


   void * interprocess_handler::on_interprocess_post(::inteprocess::handler * prx, long long int a, long long int b)
   {

      if (m_preceiver != nullptr)
      {

         m_preceiver->on_interprocess_post(prx, a, b);

      }

      // ODOW - on date of writing : return ignored by this windows implementation

      return nullptr;

   }


   LRESULT CALLBACK s_rx_message_queue_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
   {

      int iRet = 0;

      interprocess_handler * pchannel = (interprocess_handler *)GetWindowLongPtr((HWND)hwnd, GWLP_USERDATA);

      if (pchannel == nullptr)
      {

         return ::DefWindowProcW((HWND)hwnd, message, wparam, lparam);

      }
      else
      {

         return pchannel->message_queue_proc((enum_message)message, wparam, lparam);

      }

   }


   ATOM rx_register_class(HINSTANCE hInstance)
   {
      WNDCLASSEXW wcex = {};

      wcex.cbSize = sizeof(WNDCLASSEX);

      wcex.style = 0;
      wcex.lpfnWndProc = &s_rx_message_queue_proc;

      wcex.cbClsExtra = 0;
      wcex.cbWndExtra = 40;
      wcex.hInstance = hInstance;
      wcex.hIcon = nullptr;
      //wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
      wcex.hCursor = nullptr;
      wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
      wcex.lpszMenuName = nullptr;

      wcex.lpszClassName = L"small_ipc_rx_::color::e_channel_message_queue_class";

      wcex.hIconSm = nullptr;

      return RegisterClassExW(&wcex);
   }


   LRESULT interprocess_handler::message_queue_proc(UINT message, WPARAM wparam, LPARAM lparam)
   {

      if (message == WM_USER + 100)
      {

         on_interprocess_post(this, wparam, lparam);

      }
      else if (message == WM_COPYDATA)
      {

         COPYDATASTRUCT * pcds = (COPYDATASTRUCT *)lparam;


         if (pcds == nullptr)
         {

            return 0;

         }
         else if (pcds->dwData == I32_MINIMUM)
         {

            string strMessage((const ::string &)pcds->lpData, pcds->cbData);

            on_interprocess_receive(this, strMessage.c_str());

         }
         else
         {

            on_interprocess_receive(this, (int)pcds->dwData, pcds->lpData, pcds->cbData);

         }

      }
      else if (message >= WM_APP)
      {

         on_interprocess_receive(this, message, (void *)wparam, lparam);

      }
      else
      {

         return ::DefWindowProcW((HWND) get_hwnd(), message, wparam, lparam);

      }

      return 0;

   }


   bool interprocess_handler::on_idle()
   {

      return false;

   }


   bool interprocess_handler::is_rx_ok()
   {

      return ::IsWindow((HWND) get_hwnd()) != false;

   }


   //inteprocess_channel::inteprocess_channel()
   //{

   //   m_durationTimeout = (5000) * 11;

   //}


   //inteprocess_channel::~inteprocess_channel()
   //{


   //}


   //bool inteprocess_channel::open_ab(const ::string & pszKey, const ::string & pszModule, launcher * plauncher)
   //{

   //   m_strChannel = pszKey;

   //   m_prx->m_preceiver = this;

   //   string strChannelRx = m_strChannel + "-a";
   //   string strChannelTx = m_strChannel + "-b";

   //   if (!::IsWindow((HWND) m_prx->get_os_data()))
   //   {

   //      if (!m_prx->create(strChannelRx.c_str()))
   //      {

   //         return false;

   //      }

   //   }

   //   if (!m_ptx->open(strChannelTx.c_str(), plauncher))
   //   {

   //      return false;

   //   }

   //   return true;

   //}


   //bool inteprocess_channel::open_ba(const ::string & pszKey, const ::string & pszModule, launcher * plauncher)
   //{

   //   m_strChannel = pszKey;

   //   m_prx->m_preceiver = this;

   //   string strChannelRx = m_strChannel + "-b";
   //   string strChannelTx = m_strChannel + "-a";


   //   if (!::IsWindow((HWND) m_prx->get_os_data()))
   //   {

   //      if (!m_prx->create(strChannelRx.c_str()))
   //      {

   //         return false;

   //      }

   //   }

   //   if (!m_ptx->open(strChannelTx.c_str(), plauncher))
   //   {

   //      return false;

   //   }

   //   return true;

   //}


   //bool inteprocess_channel::is_rx_tx_ok()
   //{

   //   return m_prx->is_rx_ok() && m_ptx->is_tx_ok();

   //}


   //void inteprocess_channel::restart_apex_ipc()
   //{

   //}


   //bool inteprocess_channel::close()
   //{

   //   return false;

   //}

} // namespace windows



