#pragma once


namespace draw2d_direct2d
{


   class CLASS_DECL_DRAW2D_DIRECT2D object :
      virtual public ::draw2d::object
   {
   public:


      object();
      virtual ~object();


      int get_object(int nCount, LPVOID lpObject) const;
      unsigned int GetObjectType() const;
      bool CreateStockObject(int nIndex);
      bool UnrealizeObject();
      bool operator==(const object& obj) const;
      bool operator!=(const object& obj) const;

      void dump(dump_context & dumpcontext) const override;
      void assert_ok() const override;


      ::draw2d_direct2d::draw2d * draw2d()
      {

         return (::draw2d_direct2d::draw2d * ) pdraw2d->layer(LAYERED_IMPL);

      }


   };


} // namespace aura



