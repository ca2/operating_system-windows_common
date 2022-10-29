#pragma once


#include "aura/graphics/image/context_image.h"


namespace imaging_wic
{


   class CLASS_DECL_IMAGING_WIC context_image :
      virtual public ::context_image
   {
   public:


      //__creatable_from_base(context_image, ::context_image);


      ::pointer<handler_manager>       m_pmanagerImageLoad;


      context_image();
      ~context_image() override;


      virtual void initialize(::particle * pparticle) override;


      //virtual void _load_image(::context_image * pcontextimage, ::image * pimageParam, const ::payload & payloadFile, bool bSync, bool bCreateHelperMaps) override;
      void _load_image(image* pimage, const ::payload& payloadFile, const ::image::load_options & loadoptions = ::image::load_options()) override;

      void _load_image(::image * pimage, ::pointer<image_frame_array>& pframea, ::memory & memory) override;
      void save_image(memory & memory, const ::image * pimage, const ::save_image * psaveimage) override;


      //void _load_icon(::draw2d::icon * picon, const ::payload & payloadFile);


      //virtual void _load_image(::image* pimage, const ::payload& payloadFile, bool bSync, bool bCreateHelperMaps);


      //virtual void _load_image(::image* pimage, ::pointer<image_frame_array> pframea, ::memory_pointer pmemory);
      //virtual void save_image(memory& memory, const ::image* pimage, ::save_image* psaveimage);


      //virtual void load_cursor(::draw2d::cursor* pcursor, ::file::path path, bool bSync, bool bCache = true) override;

      //virtual void load_svg(::image* pimage, ::memory_pointer pmemory);


#ifdef _UWP
      virtual bool _desk_to_image(::image* pimage);
      virtual bool _desk_has_image();
      virtual bool _image_to_desk(const ::image* pimage);
#endif


      virtual bool _save_image(::file::file* pfile, const ::image* pimage, const ::save_image* psaveimage);

      void _os_load_image(::image * pimage, memory & memory) override;


   };


} // namespace imaging_wic



