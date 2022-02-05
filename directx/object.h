#pragma once


namespace directx
{


   class CLASS_DECL_DIRECTX object :
      virtual public ::draw2d::object
   {
   public:


      object();
      ~object() override;


      int get_object(int nCount, LPVOID lpObject) const;
      ::u32 GetObjectType() const;
      bool CreateStockObject(int nIndex);
      bool UnrealizeObject();
      bool operator==(const object& obj) const;
      bool operator!=(const object& obj) const;

      void dump(dump_context & dumpcontext) const override;
      void assert_ok() const override;


      //::directx::draw2d * draw2d()
      //{

      //   return (::directx::draw2d * ) pdraw2d->layer(LAYERED_IMPL);

      //}


   };


} // namespace directx



