// Created by camilo on 2021-01-21 05:05 PM <3ThomasBorregaardSorensen
#pragma once


#include "acme/user/micro/window_implementation.h"
#include "acme/operating_system/windows/window.h"
#undef USUAL_OPERATING_SYSTEM_SUPPRESSIONS
#include "acme/_operating_system.h"


namespace universal_windows
{


   namespace nano
   {


      namespace user
      {


         class window :
            virtual public ::micro::window_implementation,
            virtual public ::windows::window
         {
         public:


            //CreatableFromBase(window, ::micro::window_implementation);
            //bool m_bSizeMoveMode;
            //HWND m_hwnd;
            //HMENU m_hmenuSystem;
            //bool m_bDestroy;
      //      HFONT m_hfont;
      //      color32_t m_crText;
      //      color32_t m_crFocus;
      //      color32_t m_crWindow;
      //      string m_strTitle;
      //      bool m_bNcActive;

      //      int_rectangle m_rectangle;
      //      int_rectangle m_rectangleX;
      //
      //      pointer_array < ::micro::child > m_childa;
      //      ::atom m_atomLeftButtonDown;
      //      ::atom m_atomLeftButtonUp;
      //      //::atom                             m_atomResult;
      //      ::pointer<::micro::child>m_pchildFocus;
            ::task_pointer       m_ptask;

            window();

            ~window() override;

            void create() override;

            void display() override;

            virtual void _draw(HDC hdc);

            //void on_draw(::nano::graphics::device * pnanodevice) override;

            void on_char(int iChar) override;

            //bool is_active() override;

            //virtual void draw_children(::nano::graphics::device * pnanodevice);

            void delete_drawing_objects() override;

            bool get_dark_mode() override;

            void create_drawing_objects() override;

            void update_drawing_objects() override;

            //::micro::child * on_hit_test(const ::int_point & point, ::user::e_zorder ezorder) override;

            void add_child(::micro::child* pchild) override;

            ::payload get_result() override;

            void on_mouse_move(::user::mouse* pmouse) override;

            void on_left_button_down(::user::mouse* pmouse) override;

            void on_left_button_up(::user::mouse* pmouse) override;

            void on_click(const ::payload& payload, ::user::mouse* pmouse) override;

            void on_right_button_down(::user::mouse* pmouse) override;

            void on_right_button_up(::user::mouse* pmouse) override;

            void on_right_click(const ::payload& payload, ::user::mouse* pmouse) override;

            virtual LRESULT window_procedure(UINT message, WPARAM wparam, LPARAM lparam);

            void move_to(const ::int_point& point) override;

            //void _destroy_window();

            void destroy() override;

            void redraw() override;


            ::int_point try_absolute_mouse_position(const ::int_point& point) override;



            void get_client_rectangle(::int_rectangle& rectangle) override;

            void get_window_rectangle(::int_rectangle& rectangle) override;

            void set_capture() override;

            bool has_capture() const override;

            void release_capture() override;

            void set_cursor(enum_cursor ecursor) override;

            static bool _is_light_theme();


            ::int_size get_main_screen_size() override;


            void user_post(const ::procedure& procedure) override;


            void implementation_message_loop_step() override;

            void defer_show_system_menu(const ::int_point & pointAbsolute) override;

         };


         CLASS_DECL_ACME void process_messages(bool bWait = false);


      } // namespace user

      
   } // namespace nano


} // namespace universal_windows







