// Created by camilo on 2025-06-10 18:23 <3ThomasBorregaardSørensen!!
#pragma once


#include "directx11/swap_chain.h"


namespace gpu_directx11
{


   class CLASS_DECL_GPU_DIRECTX11 swap_chain :
      virtual public ::directx11::swap_chain
   {
   public:



      swap_chain();
      ~swap_chain() override;


      void _update_swap_chain() override;


   };


} // namespace gpu_directx11



