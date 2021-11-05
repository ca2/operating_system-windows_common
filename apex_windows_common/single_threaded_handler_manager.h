#pragma once


class CLASS_DECL_APEX_WINDOWS_COMMON single_threaded_handler_manager:
   virtual public handler_manager
{
public:


   single_threaded_handler_manager();
   ~single_threaded_handler_manager() override;


   void on_start_loop();


};



