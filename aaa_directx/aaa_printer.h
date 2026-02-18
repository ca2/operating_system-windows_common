#pragma once


#if 0


namespace draw2d_directx
{


   class CLASS_DECL_DRAW2D_DIRECTX printer :
      virtual public ::aura::printer
   {
   public:


      class CLASS_DECL_DRAW2D_DIRECTX document_properties :
         virtual public ::object
      {
      public:


         DEVMODE *      m_pdevmode;
         HDC            m_hdc;


         document_properties(::particle * pparticle);
         virtual ~document_properties();


         virtual bool initialize(::draw2d_directx::printer * pprinter, DEVMODE * pdevmode = nullptr);
         virtual bool close();
         virtual ::draw2d::graphics * create_graphics();

      };


      HANDLE                  m_hPrinter;
      document_properties     m_documentproperties;


      printer();
      virtual ~printer();


      virtual bool open(const ::scoped_string & scopedstrDeviceName);
      virtual ::draw2d::graphics * create_graphics();
      virtual bool is_opened();
      virtual bool close();



   };



} // namespace user



#endif




