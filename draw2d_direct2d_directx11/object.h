#pragma once


#include "aura/graphics/draw2d/object.h"
#include "aura/graphics/draw2d/draw2d.h"
#include "direct2d_directx11/object.h"

namespace draw2d_direct2d_directx11
{


   class CLASS_DECL_DRAW2D_DIRECT2D_DIRECTX11 object :
      virtual public ::draw2d::object,
      virtual public ::direct2d_directx11::object
   {
   public:


      object();
      ~object() override;


      int get_object(int nCount, LPVOID lpObject) const;
      unsigned int GetObjectType() const;
      bool CreateStockObject(int nIndex);
      bool UnrealizeObject();
      bool operator==(const object& obj) const;
      bool operator!=(const object& obj) const;

      //void dump(dump_context & dumpcontext) const override;
      //void assert_ok() const override;


      ::draw2d_direct2d_directx11::draw2d * draw2d()
      {

         return (::draw2d_direct2d_directx11::draw2d * ) system()->draw2d()->m_pimpl;

      }


   };


} // namespace aura



