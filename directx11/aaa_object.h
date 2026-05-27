#pragma once


namespace draw2d_directx11
{


   class CLASS_DECL_DRAW2D_DIRECTX11 object :
      virtual public ::draw2d::object
   {
   public:


      object();
      virtual ~object();


      ::i32 get_object(::i32 nCount, LPVOID lpObject) const;
      ::u32 GetObjectType() const;
      bool CreateStockObject(::i32 nIndex);
      bool UnrealizeObject();
      bool operator==(const object& obj) const;
      bool operator!=(const object& obj) const;

      void dump(dump_context & dumpcontext) const override;
      void assert_ok() const override;


      ::draw2d_directx11::draw2d * draw2d()
      {

         return (::draw2d_directx11::draw2d * ) pdraw2d->layer(LAYERED_IMPL);

      }


   };


} // namespace aura



