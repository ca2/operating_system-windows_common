#pragma once


#include "aura/graphics/draw2d/object.h"
#include "aura/graphics/draw2d/draw2d.h"
#include "directx11/object.h"

namespace draw2d_directx11
{


   class CLASS_DECL_DRAW2D_DIRECTX11 object :
      virtual public ::draw2d::object,
      virtual public ::directx11::object
   {
   public:


      ::pointer < ::draw2d_directx11::graphics > m_pgraphics;

      object();
      ~object() override;


      void initialize_draw2d_directx11_object(::draw2d_directx11::graphics* pgraphics);


      ::i32 get_object(::i32 nCount, LPVOID lpObject) const;
      ::u32 GetObjectType() const;
      bool CreateStockObject(::i32 nIndex);
      bool UnrealizeObject();
      bool operator==(const object& obj) const;
      bool operator!=(const object& obj) const;

      //void dump(dump_context & dumpcontext) const override;
      //void assert_ok() const override;


      ::draw2d_directx11::draw2d * draw2d()
      {

         return (::draw2d_directx11::draw2d * ) system()->draw2d()->m_pimpl;

      }


      void defer_update(::draw2d::graphics* pgraphics, ::i8 i) const override;


   };


} // namespace aura



