#pragma once


#include "aura/graphics/draw2d/object.h"
#include "aura/graphics/draw2d/draw2d.h"
#include "direct2d/object.h"

namespace draw2d_direct2d
{


   class CLASS_DECL_DRAW2D_DIRECT2D object :
      virtual public ::draw2d::object,
      virtual public ::direct2d::object
   {
   public:


      ::pointer < ::draw2d_direct2d::graphics > m_pgraphics;

      object();
      object(object && object);
      ~object() override;


      void initialize_draw2d_direct2d_object(::draw2d_direct2d::graphics* pdraw2dgraphics);


      ::i32 get_object(::i32 nCount, LPVOID lpObject) const;
      ::u32 GetObjectType() const;
      bool CreateStockObject(::i32 nIndex);
      bool UnrealizeObject();
      bool operator==(const object& obj) const;
      bool operator!=(const object& obj) const;

      //void dump(dump_context & dumpcontext) const override;
      //void assert_ok() const override;


      

      ::draw2d_direct2d::draw2d * draw2d()
      {

         return (::draw2d_direct2d::draw2d * ) system()->draw2d()->m_pimpl;

      }


      void defer_update(::draw2d::graphics * pdraw2dgraphics) override;


   };


} // namespace aura



