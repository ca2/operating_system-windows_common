// Created by camilo on 2021-08-10 13:49 <3ThomasBorregaardSørensen!!
#include "framework.h"
#include "apex/parallelization/handler_manager.h"
#include "single_threaded_handler_manager.h"


single_threaded_handler_manager::single_threaded_handler_manager()
{


}


single_threaded_handler_manager::~single_threaded_handler_manager()
{


}


void single_threaded_handler_manager::on_start_loop()
{

   defer_co_initialize_ex(false, true);

}



